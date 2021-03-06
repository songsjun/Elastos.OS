//=========================================================================
// Copyright (C) 2012 The Elastos Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=========================================================================

#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Media.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/media/CRingtoneManager.h"
#include "elastos/droid/net/Uri.h"
#include "elastos/droid/preference/RingtonePreference.h"
#include "elastos/droid/text/TextUtils.h"
#include "elastos/droid/R.h"

using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IFragment;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Media::IRingtoneManager;
using Elastos::Droid::Media::CRingtoneManager;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Text::TextUtils;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Preference {

CAR_INTERFACE_IMPL(RingtonePreference::InnerListener, Object, IPreferenceManagerOnActivityResultListener)

RingtonePreference::InnerListener::InnerListener(
    /* [in] */ RingtonePreference* host)
    : mHost(host)
{}

ECode RingtonePreference::InnerListener::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data,
    /* [out] */ Boolean* result)
{
    return mHost->OnActivityResult(requestCode, resultCode, data, result);
}

const String RingtonePreference::TAG("RingtonePreference");

CAR_INTERFACE_IMPL(RingtonePreference, Preference, IRingtonePreference)

RingtonePreference::RingtonePreference()
    : mRingtoneType(0)
    , mShowDefault(FALSE)
    , mShowSilent(FALSE)
    , mDialogStyle(0)
    , mRequestCode(0)
    , mSubscriptionID(0)
{}

ECode RingtonePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    FAIL_RETURN(Preference::constructor(context, attrs, defStyleAttr, defStyleRes));

    AutoPtr<ArrayOf<Int32> > arrayAttrs = TO_ATTRS_ARRAYOF(R::styleable::RingtonePreference);
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, arrayAttrs, defStyleAttr, defStyleRes, (ITypedArray**)&a);
    a->GetInt32(R::styleable::RingtonePreference_ringtoneType, IRingtoneManager::TYPE_RINGTONE, &mRingtoneType);
    a->GetBoolean(R::styleable::RingtonePreference_showDefault, TRUE, &mShowDefault);
    a->GetBoolean(R::styleable::RingtonePreference_showSilent, TRUE, &mShowSilent);
    a->GetResourceId(R::styleable::RingtonePreference_dialogStyle, 0, &mDialogStyle);
    a->Recycle();
    return NOERROR;
}

ECode RingtonePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode RingtonePreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::ringtonePreferenceStyle);
}

ECode RingtonePreference::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode RingtonePreference::GetRingtoneType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type)
    *type = mRingtoneType;
    return NOERROR;
}

ECode RingtonePreference::SetRingtoneType(
    /* [in] */ Int32 type)
{
    mRingtoneType = type;
    return NOERROR;
}

ECode RingtonePreference::GetSubId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSubscriptionID;
    return NOERROR;
}

ECode RingtonePreference::SetSubId(
    /* [in] */ Int32 subId)
{
    mSubscriptionID = subId;
    return NOERROR;
}

ECode RingtonePreference::GetShowDefault(
    /* [out] */ Boolean* showDefault)
{
    VALIDATE_NOT_NULL(showDefault)
    *showDefault = mShowDefault;
    return NOERROR;
}

ECode RingtonePreference::SetShowDefault(
    /* [in] */ Boolean showDefault)
{
    mShowDefault = showDefault;
    return NOERROR;
}

ECode RingtonePreference::GetShowSilent(
    /* [out] */ Boolean* showSilent)
{
    VALIDATE_NOT_NULL(showSilent)
    *showSilent = mShowSilent;
    return NOERROR;
}

ECode RingtonePreference::SetShowSilent(
    /* [in] */ Boolean showSilent)
{
    mShowSilent = showSilent;
    return NOERROR;
}

ECode RingtonePreference::GetDialogStyle(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mDialogStyle;
    return NOERROR;
}

ECode RingtonePreference::SetDialogStyle(
    /* [in] */ Int32 dialogStyle)
{
    mDialogStyle = dialogStyle;
    return NOERROR;
}

ECode RingtonePreference::OnClick()
{
    AutoPtr<IIntent> intent;
    CIntent::New(IRingtoneManager::ACTION_RINGTONE_PICKER, (IIntent**)&intent);
    OnPrepareRingtonePickerIntent(intent);
    AutoPtr<IPreferenceManager> preferencemanager;
    GetPreferenceManager((IPreferenceManager**)&preferencemanager);
    AutoPtr<IPreferenceFragment> owningFragment;
    preferencemanager->GetFragment((IPreferenceFragment**)&owningFragment);
    if (owningFragment != NULL) {
        AutoPtr<IFragment> fragment = IFragment::Probe(owningFragment);
        fragment->StartActivityForResult(intent, mRequestCode);
    }
    else {
        AutoPtr<IActivity> activity;
        preferencemanager->GetActivity((IActivity**)&activity);
        activity->StartActivityForResult(intent, mRequestCode);
    }

    return NOERROR;
}

ECode RingtonePreference::OnPrepareRingtonePickerIntent(
    /* [in] */ IIntent* ringtonePickerIntent)
{
    AutoPtr<IUri> uri;
    OnRestoreRingtone((IUri**)&uri);
    ringtonePickerIntent->PutExtra(IRingtoneManager::EXTRA_RINGTONE_EXISTING_URI, IParcelable::Probe(uri));
    ringtonePickerIntent->PutBooleanExtra(IRingtoneManager::EXTRA_RINGTONE_SHOW_DEFAULT, mShowDefault);
    if (mShowDefault) {
        Int32 ringtoneType = 0;
        GetRingtoneType(&ringtoneType);
        if (ringtoneType == IRingtoneManager::TYPE_RINGTONE) {
            Int32 subId = 0;
            GetSubId(&subId);
            AutoPtr<IUri> defaultRingtoneUri;
            CRingtoneManager::GetDefaultRingtoneUriBySubId(subId, (IUri**)&defaultRingtoneUri);
            ringtonePickerIntent->PutExtra(IRingtoneManager::EXTRA_RINGTONE_DEFAULT_URI, IParcelable::Probe(defaultRingtoneUri));
        }
        else {
            Int32 ringtoneType = 0;
            GetRingtoneType(&ringtoneType);
            AutoPtr<IUri> defaultUri;
            CRingtoneManager::GetDefaultUri(ringtoneType, (IUri**)&defaultUri);
            ringtonePickerIntent->PutExtra(IRingtoneManager::EXTRA_RINGTONE_DEFAULT_URI, IParcelable::Probe(defaultUri));
        }
    }
    if (mDialogStyle != 0) {
        ringtonePickerIntent->PutExtra(IRingtoneManager::EXTRA_RINGTONE_DIALOG_THEME, mDialogStyle);
    }

    ringtonePickerIntent->PutBooleanExtra(IRingtoneManager::EXTRA_RINGTONE_SHOW_SILENT, mShowSilent);
    ringtonePickerIntent->PutExtra(IRingtoneManager::EXTRA_RINGTONE_TYPE, mRingtoneType);
    AutoPtr<ICharSequence> charSequenceTmp;
    GetTitle((ICharSequence**)&charSequenceTmp);
    ringtonePickerIntent->PutExtra(IRingtoneManager::EXTRA_RINGTONE_TITLE, charSequenceTmp);
    return NOERROR;
}

ECode RingtonePreference::OnSaveRingtone(
    /* [in] */ IUri* ringtoneUri)
{
    String uri = Object::ToString(ringtoneUri);
    if (uri.IsNull()) {
        uri = "";
    }
    Boolean result;
    return PersistString(uri, &result);
}

ECode RingtonePreference::OnRestoreRingtone(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    String uriString;
    GetPersistedString(String(NULL), &uriString);
    if(!TextUtils::IsEmpty(uriString)){
        AutoPtr<IUri> uritmp;
        Uri::Parse(uriString, (IUri**)&uritmp);
        *uri = uritmp;
        REFCOUNT_ADD(*uri)
    }
    else {
        *uri = NULL;
    }
    return NOERROR;
}

ECode RingtonePreference::OnGetDefaultValue(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)
    String out;
    a->GetString(index, (String*)&out);
    AutoPtr<ICharSequence> charsequ;
    CString::New(out, (ICharSequence**)&charsequ);
    *value = charsequ;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode RingtonePreference::OnSetInitialValue(
    /* [in] */ Boolean restorePersistedValue,
    /* [in] */ IInterface* defaultObj)
{
    String defaultValue;
    ICharSequence::Probe(defaultObj)->ToString(&defaultValue);

    /*
     * This method is normally to make sure the internal state and UI
     * matches either the persisted value or the default value. Since we
     * don't show the current value in the UI (until the dialog is opened)
     * and we don't keep local state, if we are restoring the persisted
     * value we don't need to do anything.
     */
    if (restorePersistedValue) {
        return NOERROR;
    }

    // If we are setting to the default value, we should persist it.
    if (!TextUtils::IsEmpty(defaultValue)) {
        AutoPtr<IUri> uri;
        Uri::Parse(defaultValue, (IUri**)&uri);
        OnSaveRingtone(uri);
    }
    return NOERROR;
}

ECode RingtonePreference::OnAttachedToHierarchy(
    /* [in] */ IPreferenceManager* preferenceManager)
{
    FAIL_RETURN(Preference::OnAttachedToHierarchy(preferenceManager))

    AutoPtr<InnerListener> listener = new InnerListener(this);
    preferenceManager->RegisterOnActivityResultListener(listener.Get());
    preferenceManager->GetNextRequestCode(&mRequestCode);
    return NOERROR;
}

ECode RingtonePreference::OnActivityResult(
    /* [in] */ Int32 requestCode,
    /* [in] */ Int32 resultCode,
    /* [in] */ IIntent* data,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (requestCode == mRequestCode) {
        if (data != NULL) {
            AutoPtr<IParcelable> p;
            data->GetParcelableExtra(IRingtoneManager::EXTRA_RINGTONE_PICKED_URI, (IParcelable**)&p);
            AutoPtr<IUri> uri = IUri::Probe(p);
            String str("");
            if (uri != NULL) IObject::Probe(uri)->ToString((String*)&str);
            AutoPtr<ICharSequence> cs;
            CString::New(str, (ICharSequence**)&cs);
            Boolean value;
            if (CallChangeListener(cs, &value), value) {
                OnSaveRingtone(uri);
            }
        }
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

}
}
}


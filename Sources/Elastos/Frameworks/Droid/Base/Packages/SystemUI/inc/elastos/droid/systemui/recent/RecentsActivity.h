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

#ifndef __ELASTOS_DROID_SYSTEMUI_RECENT_RECENTSACTIVITY_H__
#define __ELASTOS_DROID_SYSTEMUI_RECENT_RECENTSACTIVITY_H__

#include "_Elastos.Droid.SystemUI.h"
#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Os.h"
#include "Elastos.Droid.View.h"
#include "elastos/droid/app/Activity.h"
#include "elastos/droid/content/BroadcastReceiver.h"

using Elastos::Droid::App::Activity;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::SystemUI::StatusBar::IStatusBarPanel;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewOnTouchListener;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recent {

class RecentsActivity
    : public Activity
    , public IRecentsActivity
{
private:
    class IntentReceiver
        : public BroadcastReceiver
    {
    public:
        TO_STRING_IMPL("RecentsActivity.IntentReceiver")

        IntentReceiver(
            /* [in] */ RecentsActivity* host);

        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        RecentsActivity* mHost;
    };

public:
    class TouchOutsideListener
        : public Object
        , public IViewOnTouchListener
    {
    public:
        CAR_INTERFACE_DECL()

        TouchOutsideListener(
            /* [in] */ IStatusBarPanel* panel,
            /* [in] */ RecentsActivity* host);

        // @Override
        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* ev,
            /* [out] */ Boolean* result);

    private:
        RecentsActivity* mHost;
        AutoPtr<IStatusBarPanel> mPanel;
    };

public:
    CAR_INTERFACE_DECL()

    RecentsActivity();

    CARAPI constructor();

    // @Override
    CARAPI OnPause();

    // @Override
    CARAPI OnStop();

    static CARAPI ForceOpaqueBackground(
        /* [in] */ IContext* context,
        /* [out] */ Boolean* result);

    // @Override
    CARAPI OnStart();

    // @Override
    CARAPI OnResume();

    // @Override
    CARAPI OnBackPressed();

    CARAPI DismissAndGoHome();

    CARAPI DismissAndGoBack();

    CARAPI IsActivityShowing(
        /* [out] */ Boolean* result);

protected:
    // @Override
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    // @Override
    CARAPI OnSaveInstanceState(
        /* [in] */ IBundle* outState);

    // @Override
    CARAPI OnDestroy();

    // @Override
    CARAPI OnNewIntent(
        /* [in] */ IIntent* intent);

private:
    CARAPI_(void) UpdateWallpaperVisibility(
        /* [in] */ Boolean visible);

    CARAPI_(void) HandleIntent(
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean checkWaitingForAnimationParam);

    CARAPI_(Boolean) IsForeground();

private:
    static const String WAS_SHOWING;

    AutoPtr<IRecentsPanelView> mRecentsPanel;
    AutoPtr<IIntentFilter> mIntentFilter;
    Boolean mShowing;
    Boolean mForeground;

    AutoPtr<IntentReceiver> mIntentReceiver;
};

} // namespace Recent
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SYSTEMUI_RECENT_RECENTSACTIVITY_H__
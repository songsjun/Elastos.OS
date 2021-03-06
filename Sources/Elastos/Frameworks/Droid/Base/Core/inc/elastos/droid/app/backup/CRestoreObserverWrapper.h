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

#ifndef __ELASTOS_DROID_APP_BACKUP_CRESTOREOBSERVERWRAPPER_H__
#define __ELASTOS_DROID_APP_BACKUP_CRESTOREOBSERVERWRAPPER_H__

#include "_Elastos_Droid_App_Backup_CRestoreObserverWrapper.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/os/HandlerBase.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::App::Backup::IRestoreObserver;
using Elastos::Droid::App::Backup::IRestoreSet;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::HandlerBase;

namespace Elastos {
namespace Droid {
namespace App {
namespace Backup {
/*
 * We wrap incoming binder calls with a private class implementation that
 * redirects them into main-thread actions.  This serializes the restore
 * progress callbacks nicely within the usual main-thread lifecycle pattern.
 */
CarClass(CRestoreObserverWrapper)
{

private:
    class WrapperHandler: public HandlerBase
    {
    public:
        TO_STRING_IMPL("CRestoreObserverWrapper::WrapperHandler")

        WrapperHandler(
            /* [in] */ IRestoreObserver* appObserver);

        HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        AutoPtr<IRestoreObserver> mObserver;
    };

public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CRestoreObserverWrapper();

    ~CRestoreObserverWrapper();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IRestoreObserver* appObserver);

    // Binder calls into this object just enqueue on the main-thread handler
    CARAPI RestoreSetsAvailable(
        /* [in] */ ArrayOf<IRestoreSet*>* result);

    CARAPI RestoreStarting(
        /* [in] */ Int32 numPackages);

    CARAPI OnUpdate(
        /* [in] */ Int32 nowBeingRestored,
        /* [in] */ const String& currentPackage);

    CARAPI RestoreFinished(
        /* [in] */ Int32 error);

private:
    AutoPtr<WrapperHandler> mHandler;
    AutoPtr<IRestoreObserver> mAppObserver;

    static const Int32 MSG_RESTORE_STARTING;
    static const Int32 MSG_UPDATE;
    static const Int32 MSG_RESTORE_FINISHED;
    static const Int32 MSG_RESTORE_SETS_AVAILABLE;
};

}
}
}
}
#endif  //__ELASTOS_DROID_APP_BACKUP_CRESTOREOBSERVERWRAPPER_H__


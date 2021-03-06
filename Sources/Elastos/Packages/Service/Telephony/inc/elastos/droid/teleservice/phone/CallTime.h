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

#ifndef  __ELASTOS_DROID_PHONE_CALLTIME_H__
#define  __ELASTOS_DROID_PHONE_CALLTIME_H__

#include "_Elastos.Droid.TeleService.h"
#include "Elastos.Droid.Internal.h"
#include "elastos/droid/os/Handler.h"
#include <elastos/core/Runnable.h>

using Elastos::Droid::Internal::Telephony::ICall;
using Elastos::Droid::Os::Handler;
using Elastos::Core::Runnable;

namespace Elastos {
namespace Droid {
namespace TeleService {
namespace Phone {

/**
 * Helper class used to keep track of various "elapsed time" indications
 * in the Phone app, and also to start and stop tracing / profiling.
 */
class CallTime
    : public Handler
{
private:
    class PeriodicTimerCallback
        : public Runnable
    {
    public:
        TO_STRING_IMPL("CallTime::PeriodicTimerCallback")

        PeriodicTimerCallback(
            /* [in] */ CallTime* host)
            : mHost(host)
        {}

        CARAPI Run();

    private:
        CallTime* mHost;
    };

public:
    TO_STRING_IMPL("CallTime")

    CallTime(
        /* [in] */ ICallTimeOnTickListener* listener);

    /**
     * Sets the call timer to "active call" mode, where the timer will
     * periodically update the UI to show how long the specified call
     * has been active.
     *
     * After calling this you should also call reset() and
     * periodicUpdateTimer() to get the timer started.
     */
    CARAPI SetActiveCallMode(
        /* [in] */ ICall* call);

    CARAPI Reset();

    CARAPI PeriodicUpdateTimer();

    CARAPI CancelTimer();

    /**
     * Returns a "call duration" value for the specified Call, in msec,
     * suitable for display in the UI.
     */
    static CARAPI_(Int64) GetCallDuration(
        /* [in] */ ICall* call);

    static CARAPI_(void) SetTraceReady();

    CARAPI_(Boolean) IsTraceReady();

    CARAPI_(Boolean) IsTraceRunning();

    CARAPI_(void) StartTrace();

    CARAPI_(void) StopTrace();

private:
    CARAPI_(void) UpdateElapsedTime(
        /* [in] */ ICall* call);

    static CARAPI_(void) Log(
        /* [in] */ const String& msg);

private:
    static const String TAG;
    static const Boolean DBG;
    static const Boolean PROFILE;

    static const Int32 PROFILE_STATE_NONE;
    static const Int32 PROFILE_STATE_READY;
    static const Int32 PROFILE_STATE_RUNNING;

    static Int32 sProfileState;

    AutoPtr<ICall> mCall;
    Int64 mLastReportedTime;
    Boolean mTimerRunning;
    Int64 mInterval;
    AutoPtr<PeriodicTimerCallback> mTimerCallback;
    AutoPtr<ICallTimeOnTickListener> mListener;

};

} // namespace Phone
} // namespace TeleService
} // namespace Droid
} // namespace Elastos


#endif // __ELASTOS_DROID_PHONE_CALLTIME_H__
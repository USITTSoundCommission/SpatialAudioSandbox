/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the 
"Apache License"); you may not use this file except in compliance with the 
Apache License. You may obtain a copy of the Apache License at 
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2024 Audiokinetic Inc.
*******************************************************************************/

#ifndef __AK_PLATFORM_CONTEXT_IOS_H__
#define __AK_PLATFORM_CONTEXT_IOS_H__

#include <AK/SoundEngine/Common/IAkPlatformContext.h>

namespace AK
{
	/// Context specific to the iOS port of Wwise SDK.
	class IAkiOSContext : public IAkPlatformContext
	{
	public:
		// Retrieve the latest effective audio session properties
		// \sa AkPlatformInitSettings::audioSession
		// \sa AK::SoundEngine::iOS::ChangeAudioSessionProperties
		virtual AkAudioSessionProperties GetAudioSessionProperties() = 0;
		
		// This should be called at Init() time by any sink plug-in requiring an active AVAudioSession to function properly.
		// If ActivateAudioSession returns a result other than AK_Success, this result should be returned by the calling sink's Init() function.
		// \sa DeactivateAudioSession
		virtual AKRESULT ActivateAudioSession() = 0;
		
		// Each call to ActivateAudioSession should be balanced with DeactivateAudioSession when the sink no longer requires the audio session.
		// Usually this is done in the sink's Term() function.
		// \sa ActivateAudioSession
		virtual void DeactivateAudioSession() = 0;
		
		// Returns the minimum buffer period, in seconds, required to avoid starvation on the current audio route
		virtual float GetCurrentRouteMinimumLatency() const = 0;
		
		// Request a change in the audio session's output channel count
		// The request may be denied by the system, in which case AK_UnsupportedChannelConfig will be returned.
		// Pass 0 to maximize the number of output channels with regards to AVAudioSession::maximumOutputNumberOfChannels
		virtual AKRESULT SetPreferredNumberOfOutputChannels(AkUInt32 uNumChannels) = 0;
	};
}

#endif // __AK_PLATFORM_CONTEXT_IOS_H__

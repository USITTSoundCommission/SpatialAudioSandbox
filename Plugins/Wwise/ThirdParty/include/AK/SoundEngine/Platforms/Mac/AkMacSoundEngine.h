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

// AkMacSoundEngine.h

/// \file 
/// Main Sound Engine interface, specific Mac.

#ifndef _AK_MAC_SOUND_ENGINE_H_
#define _AK_MAC_SOUND_ENGINE_H_

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

///< API used for audio output
///< Use with AkPlatformInitSettings to select the API used for audio output. The selected API may be ignored if the device does not support it.
///< Use AkAPI_Default, it will select the more appropriate API depending on the device's capabilities.  Other values should be used for testing purposes.
///< \sa AK::SoundEngine::Init
typedef enum AkAudioAPIMac
{
	AkAudioAPI_AVAudioEngine    = 1 << 0,                                 ///< Use AVFoundation framework (modern, has more capabilities, available only for macOS 10.15 or above)
	AkAudioAPI_AudioUnit        = 1 << 1,                                 ///< Use AudioUnit framework (basic functionality, compatible with all macOS devices)
	AkAudioAPI_Default = AkAudioAPI_AVAudioEngine | AkAudioAPI_AudioUnit, ///< Default value, will select the more appropriate API (AVAudioEngine for compatible devices, AudioUnit for others)
} AkAudioAPI;

/// Platform specific initialization settings
/// \sa AK::SoundEngine::Init
/// \sa AK::SoundEngine::GetDefaultPlatformInitSettings
struct AkPlatformInitSettings
{
	// Threading model.
    AkThreadProperties  threadLEngine;			///< Lower engine threading properties
	AkThreadProperties  threadOutputMgr;		///< Ouput thread threading properties
	AkThreadProperties  threadBankManager;		///< Bank manager threading properties (its default priority is AK_THREAD_PRIORITY_NORMAL)
	AkThreadProperties  threadMonitor;			///< Monitor threading properties (its default priority is AK_THREAD_PRIORITY_ABOVENORMAL). This parameter is not used in Release build.
	
	AkUInt32			uSampleRate;			///< Sampling Rate. Default 48000 Hz
	// Voices.
	AkUInt16            uNumRefillsInVoice;		///< Number of refill buffers in voice buffer. 2 == double-buffered, defaults to 4.
	
	AkAudioAPI          eAudioAPI;  ///< Main audio API to use. Leave to AkAPI_Default for the default sink (default value).
									///< \ref AkAudioAPI
	
	AkUInt32            uNumSpatialAudioPointSources; ///< Number of Apple Spatial Audio point sources to allocate for 3D audio use (each point source is a system audio object). Default: 128
	
	bool                bVerboseSystemOutput;   ///< Print detailed system output information to the console log
};

namespace AK
{
	namespace SoundEngine
	{
		/// Get the motion device ID corresponding to a GCController player index. This device ID can be used to add/remove motion output for that gamepad.
		/// The player index is 0-based, and corresponds to a value of type GCControllerPlayerIndex in the Core.Haptics framework.
		/// \note The ID returned is unique to Wwise and does not correspond to any sensible value outside of Wwise.
		/// \return Unique device ID
		AK_EXTERNAPIFUNC(AkDeviceID, GetDeviceIDFromPlayerIndex) (int playerIndex);
	}
}

#endif //_AK_MAC_SOUND_ENGINE_H_

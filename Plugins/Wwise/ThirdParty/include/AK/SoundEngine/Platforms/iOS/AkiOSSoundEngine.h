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

// AkiOSSoundEngine.h

/// \file 
/// Main Sound Engine interface, specific iOS.

#ifndef _AK_IOS_SOUND_ENGINE_H_
#define _AK_IOS_SOUND_ENGINE_H_

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>

#include <AudioToolbox/AudioToolbox.h>

/// The IDs of the iOS audio session categories, useful for defining app-level audio behaviours such as inter-app audio mixing policies and audio routing behaviours. These IDs are funtionally equivalent to the corresponding constants defined by the iOS audio session service backend (AVAudioSession). Refer to Xcode documentation for details on the audio session categories. The original prefix "AV" is replaced with "Ak" for the ID names.
///
/// \sa
/// - \ref AkPlatformInitSettings
/// - \ref AkAudioSessionCategoryOptions
/// - \ref AkAudioSessionProperties
enum AkAudioSessionCategory
{
	AkAudioSessionCategoryAmbient, ///< Audio session category corresponding to the AVAudiosession's AVAudioSessionCategoryAmbient constant
	AkAudioSessionCategorySoloAmbient, ///< Audio session category corresponding to the AVAudiosession's AVAudioSessionCategorySoloAmbient constant
    AkAudioSessionCategoryPlayAndRecord, ///< Audio session category corresponding to the AVAudiosession's AVAudioSessionCategoryPlayAndRecord constant
	AkAudioSessionCategoryPlayback ///< Audio session category corresponding to the AVAudiosession's AVAudioSessionCategoryPlayback constant
};

/// The IDs of the iOS audio session category options, used for customizing the audio session category features. These IDs are funtionally equivalent to the corresponding constants defined by the iOS audio session service backend (AVAudioSession). Refer to Xcode documentation for details on the audio session category options. The original prefix "AV" is replaced with "Ak" for the ID names.
/// \remark These options only have an effect with specific audio session categories. See the documentation for each option to learn which category they affect.
///
/// \sa
/// - \ref AkPlatformInitSettings
/// - \ref AkAudioSessionCategory
/// - \ref AkAudioSessionProperties
enum AkAudioSessionCategoryOptions
{
	AkAudioSessionCategoryOptionMixWithOthers  = 1, ///< Same as AVAudioSessionCategoryOptionMixWithOthers. Only affects PlayAndRecord and Playback categories.
	AkAudioSessionCategoryOptionDuckOthers  = 2, ///< Same as AVAudioSessionCategoryOptionDuckOthers. Implicitely sets the MixWithOthers option. Only affects PlayAndRecord and Playback categories.
	AkAudioSessionCategoryOptionAllowBluetooth  = 4, ///< Same as AVAudioSessionCategoryOptionAllowBluetooth. Only affects PlayAndRecord category.
	AkAudioSessionCategoryOptionDefaultToSpeaker  = 8, ///< Same as AVAudioSessionCategoryOptionDefaultToSpeaker. Only affects PlayAndRecord category.
	AkAudioSessionCategoryOptionAllowBluetoothA2DP = 0x20 ///< Same as AVAudioSessionCategoryOptionAllowBluetoothA2DP. Only affects PlayAndRecord category.
};

/// The IDs of the iOS audio session modes, used for customizing the audio session for typical app types. These IDs are funtionally equivalent to the corresponding constants defined by the iOS audio session service backend (AVAudioSession). Refer to Xcode documentation for details on the audio session category options. The original prefix "AV" is replaced with "Ak" for the ID names.
///
/// \sa
/// - \ref AkPlatformInitSettings
/// - \ref AkAudioSessionProperties
enum AkAudioSessionMode
{
	AkAudioSessionModeDefault = 0, ///< Audio session mode corresponding to the AVAudiosession's AVAudioSessionModeDefault constant
	AkAudioSessionModeVoiceChat, ///< Audio session mode corresponding to the AVAudiosession's AVAudioSessionModeVoiceChat constant
	AkAudioSessionModeGameChat, ///< Audio session mode corresponding to the AVAudiosession's AVAudioSessionModeGameChat constant
	AkAudioSessionModeVideoRecording, ///< Audio session mode corresponding to the AVAudiosession's AVAudioSessionModeVideoRecording constant
	AkAudioSessionModeMeasurement, ///< Audio session mode corresponding to the AVAudiosession's AVAudioSessionModeMeasurement constant
	AkAudioSessionModeMoviePlayback, ///< Audio session mode corresponding to the AVAudiosession's AVAudioSessionModeMoviePlayback constant
	AkAudioSessionModeVideoChat ///< Audio session mode corresponding to the AVAudiosession's AVAudioSessionModeMoviePlayback constant
};

/// The behavior flags for when iOS audio session is activated. These IDs are functionally equivalent to the corresponding constants defined by the iOS audio session service backend (AVAudioSession). Refer to Xcode documentation for details on the audio session options. The original prefix "AV" is replaced with "Ak" for the ID names.
///
/// \sa
/// - \ref AkPlatformInitSettings
/// - \ref AkAudioSessionProperties
enum AkAudioSessionSetActiveOptions
{
	 AkAudioSessionSetActiveOptionNotifyOthersOnDeactivation  = 1 ///< Audio session activation option corresponding to the AVAudiosession's AVAudioSessionSetActiveOptionNotifyOthersOnDeactivation constant
};

/// Flags that can modify the default Sound Engine behavior related to the management of the audio session. These do not have equivalences in the official iOS SDK; they apply uniquely to Wwise's approach to interruption handling.
enum AkAudioSessionBehaviorOptions
{
	AkAudioSessionBehaviorSuspendInBackground = 0x1 ///< By default, the Sound Engine continues to render audio in the background when using PlayAndRecord or Playback categories. Setting this flag causes the Sound Engine to suspend audio rendering when in the background, thus disabling background audio. Only affects Playback and PlayAndRecord audio session categories.
};

/// The API structure used with AkPlatformInitSettings for specifying iOS audio session for the sound engine.
///
/// \sa
/// - \ref AkPlatformInitSettings
/// - \ref AkAudioSessionCategory
/// - \ref AkAudioSessionCategoryOptions
/// - \ref AkAudioSessionMode
/// - \ref AkAudioSessionSetActiveOptions
struct AkAudioSessionProperties
{
	AkAudioSessionCategory eCategory; ///< \sa AkAudioSessionCategory
	AkAudioSessionCategoryOptions eCategoryOptions; ///< \sa AkAudioSessionCategoryOptions
	AkAudioSessionMode eMode; ///< \sa AkAudioSessionMode
	AkAudioSessionSetActiveOptions eSetActivateOptions; ///< \sa AkAudioSessionSetActiveOptions
	AkAudioSessionBehaviorOptions eAudioSessionBehavior; ///< Flags to change the default Sound Engine behavior related to the management of the iOS Audio Session with regards to application lifecycle events. \sa AkAudioSessionBehaviorFlags
};

///< API used for audio output
///< Use with AkPlatformInitSettings to select the API used for audio output. The selected API may be ignored if the device does not support it.
///< Use AkAPI_Default, it will select the more appropriate API depending on the device's capabilities.  Other values should be used for testing purposes.
///< \sa AK::SoundEngine::Init
typedef enum AkAudioAPIiOS
{
	AkAudioAPI_AVAudioEngine    = 1 << 0,                                 ///< Use AVFoundation framework (modern, has more capabilities, available only for iOS/tvOS 13 or above)
	AkAudioAPI_AudioUnit        = 1 << 1,                                 ///< Use AudioUnit framework (basic functionality, compatible with all iOS devices)
	AkAudioAPI_Default = AkAudioAPI_AVAudioEngine | AkAudioAPI_AudioUnit, ///< Default value, will select the more appropriate API (AVAudioEngine for compatible devices, AudioUnit for others)
} AkAudioAPI;

namespace AK
{
	namespace SoundEngine
	{
		namespace iOS
		{
			/// iOS-only callback function prototype used for audio input source plugin. Implement this function to transfer the 
			/// input sample data to the sound engine and perform brief custom processing.
			/// \remark See the remarks of \ref AkGlobalCallbackFunc.
			///
			/// \sa
			/// - \ref AkPlatformInitSettings
			typedef AKRESULT ( * AudioInputCallbackFunc )(
				const AudioBufferList* io_Data,	///< An exposed CoreAudio structure that holds the input audio samples generated from 
											///< audio input hardware. The buffer is pre-allocated by the sound engine and the buffer 
											///< size can be obtained from the structure. Refer to the microphone demo of the IntegrationDemo for an example of usage.
				void* in_pCookie ///< User-provided data, e.g., a user structure.
				);

			/// iOS-only callback function prototype used for handling audio session interruptions.
			/// Implementing this is optional, but useful for application-specific responses to interruptions. For example, an application can enable or disable certain UI elements when an interruption begins and ends.
			/// \remark 
			/// - There is no need to call AK::SoundEngine::Suspend() and AK::SoundEngine::WakeupFromSuspend() in this callback. The sound engine call them internally depending on the interruption status.
			/// - When in_bEnterInterruption is true, this callback is called \a before the sound engine calls AK::SoundEngine::Suspend(), where the user can take actions to prepare for the suspend, e.g., posting global pause events or switching to a special user interface.
			/// - When in_bEnterInterruption is false, this callback is called \a before the sound engine calls AK::SoundEngine::WakeFromSuspend(). In this callback, the user can restore suspended resources, e.g., post global resume events or switch back to the default user interface.
			/// - To receive a callback \a after the Sound Engine has woken up from suspend after an interruption, use AK::SoundEngine::RegisterGlobalCallback with the AkGlobalCallbackLocation_WakeupFromSuspend location instead.
			///
			/// \sa
			/// - \ref AkGlobalCallbackFunc
			/// - \ref AkPlatformInitSettings
			/// - \ref AK::SoundEngine::Suspend
			/// - \ref AK::SoundEngine::WakeupFromSuspend
			/// - \ref AK::SoundEngine::RegisterGlobalCallback
			typedef void ( * AudioInterruptionCallbackFunc )(
				bool in_bEnterInterruption,	///< Indicating whether or not an interruption is about to start (e.g., an incoming 
											///< call is received) or end (e.g., the incoming call is dismissed).

				void* in_pCookie ///< User-provided data, e.g., a user structure.
				);

			/// Change the category and options of the app's AVAudioSession without restarting the entire Sound Engine.
			/// \remark
			/// As per Apple recommendations, the AVAudioSession will be deactivated and then reactivated. Therefore, 
			/// the primary output device must be reinitialized, which causes all audio to stop for a short period of time.
			/// For a seamless transition, call this API during moments of complete silence in your game.
			///
			/// \sa
			/// - \ref AkAudioSessionProperties
			AK_EXTERNAPIFUNC( void, ChangeAudioSessionProperties )(
				const AkAudioSessionProperties &in_properties                 ///< New properties to apply to the app's AVAudioSession shared instance.
				);
		}

		/// Get the motion device ID corresponding to a GCController player index. This device ID can be used to add/remove motion output for that gamepad.
		/// The player index is 0-based, and corresponds to a value of type GCControllerPlayerIndex in the Core.Haptics framework.
		/// \note The ID returned is unique to Wwise and does not correspond to any sensible value outside of Wwise.
		/// \return Unique device ID
		AK_EXTERNAPIFUNC(AkDeviceID, GetDeviceIDFromPlayerIndex) (int playerIndex);
	}
}

/// The API structure used for specifying all iOS-specific callback functions and user data from the app side.
///
/// \sa
/// - \ref AkPlatformInitSettings
struct AkAudioCallbacks
{
	AK::SoundEngine::iOS::AudioInputCallbackFunc inputCallback;					///< Application-defined audio input callback function
	void* inputCallbackCookie;													///< Application-defined user data for the audio input callback function
	AK::SoundEngine::iOS::AudioInterruptionCallbackFunc interruptionCallback;	///< Application-defined audio interruption callback function
	void* interruptionCallbackCookie;											///< Application-defined user data for the audio interruption callback function
};

/// Platform specific initialization settings
/// \sa AK::SoundEngine::Init
/// \sa AK::SoundEngine::GetDefaultPlatformInitSettings
/// - \ref AK::SoundEngine::iOS::AkAudioSessionCategory
struct AkPlatformInitSettings
{
	// Threading model.
    AkThreadProperties  threadLEngine;			///< Lower engine threading properties
	AkThreadProperties  threadOutputMgr;		///< Ouput thread threading properties
	AkThreadProperties  threadBankManager;		///< Bank manager threading properties (its default priority is AK_THREAD_PRIORITY_NORMAL)
	AkThreadProperties  threadMonitor;			///< Monitor threading properties (its default priority is AK_THREAD_PRIORITY_ABOVENORMAL). This parameter is not used in Release build.
	
	AkUInt32			uSampleRate;			///< Sampling Rate. Default 48000 Hz
	// Voices.
	AkUInt16            uNumRefillsInVoice;		///< Number of refill buffers in voice buffer. 2 == double-buffered, defaults to 4
	AkAudioSessionProperties audioSession;	///< iOS audio session properties 
	AkAudioCallbacks audioCallbacks; ///< iOS audio callbacks
	
	AkAudioAPI          eAudioAPI;  ///< Main audio API to use. Leave to AkAPI_Default for the default sink (default value).
									///< \ref AkAudioAPI
	
	AkUInt32            uNumSpatialAudioPointSources; ///< Number of Apple Spatial Audio point sources to allocate for 3D audio use (each point source is a system audio object). Default: 128

	bool bVerboseSystemOutput; ///< Print additional debugging information specific to iOS to the system output log.
};


#endif //_AK_IOS_SOUND_ENGINE_H_

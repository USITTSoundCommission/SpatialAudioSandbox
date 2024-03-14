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

#pragma once

#include <AK/Tools/Common/AkAssert.h>
#include <AK/SoundEngine/Common/AkTypes.h>
#include <android/log.h>

#if (defined(AK_CPU_X86_64) || defined(AK_CPU_X86))
#include <cpuid.h>
#endif
#include <time.h>
#include <stdlib.h>

#define AK_THREAD_INIT_CODE(_threadProperties) syscall(__NR_sched_setaffinity, 0, sizeof(_threadProperties.dwAffinityMask), &_threadProperties.dwAffinityMask)
#define AK_SEC_TO_NANOSEC 1000000000ULL

namespace AKPLATFORM
{
	/// Platform Independent Helper
	inline void PerformanceCounter( AkInt64 * out_piLastTime )
	{
		struct timespec clockNow;
		clock_gettime(CLOCK_MONOTONIC, &clockNow);
		//This give the wallclock time in NS
		*out_piLastTime = clockNow.tv_sec*AK_SEC_TO_NANOSEC + clockNow.tv_nsec;
	}
	
	/// Platform Independent Helper
	inline void PerformanceFrequency( AkInt64 * out_piFreq )
	{
		//Since Wall Clock is used, 1 NS is the frequency independent of the clock resolution
		*out_piFreq = AK_SEC_TO_NANOSEC;
	}
}

#include <AK/Tools/POSIX/AkPlatformFuncs.h>

/// Stack allocations.
#define AkAlloca( _size_ ) __builtin_alloca( _size_ )

namespace AKPLATFORM
{
	/// Output a debug message on the console (Ansi string)

#if defined(AK_OPTIMIZED)
	inline void OutputDebugMsg( const char* ){}

	template <int MaxSize = 0> // Unused
	inline void OutputDebugMsgV( const char* in_pszFmt, ... ) {}
#else // AK_ANDROID
	inline void OutputDebugMsg( const char* in_pszMsg )
	{
		// To see output of this
		// adb logcat ActivityManager:I YourApplication:D AKDEBUG:D *:S
		__android_log_print(ANDROID_LOG_INFO, "AKDEBUG", "%s", in_pszMsg);
	}

	/// Output a debug message on the console (variadic function).
	template <int MaxSize = 0> // Unused
	inline void OutputDebugMsgV( const char* in_pszFmt, ...)
	{
		va_list args;
		va_start(args, in_pszFmt);
		__android_log_vprint(ANDROID_LOG_INFO, "AKDEBUG", in_pszFmt, args);
		va_end(args);
	}
#endif

	template<class destType, class srcType>
	inline size_t AkSimpleConvertString( destType* in_pdDest, const srcType* in_pSrc, size_t in_MaxSize, size_t destStrLen(const destType *),  size_t srcStrLen(const srcType *) )
	{ 
		size_t i;
		size_t lenToCopy = srcStrLen(in_pSrc);
		
		lenToCopy = (lenToCopy > in_MaxSize-1) ? in_MaxSize-1 : lenToCopy;
		for(i = 0; i < lenToCopy; i++)
		{
			in_pdDest[i] = (destType) in_pSrc[i];
		}
		in_pdDest[lenToCopy] = (destType)0;
		
		return lenToCopy;
	}

	#define CONVERT_UTF16_TO_CHAR( _astring_, _charstring_ ) \
		_charstring_ = (char*)AkAlloca( (1 + AKPLATFORM::AkUtf16StrLen((const AkUtf16*)_astring_)) * sizeof(char) ); \
		AK_UTF16_TO_CHAR( _charstring_, (const AkUtf16*)_astring_, AKPLATFORM::AkUtf16StrLen((const AkUtf16*)_astring_)+1 ) 

	#define AK_UTF8_TO_OSCHAR(	in_pdDest, in_pSrc, in_MaxSize )	AKPLATFORM::AkSimpleConvertString( in_pdDest, in_pSrc, in_MaxSize, strlen, strlen )
	#define AK_UTF16_TO_OSCHAR(	in_pdDest, in_pSrc, in_MaxSize )	AKPLATFORM::AkSimpleConvertString(	in_pdDest, in_pSrc, in_MaxSize, strlen, AKPLATFORM::AkUtf16StrLen )
	#define AK_UTF16_TO_CHAR(	in_pdDest, in_pSrc, in_MaxSize )	AKPLATFORM::AkSimpleConvertString(	in_pdDest, in_pSrc, in_MaxSize, strlen, AKPLATFORM::AkUtf16StrLen )
	#define AK_CHAR_TO_UTF16(	in_pdDest, in_pSrc, in_MaxSize )	AKPLATFORM::AkSimpleConvertString(	in_pdDest, in_pSrc, in_MaxSize, AKPLATFORM::AkUtf16StrLen, strlen)	
	#define AK_OSCHAR_TO_UTF16(	in_pdDest, in_pSrc, in_MaxSize )	AKPLATFORM::AkSimpleConvertString(	in_pdDest, in_pSrc, in_MaxSize, AKPLATFORM::AkUtf16StrLen, strlen)	
	
	#if __BIGGEST_ALIGNMENT__ < AK_SIMD_ALIGNMENT
	#define AkAllocaSIMD( _size_ ) __builtin_alloca_with_align( _size_, AK_SIMD_ALIGNMENT*8 )
	#endif

	/// Platform Independent Helper
	inline void AkCreateThread( 
		AkThreadRoutine pStartRoutine,					// Thread routine.
		void * pParams,									// Routine params.
		const AkThreadProperties & in_threadProperties,	// Properties. NULL for default.
		AkThread * out_pThread,							// Returned thread handle.
		const char * /*in_szThreadName*/ )				// Opt thread name.
	{
		AKASSERT( out_pThread != NULL );

		pthread_attr_t  attr;

		// Create the attr
		AKVERIFY(!pthread_attr_init(&attr));
		// Set the stack size
		AKVERIFY(!pthread_attr_setstacksize(&attr,in_threadProperties.uStackSize));

		AKVERIFY(!pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE));		

		// Create the tread
		int     threadError = pthread_create( out_pThread, &attr, pStartRoutine, pParams);
		AKASSERT( threadError == 0 );
		AKVERIFY(!pthread_attr_destroy(&attr));

		if( threadError != 0 )
		{
			AkClearThread( out_pThread );
			return;
		}

		// ::CreateThread() return NULL if it fails.
		if ( !*out_pThread )
		{
			AkClearThread( out_pThread );
			return;
		}		

		// Try to set the thread policy
		int sched_policy = in_threadProperties.uSchedPolicy;		

		// Get the priority for the policy
		int minPriority, maxPriority;
		minPriority = sched_get_priority_min(sched_policy);
		maxPriority = sched_get_priority_max(sched_policy);

		// Set the thread priority if valid
		sched_param schedParam;
		schedParam.sched_priority = in_threadProperties.nPriority;	
		AKASSERT( in_threadProperties.nPriority >= minPriority && in_threadProperties.nPriority <= maxPriority );		

		//pthread_setschedparam WILL fail on Android Lollipop when used with SCHED_FIFO (the default).  Not allowed anymore. (ignore the error code).
		int err = pthread_setschedparam(*out_pThread, sched_policy, &schedParam);
		if (err != 0)
		{			
			//Make sure the priority is well set, even if the policy could not.			
			sched_policy = SCHED_NORMAL;
			minPriority = sched_get_priority_min(sched_policy);
			maxPriority = sched_get_priority_max(sched_policy);
			if (in_threadProperties.nPriority == AK_THREAD_PRIORITY_ABOVE_NORMAL)
				schedParam.sched_priority = maxPriority;
			else if (in_threadProperties.nPriority == AK_THREAD_PRIORITY_BELOW_NORMAL)
				schedParam.sched_priority = minPriority;
			else
				schedParam.sched_priority = (maxPriority + minPriority) / 2;
			err = pthread_setschedparam(*out_pThread, sched_policy, &schedParam);						
			AKASSERT(err == 0);
		}
	}

	#define AK_FILEHANDLE_TO_UINTPTR(_h) ((AkUIntPtr)_h)
	#define AK_SET_FILEHANDLE_TO_UINTPTR(_h,_u) _h = (AkFileHandle)_u

#if (defined(AK_CPU_X86_64) || defined(AK_CPU_X86))
	// Once our minimum compiler version supports __get_cpuid_count, these asm blocks can be replaced
	#if defined(__i386__) && defined(__PIC__)
		// %ebx may be the PIC register. 
		#define __ak_cpuid_count(level, count, a, b, c, d)		\
			__asm__ ("xchg{l}\t{%%}ebx, %k1\n\t"				\
				"cpuid\n\t"										\
				"xchg{l}\t{%%}ebx, %k1\n\t"						\
				: "=a" (a), "=&r" (b), "=c" (c), "=d" (d)		\
				: "0" (level), "2" (count))
	#elif defined(__x86_64__) && defined(__PIC__)
		// %rbx may be the PIC register. 
		#define __ak_cpuid_count(level, count, a, b, c, d)		\
			__asm__ ("xchg{q}\t{%%}rbx, %q1\n\t"				\
				"cpuid\n\t"										\
				"xchg{q}\t{%%}rbx, %q1\n\t"						\
				: "=a" (a), "=&r" (b), "=c" (c), "=d" (d)		\
				: "0" (level), "2" (count))
	#else
	#define __ak_cpuid_count(level, count, a, b, c, d)			\
			__asm__ ("cpuid\n\t"								\
				: "=a" (a), "=b" (b), "=c" (c), "=d" (d)		\
				: "0" (level), "2" (count))
	#endif
	
	static __inline int __ak_get_cpuid_count(unsigned int __leaf,
		unsigned int __subleaf,
		unsigned int *__eax, unsigned int *__ebx,
		unsigned int *__ecx, unsigned int *__edx)
	{
		unsigned int __max_leaf = __get_cpuid_max(__leaf & 0x80000000, 0);

		if (__max_leaf == 0 || __max_leaf < __leaf)
			return 0;

		__ak_cpuid_count(__leaf, __subleaf, *__eax, *__ebx, *__ecx, *__edx);
		return 1;
	}

	/// Support to fetch the CPUID for the platform. Only valid for X86 targets
	/// \remark Note that IAkProcessorFeatures should be preferred to fetch this data
	/// as it will have already translated the feature bits into AK-relevant enums
	inline void CPUID(AkUInt32 in_uLeafOpcode, AkUInt32 in_uSubLeafOpcode, unsigned int out_uCPUFeatures[4])
	{
		__ak_get_cpuid_count( in_uLeafOpcode, in_uSubLeafOpcode,
			&out_uCPUFeatures[0],
			&out_uCPUFeatures[1],
			&out_uCPUFeatures[2],
			&out_uCPUFeatures[3]);
	}
#endif
}

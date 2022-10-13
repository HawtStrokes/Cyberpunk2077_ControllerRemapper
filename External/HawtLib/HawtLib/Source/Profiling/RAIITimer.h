#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif
#include <chrono>


namespace HawtLib {
	namespace Profiling {

		class SPEC RAIITimer{
		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
			float* duration;

		public:			
			// no copy
			RAIITimer(const RAIITimer&) = delete;	
			RAIITimer operator=(const RAIITimer&) = delete;

		private:
			inline void Internal_Start();
			inline void Internal_Stop();

		public:
			RAIITimer(float* outDuration);
			~RAIITimer();
		};
	}
}

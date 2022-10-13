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

		class SPEC ControlledTimer {
		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
			float duration;	// in ns
		public:
			ControlledTimer() = default;
			ControlledTimer(const ControlledTimer&) = delete;
			ControlledTimer operator=(const ControlledTimer&) = delete;
			~ControlledTimer() = default;

		private:
			inline void Internal_Start();
			inline void Internal_Stop();

		public:
			void Start();
			void Stop();

			float GetDuration() const;
		};
	}
}
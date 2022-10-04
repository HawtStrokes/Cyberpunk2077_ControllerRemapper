#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif

#include "Interfaces/ITimer.h"

namespace HawtLib {
	namespace Profiling {

		class SPEC ControlledTimer : public ITimer {
		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
			float duration;	// in ns
		public:
			ControlledTimer();
			ControlledTimer(const ControlledTimer&) = delete;
			ControlledTimer operator=(const ControlledTimer&) = delete;
			~ControlledTimer() override = default;

		private:
			inline void _Start() noexcept override;

			inline void _Stop() noexcept override;
		public:

			void Start();

			void Stop();

			float GetDuration() const;
		};
	}
}
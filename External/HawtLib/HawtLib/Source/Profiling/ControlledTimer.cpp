#include "ControlledTimer.h"

#include <chrono>

namespace HawtLib {
	namespace Profiling {
		inline void ControlledTimer::Internal_Start() {
			startTime = std::chrono::high_resolution_clock::now();
		}

		inline void ControlledTimer::Internal_Stop() {
			duration = static_cast<float>((std::chrono::high_resolution_clock::now() - startTime).count());
		}

		void ControlledTimer::Start() {
			Internal_Start();
		}

		void ControlledTimer::Stop() {
			Internal_Stop();
		}

		float ControlledTimer::GetDuration() const {
			return duration;
		}
	}
}


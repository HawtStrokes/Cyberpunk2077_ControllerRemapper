#include "ControlledTimer.h"
#include <iostream>

namespace HawtLib {
	namespace Profiling {
		inline void ControlledTimer::_Start() noexcept {
			startTime = std::chrono::high_resolution_clock::now();
		}

		inline void ControlledTimer::_Stop() noexcept {
			duration = static_cast<float>((std::chrono::high_resolution_clock::now() - startTime).count());
		}

		void ControlledTimer::Start() {
			_Start();
		}

		void ControlledTimer::Stop() {
			_Stop();
		}

		float ControlledTimer::GetDuration() const {
			return duration;
		}
	}
}


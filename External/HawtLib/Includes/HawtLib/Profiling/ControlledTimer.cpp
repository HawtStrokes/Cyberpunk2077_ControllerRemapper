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

		ControlledTimer::ControlledTimer() : duration(0) {}


		float ControlledTimer::GetDuration() const {
			return duration;
		}
	}
}


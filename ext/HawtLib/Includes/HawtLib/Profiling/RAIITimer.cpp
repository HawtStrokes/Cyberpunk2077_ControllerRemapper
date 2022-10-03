#include "RAIITimer.h"
#include <iostream>

namespace HawtLib {
	namespace Profiling {
		inline void RAIITimer::_Start() noexcept {
			startTime = std::chrono::high_resolution_clock::now();
		}

		inline void RAIITimer::_Stop() noexcept {
			// duration in nanoSeconds
			*duration = static_cast<float>((std::chrono::high_resolution_clock::now() - startTime).count());	
		}

		RAIITimer::RAIITimer(float* outDuration) : duration(outDuration) {
			_Start();
		}
		RAIITimer::~RAIITimer() {
			_Stop();
		}
	}
}


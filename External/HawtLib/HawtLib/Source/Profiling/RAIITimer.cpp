#include "RAIITimer.h"

#include <chrono>

namespace HawtLib {
	namespace Profiling {
		inline void RAIITimer::Internal_Start() {
			startTime = std::chrono::high_resolution_clock::now();
		}

		inline void RAIITimer::Internal_Stop() {
			// duration in nanoSeconds
			*duration = static_cast<float>((std::chrono::high_resolution_clock::now() - startTime).count());	
		}

		RAIITimer::RAIITimer(float* outDuration) : duration(outDuration) {
			Internal_Start();
		}
		RAIITimer::~RAIITimer() {
			Internal_Stop();
		}
	}
}


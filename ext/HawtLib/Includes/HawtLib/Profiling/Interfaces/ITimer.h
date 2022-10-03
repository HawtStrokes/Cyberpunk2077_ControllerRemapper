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
		class SPEC ITimer {
		private:
			virtual inline void _Start() noexcept = 0;
			virtual inline void _Stop() noexcept = 0;
		public:
			virtual ~ITimer() = default;
		};
	}
}
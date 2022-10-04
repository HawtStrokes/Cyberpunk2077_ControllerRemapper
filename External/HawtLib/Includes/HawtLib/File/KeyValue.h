#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif

namespace HawtLib {
	namespace File {
		template <typename T, typename U> struct SPEC KeyValue {
			T key;
			U value;
		};
	}
}


#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif

#include <exception>

namespace HawtLib {
	namespace Text {
		class SPEC SmallStringBufferError : public std::exception {
		public:
			SmallStringBufferError() : std::exception("String Buffer Must At Least Be Equal to String Length!") {}
		};

	}
}
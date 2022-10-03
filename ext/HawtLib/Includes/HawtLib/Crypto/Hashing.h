#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif

#include <string>


namespace HawtLib {
    namespace Crypto {
        SPEC unsigned long long DJB2(const std::string& str);
        SPEC unsigned long long SDBM(const std::string& str);
	}
}
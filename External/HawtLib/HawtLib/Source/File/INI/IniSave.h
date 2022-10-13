#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif

#include <fstream>

namespace HawtLib {
	namespace File {
		class IniFile;

		namespace Persistence {
			SPEC void IniSave(IniFile& iniFile, const std::string& file);
		}
	}
}

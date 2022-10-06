#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif

#include <exception>
#include <string>

namespace HawtLib
{
	namespace File
	{
		class SPEC FileDoesNotExistError : public std::exception 
		{
		public:
			FileDoesNotExistError() : std::exception("File Does Not Exist!") {}
		};

		class SPEC FileTokenError : public std::exception {
		public:
			FileTokenError() : std::exception("File Contains Incorrect Syntax!") {}
			FileTokenError(const std::string& msg) : std::exception(msg.c_str()) {}
		};

	}
}
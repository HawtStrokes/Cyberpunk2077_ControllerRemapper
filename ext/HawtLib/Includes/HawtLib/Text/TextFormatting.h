#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif

#include <string>
#include <vector>

namespace HawtLib {

	namespace Text {
		SPEC std::string CenterText(const std::string& inStr, size_t space = 120);
		SPEC std::string RightText(const std::string& inStr, size_t space = 120);
		SPEC std::string ToLower(const std::string& inStr);
		SPEC char ToLower(char inC);
		SPEC std::string ToUpper(const std::string& inStr);
		SPEC char ToUpper(char inC);
		SPEC std::string Title(const std::string& inStr);
		SPEC std::vector<std::string> SplitString(const std::string& inStr, char delim = ' ');
		SPEC std::string TrimLead(const std::string& str);
		SPEC std::string TrimTrail(const std::string& str);
		SPEC std::string Trim(const std::string& str);
		// std::string SurroundString(const std::string& str);
	}
}

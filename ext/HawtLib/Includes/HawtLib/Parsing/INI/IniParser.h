#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif

#include <vector>
#include <string>

namespace HawtLib::Parsing::Ini {
	class IniFile;

	class SPEC IniParser {
	public:
		enum class TokenType {
			Section = 0,
			Key = 1,
			Value = 2
		};
		struct Token {
			TokenType type;
			std::string data;
		};

	private:
		// Tokenize a line
		std::vector<Token*>* _Lex(std::string& line);

		// creates an IniFile object
		void _Parse(IniFile* iniFile, std::vector<std::vector<IniParser::Token*>>&);

		// cleans the std::vector<Token*>* made by _Lex()
		void _CleanTokens(std::vector<std::vector<IniParser::Token*>>&);

	public:
		IniParser() = delete;
		IniParser(const IniParser&) = delete;
		IniParser operator=(const IniParser&) = delete;

		static IniParser& Get();

		void Read(IniFile* iniFile, const std::string& file);

		// friend class IniFile;
	};
}
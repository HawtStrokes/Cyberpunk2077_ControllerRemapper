#include "../../Text/TextFormatting.h"
#include "IniParser.h"
#include "IniFile.h"

#include <fstream>
#include <exception>

namespace HawtLib::Parsing::Ini {
	IniParser& IniParser::Get() {
		static IniParser* instance = nullptr;
		return *instance;
	}

	void IniParser::Read(IniFile* iniFile, const std::string& file) {
		std::ifstream ifs(file);

		if (!ifs) {
			throw std::exception("Cannot Open File!");
		}

		std::vector<std::vector<IniParser::Token*>> allTokens;
		

		// Tokenize
		std::string line;
		while (std::getline(ifs, line)) {
			line = Text::Trim(line);
		 	std::vector<IniParser::Token*>* lineTokens = _Lex(line);
			if (lineTokens->size() > 0) allTokens.push_back(*lineTokens);
		}


		if (!iniFile->m_Sections.empty()) iniFile->_CleanUp();	// clean if not empty

		// Fill IniFile construct with data
		_Parse(iniFile, allTokens);
		ifs.close();
	}

	std::vector<IniParser::Token*>* IniParser::_Lex(std::string& line) {
		line = HawtLib::Text::Trim(line);
		bool openSection = false;
		bool shouldEscape = false;

		size_t tokenBegin = 0;
		size_t lineEnd = line.empty() ? 0 : line.size() - 1;		
		// size_t lineEnd = line.length();												// returns weird number for some reason if string length is zero

		std::vector<IniParser::Token*>* tokens = new std::vector<IniParser::Token*>;	// (#D3)

		// comment remover
		auto CommentRemover = [&line, &lineEnd]() -> void {
			for (size_t i = line.find(';'); i != line.npos; i = line.substr(i + 1, line.size() - (i + 1)).find(';')) {
				if (i == 0) {
					lineEnd = 0;
					break;
				}
			}
			for (size_t i = line.find('#'); i != line.npos; i = line.substr(i + 1, line.size() - (i + 1)).find('#')) {
				if (i == 0) {
					lineEnd = 0;
					break;
				}
			}
		};

		CommentRemover();

		for (size_t i = 0; i < lineEnd; ++i) {
			// Section
			if (line[0] == '[' && line[lineEnd] == ']') {
				++tokenBegin;
				tokens->push_back(new IniParser::Token{ IniParser::TokenType::Section,line.substr(tokenBegin, lineEnd - 1) });	// Add Section 
				break; 
			} 

			// KeyValue
			else if (line[i] == '=' && !openSection) {

				if (i == 0) {	// Check for Key
					throw std::exception("Key Must Be Present!");
				}

				else if (i == lineEnd) {	// Check for Value 
					throw std::exception("Value Must Be Present!");
				}

				else {	// Add Key Value
					tokens->push_back(new Token{ IniParser::TokenType::Key,
						HawtLib::Text::Trim(line.substr(tokenBegin, i)) });							// Add Key

					tokenBegin = i + 1;																// Skip equals

					tokens->push_back(new Token{ IniParser::TokenType::Value,
						HawtLib::Text::Trim(line.substr(tokenBegin, lineEnd - tokenBegin + 1)) });	// Add Value
				}
			}
		}
		return tokens;
	}

	void IniParser::_Parse(IniFile* iniFile, std::vector<std::vector<IniParser::Token*>>& allTokens) {
		// loop through each line of tokens

		std::string currentSection = "";

		for (std::vector<IniParser::Token*>& lineTokens : allTokens) {
			if (lineTokens[0]->type == IniParser::TokenType::Section) {	// Section
				currentSection = lineTokens[0]->data;
				iniFile->CreateSection(lineTokens[0]->data);
			}
			else {	// Key Value
				iniFile->AddKeyValue(currentSection, lineTokens[0]->data, lineTokens[1]->data);	// Add Key Value
			}
		}
		_CleanTokens(allTokens);
	}

	void IniParser::_CleanTokens(std::vector<std::vector<IniParser::Token*>>& allTokens) {
		for (size_t y = 0; y < allTokens.size(); ++y) {
			for (size_t x = 0; x < allTokens[y].size(); ++x) {
				delete allTokens[y][x];
			}
		}
	}
}
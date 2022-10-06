#include "TextFormatting.h"

namespace HawtLib {
	namespace Text {

		// Text Alignments
		std::string CenterText(const std::string& inStr, size_t space) {
			if (space < inStr.size()) {
				printf("space must be atleast euqal to str size");
				__debugbreak();
			}
			 return std::string((space - inStr.size()) / 2, ' ') 
				 + inStr + std::string(space - inStr.size() - ((space - inStr.size()) / 2), ' ');
		}

		std::string RightText(const std::string& inStr, size_t space) {
			if (space < inStr.size()) {
				printf("space must be atleast euqal to str size");
				__debugbreak();
			}
			return std::string(space - inStr.size(), ' ') + inStr;
		}


		// Text Casings
		std::string ToLower(const std::string& inStr) {
			std::string outStr = "";
			for (char c : inStr) {
				if (c <= 90 && c >= 65) c += 32;
				outStr += c;
			}
			return outStr;
		}

		char ToLower(char inC) {
			if (inC <= 90 && inC >= 65) return inC += 32;
			return inC;
		}

		std::string ToUpper(const std::string& inStr) {
			std::string outStr;
			for (char c : inStr) {
				if (c <= 122 && c >= 97) c -= 32;
				outStr += c;
			}
			return outStr;
		}

		char ToUpper(char inC) {
			if (inC <= 122 && inC >= 97) return inC -= 32;
			return inC;
		}

		std::string Title(const std::string& inStr) {
			std::string outStr = "";
			std::vector<std::string> words = SplitString(inStr);
			for (auto& word : words) {
				word[0] = ToUpper(word[0]);
				outStr += word;
			}
			return outStr;
		}


		// Utilities

		std::vector<std::string> SplitString(const std::string& inStr, char delim) {
			std::vector<std::string> outVec;
			std::string word = "";
			for (char c : inStr) {
				if (c != delim) word += c;
				else {
					outVec.emplace_back(word);
					word.clear();
				}
			}
			if (!word.empty()) {
				outVec.emplace_back(word);
			}
			return outVec;
		}
	
		std::string TrimLead(const std::string& str) {
			std::string outStr = str;
			for (size_t i = 0; i < str.size(); ++i) {
				if (str[i] != ' ') {
					outStr.erase(0, i);
					return outStr;
				}
			}
			return str;
		}
		std::string TrimTrail(const std::string& str) {
			std::string outStr = str;
			for (size_t i = 0; i < str.size(); ++i) {
				if (str[str.size() - (i + 1)] != ' ') {
					outStr.erase(str.size() - i, i);
					return outStr;
				}
			}
			return str;
		}
		std::string Trim(const std::string& str) {
			return TrimTrail(TrimLead(str));
		}
	}
}

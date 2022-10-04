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
#include <unordered_map>

#include "../KeyValue.h"



namespace HawtLib {
	namespace File {
		namespace Persistence {
			void IniSave(const std::string& file);
		}

		namespace Parsing
		{
			class IniParser;
		}

		struct SPEC Section {
			std::string name;
			std::vector<KeyValue<std::string, std::string>*> keyValues;	//(#D2)

			~Section();
		};

		class SPEC IniFile {
		private:
			std::unordered_map<std::string, Section*> m_Sections;	// (#D1)
			std::vector<std::string*> m_SectionNames;

		private:
			void _CleanUp();

		public:
			IniFile() = default;
			IniFile(const std::string&);

			// Copy
			IniFile(IniFile& other);
			IniFile& operator=(IniFile& other);

			// Move
			IniFile(IniFile&& other) noexcept;
			IniFile& operator=(IniFile&& other) noexcept;

			~IniFile();

		public:
			IniFile& CreateSection(const std::string& name);
			IniFile& AddKeyValue(const std::string& sectionName, const std::string& key, const std::string& value);
			std::vector<std::string*> GetSectionNames();
			Section* GetSectionKV(const std::string& sectionName);
			void Save(const std::string& file);
			friend class ::HawtLib::File::Parsing::IniParser;
		};
	}

	
}
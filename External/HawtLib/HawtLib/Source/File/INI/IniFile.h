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
#include <memory>

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

		class SPEC IniFile {
		public:
			struct SPEC Section {
				std::vector<KeyValue<std::string, std::string>*> keyValues;
				~Section();
			};

			typedef std::unordered_map<std::string, std::shared_ptr<Section>> Sections;

		private:
			Sections m_Sections;

		public:
			IniFile() = default;
			IniFile(const std::string&);

			// Copy
			IniFile(IniFile& other);
			IniFile& operator=(const IniFile& other);

			// Move
			IniFile(IniFile&& other) noexcept;
			IniFile& operator=(IniFile&& other) noexcept;

			~IniFile() = default;

		public:
			IniFile& CreateSection(const std::string& name);
			IniFile& AddKeyValue(const std::string& sectionName, const std::string& key, const std::string& value);
			std::vector<std::string> GetSectionNames();
			std::shared_ptr<Section> GetSectionKV(const std::string& sectionName);
			std::shared_ptr<Section> GetSectionKV(const std::string& sectionName) const;
			void Save(const std::string& file);
			friend class ::HawtLib::File::Parsing::IniParser;
		};
	}

	
}
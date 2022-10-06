#include <fstream>

#include "fmt/format.h"

#include "IniFile.h"
#include "Parsing/IniParser.h"
#include "Persistence/IniSave.h"

namespace HawtLib {
	namespace File {
		IniFile::Section::~Section() {
			for (const KeyValue<std::string, std::string>* kv : keyValues) {
				delete kv;
			}
		}

		IniFile::IniFile(const std::string& fileName) {
			Parsing::IniParser::Get().Read(this, fileName);
		}

		// Copy
		IniFile::IniFile(IniFile& other) {
			for (auto& otherSections: other.m_Sections)
			{
				this->CreateSection(otherSections.first);
				for (const KeyValue<std::string, std::string>* kv : other.GetSectionKV(otherSections.first)->keyValues)
				{
					this->AddKeyValue(otherSections.first, kv->key, kv->value);
				}
			}
		}
		IniFile& IniFile::operator=(const IniFile& other) {

			for (auto& otherSections : other.m_Sections) {
				this->CreateSection(otherSections.first);
				for (const KeyValue<std::string, std::string>* kv : other.GetSectionKV(otherSections.first)->keyValues) {
					this->AddKeyValue(otherSections.first, kv->key, kv->value);
				}
			}

			return *this;
		}

		// Move
		IniFile::IniFile(IniFile&& other) noexcept {
			for (auto& otherSections : other.m_Sections)
			{
				this->m_Sections[otherSections.first] = std::move(otherSections.second);
			}
		}
		IniFile& IniFile::operator=(IniFile&& other) noexcept {
			for (auto& otherSections : other.m_Sections) {
				this->m_Sections[otherSections.first] = std::move(otherSections.second);
			}
			return *this;
		}


		IniFile& IniFile::CreateSection(const std::string& name) {
			m_Sections[name] = std::make_shared<Section>();
			return *this;
		}

		IniFile& IniFile::AddKeyValue(const std::string& sectionName, const std::string& key, const std::string& value) {
			m_Sections.at(sectionName)->keyValues.push_back(new KeyValue{ key, value });
			return *this;
		}

		std::vector<std::string> IniFile::GetSectionNames() {
			std::vector<std::string> names;
			names.reserve(m_Sections.size());
			for (auto& kv: m_Sections)
			{
				names.push_back(kv.first);
			}
			return names;
		}

		std::shared_ptr<IniFile::Section> IniFile::GetSectionKV(const std::string& sectionName) {
			return m_Sections.at(sectionName);
		}

		std::shared_ptr<IniFile::Section> IniFile::GetSectionKV(const std::string& sectionName) const {
			return m_Sections.at(sectionName);
		}

		void IniFile::Save(const std::string& file) {
			Persistence::IniSave(*this, file);
		}
	}
}
			


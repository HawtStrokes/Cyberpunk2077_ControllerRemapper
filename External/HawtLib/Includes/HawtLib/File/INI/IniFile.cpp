#include <fstream>

#include "fmt/format.h"

#include "IniFile.h"
#include "Parsing/IniParser.h"
#include "Persistence/IniSave.h"

namespace HawtLib {
	namespace File {
		// delete key value pairs (keyValue) (#D1)
		void IniFile::_CleanUp() {
			for (std::string* sectionName : m_SectionNames) {
				delete m_Sections[*sectionName];	// delete section
			}
		}

		// delete section pointers (m_Sections) (#D2)
		Section::~Section() {
			for (KeyValue<std::string, std::string>* kv : keyValues) {
				delete kv;
			}
		}
		IniFile::~IniFile() {
			_CleanUp();
		}



		IniFile::IniFile(const std::string& fileName) {
			Parsing::IniParser::Get().Read(this, fileName);
		}

		// Copy
		IniFile::IniFile(IniFile& other) {
			for (std::string* sectionName : other.m_SectionNames) {
				this->CreateSection(*sectionName);
				for (KeyValue<std::string, std::string>* kv : other.GetSectionKV(*sectionName)->keyValues) {
					this->AddKeyValue(*sectionName, kv->key, kv->value);
				}
			}
		}
		IniFile& IniFile::operator=(IniFile& other) {
			for (std::string* sectionName : other.m_SectionNames) {
				this->CreateSection(*sectionName);
				for (KeyValue<std::string, std::string>* kv : other.GetSectionKV(*sectionName)->keyValues) {
					this->AddKeyValue(*sectionName, kv->key, kv->value);
				}
			}
			return *this;
		}

		// Move

		IniFile::IniFile(IniFile&& other) noexcept {
			this->m_SectionNames = other.m_SectionNames;
			this->m_Sections = other.m_Sections;

			for (std::string* sectionName : other.m_SectionNames) {
				sectionName = nullptr;
				for (KeyValue<std::string, std::string>* kv : other.GetSectionKV(*sectionName)->keyValues) {
					kv = nullptr;
				}
			}
		}
		IniFile& IniFile::operator=(IniFile&& other) noexcept {
			if (this != &other) {
				this->_CleanUp();
				this->m_SectionNames = other.m_SectionNames;
				this->m_Sections = other.m_Sections;

				for (std::string* sectionName : other.m_SectionNames) {
					sectionName = nullptr;
					for (KeyValue<std::string, std::string>* kv : other.GetSectionKV(*sectionName)->keyValues) {
						kv = nullptr;
					}
				}
				return *this;
			}
			return *this;
		}


		IniFile& IniFile::CreateSection(const std::string& name) {
			m_Sections[name] = new Section{ name };
			m_SectionNames.push_back(&m_Sections[name]->name);
			return *this;
		}

		IniFile& IniFile::AddKeyValue(const std::string& sectionName, const std::string& key, const std::string& value) {
			KeyValue<std::string, std::string>* kv = new KeyValue<std::string, std::string>{ key, value }; // (#D1)
			m_Sections[sectionName]->keyValues.push_back(kv);
			return *this;
		}

		std::vector<std::string*> IniFile::GetSectionNames() {
			return m_SectionNames;
		}

		Section* IniFile::GetSectionKV(const std::string& sectionName) {
			return m_Sections[sectionName];
		}
		void IniFile::Save(const std::string& file) {
			Persistence::IniSave(*this, file);
		}
	}
}
			


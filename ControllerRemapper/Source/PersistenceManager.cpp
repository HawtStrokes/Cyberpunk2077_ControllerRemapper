#include <filesystem>

#include "ControllerMapper.h"
#include "PersistenceManager.h"
#include "ButtonSingle.h"
#include "ButtonCombo.h"
#include "Action.h"
#include "Exceptions.h"

#include "HawtLib/File/File.h"

// TODO: Update Persistence Manager to Conform to the new XMLName Creation Standard (see IButton.h and Action.h static member functions)
// TODO: Deprecate use of manual XML code/name assignment

namespace ControllerMapper {

	void PersistenceManager::UpdateSaveCount() {
		int saveCount = 0;
		for (auto& entry : std::filesystem::directory_iterator(m_SaveDir)) {
			if (entry.is_directory()) {
				++saveCount;
			}
		}
		m_NumberOfSaves = saveCount;
	}

	bool PersistenceManager::VerifyGameDir(const std::string& gameDir) {
		if (!std::filesystem::exists(gameDir + R"(\REDprelauncher.exe)") &&
			!std::filesystem::exists(gameDir + R"(\bin\x64\Cyberpunk2077.exe)")) return false;

		return true;
	}

	PersistenceManager& PersistenceManager::Get() {
		static PersistenceManager instance;
		return instance;
	}

	void PersistenceManager::Save() {	// Make Ini File
		HawtLib::File::IniFile iniFile;
		auto x = ControllerMapper::Get().m_Binds;
		for (const auto& kv : x) {
			// Use Action integer equivalent as section name
			std::string sectionName = std::to_string(static_cast<unsigned int>(kv.second->GetCharacterAction().actionType));
			const auto btnSPtr = dynamic_cast<ButtonSingle*>(kv.first);
			if (btnSPtr != nullptr) {
				iniFile.AddKeyValue(sectionName, "IsCombo", "false")
					.AddKeyValue(sectionName, "Key", std::to_string(static_cast<unsigned>(btnSPtr->GetKey())));
			}
			else {
				const auto btnCPtr = dynamic_cast<ButtonCombo*>(kv.first);
				iniFile.AddKeyValue(sectionName, "IsCombo", "false")
					.AddKeyValue(sectionName, "Key1", std::to_string(static_cast<unsigned>(btnCPtr->GetKey1())))
					.AddKeyValue(sectionName, "Key2", std::to_string(static_cast<unsigned>(btnCPtr->GetKey2())));
			}
			iniFile.CreateSection(sectionName);
			iniFile.AddKeyValue(std::to_string(static_cast<unsigned int>(kv.second->GetCharacterAction().actionType)),
				"ButtonXMLName",	// Button XML Name
				kv.first->GetXMLName())
				.AddKeyValue(sectionName,
					"Options", std::to_string(kv.second->GetOptions().options));
		}
		UpdateSaveCount();
		/* Sample Ini Output
		 *[1]
		 *IsCombo = true
		 *key1 = 1
		 *key2 = 4
		 *ButtonXMLName = CUSTOM_NAME
		 *Options = 5
		 *[2]
		 *isCombo = false
		 *key = 3
		 *ButtonXMLName = Custom_NaMe
		 *Options = 5
		 */
	}
	
	void PersistenceManager::Load(unsigned saveCount) {
		m_LoadedSave = saveCount;
		// Load Ini File
		HawtLib::File::IniFile iniFile(m_SaveDir + R"(/)" + std::to_string(m_LoadedSave) + R"(save.ini)");
		auto sectionNames = iniFile.GetSectionNames();


		// Loop through each section
		for (auto sectionName : sectionNames) {
			bool isCombo;
			ControllerKey key1 = ControllerKey::None;
			ControllerKey key2 = ControllerKey::None;
			std::string buttonXMLName = "";
			CharacterAction characterAction = static_cast<CharacterAction::ActionType>(std::stoi(*sectionName));
			CharacterOptions characterOption = CharacterOptions(0);

			// Loop through each keyVal in Section and update local vars
			for (auto kV : iniFile.GetSectionKV(*sectionName)->keyValues) {
				if (kV->key == "isCombo") {
					if (kV->value == "true") {
						isCombo = true;
						//dynamic_cast<ButtonCombo*>(button);
					}
					else if (kV->value == "false") {
						isCombo = false;
					}
					else {
						throw ConfigurationCorruptedError();
					}
				}
				else if (kV->key == "key1") {
					key1 = static_cast<ControllerKey>(std::stoi(kV->value));
				}
				else if (kV->key == "key2") {
					key2 = static_cast<ControllerKey>(std::stoi(kV->value));
				}
				else if (kV->key == "ButtonXMLName") {
					buttonXMLName = kV->value;
				}
				else if (kV->key == "Options") {
					characterOption = static_cast<CharacterOptions>(std::stoi(kV->value));
				}
			}

			// After looping through each keyVal, bind an action to a button
			// Also check if configuration loaded is valid
			if (characterOption.options != 0 && buttonXMLName.length() != 0 && (key1 != ControllerKey::None && key2 != ControllerKey::None)) {
				ControllerMapper::Get()._CleanUp();
				ControllerMapper::Get().m_Binds.clear();
				try {

					auto action = Action::BuildActionPtr(characterAction, characterOption);
					if (isCombo == true) {
						auto button = new ButtonCombo(key1, key2);
						ControllerMapper::Get().Bind(button, action);	// Make a bind
					}
					else {
						auto button = new ButtonSingle(key1);
						ControllerMapper::Get().Bind(button, action);	// Make a bind
					}
				}

				catch (InvalidCharacterOptions&) {
					throw ConfigurationCorruptedError();
				}
			}
			else {
				throw ConfigurationCorruptedError();
			}
		}

	}

	void PersistenceManager::Apply(unsigned) {
		ControllerMapper::Get().BuildXML(m_GameDir + R"(\r6\config\)");
	}

	void PersistenceManager::SetGameDir(const std::string& gameDir) {
		if (!VerifyGameDir(gameDir)) throw InvalidGameDir();
		else m_GameDir = gameDir;
	}

	std::string PersistenceManager::GetGameDir() {
		return m_GameDir;
	}

	unsigned PersistenceManager::GetLoadedSave() {
		return m_LoadedSave;
	}

	unsigned PersistenceManager::GetNumberOfSaves() {
		return m_NumberOfSaves;
	}
}


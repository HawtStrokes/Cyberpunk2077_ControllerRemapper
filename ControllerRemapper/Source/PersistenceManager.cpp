#include <filesystem>

#include "ActionButtonBinder.h"
#include "PersistenceManager.h"
#include "ButtonSingle.h"
#include "ButtonCombo.h"
#include "Action.h"
#include "Exceptions.h"

#include "HawtLib/File/File.h"
#include "HawtLib/Text/TextFormatting.h"

///	###Sample INI Config:
///	[(int) ActionType]
///	options = CharacterOptions
///	isCombo = false
///	key = ControllerKey
///
/// [(int) ActionType]
///	options = CharacterOptions
///	isCombo = true
///	key1 = ControllerKey
///	key2 = ConrollerKey
///
///	###How to config input to usable data:
///	BuildAction(CharacterAction(ActionType), options)
///	if(isCombo) ButtonCombo(key1, key2)
///	...
///
///	What we need:
///	ActionType as section (1), KeyValues: options = CharacterOptions, isCombo = true/false, keys...

namespace ControllerMapper {

	PersistenceManager::PersistenceManager() : m_SaveDir("Saves/") {
		UpdateSaveCount();
	}

	void PersistenceManager::UpdateSaveCount() {
		int saveCount = 0;
		for (auto& entry : std::filesystem::directory_iterator(std::filesystem::path(m_SaveDir))) {
			if (entry.is_directory()) {
				++saveCount;
			}
		}
		m_NumberOfSaves = saveCount;
	}

	bool PersistenceManager::Internal_VerifyGameDir(const std::string& gameDir) {
		if (!std::filesystem::exists(gameDir + R"(/REDprelauncher.exe)") &&
			!std::filesystem::exists(gameDir + R"(/bin/x64/Cyberpunk2077.exe)")) return false;

		return true;
	}

	PersistenceManager& PersistenceManager::Get() {
		static PersistenceManager instance;
		return instance;
	}

	// Saves current config as Ini
	void PersistenceManager::Save(const std::string& configName) {	// Make Ini File
		HawtLib::File::IniFile iniFile;
		auto x = ActionButtonBinder::Get().m_Binds;
		for (const auto& kv : x) {
			// Use Action integer equivalent as section name
			std::string sectionName = std::to_string(static_cast<unsigned int>(kv.first->GetCharacterAction().actionType));

			iniFile.CreateSection(sectionName);	// Create the section
			iniFile.AddKeyValue(sectionName,	// Add options
					"options", std::to_string(kv.first->GetOptions().options));

			const auto btnSPtr = dynamic_cast<ButtonSingle*>(kv.second.get());
			if (btnSPtr != nullptr) {
				// add one controllerkey
				iniFile.AddKeyValue(sectionName, "isCombo", "false")
					.AddKeyValue(sectionName, "key", std::to_string(static_cast<unsigned>(btnSPtr->GetKey())));
			}
			else {
				// Add two controllerkeys
				const auto btnCPtr = dynamic_cast<ButtonCombo*>(kv.second.get());
				iniFile.AddKeyValue(sectionName, "isCombo", "false")
					.AddKeyValue(sectionName, "key1", std::to_string(static_cast<unsigned>(btnCPtr->GetKey1())))
					.AddKeyValue(sectionName, "key2", std::to_string(static_cast<unsigned>(btnCPtr->GetKey2())));
			}
		}
		const std::string outPutDir = m_SaveDir + "/" + configName;


		if (!std::filesystem::exists(std::filesystem::path(outPutDir)))
		{
			auto vecDir = HawtLib::Text::SplitString(outPutDir, '/');
			std::string buffer;
			for (auto& dir : vecDir)
			{
				buffer += dir + "/";
				std::filesystem::create_directory(std::filesystem::path(buffer));
			}
		}
			

		iniFile.Save(outPutDir + "/config.ini");	// save the ini File in $(SaveDir)\$(saveCount)\config.ini
		// UpdateSaveCount();	// update save count
	}

	// Loads a configuratin ini and update Binds
	void PersistenceManager::Load(const std::string& configName) {
		std::string outPutDir = m_SaveDir + "/" + configName + "/";
		if (!std::filesystem::exists(std::filesystem::path(outPutDir)) && !std::filesystem::exists(std::filesystem::path(outPutDir + "/config.ini"))) {
			throw ConfigurationDoesNotExist();
		}

		// Load Ini File
		HawtLib::File::IniFile iniFile(outPutDir + "config.ini");
		const auto sectionNames = iniFile.GetSectionNames();

		//ActionButtonBinder::Get()._CleanUp();		// delete heap allocated stuff
		ActionButtonBinder::Get().m_Binds.clear();	// clear the map

		// Loop through each section
		for (const auto sectionName : sectionNames) {
			bool isCombo = false;
			struct
			{
				union
				{
					ControllerKey key1;
					ControllerKey key;
				};
				ControllerKey key2;
			} keys = { {ControllerKey::None}, ControllerKey::None };


			CharacterAction characterAction = static_cast<CharacterAction::ActionType>(std::stoi(sectionName));
			CharacterOptions characterOption = CharacterOptions(0);

			// Loop through each keyVal in Section and update local vars
			for (auto kV : iniFile.GetSectionKV(sectionName)->keyValues) {
				if (kV->key == "isCombo") {
					if (kV->value == "true") {
						isCombo = true;
					}
					else if (kV->value == "false") {
						isCombo = false;
					}
					else {
						throw ConfigurationCorruptedError();
					}
				}
				else if (kV->key == "options") {
					characterOption.options = static_cast<unsigned int>(std::stoi(kV->value));
				}
				else if (kV->key == "key1" && isCombo) {
					keys.key1 = static_cast<ControllerKey>(std::stoi(kV->value));
				}
				else if (kV->key == "key2" && isCombo) {
					keys.key2 = static_cast<ControllerKey>(std::stoi(kV->value));
				}
				else if (kV->key == "key" && !isCombo) {
					keys.key = static_cast<ControllerKey>(std::stoi(kV->value));
				}
			}

			// After looping through each keyVal, bind an action to a button
			// Also check if configuration loaded is valid
			if (characterOption.options != 0 && keys.key1 != ControllerKey::None &&
				(isCombo? ControllerKey::None != keys.key2 : ControllerKey::None == keys.key2)) {	// if combo and key2 is not null || if !combo and key2 is null
				try {
					
					if (isCombo == true) {
						//ActionButtonBinder::Get().Bind(Action::BuildActionPtr(characterAction, characterOption), new ButtonCombo(keys.key1, keys.key2));	// Make a bind
						ActionButtonBinder::Get().Bind(Action::BuildActionPtr(characterAction, characterOption), std::make_shared<ButtonCombo>(keys.key1, keys.key2));
					}
					else {
						ActionButtonBinder::Get().Bind(Action::BuildActionPtr(characterAction, characterOption), std::make_shared<ButtonSingle>(keys.key));	// Make a bind
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

	void PersistenceManager::Delete(const std::string& configName) {
		std::filesystem::remove_all(std::filesystem::path(m_SaveDir + "/" + configName + "/"));
	}

	// Applies custom binds to game
	void PersistenceManager::Apply() {
		ActionButtonBinder::Get().BuildXML(m_GameDir + R"(\r6\config\)");
	}

	// Set Game directory
	void PersistenceManager::SetGameDir(const std::string& gameDir) {
		if (!Internal_VerifyGameDir(gameDir)) throw InvalidGameDir();
		else m_GameDir = gameDir + "/";
	}

	void PersistenceManager::SetSaveDir(const std::string& saveDir) {
		m_SaveDir = saveDir + "/";
	}

	void PersistenceManager::SetLoadedSave(const std::string& configName) {
		m_LoadedSave = configName;
	}

	// Get Game directory
	std::string PersistenceManager::GetGameDir() {
		return m_GameDir;
	}

	// Get loaded saveCount
	std::string PersistenceManager::GetLoadedSave() {
		return m_LoadedSave;
	}

	// Get number of saves
	unsigned PersistenceManager::GetNumberOfSaves() {
		return m_NumberOfSaves;
	}
}


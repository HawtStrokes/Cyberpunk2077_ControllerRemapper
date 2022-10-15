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


#include <filesystem>

#include "ActionButtonBinder.h"
#include "PersistenceManager.h"
#include "ButtonSingle.h"
#include "ButtonCombo.h"
#include "Action.h"
#include "Exceptions.h"
#include "../Util/PlatformUtil.h"

#include "HawtLib/File/File.h"

namespace ControllerMapper {


	// Private Ctor
	PersistenceManager::PersistenceManager() : m_SaveDir("Saves"), m_NumberOfSaves(0) {
		const auto gamePath = Util::GetGamePath();
		if (gamePath.has_value())
		{
			m_GameDir = gamePath.value();
		}
		UpdateSaveCount();
	}

	// Return Static Instance
	PersistenceManager& PersistenceManager::Get() {
		static PersistenceManager instance;
		return instance;
	}

	// Updates save counts
	void PersistenceManager::UpdateSaveCount() {
		m_NumberOfSaves = 0;
		Internal_CheckMakeDir(m_SaveDir);

		for (auto& entry : std::filesystem::directory_iterator(std::filesystem::path(m_SaveDir))) {
			if (entry.is_directory()) {
				++m_NumberOfSaves;
			}
		}
	}


	///
	///	INTERNALS
	///

	// Throw if string is empty
	inline void PersistenceManager::Internal_ThrowEmpty(const std::string& str)
	{
		if (str.empty()) throw std::runtime_error("Input Must Not Be Empty");
	}

	// Throw if config is empty or has folders
	inline void PersistenceManager::Internal_ThrowCheckConfig(const std::string& configName) {
		Internal_CheckIllegals(configName);
		Internal_ThrowEmpty(configName);
		if (Internal_IsDir(configName)) throw DirectoryIsDisallowed();
	}

	// Throws if contains illegal characters
	inline void PersistenceManager::Internal_CheckIllegals(const std::string& str)
	{
		constexpr char illegalCharacters[] = {
		'<',
		'>',
		':',
		'"',
			'.',	// Not really illegal unless used alone, just disable for safety
		'/',
		'\\',
		'|',
		'?',
		'*'
		};
		for (const char c : illegalCharacters)
		{
			if (str.find(c) != std::string::npos)
				throw std::runtime_error("File contains Invalid Characters!");
		}
	}

	// Shallowly checks if dir is indeed cp2077 dir
	inline bool PersistenceManager::Internal_VerifyGameDir(const std::string& gameDir) {
		if (!std::filesystem::exists(gameDir + R"(/REDprelauncher.exe)") &&
			!std::filesystem::exists(gameDir + R"(/bin/x64/Cyberpunk2077.exe)")) return false;

		return true;
	}

	// Creates dirs if they don't exist
	inline void PersistenceManager::Internal_CheckMakeDir(const std::string& path) {
		if (!std::filesystem::exists(std::filesystem::path(path))) {
			std::filesystem::create_directories(std::filesystem::path(path));
		}
	}

	// Checks if param is a dir
	inline bool PersistenceManager::Internal_IsDir(const std::string& path) {
		if (path.find('\\') != std::string::npos || path.find('/') != std::string::npos) return true;
		return false;
	}


	///
	///	Core Persistence Functionalities
	///

	// Saves current config as Ini
	void PersistenceManager::Save(const std::string& configName) {	// Make Ini File
		Internal_ThrowCheckConfig(configName);
		HawtLib::File::IniFile iniFile;
		for (const auto& kv : ActionButtonBinder::Get().m_Binds) {
			// Use Action integer equivalent as section name
			std::string sectionName = std::to_string(static_cast<unsigned int>(kv.first->GetCharacterAction().actionType));

			iniFile.CreateSection(sectionName);	// Create the section
			iniFile.AddKeyValue(sectionName,	// Add options
					"options", std::to_string(kv.first->GetCharacterOptions().options));

			const auto btnSPtr = dynamic_cast<ButtonSingle*>(kv.second.get());
			if (btnSPtr != nullptr) {
				// add one controllerkey
				iniFile.AddKeyValue(sectionName, "isCombo", "false")
					.AddKeyValue(sectionName, "key", std::to_string(static_cast<unsigned>(btnSPtr->GetKey())));
			}
			else {
				// Add two controllerkeys
				const auto btnCPtr = dynamic_cast<ButtonCombo*>(kv.second.get());
				iniFile.AddKeyValue(sectionName, "isCombo", "true")
					.AddKeyValue(sectionName, "key1", std::to_string(static_cast<unsigned>(btnCPtr->GetKey1())))
					.AddKeyValue(sectionName, "key2", std::to_string(static_cast<unsigned>(btnCPtr->GetKey2())));
			}
		}

		const std::string outPutDir = m_SaveDir + "/" + configName + "/";
		Internal_CheckMakeDir(outPutDir);
		iniFile.Save(outPutDir + "/config.ini");	// save the ini File in $(SaveDir)\$(saveCount)\config.ini
		UpdateSaveCount();	// update save count
	}

	// Loads a configuration ini and update Binds
	void PersistenceManager::Load(const std::string& configName) {
		Internal_ThrowCheckConfig(configName);

		const std::string outPutDir = m_SaveDir + "/" + configName + "/";
		if (!std::filesystem::exists(std::filesystem::path(outPutDir + "/config.ini"))) {
			throw ConfigurationDoesNotExist();
		}

		// Load Ini File
		HawtLib::File::IniFile iniFile(outPutDir + "config.ini");
		const auto sectionNames = iniFile.GetSectionNames();

		//ActionButtonBinder::Get().Internal_CleanUp();		// delete heap allocated stuff
		// ActionButtonBinder::Get().m_Binds.clear();	// clear the map

		// Loop through each section
		for (const auto& sectionName : sectionNames) {
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


			const CharacterAction characterAction = static_cast<CharacterAction::ActionType>(std::stoi(sectionName));
			CharacterOptions characterOption = CharacterOptions(0);

			// Loop through each keyVal in Section and update local vars
			for (const auto kV : iniFile.GetSectionKV(sectionName)->keyValues) {
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
				else if (kV->key == "key1") {
					keys.key1 = static_cast<ControllerKey>(std::stoi(kV->value));
				}
				else if (kV->key == "key2") {
					keys.key2 = static_cast<ControllerKey>(std::stoi(kV->value));
				}
				else if (kV->key == "key") {
					keys.key = static_cast<ControllerKey>(std::stoi(kV->value));
				}
			}

			// After looping through each keyVal, bind an action to a button
		
			try {
				
				if (isCombo == true) {
					ActionButtonBinder::Get().Bind(Action::GetActionPtr(characterAction, characterOption), std::make_shared<ButtonCombo>(keys.key1, keys.key2));
				}
				else {
					ActionButtonBinder::Get().Bind(Action::GetActionPtr(characterAction, characterOption), std::make_shared<ButtonSingle>(keys.key));	// Make a bind
				}
			}

			catch (InvalidCharacterOptions&) {
				throw ConfigurationCorruptedError();
			}
		}

		Internal_SetLoadedSave(configName);
	}

	// Deletes configuration
	void PersistenceManager::Delete(const std::string& configName) {
		Internal_ThrowCheckConfig(configName);
		std::filesystem::remove_all(std::filesystem::path(m_SaveDir + "/" + configName));
		UpdateSaveCount();
	}

	// Applies custom binds to game
	void PersistenceManager::Apply() {
		ActionButtonBinder::Get().BuildXML(m_GameDir + R"(\r6\config\)");
	}


	///
	///	SETTERS
	///

	// Set Game directory
	void PersistenceManager::SetGameDir(const std::string& gameDir) {
		const auto gamePath = std::filesystem::path(gameDir);
		if (!std::filesystem::exists(gamePath)) {
			if (!std::filesystem::is_directory(gamePath))
				throw InvalidGameDir("Must be an existing Directory!");
			throw InvalidGameDir("Must be an existing Directory!");
		}
		if (!Internal_VerifyGameDir(gameDir))
		{
			throw InvalidGameDir("Directory doesn't contain binary executables associated with Cyberpunk 2077");
		}
			

		m_GameDir = gameDir;
	}

	// Set Save directory
	void PersistenceManager::SetSaveDir(const std::string& saveDir) {
		Internal_ThrowEmpty(saveDir);
		m_SaveDir = saveDir;
	}

	// Set Loaded Save
	void PersistenceManager::Internal_SetLoadedSave(const std::string& configName) {
		if (Internal_IsDir(configName)) throw DirectoryIsDisallowed();
		m_LoadedSave = configName;
	}


	///
	///	GETTERS
	///

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

	// returns vector of all config names in saveDir
	std::vector<std::string> PersistenceManager::GetSaves()
	{
		Internal_CheckMakeDir(m_SaveDir);
		std::vector<std::string> retVec;
		for (auto& entry : std::filesystem::directory_iterator(std::filesystem::path(m_SaveDir))) {
			if (entry.is_directory()) {
				std::string strDir = entry.path().string();

				// if all later checks fail, default is the initialized 0
				size_t beginIdx = 0;
				size_t fwdSlashIdx = strDir.rfind('/');
				size_t bwdSlashIdx = strDir.rfind('\\');

				// check if fwd is npos
				if (fwdSlashIdx != std::string::npos) {
					// check if bwd is npos
					if (bwdSlashIdx != std::string::npos) {
						beginIdx = std::max(fwdSlashIdx + 1, bwdSlashIdx + 1);	// get bigger number of the two
					}
					else {
						beginIdx = fwdSlashIdx + 1;	// else set idx to the defined
					}
				}

				// else check if bwd is npos
				else if (bwdSlashIdx != std::string::npos)
					beginIdx = bwdSlashIdx + 1;


				retVec.emplace_back(strDir.substr(beginIdx, strDir.size() - beginIdx));
			}
		}
		return retVec;
	}
}


#pragma once
#include <string>

namespace ControllerMapper
{
	class PersistenceManager
	{
	private:
		// Exception Throwers
		inline void Internal_ThrowEmpty(const std::string&);
		inline void Internal_ThrowCheckConfig(const std::string&);

		// Verifiers
		inline bool Internal_VerifyGameDir(const std::string&);
		inline bool Internal_IsDir(const std::string&);

		inline void Internal_CheckMakeDir(const std::string&);

	private:
		std::string m_GameDir;
		std::string m_SaveDir;
		std::string m_LoadedSave;
		unsigned int m_NumberOfSaves;

		// private Ctor
		PersistenceManager();

	public:
		static PersistenceManager& Get();

		// Save current config as Ini in SaveDir + configName/config.ini
		void Save(const std::string& configName);

		// Loads Ini from SaveDir + configName/config.ini
		void Load(const std::string& configName);

		// Deletes Config Ini in SaveDir + configName/config.ini
		void Delete(const std::string& configName);

		// Apply Loaded Config
		void Apply();



		// Update SaveCount (refresh)
		void UpdateSaveCount();



		// Set Game Dir
		void SetGameDir(const std::string& gameDir);

		// Set Save Dir
		void SetSaveDir(const std::string& saveDir);

		// Set Loaded Dir
		void SetLoadedSave(const std::string& configName);

		// Get Game Dir
		std::string GetGameDir();

		// Get Loaded configName
		std::string GetLoadedSave();

		// Return number of Saves
		unsigned int GetNumberOfSaves();
	};
}
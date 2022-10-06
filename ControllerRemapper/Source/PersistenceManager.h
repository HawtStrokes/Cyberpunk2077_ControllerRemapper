#pragma once
#include <string>

namespace ControllerMapper
{
	struct
	{
		Action* actionPtr;
		IButton* buttonPtr;
	} pointerStorage;

	class PersistenceManager
	{
	private:
		bool Internal_VerifyGameDir(const std::string&);

	private:
		std::string m_GameDir;
		std::string m_SaveDir;
		std::string m_LoadedSave;
		unsigned int m_NumberOfSaves;

		PersistenceManager();

	public:
		static PersistenceManager& Get();

		void Save(const std::string& configName);
		void Load(const std::string& configName);
		void Delete(const std::string& configName);

		void Apply();
		void UpdateSaveCount();

		void SetGameDir(const std::string& gameDir);
		void SetSaveDir(const std::string& saveDir);
		void SetLoadedSave(const std::string& configName);

		std::string GetGameDir();
		std::string GetLoadedSave();
		unsigned int GetNumberOfSaves();
	};
}
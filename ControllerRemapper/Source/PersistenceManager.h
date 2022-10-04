#pragma once
#include <string>

namespace ControllerMapper
{
	class PersistenceManager
	{
	private:
		std::string m_GameDir;
		std::string m_SaveDir;
		unsigned int m_LoadedSave;
		unsigned int m_NumberOfSaves;

		PersistenceManager() = default;

	public:
		static PersistenceManager& Get();

		void Save();
		void Load(unsigned);
		void Apply(unsigned);
		void UpdateSaveCount();

		void SetGameDir(const std::string& gameDir);
		bool VerifyGameDir(const std::string&);
		std::string GetGameDir();
		unsigned int GetLoadedSave();
		unsigned int GetNumberOfSaves();
	};
}
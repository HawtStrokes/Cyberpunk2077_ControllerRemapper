#include "Action.h"
#include "ActionButtonBinder.h"
#include "ButtonSingle.h"
#include "ButtonCombo.h"
#include "PersistenceManager.h"

#ifdef GUI

	// Add WXWidgets Code here

#else
#include <iostream>

using namespace ControllerMapper;


void Test()
{
	std::string testDir;
	PersistenceManager::Get().SetGameDir(R"(C:\Users\hawts\Desktop\TestSite)");
	std::cout << PersistenceManager::Get().GetGameDir() << std::endl;
	CharacterOptions characterOptions = { OPTIONS_CALLCAR };
	CharacterAction characterAction = CharacterAction::ActionType::CallCar;

	const auto btnPtr = std::make_shared<ButtonSingle>(ButtonSingle(ControllerKey::A_CROSS));

	ActionButtonBinder::Get().Bind(Action::BuildActionPtr(characterAction, characterOptions), btnPtr);
	ActionButtonBinder::Get().Bind(Action::BuildActionPtr(characterAction, characterOptions), btnPtr);
	PersistenceManager::Get().SetSaveDir("Saves/");
	PersistenceManager::Get().Save("NewSave");
	try
	{
		PersistenceManager::Get().Load("2");
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}
	
	auto x = ActionButtonBinder::Get().GetBinds();
	for(auto& kv : x)
	{
		std::cout << kv.first << std::endl << kv.second << std::endl << std::endl;
	}
	ActionButtonBinder::Get().BuildXML("");
	std::cin.get();
}

int main()
{
	Test();
	std::cin.get();
}

#endif
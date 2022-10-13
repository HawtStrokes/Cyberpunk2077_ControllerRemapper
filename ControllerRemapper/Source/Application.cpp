#include <vector>

#include "Core/Action.h"
#include "Core/ActionButtonBinder.h"
#include "Core/ButtonSingle.h"
#include "Core/ButtonCombo.h"
#include "Core/PersistenceManager.h"

#include "HawtLib/Text/TextFormatting.h"


using namespace ControllerMapper;


#ifdef GUI

	// Add WXWidgets Code here

#else
#include <iostream>

#include  "HawtLib/Table/Table.h"


// Outputs text, gets userInput, and returns vector of splitted string
static std::vector<std::string> ConsoleInteractiveWriteLine(const std::string& text, std::string& userInput, char delim = ' ')
{
	std::cout << text << std::endl;
	std::getline(std::cin, userInput);
	return HawtLib::Text::SplitString(userInput, delim);
}

static std::vector<std::string> ConsoleInteractiveWrite(const std::string& text, std::string& userInput, char delim = ' ') {
	std::cout << text;
	std::getline(std::cin, userInput);
	return HawtLib::Text::SplitString(userInput, delim);
}

static void MapButtonWizard(ActionButtonBinder& abbInstance) {
	std::cout << HawtLib::Text::CenterText("--------------------------") << std::endl;
	std::cout << HawtLib::Text::CenterText("Map Button Wizard") << std::endl;
	std::cout << HawtLib::Text::CenterText("--------------------------") << std::endl;

	// Create Table
	HawtLib::Tables::Table table;
	table.AddCell("INDEX", 0)
		.AddCell("ACTION", 0)
		.AddCell("BOUND_BUTTON", 0)
		.AddCell("MODE", 0);

	std::unordered_map<size_t, CharacterAction::ActionType> actionIndexMap;
	{
		auto& binds = abbInstance.GetBinds();
		size_t idx = 1;
		for (const auto& actionButton : binds) {
			table.AddCell(std::to_string(idx), idx);
			table.AddCell(actionButton.first->GetXMLActionName(), idx);
			table.AddCell(actionButton.second->GetXMLName(), idx);
			actionIndexMap[idx] = actionButton.first->GetCharacterAction().actionType;
			std::string modeEntry;
			if (Action::IsToggle(actionButton.first->GetCharacterOptions()))
				modeEntry += "Toggle_";
			else
				modeEntry += "Hold_";


			if (Action::IsDoubleTap(actionButton.first->GetCharacterOptions()))
				modeEntry += "Dtap";
			else
				modeEntry += "Stap";

			table.AddCell(modeEntry, idx);

			++idx;
		}
	}
	table.PrintTable();
	auto Help = []() {
		std::cout << std::string(
			R"(---------------------
Map Button Wizard
------Help Text------
help		=>		Show help text
showtable	=>		Show summary table
bind		=>		Bind Action to Button
exit		=>		Exit Wizard)") << std::endl;
	};
		
	Help();
	bool run = true;
	while (run) 
	{
		std::string userInput;
		std::vector<std::string> vecArgs = ConsoleInteractiveWrite(">>>", userInput);

		if (vecArgs.size() != 1)
		{
			Help();
			continue;
		}

		if (userInput == "exit")
			run = false;

		else if (userInput == "showtable") {
			table.PrintTable();
		}

		else if (userInput == "help") {
			Help();
		}

		else if (userInput == "bind") {
			try
			{
				table.PrintTable();
				vecArgs = ConsoleInteractiveWrite("Type index of action you wish to bind: ", userInput);
				size_t actionIdx = std::stoul(userInput);
				Action* action = Action::GetActionPtr(actionIndexMap.at(actionIdx));

				// Options Setup
				unsigned supportedOptions = action->GetCharacterAction().supportedOptions;
				std::cout << "Supported Options:" << std::endl;
				if ((supportedOptions | DTAP) == supportedOptions)
					std::cout << "1 - DoubleTap" << std::endl;
				
				if ((supportedOptions | STAP) == supportedOptions) 
					std::cout << "2 - SingleTap" << std::endl;
				
				if ((supportedOptions | HOLD) == supportedOptions) 
					std::cout << "3 - HoldMode" << std::endl;
				
				if ((supportedOptions | TOGGLE) == supportedOptions) 
					std::cout << "4 - ToggleMode" << std::endl;



				vecArgs = ConsoleInteractiveWrite("Type in the indices of each option you wish to enable delimited with a space: ", userInput);
				unsigned inputOptions = 0;

				for (const auto& arg : vecArgs)
				{
					switch (std::stoul(arg))
					{
					case 1:
						inputOptions |= DTAP;
						break;
					case 2:
						inputOptions |= STAP;
						break;
					case 3:
						inputOptions |= HOLD;
						break;
					case 4:
						inputOptions |= TOGGLE;
						break;
					}
				}

				action = Action::GetActionPtr(CharacterAction(actionIndexMap.at(actionIdx)), CharacterOptions(inputOptions));

				// Button Setup
				HawtLib::Tables::Table buttonTable;
				buttonTable.AddCell("INDICES", 0).
					AddCell("CONTROLLER KEYS", 0);
				for (size_t i = static_cast<size_t>(ControllerKey::None); i < static_cast<size_t>(ControllerKey::DigitRight); ++i)
				{
					buttonTable.AddCell(std::to_string(i), i + 1);
					buttonTable.AddCell(GetControllerKeyXML(static_cast<ControllerKey>(i)), i + 1);
				}
				buttonTable.PrintTable();
				vecArgs = ConsoleInteractiveWrite("Type in index/indices of Controller Key/s (maximum of two) you wish to bind delimited with a space: ", userInput);
				std::shared_ptr<IButton> button;
				if (vecArgs.size() == 1)
				{
					button = std::make_shared<ButtonSingle>(ButtonSingle(static_cast<ControllerKey>(std::stoi(vecArgs[0]))));
				}
				else if (vecArgs.size() == 2)
				{
					button = std::make_shared<ButtonCombo>(ButtonCombo(static_cast<ControllerKey>(std::stoi(vecArgs[0])),
																		static_cast<ControllerKey>(std::stoi(vecArgs[1]))));
				}
				else
				{
					throw std::runtime_error("Only One or Two Controller Keys Can Be Bound Per Action!");
				}

				// Make Bind
				abbInstance.Bind(Action::GetActionPtr(action->GetCharacterAction(), CharacterOptions(inputOptions)), button);

			}
			catch(std::invalid_argument& e)
			{
				std::cout << "Please Type Index!" << std::endl;
			}

			catch (std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}

		else
		{
			Help();
		}
	}

}

static void Summary(ActionButtonBinder& abbInstance)
{
	// Create Table
	HawtLib::Tables::Table table;
	table.AddCell("ACTION", 0)
		.AddCell("BUTTON", 0)
		.AddCell("MODE", 0);

	{
		auto binds = abbInstance.GetBinds();
		size_t idx = 1;
		for (const auto& actionButton : binds) {
			table.AddCell(actionButton.first->GetXMLActionName(), idx);
			table.AddCell(actionButton.second->GetXMLName(), idx);
			std::string modeEntry;

			if (Action::IsToggle(actionButton.first->GetCharacterOptions()))
				modeEntry += "Toggle_";
			else
				modeEntry += "Hold_";
			

			if (Action::IsDoubleTap(actionButton.first->GetCharacterOptions()))
				modeEntry += "Dtap";
			else
				modeEntry += "Stap";

			table.AddCell(modeEntry, idx);

			++idx;
		}
	}
	table.PrintTable();
}

int main()
{
	auto Help = []() {
		std::cout << std::string(
			R"(------Help Text------
help			=>		Show help text
setgd <dir>		=>		Set Cyberpunk 2077 Root Directory to <dir>
showl			=>		List All Saved Configs
showgd			=>		Show Current Set Game Directory
showc			=>		Show Loaded Config
load <name>		=>		Load Config <name>
save <name>		=>		Save Active Config as <name>
del  <name>		=>		Delete Config <name>
apply			=>		Apply Current Binds to Game
reset			=>		Reset Binds to Game Defaults
mapbutton		=>		Open Button Remapper Wizard
summary			=>		Display Current Button Layout)") << std::endl;
	};
	PersistenceManager& pmInstance = PersistenceManager::Get();
	ActionButtonBinder& abbInstance = ActionButtonBinder::Get();

	if (!pmInstance.GetGameDir().empty()) 
		std::cout << "Detected Game DIR: " << pmInstance.GetGameDir() << std::endl;

	Help();

	bool run = true;
	while (run) {

		std::cout << ">>>";
		std::string userInput;
		std::getline(std::cin, userInput);
		std::vector<std::string> vecArgs = HawtLib::Text::SplitString(userInput, ' ');


		if(vecArgs.size() == 1)
		{
			if (vecArgs[0] == "help") 
			{
				Help();
			}
			else if (vecArgs[0] == "exit") {
				run = false;
			}
			else if (vecArgs[0] == "reset") {
				abbInstance.ResetBinds();
			}
			else if (vecArgs[0] == "showl")
			{
				std::vector<std::string> vecSaveNames = pmInstance.GetSaves();
				std::cout << "Saves List:" << std::endl;
				for (const std::string& saveName : vecSaveNames)
				{
					std::cout << saveName << std::endl;
				}
			}
			else if (vecArgs[0] == "showc")
			{
				if (pmInstance.GetLoadedSave().empty())
					std::cout << "No Loaded Save" << std::endl;
				else std::cout << pmInstance.GetLoadedSave() << std::endl;
				
			}
			else if (vecArgs[0] == "apply")
			{
				pmInstance.Apply();
			}
			else if (vecArgs[0] == "showgd") {
				std::cout << "Game Directory: " << pmInstance.GetGameDir() << std::endl;
			}
			else if (vecArgs[0] == "mapbutton")
			{
				MapButtonWizard(abbInstance);
			}
			else if (vecArgs[0] == "summary")
			{
				Summary(abbInstance);
			}
			else
			{
				Help();
			}
		}

		else if (vecArgs.size() == 2)
		{
			if (vecArgs[0] ==  "load")
			{
				pmInstance.Load(vecArgs[1]);
			}
			else if (vecArgs[0] == "save")
			{
				pmInstance.Save(vecArgs[1]);
			}
			else if (vecArgs[0] == "delete") 
			{
				pmInstance.Delete(vecArgs[1]);
			}
			else if (vecArgs[0] == "setgd") 
			{
				pmInstance.SetGameDir(vecArgs[1]);
			}
			else
			{
				Help();
			}
		}

		else
		{
			Help();
		}
	}
}

#endif
#include <vector>

#include "Core/Action.h"
#include "Core/ActionButtonBinder.h"
#include "Core/ButtonSingle.h"
#include "Core/ButtonCombo.h"
#include "Core/PersistenceManager.h"

using namespace ControllerMapper;

#ifdef GUI

#include "Gui/GuiApp.h"
#include "imgui.h"
#include "fmt/format.h"

namespace ControllerMapper
{
	static ImVec2 g_WindowDimensions = { 800, 1000 };
	static Action* g_OpenedAction = ActionButtonBinder::Get().GetBinds().begin()->first;
	static std::shared_ptr<IButton> g_BoundButton = ActionButtonBinder::Get().GetBinds().begin()->second;
	static ImGuiTextBuffer g_LogBuf;
	static int g_TapMode = 0;
	static int g_ToggleMode = 0;
	static bool g_IsTwoButton = false;
	struct TwoUIntCache
	{
		size_t x, y;

		void Push(size_t val)
		{
			this->x = y;
			this->y = val;
		}

	} g_twoIntCache = {0, 0};

	namespace GUIComponents
	{
		static void Save()
		{
			PersistenceManager& pmInstance = PersistenceManager::Get();

			ImGui::Begin("Save Menu", nullptr);
			static char buffer[512];
			ImGui::InputText("Configuration Name", buffer, sizeof(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Confirm"))
			{
				try
				{
					pmInstance.Save(std::string(buffer));
					g_LogBuf.appendf("Saved Current Configuration to %s\n", buffer);
				}
				catch (std::exception& e)
				{
					g_LogBuf.appendf("%s\n", e.what());
				}
			}

			ImGui::BeginChild("Saves List");
			auto saves = pmInstance.GetSaves();

			std::string allSaves;
			for(const auto& save : saves)
			{
				allSaves += save + "\n";
			}

			ImGui::Text("%s", allSaves.c_str());
			ImGui::EndChild();

			ImGui::End();
		}

		static void Load()
		{
			PersistenceManager& pmInstance = PersistenceManager::Get();

			ImGui::Begin("Load Menu", nullptr);
			static char buffer[512];
			ImGui::InputText("Configuration Name", buffer, sizeof(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Confirm"))
			{
				try
				{
					pmInstance.Load(std::string(buffer));
					g_LogBuf.appendf("Loaded Configuration %s\n", buffer);
				}
				catch (std::exception& e)
				{
					g_LogBuf.appendf("%s\n", e.what());
				}
			}

			ImGui::BeginChild("Saves List");
			const auto saves = pmInstance.GetSaves();
			std::string allSaves;
			for (const auto& save : saves)
			{
				allSaves += save + "\n";
			}
			ImGui::Text("%s", allSaves.c_str());
			ImGui::EndChild();

			ImGui::End();
		}

		static void Delete()
		{
			PersistenceManager& pmInstance = PersistenceManager::Get();

			ImGui::Begin("Delete Menu", nullptr);
			static char buffer[512];
			ImGui::InputText("Configuration Name", buffer, sizeof(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Confirm"))
			{
				try
				{
					pmInstance.Delete(std::string(buffer));
					g_LogBuf.appendf("Deleted Configuration %s\n", buffer);
				}
				catch (std::exception& e)
				{
					g_LogBuf.appendf("%s\n", e.what());
				}
			}

			ImGui::BeginChild("Saves List");
			const auto saves = pmInstance.GetSaves();
			std::string allSaves;
			for (const auto& save : saves)
			{
				allSaves += save + "\n";
			}
			ImGui::Text("%s", allSaves.c_str());
			ImGui::EndChild();
			ImGui::End();
		}

		static void About(bool& showAbout)
		{
			ImGui::Begin("About", &showAbout);
			ImGui::TextWrapped("Cyberpunk 2077 Controller Button Remapper");
			ImGui::End();
		}

		static void Summary(bool& showSummary)
		{
			ActionButtonBinder& abbInstance = ActionButtonBinder::Get();
			const auto& binds = abbInstance.GetBinds();

			ImGui::Begin("Summary", &showSummary);
			if (ImGui::BeginTable("Summary Table", 4, ImGuiTableFlags_BordersH))
			{
				for (int row = 0; row < static_cast<int>(CharacterAction::ActionType::Reload); row++)
				{
					ImGui::TableNextRow();
					for (int column = 0; column < 4; column++)
					{
						ImGui::TableSetColumnIndex(column);
						if (column == 0)	// Action
							
							ImGui::Text("%s",Action::GetActionPtr(CharacterAction(static_cast<CharacterAction::ActionType>(row)))->GetXMLActionName().c_str());
						
						else if (column == 1) // Button
						{
							auto button = binds.at(Action::GetActionPtr(CharacterAction(static_cast<CharacterAction::ActionType>(row))));
							std::string keyName = button->GetXMLName();
							if (keyName != "NONE")
							{
								const auto* btnDptr = dynamic_cast<ButtonCombo*>(button.get());
								if (btnDptr != nullptr)
									keyName = GetControllerKeyXML(btnDptr->GetKey1()).substr(7) +
									"\n" +
									GetControllerKeyXML(btnDptr->GetKey2()).substr(7);
								
								else
									keyName = button->GetXMLName().substr(7);
							}
							ImGui::TextWrapped("%s", keyName.c_str());
						}

						// Skip two for extra space

						else if (column == 3) 	// Mode
						{
							std::string modeEntry;
							if (Action::IsToggle(Action::GetActionPtr(CharacterAction(static_cast<CharacterAction::ActionType>(row)))->GetCharacterOptions()))
								modeEntry += "Toggle_";
							else
								modeEntry += "Hold_";


							if (Action::IsDoubleTap(Action::GetActionPtr(CharacterAction(static_cast<CharacterAction::ActionType>(row)))->GetCharacterOptions()))
								modeEntry += "Dtap";
							else
								modeEntry += "Stap";
							ImGui::Text("%s", modeEntry.c_str());
						}
					}
				}
				ImGui::EndTable();
			}
			ImGui::End();
		}

		static void ChooseButtons(Action* action)
		{
			ImGui::Begin("Map Buttons");

			ImGui::Checkbox("Two Buttons", &g_IsTwoButton);

			ImGui::Separator();

			ImGui::Text("Supported Options");

			const auto supportedOptions = action->GetCharacterAction().supportedOptions;

			if ((supportedOptions | DTAP) == supportedOptions)
				ImGui::RadioButton("Double Tap", &g_TapMode, 0);

			if ((supportedOptions | STAP) == supportedOptions)
				ImGui::RadioButton("Single Tap", &g_TapMode, 1);

			if ((supportedOptions | HOLD) == supportedOptions)
				ImGui::RadioButton("Hold Mode", &g_ToggleMode, 0);

			if ((supportedOptions | TOGGLE) == supportedOptions)
				ImGui::RadioButton("Toggle Mode", &g_ToggleMode, 1);

			// TODO: Swap Order of Single Tap and DTap (everything supports stap, so it makes sense for that to be default to make this check unnecessary)
			if (g_TapMode == 0 && (supportedOptions | DTAP) != supportedOptions)
			{
				g_TapMode = 1;
			}
			ImGui::Separator();

			for (size_t i = static_cast<size_t>(ControllerKey::None); i <= static_cast<size_t>(ControllerKey::DigitRight); ++i)
			{
				std::string keyName = ControllerMapper::GetControllerKeyXML(static_cast<ControllerKey> (i));
				if (i > 0)
					keyName = keyName.substr(7);

				if (ImGui::Button(keyName.c_str()))
				{
					if (i != g_twoIntCache.y)
					{
						if ((g_IsTwoButton && keyName != "NONE") || !g_IsTwoButton)
						{
							g_twoIntCache.Push(i);
							
						}
					}
				}
			}

			ImGui::End();
		}

		static void ChooseActions()
		{
			ImGui::Begin("Map Actions");
			auto& abbInstance = ActionButtonBinder::Get();
			const auto& binds = abbInstance.GetBinds();

			for (auto& kV : binds)
			{
				// Personal preference, I like the way it looks -HawtStrokes
				if (ImGui::Button(kV.first->GetXMLActionName().c_str()))
				{
					g_OpenedAction = kV.first;
				}
			}

			ImGui::End();
		}

		static void SelectedActionButton() {
			ImGui::Begin("Pending Binds");
			ImGui::TextWrapped("Selected Action:\n    %s", g_OpenedAction->GetXMLActionName().c_str());

			ImGui::Separator();

			if (g_IsTwoButton)
			{
				g_BoundButton = std::make_shared<ButtonCombo>(ButtonCombo(static_cast<ControllerKey>(g_twoIntCache.x), static_cast<ControllerKey>(g_twoIntCache.y)));
				std::string firstKey = GetControllerKeyXML(static_cast<ControllerKey>(g_twoIntCache.x));
				std::string secondKey = GetControllerKeyXML(static_cast<ControllerKey>(g_twoIntCache.y));

				ImGui::TextWrapped("Selected Button Combo:\n   %s\n   %s",
					firstKey == "NONE" ? firstKey.c_str() : firstKey.substr(7).c_str(),
					secondKey == "NONE" ? secondKey.c_str() : secondKey.substr(7).c_str());
			}
			else
			{
				g_BoundButton = std::make_shared<ButtonSingle>(ButtonSingle(static_cast<ControllerKey>(g_twoIntCache.y)));
				if (g_BoundButton->GetXMLName() != "NONE")
				{
					ImGui::TextWrapped("Selected Button:\n   %s", g_BoundButton->GetXMLName().substr(7).c_str());
				}
				else ImGui::TextWrapped("Selected Button:\n   %s", g_BoundButton->GetXMLName().c_str());
			}

			ImGui::Separator();

			std::string tapOptions;
			std::string modeOptions;

			switch (g_TapMode)
			{
			case 0:
				tapOptions += "Double Tap";
				break;
			case 1:
				tapOptions += "Single Tap";
				break;
			default:
				break;
			}

			switch (g_ToggleMode)
			{
			case 0:
				modeOptions += "Hold Mode";
				break;
			case 1:
				modeOptions += "Toggle Mode";
				break;
			default:
				break;
			}

			ImGui::Text("Selected Options:\n   %s\n   %s", tapOptions.c_str(), modeOptions.c_str());

			ImGui::End();
		}
	}

	void GUIMain()
	{
		static bool showAbout = false;
		static bool showSummary = false;

		auto& guiInstance = WrapGui::GuiApp::Get();
		auto& abbinstance = ActionButtonBinder::Get();
		auto& pmInstance = PersistenceManager::Get();

		guiInstance.HideWindow();
		ImGui::SetNextWindowSize(g_WindowDimensions);
		ImGui::Begin("Cyberpunk 2077 Controller Buttons Remapper", guiInstance.GetRunStatePtr(), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Reset"))
				{
					abbinstance.ResetBinds();
					g_LogBuf.append("Button Binds Has Been Reset! Click Apply To Change Game Controller Mappings.\n");
				}
				if (ImGui::MenuItem("About"))
				{
					showAbout = true;
				}
				if (ImGui::MenuItem("Exit"))
				{
					guiInstance.Close();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// GameDirectoryManager
		ImGui::Text("%s%s", "Detected Game Dir: ", pmInstance.GetGameDir().c_str());
		static char gameDirBuf[512];
		ImGui::InputText("Set Game Directory", gameDirBuf, sizeof(gameDirBuf));
		ImGui::SameLine();
		if (ImGui::Button("Confirm"))
		{
			try
			{
				pmInstance.SetGameDir(std::string(gameDirBuf));
				g_LogBuf.appendf("Game Directory Set To %s\n", gameDirBuf);
			}
			catch (std::exception& e)
			{
				g_LogBuf.appendf("%s\n", e.what());
			}
		}
		ImGui::Separator();

		ImGui::DockSpace(ImGui::GetID("ActionBinds"), ImVec2(g_WindowDimensions.x/3.125f, g_WindowDimensions.y / 2.5f));
		ImGui::SetNextWindowDockID(ImGui::GetID("ActionBinds"));
		GUIComponents::ChooseActions();

		ImGui::SameLine();
		
		ImGui::DockSpace(ImGui::GetID("ButtonBinds"), ImVec2(g_WindowDimensions.x/3.125f, g_WindowDimensions.y / 2.5f));
		ImGui::SetNextWindowDockID(ImGui::GetID("ButtonBinds"));
		GUIComponents::ChooseButtons(g_OpenedAction);

		ImGui::SameLine();

		ImGui::DockSpace(ImGui::GetID("Binds"), ImVec2(g_WindowDimensions.x / 3.125f, g_WindowDimensions.y / 2.5f));
		ImGui::SetNextWindowDockID(ImGui::GetID("Binds"));
		GUIComponents::SelectedActionButton();

		ImGui::Separator();
		
		if (ImGui::Button("Bind", ImVec2(g_WindowDimensions.x / 3.125f, 50)))
		{
			unsigned inputOptions = 0;
			switch (g_TapMode)
			{
			case 0:
				inputOptions |= DTAP;
				break;
			case 1:
				inputOptions |= STAP;
				break;
			default:
				break;
			}

			switch (g_ToggleMode)
			{
			case 0:
				inputOptions |= HOLD;
				break;
			case 1:
				inputOptions |= TOGGLE;
				break;
			default:
				break;
			}

			g_LogBuf.append("Attempting to Bind...\n");
			if ((g_IsTwoButton && g_twoIntCache.x != g_twoIntCache.y) || !g_IsTwoButton)
			{
				abbinstance.Bind(Action::GetActionPtr(g_OpenedAction->GetCharacterAction(), CharacterOptions(inputOptions)), g_BoundButton);
				g_LogBuf.appendf("Successfully Bound %s to %s! Click Apply To Change Game Controller Mappings.\n",
					g_OpenedAction->GetXMLActionName().c_str(), g_BoundButton->GetXMLName().c_str());
			}
			else
			{
				g_LogBuf.append("Error: Selected Buttons must be different!\n");
			}
		}

		ImGui::SameLine();
		if (ImGui::Button("Summary", ImVec2(g_WindowDimensions.x / 3.125f, 50)))
		{
			showSummary = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Apply", ImVec2(g_WindowDimensions.x / 3.125f, 50)))
		{
			pmInstance.Apply();
			g_LogBuf.append("Successfully Applied Configuration!\n");
		}

		ImGui::DockSpace(ImGui::GetID("Persistence"), ImVec2(g_WindowDimensions.x - 10.0f, g_WindowDimensions.y/4.5f));
		ImGui::DockSpace(ImGui::GetID("Logs"));
		ImGui::SetNextWindowDockID(ImGui::GetID("Logs"));
		ImGui::Begin("Log Window");
		if (ImGui::Button("Clear"))
			g_LogBuf.clear();
			if (ImGui::CollapsingHeader("Logs"))
			{
				ImGui::Text("%s", g_LogBuf.begin());
			}
				
		ImGui::End();

		ImGui::SetNextWindowDockID(ImGui::GetID("Persistence"));
		GUIComponents::Save();
		ImGui::SetNextWindowDockID(ImGui::GetID("Persistence"));
		GUIComponents::Load();
		ImGui::SetNextWindowDockID(ImGui::GetID("Persistence"));
		GUIComponents::Delete();

		if(showAbout)
		{
			ImGui::SetNextWindowDockID(ImGui::GetID("Persistence"));
			GUIComponents::About(showAbout);
		}
		
		if (showSummary)
		{
			ImGui::SetNextWindowDockID(ImGui::GetID("Persistence"));
			GUIComponents::Summary(showSummary);
		}

		ImGui::End();
	}
}

int main()
{
	auto& guiInstance = WrapGui::GuiApp::Get();
	guiInstance.SetMain(GUIMain);
	guiInstance.Run();
}

#else
#include <iostream>

#include  "HawtLib/Table/Table.h"
#include "HawtLib/Text/TextFormatting.h"

// Outputs text, gets userInput, and returns vector of splitted string
static std::vector<std::string> ConsoleInteractiveWrite(const std::string& text, std::string& userInput, char delim = ' ') {
	std::cout << text;
	std::getline(std::cin, userInput);
	return HawtLib::Text::SplitString(userInput, delim);
}

static void MapButtonWizard() {
	std::cout << HawtLib::Text::CenterText("--------------------------") << std::endl;
	std::cout << HawtLib::Text::CenterText("Map Button Wizard") << std::endl;
	std::cout << HawtLib::Text::CenterText("--------------------------") << std::endl;

	// Create Table
	HawtLib::Tables::Table table;
	table.AddCell("INDEX", 0)
		.AddCell("ACTION", 0)
		.AddCell("BOUND_BUTTON", 0)
		.AddCell("MODE", 0);


	ActionButtonBinder& abbInstance = ActionButtonBinder::Get();
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
				for (size_t i = static_cast<size_t>(ControllerKey::None); i <= static_cast<size_t>(ControllerKey::DigitRight); ++i)
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

static void Summary()
{
	// Create Table
	HawtLib::Tables::Table table;
	table.AddCell("ACTION", 0)
		.AddCell("BUTTON", 0)
		.AddCell("MODE", 0);

	{
		ActionButtonBinder& abbInstance = ActionButtonBinder::Get();
		const auto& binds = abbInstance.GetBinds();
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
				MapButtonWizard();
			}
			else if (vecArgs[0] == "summary")
			{
				Summary();
			}
			else
			{
				Help();
			}
		}

		else if (vecArgs.size() == 2)
		{
			try		// Wrap Persistence Functions in try block
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

#endif
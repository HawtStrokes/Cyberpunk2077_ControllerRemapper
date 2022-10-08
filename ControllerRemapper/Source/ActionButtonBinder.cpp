#define FMT_HEADER_ONLY

#include "ActionButtonBinder.h"
#include "Action.h"
#include "ButtonCombo.h"
#include "ButtonSingle.h"
#include "Bases.h"

#include "fmt/format.h"

#include <fstream>
#include <deque>

namespace ControllerMapper
{
	// Returns Static Instance of ABB
	ActionButtonBinder& ActionButtonBinder::Get() {
		static ActionButtonBinder instance;
		return instance;
	}

	// Builds XML to FileLocation
	void ActionButtonBinder::BuildXML(const std::string& fileLocation) {
		auto CreateButtonComboXML = [](ButtonCombo& button)->std::string {
			return fmt::format(fmt::runtime( R"(<buttonGroup id="Custom_{}" timeWindow="0.08f" >
        <button id="{}" />
        <button id="{}" />
    </buttonGroup>)"), button.GetXMLName(), GetControllerKeyXML(button.GetKey1()), GetControllerKeyXML(button.GetKey2()));
		};
		auto CreateButtonBindXML = [](IButton* button, Action* action)-> std::string {
			return  fmt::format(fmt::runtime(action->GetXMLCodeUM()), button->GetXMLName());
		};
		auto CreateDoubleTapXML = [](Action* action) -> std::string {
			return fmt::format(fmt::runtime(action->GetXMLCodeIC()), GetXMLActionName(action->GetCharacterAction().actionType));
		};

		std::deque<std::string> vecToUM;
		std::vector<std::string> vecToIC;

		for(auto& kv : m_Binds)
		{
			auto btnSPtr = dynamic_cast<ButtonSingle*>(kv.second.get());
			if (btnSPtr != nullptr)
			{
				vecToUM.emplace_back(CreateButtonBindXML(btnSPtr, kv.first));
			}
			else
			{
				auto btnDPtr = dynamic_cast<ButtonCombo*>(kv.second.get());
				vecToUM.emplace_front(CreateButtonComboXML(*btnDPtr));	// emplace_front custom button layout 
			}

			// check if action supports double tap and if double tap is enabled
			CharacterAction kvAction = kv.first->GetCharacterAction();	// Store to variable to avoid repetitive function call in next if statement
			if ((kvAction.actionType == CharacterAction::ActionType::Crouch ||
				 kvAction.actionType == CharacterAction::ActionType::Sprint || 
				 kvAction.actionType == CharacterAction::ActionType::Scan) && 
				IsDoubleTapEnabled(kv.first->GetCharacterOptions()))
			{
				vecToIC.emplace_back(CreateDoubleTapXML(kv.first));
			}
		}

		// Add to respective files
		std::ofstream ofsUM(fileLocation + "inputUserMappings.xml");
		ofsUM << g_Bases.baseUM;
		for(const auto& str : vecToUM) ofsUM << str;
		ofsUM << "</bindings>";
		ofsUM.close();

		std::ofstream ofsIC(fileLocation + "inputContexts.xml");
		ofsIC << g_Bases.baseIC;
		for (const auto& str : vecToIC) ofsIC << str;
		ofsIC << "</bindings>";
		ofsIC.close();
	}

	// Binds button shared_ptr to Action*
	void ActionButtonBinder::Bind(Action* action, std::shared_ptr<IButton> button) {
		m_Binds[action] = std::move(button);
	}

	// Deletes Action_ptrs
	void ActionButtonBinder::_CleanUp() {
		for (auto& kv : m_Binds)
		{
			delete kv.first;
		}
	}

	// Returns Binds Hash Map
	std::unordered_map<Action*, std::shared_ptr<IButton>>& ActionButtonBinder::GetBinds() {
		return m_Binds;
	}

	// DTOR - calls  _Cleanup which cleans allocated ActionPtrs
	ActionButtonBinder::~ActionButtonBinder() {
		_CleanUp();
	}

}
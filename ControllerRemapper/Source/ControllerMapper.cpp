#define FMT_HEADER_ONLY

#include "ControllerMapper.h"
#include "Action.h"
#include "ButtonCombo.h"
#include "ButtonSingle.h"

#include "fmt/format.h"

#include <fstream>
#include <deque>

namespace ControllerMapper
{
	void ControllerMapper::BuildXML(const std::string& fileLocation) {

		auto CreateButtonComboXML = [](ButtonCombo& button)->std::string {
			return fmt::format(fmt::runtime( R"(<buttonGroup id="Custom_{}" timeWindow="0.08f" >
        <button id="{}" />
        <button id="{}" />
    </buttonGroup>)"), button.GetXMLName(), IButton::Static_GetControllerKeyXML(button.GetKey1()), IButton::Static_GetControllerKeyXML(button.GetKey2()));
		};
		/*auto CreateButtonBindXML = [](IButton* button, Action* action)-> std::string {
			return fmt::format(action->GetXMLCodeUM(), button->GetXMLName());
		};
		auto CreateDoubleTapXML = [](Action* action) -> std::string {
			return fmt::format(action->GetXMLCodeIC(), CharacterAction::Static_GetXMLActionName(action->GetCharacterAction().actionType));
		};*/
		auto CreateButtonBindXML = [](IButton* button, Action* action)-> std::string {
			return  fmt::format(fmt::runtime(action->GetXMLCodeUM()), button->GetXMLName());
		};
		auto CreateDoubleTapXML = [](Action* action) -> std::string {
			return fmt::format(fmt::runtime(action->GetXMLCodeIC()), CharacterAction::Static_GetXMLActionName(action->GetCharacterAction().actionType));
		};

		std::deque<std::string> vecToUM;
		std::vector<std::string> vecToIC;

		for(auto& kv : m_Binds)
		{
			auto btnSPtr = dynamic_cast<ButtonSingle*>(kv.first);
			if (btnSPtr != nullptr)
			{
				vecToUM.push_back(CreateButtonBindXML(btnSPtr, m_Binds.at(btnSPtr)));
			}
			else
			{
				auto btnDPtr = dynamic_cast<ButtonCombo*>(kv.first);
				vecToUM.push_front(CreateButtonComboXML(*btnDPtr));	// push_front custom button layout 
			}

			// check if action supports double tap and if double tap is enabled
			if ((kv.second->GetCharacterAction().actionType == CharacterAction::ActionType::Crouch ||
				kv.second->GetCharacterAction().actionType == CharacterAction::ActionType::Sprint || 
				kv.second->GetCharacterAction().actionType == CharacterAction::ActionType::Scan) && 
				Action::Static_DoubleTapEnabled(kv.second->GetOptions()))
			{
				vecToIC.push_back(CreateDoubleTapXML(kv.second));
			}
		}

		// Add to respective files

		std::ofstream ofsUM(fileLocation + "inputUserMappings.xml");
		for(const auto& str : vecToUM) ofsUM << str;

		std::ofstream ofsIC(fileLocation + "inputContexts.xml");
		for (const auto& str : vecToIC) ofsIC << str;
	}

	void ControllerMapper::Bind(IButton* button, Action* action) {
		m_Binds[button] = action;
	}

	void ControllerMapper::_CleanUp() {
		for (auto& kv : m_Binds)
		{
			delete kv.first;
			delete kv.second;
		}
	}

	ControllerMapper::~ControllerMapper() {
		_CleanUp();
	}

	ControllerMapper& ControllerMapper::Get() {
		static ControllerMapper instance;
		return instance;
	}
}
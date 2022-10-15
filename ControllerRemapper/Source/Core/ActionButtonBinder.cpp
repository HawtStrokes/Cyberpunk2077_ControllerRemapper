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
	// Ctor calls reset
	ActionButtonBinder::ActionButtonBinder() {
		Internal_Reset();
	}

	// Returns Static Instance of ABB
	ActionButtonBinder& ActionButtonBinder::Get() {
		static ActionButtonBinder instance;
		return instance;
	}

	// DTOR - calls  _Cleanup which cleans allocated ActionPtrs
	ActionButtonBinder::~ActionButtonBinder() {
		Internal_CleanUp();
	}



	// Deletes Action_ptrs
	void ActionButtonBinder::Internal_CleanUp() {
		for (const auto& kv : m_Binds) {
			delete kv.first;
		}
	}

	// Reset Binds
	void ActionButtonBinder::Internal_Reset() {
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Cyberware, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonCombo>(ControllerKey::LeftShoulder, ControllerKey::RightShoulder));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::CombatGadget, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::RightShoulder));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Consumable, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::DigitUp));

		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Jump, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::A_CROSS));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Crouch, CharacterOptions(STAP | TOGGLE)), std::make_shared<ButtonSingle>(ControllerKey::B_CIRCLE));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Dodge, CharacterOptions(DTAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::B_CIRCLE));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Sprint, CharacterOptions(STAP | TOGGLE)), std::make_shared<ButtonSingle>(ControllerKey::LeftThumb));

		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Phone, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::DigitDown));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::CallCar, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::DigitRight));

		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Scan, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::LeftShoulder));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Tag, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::RightThumb));

		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Choice1, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::X_SQUARE));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Choice2, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::Y_TRIANGLE));

		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Notification, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::DigitLeft));

		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::CycleWeapons, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::Y_TRIANGLE));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::PreviousWeapon, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::None));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::QuickMelee, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::RightThumb));
		Internal_Bind(Action::GetActionPtr(CharacterAction::ActionType::Reload, CharacterOptions(STAP | HOLD)), std::make_shared<ButtonSingle>(ControllerKey::X_SQUARE));
	}

	// Version of Bind that allows creation of new Action*
	void ActionButtonBinder::Internal_Bind(Action* action, std::shared_ptr<IButton> button) {
		m_Binds[action] = std::move(button);
	}



	// Builds XML to FileLocation
	void ActionButtonBinder::BuildXML(const std::string& fileLocation) {
		auto CreateButtonComboXML = [](ButtonCombo& button)->std::string {
			return fmt::format( R"(<buttonGroup id="{}" timeWindow="0.08f" > <button id="{}" /> <button id="{}" /> </buttonGroup>)", button.GetXMLName(), GetControllerKeyXML(button.GetKey1()), GetControllerKeyXML(button.GetKey2()));
		};
		auto CreateButtonBindXML = [](IButton* button, Action* action)-> std::string {
			return  fmt::format(fmt::runtime(action->GetXMLCodeUM()), button->GetXMLName());
		};
		auto CreateDoubleTapXML = [](Action* action) -> std::string {
			return fmt::format(R"(<multitap action="{}" count="2" uptime="0.2" downtime="0.2" />)", action->GetXMLActionName());

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
				vecToUM.emplace_back(CreateButtonBindXML(btnDPtr, kv.first));
			}

			// check if action supports double tap and if double tap is enabled
			if (Action::IsDoubleTap(kv.first->GetCharacterOptions()))
			{
				vecToIC.emplace_back(CreateDoubleTapXML(kv.first));
			}
		}

		// Add to respective files
		std::ofstream ofsUM(fileLocation + "inputUserMappings.xml");
		ofsUM << Bases::GetBaseUM();
		for (const auto& str : vecToUM)
			ofsUM << str;
		
		ofsUM << "</bindings>";
		ofsUM.close();

		std::ofstream ofsIC(fileLocation + "inputContexts.xml");
		ofsIC << Bases::GetBaseIC();
		for (const auto& str : vecToIC) 
			ofsIC << str;

		ofsIC << "</bindings>";
		ofsIC.close();
	}

	// Binds button shared_ptr to Action*
	void ActionButtonBinder::Bind(Action* action, std::shared_ptr<IButton> button) {
		m_Binds.at(action) = std::move(button);
	}

	// Reset Binds
	void ActionButtonBinder::ResetBinds() {
		Internal_Reset();
	}

	// Returns Binds Hash Map
	std::unordered_map<Action*, std::shared_ptr<IButton>>& ActionButtonBinder::GetBinds() {
		return m_Binds;
	}

	

}


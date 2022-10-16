#include "Action.h"
#include "Exceptions.h"

#include <unordered_map>
#include <sstream>

namespace ControllerMapper
{

	std::unordered_map<CharacterAction, Action*> Action::m_ActionCache;

	unsigned int CharacterAction::Internal_GetSupportedOptions() {
		switch (actionType) {
		case ActionType::Cyberware:
			return OPTIONS_CYBERWARE;
		case ActionType::CombatGadget:
			return OPTIONS_COMBATGADGET;
		case ActionType::Consumable:
			return OPTIONS_CONSUMABLE;

		case ActionType::Jump:
			return OPTIONS_JUMP;
		case ActionType::Crouch:
			return OPTIONS_CROUCH;
		case ActionType::Dodge:
			return OPTIONS_DODGE;
		case ActionType::Sprint:
			return OPTIONS_SPRINT;

		case ActionType::Phone:
			return OPTIONS_SPRINT;
		case ActionType::CallCar:
			return OPTIONS_CALLCAR;

		case ActionType::Scan:
			return OPTIONS_SCAN;
		case ActionType::Tag:
			return OPTIONS_TAG;

		case ActionType::Choice1:
			return OPTIONS_CHOICE1;
		case ActionType::Choice2:
			return OPTIONS_CHOICE2;

		case ActionType::Notification:
			return OPTIONS_NOTIFICATION;

		case ActionType::CycleWeapons:
			return OPTIONS_CYCLEWEAPONS;
		case ActionType::PreviousWeapon:
			return OPTIONS_PREVIOUSWEAPON;
		case ActionType::SwitchItem:
			return OPTIONS_SWITCHITEM;
		case ActionType::QuickMelee:
			return OPTIONS_QUICKMELEE;
		case ActionType::Reload:
			return OPTIONS_RELOAD;
		}

		throw InvalidCharacterOptions();
	}

	CharacterAction::CharacterAction(ActionType actionType)
	{
		this->actionType = actionType;
		supportedOptions = Internal_GetSupportedOptions();
	}

	bool CharacterAction::operator==(const CharacterAction& other) const {
		return (this->actionType == other.actionType);
	}

	///
	///	Actions
	///

	// returns true if all actions are supported and there are no conflicts found
	bool Action::Internal_VerifyActionSupport(CharacterAction characterAction, CharacterOptions characterOptions) {
		// returns true if options are supported
		auto CheckOptionSupport = [&]() -> bool {
			if ((characterAction.supportedOptions | characterOptions.options) == characterAction.supportedOptions) return true;	// options supported
			else return false;	// optionList not supported
		};

		// returns true there are no conflicting and/or missing options
		auto CheckConflictingOptions = [&]() -> bool {
			// 11 11	->	Nibble
			// A   B
			// resuls each half of a nibble must strictly not be 11 (3_Base_10) or 00

			const unsigned int yA = characterOptions.options >> 2;															// get last two bits
			const unsigned int yB = static_cast<unsigned>(static_cast<unsigned char>(characterOptions.options << 6)) >> 6;	// get first two bits

			if (yA == 3 || yA == 0 || yB == 3 || yB == 0) {
				return false;	// Conflicting Options or lack of options detected
			}
			return true;	// No Conflicts
		};
		return CheckOptionSupport() && CheckConflictingOptions();
	}

	void Action::Internal_CreateXMLCodeBlock(CharacterAction characterAction, CharacterOptions characterOptions, bool checkSupport) {
		static std::unordered_map <CharacterAction::ActionType, std::string> xmlBlockMap = {
			{CharacterAction::ActionType::Cyberware,
R"(<mapping name="IconicCyberware_Button" type="Button" >
        <button id="IK_E" overridableUI="iconic"/>
        <button id="{}" />
    </mapping>)"},
			{CharacterAction::ActionType::CombatGadget,
R"(<mapping name="CombatGadget_Button" type="Button" >
        <button id="{}" />
        <button id="IK_MiddleMouse" overridableUI="combatGadget"/>
    </mapping> )"},
			{CharacterAction::ActionType::Consumable,
R"( <mapping name="UseConsumable_Button" type="Button" >
        <button id="{}" />
        <button id="IK_X" overridableUI="useConsumable"/>
    </mapping>)"},

			{CharacterAction::ActionType::Jump,
R"(<mapping name="Jump_Button" type="Button" >
        <button id="{}" />
        <button id="IK_Space" overridableUI="jump"/>
    </mapping>)"},
			{CharacterAction::ActionType::Crouch,
R"(<mapping name="Crouch_Button" type="Button" >
		<button id="{}" overridableUI="crouchHold"/>
    </mapping>
				#toggle
	<mapping name="ToggleCrouch_Button" type="Button" >
		<button id="{}" />
        <button id="IK_C" overridableUI="crouchToggle"/>
    </mapping>)"},	// Supports Toggle
			{CharacterAction::ActionType::Dodge,
R"(<mapping name="Dodge_Button" type="Button" >
		<button id="IK_Pad_B_CIRCLE" />
		<button id="IK_LControl" overridableUI="crouchHold"/>
		<button id="IK_C" overridableUI="crouchToggle"/>
    </mapping>)"},
			{CharacterAction::ActionType::Sprint,
R"(<mapping name="Sprint_Button" type="Button" >
    	<button id="{}" overridableUI="sprintHold"/>
    </mapping>
				#toggle
	<mapping name="ToggleSprint_Button" type="Button" >
		<button id="{}" />
		<button id="IK_LShift" overridableUI="sprintToggle"/>
    </mapping>)"},	// Supports Toggle

			{CharacterAction::ActionType::Phone,
R"(	<mapping name="PhoneInteract_Button" type="Button" >        
		<button id="IK_T" overridableUI="openPhone"/>
		<button id="{}" />
    </mapping>)"},
			{CharacterAction::ActionType::CallCar,
R"(    <mapping name="CallVehicle_Button" type="Button" >
        <button id="{}" />
        <button id="IK_V" overridableUI="callVehicle"/>
    </mapping> )"},
			{CharacterAction::ActionType::Scan,
R"(<mapping name="Vision_Hold_Button" type="Button" >
		<button id="IK_Tab" overridableUI="visionHold"/>
		<button id="{}" />
	</mapping>
				#toggle
<mapping name="Vision_Toggle_Button" type="Button" >
		<button id="IK_CapsLock" overridableUI="visionToggle"/>
		<button id="{}" />
	</mapping>
				)"},	// Supports Toggle
			{CharacterAction::ActionType::Tag,
R"(<mapping name="Tag_Button" type="Button" >
		<button id="{}" />
		<button id="IK_MiddleMouse" overridableUI="tag"/>
	</mapping>	)"},

			{CharacterAction::ActionType::Choice1,
R"(    <mapping name="Choice1" type="Button" >
		<button id="IK_F" overridableUI="selectChoice"/>
		<button id="{}" />
	</mapping>)"},
			{CharacterAction::ActionType::Choice2,
R"(	<mapping name="Choice2" type="Button" >
		<button id="IK_R" overridableUI="choice2"/>
		<button id="{}" />
	</mapping>)"},

			{CharacterAction::ActionType::Notification,
R"(    <mapping name="Notification_Button" type="Button" >        
		<button id="IK_Z" overridableUI="openNotifiation"/>
		<button id="{}" />
    </mapping>)"},

			{CharacterAction::ActionType::CycleWeapons,
R"(    <mapping name="WeaponWheel_Button" type="Button" >
		<button id="{}" />
		<button id="IK_Alt" overridableUI="switchItem"/>
    </mapping> )"},
			{CharacterAction::ActionType::PreviousWeapon,
R"(	<mapping name="PreviousWeapon_Button" type="Button" >
		<button id="IK_MouseWheelDown" overridableUI="previousWeapon"/>
		<button id="{}" overridableUI="previousWeapon"/> 
	</mapping>	)"},
			{ CharacterAction::ActionType::SwitchItem,
R"(	<mapping name="SwitchItem_Button" type="Button" >
		<button id="IK_Alt" overridableUI="switchItem"/>
		<button id="{}" />
	</mapping>)" },
			{CharacterAction::ActionType::QuickMelee,
R"( <mapping name="QuickMelee_Button" type="Button" >
		<button id="{}" />
		<button id="IK_Q" overridableUI="quickmelee"/>
    </mapping>
)"},

			{CharacterAction::ActionType::Reload,
R"(	<mapping name="Reload_Button" type="Button" >
        <button id="IK_R" overridableUI="reload"/>
		<button id="{}" />
    </mapping>)"}
		};


		if (checkSupport) {
			if(!Internal_VerifyActionSupport(characterAction, characterOptions))
			{
				throw InvalidCharacterOptions();
			}
		}
		// Fill XMLCodeBlocks with corresponsing content (still requires formatting to fill in keyName and actionName)
		m_XMLCodeUM = xmlBlockMap.at(characterAction.actionType);
	}


	CharacterAction Action::GetCharacterAction() const
	{
		return m_CharacterAction;
	}

	CharacterAction Action::GetCharacterAction()
	{
		return m_CharacterAction;
	}

	CharacterOptions Action::GetCharacterOptions() const
	{
		return m_CharacterOptions;
	}

	CharacterOptions Action::GetCharacterOptions()
	{
		return m_CharacterOptions;
	}


	Action::Action(CharacterAction characterAction, CharacterOptions characterOptions) : m_CharacterAction(characterAction), m_CharacterOptions(characterOptions)
	{
		// XML Built in the function who called this ctor
	}

	Action* Action::GetActionPtr(CharacterAction characterAction, CharacterOptions characterOptions) {
		// Verifty Action Support and init
		if (Internal_VerifyActionSupport(characterAction, characterOptions)) {
			if (m_ActionCache.contains(characterAction))	// check if characterAction is stored in cache
			{
				auto* action = m_ActionCache.at(characterAction);
				action->m_CharacterOptions = characterOptions;	// Update character options
				return action;
			}
			// if not Create a pointer, store in cache and return pointer
			auto* action = new Action{ characterAction, characterOptions };
			action->Internal_CreateXMLCodeBlock(characterAction, characterOptions);
			m_ActionCache[characterAction] = action;
			return action;
		}
		// throw if flipped options are not supported
		else throw InvalidCharacterOptions();
	}

	Action* Action::GetActionPtr(CharacterAction characterAction) {
		if (m_ActionCache.contains(characterAction)) return m_ActionCache.at(characterAction);
		else throw std::runtime_error("Cannot Find Action!");
	}

	std::string Action::GetXMLCodeUM()
	{
		std::string retStr;
		// hold mode
		if (!IsToggle(this->m_CharacterOptions))
		{
			std::istringstream iss(m_XMLCodeUM);
			std::string line;
			while (std::getline(iss, line)) {
				if (line.find("#toggle") != std::string::npos) {
					break;
				}
				retStr += line;
			}
		}

		// Toggle Mode
		else
		{
			std::istringstream iss(m_XMLCodeUM);
			std::string line;
			bool getStr = false;
			while (std::getline(iss, line)) {
				if (line.find("#toggle") != std::string::npos) {
					getStr = true;
					continue;	// skip #toggle
				}
				if (getStr)
				{
					retStr += line;
				}
			}
		}
		return retStr;
	}
	///
	/// Utility Functions
	///

	std::string Action::GetXMLActionMapName() {
		static std::unordered_map<CharacterAction::ActionType, std::string> xmlActionNameMap = {
			{CharacterAction::ActionType::Cyberware, "IconicCyberware_Button"},
			{CharacterAction::ActionType::CombatGadget, "CombatGadget_Button"},
			{CharacterAction::ActionType::Consumable, "UseConsumable_Button"},

			{CharacterAction::ActionType::Jump, "Jump_Button"},
			{CharacterAction::ActionType::Crouch, "Crouch_Button\nToggleCrouch_Button"},
			{CharacterAction::ActionType::Dodge, "Dodge_Button"},
			{CharacterAction::ActionType::Sprint, "Sprint_Button\nToggleSprint_Button"},

			{CharacterAction::ActionType::Phone, "PhoneInteract_Button"},
			{CharacterAction::ActionType::CallCar, "CallVehicle_Button"},
			{CharacterAction::ActionType::Scan, "Vision_Hold_Button\nVision_Toggle_Button"},
			{CharacterAction::ActionType::Tag, "Tag_Button"},

			{CharacterAction::ActionType::Choice1, "Choice1"},
			{CharacterAction::ActionType::Choice2, "Choice2"},

			{CharacterAction::ActionType::Notification, "Notification_Button"},

			{CharacterAction::ActionType::CycleWeapons, "WeaponWheel_Button"},
			{CharacterAction::ActionType::PreviousWeapon, "PreviousWeapon_Button"},
			{CharacterAction::ActionType::SwitchItem, "SwitchItem_Button"},
			{CharacterAction::ActionType::QuickMelee, "QuickMelee_Button"},

			{CharacterAction::ActionType::Reload, "Reload_Button"}
		};

		const auto actionType = this->m_CharacterAction.actionType;
		const bool isToggle = IsToggle(this->m_CharacterOptions);

		// TODO: Review if statement
		if (actionType == CharacterAction::ActionType::Crouch || actionType == CharacterAction::ActionType::Sprint || actionType == CharacterAction::ActionType::Scan) {
			std::istringstream iss(xmlActionNameMap.at(actionType));
			std::string line;
			const int returnIdx = isToggle ? 1 : 0; // return second line of string if isToggle is true; otherwose return the first line
			for (int i = 0; i < 2; ++i) {
				std::getline(iss, line);
				if (i == returnIdx) return line;
			}
		}
		else if (!isToggle) {
			return xmlActionNameMap.at(actionType);
		}

		throw InvalidCharacterOptions();	// we can't have any more actions that support toggle because of the first branch
	}

	std::string Action::GetXMLActionName()
	{
		static std::unordered_map<CharacterAction::ActionType, std::string> xmlActionNameMap = {
			{CharacterAction::ActionType::Cyberware, "IconicCyberware"},
			{CharacterAction::ActionType::CombatGadget, "UseCombatGadget"},
			{CharacterAction::ActionType::Consumable, "UseConsumable"},

			{CharacterAction::ActionType::Jump, "Jump"},
			{CharacterAction::ActionType::Crouch, "Crouch\nToggleCrouch"},
			{CharacterAction::ActionType::Dodge, "Dodge"},
			{CharacterAction::ActionType::Sprint, "Sprint\nToggleSprint"},

			{CharacterAction::ActionType::Phone, "PhoneInteract"},
			{CharacterAction::ActionType::CallCar, "CallVehicle"},
			{CharacterAction::ActionType::Scan, "VisionHold\nVisionToggle"},
			{CharacterAction::ActionType::Tag, "TagButton"},

			{CharacterAction::ActionType::Choice1, "Choice1"},
			{CharacterAction::ActionType::Choice2, "Choice2"},

			{CharacterAction::ActionType::Notification, "NotificationOpen"},

			{CharacterAction::ActionType::CycleWeapons, "WeaponWheel"},
			{CharacterAction::ActionType::PreviousWeapon, "PreviousWeapon"},
			{CharacterAction::ActionType::SwitchItem, "SwitchItem"},
			{CharacterAction::ActionType::QuickMelee, "QuickMelee"},

			{CharacterAction::ActionType::Reload, "Reload"}
		};

		const auto actionType = this->m_CharacterAction.actionType;
		const bool isToggle = IsToggle(this->m_CharacterOptions);

		if (actionType == CharacterAction::ActionType::Crouch || actionType == CharacterAction::ActionType::Sprint || actionType == CharacterAction::ActionType::Scan) {
			std::istringstream iss(xmlActionNameMap.at(actionType));
			std::string line;
			const int returnIdx = isToggle ? 1 : 0; // return second line of string if isToggle is true; otherwose return the first line
			for (int i = 0; i < 2; ++i) {
				std::getline(iss, line);
				if (i == returnIdx) return line;
			}
		}
		else if (!isToggle) {
			return xmlActionNameMap.at(actionType);
		}

		throw InvalidCharacterOptions();	// we can't have any more actions that support toggle because of the first branch
	}
}

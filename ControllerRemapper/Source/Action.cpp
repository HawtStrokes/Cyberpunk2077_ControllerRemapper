#include "Action.h"
#include "Exceptions.h"

#include <unordered_map>
#include <sstream>

namespace ControllerMapper
{
	// CharacterAction //

	std::string CharacterAction::Static_GetXMLBlock(ActionType actionType) {
		static std::unordered_map <ActionType, std::string> xmlBlockMap = {
			{ActionType::Cyberware,		R"(<mapping name="IconicCyberware_Button" type="Button" >
        <button id="IK_E" overridableUI="iconic"/>
        <button id="{}" />
    </mapping>)"},
			{ActionType::CombatGadget,	R"(<mapping name="CombatGadget_Button" type="Button" >
        <button id="{}" />
        <button id="IK_MiddleMouse" overridableUI="combatGadget"/>
    </mapping> )"},
			{ActionType::Consumable,	R"( <mapping name="UseConsumable_Button" type="Button" >
        <button id="{}" />
        <button id="IK_X" overridableUI="useConsumable"/>
    </mapping>)"},

			{ActionType::Jump,			R"(<mapping name="Jump_Button" type="Button" >
        <button id="{}" />
        <button id="IK_Space" overridableUI="jump"/>
    </mapping>)"},
			{ActionType::Crouch,		R"(<mapping name="Crouch_Button" type="Button" >
		<button id="{}" overridableUI="crouchHold"/>
    </mapping>
	<mapping name="ToggleCrouch_Button" type="Button" >
		<button id="{}" />
        <button id="IK_C" overridableUI="crouchToggle"/>
    </mapping>)"},	// Supports Toggle
			{ActionType::Dodge,			R"(<mapping name="Dodge_Button" type="Button" >
		<button id="IK_Pad_B_CIRCLE" />
		<button id="IK_LControl" overridableUI="crouchHold"/>
		<button id="IK_C" overridableUI="crouchToggle"/>
    </mapping>)"},	
			{ActionType::Sprint,		R"(<mapping name="Sprint_Button" type="Button" >
    	<button id="{}" overridableUI="sprintHold"/>
    </mapping>
	<mapping name="ToggleSprint_Button" type="Button" >
		<button id="{}" />
		<button id="IK_LShift" overridableUI="sprintToggle"/>
    </mapping>)"},	// Supports Toggle

			{ActionType::Phone,			R"(	<mapping name="PhoneInteract_Button" type="Button" >        
		<button id="IK_T" overridableUI="openPhone"/>
		<button id="{}" />
    </mapping>)"},
			{ActionType::CallCar,		R"(    <mapping name="CallVehicle_Button" type="Button" >
        <button id="{}" />
        <button id="IK_V" overridableUI="callVehicle"/>
    </mapping> )"},
			{ActionType::Scan,			R"(<mapping name="Vision_Hold_Button" type="Button" >
		<button id="IK_Tab" overridableUI="visionHold"/>
		<button id="{}" />
	</mapping>
<mapping name="Vision_Toggle_Button" type="Button" >
		<button id="IK_CapsLock" overridableUI="visionToggle"/>
		<button id="{}" />
	</mapping>
				)"},	// Supports Toggle
			{ActionType::Tag,			R"(
    <mapping name="Tag_Button" type="Button" >
		<button id="{}" />
		<button id="IK_MiddleMouse" overridableUI="tag"/>
	</mapping>	)"},

			{ActionType::Choice1,		R"(    <mapping name="Choice1" type="Button" >
		<button id="IK_F" overridableUI="selectChoice"/>
		<button id="{}" />
	</mapping>)"},
			{ActionType::Choice2,		R"(	<mapping name="Choice2" type="Button" >
		<button id="IK_R" overridableUI="choice2"/>
		<button id="{}" />
	</mapping>)"},

			{ActionType::Notification,	R"(    <mapping name="Notification_Button" type="Button" >        
		<button id="IK_Z" overridableUI="openNotifiation"/>
		<button id="{}" />
    </mapping>)"},

			{ActionType::CycleWeapons,	R"(    <mapping name="WeaponWheel_Button" type="Button" >
		<button id="{}" />
		<button id="IK_Alt" overridableUI="switchItem"/>
    </mapping> )"},
			{ActionType::PreviousWeapon,R"(	<mapping name="PreviousWeapon_Button" type="Button" >
		<button id="IK_MouseWheelDown" overridableUI="previousWeapon"/>
		<button id="{}" overridableUI="previousWeapon"/> 
	</mapping>	)"},
			{ActionType::QuickMelee,	R"(    <mapping name="QuickMelee_Button" type="Button" >
		<button id="{}" />
		<button id="IK_Q" overridableUI="quickmelee"/>
    </mapping>
)"},

			{ActionType::Reload,		R"(	<mapping name="Reload_Button" type="Button" >
        <button id="IK_R" overridableUI="reload"/>
		<button id="{}" />
    </mapping>)"}
		};
		return xmlBlockMap.at(actionType);
	}

	std::string CharacterAction::Static_GetXMLActionName(ActionType actionType, bool isToggle) {
		static std::unordered_map<ActionType, std::string> xmlActionNameMap = {
			{ActionType::Cyberware, "IconicCyberware_Button"},
			{ActionType::CombatGadget, "CombatGadget_Button"},
			{ActionType::Consumable, "UseConsumable_Button"},

			{ActionType::Jump, "Jump_Button"},
			{ActionType::Crouch, "Crouch_Button\nToggleCrouch_Button"},
			{ActionType::Dodge, "Dodge_Button"},
			{ActionType::Sprint, "Sprint_Button\nToggleSprint_Button"},

			{ActionType::Phone, "PhoneInteract_Button"},
			{ActionType::CallCar, "CallVehicle_Button"},
			{ActionType::Scan, "Vision_Hold_Button\nVision_Toggle_Button"},
			{ActionType::Tag, "Tag_Button"},

			{ActionType::Choice1, "Choice1"},
			{ActionType::Choice2, "Choice2"},

			{ActionType::Notification, "Notification_Button"},

			{ActionType::CycleWeapons, "WeaponWheel_Button"},
			{ActionType::PreviousWeapon, "PreviousWeapon_Button"},
			{ActionType::QuickMelee, "QuickMelee_Button"},

			{ActionType::Reload, "Reload_Button"}
		};
		
			// check if action supports toggle
		if (actionType == ActionType::Crouch || actionType == ActionType::Sprint || actionType == ActionType::Scan) {
			std::istringstream iss(xmlActionNameMap.at(actionType));
			std::string line;
			const int returnIdx = isToggle ? 1 : 0; // return second line of string if isToggle is true; otherwose return the first line
			for (int i = 0; i < 2; ++i) {
				std::getline(iss, line);
				if (i == returnIdx) return line;
			}
		}
		else if (!isToggle)
		{
			return xmlActionNameMap.at(actionType);
		}
		else
		{
			throw InvalidCharacterOptions();	// we can't have any more actions that support toggle because of the first branch
		}
			
	}

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
		case ActionType::QuickMelee:
			return OPTIONS_QUICKMELEE;
		case ActionType::Reload:
			return OPTIONS_RELOAD;

		default:
			return 0;
		}
	}

	CharacterAction::CharacterAction(ActionType actionType)
	{
		this->actionType = actionType;
		supportedOptions = Internal_GetSupportedOptions();
	}


	// Actions //

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
			else
			{
				return true;	// No Conflicts
			}

		};
		return CheckOptionSupport() && CheckConflictingOptions();
	}

	void Action::Internal_CreateXMLCodeBlock(CharacterAction characterAction, CharacterOptions characterOptions, bool checkSupport) {
		if (checkSupport) {
			if(!Internal_VerifyActionSupport(characterAction, characterOptions))
			{
				throw InvalidCharacterOptions();
			}
		}
		// Fill XMLCodeBlocks with corresponsing content (still requires formatting to fill in keyName and actionName)
		m_XMLCode = {CharacterAction::Static_GetXMLBlock(characterAction.actionType), [&]()->std::string
		{
			if (Static_DoubleTapEnabled(characterOptions)) return R"(<multitap action="{}" count="2" uptime="0.2" downtime="0.2" />)";
			else return "";
		}()};
	}

	CharacterAction Action::GetCharacterAction() const
	{
		return m_CharacterAction;
	}

	CharacterAction Action::GetCharacterAction()
	{
		return m_CharacterAction;
	}

	CharacterOptions Action::GetOptions() const
	{
		return m_Options;
	}

	CharacterOptions Action::GetOptions()
	{
		return m_Options;
	}


	Action::Action(CharacterAction characterAction, CharacterOptions characterOptions) : m_CharacterAction(characterAction), m_Options(characterOptions)
	{
		// XML Built in the function who called this ctor
	}

	// Factory-esque Public Member Function
	Action Action::BuildAction(CharacterAction characterAction, CharacterOptions characterOptions)
	{
		// Verifty Action Support and init
		if(Internal_VerifyActionSupport(characterAction, characterOptions))
		{
			Action action {characterAction, characterOptions};
			action.Internal_CreateXMLCodeBlock(characterAction, characterOptions);
			return action;
		}
		else throw InvalidCharacterOptions();
	}

	Action* Action::BuildActionPtr(CharacterAction characterAction, CharacterOptions characterOptions) {
		// Verifty Action Support and init
		if (Internal_VerifyActionSupport(characterAction, characterOptions)) {
			auto action = new Action{ characterAction, characterOptions };
			action->Internal_CreateXMLCodeBlock(characterAction, characterOptions);
			return action;
		}
		else throw InvalidCharacterOptions();
	}

	std::string Action::GetXMLCodeUM()
	{
		return m_XMLCode.UM;
	}

	std::string Action::GetXMLCodeIC()
	{
		return m_XMLCode.IC;
	}

	XMLCode Action::GetXMLCode()
	{
		return m_XMLCode;
	}

	bool Action::Static_DoubleTapEnabled(CharacterOptions characterOptions) {
		// Read first bit (which is either a 1 or 0)
		return (static_cast<unsigned>(static_cast<unsigned char>(characterOptions.options << 7)) >> 7);
	}
}


#pragma once
#include <optional>
#include <string>

#include "SharedTypes.h"

#define HOLD	0b1000
#define TOGGLE	0b0100
#define STAP	0b0010
#define DTAP	0b0001

#define OPTIONS_CYBERWARE			(HOLD | STAP		 )
#define OPTIONS_COMBATGADGET		(HOLD | STAP		 )
#define OPTIONS_CONSUMABLE			(HOLD | STAP | DTAP	 )

#define OPTIONS_JUMP				(HOLD | STAP | DTAP	 )
#define OPTIONS_CROUCH				(HOLD | TOGGLE | STAP)
#define OPTIONS_DODGE				(HOLD | STAP | DTAP	 )
#define OPTIONS_SPRINT				(HOLD | TOGGLE | STAP)

#define OPTIONS_PHONE				(HOLD | STAP		 )
#define OPTIONS_CALLCAR				(HOLD | STAP		 )
#define OPTIONS_SCAN				(HOLD | TOGGLE | STAP)
#define OPTIONS_TAG					(HOLD | TOGGLE | STAP)

#define OPTIONS_CHOICE1				(HOLD | STAP		 )
#define OPTIONS_CHOICE2				(HOLD | STAP		 )

#define OPTIONS_NOTIFICATION		(HOLD | STAP		 )

#define	OPTIONS_CYCLEWEAPONS		(HOLD | STAP		 )
#define OPTIONS_PREVIOUSWEAPON		(HOLD | STAP | DTAP	 )
#define OPTIONS_QUICKMELEE			(HOLD | STAP		 )
#define OPTIONS_RELOAD				(HOLD | STAP | DTAP	 )

namespace ControllerMapper
{
	struct XMLCode;

	struct CharacterAction
	{
	public:
		enum class ActionType : char {

			Cyberware,
			CombatGadget,
			Consumable,

			Jump,
			Crouch,
			Dodge,
			Sprint,

			Phone,
			CallCar,
			Scan,
			Tag,

			Choice1,
			Choice2,

			Notification,

			CycleWeapons,
			PreviousWeapon,
			QuickMelee,

			Reload
		};

	private:
		unsigned int Internal_GetSupportedOptions();

	public:
		ActionType actionType;
		unsigned int supportedOptions;
		
		CharacterAction(ActionType actionType);
	};

	struct CharacterOptions
	{
		unsigned int options;
	};

	class Action
	{
	private:
		static bool Internal_VerifyActionSupport(CharacterAction, CharacterOptions);
		void Internal_CreateXMLCodeBlock(CharacterAction, CharacterOptions, bool checkSupport = false);

		Action(CharacterAction, CharacterOptions);

	private:
		CharacterAction m_CharacterAction;
		CharacterOptions m_Options;
		XMLCode m_XMLCode;


	public:

		Action(const Action&) = default;
		Action& operator= (const Action&) = default;
		//Action(const Action&&) = default;
		//Action& operator= (const Action&&) = default;
		~Action() = default;

		CharacterAction GetCharacterAction() const;
		CharacterAction GetCharacterAction();

		CharacterOptions GetOptions() const;
		CharacterOptions GetOptions();

		static Action BuildAction(CharacterAction, CharacterOptions);
		static Action* BuildActionPtr(CharacterAction, CharacterOptions);

		std::string GetXMLCodeUM();
		std::string GetXMLCodeIC();
		XMLCode GetXMLCode();

	public:
		//static bool Static_DoubleTapEnabled(CharacterOptions characterOptions);
	};

	bool IsDoubleTapEnabled(CharacterOptions characterOptions);
	std::string GetXMLBlock(CharacterAction::ActionType actionType);
	std::string GetXMLActionName(CharacterAction::ActionType actionType, bool isToggle = false);

}

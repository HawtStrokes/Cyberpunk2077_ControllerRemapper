#pragma once
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
		CharacterOptions m_CharacterOptions;
		XMLCode m_XMLCode;


	public:
		Action(const Action&) = default;
		Action& operator= (const Action&) = default;
		~Action() = default;

	public:
		// Get Character Action
		CharacterAction GetCharacterAction() const;
		// Get Character Action
		CharacterAction GetCharacterAction();


		// Get Character Options
		CharacterOptions GetCharacterOptions() const;
		// Get Character Options
		CharacterOptions GetCharacterOptions();


		// build action and return copy
		static Action BuildAction(CharacterAction, CharacterOptions);
		// build action in heap and return pointer
		static Action* BuildActionPtr(CharacterAction, CharacterOptions);


		// Get UM xml block
		std::string GetXMLCodeUM();

		// Get IC xml block
		std::string GetXMLCodeIC();

		// Get both UM and IC as an entire struct
		XMLCode GetXMLCode();

	public:
		//static bool Static_DoubleTapEnabled(CharacterOptions characterOptions);
	};


	// Checks if double tap is enabled
	bool IsDoubleTapEnabled(CharacterOptions characterOptions);

	// Get XML Block to be appended to baseUM
	std::string GetXMLBlock(CharacterAction::ActionType actionType);

	// Get the associated xml action name to be formatted into an xml block
	std::string GetXMLActionName(CharacterAction::ActionType actionType, bool isToggle = false);
}

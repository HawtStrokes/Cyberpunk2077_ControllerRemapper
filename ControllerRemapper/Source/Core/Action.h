#pragma once
#include <string>
#include <unordered_map>

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
	class Action
	{
	private:
		static bool Internal_VerifyActionSupport(CharacterAction, CharacterOptions);
		void Internal_CreateXMLCodeBlock(CharacterAction, CharacterOptions, bool checkSupport = false);

		Action(CharacterAction, CharacterOptions);

	private:
		static std::unordered_map<CharacterAction, Action*> m_ActionCache;
		CharacterAction m_CharacterAction;
		CharacterOptions m_CharacterOptions;
		std::string m_XMLCodeUM;

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


		// build or find action in heap and return pointer
		static Action* GetActionPtr(CharacterAction, CharacterOptions);

		// build or find action in heap and return pointer
		static Action* GetActionPtr(CharacterAction);


		// Get UM xml block
		std::string GetXMLCodeUM();

		// Get IC xml block
		std::string GetXMLCodeIC();

		std::string GetXMLActionMapName();

		std::string GetXMLActionName();

	public:
		// Checks if double tap is enabled
		static inline bool IsDoubleTap(CharacterOptions characterOptions) 
		{
			// Read first bit (which is either a 1 or 0)
			return (static_cast<unsigned>(static_cast<unsigned char>(characterOptions.options << 7)) >> 7);
		}

		// Checks if Toggle is enabled
		static inline bool IsToggle(CharacterOptions characterOptions)
		{
			return ((characterOptions.options >> 2) == 1);
		}
	};

	//// Get XML Block to be appended to baseUM
	//std::string GetXMLBlock(CharacterAction::ActionType actionType);

	//// Get the associated xml action name to be formatted into an xml block
	

}

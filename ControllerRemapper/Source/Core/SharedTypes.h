#pragma once
#include <string>

namespace ControllerMapper {

	struct XMLCode {
		std::string UM;
		std::string IC;
	};

	struct CharacterAction {
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
			SwitchItem,
			QuickMelee,

			Reload
		};

	private:
		unsigned int Internal_GetSupportedOptions();

	public:
		bool operator==(const CharacterAction&) const;

	public:
		ActionType actionType;
		unsigned int supportedOptions;

		CharacterAction(ActionType actionType);
	};

	struct CharacterOptions {
		unsigned int options;
	};;
}

template <>
struct std::hash<ControllerMapper::CharacterAction> {
	std::size_t operator()(const ControllerMapper::CharacterAction& key) const noexcept
	{
		return static_cast<unsigned>(key.actionType);
	}
};

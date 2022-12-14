#pragma once
#include <string>
#include <unordered_map>

namespace ControllerMapper {
	enum class ControllerKey : char {
		None,
		LeftThumb,
		RightThumb,

		LeftTrigger,
		RightTrigger,

		LeftShoulder,
		RightShoulder,

		X_SQUARE,
		Y_TRIANGLE,
		A_CROSS,
		B_CIRCLE,

		DigitUp,
		DigitDown,
		DigitLeft,
		DigitRight
	};

	inline std::string GetControllerKeyXML(ControllerKey cK) {
		static std::unordered_map<ControllerKey, std::string> mapXMLName = {
			{ControllerKey::A_CROSS,		"IK_Pad_A_CROSS"},
			{ControllerKey::B_CIRCLE,		"IK_Pad_B_CIRCLE"},
			{ControllerKey::X_SQUARE,		"IK_Pad_X_SQUARE"},
			{ControllerKey::Y_TRIANGLE,		"IK_Pad_Y_TRIANGLE"},
			{ControllerKey::DigitUp,		"IK_Pad_DigitUp"},
			{ControllerKey::DigitDown,		"IK_Pad_DigitDown"},
			{ControllerKey::DigitLeft,		"IK_Pad_DigitLeft"},
			{ControllerKey::DigitRight,		"IK_Pad_DigitRight"},
			{ControllerKey::RightShoulder,	"IK_Pad_RightShoulder"},
			{ControllerKey::LeftShoulder,	"IK_Pad_LeftShoulder"},
			{ControllerKey::RightTrigger,	"IK_Pad_RightTrigger"},
			{ControllerKey::LeftTrigger,	"IK_Pad_LeftTrigger"},
			{ControllerKey::LeftThumb,		"IK_Pad_LeftThumb"},
			{ControllerKey::RightThumb,		"IK_Pad_RightThumb"},
			{ ControllerKey::None,		"NONE" }
		};
		return mapXMLName.at(cK);
	}


	class IButton {
	public:
		virtual bool operator==(const IButton* other) const = 0;
		virtual std::string GetXMLName() = 0;
		virtual ~IButton() = default;
	};
}
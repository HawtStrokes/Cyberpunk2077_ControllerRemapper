#include "ButtonCombo.h"
// #include <HawtLib/Crypto/Hashing.h>

namespace ControllerMapper
{
	std::string ButtonCombo::GetXMLName()
	{
		return m_XMLName;
	}

	bool ButtonCombo::operator==(const IButton* other) const
	{
		const auto otherPtr = dynamic_cast<const ButtonCombo*>(other);	 // check if other is of the same type
		if (otherPtr != nullptr) {
			return ((this->m_Key1 == otherPtr->m_Key1) && this->m_Key2 == otherPtr->m_Key2);	// return true if all keys are the same
		}
		return false;	// not the same
	}

	ButtonCombo::ButtonCombo(ControllerKey key1, ControllerKey key2) : m_Key1(key1), m_Key2(key2) {
		m_XMLName = "Custom_" + GetControllerKeyXML(key1) + GetControllerKeyXML(key2);
	}
}

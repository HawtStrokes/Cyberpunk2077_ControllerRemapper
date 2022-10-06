#include "ButtonSingle.h"

#include <HawtLib/Crypto/Hashing.h>

namespace ControllerMapper
{
	std::string ButtonSingle::GetXMLName()
	{
		return m_XMLName;
	}

	bool ButtonSingle::operator==(const IButton* other) const
	{
		const auto otherPtr = dynamic_cast<const ButtonSingle*>(other);	 // check if other is of the same type
		if (otherPtr != nullptr) {
			return this->m_Key == otherPtr->m_Key;	// return true if all keys are the same
		}
		return false;	// not the same
	}

	//size_t ButtonSingle::Hash()
	//{
	//	return HawtLib::Crypto::DJB2(m_XMLName);	// Hash the xml name
	//}

	//size_t ButtonSingle::Hash() const
	//{
	//	return HawtLib::Crypto::DJB2(m_XMLName);	// Hash the xml name
	//}

	ButtonSingle::ButtonSingle(ControllerKey key) : m_Key(key) {
		m_XMLName = GetControllerKeyXML(key);
	}
	ControllerKey ButtonSingle::GetKey() const {
		return m_Key;
	}
}

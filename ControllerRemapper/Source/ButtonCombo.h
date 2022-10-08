#pragma once

#include "IButton.h"

namespace ControllerMapper {
	class ButtonCombo : public IButton
	{
	private:
		ControllerKey m_Key1;
		ControllerKey m_Key2;
		std::string m_XMLName;

	public:
		std::string GetXMLName() override;

		bool operator==(const IButton* other) const override;

	public:
		ButtonCombo(ControllerKey key1, ControllerKey key2);

		ControllerKey GetKey1() const {
			return m_Key1;
		}

		ControllerKey GetKey2() const {
			return m_Key2;
		}

	};
}

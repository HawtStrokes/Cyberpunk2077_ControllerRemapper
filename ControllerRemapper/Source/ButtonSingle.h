#pragma once

#include "IButton.h"

namespace ControllerMapper {
	class ButtonSingle : public IButton
	{
	private:
		ControllerKey m_Key;
		std::string m_XMLName;

	public:
		std::string GetXMLName() override;
		//void SetXMLName(const std::string& xmlName) override;

		bool operator==(const IButton* other) const override;

		size_t Hash() override;
		size_t Hash() const override;

	public:
		//// deprecated
		//ButtonSingle(ControllerKey key, std::string xmlName);
		ButtonSingle(ControllerKey key);

		ControllerKey GetKey() const;
	};
}

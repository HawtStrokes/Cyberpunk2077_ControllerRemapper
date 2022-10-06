#include <iostream>

#include "Card.h"
#include "../Text/TextFormatting.h"

namespace HawtLib {

	namespace Tables {
		Card::Card(Table* parent, const std::string& text, TxtAlignment txtAlignment, char Bcap)
			: parentT(parent), text(text), txtAlignment(txtAlignment), Bcap(Bcap) {}

		void Card::Print() {
			
			if (txtAlignment == TxtAlignment::Left) {
				printf("%c%s%s", Bcap, text.c_str(), std::string(parentT->_GetExtraSpaces(text.size()), ' ').c_str());
			}
			else if (txtAlignment == TxtAlignment::Center) {
				printf("%c%s%s", Bcap,
					Text::CenterText(text, parentT->GetBiggestCardLength()).c_str(), "");
			}
			else {	// Right
				printf("%c%s%s", Bcap,
					Text::RightText(text, parentT->GetBiggestCardLength()).c_str(), "");
			}
		}
	}
}


#include <iostream>

#include "Table.h"
#include "Card.h"

namespace HawtLib {

	namespace Tables {
		Table::Table(TxtAlignment txtAlignment, char Ecap) : m_Ecap(Ecap), m_TxtAlignment(txtAlignment), m_BiggestCardLength(0) {}



		void Table::Row::Print() {
			for (size_t i = 0; i < Cards.size(); ++i) {
				Cards[i]->txtAlignment = Cards[i]->parentT->m_TxtAlignment;
				Cards[i]->Print();
			}
			std::cout << std::endl;
		}

		Table::Row::~Row() {
			for (size_t i = 0; i < Cards.size(); ++i) {
				delete Cards[i];
			}
		}



		size_t Table::_GetExtraSpaces(size_t letterCount) {
			return m_BiggestCardLength - letterCount;
		}


		void Table::AddRows(size_t rows) {
			m_Rows.reserve(rows);
			for (size_t i = 0; i < rows; ++i) {
				m_Rows.emplace_back(new Row);
			}
		}

		void Table::PrintTable() const {
			if (!m_Rows.size()) {
				printf("ERROR -> No Entires added in table");
				__debugbreak();
			}
			for (size_t i = 0; i < m_Rows.size(); ++i) {
				_EndRow(i);
				_AddDivider(i);	// not part of m_Rows
				m_Rows[i]->Print();
			}
			_AddDivider(m_Rows.size() - 1);
		}

		void Table::_AddDivider(size_t rowIdx) const{
			//std::string((m_BiggestCardLength + 1) * (m_Rows[i]->Cards.size() - 1)

			Card* dividerCard = new Card{ m_This, std::string((m_BiggestCardLength), '-'), TxtAlignment::Left,  '+' };
			for (size_t j = 1; j < m_Rows[rowIdx]->Cards.size(); ++j) {
				dividerCard->Print();
			}
			std::cout << "+" << std::endl;
			delete dividerCard;
		}

		Table& Table::AddCard(const std::string& text, size_t rowIdx, TxtAlignment txtAlignment, char bCap) {
			if (rowIdx >= m_Rows.size()) {	// check if out of bounds
				printf("ERROR -> Out of Bounds");
				__debugbreak();
			}

			// check if the card is bigger than the current biggest card
			if (text.size() > m_BiggestCardLength) {
				m_BiggestCardLength = text.size();
			}

			m_Rows[rowIdx]->Cards.emplace_back(new Card{ this, text, txtAlignment, bCap });	// add card to specific row

			

			return *this; // builder, allows .AddCard().AddCard()...
		}

		void Table::_EndRow(size_t rowIdx) const{
			// size is always greater than the max index
			if (rowIdx >= m_Rows.size()) {	// check if out of bounds
				printf("ERROR -> Out of Bounds in line <%d>", __LINE__);
				__debugbreak();
			}

			m_Rows[rowIdx]->Cards.emplace_back(new Card{ m_This, "", TxtAlignment::Left, m_Ecap });
		}

		size_t Table::GetBiggestCardLength() const {
			return m_BiggestCardLength;
		}


		Table::~Table() {
			for (size_t i = 0; i < m_Rows.size(); ++i) {
				delete m_Rows[i];
			}
		}

	}
}


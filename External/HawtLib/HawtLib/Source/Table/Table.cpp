#include "Table.h"
#include "../Text/TextFormatting.h"

#include <iostream>

namespace HawtLib
{
	namespace Tables
	{
		void Table::Cell::Print(size_t bufferSize, const char cap)
		{
			switch (txtAlignment)
			{
			case TextAlignment::LEFT:
				printf("%c%s%s", cap, content.c_str(), std::string(bufferSize - content.size(), ' ').c_str());
				break;
			case TextAlignment::CENTER:
				printf("%c%s", cap, Text::CenterText(content, bufferSize).c_str());
				break;
			case TextAlignment::RIGHT:
				printf("%c%s", cap, Text::RightText(content, bufferSize).c_str());
				break;
			}
		}

		Table::Row::Row(const Table& parentTable) : parentTable(parentTable) {}

		// Add empty cells if needed and print row
		void Table::Row::Print()
		{
			for (size_t i = 0; i < parentTable.m_ColumnLengths.size(); ++i)
			{
				// if still indexable
				if (i < vecCells.size())
					vecCells[i]->Print(parentTable.m_ColumnLengths[i]);

				// else create new cells and print
				else
				{
					Cell cell;
					cell.Print(parentTable.m_ColumnLengths[i]);
				}
			}
		}


		Table::Table(size_t rows)
		{
			m_Rows.reserve(rows);
		}

		void Table::Internal_PrintDivider() {
			Cell dividerCell;
			for (const size_t m_ColumnLength : m_ColumnLengths)
			{
				dividerCell.content = std::string(m_ColumnLength, '-');
				dividerCell.Print(m_ColumnLength, '+');
			}
			std::cout << "+" << std::endl;
		}

		void Table::Internal_CheckColumnLenghts(size_t colIdx, size_t checkSize) {
			if (m_ColumnLengths[colIdx] < checkSize)
				m_ColumnLengths[colIdx] = checkSize;
		}

		Table& Table::AddCell(const std::string& txt, size_t rowIdx, size_t colIdx, TextAlignment txtAlignment)
		{
			// check if we have enough rows; otherwise create new rows
			while (m_Rows.size() <= rowIdx)
			{
				m_Rows.emplace_back(std::make_shared<Row>(*this));	// Add New Rows
			}

			// Check if we have enough columns; otherwise add new cells
			while (m_Rows[rowIdx]->vecCells.size() <= colIdx)
			{
				m_Rows[rowIdx]->vecCells.emplace_back(std::make_shared<Cell>());	// Add New Cells
			}

			for(size_t i = m_ColumnLengths.size(); i < colIdx + 1; ++i)
			{
				m_ColumnLengths.emplace_back(0);
			}

			m_Rows[rowIdx]->vecCells[colIdx]->content = txt;
			m_Rows[rowIdx]->vecCells[colIdx]->txtAlignment = txtAlignment;
			Internal_CheckColumnLenghts(colIdx, txt.size());

			return *this;
		}

		Table& Table::AddCell(const std::string& txt, size_t rowIdx, TextAlignment txtAlignment) {
			// check if we have enough rows; otherwise create new rows
			while (m_Rows.size() <= rowIdx)
			{
				m_Rows.emplace_back(std::make_shared<Row>(*this));	// Add New Rows
			}

			m_Rows[rowIdx]->vecCells.emplace_back(std::make_shared<Cell>());	// Add New Cell to row
			const auto vecCells = m_Rows[rowIdx]->vecCells;

			for(size_t i = m_ColumnLengths.size(); i < vecCells.size(); ++i)
			{
				m_ColumnLengths.emplace_back(0);
			}


			const size_t colIdx = vecCells.size() - 1;
			vecCells[colIdx]->content = txt;
			vecCells[colIdx]->txtAlignment = txtAlignment;
			Internal_CheckColumnLenghts(colIdx, txt.size());

			return *this;
		}

		void Table::PrintTable()
		{
			if (!m_Rows.empty())
			{
				for (const auto& m_Row : m_Rows)
				{
					Internal_PrintDivider();
					m_Row->Print();
					std::cout << '|' << std::endl;
				}
				Internal_PrintDivider();
			}
		}
	}
}

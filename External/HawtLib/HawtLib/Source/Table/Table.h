#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif

#include <vector>
#include <string>
#include <memory>

namespace HawtLib
{
	namespace Tables
	{
		class SPEC Table
		{
		private:
			enum class TextAlignment
			{
				LEFT = 0,
				CENTER,
				RIGHT
			};

			struct Cell
			{
				std::string content;
				TextAlignment txtAlignment = TextAlignment::LEFT;

				// Prints the Cell
				void Print(size_t bufferSize, const char cap = '|');
			};

			struct Row
			{
				const Table& parentTable;
				std::vector<std::shared_ptr<Cell>> vecCells;
				Row(const Table& parentTable);

				// Prints All Cells in a Row
				void Print();
			};

		private:
			std::vector<size_t> m_ColumnLengths;
			std::vector<std::shared_ptr<Row>> m_Rows;

		private:
			// Print Divider Row
			void Internal_PrintDivider();

			// Checks if current column length is still bigger than checkSize; otherwise update to checkSize
			void Internal_CheckColumnLenghts(size_t colIdx, size_t checkSize);

		public:
			Table() = default;
			// Reserve Rows
			Table(size_t rows);

		public:
			Table& AddCell(const std::string& txt, size_t rowIdx, size_t colIdx, TextAlignment = TextAlignment::LEFT);
			Table& AddCell(const std::string& txt, size_t rowIdx, TextAlignment = TextAlignment::LEFT);
			void PrintTable();

		};
	}
}

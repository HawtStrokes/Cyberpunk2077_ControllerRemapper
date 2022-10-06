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


namespace HawtLib {

	namespace Tables {
		struct SPEC Card;

		SPEC enum class TxtAlignment {
			Left, Center, Right
		};

		class SPEC Table {
		private:
			struct Row {
				std::vector<Card*> Cards;

				void Print();

				~Row();
			};


		private:
			std::vector<Row*> m_Rows;

			TxtAlignment m_TxtAlignment;
			char m_Ecap;	// character to add at the end of a row

			size_t m_BiggestCardLength;
			Table* m_This = this;

		private:
			size_t _GetExtraSpaces(size_t letterCount);
			void _AddDivider(size_t rowIdx) const;
			void _EndRow(size_t rowIdx) const;
		public:

			friend struct Card;

			Table(TxtAlignment txtAlignment = TxtAlignment::Left, char Ecap = '|');

			void AddRows(size_t rows);

			void PrintTable() const;

			Table& AddCard(const std::string& text, size_t rowIdx,TxtAlignment txtAlignment = TxtAlignment::Left, char bCap = '|');

			size_t GetBiggestCardLength() const;

			void SetTextAlignment(TxtAlignment txtAlignment) {
				m_TxtAlignment = txtAlignment;
			}

			~Table();
		};

	}
}


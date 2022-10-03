#pragma once

#if defined(BUILD_DLL)
#define SPEC __declspec(dllexport)
#elif defined(BUILD_STATIC)
#define SPEC
#else
#define SPEC __declspec(dllimport)
#endif

#include "Table.h"

namespace HawtLib {

	namespace Tables {
		struct SPEC Card {
			Table* parentT;
			std::string text;
			TxtAlignment txtAlignment;
			char Bcap;
			
			explicit Card(Table* parent,
				const std::string& text,
				TxtAlignment txtAlignmnt = TxtAlignment::Left,
				char Bcap = '|');

			void Print();
		};
	}
}

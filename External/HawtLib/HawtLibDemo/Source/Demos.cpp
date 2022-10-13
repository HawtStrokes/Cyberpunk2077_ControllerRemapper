#include <iostream>
#include <fstream>

#include "HawtLib/HawtLib.h"


using namespace HawtLib;

void IniParserDemo() {
	std::cout << __FUNCTION__ << std::endl;
	std::ofstream ofs("sample.ini");
	std::string iniContent = 
R"(; last modified 1 April 2001 by John Doe
[owner]
name = John Doe
organization = Acme Widgets Inc.

[database]
; use IP address in case network name resolution is not working
server = 192.0.2.62     
port = 143
file = "payroll.dat"
)";
	ofs << iniContent;
	ofs.close();
	try
	{
		HawtLib::File::IniFile iniFile("sample.ini");

		std::vector<std::string> sectionNames = iniFile.GetSectionNames();
		for (std::string& sectionName_ptr : sectionNames) {
			std::cout << sectionName_ptr << std::endl;
			auto keyValues = iniFile.GetSectionKV(sectionName_ptr);
			for (HawtLib::File::KeyValue<std::string, std::string>* kv : keyValues->keyValues) {
				std::cout << "Key: " << kv->key << std::endl << "Value: " << kv->value  << "[.]" << std::endl;
			}
			std::cout << "------------------" << std::endl;
		}

		HawtLib::File::Persistence::IniSave(iniFile, "persistenceTest.ini");
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}


	std::cin.get();
}

void TableDemo() {
	std::cout << __FUNCTION__ << std::endl;
	Tables::Table table(1);				// Init Table obj

	table.AddCell("Test", 0, 0)
		.AddCell("Test", 1, 0)
		.AddCell("Hello World", 2, 1)
		.AddCell("WOW", 1, 2)
		.AddCell("NewTest", 0)
		.AddCell("NewTest", 0)
		.AddCell("NewTest", 0)
		.AddCell("NewTest", 0)
		.AddCell("NewTest", 11, 5);


	table.PrintTable();			// Print Table
	std::cin.get();
}

void TextFormatDemo() {
	std::cout << __FUNCTION__ << std::endl;
	std::string test = "Hello";
	auto x = Text::CenterText(test);
	std::cout << x << std::endl;
	test = "   hello";
	x = Text::TrimLead(test);
	std::cout << x << std::endl;
	test = "hello       ";
	x = Text::TrimTrail(test);
	std::cout << x << std::endl;
	test = "   hello     ";
	x = Text::Trim(test);
	std::cout << x << std::endl;
	std::cin.get();
}

void CryptoDemo() {
	std::cout << __FUNCTION__ << std::endl;
	std::cout << Crypto::DJB2("Hello World") << std::endl;
	std::cout << Crypto::SDBM("Hello World") << std::endl;
	std::cin.get();
}

int main() {
	TableDemo();
	TextFormatDemo();
	CryptoDemo();
	IniParserDemo();
}


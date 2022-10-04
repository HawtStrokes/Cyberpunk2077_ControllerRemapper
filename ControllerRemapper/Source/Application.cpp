#include "ControllerMapper.h"
#include "ButtonSingle.h"
#include "ButtonCombo.h"
#include "PersistenceManager.h"

#ifdef GUI

	// Add WXWidgets Code here

#else
#include <iostream>

namespace cM = ControllerMapper;

int main()
{
	std::cout << "Enter Game Dir: ";
	std::string userInput;
	std::getline(std::cin, userInput);
	cM::PersistenceManager::Get().SetGameDir(userInput);
	std::cout << cM::PersistenceManager::Get().GetGameDir() << std::endl;
	std::cin.get();
}

#endif
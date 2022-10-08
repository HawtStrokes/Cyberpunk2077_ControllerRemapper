#pragma once
#include <exception>

namespace ControllerMapper
{
	class ConfigurationCorruptedError: public std::exception
	{
	public:
		ConfigurationCorruptedError() : exception("Ini Configuration Corrupted!") {}
	};

	class InvalidGameDir : public std::exception {
	public:
		InvalidGameDir() : exception("Invalid Game Directory!") {}
	};

	class InvalidCharacterOptions : public std::exception
	{
	public:
		InvalidCharacterOptions() : exception("Invalid CharacterOptions For Corresponding CharacterActio!") {}
	};

	class ConfigurationDoesNotExist : public std::exception
	{
	public:
		ConfigurationDoesNotExist() : exception("Configuration Does Not Exist!") {}
	};

	class DirectoryIsDisallowed : public std::exception
	{
	public:
		DirectoryIsDisallowed() : exception("Input Is Not A File!") {}
	};
}
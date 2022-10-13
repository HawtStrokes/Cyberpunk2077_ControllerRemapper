#pragma once
#include <exception>
#include <string>

namespace ControllerMapper
{
	class ConfigurationCorruptedError: public std::exception
	{
	public:
		ConfigurationCorruptedError() : exception("Ini Configuration Corrupted!") {}
		ConfigurationCorruptedError(const std::string& msg) : exception(msg.c_str()) {}
	};

	class InvalidGameDir : public std::exception {
	public:
		InvalidGameDir() : exception("Invalid Game Directory!") {}
		InvalidGameDir(const std::string& msg) : exception(msg.c_str()) {}
	};

	class InvalidCharacterOptions : public std::exception
	{
	public:
		InvalidCharacterOptions() : exception("Invalid CharacterOptions For Corresponding CharacterAction!") {}
		InvalidCharacterOptions(const std::string& msg) : exception(msg.c_str()) {}
	};

	class ConfigurationDoesNotExist : public std::exception
	{
	public:
		ConfigurationDoesNotExist() : exception("Configuration Does Not Exist!") {}
		ConfigurationDoesNotExist(const std::string& msg) : exception(msg.c_str()) {}
	};

	class DirectoryIsDisallowed : public std::exception
	{
	public:
		DirectoryIsDisallowed() : exception("Input Is Not A File!") {}
		DirectoryIsDisallowed(const std::string& msg) : exception(msg.c_str()) {}
	};
}
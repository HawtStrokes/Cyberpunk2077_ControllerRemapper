#pragma once
#include <exception>

namespace ControllerMapper
{
	class ConfigurationCorruptedError: std::exception
	{
	public:
		ConfigurationCorruptedError() : exception("Ini Configuration Corrupted!") {}
	};

	class InvalidGameDir : std::exception {
	public:
		InvalidGameDir() : exception("Invalid Game Directory!") {}
	};

	class InvalidCharacterOptions : std::exception
	{
	public:
		InvalidCharacterOptions() : exception("Invalid CharacterOptions For Corresponding CharacterAction") {}
	};
}
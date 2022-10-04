#pragma once

#include <unordered_map>
#include <memory>

namespace  ControllerMapper {

	class IButton;
	class Action;

	class ControllerMapper
	{
	private:
		std::unordered_map<IButton*, Action*> m_Binds;
		ControllerMapper() = default;
		~ControllerMapper();

	public:
		void BuildXML(const std::string& file);
		void Bind(IButton*, Action*);
		void _CleanUp();

		friend class PersistenceManager;

		static ControllerMapper& Get();
	};
}

#pragma once

#include <unordered_map>
#include <memory>

namespace  ControllerMapper {

	class IButton;
	class Action;

	class ActionButtonBinder
	{
	private:
		//std::unordered_map<Action*, IButton*> m_Binds;
		std::unordered_map<Action*, std::shared_ptr<IButton>> m_Binds;
		ActionButtonBinder() = default;
		~ActionButtonBinder();

	public:
		// Returns static instance of class ABB
		static ActionButtonBinder& Get();

		// Builds XML to FileLocation
		void BuildXML(const std::string& file);

		// Binds button shared_ptr to Action*
		void Bind(Action*, std::shared_ptr<IButton>);

		// Deletes Action_ptrs
		void _CleanUp();

		// returns binds hash map
		std::unordered_map<Action*, std::shared_ptr<IButton>>& GetBinds();

		friend class PersistenceManager;

	};
}


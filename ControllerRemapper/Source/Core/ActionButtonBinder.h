#pragma once

#include <unordered_map>
#include <memory>

namespace  ControllerMapper {

	class IButton;
	class Action;
	struct CharacterAction;

	class ActionButtonBinder
	{
	private:
		void Internal_Reset();

		// Bind Action to Button (will create new keys)
		void Internal_Bind(Action*, std::shared_ptr<IButton>);

		// Deletes Action_ptrs
		void Internal_CleanUp();

	private:
		std::unordered_map<Action*, std::shared_ptr<IButton>> m_Binds;
		ActionButtonBinder();
		~ActionButtonBinder();

	public:
		// Returns static instance of class ABB
		static ActionButtonBinder& Get();

		// Builds XML to FileLocation
		void BuildXML(const std::string& file);

		// Binds button shared_ptr to Action*

		// Binds button shared_ptr to Action* (Will not create new keys)
		void Bind(Action*, std::shared_ptr<IButton>);

		// ResetBinds
		void ResetBinds();

		// returns binds hash map
		std::unordered_map<Action*, std::shared_ptr<IButton>>& GetBinds();

		friend class PersistenceManager;

	};
}


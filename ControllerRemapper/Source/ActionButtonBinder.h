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
		void BuildXML(const std::string& file);
		//void Bind(Action*, IButton*&&);
		void Bind(Action*, std::shared_ptr<IButton>);
		void _CleanUp();
		std::unordered_map<Action*, std::shared_ptr<IButton>>& GetBinds();


		friend class PersistenceManager;

		static ActionButtonBinder& Get();
	};
}


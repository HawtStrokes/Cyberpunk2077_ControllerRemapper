#define FMT_HEADER_ONLY

#include "ActionButtonBinder.h"
#include "Action.h"
#include "ButtonCombo.h"
#include "ButtonSingle.h"
#include "Bases.h"

#include "fmt/format.h"

#include <fstream>
#include <deque>

namespace ControllerMapper
{
	void ActionButtonBinder::BuildXML(const std::string& fileLocation) {

		auto CreateButtonComboXML = [](ButtonCombo& button)->std::string {
			return fmt::format(fmt::runtime( R"(<buttonGroup id="Custom_{}" timeWindow="0.08f" >
        <button id="{}" />
        <button id="{}" />
    </buttonGroup>)"), button.GetXMLName(), GetControllerKeyXML(button.GetKey1()), GetControllerKeyXML(button.GetKey2()));
		};
		auto CreateButtonBindXML = [](IButton* button, Action* action)-> std::string {
			return  fmt::format(fmt::runtime(action->GetXMLCodeUM()), button->GetXMLName());
		};
		auto CreateDoubleTapXML = [](Action* action) -> std::string {
			return fmt::format(fmt::runtime(action->GetXMLCodeIC()), GetXMLActionName(action->GetCharacterAction().actionType));
		};

		std::deque<std::string> vecToUM;
		std::vector<std::string> vecToIC;

		for(auto& kv : m_Binds)
		{
			auto btnSPtr = dynamic_cast<ButtonSingle*>(kv.second.get());
			if (btnSPtr != nullptr)
			{
				vecToUM.push_back(CreateButtonBindXML(btnSPtr, kv.first));
			}
			else
			{
				auto btnDPtr = dynamic_cast<ButtonCombo*>(kv.second.get());
				vecToUM.push_front(CreateButtonComboXML(*btnDPtr));	// push_front custom button layout 
			}

			// check if action supports double tap and if double tap is enabled
			if ((kv.first->GetCharacterAction().actionType == CharacterAction::ActionType::Crouch ||
				kv.first->GetCharacterAction().actionType == CharacterAction::ActionType::Sprint || 
				kv.first->GetCharacterAction().actionType == CharacterAction::ActionType::Scan) && 
				IsDoubleTapEnabled(kv.first->GetOptions()))
			{
				vecToIC.push_back(CreateDoubleTapXML(kv.first));
			}
		}

		// Add to respective files

		std::ofstream ofsUM(fileLocation + "inputUserMappings.xml");
		ofsUM << g_Bases.baseUM;
		for(const auto& str : vecToUM) ofsUM << str;
		ofsUM << "</bindings>";
		ofsUM.close();

		std::ofstream ofsIC(fileLocation + "inputContexts.xml");
		ofsIC << g_Bases.baseIC;
		for (const auto& str : vecToIC) ofsIC << str;
		ofsIC << "</bindings>";
		ofsIC.close();
	}

	//void ActionButtonBinder::Bind(Action* action, IButton*& button)
	//{
	//	__debugbreak();

	//}
	void ActionButtonBinder::Bind(Action* action, std::shared_ptr<IButton> button) {
		//static std::vector<Action*> cachedActionPtrs;
		//static std::vector<IButton*> cachedButtonPtrs;

		//if(std::find(cachedActionPtrs.begin(), cachedActionPtrs.end(), action) != cachedActionPtrs.end())	// we found a matching action*
		//{
		//	// delete former button if not the same as to bind
		//	if(std::find(cachedButtonPtrs.begin(), cachedButtonPtrs.end(), action) == cachedButtonPtrs.end())
		//	{
		//		delete m_Binds.at(action);	// what if this button is bound to another action tho???? TODO: CHECK ON THIS!!! PROPOSAL: GARBAGE COLLECTOR
		//	}

		//	m_Binds[action] = button;

		//	cachedButtonPtrs.push_back(button);
		//	cachedActionPtrs.push_back(action);
		//}
		//else
		//{
		//	m_Binds[action] = button;
		//}
		m_Binds[action] = std::move(button);
		
		//static std::unordered_map<CharacterAction::ActionType, Action*> cachedMap;
		//if(cachedMap.find(action->GetCharacterAction().actionType) == cachedMap.end())
		//{
		//	cachedMap[action->GetCharacterAction().actionType] = action;
		//	m_Binds[action] = button;
		//}
		//else
		//{
		//	delete m_Binds.at(cachedMap[action->GetCharacterAction().actionType]);
		//	m_Binds.at(cachedMap[action->GetCharacterAction().actionType]) = button;
		//	auto x = cachedMap[action->GetCharacterAction().actionType];
		//	cachedMap.erase(action->GetCharacterAction().actionType);
		//	delete action;
		//}
		
	}

	void ActionButtonBinder::_CleanUp() {
		
		// static std::vector<IButton*> cachedMap;
		for (auto& kv : m_Binds)
		{
			delete kv.first;
			// kv.second is a smart pointer, no need to manually free

			//if (std::find(cachedMap.begin(), cachedMap.end(), kv.second) != cachedMap.end())
			//{
			//	//delete kv.second.get();
			//	//cachedMap.push_back(kv.second.get());
			//}
		}
		// cachedMap.clear();
	}

	std::unordered_map<Action*, std::shared_ptr<IButton>>& ActionButtonBinder::GetBinds() {
		return m_Binds;
	}

	ActionButtonBinder::~ActionButtonBinder() {
		_CleanUp();
	}

	ActionButtonBinder& ActionButtonBinder::Get() {
		static ActionButtonBinder instance;
		return instance;
	}
}
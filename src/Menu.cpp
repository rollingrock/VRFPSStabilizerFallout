#include "Menu.h"
#include <vector>
#include <algorithm>

#pragma warning(disable : 4100)

namespace VRFpsStabilizer 
{
	//Menu open event functions
	AllMenuEventHandler menuEvent;

	std::vector<std::string> menus;

	RE::BSEventNotifyControl AllMenuEventHandler::ProcessEvent(const RE::MenuOpenCloseEvent& a_event, RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_source)
	{
		const char* menuName = a_event.menuName.c_str();

		if (a_event.opening) {
			menus.push_back(menuName);
		} 
		else {
			menus.erase(std::remove(menus.begin(), menus.end(), menuName), menus.end());
		}

		return RE::BSEventNotifyControl::kContinue;
	}

	bool isInMenu() {
		return !menus.empty();
	}


}

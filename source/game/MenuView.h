#ifndef GB_MENUVIEW_H
#define GB_MENUVIEW_H

#include "IRenderingSystem.h"
#include "IViewConfiguration.h"
#include <map>

class InputState;
class IRenderCommand;
class MenuLabel;

namespace GBLib
{

class ButtonControl;
class IMenuModel;
class IViewConfiguration;

class MenuView : public IRenderingSystem
{
	public:
		MenuView(IMenuModel & menuModel, IViewConfiguration & viewConfiguration);
		~MenuView();
	
	public:
		Option update(InputState & inputState);
		IRenderCommand * draw();

	public:
		IMenuModel & m_menuModel;
		std::map<Option, ButtonControl *> m_buttonControls;
		std::vector<MenuLabel *> m_textLabels;
};

}

#endif

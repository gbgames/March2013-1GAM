#include "MenuView.h"

#include "ButtonControlConfiguration.h"
#include "ButtonControl.h"
#include "DrawSpriteCommand.h"
#include "InputState.h"
#include "MenuLabel.h"
#include "MenuModel.h"
#include "TextLabelConfiguration.h"

using namespace GBLib;

MenuView::MenuView(IMenuModel & menuModel, IViewConfiguration & viewConfiguration) : 
	m_menuModel(menuModel)
{
	ButtonControlConfigurations buttonControls = viewConfiguration.getButtonControlConfigurations();
	for (ButtonControlConfigurations::iterator iter = buttonControls.begin();
			iter != buttonControls.end();
			++iter)
	{
		Option option = iter->option;
		Rectangle dimensions = iter->buttonDimensionsAndPosition;
		m_buttonControls[option] = new ButtonControl(option,
				iter->disabledSpriteID,
				iter->availableSpriteID,
				iter->hotSpriteID,
				iter->activeSpriteID,
				dimensions.position().X(),
				dimensions.position().Y(),
				dimensions.width(),
				dimensions.height());
	}

	TextLabelConfigurations textLabels = viewConfiguration.getTextWidgets();
	for (TextLabelConfigurations::iterator iter = textLabels.begin(); iter!= textLabels.end(); ++iter)
	{
		TextLabelConfiguration textLabel = *iter;
		m_textLabels.push_back(new MenuLabel(textLabel.text, textLabel.renderPosition, textLabel.fontID, textLabel.textColor.redness, textLabel.textColor.greenness, textLabel.textColor.blueness, textLabel.hotSpot));
	}
}

MenuView::~MenuView()
{
	for (std::map<Option, ButtonControl *>::iterator iter = m_buttonControls.begin(); iter != m_buttonControls.end(); ++iter)
	{
		delete iter->second;
	}
	m_buttonControls.clear();

	while (!m_textLabels.empty())
	{
		delete m_textLabels.back();
		m_textLabels.pop_back();
	}
}


Option MenuView::update(InputState & inputState)
{
	Option option(Option::NO_OPTION);

	for (std::map<Option, ButtonControl *>::iterator iter = m_buttonControls.begin(); iter != m_buttonControls.end(); ++iter)
	{
		if (iter->second->update(inputState))
		{
			option = iter->first;
		}
	}

	return option;
}

IRenderCommand * MenuView::draw()
{
	DrawSpriteCommand * cmd = new DrawSpriteCommand();
	for (std::map<Option, ButtonControl *>::iterator iter = m_buttonControls.begin(); iter != m_buttonControls.end(); ++iter)
	{
		cmd->add(iter->second->draw());
	}
	for (std::vector<MenuLabel *>::iterator iter = m_textLabels.begin(); iter != m_textLabels.end(); ++iter)
	{
		cmd->add((*iter)->draw());
	}

	return cmd;
}

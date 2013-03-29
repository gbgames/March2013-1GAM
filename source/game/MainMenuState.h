#ifndef GB_MAINMENUSTATE_H
#define GB_MAINMENUSTATE_H

#include "IState.h"

#include "SpriteManager.h"

namespace GBLib
{
	class IMenuModel;
	class IRenderingSystem;
	class IViewConfiguration;
}
class IGame;
class ISpriteConfig;
class SpriteImage;

class MainMenuState : public IState
{
	public:
		MainMenuState(IGame & game, ISpriteConfig & spriteConfig, ISpriteManager & spriteManager); 
		virtual ~MainMenuState();
	
	public:
		virtual void onEnter();
		virtual void update(int timeElapsed);
		virtual IRenderCommand * draw(); 
		virtual void onExit(); 

		virtual void onSuspend();
		virtual void onUnsuspend();

		enum MAIN_MENU_OPTION_IDs
		{
			START_GAME = 1,
			QUIT_GAME = 2
		};
	
	public:
		IGame & m_game;
		ISpriteConfig & m_spriteConfig;
		ISpriteManager & m_spriteManager;
		SpriteImage * m_splashScreenSprite;
		bool m_stateSuspended;
		GBLib::IMenuModel * m_menuModel;
		GBLib::IViewConfiguration * m_viewConfiguration;
		GBLib::IRenderingSystem * m_menuView;
		GBLib::IRenderingSystem * m_mouseRenderer;
};

#endif

#ifndef GB_INGAMESTATE_H
#define GB_INGAMESTATE_H

#include "IState.h"
#include "Point.h"

namespace GBLib
{
	class IMenuModel;
	class IRenderingSystem;
	class IViewConfiguration;
}
class IGame;
class ISpriteManager;
class ISpriteConfig;

class InGameState : public IState
{
	public:
		InGameState(IGame & game, ISpriteConfig & spriteConfig, ISpriteManager & spriteManager); 
		virtual ~InGameState();
	
	public:
		virtual void onEnter();
		virtual void update(int timeElapsed);
		virtual IRenderCommand * draw(); 
		virtual void onExit(); 

		virtual void onSuspend();
		virtual void onUnsuspend();
	
	//Custom?
	public:
		void reset();
	
	public:
		IGame & m_game;
		ISpriteConfig & m_spriteConfig;
		ISpriteManager & m_spriteManager;
		bool m_stateSuspended;
		GBLib::IRenderingSystem * m_mouseRenderer;
		GBLib::IMenuModel * m_menuModel;
		GBLib::IViewConfiguration * m_viewConfiguration;
		GBLib::IRenderingSystem * m_menuView;
};

#endif

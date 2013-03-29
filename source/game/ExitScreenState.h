#ifndef GB_EXITSCREENSTATE_H
#define GB_EXITSCREENSTATE_H

#include "IState.h"

#include "SpriteManager.h"

class IGame;
class IVersionInfo;
class SpriteImage;

class ExitScreenState : public IState
{
	public:
		ExitScreenState(IGame & game, ISpriteManager & spriteManager, IVersionInfo & versionInfo);
		virtual ~ExitScreenState();
	
	public:
		virtual void onEnter();
		virtual void update(int timeElapsed);
		virtual IRenderCommand * draw(); 
		virtual void onExit(); 

		virtual void onSuspend();
		virtual void onUnsuspend();
	
	public:
		IGame & m_game;
		ISpriteManager & m_spriteManager;
		IVersionInfo & m_versionInfo;
		SpriteImage * m_exitScreenSprite;
		bool m_hasRendered;
		bool m_stateSuspended;
		bool m_hasShownDemoSalesPage;
};

#endif

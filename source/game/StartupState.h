#ifndef GB_STARTUPSTATE_H
#define GB_STARTUPSTATE_H

#include "IState.h"

class IGame;
class ISpriteConfig;
class ISpriteManager;
class SpriteImage;

class StartupState : public IState
{
	public:
		StartupState(IGame & game, ISpriteConfig & spriteConfig, ISpriteManager & spriteManager);
		virtual ~StartupState();
	
	public:
		virtual void onEnter();
		virtual void update(int timeElapsed);
		virtual IRenderCommand * draw(); 
		virtual void onExit(); 

		virtual void onSuspend();
		virtual void onUnsuspend();
	
	private:
		void configureSplashScreenSpriteConfig();
	
	public:
		IGame & m_game;
		ISpriteConfig & m_spriteConfig;
		ISpriteManager & m_spriteManager;
		SpriteImage * m_splashScreenSprite;
		int m_tickCountdown;
};

#endif

#ifndef GB_MEDIALOADSTATE_H
#define GB_MEDIALOADSTATE_H

#include "IState.h"

#include "SpriteManager.h"

class IGame;
class ISpriteConfig;
class SpriteImage;

class MediaLoadState : public IState
{
	public:
		MediaLoadState(IGame & game, ISpriteConfig & spriteConfig, ISpriteManager & spriteManager); 
		virtual ~MediaLoadState();
	
	public:
		virtual void onEnter();
		virtual void update(int timeElapsed);
		virtual IRenderCommand * draw(); 
		virtual void onExit(); 

		virtual void onSuspend();
		virtual void onUnsuspend();
	
	private:
		void loadFonts();
		void loadSounds();
		void configureSpriteConfig();
		void createSpriteImages();
	
	public:
		IGame & m_game;
		ISpriteConfig & m_spriteConfig;
		ISpriteManager & m_spriteManager;
		SpriteImage * m_splashScreenSprite;
};

#endif

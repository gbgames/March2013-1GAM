#ifndef GB_SDLHARDWARELAYER_H
#define GB_SDLHARDWARELAYER_H

#include "IHardwareLayer.h"

#include "SDL_stdinc.h"
#include "SDL_ttf.h"

#include <map>

struct Mix_Chunk;
struct _Mix_Music;
typedef _Mix_Music Mix_Music;
struct SDL_Surface;

class SDLHardwareLayer : public IHardwareLayer
{
	public:
		SDLHardwareLayer(IKeyboardState & keyboardState);
		virtual ~SDLHardwareLayer();

	public:
		virtual void setScreen(std::string windowTitle,  unsigned int x,  unsigned int y,  unsigned int bitdepth);
		virtual void setAudio(int frequency,  int channels,  int chunkSize);
		virtual void initialize();
		virtual unsigned int getTicks();
		virtual void delay(int delayTime);

		virtual void update();
		virtual Point getMousePosition();
		virtual int getMouseButtonStatus();
		virtual bool isMouseInFocus();
		virtual IKeyboardState & getKeyboardState();
		virtual bool hasQuitEventOccurred();

		virtual void render();
		virtual void renderRectangle(int x,  int y,  int width,  int height,  int redness,  int greenness,  int blueness, std::string dstSprite = "");

		virtual void loadSprite(std::string spriteName,  std::string pathToFile);
		virtual void createSprite(std::string spriteName, Rectangle dimensions);

		virtual void loadFont(std::string fontName, std::string pathToFile, int fontSize);

		virtual Rectangle getSizeOfSprite(std::string spriteName);
		virtual bool isColorInSprite(std::string, int red, int green, int blue, Rectangle portion);
		virtual void renderSprite(std::string spriteName, int xPos, int yPos, std::string dstSprite = "");
		virtual void renderSprite(std::string spriteName, int srcX, int srcY, int srcWidth, int srcHeight, std::string surfaceToRenderTo, int destX, int destY);

		virtual void renderTextToSurface(std::string fontName, std::string textToRender, int redness, int greenness, int blueness, int alphaness, int destX, int destY, std::string surfaceToRenderTo = ""); 

		virtual Rectangle getTextSize(std::string fontName, std::string textToRender); 

		virtual void loadSound(std::string soundID, std::string pathToFile, int volume);
		virtual int playSound(int channelID, std::string soundID, int numLoops);
		virtual void loadMusic(std::string musicID, std::string pathToFile);
		virtual int playMusic(std::string musicID, int numLoops, int timeToFadeIn = 0); 
		virtual void stopMusic(); 
		virtual void setMusicVolume(int volume);
		virtual void fadeOutMusic(int timeToFadeOut); 
		virtual void muteMusic();
		virtual void unmuteMusic();
		virtual bool isMusicMuted();
		virtual void muteSound();
		virtual void unmuteSound();
		virtual bool isSoundMuted();
		virtual void quit();
		virtual void resetQuitStatus();

	public:
		std::string m_title;
		int m_x;
		int m_y;
		int m_bitDepth;

		int m_frequency;
		int m_channels;
		int m_chunkSize;

		SDL_Surface * m_screen;
		std::map<std::string, SDL_Surface *> spriteCollection;
		std::map<std::string, TTF_Font *> fontCollection;
		std::map<std::string, Mix_Chunk *> soundCollection;
		std::map<std::string, Mix_Music *> musicCollection;
		bool m_quitEventOccurred;

		Point m_mousePosition;
		int m_mouseButtonStatus;
		IKeyboardState & m_keyboardState;
		bool m_mouseInFocus;

	private:
		Uint32 getPixelFromSprite(SDL_Surface * surface, int x, int y);
		void setKeyMapping();
};

#endif

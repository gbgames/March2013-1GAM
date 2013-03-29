#ifndef GB_IHARDWARELAYER_H
#define GB_IHARDWARELAYER_H

#include "Point.h"
#include "Rectangle.h"
#include <string>

class IKeyboardState;

class IHardwareLayer
{
	public:
		IHardwareLayer() {}
		virtual ~IHardwareLayer() {}

	public:
		virtual void setScreen(std::string windowTitle, unsigned int x, unsigned int y,  unsigned int bitdepth) = 0;
		virtual void setAudio(int frequency,  int channels,  int chunkSize) = 0;
		virtual void initialize() = 0;
		virtual unsigned int getTicks() = 0;
		virtual void delay(int delayTime) = 0;

		virtual void update() = 0;
		virtual Point getMousePosition() = 0;
		virtual int getMouseButtonStatus() = 0;
		virtual bool isMouseInFocus() = 0;
		virtual IKeyboardState & getKeyboardState() = 0;
		virtual bool hasQuitEventOccurred() = 0;

		virtual void render() = 0;
		virtual void renderRectangle(int x,  int y,  int width,  int height,  int redness,  int greenness,  int blueness, std::string dstSprite = "") = 0;

		virtual void loadSprite(std::string spriteName,  std::string pathToFile) = 0;
		virtual void createSprite(std::string spriteName, Rectangle dimensions) = 0;

		virtual void loadFont(std::string fontName, std::string pathToFile, int fontSize) = 0;

		virtual Rectangle getSizeOfSprite(std::string spriteName) = 0;
		virtual bool isColorInSprite(std::string, int red, int green, int blue, Rectangle portion) = 0;
		virtual void renderSprite(std::string spriteName,  int xPos,  int yPos, std::string dstSprite = "") = 0;
		virtual void renderSprite(std::string spriteName, int srcX, int srcY, int srcWidth, int srcHeight, std::string surfaceToRenderTo, int destX, int destY) = 0;
		
		virtual void renderTextToSurface(std::string fontName, std::string textToRender, int redness, int greenness, int blueness, int alphaness, int destX, int destY, std::string surfaceToRenderTo = "") = 0;

		virtual Rectangle getTextSize(std::string fontName, std::string textToRender) = 0;
		
		virtual void loadSound(std::string soundID, std::string pathToFile, int volume) = 0;
		virtual int playSound(int channelID, std::string soundID, int numLoops) = 0;
		virtual void loadMusic(std::string musicID, std::string pathToFile) = 0;
		virtual int playMusic(std::string musicID, int numLoops, int timeToFadeIn = 0) = 0;
		virtual void stopMusic() = 0;
		virtual void setMusicVolume(int volume) = 0;
		virtual void fadeOutMusic(int timeToFadeOut) = 0;
		virtual void muteMusic() = 0;
		virtual void unmuteMusic() = 0;
		virtual bool isMusicMuted() = 0;
		virtual void muteSound() = 0;
		virtual void unmuteSound() = 0;
		virtual bool isSoundMuted() = 0;
		virtual void quit() = 0;
		virtual void resetQuitStatus() = 0;
};

#endif

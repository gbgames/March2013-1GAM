#include "SDLHardwareLayer.h"

#include "IKeyboardState.h"
#include "ISDLInstanceDelegator.h"
#include "SDLLayerFactory.h"

#include "SDL.h"

#include <sstream>
#include <iostream>

void throwError(std::string message)
{
	std::cerr << message << std::endl;
	throw 1;
}

SDLHardwareLayer::SDLHardwareLayer(IKeyboardState & keyboardState):
	m_title(""),
	m_x(0),
	m_y(0),
	m_bitDepth(0),
	m_frequency(0),
	m_channels(0),
	m_chunkSize(0),
	m_screen(NULL),
	m_quitEventOccurred(false),
	m_mouseButtonStatus(0),
	m_keyboardState(keyboardState),
	m_mouseInFocus(true)
{
}

SDLHardwareLayer::~SDLHardwareLayer()
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (NULL != sdlInstance)
	{
		for (std::map<std::string, SDL_Surface *>::iterator iter = spriteCollection.begin();
				iter != spriteCollection.end();
				++iter)
		{
			sdlInstance->FreeSurface(iter->second);
		}

		for(std::map<std::string, TTF_Font *>::iterator iter = fontCollection.begin(); 
			iter != fontCollection.end(); 
			++iter)
		{
			TTF_Font * font = (*iter).second;
			sdlInstance->TTF_CloseFont(font);
		}
		fontCollection.clear();

		for(std::map<std::string, Mix_Chunk *>::iterator iter = soundCollection.begin(); 
			iter != soundCollection.end(); 
			++iter)
		{
			Mix_Chunk * sound = (*iter).second;
			sdlInstance->freeSound(sound);
		}
		soundCollection.clear();

		for(std::map<std::string, Mix_Music *>::iterator iter = musicCollection.begin(); 
			iter != musicCollection.end(); 
			++iter)
		{
			Mix_Music * music = (*iter).second;
			sdlInstance->freeMusic(music);
		}
		musicCollection.clear();

		sdlInstance->CloseAudio();
		sdlInstance->SDL_Quit();
	}
}

void SDLHardwareLayer::setScreen(std::string windowTitle, unsigned int x, unsigned int y, unsigned int bitdepth)
{
	m_title = windowTitle;
	m_x = x;
	m_y = y;
	m_bitDepth = bitdepth;
}

void SDLHardwareLayer::setAudio(int frequency, int channels, int chunkSize)
{
	m_frequency = frequency;
	m_channels = channels;
	m_chunkSize = chunkSize;
}

void SDLHardwareLayer::initialize()
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (NULL == sdlInstance)
	{
		throwError("No SDL Instance!");
	}

	const SDL_version * sdlVersion = sdlInstance->SDL_Linked_Version();
	if ((NULL != sdlVersion) && (sdlVersion->minor < 2 && sdlVersion->major < 2))
	{
		//TODO Error
	}
	else if (sdlInstance->SDL_Init(SDL_INIT_VIDEO | 
				SDL_INIT_AUDIO |
				SDL_INIT_NOPARACHUTE) < 0)
	{
		//TODO Error
	}
	else 
	{
		m_screen = sdlInstance->SDL_SetVideoMode(m_x, m_y, m_bitDepth, SDL_SWSURFACE | SDL_ASYNCBLIT | SDL_DOUBLEBUF | SDL_ANYFORMAT | SDL_SRCALPHA); // | SDL_FULLSCREEN);
		if (NULL != m_screen)
		{
			sdlInstance->SDL_WM_SetCaption(m_title.c_str(), 0);
			sdlInstance->TTF_Init();
			sdlInstance->ShowCursor(SDL_DISABLE);

			int frequency = 44100; //sdlInstance->defaultAudioFrequency();
			int format = sdlInstance->defaultAudioFormat();
			int numChannels = sdlInstance->defaultAudioChannels();
			int chunkSize = sdlInstance->defaultChunkSize();
			int audioInitialized = sdlInstance->OpenAudio(frequency, format, numChannels, chunkSize);
			if (!audioInitialized)
			{
				//TODO Error
			}
			else
			{
			}
		}

		setKeyMapping();
	}
}

unsigned int SDLHardwareLayer::getTicks()
{
	unsigned int ticks(0);
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance)
	{
		ticks = sdlInstance->GetTicks();
	}

	return ticks;
}

void SDLHardwareLayer::delay(int delayTime)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance)
	{
		sdlInstance->Delay(delayTime);
	}
}

void SDLHardwareLayer::update()
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();

	if (NULL != sdlInstance)
	{
		SDL_Event event;
		while (1 == sdlInstance->PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_QUIT:
					{
						quit();
					}
					break;

				case SDL_MOUSEMOTION:
					{
						m_mousePosition.setPosition(event.motion.x, event.motion.y);
					}
					break;

				case SDL_MOUSEBUTTONUP:
					{
						if (1 == event.button.button)
						{
							m_mouseButtonStatus = 0;
						}
					}
					break;

				case SDL_MOUSEBUTTONDOWN:
					{
						if (1 == event.button.button)
						{
							m_mouseButtonStatus = 1;
						}
					}
					break;

				case SDL_APPMOUSEFOCUS:
					{
						m_mouseInFocus = event.active.gain;
					}
					break;

				case SDL_KEYDOWN:
					{
						m_keyboardState.setKeyDown(event.key.keysym.sym);
					}
					break;

				case SDL_KEYUP:
					{
						m_keyboardState.setKeyUp(event.key.keysym.sym);
					}
					break;

				default:
					break;
			}
		}
	}
}

Point SDLHardwareLayer::getMousePosition()
{
	return m_mousePosition;
}

int SDLHardwareLayer::getMouseButtonStatus()
{
	return m_mouseButtonStatus;
}

bool SDLHardwareLayer::isMouseInFocus()
{
	return m_mouseInFocus;
}

IKeyboardState & SDLHardwareLayer::getKeyboardState()
{
	return m_keyboardState;
}

bool SDLHardwareLayer::hasQuitEventOccurred()
{
	return m_quitEventOccurred;
}

void SDLHardwareLayer::render()
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (NULL != sdlInstance && NULL != m_screen)
	{
		sdlInstance->SDL_Flip(m_screen);	
	}
}

void SDLHardwareLayer::renderRectangle(int x, int y, int width, int height, int redness, int greenness, int blueness, std::string dstSprite)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (NULL != sdlInstance)// && NULL != m_screen)
	{
		SDL_Surface * canvas = m_screen;
		if (NULL != spriteCollection[dstSprite])
		{
			canvas = spriteCollection[dstSprite];
		}
		
		Uint32 color = 0;
		if (NULL != canvas->format)
		{
			color = sdlInstance->SDL_MapRGB(canvas->format, redness, greenness, blueness);
		}

		SDL_Rect rect = {x, y, width, height};
		sdlInstance->SDL_FillRect(canvas, &rect, color); 
	}
}

void SDLHardwareLayer::loadSprite(std::string spriteName, std::string pathToFile)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	SDL_Surface * sprite(NULL);
	if (NULL != sdlInstance && NULL == spriteCollection[spriteName])
	{
		sprite = sdlInstance->IMG_Load(pathToFile.c_str());
		if (NULL != sprite)
		{
			spriteCollection[spriteName] = sprite;
		}
		else
		{
			//TODO: I'd like this to log somewhere instead of outputting to cerr by default.
			//std::cerr << "Error loading " << spriteName << " from " << pathToFile << ": " << std::endl;
			//std::cerr << sdlInstance->GetError() << std::endl;
			std::stringstream errorMessage;
			errorMessage << "Error loading " << spriteName << " from " << pathToFile << ": " << std::endl;
			errorMessage << sdlInstance->GetError();
			throwError(errorMessage.str());
		}
	}
}

void SDLHardwareLayer::createSprite(std::string spriteName, Rectangle dimensions)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (NULL != sdlInstance && 0 != m_screen)
	{
		if (spriteCollection.end() != spriteCollection.find(spriteName))
		{
			sdlInstance->FreeSurface(spriteCollection[spriteName]);
		}
		const SDL_PixelFormat& fmt = *(m_screen->format);
		spriteCollection[spriteName] = sdlInstance->CreateRGBSurface(m_screen->flags, dimensions.width(), dimensions.height(), fmt.BitsPerPixel, fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask);
	}
}

void SDLHardwareLayer::loadFont(std::string fontName, std::string pathToFile, int fontSize)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	TTF_Font * tempFont(0);
	if (0 != sdlInstance && 0 == fontCollection[fontName])
	{
		tempFont = sdlInstance->TTF_OpenFont(pathToFile.c_str(), fontSize);
		if (0 != tempFont)
		{
			fontCollection[fontName] = tempFont;
		}
		else
		{
			//TODO: I'd like this to log somewhere instead of outputting to cerr by default.
			////std::cerr << "Error loading " << fontName << " from " << pathToFile << ": " << std::endl;
			//std::cerr << sdlInstance->GetError() << std::endl;
			std::stringstream errorMessage;
			errorMessage << "Error loading " << fontName << " from " << pathToFile << ": " << std::endl;
			errorMessage << sdlInstance->GetError();
			throwError(errorMessage.str());

		}
	}
}

Rectangle SDLHardwareLayer::getSizeOfSprite(std::string spriteName)
{
	Rectangle spriteDimensions(0, 0, 0, 0);

	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (NULL != sdlInstance && NULL != spriteCollection[spriteName])
	{
		SDL_Surface * sprite = spriteCollection[spriteName];
		spriteDimensions.setWidth(sprite->w);
		spriteDimensions.setHeight(sprite->h);
	}

	return spriteDimensions;
}

bool SDLHardwareLayer::isColorInSprite(std::string spriteName, int red, int green, int blue, Rectangle portion)
{
	bool colorExists = false;
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (NULL != sdlInstance)
	{
		SDL_Surface * surface = spriteCollection[spriteName];
		if (NULL != surface && !sdlInstance->LockSurface(surface))
		{
			int startX = 0;
			int startY = 0;
			int portionWidth = surface->w; 
			int portionHeight = surface->h;
			if (Rectangle(0, 0, 0, 0) != portion)
			{
				startX = portion.position().X();
				startY = portion.position().Y();
				portionWidth = portion.width();
				portionHeight = portion.height();
			}

			Uint32 color = sdlInstance->SDL_MapRGB(surface->format, red, green, blue);
			//Using the w and h on the surface, start going through the pixels.
			for (int width = 0; width < portionWidth && !colorExists; ++width)
			{
				for (int height = 0; height < portionHeight && !colorExists; ++height)
				{
					Uint32 pixelColor = getPixelFromSprite(surface, width + startX, height + startY);
					if (color == pixelColor)
					{
						colorExists = true;
					}
				}
			}
			sdlInstance->UnlockSurface(surface);
		}
	}

	return colorExists;
}

Uint32 SDLHardwareLayer::getPixelFromSprite(SDL_Surface * surface, int x, int y)
{
	/*
	 * Return the pixel value at (x, y)
	 * NOTE: The surface must be locked before calling this!
	 */
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
		case 1:
			return *p;

		case 2:
			return *(Uint16 *)p;

		case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				return p[0] << 16 | p[1] << 8 | p[2];
			else
				return p[0] | p[1] << 8 | p[2] << 16;

		case 4:
			return *(Uint32 *)p;

		default:
			return 0;       /* shouldn't happen, but avoids warnings */
	}

	return 0;
}

void SDLHardwareLayer::renderSprite(std::string spriteName, int xPos, int yPos, std::string dstSprite)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (NULL != sdlInstance && NULL != spriteCollection[spriteName])
	{
		SDL_Surface * canvas = m_screen;
		if (NULL != spriteCollection[dstSprite])
		{
			canvas = spriteCollection[dstSprite];
		}
		SDL_Rect destinationPosition = {xPos, yPos, 0, 0};
		sdlInstance->BlitSurface(spriteCollection[spriteName], NULL, canvas, &destinationPosition);
	}
}

void SDLHardwareLayer::renderSprite(std::string spriteName, int srcX, int srcY, int srcWidth, int srcHeight, std::string surfaceToRenderTo, int destX, int destY)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (NULL != sdlInstance && NULL != spriteCollection[spriteName])
	{
		SDL_Surface * canvas = m_screen;
		if (NULL != spriteCollection[surfaceToRenderTo])
		{
			canvas = spriteCollection[surfaceToRenderTo];
		}
		SDL_Rect sourceDimensions = {srcX, srcY, srcWidth, srcHeight};
		SDL_Rect destinationPosition = {destX, destY, 0, 0};
		sdlInstance->BlitSurface(spriteCollection[spriteName], &sourceDimensions, canvas, &destinationPosition);
	}
}

void SDLHardwareLayer::renderTextToSurface(std::string fontName, std::string textToRender, int redness, int greenness, int blueness, int alphaness, int destX, int destY, std::string surfaceToRenderTo)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance && 0 != fontCollection[fontName])
	{
		SDL_Surface * canvas = m_screen;
		if (0 != surfaceToRenderTo.compare("") && 0 != spriteCollection[surfaceToRenderTo])
		{
			canvas = spriteCollection[surfaceToRenderTo];
		}

		if (0 != canvas) 
		{
			SDL_Color color = {redness, greenness, blueness, alphaness};
			SDL_Surface * surface = sdlInstance->TTF_RenderText_Solid(fontCollection[fontName], textToRender.c_str(), color);

			SDL_Rect destinationPosition = {destX, destY, 0, 0};
			sdlInstance->BlitSurface(surface, NULL, canvas, &destinationPosition);
			sdlInstance->FreeSurface(surface);
		}
	}
}

Rectangle SDLHardwareLayer::getTextSize(std::string fontName, std::string textToRender)
{
	Rectangle textDimensions(0, 0, 999, 999);
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance && 0 != fontCollection[fontName])
	{
		int width, height;
		sdlInstance->TTF_SizeText(fontCollection[fontName], textToRender, &width, &height);
		textDimensions.setWidth(width);
		textDimensions.setHeight(height);
	}

	return textDimensions;

}

void SDLHardwareLayer::loadSound(std::string soundID, std::string pathToFile, int volume)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance && 0 == soundCollection[soundID])
	{
		Mix_Chunk * chunk = sdlInstance->loadAudioFile(pathToFile);
		if (chunk != 0)
		{
			sdlInstance->setSoundVolume(chunk, volume);
			soundCollection[soundID] = chunk;
		}
		else
		{
			//TODO: I'd like this to log somewhere instead of outputting to cerr by default.
			////std::cerr << "Error loading " << soundID << " from " << pathToFile << ": " << std::endl;
			//std::cerr << sdlInstance->GetError() << std::endl;
			std::stringstream errorMessage;
			errorMessage << "Error loading " << soundID << " from " << pathToFile << ": " << std::endl;
			errorMessage << sdlInstance->GetError();
			throwError(errorMessage.str());
		}
	}

}

int SDLHardwareLayer::playSound(int channelID, std::string soundID, int numLoops)
{
	int playedChannel(-1);
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance && 0 != soundCollection[soundID])
	{
		playedChannel = sdlInstance->playChannel(channelID, soundCollection[soundID], numLoops);
	}

	return playedChannel;
}
		
void SDLHardwareLayer::loadMusic(std::string musicID, std::string pathToFile)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance && 0 == musicCollection[musicID])
	{
		Mix_Music * music = sdlInstance->loadMusicFile(pathToFile);
		if (music != 0)
		{
			musicCollection[musicID] = music;
		}
		else
		{
			//TODO: I'd like this to log somewhere instead of outputting to cerr by default.
			////std::cerr << "Error loading " << musicID << " from " << pathToFile << ": " << std::endl;
			//std::cerr << sdlInstance->GetError() << std::endl;
			std::stringstream errorMessage;
			errorMessage << "Error loading " << musicID << " from " << pathToFile << ": " << std::endl;
			errorMessage << sdlInstance->GetError();
			throwError(errorMessage.str());
		}
	}
}

int SDLHardwareLayer::playMusic(std::string musicID, int numLoops, int timeToFadeIn)
{
	int success(-1);
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance && 0 != musicCollection[musicID])
	{
		success = sdlInstance->playMusic(musicCollection[musicID], numLoops, timeToFadeIn);
	}

	return success;
}

void SDLHardwareLayer::stopMusic()
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance) 
	{
		sdlInstance->haltMusic();
	}
}

void SDLHardwareLayer::setMusicVolume(int volume)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance)
	{
		sdlInstance->volumeMusic(volume);
	}
}

void SDLHardwareLayer::fadeOutMusic(int timeToFadeOut)
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance)
	{
		sdlInstance->fadeOutMusic(timeToFadeOut);
	}
}

void SDLHardwareLayer::muteMusic()
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance)
	{
		sdlInstance->pauseMusic();
	}
}

void SDLHardwareLayer::unmuteMusic()
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance)
	{
		sdlInstance->resumeMusic();
	}
}

bool SDLHardwareLayer::isMusicMuted()
{
	bool isMuted(false);
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance)
	{
		isMuted = sdlInstance->pausedMusic();
	}

	return isMuted;
}

void SDLHardwareLayer::muteSound()
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance)
	{
		sdlInstance->setChannelSoundVolume(-1, 0);
	}
}

void SDLHardwareLayer::unmuteSound()
{
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance)
	{
		sdlInstance->setChannelSoundVolume(-1, 128);
	}
}

bool SDLHardwareLayer::isSoundMuted()
{
	bool isMuted(false);
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	if (0 != sdlInstance)
	{
		isMuted = (sdlInstance->setChannelSoundVolume(-1, -1) == 0);
	}

	return isMuted;
}

void SDLHardwareLayer::quit()
{
	m_quitEventOccurred = true;
	ISDLInstanceDelegator * sdlInstance = SDLLayerFactory::getInstance();
	//TODO: Expose the ShowCursor API.
	sdlInstance->ShowCursor(SDL_ENABLE);
	m_mouseInFocus = false;
}

void SDLHardwareLayer::resetQuitStatus()
{
	m_quitEventOccurred = false;
}

void SDLHardwareLayer::setKeyMapping()
{
	std::map<std::string, int> stringToKey;
    stringToKey["UP"] = SDLK_UP;
    stringToKey["DOWN"] = SDLK_DOWN;
    stringToKey["RIGHT"] = SDLK_RIGHT;
    stringToKey["LEFT"] = SDLK_LEFT;
    stringToKey["BACKSPACE"] = SDLK_BACKSPACE;
    stringToKey["TAB"] = SDLK_TAB;
    stringToKey["RETURN"] = SDLK_RETURN;
    stringToKey["ESC"] = SDLK_ESCAPE;
    stringToKey["SPACE"] = SDLK_SPACE;
    stringToKey["PLUS"] = SDLK_EQUALS;
    stringToKey["MINUS"] = SDLK_MINUS;
    stringToKey["0"] = SDLK_0;
    stringToKey["1"] = SDLK_1;
    stringToKey["2"] = SDLK_2;
    stringToKey["3"] = SDLK_3;
    stringToKey["4"] = SDLK_4;
    stringToKey["5"] = SDLK_5;
    stringToKey["6"] = SDLK_6;
    stringToKey["7"] = SDLK_7;
    stringToKey["8"] = SDLK_8;
    stringToKey["9"] = SDLK_9;
    stringToKey["a"] = SDLK_a;
    stringToKey["b"] = SDLK_b;
    stringToKey["c"] = SDLK_c;
    stringToKey["d"] = SDLK_d;
    stringToKey["e"] = SDLK_e;
    stringToKey["f"] = SDLK_f;
    stringToKey["g"] = SDLK_g;
    stringToKey["h"] = SDLK_h;
    stringToKey["i"] = SDLK_i;
    stringToKey["j"] = SDLK_j;
    stringToKey["k"] = SDLK_k;
    stringToKey["l"] = SDLK_l;
    stringToKey["m"] = SDLK_m;
    stringToKey["n"] = SDLK_n;
    stringToKey["o"] = SDLK_o;
    stringToKey["p"] = SDLK_p;
    stringToKey["q"] = SDLK_q;
    stringToKey["r"] = SDLK_r;
    stringToKey["s"] = SDLK_s;
    stringToKey["t"] = SDLK_t;
    stringToKey["u"] = SDLK_u;
    stringToKey["v"] = SDLK_v;
    stringToKey["w"] = SDLK_w;
    stringToKey["x"] = SDLK_x;
    stringToKey["y"] = SDLK_y;
    stringToKey["z"] = SDLK_z;
    stringToKey["INSERT"] = SDLK_INSERT;
    stringToKey["HOME"] = SDLK_HOME;
    stringToKey["END"] = SDLK_END;
    stringToKey["PAGEUP"] = SDLK_PAGEUP;
    stringToKey["PAGEDOWN"] = SDLK_PAGEDOWN;
    stringToKey["F1"] = SDLK_F1;
    stringToKey["F2"] = SDLK_F2;
    stringToKey["F3"] = SDLK_F3;
    stringToKey["F4"] = SDLK_F4;
    stringToKey["F5"] = SDLK_F5;
    stringToKey["F6"] = SDLK_F6;
    stringToKey["F7"] = SDLK_F7;
    stringToKey["F8"] = SDLK_F8;
    stringToKey["F9"] = SDLK_F9;
    stringToKey["F10"] = SDLK_F10;
    stringToKey["F11"] = SDLK_F11;
    stringToKey["F12"] = SDLK_F12;
    stringToKey["RSHIFT"] = SDLK_RSHIFT;
    stringToKey["LSHIFT"] = SDLK_LSHIFT;
    stringToKey["RCTRL"] = SDLK_RCTRL;
    stringToKey["LCTRL"] = SDLK_LCTRL;
    stringToKey["RALT"] = SDLK_RALT;
    stringToKey["LALT"] = SDLK_LALT;
    stringToKey["KP_PLUS"] = SDLK_KP_PLUS;
    stringToKey["KP_MINUS"] = SDLK_KP_MINUS;

	KeyNameMap mapping;
	m_keyboardState.setKeyNameMapping(stringToKey);
}

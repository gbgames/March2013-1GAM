#include "RealSDLInstanceDelegator.h"

#include "SDL.h"
#include "SDL_mixer.h"

#include "lodepng.h"

#include <iostream>
#include <sstream>

RealInstanceDelegator::RealInstanceDelegator()
{
}

RealInstanceDelegator::~RealInstanceDelegator()
{
}

const SDL_version * RealInstanceDelegator::SDL_Linked_Version()
{
	return ::SDL_Linked_Version();
}

int RealInstanceDelegator::SDL_Init(int bitMask)
{
	return ::SDL_Init(bitMask);
}

SDL_Surface * RealInstanceDelegator::SDL_SetVideoMode(int width, int height, int bitDepth, int mode)
{
	return ::SDL_SetVideoMode(width, height, bitDepth, mode);
}

void RealInstanceDelegator::SDL_WM_SetCaption(const char * title, const char * icon)
{
	::SDL_WM_SetCaption(title, icon);
}

void RealInstanceDelegator::SDL_Quit()
{
	::SDL_Quit();
}

int RealInstanceDelegator::SDL_FillRect(SDL_Surface * destination, SDL_Rect * destinationSurface, Uint32 color)
{
	return ::SDL_FillRect(destination, destinationSurface, color);
}

Uint32 RealInstanceDelegator::SDL_MapRGB(SDL_PixelFormat * format, Uint8 red, Uint8 green, Uint8 blue)
{
	return ::SDL_MapRGB(format, red, green, blue);
}


int RealInstanceDelegator::SDL_Flip(SDL_Surface * screen)
{
	return ::SDL_Flip(screen);
}

void RealInstanceDelegator::SDL_UpdateRect(SDL_Surface * screen, Sint32 x, Sint32 y, Sint32 w, Sint32 h)
{
	::SDL_UpdateRect(screen, x, y, w, h);
}
		
SDL_Surface * RealInstanceDelegator::CreateRGBSurface(int flags, int width, int height, int depth, Uint32 RMask, Uint32 GMask, Uint32 BMask, Uint32 AMask)
{
	return ::SDL_CreateRGBSurface(flags, width, height, depth, RMask, GMask, BMask, AMask);
}

void RealInstanceDelegator::FreeSurface(SDL_Surface * surface)
{
	::SDL_FreeSurface(surface);
}

SDL_Surface * RealInstanceDelegator::IMG_Load(const char * filename)
{
	//Using LodePNG instead of SDL_image because of bug with Mac OS X
	//that prevents PNGs from being loaded without corruption.

	std::vector<unsigned char> image;
	unsigned width, height;
  	unsigned error = LodePNG::decode(image, width, height, filename); //load the image file with given filename
	
	SDL_Surface * surface = 0;
	if (error == 0)
	{
		Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif
		int depth = 32;
		surface = CreateRGBSurface(SDL_SWSURFACE, width, height, depth, rmask, gmask, bmask, amask);

		// Lock the surface, then store the pixel data.
		SDL_LockSurface(surface);	

		unsigned char * pixelPointer = static_cast<unsigned char *>(surface->pixels);
		for (std::vector<unsigned char>::iterator iter = image.begin();
					iter != image.end();
					++iter)
		{
			*pixelPointer = *iter;
			++pixelPointer;
		}

		SDL_UnlockSurface(surface);

		SDL_Surface * convertedSurface = SDL_DisplayFormatAlpha(surface);
		if (convertedSurface != NULL)
		{
			SDL_FreeSurface(surface);
			surface = convertedSurface;
		}
	}

	return surface;
}

const char * RealInstanceDelegator::GetError()
{
	return ::SDL_GetError();
}
		
int RealInstanceDelegator::BlitSurface(SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect)
{
	return ::SDL_BlitSurface(src, srcrect, dst, dstrect);
}

int RealInstanceDelegator::PollEvent(SDL_Event * event)
{
	return ::SDL_PollEvent(event);
}

unsigned int RealInstanceDelegator::GetTicks()
{
	return ::SDL_GetTicks();
}

void RealInstanceDelegator::Delay(int milliseconds)
{
	::SDL_Delay(milliseconds);
}

int RealInstanceDelegator::LockSurface(SDL_Surface * surface)
{
	return ::SDL_LockSurface(surface);
}

void RealInstanceDelegator::UnlockSurface(SDL_Surface * surface)
{
	::SDL_UnlockSurface(surface);
}

std::string RealInstanceDelegator::SDL_GetKeyName(int keyIndex)
{
	return ::SDL_GetKeyName(static_cast<SDLKey>(keyIndex));
}

int RealInstanceDelegator::getNumKeys()
{
	return ::SDLK_LAST;
}

int RealInstanceDelegator::ShowCursor(int toggle)
{
	return ::SDL_ShowCursor(toggle);
}

int RealInstanceDelegator::TTF_Init()
{
	return ::TTF_Init();
}

void RealInstanceDelegator::TTF_Quit()
{
	::TTF_Quit();
}

TTF_Font * RealInstanceDelegator::TTF_OpenFont(std::string fontPath, int fontSize)
{
	return ::TTF_OpenFont(fontPath.c_str(), fontSize);
}

void RealInstanceDelegator::TTF_CloseFont(TTF_Font * font) 
{
	::TTF_CloseFont(font);
}

SDL_Surface *  RealInstanceDelegator::TTF_RenderText_Solid(TTF_Font * font, std::string text, SDL_Color color) 
{
	/* TO DEBUG WITH
	SDL_Surface * surface = ::TTF_RenderText_Solid(font, text.c_str(), color);

	Uint32 colorValue = ::SDL_MapRGB(surface->format, color.r, color.g, color.b);
	::SDL_FillRect(surface, NULL, colorValue);

	return surface; 
	*/

	return ::TTF_RenderText_Solid(font, text.c_str(), color);
}

int RealInstanceDelegator::TTF_SizeText(TTF_Font *font, std::string text, int *w, int *h)
{
	return ::TTF_SizeText(font, text.c_str(), w, h);
}

int RealInstanceDelegator::OpenAudio(int frequency, Uint16 format, int numChannels, int chunkSize)
{
	return ::Mix_OpenAudio(frequency, format, numChannels, chunkSize);
}

int RealInstanceDelegator::defaultAudioFrequency()
{
	return MIX_DEFAULT_FREQUENCY;
}

int RealInstanceDelegator::defaultAudioFormat()
{
	return MIX_DEFAULT_FORMAT;
}

int RealInstanceDelegator::defaultAudioChannels()
{
	return MIX_DEFAULT_CHANNELS;
}

int RealInstanceDelegator::defaultChunkSize()
{
	return 4096;
}

void RealInstanceDelegator::CloseAudio()
{
	::Mix_CloseAudio();
}

Mix_Chunk * RealInstanceDelegator::loadAudioFile(std::string pathToFile)
{
	return ::Mix_LoadWAV(pathToFile.c_str());
}

int RealInstanceDelegator::setSoundVolume(Mix_Chunk * chunk, int volume)
{
	return ::Mix_VolumeChunk(chunk, volume);
}

void RealInstanceDelegator::freeSound(Mix_Chunk * chunk)
{
	::Mix_FreeChunk(chunk);
}

int RealInstanceDelegator::playChannel(int channelID, Mix_Chunk * chunk, int numLoops)
{
	return ::Mix_PlayChannel(channelID, chunk, numLoops);
}

int RealInstanceDelegator::setChannelSoundVolume(int channel, int volume)
{
	return ::Mix_Volume(channel, volume);
}

Mix_Music * RealInstanceDelegator::loadMusicFile(std::string pathToFile)
{
	return ::Mix_LoadMUS(pathToFile.c_str());
}

void RealInstanceDelegator::freeMusic(Mix_Music * music) 
{
	::Mix_FreeMusic(music);
}

int RealInstanceDelegator::playMusic(Mix_Music * music, int numLoops, int timeToFadeIn) 
{
	return ::Mix_FadeInMusic(music, numLoops, timeToFadeIn);
}

int RealInstanceDelegator::haltMusic()
{
	return ::Mix_HaltMusic();
}

int RealInstanceDelegator::fadeOutMusic(int timeToFadeOut)
{
	return ::Mix_FadeOutMusic(timeToFadeOut);
}

int RealInstanceDelegator::volumeMusic(int volume)
{
	return ::Mix_VolumeMusic(volume);
}

int RealInstanceDelegator::pausedMusic()
{
	return ::Mix_PausedMusic();
}


void RealInstanceDelegator::pauseMusic()
{
	::Mix_PauseMusic();
}

void RealInstanceDelegator::resumeMusic()
{
	::Mix_ResumeMusic();
}

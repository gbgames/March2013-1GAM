#ifndef GB_ISDLINSTANCEDELEGATOR_H
#define GB_ISDLINSTANCEDELEGATOR_H

#include "SDL_stdinc.h"
#include "SDL_ttf.h"

#include <string>

struct Mix_Chunk;
struct _Mix_Music;
typedef _Mix_Music Mix_Music;
struct SDL_PixelFormat;
struct SDL_Rect;
struct SDL_Surface;
struct SDL_version;
union SDL_Event;

class ISDLInstanceDelegator
{
	public:
		ISDLInstanceDelegator() {}
		virtual ~ISDLInstanceDelegator() {}

	public:
		virtual const SDL_version * SDL_Linked_Version() = 0;
		virtual int SDL_Init(int bitMask) = 0;
		virtual SDL_Surface * SDL_SetVideoMode(int width, int height, int bitDepth, int mode) = 0;
		virtual void SDL_WM_SetCaption(const char * title, const char * icon) = 0;
		virtual void SDL_Quit() = 0;
		virtual int SDL_FillRect(SDL_Surface * destination, SDL_Rect * destinationSurface, Uint32 color) = 0; 
		virtual Uint32 SDL_MapRGB(SDL_PixelFormat * format, Uint8 red, Uint8 green, Uint8 blue) = 0;
		virtual int SDL_Flip(SDL_Surface * screen) = 0;
		virtual void SDL_UpdateRect(SDL_Surface * screen, Sint32 x, Sint32 y, Sint32 w, Sint32 h) = 0;
		virtual SDL_Surface * CreateRGBSurface(int flags, int width, int height, int depth, Uint32 RMask, Uint32 GMask, Uint32 BMask, Uint32 AMask) = 0;
		virtual void FreeSurface(SDL_Surface * surface) = 0; 
		virtual SDL_Surface * IMG_Load(const char * filename) = 0;
		virtual const char * GetError() = 0;
		virtual int BlitSurface(SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect) = 0;
		virtual int PollEvent(SDL_Event * event) = 0;
		virtual unsigned int GetTicks() = 0;
		virtual void Delay(int milliseconds) = 0;
		virtual int LockSurface(SDL_Surface *) = 0;
		virtual void UnlockSurface(SDL_Surface *) = 0;
		virtual std::string SDL_GetKeyName(int) = 0;
		virtual int getNumKeys() = 0;
		virtual int ShowCursor(int) = 0;

		virtual int TTF_Init() = 0;
		virtual void TTF_Quit() = 0;
		virtual TTF_Font * TTF_OpenFont(std::string, int) = 0;
		virtual void TTF_CloseFont(TTF_Font *) = 0;
		virtual SDL_Surface * TTF_RenderText_Solid(TTF_Font *, std::string, SDL_Color) = 0;
		virtual int TTF_SizeText(TTF_Font *font, std::string, int *w, int *h) = 0; 

		virtual int OpenAudio(int frequency, Uint16 format, int numChannels, int chunkSize) = 0;
		virtual int defaultAudioFrequency() = 0; 
		virtual int defaultAudioFormat() = 0; 
		virtual int defaultAudioChannels() = 0;
		virtual int defaultChunkSize() = 0;
		virtual void CloseAudio() = 0;
		
		virtual Mix_Chunk * loadAudioFile(std::string) = 0;
		virtual void freeSound(Mix_Chunk *) = 0;
		virtual int setSoundVolume(Mix_Chunk *, int volume) = 0;
		virtual int playChannel(int, Mix_Chunk *, int) = 0;
		virtual int setChannelSoundVolume(int channel, int volume) = 0;
		virtual Mix_Music * loadMusicFile(std::string) = 0;
		virtual void freeMusic(Mix_Music *) = 0;
		virtual int playMusic(Mix_Music *, int, int) = 0;
		virtual int haltMusic() = 0;
		virtual int fadeOutMusic(int timeToFadeOut) = 0;
		virtual int volumeMusic(int) = 0;
		virtual int pausedMusic() = 0;
		virtual void pauseMusic() = 0;
		virtual void resumeMusic() = 0;
};

#endif

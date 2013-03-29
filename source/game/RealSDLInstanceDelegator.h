#ifndef GB_REALSDLINSTANCEDELEGATOR_H
#define GB_REALSDLINSTANCEDELEGATOR_H

#include "ISDLInstanceDelegator.h"

struct SDL_Surface;
struct SDL_version;

class RealInstanceDelegator : public ISDLInstanceDelegator
{
	public:
		RealInstanceDelegator();
		virtual ~RealInstanceDelegator();

	public:
		virtual const SDL_version * SDL_Linked_Version();
		virtual int SDL_Init(int bitMask);
		virtual SDL_Surface * SDL_SetVideoMode(int width, int height, int bitDepth, int mode);
		virtual void SDL_WM_SetCaption(const char * title, const char * icon);
		virtual void SDL_Quit();
		virtual int SDL_FillRect(SDL_Surface * destination, SDL_Rect * destinationSurface, Uint32 color); 
		virtual Uint32 SDL_MapRGB(SDL_PixelFormat * format, Uint8 red, Uint8 green, Uint8 blue);
		virtual int SDL_Flip(SDL_Surface * screen);
		virtual void SDL_UpdateRect(SDL_Surface * screen, Sint32 x, Sint32 y, Sint32 w, Sint32 h);
		virtual SDL_Surface * CreateRGBSurface(int flags, int width, int height, int depth, Uint32 RMask, Uint32 GMask, Uint32 BMask, Uint32 AMask);
		virtual void FreeSurface(SDL_Surface * surface); 
		virtual SDL_Surface * IMG_Load(const char * filename);
		virtual const char * GetError();
		virtual int BlitSurface(SDL_Surface * src, SDL_Rect * srcrect, SDL_Surface * dst, SDL_Rect * dstrect);
		virtual int PollEvent(SDL_Event * event);
		virtual unsigned int GetTicks();
		virtual void Delay(int milliseconds);
		virtual int LockSurface(SDL_Surface * surface);
		virtual void UnlockSurface(SDL_Surface * surface);
		virtual std::string SDL_GetKeyName(int keyIndex);
		virtual int getNumKeys();
		virtual int ShowCursor(int toggle); 

		virtual int TTF_Init(); 
		virtual void TTF_Quit();
		virtual TTF_Font * TTF_OpenFont(std::string, int);
		virtual void TTF_CloseFont(TTF_Font *);
		virtual SDL_Surface * TTF_RenderText_Solid(TTF_Font * font, std::string text, SDL_Color color); 
		virtual int TTF_SizeText(TTF_Font *font, std::string, int *w, int *h); 
	
	public:
		virtual int OpenAudio(int frequency, Uint16 format, int numChannels, int chunkSize); 
		virtual int defaultAudioFrequency();
		virtual int defaultAudioFormat(); 
		virtual int defaultAudioChannels(); 
		virtual int defaultChunkSize(); 
		virtual void CloseAudio(); 

		virtual Mix_Chunk * loadAudioFile(std::string pathToFile);
		virtual void freeSound(Mix_Chunk * chunk);
		virtual int setSoundVolume(Mix_Chunk * chunk, int volume);
		virtual int playChannel(int channelID, Mix_Chunk * chunk, int numLoops); 
		virtual int setChannelSoundVolume(int channel, int volume);
		virtual Mix_Music * loadMusicFile(std::string); 
		virtual void freeMusic(Mix_Music * music);
		virtual int playMusic(Mix_Music * music, int numLoops, int timeToFadeIn); 
		virtual int haltMusic();
		virtual int fadeOutMusic(int timeToFadeOut);
		virtual int volumeMusic(int volume);
		virtual int pausedMusic();
		virtual void pauseMusic();
		virtual void resumeMusic();

};

#endif

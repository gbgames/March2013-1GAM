#ifndef GB_SDLLAYERFACTORY_H
#define GB_SDLLAYERFACTORY_H

class ISDLInstanceDelegator;

class SDLLayerFactory
{
	public:
		static ISDLInstanceDelegator * getInstance();
		static ISDLInstanceDelegator * replaceInstance(ISDLInstanceDelegator * replacement);

	private:
		static ISDLInstanceDelegator * instance;

		SDLLayerFactory();
		~SDLLayerFactory();
};

#endif

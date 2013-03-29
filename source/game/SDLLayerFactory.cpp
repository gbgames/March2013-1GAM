#include "SDLLayerFactory.h"

#include <iostream>

ISDLInstanceDelegator * SDLLayerFactory::instance = 0;

SDLLayerFactory::SDLLayerFactory() 
{
}

SDLLayerFactory::~SDLLayerFactory() 
{
}

ISDLInstanceDelegator * SDLLayerFactory::getInstance()
{
	return instance;
}

ISDLInstanceDelegator * SDLLayerFactory::replaceInstance(ISDLInstanceDelegator * replacement)
{
	ISDLInstanceDelegator * original = instance;
	instance = replacement;
	return original;
}

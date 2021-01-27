#include "sdl_adapter.h"
#include <stdexcept>


void SDLAdapter::renderTexture(TextureAbstract* tx_, int x, int y)
{
	auto txAd = dynamic_cast<SDLTextureAdapter*>(tx_);
	if(txAd == nullptr)
		throw std::runtime_error("Bad texture downcast");
	sdl.renderTexture(txAd->tx, x, y);
}

TextureAbstract* SDLAdapter::makeTextureFromText(
	const std::string& text,
	GraphicalEngineAbstract::Color& color)
{
	SDL_Color c = {color.r, color.g, color.b, color.a};
	txAdapters.push_back(SDLTextureAdapter(this, sdl.makeTextureFromText(text, c)));
	return &txAdapters.back();
}

int SDLAdapter::processEvents()
{
	for(SDL_Event event; SDL_PollEvent(&event);)
	{
		switch(event.type)
		{
		case SDL_QUIT:
			return 1;
			break;
		default:
			;
		}
	}
	return 0;
}

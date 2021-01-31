#include "sdl_adapter.h"
#include <stdexcept>


void SDLAdapter::renderTexture(TextureAbstract* tx_, int x, int y)
{
	SDLTextureAdapter* tx_ad = dynamic_cast<SDLTextureAdapter*>(tx_);
	if(tx_ad == nullptr)
		throw std::runtime_error("Bad texture downcast");
	sdl.renderTexture(tx_ad->tx.get(), x, y);
}

std::unique_ptr<TextureAbstract> SDLAdapter::makeTextureFromText(
	const std::string& text,
	GraphicalEngineAbstract::Color& color)
{
	SDL_Color c = {color.r, color.g, color.b, color.a};
	return std::make_unique<SDLTextureAdapter>(this, sdl.makeTextureFromText(text, c));
}

std::unique_ptr<TextureAbstract> SDLAdapter::makeSquareTexture(
	int w,
	int h,
	GraphicalEngineAbstract::Color& color)
{
	SDL_Color c = {color.r, color.g, color.b, color.a};
	return std::make_unique<SDLTextureAdapter>(this, sdl.makeSquareTexture(w, h, c));
}

std::unique_ptr<TextureAbstract> SDLAdapter::combineTextures(
	const TextureAbstract* tx1,
	const TextureAbstract* tx2)
{
	const SDLTextureAdapter* tx_ad1 = dynamic_cast<const SDLTextureAdapter*>(tx1);
	const SDLTextureAdapter* tx_ad2 = dynamic_cast<const SDLTextureAdapter*>(tx2);
	if(!tx_ad1 || !tx_ad2)
		throw std::runtime_error("Bad texture downcast");
	return std::make_unique<SDLTextureAdapter>(this, sdl.combineTextures(tx_ad1->tx.get(), tx_ad2->tx.get()));
}

int SDLAdapter::processEvents() noexcept
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

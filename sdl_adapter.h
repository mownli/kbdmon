#ifndef SDL_ADAPTER_H
#define SDL_ADAPTER_H

#include "sdl/sdl.h"
#include "texture_abstract.h"
#include "graphical_engine_abstract.h"
#include <deque>
#include <cassert>


class SDLAdapter : public GraphicalEngineAbstract
{
	class SDLTextureAdapter : public TextureAbstract
	{
		SDLAdapter* parent;
	public:
		SDLTextureAdapter(SDLAdapter* ad, SDL::Texture* tx_) : parent(ad), tx(tx_) { assert(ad); };
		void renderTexture(int x, int y) override { parent->sdl.renderTexture(tx, x, y); };
		SDL::Texture* tx;
	};
public:
	SDLAdapter(const std::string& title, int w, int h)
		: sdl(SDL(title, w, h)) { sdl.setKeyboardIgnored(); }

	int setupFont(const std::string& path, int size) override { return sdl.setupFont(path, size); }
	void clear() override { sdl.clear(); }
	void update() override { sdl.update(); }

	int processEvents() override;
	void renderTexture(TextureAbstract* tx_, int x, int y) override;
	TextureAbstract* makeTextureFromText(const std::string& text, Color& color) override;

	// Deque for immutable storage (of addresses)
	std::deque<SDLTextureAdapter> txAdapters;
	SDL sdl;
};

#endif // SDL_ADAPTER_H

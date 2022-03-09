#ifndef SDL_ADAPTER_H
#define SDL_ADAPTER_H

#include "sdl/sdl.h"
#include "texture_abstract.h"
#include "graphical_engine_abstract.h"
#include <deque>
#include <cassert>


class SDLAdapter : public GraphicalEngineAbstract
{
private:
	class SDLTextureAdapter : public TextureAbstract
	{
	private:
		SDLAdapter* parent;
	public:
		SDLTextureAdapter(SDLAdapter* ad, std::unique_ptr<EngineSDL::Texture> tx_) :
			parent(ad),
			tx(std::move(tx_)) {}
		void renderTexture(int x, int y) override
		{
			parent->sdl.renderTexture(tx.get(), x, y);
		}
		std::unique_ptr<EngineSDL::Texture> tx;
	};

	EngineSDL sdl;
public:
	SDLAdapter(const std::string& title, int w, int h)
		: sdl(EngineSDL(title, w, h)) { sdl.setKeyboardIgnored(); }

	int setupFont(const std::string& path, int size) override
	{
		return sdl.setupFont(path, size);
	}
	void clear() noexcept override { sdl.clear(); }
	void update() noexcept override { sdl.update(); }

	int processEvents() noexcept override;
	void renderTexture(TextureAbstract* tx_, int x, int y) override;
	std::unique_ptr<TextureAbstract> makeTextureFromText(const std::string& text, Color& color) override;
	std::unique_ptr<TextureAbstract> makeSquareTexture(int w, int h, Color& color) override;
	std::unique_ptr<TextureAbstract> combineTextures(
		const TextureAbstract* tx1,
		const TextureAbstract* tx2) override;
};

#endif // SDL_ADAPTER_H

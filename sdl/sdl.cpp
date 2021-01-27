#include "sdl.h"

#include "dbg.h"
#include <cassert>
#include <string>
#include <stdexcept>

static inline void throwSDLError() { throw std::runtime_error(SDL_GetError()); }

SDL::Engine::Engine()
{
	DEBUG("Creating Engine object");
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0 || TTF_Init() != 0)
		throwSDLError();
}

SDL::Engine::~Engine()
{
	DEBUG("Destroying Engine object");
	TTF_Quit();
	SDL_Quit();
}

SDL::Window::Window(const std::string& title, int w, int h)
{
	DEBUG("Creating Window object");
	ptr = SDL_CreateWindow(
				title.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				w,
				h,
				SDL_WINDOW_SHOWN);
	if(ptr == nullptr)
		throwSDLError();
}

SDL::Window::~Window()
{
	DEBUG("Destroying Window object");
	if(ptr != nullptr) SDL_DestroyWindow(ptr);
}

SDL::Renderer::Renderer(SDL_Window* window)
{
	DEBUG("Creating Renderer object");
	assert(window);

	ptr = SDL_CreateRenderer(
				window,
				-1,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ptr == nullptr)
		throwSDLError();
}

SDL::Renderer::~Renderer()
{
	DEBUG("Destroying Renderer object");
	// Texture should be deleted before freeing renderer
	textures.clear();
	if(ptr != nullptr) SDL_DestroyRenderer(ptr);
}

void SDL::Renderer::addTexture(std::unique_ptr<SDL::Texture> tx)
{
	textures.push_back(std::move(tx));
}

SDL::Font::Font(const std::string& path, int size)
{
	DEBUG("Creating Font object");
	ptr = TTF_OpenFont(path.c_str(), size);
	if(ptr == nullptr) throwSDLError();
}

SDL::Font::~Font()
{
	DEBUG("Destroying Font object");
	if(ptr != nullptr) TTF_CloseFont(ptr);
}

SDL::SDL(const std::string& title, int w, int h) : win(title, w, h), rend(win.getPtr())
{
	DEBUG("Creating SDL object");
//	win = std::make_unique<Window>(title, w, h);
//	if(!win.get())
//		throwSDLError();
}

int SDL::setupFont(const std::string& path, int size)
{
	font = std::make_unique<Font>(path, size);
	DEBUG("Font set up");
	return font == nullptr;
}

SDL::Texture* SDL::makeTextureFromText(const std::string& text, SDL_Color& color)
{
	DEBUG("Creating Texture object");
	if(font == nullptr) throw std::runtime_error("Font is not initialized");

//	SDL_Surface *surf = TTF_RenderText_Blended(font->getPtr(), text.c_str(), color);
//	if(surf == nullptr) throw std::runtime_error(TTF_GetError());
	auto surf = makeSurfaceFromText(font.get(), text, color);

	SDL_Texture *texture_ll = SDL_CreateTextureFromSurface(rend.getPtr(), surf->getPtr());
	if(texture_ll == nullptr)
		throwSDLError();

	auto tx = std::make_unique<Texture>(texture_ll);
	auto ret = tx.get();
	rend.addTexture(std::move(tx));

	return ret;
}

void SDL::renderTexture(SDL::Texture* texture, int x, int y)
{
	if(texture == nullptr) return;

	// Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	// Query the texture to get its width and height to use
	SDL_QueryTexture(texture->getPtr(), NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(rend.getPtr(), texture->getPtr(), NULL, &dst);
}

void SDL::setKeyboardIgnored()
{
	SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
	SDL_EventState(SDL_KEYUP, SDL_IGNORE);
}

SDL::Texture::~Texture()
{
	DEBUG("Destroying Texture object");
	if(ptr != nullptr) SDL_DestroyTexture(ptr);
}

SDL::Surface::~Surface()
{
	DEBUG("Destroying Surface object");
	if(ptr != nullptr) SDL_FreeSurface(ptr);
}

std::unique_ptr<SDL::Surface> SDL::makeSurfaceFromText(
	SDL::Font* font,
	const std::string& text,
	SDL_Color& color)
{
	DEBUG("Creating Surface object");
	SDL_Surface *surf = TTF_RenderText_Blended(font->getPtr(), text.c_str(), color);
	if(surf == nullptr) throw std::runtime_error(TTF_GetError());

	return std::make_unique<SDL::Surface>(surf);
}

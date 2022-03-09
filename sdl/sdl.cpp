#include "sdl.h"
#include "dbg.h"

#include <string>


EngineSDL::Engine::Engine()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0 || TTF_Init() != 0)
		throwSDLError();
}

EngineSDL::Engine::~Engine() noexcept
{
	TTF_Quit();
	SDL_Quit();
}

EngineSDL::Window::Window(const std::string& title, int w, int h)
{
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

EngineSDL::Window::~Window() noexcept
{
	if(ptr != nullptr) SDL_DestroyWindow(ptr);
}

EngineSDL::Renderer::Renderer(SDL_Window* window)
{
	ptr = SDL_CreateRenderer(
				window,
				-1,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if(ptr == nullptr)
		throwSDLError();
}

EngineSDL::Renderer::~Renderer() noexcept
{
	if(ptr != nullptr) SDL_DestroyRenderer(ptr);
}

EngineSDL::Font::Font(const std::string& path, int size)
{
	DEBUG("Creating Font object");
	ptr = TTF_OpenFont(path.c_str(), size);
	if(ptr == nullptr)
		throwSDLError();
	DEBUG("Font address: %p", ptr);
}

EngineSDL::Font::~Font() noexcept
{
	DEBUG("Destroying Font object %p", ptr);
	if(ptr != nullptr) TTF_CloseFont(ptr);
}

int EngineSDL::setupFont(const std::string& path, int size)
{
	DEBUG("Setting up font");
	font = std::make_unique<Font>(path, size);
	return font == nullptr;
}

std::unique_ptr<EngineSDL::Texture> EngineSDL::makeTextureFromText(const std::string& text, SDL_Color& color)
{
	DEBUG("Creating Texture object");
	if(font == nullptr) throw std::runtime_error("Font is not initialized");

	std::unique_ptr<EngineSDL::Surface> surf = makeSurfaceFromText(font.get(), text, color);

	SDL_Texture *texture_ll = SDL_CreateTextureFromSurface(renderer.getPtr(), surf->getPtr());
	if(texture_ll == nullptr)
		throwSDLError();

	DEBUG("Texture address: %p", texture_ll);
	return std::make_unique<Texture>(texture_ll);
}

std::unique_ptr<EngineSDL::Texture> EngineSDL::combineTextures(const EngineSDL::Texture* tx1, const EngineSDL::Texture* tx2)
{
	DEBUG("Combining textures %p and %p", tx1->getPtr(), tx2->getPtr());
	int w1, h1;
	Uint32 fmt;
	SDL_QueryTexture(tx1->getPtr(), &fmt, nullptr, &w1, &h1);

	SDL_Texture* target_tx = SDL_CreateTexture(
		renderer.getPtr(),
		fmt,
		SDL_TEXTUREACCESS_TARGET,
		w1, h1
	);
	if(target_tx == nullptr)
		throwSDLError();

	SDL_SetRenderTarget(renderer.getPtr(), target_tx);
	SDL_RenderClear(renderer.getPtr());
	SDL_RenderCopy(renderer.getPtr(), tx1->getPtr(), nullptr, nullptr);

	int w2, h2;
	SDL_QueryTexture(tx2->getPtr(), nullptr, nullptr, &w2, &h2);
	SDL_Rect dest_rect{(w1-w2)/2, (h1-h2)/2, w2, h2};

	SDL_RenderCopy(renderer.getPtr(), tx2->getPtr(), nullptr, &dest_rect);
	// Reset renderer
	SDL_SetRenderTarget(renderer.getPtr(), nullptr);

	DEBUG("Combination result: %p", target_tx);
	return std::make_unique<EngineSDL::Texture>(target_tx);
}

std::unique_ptr<EngineSDL::Texture> EngineSDL::makeSquareTexture(int w, int h, SDL_Color& color)
{
	DEBUG("Creating square Texture object");
	std::unique_ptr<EngineSDL::Surface> surf = makeSquareSurface(w, h, color);

	SDL_Texture *texture_ll = SDL_CreateTextureFromSurface(renderer.getPtr(), surf->getPtr());
	if(texture_ll == nullptr)
		throwSDLError();

	DEBUG("Texture address: %p", texture_ll);
	return std::make_unique<Texture>(texture_ll);
}

void EngineSDL::renderTexture(EngineSDL::Texture* texture, int x, int y) noexcept
{
	// DEBUG("Rendering texture %p", texture ? texture->getPtr() : nullptr);
	if(texture == nullptr)
		return;

	// Setup the destination rectangle to be at the position we want
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	// Query the texture to get its width and height to use
	SDL_QueryTexture(texture->getPtr(), NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer.getPtr(), texture->getPtr(), NULL, &dst);
}

void EngineSDL::setKeyboardIgnored() noexcept
{
	SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
	SDL_EventState(SDL_KEYUP, SDL_IGNORE);
}

EngineSDL::Texture::~Texture() noexcept
{
	DEBUG("Destroying Texture object %p", ptr);
	if(ptr != nullptr) SDL_DestroyTexture(ptr);
}

EngineSDL::Surface::~Surface() noexcept
{
	DEBUG("Destroying Surface object %p", ptr);
	if(ptr != nullptr) SDL_FreeSurface(ptr);
}

std::unique_ptr<EngineSDL::Surface> EngineSDL::makeSurfaceFromText(
	EngineSDL::Font* font,
	const std::string& text,
	SDL_Color& color)
{
	DEBUG("Creating Surface object from text");
	SDL_Surface *surf = TTF_RenderText_Blended(font->getPtr(), text.c_str(), color);
	if(surf == nullptr) throw std::runtime_error(TTF_GetError());

	DEBUG("Surface address: %p", surf);
	return std::make_unique<EngineSDL::Surface>(surf);
}

std::unique_ptr<EngineSDL::Surface> EngineSDL::makeSquareSurface(int w, int h, SDL_Color& color)
{
	DEBUG("Creating square Surface object");
	SDL_Surface* surf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	if(surf == nullptr)
		throwSDLError();
	SDL_FillRect(surf, nullptr, SDL_MapRGB(surf->format, color.r, color.g, color.b));

	DEBUG("Surface address: %p", surf);
	return std::make_unique<EngineSDL::Surface>(surf);
}

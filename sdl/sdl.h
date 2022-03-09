#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <memory>
#include <vector>
#include <stdexcept>


class EngineSDL
{
public:
	EngineSDL(const std::string& title, int w, int h) : win(title, w, h), renderer(win.getPtr()) {}

	class Font
	{
		TTF_Font* ptr = nullptr;
	public:
		Font(const std::string& path, int size);
		~Font() noexcept;

		TTF_Font* getPtr() const noexcept { return ptr; }
	};

	class Texture
	{
		SDL_Texture* ptr = nullptr;
	public:
		Texture(Texture* tx) noexcept : ptr(tx->getPtr())  {};
		Texture(SDL_Texture* tx_ll) noexcept : ptr(tx_ll) {};
		//Texture(const Texture& obj);
		~Texture() noexcept;

		SDL_Texture* getPtr() const noexcept { return ptr; }
	};

	class Surface
	{
		SDL_Surface* ptr = nullptr;
	public:
		Surface(SDL_Surface* surface) noexcept : ptr(surface) {};
		~Surface() noexcept;

		SDL_Surface* getPtr() const noexcept { return ptr; }
	};

	int setupFont(const std::string& path, int size);
	std::unique_ptr<Texture> makeTextureFromText(const std::string& text, SDL_Color& color);
	std::unique_ptr<Texture> combineTextures(const Texture* tx1, const Texture* tx2);
	std::unique_ptr<Texture> makeSquareTexture(int w, int h, SDL_Color& color);
	void clear() noexcept { renderer.clear(); }
	void update() noexcept { renderer.setPresent(); }
	void renderTexture(Texture* texture, int x, int y) noexcept;
	void setKeyboardIgnored() noexcept;
private:
	class Engine
	{
	public:
		Engine();
		~Engine() noexcept;
	} engine; // Needs to be at the top of the class stack

	class Window
	{
		SDL_Window *ptr = nullptr;
	public:
		Window(const std::string& title, int w, int h);
		~Window() noexcept;

		SDL_Window* getPtr() const noexcept { return ptr; }
	};

	class Renderer
	{
		SDL_Renderer *ptr = nullptr;
	public:
		Renderer(SDL_Window* window);
		~Renderer() noexcept;

		void clear() noexcept { SDL_RenderClear(ptr); }
		void setPresent() noexcept { SDL_RenderPresent(ptr); }
		SDL_Renderer* getPtr() const noexcept { return ptr; }
	};

	Window win;
	Renderer renderer;
	std::unique_ptr<Font> font;

	std::unique_ptr<Surface> makeSurfaceFromText(
		Font* font,
		const std::string& text,
		SDL_Color& color);
	std::unique_ptr<Surface> makeSquareSurface(int w, int h, SDL_Color& color);
	static void throwSDLError() { throw std::runtime_error(SDL_GetError()); }
};

#endif // SDL_H

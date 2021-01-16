#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include <vector>


class SDL
{
public:
	SDL(const std::string& title, int w, int h);

	class Font
	{
		TTF_Font* ptr = nullptr;
	public:
		Font() {};
		Font(const std::string& path, int size);
		~Font();

		TTF_Font* getPtr() { return ptr; }
	};

	class Texture
	{
		SDL_Texture* ptr = nullptr;
	public:
		Texture(Texture* tx) : ptr(tx->getPtr()) {};
		Texture(SDL_Texture* tx_ll) : ptr(tx_ll) {};
		//Texture(const Texture& obj);
		~Texture();

		SDL_Texture* getPtr() { return ptr; }
	};

	class Surface
	{
		SDL_Surface* ptr = nullptr;
	public:
		Surface(SDL_Surface* surface) : ptr(surface) {};
		~Surface();

		SDL_Surface* getPtr() { return ptr; }
	};

	int setupFont(const std::string& path, int size);
	Texture* makeTextureFromText(const std::string& text, SDL_Color& color);
	void clear() { rend.clear(); }
	void update() { rend.setPresent(); }
	void renderTexture(Texture* texture, int x, int y);
	void setKeyboardIgnored();
	std::unique_ptr<Surface> makeSurfaceFromText(
		Font* font,
		const std::string& text,
		SDL_Color& color);
private:
	class Engine
	{
	public:
		Engine();
		~Engine();
	} engine; // Needs to be at the top of the class stack

	class Window
	{
		SDL_Window *ptr = nullptr;
	public:
		Window(const std::string& title, int w, int h);
		~Window();

		SDL_Window* getPtr() { return ptr; }
	};

	class Renderer
	{
		SDL_Renderer *ptr = nullptr;
		std::vector<std::unique_ptr<SDL::Texture>> textures;
	public:

		Renderer(SDL_Window* window);
		~Renderer();

		void clear() { SDL_RenderClear(ptr); }
		void setPresent() { SDL_RenderPresent(ptr); }
		SDL_Renderer* getPtr() { return ptr; }
		void addTexture(std::unique_ptr<Texture> tx);
	};

	// Renderer should be declared after Window (for proper initialization order)
	Window win;
	Renderer rend;
	std::unique_ptr<Font> font;
};

#endif // SDL_H

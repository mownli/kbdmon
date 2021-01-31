#include "app.h"
#include "sdl_adapter.h"
#include "dbg.h"

#include <cassert>
#include <iostream>


App::App(Conf& conf) :
	engine(std::make_unique<SDLAdapter>(
		conf.title, conf.win_width, conf.win_height)),
	input(std::make_unique<LibinputWrapper>(conf.kbd_path))
{
	std::vector<std::vector<std::string>>& layout = conf.layout;

	for(const std::pair<const std::string, int>& pair : conf.scancodes)
	{
		scancodes_set.insert(pair.second);
	}
	printScancodes();

	GraphicalEngineAbstract::Color color1 = {.r = 50, .g = 50, .b = 50};
	GraphicalEngineAbstract::Color color2 = {.r = 200, .g = 200, .b = 200};
	GraphicalEngineAbstract::Color color_black = {.r = 0, .g = 0, .b = 0};
	GraphicalEngineAbstract::Color color_orange = {.r = 150, .g = 100, .b = 50};

	int max_row_size = findLayoutRowSizeMax(layout);
	int sector_width = conf.win_width / max_row_size;
	int sector_height = conf.win_height / layout.size();

	engine->setupFont(conf.font_path, sector_height*0.5);

	auto bg_black = engine->makeSquareTexture(sector_width, sector_height, color_black);
	auto bg_orange = engine->makeSquareTexture(sector_width, sector_height, color_orange);

	auto get_txs = [&](const std::string& label) -> std::tuple<std::unique_ptr<TextureAbstract>, std::unique_ptr<TextureAbstract>>
	{
		return {
			engine->combineTextures(bg_black.get(), engine->makeTextureFromText(label, color1).get()),
			engine->combineTextures(bg_orange.get(), engine->makeTextureFromText(label, color2).get())
		};
	};

	for(decltype(layout.size()) row = 0; row < layout.size(); row++)
	{
		for(decltype(layout.front().size()) col = 0; col < layout[row].size(); col++)
		{
			const std::string& label = layout[row][col];
			if(label == "")
				continue;
			int code = conf.scancodes.at(label);
			buttons.emplace_back(get_txs(label), sector_width * col, sector_height * row);
			button_map.insert({code, buttons.back()});
		}
	}
}

void App::renderButtons() noexcept
{
	for(Button& b : buttons)
	{
		if(b.visible)
			b.getCurrentTx()->renderTexture(b.getX(), b.getY());
	}
}

void App::processEvent(const LibinputWrapper::Event& ev) noexcept
{
	if((ev.scancode == 0) || (scancodes_set.find(ev.scancode) == scancodes_set.cend()))
	{
		DEBUG("Ignored scancode %d", ev.scancode);
		return;
	}

	assert(!button_map.empty());
	Button& b = button_map.at(ev.scancode);
	switch(ev.type)
	{
	case LibinputWrapper::EventType::KEY_UP:
		b.setStatusUp(true);
		break;
	case LibinputWrapper::EventType::KEY_DOWN:
		b.setStatusUp(false);
		break;
	case LibinputWrapper::EventType::NONE:
	default:
		break;
	}
}

int App::findLayoutRowSizeMax(const std::vector<std::vector<std::string>>& layout) const noexcept
{
	std::vector<std::string>::size_type s = 0;
	for(const std::vector<std::string>& i : layout)
	{
		if(i.size() > s)
			s = i.size();
	}
	assert(s != 0);
	return s;
}

void App::printScancodes() const noexcept
{
	std::cout << "Requested scancodes: {";
	for(const int& code : scancodes_set)
		std::cout << code << ",";
	std::cout << "}" << std::endl;
}

App::Button::Button(App::Button&& o) noexcept :
	up_tx(std::move(o.up_tx)), down_tx(std::move(o.down_tx)),
	x(o.x), y(o.y), visible(o.visible)
{
	if(o.current_tx == &o.up_tx)
		current_tx = &up_tx;
	else
		current_tx = &down_tx;
	o.current_tx = nullptr;
}

void App::Button::setStatusUp(bool keyUp) noexcept
{
	if(keyUp)
		current_tx = &up_tx;
	else
		current_tx = &down_tx;
}

int App::exec()
{
	for(;;)
	{
		engine->clear();
		if(engine->processEvents())
			return 0;

		for(int i = 0; i < 10; i++)
		{
			LibinputWrapper::Event* ev = input->pollForEvent();

			if(ev)
			{
				processEvent(*ev);
			}
		}

		renderButtons();
		engine->update(); // VSync introduces delay
		//SDL_Delay(1000);
		//return 0;
	}
}

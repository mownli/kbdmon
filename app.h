#ifndef APP_H
#define APP_H

#include <deque>
#include <vector>
#include <utility>
#include <map>
#include <unordered_map>
#include <set>
#include <tuple>

#include "libinput_wrapper/libinput_wrapper.h"
#include "texture_abstract.h"
#include "graphical_engine_abstract.h"


class App
{
	class Button
	{
		std::unique_ptr<TextureAbstract> up_tx;
		std::unique_ptr<TextureAbstract> down_tx;
		std::unique_ptr<TextureAbstract>* current_tx;
	public:
		Button(
				std::tuple<std::unique_ptr<TextureAbstract>, std::unique_ptr<TextureAbstract>> textures,
				int x_, int y_) noexcept :
			up_tx(std::move(std::get<0>(textures))),
			down_tx(std::move(std::get<1>(textures))),
			current_tx(&up_tx),
			x(x_), y(y_) {}
		Button(Button&& o) noexcept;

		int getX() const { return x; }
		int getY() const { return y; }
		TextureAbstract* getUpTx() const { return up_tx.get(); }
		TextureAbstract* getDownTx() const { return down_tx.get(); }
		TextureAbstract* getCurrentTx() const { return current_tx->get(); }
		void setStatusUp(bool keyUp) noexcept;

		int x;
		int y;
		bool visible = true;
	};

	std::unique_ptr<GraphicalEngineAbstract> engine;
	std::unique_ptr<LibinputWrapper> input;
	std::set<int> scancodes_set;
	std::deque<Button> buttons;
	std::unordered_map<int, Button&> button_map;

	void renderButtons() noexcept;
	void processEvent(const LibinputWrapper::Event& ev) noexcept;
	int findLayoutRowSizeMax(const std::vector<std::vector<std::string>>& codes_layout) const noexcept;
	void printScancodes() const noexcept;
public:
	struct Conf {
		int win_height;
		int win_width;
		std::string title;
		std::string kbd_path;
		std::string font_path;
		std::vector<std::vector<std::string>> layout;
		std::map<std::string, int> scancodes;
	};

	App(Conf& conf);

	int exec();
};

#endif // APP_H

#ifndef APP_H
#define APP_H

#include <memory>
#include <deque>
#include <vector>

#include "libinput_wrapper/libinput_wrapper.h"

#include "scancodemap.h"
#include "dbg.h"
#include "texture_abstract.h"
#include "graphical_engine_abstract.h"


class App
{
	const ScanCodeMap scanCodeMap;

	std::unique_ptr<GraphicalEngineAbstract> engine;
	LibinputWrapper input;

	std::set<int> scanCodesSet;

	class Button
	{
		bool up = true;
		TextureAbstract* upTx;
		TextureAbstract* downTx;
		TextureAbstract* currentTx;
	public:
		Button(
			TextureAbstract* upTx_, TextureAbstract* downTx_,
			int x_, int y_)
			: upTx(upTx_), downTx(downTx_), currentTx(upTx_), x(x_), y(y_) {};

		bool isUp() const { return up; }
		int getX() const { return x; }
		int getY() const { return y; }
		TextureAbstract* getUpTx() const { return upTx; }
		TextureAbstract* getDownTx() const { return downTx; }
		TextureAbstract* getCurrentTx() const { return currentTx; }
		void setStatusUp(bool keyUp);

		int x;
		int y;
	};
	std::unordered_map<int, std::unique_ptr<Button>> buttons;

	void renderButtons();
	void processEvent(const LibinputWrapper::Event& ev);

public:
	struct Conf {
		int h = 480;
		int w = 640;
		std::string title;
		std::string kbdPath;
		std::string fontPath;
		int fontSize;
		std::deque<std::vector<int>> codesLayout;
	};

	App(Conf& conf);

	int exec();
};

#endif // APP_H

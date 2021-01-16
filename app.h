#ifndef APP_H
#define APP_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

#include "libinput_wrapper/libinput_wrapper.h"
#include "scancodemap.h"
#include "dbg.h"
#include "texture_abstract.h"
#include "graphical_engine_abstract.h"


const std::string WINDOW_TITLE = "SDL shit";
const std::string PATH_TO_FONT = "/usr/share/fonts/droid/DroidSansMono.ttf";
const std::string PATH_TO_CONFIG = "config";
const int WIN_WIDTH = 640;
const int WIN_HEIGHT = 480;
const std::string PATH_TO_KBD = "/dev/input/by-id/usb-01f3_52c0-event-kbd";
const ScanCodeMap scanCodeMap;


class App
{
	std::unique_ptr<GraphicalEngineAbstract> engine;
	LibinputWrapper in;

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

	void renderButtons();
	void processEvent(const LibinputWrapper::Event& ev);
public:
	App();

	std::unordered_map<int, std::unique_ptr<Button>> buttons;
	std::set<int> scanCodesWanted;

	int exec();
};

#endif // APP_H

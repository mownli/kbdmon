#include "app.h"
#include "sdl_adapter.h"


App::App() :
	engine(std::make_unique<SDLAdapter>(WINDOW_TITLE, WIN_WIDTH, WIN_HEIGHT)),
	in(PATH_TO_KBD)
{
	//readConfig(PATH_TO_CONFIG);
	engine->setupFont(PATH_TO_FONT, 100);

	scanCodesWanted = {0x10, 0x11, 0x12};

	GraphicalEngineAbstract::Color color1 = {.r = 50, .g = 50, .b = 50};
	GraphicalEngineAbstract::Color color2 = {.r = 200, .g = 200, .b = 200};

	for(int scode : scanCodesWanted)
	{
		buttons[scode] = std::make_unique<Button>(
			engine->makeTextureFromText(scanCodeMap.at(scode), color1),
			engine->makeTextureFromText(scanCodeMap.at(scode), color2),
			0, 0);
	}

	auto i = scanCodesWanted.cbegin();
	buttons[*i]->x = 40;
	buttons[*i]->y = 40;

	i++;
	buttons[*i]->x = 80;
	buttons[*i]->y = 80;

	i++;
	buttons[*i]->x = 120;
	buttons[*i]->y = 120;
}

void App::renderButtons()
{
	for(auto scode : scanCodesWanted)
	{
//		auto b = buttons.at(scode).get();
//		engine->renderTexture(b->getCurrentTx(), b->getX(), b->getY());
		auto b = buttons.at(scode).get();
		b->getCurrentTx()->renderTexture(b->getX(), b->getY());
	}
}

void App::processEvent(const LibinputWrapper::Event& ev)
{
	if(scanCodesWanted.find(ev.scancode) == scanCodesWanted.cend())
	{
		DEBUG("Ignored scancode %d", ev.scancode);
		return;
	}

	auto b = buttons.at(ev.scancode).get();
	switch(ev.type)
	{
	case LibinputWrapper::EventType::KEY_UP:
		b->setStatusUp(true);
		break;
	case LibinputWrapper::EventType::KEY_DOWN:
		b->setStatusUp(false);
		break;
	case LibinputWrapper::EventType::NONE:
	default:
		break;
	}


//	try
//	{
//	processEventSwitch(ev);
//		b->setStatusUp(
//			ev.type == LibinputWrapper::EventType::KEY_UP ? true : false);
//	}
//	catch (...)
//	{
//		DEBUG("Ignored scancode %d", ev.scancode);
//	}
}


void App::Button::setStatusUp(bool keyUp)
{
	up = keyUp;
	if(keyUp)
		currentTx = upTx;
	else
		currentTx = downTx;
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
			auto ev = in.pollForEvent();

			if(ev)
			{
				DEBUG("Scancode: %d", ev->scancode);
				processEvent(*ev);
			}
		}

//		if(!(i++ % 100))
//			std::cout << "i = " << i << std::endl;

		renderButtons();
		engine->update(); // VSync introduces delay
		SDL_Delay(1000);
	}
}

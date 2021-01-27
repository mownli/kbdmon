#include "app.h"
#include "sdl_adapter.h"
#include <cassert>


App::App(Conf& conf) :
	engine(std::make_unique<SDLAdapter>(
		conf.title, conf.w, conf.h)),
	input(conf.kbdPath)
{
	engine->setupFont(conf.fontPath, conf.fontSize);

	for(auto row = conf.codesLayout.cbegin(); row != conf.codesLayout.cend(); row++)
		for(auto i = row->cbegin(); i != row->cend(); i++)
			scanCodesSet.insert(*i);

	GraphicalEngineAbstract::Color color1 = {.r = 50, .g = 50, .b = 50};
	GraphicalEngineAbstract::Color color2 = {.r = 200, .g = 200, .b = 200};

	for(int scode : scanCodesSet)
	{
		buttons[scode] = std::make_unique<Button>(
			engine->makeTextureFromText(scanCodeMap.at(scode), color1),
			engine->makeTextureFromText(scanCodeMap.at(scode), color2),
			0, 0);
	}

	auto i = scanCodesSet.cbegin();
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
	assert(!buttons.empty());
	for(auto scode : scanCodesSet)
	{
//		auto b = buttons.at(scode).get();
//		engine->renderTexture(b->getCurrentTx(), b->getX(), b->getY());
		auto b = buttons.at(scode).get();
		b->getCurrentTx()->renderTexture(b->getX(), b->getY());
	}
}

void App::processEvent(const LibinputWrapper::Event& ev)
{
	if(scanCodesSet.find(ev.scancode) == scanCodesSet.cend())
	{
		DEBUG("Ignored scancode %d", ev.scancode);
		return;
	}

	assert(!buttons.empty());
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
			auto ev = input.pollForEvent();

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
		//SDL_Delay(1000);
	}
}

#ifndef GRAPHICAL_ENGINE_ABSTRACT_H
#define GRAPHICAL_ENGINE_ABSTRACT_H

#include "texture_abstract.h"
#include <cstdint>
#include <string>
#include "dbg.h"

class GraphicalEngineAbstract
{
public:
	struct Color
	{
		uint8_t  r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

	virtual ~GraphicalEngineAbstract() {};

	virtual void renderTexture(TextureAbstract* tx, int x, int y) = 0;
	virtual int setupFont(const std::string& path, int size) = 0;
	virtual void clear() = 0;
	virtual void update() = 0;
	virtual TextureAbstract* makeTextureFromText(const std::string& text, Color& color) = 0;
	virtual int processEvents() = 0;
};

#endif // GRAPHICAL_ENGINE_ABSTRACT_H

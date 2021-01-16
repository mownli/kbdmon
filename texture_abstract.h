#ifndef TEXTURE_ABSTRACT_H
#define TEXTURE_ABSTRACT_H

class TextureAbstract
{
public:
	virtual ~TextureAbstract() {};
	virtual void renderTexture(int x, int y) = 0;
};

#endif // TEXTURE_ABSTRACT_H

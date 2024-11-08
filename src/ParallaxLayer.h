#pragma once

#include <SFML/Graphics.hpp>

class ParallaxLayer
{
public:
	ParallaxLayer(const sf::Texture& tex, unsigned int index);

	void update(float deltaX);
	static void setWindow(sf::RenderWindow* window);
	static void setNumLayers(unsigned int numLayers);

private:
	sf::Sprite m_sprite;
	unsigned int m_layerIndex;
	
	static sf::RenderWindow* s_window;
	static unsigned int s_numLayers;
};
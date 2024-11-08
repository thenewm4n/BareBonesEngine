#include "ParallaxLayer.h"

// Initialisation of static member variable must be outside class definition
sf::RenderWindow* ParallaxLayer::s_window = nullptr;
unsigned int ParallaxLayer::s_numLayers = 0;

ParallaxLayer::ParallaxLayer(const sf::Texture& tex, unsigned int index)
	: m_layerIndex(index)
{
	m_sprite.setTexture(tex);

	// Move background into view
	float viewSizeY = s_window->getView().getSize().y;
	m_sprite.move(sf::Vector2f(0, -viewSizeY));
}

void ParallaxLayer::setWindow(sf::RenderWindow* window)
{
	s_window = window;
}

void ParallaxLayer::setNumLayers(unsigned int numLayers)
{
	s_numLayers = numLayers;
}

void ParallaxLayer::update(float deltaX)
{
	sf::Vector2f currentPosition = m_sprite.getPosition();

	// Move texture according to this change * speed according to layer index
	float speed = static_cast<float>(s_numLayers - m_layerIndex) / s_numLayers;
	float offsetX = currentPosition.x + deltaX * speed;

	// Reset texture position
	m_sprite.setPosition(offsetX, currentPosition.y);

	// Render sprite to window
	if (s_window)
	{
		s_window->draw(m_sprite);
	}	
}
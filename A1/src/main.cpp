#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

class NewShape
{
public:
	std::unique_ptr<sf::Shape> m_sprite;
	bool m_draw = true;
	//std::string m_textString;		Might not be necessary as we aren't changing it
	sf::Text m_text;
	sf::Vector2f m_velocity;

public:
	NewShape(sf::Vector2f velocity)					// std::string text
		: m_velocity(velocity) {}					// m_textString(text) 

	void move()
	{
		sf::Vector2f position = m_sprite->getPosition();
		m_sprite->setPosition(position + m_velocity);
	}
};

void readConfigFile(std::ifstream& file, std::vector<std::unique_ptr<NewShape>>& shapes, sf::Vector2i& windowDimensions)
{
	// For all shapes' text
	sf::Font font;
	std::string fontPath;
	int textSize;
	int textColour[3];

	// For reading config file
	std::string line;
	std::string firstElement;

	while (std::getline(file, line))
	{
		// Decomposes line
		std::istringstream lineStream(line);
		if (lineStream >> firstElement)
		{
			if (firstElement == "Window")
			{
				lineStream >> windowDimensions.x >> windowDimensions.y;
			}
			else if (firstElement == "Font")
			{
				lineStream >> fontPath >> textSize >> textColour[0] >> textColour[1] >> textColour[2];

				if (!font.loadFromFile(fontPath))
				{
					std::cerr << "Could not load font!\n";
					exit(-1);
				}
			}
			else
			{
				std::string shapeText;
				sf::Vector2f position, velocity, size;
				int r, g, b;																					// Necessary since sf::Color RGB components are Uint8s, so are read by istringstream as chars, not ints
				float radius;

				// Extract common attributes to rectangle and circle
				lineStream >> shapeText >> position.x >> position.y >> velocity.x >> velocity.y >> r >> g >> b;

				// Instantiate NewShape
				std::unique_ptr<NewShape> shape = std::make_unique<NewShape>(velocity);

				if (firstElement == "Rectangle")
				{
					lineStream >> size.x >> size.y;
					shape->m_sprite = std::make_unique<sf::RectangleShape>(size);
				}
				else
				{
					lineStream >> radius;
					shape->m_sprite = std::make_unique<sf::CircleShape>(radius);
				}

				// Set position and colour of sprite (not possible with constructor)
				shape->m_sprite->setPosition(position);
				shape->m_sprite->setFillColor(sf::Color(static_cast<sf::Uint8>(r), static_cast<sf::Uint8>(g), static_cast<sf::Uint8>(b)));

				// Set text string, font, size, colour
				shape->m_text = sf::Text(shapeText, font, textSize);
				shape->m_text.setFillColor(sf::Color(textColour[0], textColour[1], textColour[2]));

				// Add shape to vector of shape pointers
				shapes.push_back(std::move(shape));
			}
		}
	}
}

int main(int argc, char* argv[])
{
	// Reads config file
	std::ifstream file("config.txt");
	if (!file.is_open())
	{
		std::cerr << "Error opening file." << std::endl;
		exit(-2);
	}

	// Vector of shapes
	std::vector<std::unique_ptr<NewShape>> shapes;

	// For window; defaults to these unless changed by file
	sf::Vector2i windowDimensions(1920, 1080);

	readConfigFile(file, shapes, windowDimensions);

	file.close();
	
	sf::RenderWindow window(sf::VideoMode(windowDimensions.x, windowDimensions.y), "SFML works!");
	window.setFramerateLimit(60);

	ImGui::SFML::Init(window);
	sf::Clock deltaClock;

	ImGui::GetStyle().ScaleAllSizes(3.5f);		// Scales imgui GUI
	ImGui::GetIO().FontGlobalScale = 1.3f;		// Scale imgui text size

	float c[3] = { 0.0f, 1.0f, 1.0f };	// imgui requires RGB values as floats from 0-1

	float circleRadius = 50.0f;
	int circleSegments = 32;		// the number of segments to draw the circle with i.e. how detailed?
	sf::Vector2f circleVelocity(1.0f, 0.5f);
	bool drawCircle = true;
	bool drawText = true;

	// This will get replaced by dynamic instantiation of objects, I believe
	sf::CircleShape circle(circleRadius, circleSegments);
	circle.setPosition(10.0f, 10.0f);


	// Array for imgui text
	char displayString[255] = "Sample Text";

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Passes the event to imgui; needs to know whether it has been clicked
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// Updates imgui for this frame with the time the last frame took; .restart() returns the sf::Time object and restarts the clock
		ImGui::SFML::Update(window, deltaClock.restart());

		// Draw the UI
		ImGui::Begin("Window title");
		ImGui::Text("Window text!");
		ImGui::Checkbox("Draw Circle", &drawCircle);
		ImGui::SameLine();
		ImGui::Checkbox("Draw Text", &drawText);
		ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
		ImGui::SliderInt("Sides", &circleSegments, 3, 64);
		ImGui::ColorEdit3("Color Circle", c);
		ImGui::InputText("Text", displayString, 255);
		if (ImGui::Button("Set Text"))
		{
			//text.setString(displayString);
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset Circle"))
		{
			circle.setPosition(0, 0);
		}
		ImGui::End();

		// Update logic
		/*
		for (auto& shape : shapes)
		{
			const sf::FloatRect& shapeBounds = shape->m_sprite->getGlobalBounds();\
			cost sf::FloatRect& textBounds = shape->m_text->getLocalBounds();
			if (shapeBounds.left < 0 || shapeBounds.left + shapeBounds.width > wWidth)
				shape->velocity.x *= -1.0f;

			if (shapeBounds.top < 0 || shapeBounds.top + shapeBounds.height > wHeight)
				shape->velocity.y *= -1.0f;

			shape->move();

			// Sets text to center of shape
			shape->m_text.setPosition(shapeBounds.left + shapeBounds.width / 2.f - textBounds.width / 2.f, shapeBounds.top + shapeBounds.height / 2.f - textBounds.height / 2.f);

			shape->m_sprite->setFillColour(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255);		// converts from a value 0-1 to 0-255
			shape->m_sprite->setPointCount(WHAT GOES HERE);
			shape->m_sprite->setRadius(WHAT GOES HERE);			INSTEAD OF .setRadius() -> .scale()
		*/

		circle.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255));		// convert from a value 0-1 to 0-255
		circle.setPointCount(circleSegments);
		circle.setRadius(circleRadius);

		// Moves the circle, as long as the x and y positions are within from
		circle.setPosition(circle.getPosition().x + circleVelocity.x, circle.getPosition().y + circleVelocity.y);
		sf::FloatRect circleBounds = circle.getGlobalBounds();
		//sf::FloatRect textBounds = text.getLocalBounds();
		//text.setPosition(circleBounds.left + circleBounds.width / 2.f - textBounds.width / 2.f, circleBounds.top + circleBounds.height / 2.f - textBounds.height / 2.f);


		// Drawing to window
		window.clear();

		/*
		for (auto& shape : shapes)
		{
			if (shape->m_draw)
			{
				window.draw(shape->m_sprite);
				window.draw(shape->m_text);
			}
		}
		*/

		if (drawCircle)
		{
			window.draw(circle);
		}
		if (drawText)
		{
			//window.draw(text);
		}
		ImGui::SFML::Render(window);			// Render the imgui GUI to the back buffer window
		window.display();						// Switches the front and back buffers
	}

	return 0;
}

// void setWindowAndFont (
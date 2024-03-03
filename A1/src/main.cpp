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
	std::string m_textString;
	sf::Text m_text;
	sf::Vector2f m_velocity;

public:
	NewShape(std::string text, sf::Vector2f velocity)
		: m_textString(text), m_velocity(velocity) {}

void move()
{
	sf::Vector2f position = m_sprite->getPosition();
	m_sprite->setPosition(position + m_velocity);
}
};


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
	int wWidth = 1920;
	int wHeight = 1080;

	// For each shape's text
	sf::Font myFont;
	std::string fontPath;
	int textSize;
	int textColour[3];

	// For reading config file
	std::string line;
	std::string firstElement;
	
	while (std::getline(file, line))
	{
		// Despite size being exclusive to rectangle and radius to circle, included here for readability
		std::string shapeText;
		sf::Vector2f position, velocity, size;
		int r, g, b;									// Necessary since sf::Color RGB components are Uint8s, so are read by istringstream as chars, not ints
		float radius;

		// Decomposes line
		std::istringstream lineStream(line);
		if (lineStream >> firstElement)
		{
			if (firstElement == "Window")
			{
				lineStream >> wWidth >> wHeight;
			}
			else if (firstElement == "Font")
			{
				lineStream >> fontPath >> textSize >> textColour[0] >> textColour[1] >> textColour[2];
				
				if (!myFont.loadFromFile(fontPath))
				{
					std::cerr << "Could not load font!\n";
					exit(-1);
				}
			}
			else
			{
				// Extract common attributes to rectangle and circle
				lineStream >> shapeText >> position.x >> position.y >> velocity.x >> velocity.y >> r >> g >> b;

				// Instantiate pointer to NewShape
				std::unique_ptr<NewShape> shape = std::make_unique<NewShape>(shapeText, velocity);

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

				// Add shape to vector of shape pointers
				shapes.push_back(std::move(shape));
			}
		}
	}

	file.close();
	
	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
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

	// Text that will be for each shape
	sf::Text text("Sample Text", myFont, 24);
	text.setPosition(0, wHeight - (float)text.getCharacterSize());

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
		if (ImGui::Button("Set Text"))			// if the button "Set Text" is clicked, set the text to the input text
		{
			text.setString(displayString);
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
			const sf::FloatRect& rect = shape->m_sprite->getGlobalBounds();
			if (rect.left < 0 || rect.left + rect.width > wWidth)
				shape->velocity.x *= -1.0f;

			if (rect.top < 0 || rect.top + rect.height > wHeight)
				shape->velocity.y *= -1.0f;

			shape->move();

			shape->m_sprite->setFillColour(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255);		// converts from a value 0-1 to 0-255
			shape->m_sprite->setPointCount(WHAT GOES HERE);
			shape->m_sprite->setRadius(WHAT GOES HERE);
		}
		*/

		circle.setFillColor(sf::Color(c[0] * 255, c[1] * 255, c[2] * 255));		// convert from a value 0-1 to 0-255
		circle.setPointCount(circleSegments);
		circle.setRadius(circleRadius);

		// Moves the circle, as long as the x and y positions are within from
		circle.setPosition(circle.getPosition().x + circleVelocity.x, circle.getPosition().y + circleVelocity.y);


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
			window.draw(text);
		}
		ImGui::SFML::Render(window);		// Render the imgui GUI to the SFML window
		window.display();			// Switches the front and back buffers
	}

	return 0;
}

// void setWindowAndFont (
#include <iostream>
#include <memory>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_demo.cpp"

class NewShape
{
public:
	std::unique_ptr<sf::Shape> m_sprite;
	bool m_draw = true;
	sf::Text m_text;
	sf::Vector2f m_velocity;
	float m_scale = 1.f;

public:
	NewShape(sf::Vector2f velocity)					// std::string text
		: m_velocity(velocity) {}

	void move()
	{
		sf::Vector2f position = m_sprite->getPosition();
		m_sprite->setPosition(position + m_velocity);
	}
};

void readConfigFile(std::ifstream& file, std::vector<std::shared_ptr<NewShape>>& shapes, sf::Vector2i& windowDimensions, sf::Font& font)
{
	// For all shapes' text
	std::string fontPath;
	int textSize = 0;
	int textColour[3] = { 0, 0, 0 };

	// For reading config file
	std::string line;
	std::string firstElement;

	while (std::getline(file, line))
	{
		// Decomposes line
		std::istringstream lineStream(line);

		// If line isn't whitespace, extract until whitespace
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
					std::cerr << "Could not load font!" << std::endl;
					exit(-1);
				}
			}
			else
			{
				std::string shapeText;
				sf::Vector2f position, size, velocity;
				int shapeColour[3] = { 0, 0, 0 };    // Necessary since sf::Color RGB components are Uint8s, so are read by istringstream as chars, not ints
				float radius;

				// Extract common attributes
				lineStream >> shapeText >> position.x >> position.y >> velocity.x >> velocity.y >> shapeColour[0] >> shapeColour[1] >> shapeColour[2];

				// Instantiate NewShape
				std::shared_ptr<NewShape> shape = std::make_shared<NewShape>(velocity);

				if (firstElement == "Rectangle")
				{
					lineStream >> size.x >> size.y;
					shape->m_sprite = std::make_unique<sf::RectangleShape>(size);
				}
				else		// i.e. if Circle
				{
					lineStream >> radius;
					shape->m_sprite = std::make_unique<sf::CircleShape>(radius);
				}

				// Set position and colour of sprite (not possible with constructor)
				shape->m_sprite->setPosition(position);
				shape->m_sprite->setFillColor(sf::Color(shapeColour[0], shapeColour[1], shapeColour[2]));		// Should I static_cast<sf::Uint8> for correctness?

				// Set text string, font, size, colour
				shape->m_text = sf::Text(sf::String(shapeText), font, textSize);
				shape->m_text.setFillColor(sf::Color(textColour[0], textColour[1], textColour[2]));

				// Add shape to vector of shape pointers
				shapes.push_back(shape);
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
	std::vector<std::shared_ptr<NewShape>> shapes;
	sf::Vector2i windowDimensions(1920, 1080);
	sf::Font font;
	
	// Instantiates sf::Window, sf::Font and NewShapes according to config.txt
	readConfigFile(file, shapes, windowDimensions, font);
	file.close();
	
	sf::RenderWindow window(sf::VideoMode(windowDimensions.x, windowDimensions.y), "Assignment 1");
	window.setFramerateLimit(60);
	sf::Clock deltaClock;

	ImGui::SFML::Init(window);
	ImGui::GetStyle().ScaleAllSizes(3.5f);		// Scales imgui GUI
	ImGui::GetIO().FontGlobalScale = 1.3f;		// Scale imgui text size

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Passes the event to imgui; needs to know whether it has been clicked
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Updates imgui for this frame with the time the last frame took; .restart() returns the sf::Time object and restarts the clock
		ImGui::SFML::Update(window, deltaClock.restart());

		// GUI dropdown box index
		static int dropdownIndex = 0;

		// Prepare GUI textbox buffer
		static char guiString[255];

		// Draw the UI
		ImGui::Begin("Shape Properties");
		if (ImGui::BeginCombo("##dropdown", shapes[dropdownIndex]->m_text.getString().toAnsiString().c_str()))
		{
			for (int i = 0; i < shapes.size(); i++)
			{
				const bool isSelected = (dropdownIndex == i);
				if (ImGui::Selectable(shapes[i]->m_text.getString().toAnsiString().c_str(), isSelected))
					dropdownIndex = i;

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		std::shared_ptr<NewShape> currentShape = shapes[dropdownIndex];
		
		// Copies shape name to textbox buffer
		strncpy_s(guiString, currentShape->m_text.getString().toAnsiString().c_str(), 254);	// strncpy_s on Windows; strncpy on Cac
		//guiString[254] = '\0';		// only necessary with srncpy on Mac
		
		// Gets sf::Color of shape and convert to float array (imgui requires RGB values as floats from 0-1)
		sf::Color shapeColour = currentShape->m_sprite->getFillColor();
		float shapeColourFloat[3] = { static_cast<float>(shapeColour.r) / 256.f, static_cast<float>(shapeColour.g) / 256.f, static_cast<float>(shapeColour.b) / 256.f };
		
		// Gets the velocity as a float array (necessary for ImGui SliderFloat2)
		float velocityVec2f[2] = { currentShape->m_velocity.x, currentShape->m_velocity.y };

		ImGui::Checkbox("Draw Shape", &currentShape->m_draw);
		ImGui::SliderFloat2("Velocity", velocityVec2f, -8.f, 8.f);
		ImGui::SliderFloat("Scale", &currentShape->m_scale, 0.f, 4.f);
		ImGui::ColorEdit3("Color", shapeColourFloat);
		ImGui::InputText("Name", guiString, 255);
		ImGui::End();

		// Update shape attributes
		shapeColour = sf::Color(
			static_cast<sf::Uint8>(shapeColourFloat[0] * 256.f),
			static_cast<sf::Uint8>(shapeColourFloat[1] * 256.f),
			static_cast<sf::Uint8>(shapeColourFloat[2] * 256.f)
		);
		currentShape->m_velocity = sf::Vector2f(velocityVec2f[0], velocityVec2f[1]);
		currentShape->m_sprite->setScale(sf::Vector2f(currentShape->m_scale, currentShape->m_scale));
		currentShape->m_sprite->setFillColor(shapeColour);
		currentShape->m_text.setString(sf::String(guiString));

		// Update logic
		for (auto& shape : shapes)
		{
			sf::FloatRect shapeBounds = shape->m_sprite->getGlobalBounds();

			// If bounds exceed window edge, reverse corresponding velocity
			if (shapeBounds.left < 0 || shapeBounds.left + shapeBounds.width > windowDimensions.x)
				shape->m_velocity.x *= -1.f;

			if (shapeBounds.top < 0 || shapeBounds.top + shapeBounds.height > windowDimensions.y)
				shape->m_velocity.y *= -1.f;

			shape->move();

			// Gets new bounds after shape has moved
			shapeBounds = shape->m_sprite->getGlobalBounds();
			const sf::FloatRect textBounds = shape->m_text.getLocalBounds();

			// Sets text to center of shape
			shape->m_text.setPosition(shapeBounds.left + shapeBounds.width / 2.f - textBounds.width / 2.f, shapeBounds.top + shapeBounds.height / 2.f - textBounds.height / 2.f);
		}

		// Clear window
		window.clear();

		// Draw each shape to window
		for (auto& shape : shapes)
		{
			if (shape->m_draw)
			{
				window.draw(*(shape->m_sprite));
				window.draw(shape->m_text);
			}
		}

		ImGui::SFML::Render(window);			// Render the imgui GUI to the back buffer window
		window.display();						// Switches the front and back buffers
	}

	return 0;
}
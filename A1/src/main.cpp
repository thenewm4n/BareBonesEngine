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
	//std::string m_textString;					Might not be necessary as we aren't changing it
	sf::Text m_text;
	sf::Vector2f m_velocity;
	float m_scale = 1.f;
	float m_colour[3] = { 0.f, 1.f, 1.f };			// imgui requires RGB values as floats from 0-1
	char m_guiString[255];

public:
	NewShape(const std::string& shapeName, sf::Vector2f velocity)					// std::string text
		: m_velocity(velocity)
	{
		strncpy_s(m_guiString, shapeName.c_str(), sizeof(m_guiString) - 1);
	}

	void move()
	{
		sf::Vector2f position = m_sprite->getPosition();
		m_sprite->setPosition(position + m_velocity);
	}
};

void readConfigFile(std::ifstream& file, std::vector<std::shared_ptr<NewShape>>& shapes, sf::Vector2i& windowDimensions)
{
	// For all shapes' text
	sf::Font font;
	std::string fontPath;
	int textSize = 0;
	int textColour[3] = { 0.f, 0.f, 0.f };

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
				int r, g, b;	                            // Necessary since sf::Color RGB components are Uint8s, so are read by istringstream as chars, not ints
				float radius;

				// Extract common attributes to rectangle and circle
				lineStream >> shapeText >> position.x >> position.y >> velocity.x >> velocity.y >> r >> g >> b;

				// Instantiate NewShape
				std::shared_ptr<NewShape> shape = std::make_shared<NewShape>(shapeText, velocity);

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

				// Add shape to vector of shape pointers; std::move() required because shape is a std::unique_ptr
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

	// For window; defaults to these unless changed by file
	sf::Vector2i windowDimensions(1920, 1080);

	readConfigFile(file, shapes, windowDimensions);

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

		//ImGui::ShowDemoWindow();

		static int dropdownIndex = 0;

		// GUI dropdown box
		if (ImGui::BeginCombo("##dropdown", shapes[dropdownIndex]->m_text.getString().toAnsiString().c_str()))
		{
			for (int i = 0; i < shapes.size(); i++)
			{
				const bool isSelected = (dropdownIndex == i);
				if (ImGui::Selectable(shapes[i]->m_text.getString().toAnsiString().c_str()), isSelected)
					dropdownIndex = i;

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}

		std::shared_ptr<NewShape> currentShape = shapes[dropdownIndex];

		// Draw the UI
		ImGui::Begin("Shape Properties");
		ImGui::Checkbox("Draw Shape", &currentShape->m_draw);
		ImGui::SliderFloat("##XVelocity", &currentShape->m_velocity.x, 0.0f, 100.f);					// Does it need text?
		ImGui::SameLine();
		ImGui::SliderFloat("Velocity", &currentShape->m_velocity.y, 0.0f, 100.f);
		ImGui::SliderFloat("Scale", &currentShape->m_scale, 0.0f, 300.0f);
		ImGui::ColorEdit3("Color", currentShape->m_colour);
		ImGui::InputText("Name", currentShape->m_guiString, 255);									// Should directly change text, as below
		ImGui::End();

		currentShape->m_text.setString(currentShape->m_guiString);
		currentShape->m_sprite->setFillColor(sf::Color(currentShape->m_colour[0] * 255, currentShape->m_colour[1] * 255, currentShape->m_colour[2] * 255));		// converts from a value 0-1 to 0-255
		currentShape->m_sprite->setScale(sf::Vector2f(currentShape->m_scale, currentShape->m_scale));			// was .setRadius()

		// Update logic
		for (auto& shape : shapes)
		{
			const sf::FloatRect& shapeBounds = shape->m_sprite->getGlobalBounds();
			const sf::FloatRect& textBounds = shape->m_text.getLocalBounds();

			// If bounds exceed window edge, reverse corresponding velocity
			if (shapeBounds.left < 0 || shapeBounds.left + shapeBounds.width > windowDimensions.x)
				shape->m_velocity.x *= -1.0f;

			if (shapeBounds.top < 0 || shapeBounds.top + shapeBounds.height > windowDimensions.y)
				shape->m_velocity.y *= -1.0f;

			shape->move();

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
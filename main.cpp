#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <array>
#include <cmath>

const unsigned int width = 1920;
const unsigned int height = 1920;

// Function to convert 3D array to 2D graphical array
std::array<float, 2> project(const std::array<float, 3>& point3D) {
	float x = point3D[0];
	float y = point3D[1];
	float z = point3D[2];

	if (z <= 0.0f) {
		z = 0.01f;
	}

	return { x / z, y / z };
}

// Translate 2D graphical array into values to position within width and height
std::array<float, 2> translate(const std::array<float, 2>& point2D) {
	float x = point2D[0];
	float y = point2D[1];

	return {
		(x + 1.0f) * (width / 2.0f),
		(1.0f - y) * (height / 2.0f)
	};
}

sf::CircleShape createPoint(const std::array<float, 2>& position) {
	const float scale = 10.0f; // Circle size
	sf::CircleShape shape(scale);
	shape.setPosition({ position[0], position[1] });
	shape.setFillColor(sf::Color::Green);
	return shape;
}

std::array<float, 3> translate_z(std::array<float, 3> point, float dz) {
	return {
		point[0],
		point[1],
		point[2] + dz
	};
}

std::array<float, 3> rotate_xz(std::array<float, 3> point, float angle) {
	float x = point[0];
	float z = point[2];
	float c = std::cos(angle);
	float s = std::sin(angle);

	// Rotate point
	float x_prime = x * c - z * s;
	float z_prime = x * s + z * c;
	
	point[0] = x_prime;
	point[2] = z_prime;

	return point;
}

void line(float p1, float p2) {
	
}

int main() {
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ width, height }), "3D Renderer");
	const float FPS = 60.0f;
	const float dt = 1.0f / FPS;
	float dz = 1;
	float angle = 0;
	window->setFramerateLimit(FPS);

	std::array<std::array<float, 3>, 8> points3D = {{
		{ 0.25f, -0.25f, 0.25f },
		{ 0.25f, 0.25f, 0.25f },
		{ -0.25f, -0.25f, 0.25f },
		{ -0.25f, 0.25f, 0.25f },

		{ 0.25f, -0.25f, -0.25f },
		{ 0.25f, 0.25f, -0.25f },
		{ -0.25f, -0.25f, -0.25f },
		{ -0.25f, 0.25f, -0.25f },
	}};

	while(window->isOpen()) {
		while (const std::optional event = window->pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window->close();
			}
			else if (const auto*keyPressed = event->getIf<sf::Event::KeyPressed>()) {
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
					window->close();
				}
			}
		}

		window->clear();

		// dz += 1 * dt;
		angle += M_PI * dt;

		for (std::array<float, 3>& point3D : points3D) {
			std::array<float, 3> rotated_point = rotate_xz(point3D, angle);
			std::array<float, 3> z_translated = translate_z(rotated_point, dz);
			std::array<float, 2> p2D = project(z_translated);
			std::array<float, 2> translated = translate(p2D);

			sf::CircleShape shape = createPoint(translated);
			window->draw(shape);
		}

		window->display();

	}

	return 0;
}

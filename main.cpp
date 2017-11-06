#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>
#include <iostream>
#include "sfCam.h"
using namespace std;



int main()
{
	bool rbwp=false;
	int deltaMouseWheelMoved;
	sf::Texture texture;
	texture.loadFromFile("C:/Users/Digital.Life/Pictures/sobaka.png");
	sf::Sprite dog;
	dog.setTexture(texture);
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
	sf::Event event;
	//sf::View view;
	sfCam path(&window);
//	path.addPoint(sf::Vector2f(18, 18));
//	path.addPoint(sf::Vector2f(300, 300));
//	path.addPoint(sf::Vector2f(600, 100));
//	path.addPoint(sf::Vector2f(300, 200));
//	path.addPoint(sf::Vector2f(600, 20));
//	path.addPoint(sf::Vector2f(720, 300));
//	path.teleportTo(0);
	path.goForvard();
	sf::RectangleShape cam(sf::Vector2f(128, 72));	cam.setFillColor(sf::Color(0, 0, 0, 0)); cam.setOrigin(cam.getSize().x / 2, cam.getSize().y / 2);
	cam.setOutlineThickness(3); cam.setPosition(300, 300); cam.setOutlineColor(sf::Color::Black);
	while (window.isOpen())
	{
		deltaMouseWheelMoved = 0;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::MouseWheelMoved:
				deltaMouseWheelMoved = event.mouseWheel.delta;
				break;
			default:
				break;
			}
		}

		path.goForvard();
		cam.setPosition(path.curPos);
		dog.setPosition(path.curPos);
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && !rbwp)
		{
			path.addPoint(sf::Vector2f(sf::Mouse::getPosition(window)));
			rbwp = true;
		}
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Right))
			rbwp = false;

		window.clear();// sf::Color::White);
		path.draw();
		path.shiftPoint(sf::Vector2f(sf::Mouse::getPosition(window)), sf::Mouse::isButtonPressed(sf::Mouse::Left), deltaMouseWheelMoved);
		//window.draw(cam);
		window.draw(dog);
		window.display();
	}
	return 0;
}
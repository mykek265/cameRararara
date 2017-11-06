#ifndef SFCAM_H
#define SFCAM_H
#include <SFML\Graphics.hpp>
#include <vector>
#include <iostream>
using namespace std;

class sfCam
{
private:
	sf::RenderWindow* window;
	vector<sf::Vector2f> points;//L
	vector<sf::Vector2f> bluePoints;//L-2
	vector<sf::Vector2f> breakPoints;//L-3
	vector<sf::Vector2f> a;
	vector<sf::Vector2f> b;
	vector<sf::Vector2f> c;
	vector<sf::Time> fTime;
	int dt;
	float t;//"time" from one point to another ~ 1; so, all "time" ~ is points.size()+1
	vector<float> v;//velosity in current position
	int L;//length of path
	int Ncf;//num of current function
	sf::CircleShape circ;//points
	sf::RenderTexture paper;
	sf::Sprite sprPaper;
	sf::Clock timer;
	int shiftThat;
	void triging();
public:
	sf::Vector2f curPos;

	sfCam();
	sfCam(sf::RenderWindow* window);

	void calculatePath();//calculation of a,b,c and d
	void teleportTo(int N);
	void goForvard();
	void goBack();
	void stop();
	void setVelosity(float* v);
	void addPoint(sf::Vector2f p);
	void shiftPoint(sf::Vector2f mpos, bool click,int dt);
	void draw();
	~sfCam();
};

#endif
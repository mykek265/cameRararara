#include "sfCam.h"
#include <vector>
using namespace std;


sfCam::sfCam()
{
}

sfCam::sfCam(sf::RenderWindow* w)
{
	window = w;
	circ.setRadius(3.f);
}

void sfCam::calculatePath()//calculation of a,b,c and d
{
	L = points.size();
	sf::Vector2f r, p[3];
	a.clear(); b.clear(); c.clear();
	breakPoints.clear(); bluePoints.clear();
	fTime.clear();
	{//Bezier curves
		/* рассчёт  псевдо-вершин и точек соединения функций*/
		/*
		for (int i = 0; i < L - 3; i++)
		{
			r.x = (points[i+1].x + points[i + 2].x) / 2;
			r.y = (points[i+1].y + points[i + 2].y) / 2;
			breakPoints.push_back(r);
		}
		for (int i = 0; i < L - 2; i++)
		{
			if (i == 0)//для первой функции
			{
				r.x = points[1].x * 2 - (points[0].x + breakPoints[0].x) / 2;
				r.y = points[1].y * 2 - (points[0].y + breakPoints[0].y) / 2;
			}
			if (i != 0 && i != L - 3)//для средних
			{
				r.x = points[i+1].x * 2 - (breakPoints[i-1].x + breakPoints[i].x) / 2;
				r.y = points[i+1].y * 2 - (breakPoints[i-1].y + breakPoints[i].y) / 2;
			}
			if (i == L - 3 && i != 0)//для последней
			{
				r.x = points[L-2].x * 2 - (breakPoints[L-4].x + points[L-1].x) / 2;
				r.y = points[L-2].y * 2 - (breakPoints[L-4].y + points[L-1].y) / 2;
			}
			bluePoints.push_back(r);
		}*/
		sf::Time tttt;
		tttt = sf::seconds(1);
		for (int i = 0; i < L - 2; i++)
		{
			r.x = points[i + 1].x * 2 - (points[i].x + points[i + 2].x) / 2;
			r.y = points[i + 1].y * 2 - (points[i].y + points[i + 2].y) / 2;
			bluePoints.push_back(r);
			fTime.push_back(tttt);
		}
		for (int i = 0; i < L - 3; i++)
		{
			r.x = (bluePoints[i].x + bluePoints[i + 1].x) / 2;
			r.y = (bluePoints[i].y + bluePoints[i + 1].y) / 2;
			breakPoints.push_back(r);
		}
		/* Рассчёт коэффициентов */
		if (L>2)
		for (int i = 0; i < L - 2; i++)
		{
			if (i == 0)
			{
				p[0] = points[0];
				p[1] = bluePoints[0];
				if (L>3)
					p[2] = breakPoints[0];
			}
			if (i>0 && i < L - 3)
			{
				p[0] = breakPoints[i - 1];
				p[1] = bluePoints[i];
				if (L>3)
					p[2] = breakPoints[i];
			}
			if (i == L - 3)
			{
				if (L>3)
					p[0] = breakPoints[L - 4];
				p[1] = bluePoints[L - 3];
				p[2] = points[L - 1];
			}
			a.push_back(sf::Vector2f(p[2].x - 2 * p[1].x + p[0].x, p[2].y - 2 * p[1].y + p[0].y));
			b.push_back(sf::Vector2f(2 * (p[1].x-p[0].x), 2 * (p[1].y-p[0].y)));
			c.push_back(sf::Vector2f(p[0]));
		}
	}
	if (L == 2)//line
	{
		a.push_back(sf::Vector2f((points[0].y - points[1].y) / (points[0].x - points[1].x), 0));
	}
}

void sfCam::teleportTo(int N)
{
	Ncf = N;
	curPos=(points[N]);
	timer.restart();
}

void sfCam::goForvard()
{
	if (L > 2)
	{
		if (Ncf == L - 2)
		{
			Ncf = 0;
			timer.restart();
		}
		float ct = timer.getElapsedTime().asSeconds()/fTime[Ncf].asSeconds();
		curPos.x = ct*ct*a[Ncf].x + b[Ncf].x * ct + c[Ncf].x;
		curPos.y = ct*ct*a[Ncf].y + b[Ncf].y * ct + c[Ncf].y;
		if (ct >= 1.f && Ncf < L - 2)
		{
			Ncf++;
			timer.restart();
		}
	}
}

void sfCam::goBack()
{

}

void sfCam::stop()
{

}

void sfCam::setVelosity(float* v)
{

}

void sfCam::addPoint(sf::Vector2f p)
{
	points.push_back(p);//Добавили новую точку и вычислили коэффициенты кривой)
	this->calculatePath();
}

void sfCam::shiftPoint(sf::Vector2f mpos, bool click, int ddt)
{
	for (int i = 0; i < L; i++)
	{
		float m=std::sqrt((points[i].x - mpos.x)*(points[i].x - mpos.x) + (points[i].y - mpos.y)*(points[i].y - mpos.y));
			std::cout <<fTime.size()<< std::endl;
		if (m < 18)
		{
			circ.setFillColor(sf::Color::Red);
			circ.setPosition(points[i]);
			window->draw(circ);
			if (i != 0 && i != L-1 && ddt)
			{
				fTime[i-1]=sf::seconds(fTime[i-1].asSeconds()+ddt*0.01);
			}
			if (click)
			{
				points[i] = mpos;
				this->calculatePath();
				i = L;
			}
		}
	}
}

void sfCam::draw()
{
	circ.setFillColor(sf::Color(100, 100, 100, 100));
	sf::Vector2f ppp;
	if (L>2)
	for (int i = 0; i < L-2; i++)
	{
		for (float ttt = 0; ttt < 1; ttt = ttt + 0.01)
		{
			ppp.x = ttt*ttt*a[i].x + b[i].x * ttt + c[i].x;
			ppp.y = ttt*ttt*a[i].y + b[i].y * ttt + c[i].y;
			circ.setPosition(ppp);
			window->draw(circ);
		}
	}
	circ.setFillColor(sf::Color::Blue);
	for (int i = 0; i < points.size(); i++)
	{
		circ.setPosition(points[i]);
		window->draw(circ);
	}
	circ.setFillColor(sf::Color::Cyan);
	for (int i = 0; i < bluePoints.size(); i++)
	{
		circ.setPosition(bluePoints[i]);
		window->draw(circ);
	}
	circ.setFillColor(sf::Color::Black);
	for (int i = 0; i < breakPoints.size(); i++)
	{
		circ.setPosition(breakPoints[i]);
		window->draw(circ);
	}
	sf::Sprite spr;
	sprPaper.setTexture(paper.getTexture());
	sprPaper.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(window->getSize())));
	//window->draw(sprPaper);
}

sfCam::~sfCam()
{
}

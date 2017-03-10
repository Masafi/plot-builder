#include <SFML/Graphics.hpp>
#include "Graph.h"
#include <vector>
#include <utility>
#include <cmath>
#include <map>
#include <string>
#include <iostream>

int main() {
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Graph");
	std::vector<Graph*> graphs;
	QuadraticFraction qf;
	qf.init(0, 0, 1, 0, 1, 0);
	qf.setColor(sf::Color::Cyan);
	graphs.push_back(&qf);
	Line horizontal;
	horizontal.init(0, 0);
	horizontal.setColor(sf::Color::Black);
	graphs.push_back(&horizontal);
	VerticalLine vertical;
	vertical.init(0);
	vertical.setColor(sf::Color::Black);
	graphs.push_back(&vertical);
	Parabola par;
	par.init(1, 0, 0);
	par.setColor(sf::Color::Black);
	//graphs.push_back(&par);
	QuadraticFraction second;
	second.init(1, 2, 3, 3, 2, 1);
	second.setColor(sf::Color::Red);
	graphs.push_back(&second);
	Polynomial pl;
	std::vector<double> koef(6);
	koef[0] = -1;
	koef[1] = 1;
	koef[2] = -1;
	koef[3] = 1;
	koef[4] = -1;
	koef[5] = 1;
	pl.init(koef);
	pl.setColor(sf::Color::Yellow);
	graphs.push_back(&pl);
	while(window.isOpen()) {
		window.clear(sf::Color::White);

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				break;
			}
		}
		
		left--;
		top++;
		right++;
		bottom--;

		for(int i = 0; i < graphs.size(); i++) {
			graphs[i]->update();
			window.draw(graphs[i]->render());
		}
		window.display();

	}
	return 0;
}

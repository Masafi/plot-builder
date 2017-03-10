#include <SFML/Graphics.hpp>

const int HEIGHT = 600;
const int WIDTH = 600;

int top = 1;
int bottom = -1;
int left = -1;
int right = 1;

enum class GraphType {
	None,
	Line,
	Parabola,
	QuadFrac,
	Polynomial,
	VerticalLine,
	Custom
};

std::map<std::string, GraphType> NameType = {
	{"None", GraphType::None},
	{"Line", GraphType::Line},
	{"Parabola", GraphType::Parabola},
	{"QuadFrac", GraphType::QuadFrac},
	{"Polynomial", GraphType::Polynomial},
	{"VerticalLine", GraphType::VerticalLine}
};

class Graph {
protected:
	int last;
	sf::VertexArray points;
	sf::Color color;
	GraphType type;
public:
	Graph() : points(sf::LineStrip, WIDTH + 10) {
		last = 1;
		type = GraphType::None;
	}

	GraphType getType() {
		return type;
	}

	void setColor(sf::Color _color) {
		color = _color;
	}

	bool check(sf::Vector2f& point) {
		return getValue(point.x) > point.y;
	}

	virtual double getValue(double x) = 0;

	virtual bool isZero(double x, double nx) = 0;

	sf::Vector2f coord(int x, int y) {
		double xdif = right - left;
		double ydif = top- bottom;
		double scx = x * xdif / WIDTH + left;
		double scy = (HEIGHT - y) * ydif / HEIGHT + bottom;
		return sf::Vector2f(scx, scy);
	}

	virtual void update() {
		bool doublePoint = false;
		last = 1;
		for(int i = 0; i < WIDTH; i++) {
			for(int j = 0; j < HEIGHT + 2; j++) {
				sf::Vector2f coords = coord(i, j);
				double nx = coord(i + 1, j).x;
				if(isZero(coords.x, nx)) {
					if(doublePoint) {
						break;
					}
					double prevy = points[last - 1].position.y;
					double prev2y = HEIGHT / 2;
					if(last >= 2) prev2y = points[last - 2].position.y;
					if(prevy > prev2y) {
						points[last].position = sf::Vector2f(i, 0);
					}
					else {
						points[last].position = sf::Vector2f(i, HEIGHT + 1);
					}
					points[last].color = color;
					last++;
					points[last].position = points[last - 1].position;
					points[last].color = sf::Color::Transparent;
					last++;
					doublePoint = true;
					break;
				}
				if(check(coords) || j > HEIGHT) {
					if(doublePoint) {
						points[last].position = sf::Vector2f(i, j);
						points[last].color = sf::Color::Transparent;
						last++;
						doublePoint = false;
					}
					points[last].position = sf::Vector2f(i, j);
					points[last].color = color;
					last++;
					break;
				}
			}
		}
		points[0].position = points[1].position;
		points[0].color = sf::Color::Transparent;
		points[last].position = points[last - 1].position;
		for(int i = last; i < WIDTH + 10; i++) {
			points[i].color = sf::Color::Transparent;
		}
	}

	sf::VertexArray& render() {
		return points;
	}
};

class Line : public Graph {
	double k;
	double b;
public:
	Line() : Graph() {
		type = GraphType::Line;
	}

	void init(double _k, double _b) {
		k = _k;
		b = _b;
	}

	void setK(double _k) {
		k = _k;
	}

	void setB(double _b) {
		b = _b;
	}

	bool isZero(double x, double nx) {
		return false;
	}

	double getValue(double x) {
		return k * x + b;
	}
};

class Parabola : public Graph {
	double a;
	double b;
	double c;
public:
	Parabola() : Graph() {
		type = GraphType::Parabola;
	}
	
	void init(double _a, double _b, double _c) {
		a = _a;
		b = _b;
		c = _c;
	}

	void setA(double _a) {
		a = _a;
	}

	void setB(double _b) {
		b = _b;
	}

	void setC(double _c) {
		c = _c;
	}

	bool isZero(double x, double nx) {
		return false;
	}

	double getValue(double x) {
		return a * x * x + b * x + c;
	}
};

class QuadraticFraction : public Graph {
	double A;
	double B;
	double C;
	double a;
	double b;
	double c;

	std::pair<double, bool> zero1;
	std::pair<double, bool> zero2;

	void recountZeroes() {
		zero1.second = false;
		zero2.second = false;
		if(a == 0 && b != 0) {
			zero1.second = true;
			zero1.first = -b / c;
		}
		else {
			double D = b * b - 4 * a * c;
			if(D == 0) {
				zero1.second = true;
				zero1.first = -b / (2 * a);
			}
			else if(D > 0) { 
				zero1.second = true;
				zero1.first = (-b + sqrt(D)) / (2 * a);
				zero2.second = true;
				zero2.first = (-b - sqrt(D)) / (2 * a);
			}
		}
	}
public:
	QuadraticFraction() : Graph() {
		type = GraphType::QuadFrac;
	}
	
	void init(double _A, double _B, double _C, double _a, double _b, double _c) {
		A = _A;
		B = _B;
		C = _C;
		a = _a;
		b = _b;
		c = _c;
		recountZeroes();
	}

	void setA(double _A) {
		A = _A;
	}
	void setB(double _B) {
		B = _B;
	}
	void setC(double _C) {
		C = _C;
	}
	void seta(double _a) {
		a = _a;
		recountZeroes();
	}
	void setb(double _b) {
		b = _b;
		recountZeroes();
	}
	void setc(double _c) {
		c = _c;
		recountZeroes();
	}
	
	bool isZero(double x, double nx) {
		bool good = false;
		if(zero1.second) {
			good = (x <= zero1.first && zero1.first <= nx);
		}
		if(zero2.second) {
			good = good || (x <= zero2.first && zero2.first <= nx);
		}	
		return good;
	}

	double getValue(double x) {
		return (A * x * x + B * x + C) / (a * x * x + b * x + c);
	}
};

class Polynomial : public Graph {
	std::vector<double> koef;
public:
	Polynomial() : Graph() {
		type = GraphType::Polynomial;
	}

	void init(std::vector<double>& _koef) {
		koef = std::vector<double>(_koef);
	}

	bool isZero(double x, double nx) {
		return false;
	}

	double getValue(double x) {
		double xp = 1;
		double val = 0;
		for(int i = 0; i < koef.size(); i++) {
			val += xp * koef[i];
			xp *= x;
		}
		return val;
	}
};

class VerticalLine : public Graph {
	double x;
public:
	VerticalLine() : Graph() {
		type = GraphType::VerticalLine;
		for(int i = 0; i < WIDTH + 10; i++) {
			points[i].color = sf::Color::Transparent;
		}
	}
	
	void init(double _x) {
		x = _x;
	}

	void setX(double _x) {
		x = _x;
	}
	
	bool isZero(double x, double nx) {
		return false;
	}

	double getValue(double x) {
		return 0;
	}

	void update() {
		last = 0;
		points[1].color = color;
		points[2].color = color;
		for(int i = -1; i < WIDTH + 1; i++) {
			double cx = coord(i, 0).x;
			if(cx >= x) {
				points[last].position = sf::Vector2f(i, 0);
				last++;
				points[last].position = sf::Vector2f(i, 0);
				last++;
				points[last].position = sf::Vector2f(i, HEIGHT + 1);
				last++;
				points[last].position = sf::Vector2f(i, HEIGHT + 1);
				break;
			}
		}
	}
};

class Custom : public Graph {
	double (*value)(double);
	
	bool (*zero)(double, double);
public:
	Custom() : Graph() {
		type = GraphType::Custom;
		value = nullptr;
		zero = nullptr;
	}

	void setValue(double (*_value)(double)) {
		value = _value;
	}

	void setZero(bool (*_zero)(double, double)) {
		zero = _zero;
	}
	
	bool isZero(double x, double nx) {
		if(zero != nullptr) {
			return zero(x, nx);
		}
		else {
			return false;
		}
	}

	double getValue(double x) {
		return value(x);
	}
};

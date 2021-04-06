#pragma once

#include <iostream>
#include <memory>

using namespace std;

class point {

public:
	char x;
	char y;
	//shared_ptr<point> next_p;
	point(char xx = 0, char yy = 0) {
		x = xx;
		y = yy;
	}

	/*point(int xx = 0, char yy = 0) {
		x = xx;
		y = yy;
	}*/

	point(const point &a) {
		x = a.x;
		y = a.y;
	}

	point operator+(const point &a)const {
		point result(a.x + x, a.y + y);
		return result;
	}

	point operator-(const point &a) {
		point result(x - a.x, y - a.y);
		return result;
	}

	point operator*(char m) const{
		point result(x*m, y*m);
		return result;
	}

	point &operator=(const point &a) {
		x = a.x;
		y = a.y;
		return *this;
	}

	bool operator==(const point &a)const {
		return (a.x == x && a.y == y);
	}

	bool operator!=(const point &a)const {
		return (a.x != x || a.y != y);
	}

	bool operator<(const point &a)const {
		if (y != a.y) { return y < a.y; }
		return x < a.x;
	}

	void show()const {
		cout << "x:" << (int)x << " y:" << (int)y << endl;
	}

	int get_distance(const point& p)const {
		return abs(x - p.x) + abs(y - p.y);
	}
};


#pragma once

class point {

public:
	char x;
	char y;

	point(char xx = 0, char yy = 0);

	point(const point &a);

	point operator+(const point &a)const;

	point operator-(const point &a)const;

	point operator*(char m) const;

	point& operator=(const point &a);

	bool operator==(const point &a)const;

	bool operator!=(const point &a)const;

	bool operator<(const point &a)const;

	void show()const;

	int get_distance(const point& p)const;
};


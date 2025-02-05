#include "point.h"
#include <iostream>
using namespace std;

point::point(char xx, char yy) {
    x = xx;
    y = yy;
}

point::point(const point &a) {
    x = a.x;
    y = a.y;
}

point point::operator+(const point &a)const {
    point result(a.x + x, a.y + y);
    return result;
}

point point::operator-(const point &a)const{
    point result(x - a.x, y - a.y);
    return result;
}

point point::operator*(char m) const{
    point result(x*m, y*m);
    return result;
}

point& point::operator=(const point &a) {
    x = a.x;
    y = a.y;
    return *this;
}

bool point::operator==(const point &a)const {
    return (a.x == x && a.y == y);
}

bool point::operator!=(const point &a)const {
    return (a.x != x || a.y != y);
}

bool point::operator<(const point &a)const {
    if (y != a.y) { return y < a.y; }
    return x < a.x;
}

void point::show()const {
    cout << "x:" << (int)x << " y:" << (int)y << endl;
}
#include <cstdio>
#include "point.h"

point::point(int8_t xx, int8_t yy) {
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

point point::operator*(int8_t m) const{
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
    printf("x:%d y:%d", int(x), int(y));
}
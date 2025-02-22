#pragma once
#include <cstdint>

class point {

public:
    int8_t x;
    int8_t y;
    point(int8_t xx = 0, int8_t yy = 0);
    point(const point &a);
    point operator+(const point &a)const;
    point operator-(const point &a)const;
    point operator*(int8_t m) const;
    point& operator=(const point &a);
    bool operator==(const point &a)const;
    bool operator!=(const point &a)const;
    bool operator<(const point &a)const;
    void show()const;
};
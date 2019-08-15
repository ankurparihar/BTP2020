#pragma once
#include "common.h"

template <typename T>
class Point {
public:
	union {
		struct {
			T x;
			T y;
		};
		T coords[2];
	};
	Point() {}
	Point(const T& n) : x(n), y(n) {}
	Point(const T& x, const T& y) : x(x), y(y) {}
	Point(const Point& p) : x(p.x), y(p.y) {}
};
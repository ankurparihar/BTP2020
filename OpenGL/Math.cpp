#include "Math.h"

template <typename T>
Point<T>& Point<T>::operator+(const Point<T>& p) {
	return Point(x + p.x, y + p.y);
}

template <typename T>
Point<T>& Point<T>::operator-(const Point<T>& p) {
	return Point(x - p.x, y - p.y);
}

template <typename T>
Point<T>& Point<T>::operator*(const Point<T>& p) {
	return Point(x * p.x, y * p.y);
}

template <typename T>
Point<T>& Point<T>::operator/(const Point<T>& p) {
	return Point(x / p.x, y / p.y);
}
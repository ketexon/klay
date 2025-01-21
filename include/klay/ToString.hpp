#pragma once

#include <klay/Geometry.hpp>

#include <string>
#include <iostream>

std::ostream& operator<<(std::ostream& os, const Klay::Unit& v);
std::ostream& operator<<(std::ostream& os, const Klay::Px& v);
std::ostream& operator<<(std::ostream& os, const Klay::Percent& v);

template<class T>
std::ostream& operator<<(std::ostream& os, const Klay::Segment<T>& v) {
	return os << "Segment { Start: " << v.start << ", Length:  " << v.length << " }";
}

template<class T>
std::ostream& operator<<(std::ostream& os, const Klay::Rect<T>& v) {
	return os << "Rect { " << v.Horizontal() << ", " << v.Vertical() << " }";
}
template<class T>
std::ostream& operator<<(std::ostream& os, const Klay::Vector2<T>& v) {
	return os << "Vector2 { " << v.Horizontal() << ", " << v.Vertical() << " }";
}
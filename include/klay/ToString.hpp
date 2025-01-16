#pragma once

#include <klay/Geometry.hpp>

#include <string>
#include <iostream>

namespace Klay {
	std::string to_string(const Unit&);
	std::string to_string(const Px&);
	std::string to_string(const Segment<Px>&);
	std::string to_string(const PxRect&);
}

std::ostream& operator<<(std::ostream& os, const Klay::Unit& v);
std::ostream& operator<<(std::ostream& os, const Klay::Px& v);
std::ostream& operator<<(std::ostream& os, const Klay::Segment<Klay::Px>& v);
std::ostream& operator<<(std::ostream& os, const Klay::PxRect& v);
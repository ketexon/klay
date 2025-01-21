#include <klay/Klay.hpp>
#include <klay/ToString.hpp>

std::ostream& operator<<(std::ostream& os, const Klay::Unit& unit) {
	unit.Visit([&](auto&& arg) -> void {
		os << arg;
	});
	return os;
}

std::ostream& operator<<(std::ostream& os, const Klay::Px& px) {
	return os << px.value << "px";
}

std::ostream& operator<<(std::ostream& os, const Klay::Percent& percent) {
	return os << percent.value << "%";
}
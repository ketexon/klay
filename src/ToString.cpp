#include <klay/Klay.hpp>

std::string Klay::to_string(const Unit& unit) {
	return std::visit([](auto&& arg) -> std::string {
		using T = std::decay_t<decltype(arg)>;
		if constexpr(std::is_same_v<T, Px>){
			return Klay::to_string(arg);
		}
	}, unit);
}

std::string Klay::to_string(const Px& unit) {
	return std::to_string(unit.Value) + "px";
}

std::string Klay::to_string(const Segment<Px>& segment) {
	return "Segment { " + to_string(segment.Start) + ", " + to_string(segment.Length) + " }";
}

std::string Klay::to_string(const PxRect& rect) {
	return "PxRect { " + to_string(rect.Horizontal()) + ", " + to_string(rect.Vertical()) + " }";
}
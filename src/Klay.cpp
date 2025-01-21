#include <klay/Klay.hpp>
#include <iostream>

Klay::Px Klay::Unit::CalculatePx(const Klay::Rect<Klay::Px>& rect, Klay::Axis axis) const noexcept {
	return std::visit([&rect, axis](auto&& arg) -> Px {
		if constexpr(std::is_same_v<std::decay_t<decltype(arg)>, Px>) {
			return arg;
		} else {
			return Px{rect.GetAxis(axis).length.value * arg.value};
		}
	}, value);
}
#pragma once

#include <variant>

namespace Klay {
	struct Px {
		float Value;

		constexpr Px(float v = 0) : Value(v) {}

		constexpr operator float() const noexcept {
			return Value;
		}

		constexpr bool operator== (const Px& other) const noexcept {
			return Value == other.Value;
		}
		constexpr bool operator== (const float other) const noexcept {
			return Value == other;
		}
		constexpr Px operator+ (const Px& other) const noexcept {
			return Px{Value + other.Value};
		}
		constexpr Px& operator+= (const Px& other) noexcept {
			Value += other.Value;
			return *this;
		}
		constexpr Px operator- (const Px& other) const noexcept {
			return Px{Value - other.Value};
		}
		constexpr Px& operator-= (const Px& other) noexcept {
			Value -= other.Value;
			return *this;
		}
		constexpr Px operator* (const Px& other) const noexcept {
			return Px{Value * other.Value};
		}
		constexpr Px operator* (const float other) const noexcept {
			return Px{Value * other};
		}
		constexpr Px operator* (const unsigned long long other) const noexcept {
			return Px{Value * other};
		}

		constexpr Px& operator*= (const float other) noexcept {
			Value *= other;
			return *this;
		}

		constexpr bool operator< (const Px& other) const noexcept {
			return Value < other.Value;
		}

		constexpr Px& operator= (const float other) noexcept {
			Value = other;
			return *this;
		}
	};

	using Unit = std::variant<Px>;
}
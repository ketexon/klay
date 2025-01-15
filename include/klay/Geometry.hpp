#pragma once

#include <variant>
#include <optional>

namespace Klay {
	enum class Axis {
		Horizontal = 0,
		Vertical = 1,
	};

	constexpr Axis CrossAxis(Axis axis) {
		return axis == Axis::Horizontal ? Axis::Vertical : Axis::Horizontal;
	}

	template<typename T>
	struct Segment {
		T Start;
		T Length;

		constexpr T End() const {
			return Start + Length;
		}

		constexpr bool operator== (const Segment& other) const noexcept {
			return Start == other.Start && Length == other.Length;
		}
	};
	struct Px {

		float Value;

		constexpr Px(float v = 0) : Value(v) {}

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

		constexpr bool operator< (const Px& other) const noexcept {
			return Value < other.Value;
		}

		constexpr Px& operator= (const float other) noexcept {
			Value = other;
			return *this;
		}
	};

	using Unit = std::variant<Px>;

	template<typename T>
	struct Range {
		T Min;
		T Value;
		T Max;
	};

	template<typename T>
	struct Vector2 {
		T Axes[2];

		constexpr T& GetAxis(Axis axis) {
			return Axes[static_cast<size_t>(axis)];
		}

		constexpr const T& GetAxis(Axis axis) const {
			return Axes[static_cast<size_t>(axis)];
		}

		constexpr T& Horizontal() {
			return GetAxis(Axis::Horizontal);
		}

		constexpr const T& Horizontal() const {
			return GetAxis(Axis::Horizontal);
		}

		constexpr T& Vertical() {
			return GetAxis(Axis::Vertical);
		}

		constexpr const T& Vertical() const {
			return GetAxis(Axis::Vertical);
		}

		constexpr Vector2<T> operator +(const Vector2<T>& other) const noexcept {
			return Vector2<T>{
				Axes[0] + other.Axes[0],
				Axes[1] + other.Axes[1]
			};
		}

		constexpr Vector2<T>& operator +=(const Vector2<T>& other) noexcept {
			Axes[0] += other.Axes[0];
			Axes[1] += other.Axes[1];
			return *this;
		}

		constexpr bool operator ==(const Vector2<T>& other) const noexcept {
			return Axes[0] == other.Axes[0] && Axes[1] == other.Axes[1];
		}
	};

	using PxSize = Vector2<Px>;
	using Size = Vector2<Unit>;
	using OptionalSize = Vector2<std::optional<Unit>>;

	using PxPoint = Vector2<Px>;

	using PxRange = Range<Px>;
	using UnitRange = Range<Unit>;
	using PxSizeRange = Range<PxSize>;
	using SizeRange = Range<Size>;
	using OptionalSizeRange = Range<OptionalSize>;

	template<typename T>
	struct Rect : public Vector2<Segment<T>> {
		static Rect FromLTRB(T left, T top, T right, T bottom) {
			return Rect{
				Segment{left, right - left},
				Segment{top, bottom - top}
			};
		}

		static Rect FromXYWH(T x, T y, T width, T height) {
			return Rect{
				Segment{x, width},
				Segment{y, height}
			};
		}
	};

	using PxRect = Rect<Px>;

	std::string to_string(const Unit&);
	std::string to_string(const Px&);
	std::string to_string(const Segment<Px>&);
	std::string to_string(const PxRect&);
}
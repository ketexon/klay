#pragma once

#include <variant>
#include <optional>

#include <klay/Unit.hpp>

namespace Klay {
	enum class Axis {
		Horizontal = 0,
		Vertical = 1,
	};

	enum class Edge {
		Start = 0,
		End = 1,
	};

	constexpr Axis CrossAxis(Axis axis) {
		return axis == Axis::Horizontal ? Axis::Vertical : Axis::Horizontal;
	}

	/// @brief Represents a length on 2 sides of a segment
	/// @tparam T
	template<typename T>
	struct EdgeLength {
		T Edges[2];

		constexpr T& GetEdge(Edge edge) {
			return Edges[static_cast<size_t>(edge)];
		}

		constexpr const T& GetEdge(Edge edge) const {
			return Edges[static_cast<size_t>(edge)];
		}

		constexpr T& Start() {
			return GetEdge(Edge::Start);
		}

		constexpr const T& Start() const {
			return GetEdge(Edge::Start);
		}

		constexpr T& End() {
			return GetEdge(Edge::End);
		}

		constexpr const T& End() const {
			return GetEdge(Edge::End);
		}
	};

	/// @brief Represents a range of values from a start to an end
	/// @tparam T
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

		constexpr Segment operator+ (const EdgeLength<T>& other) const noexcept {
			return Segment{
				Start + other.Start(),
				Length - other.Start() - other.End()
			};
		}
	};

	template<typename T>
	struct Range {
		T Min;
		T Value;
		T Max;
	};

	/// @brief Represents a value along 2 axes
	/// @tparam T
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

		template<typename U>
		constexpr Vector2<T> operator +(const Vector2<U>& other) const noexcept {
			return Vector2<T>{
				Axes[0] + other.Axes[0],
				Axes[1] + other.Axes[1]
			};
		}

		template<typename U>
		constexpr Vector2<T>& operator +=(const Vector2<U>& other) noexcept {
			Axes[0] += other.Axes[0];
			Axes[1] += other.Axes[1];
			return *this;
		}

		constexpr bool operator ==(const Vector2<T>& other) const noexcept {
			return Axes[0] == other.Axes[0] && Axes[1] == other.Axes[1];
		}
	};
	template<typename T>
	using Point = Vector2<T>;
	using PxPoint = Point<Px>;

	template<class T>
	struct EdgeArea : public Vector2<EdgeLength<T>> {};
	using PxEdgeArea = EdgeArea<Px>;

	/// @brief Same as Vector2<Segment<T>>
	/// @tparam T
	template<typename T>
	struct Rect : public Vector2<Segment<T>> {
		static constexpr Rect FromXYWH(T x, T y, T width, T height) {
			return Rect{
				Segment{x, width},
				Segment{y, height}
			};
		}

		static constexpr Rect FromPointSize(Vector2<T> point, Vector2<T> size){
			return FromXYWH(
				point.Horizontal(),
				point.Vertical(),
				size.Horizontal(),
				size.Vertical()
			);
		}

		static constexpr Rect FromWH(T width, T height) {
			return FromXYWH(0, 0, width, height);
		}

		static constexpr Rect FromLTRB(T left, T top, T right, T bottom) {
			return FromXYWH(left, top, right - left, bottom - top);
		}

		constexpr T X() const {
			return this->GetAxis(Axis::Horizontal).Start;
		}

		constexpr T Y() const {
			return this->GetAxis(Axis::Vertical).Start;
		}

		constexpr T Width() const {
			return this->GetAxis(Axis::Horizontal).Length;
		}

		constexpr T Height() const {
			return this->GetAxis(Axis::Vertical).Length;
		}
	};

	using PxSize = Vector2<Px>;
	using Size = Vector2<Unit>;
	using OptionalSize = Vector2<std::optional<Unit>>;

	using PxRange = Range<Px>;
	using UnitRange = Range<Unit>;
	using PxSizeRange = Range<PxSize>;
	using SizeRange = Range<Size>;
	using OptionalSizeRange = Range<OptionalSize>;

	using PxRect = Rect<Px>;
}

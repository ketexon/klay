#pragma once

#include <variant>

#define KLAY_DEFINE_BINOP_FOR_UNIT(T, Value, Underlying, ArgT, argv, Op)\
	constexpr T operator Op (ArgT) const noexcept { return value Op argv; }\

#define KLAY_DEFINE_ASSIGNOP_FOR_UNIT(T, Value, Underlying, ArgT, argv, AssignOp)\
	constexpr T& operator AssignOp (ArgT) noexcept { value AssignOp argv; return *this; }\

#define KLAY_DEFINE_OPERATOR_FOR_UNIT(T, Value, Underlying, ArgT, argv, Op, AssignOp)\
	KLAY_DEFINE_BINOP_FOR_UNIT(T, value, Underlying, ArgT, argv, Op)\
	KLAY_DEFINE_ASSIGNOP_FOR_UNIT(T, value, Underlying, ArgT, argv, AssignOp)\


// NOTE: the way this works is that
// all operators are implicitly defined by conversions to and from Underlying
// and the assignment operators are defined using those implicit operators
#define KLAY_DEFINE_OPERATORS_FOR_UNIT(T, Value, Underlying) \
	constexpr T() : value{0} {}\
	constexpr T(Underlying v) : value{v} {}\
	constexpr operator Underlying() const noexcept { return value; }\
	KLAY_DEFINE_ASSIGNOP_FOR_UNIT(T, value, Underlying, Underlying v, v, =)\
	KLAY_DEFINE_ASSIGNOP_FOR_UNIT(T, value, Underlying, Underlying v, v, +=)\
	KLAY_DEFINE_ASSIGNOP_FOR_UNIT(T, value, Underlying, Underlying v, v, -=)\
	KLAY_DEFINE_ASSIGNOP_FOR_UNIT(T, value, Underlying, Underlying v, v, *=)\
	KLAY_DEFINE_ASSIGNOP_FOR_UNIT(T, value, Underlying, Underlying v, v, /=)\

namespace Klay {
	template<typename T>
	struct Rect;

	enum class Axis;

	struct Px {
		float value;
		KLAY_DEFINE_OPERATORS_FOR_UNIT(Px, value, float)
	};

	struct Percent {
		float value;
		KLAY_DEFINE_OPERATORS_FOR_UNIT(Percent, value, float)
	};

	struct Unit {
		std::variant<Px, Percent> value;

		constexpr Unit() : value{Px{0}} {}
		constexpr Unit(Px v) : value{v} {}
		constexpr Unit(Percent v) : value{v} {}

		Px CalculatePx(const Rect<Px>& rect, Axis axis) const noexcept;

		template<typename T>
		constexpr bool Is() const noexcept {
			return std::holds_alternative<T>(value);
		}

		template<typename T>
		constexpr const T& Get() const noexcept(false) {
			return std::get<T>(value);
		}

		template<typename T>
		constexpr T& Get() noexcept(false) {
			return std::get<T>(value);
		}

		template<typename T>
		constexpr std::optional<T> TryGet() const noexcept {
			auto v = std::get_if<T>(&value);
			return v ? std::optional{*v} : std::nullopt;
		}

		template<typename F>
		constexpr auto Visit(F&& f) const {
			return std::visit(f, value);
		}
	};
}
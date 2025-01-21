#pragma once

#include <klay/Layout.hpp>
#include <klay/Geometry.hpp>

namespace Klay {
	struct FlexLayoutMode : LayoutMode {
		Axis main_axis;

		constexpr FlexLayoutMode(Axis mainAxis = Axis::Horizontal) noexcept
			: main_axis{mainAxis}
		{}

		void ComputeLayout(
			std::shared_ptr<Element> el,
			const PxRect& content_rect
		) noexcept override;
	};
}
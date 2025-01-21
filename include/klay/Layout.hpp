#pragma once

#include <memory>
#include <klay/Geometry.hpp>

namespace Klay {
	struct LayoutMode;
	struct Element;

	enum class Align {
		Start,
		Center,
		End,
		Stretch,
	};

	enum class Justify {
		Start,
		Center,
		End,
		SpaceBetween,
		SpaceAround,
		SpaceEvenly,
	};

	// https://i.sstatic.net/yedYz.png
	struct LayoutOptions {
		Unit main_gap = Px{0};
		Unit cross_gap = Px{0};
		EdgeArea<Unit> padding;

		Justify justify_content = Justify::Start;

		Align align_items = Align::Start;
		Justify justify_items = Justify::Start;

		int num_rows = 0;
		int num_columns = 0;
	};

	struct LayoutMode {
		virtual ~LayoutMode() = default;
		virtual void ComputeLayout(
			std::shared_ptr<Element> el,
			const PxRect& content_rect
		) noexcept = 0;
	};
}
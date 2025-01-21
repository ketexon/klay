#include <klay/Element.hpp>
#include <klay/Flex.hpp>

#include <iostream>

void Klay::Element::ComputeLayout(const Klay::PxRect& parentRect) noexcept {
	if(!layout_mode) return;

	auto content_rect = parentRect + layout_options.padding.Transform(
		[&](const EdgeLength<Unit>& edgeLength, Axis axis) -> EdgeLength<Px> {
			return edgeLength.Transform([&](const Unit& unit, Edge edge) -> Px {
				return unit.CalculatePx(parentRect, axis);
			});
		}
	);
	for(auto& child : children) {
		if(child->dirty_size) {
			child->ComputeMinSize();
		}
	}
	layout_mode->ComputeLayout(shared_from_this(), content_rect);
	dirty_layout = false;
}

void Klay::Element::ComputeMinSize() noexcept {
	if(!dirty_size) {
		return;
	}
	dirty_layout = true;
	dirty_size = false;

	PxSize computed {0, 0};
	for(auto& child : children) {
		if(child->dirty_size) {
			child->ComputeMinSize();
		}
		computed += child->computed_min_size;
	}

	computed += layout_options.padding.Transform(
		[&](const EdgeLength<Unit>& edgeLength, Axis axis) -> Px {
			return edgeLength.Start().TryGet<Px>().value_or(Px{0})
				+ edgeLength.End().TryGet<Px>().value_or(Px{0});
		}
	);

	for(int i = 0; i < 2; ++i){
		auto minSize = size.min.axes[i].value_or(Px{0});
		if(minSize.Is<Px>()) {
			computed.axes[i] = std::max(computed.axes[i], minSize.Get<Px>());
		}
	}

	computed_min_size = computed;
}

void Klay::Element::AssignDefaultLayoutMode() noexcept {
	layout_mode = std::make_unique<FlexLayoutMode>();
}
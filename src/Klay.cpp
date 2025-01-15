#include <klay/Klay.hpp>
#include <iostream>

void Klay::Element::ComputeLayout(const Klay::PxRect& parentRect) noexcept {
	for(auto& child : Children) {
		if(child->DirtySize) {
			child->ComputeMinSize();
		}
		if(child->DirtyLayout) {
			child->ComputeLayout(parentRect);
		}
	}
	DirtyLayout = false;

	if(std::holds_alternative<FlexLayout>(Layout)) {
		ComputeFlexLayout(parentRect);
	}
}

void Klay::Element::ComputeFlexLayout(const Klay::PxRect& parentRect) noexcept {
	const auto& flexLayout = std::get<FlexLayout>(Layout);
	Axis axis = flexLayout.MainAxis;
	Axis crossAxis = CrossAxis(axis);
	Px mainAxisSize;
	float totalGrow = 0;
	for(auto& child : Children) {
		mainAxisSize += child->ComputedMinSize.GetAxis(axis);
		if(auto layoutItem = std::get_if<FlexLayoutItem>(&child->LayoutItem)){
			totalGrow += layoutItem->Grow;
		}
	}

	Px remainingSize = parentRect.GetAxis(axis).Length - mainAxisSize;
	Px curChildOffset;
	for(auto& child : Children) {
		if(auto layoutItem = std::get_if<FlexLayoutItem>(&child->LayoutItem)){
			float grow = layoutItem->Grow;
			float growPx = remainingSize.Value * grow / totalGrow;

			float lengthPx = child->ComputedMinSize.GetAxis(axis).Value + growPx;

			PxRect rect = parentRect;

			rect.GetAxis(axis).Start += curChildOffset;
			rect.GetAxis(axis).Length = lengthPx;

			rect.GetAxis(crossAxis).Length = child->ComputedMinSize.GetAxis(crossAxis).Value;

			child->ComputedRect = rect;

			curChildOffset += lengthPx;
		}
	}
}

void Klay::Element::ComputeMinSize() noexcept {
	if(!DirtySize) {
		return;
	}
	DirtyLayout = true;
	DirtySize = false;

	PxSize computed {0, 0};
	for(auto& child : Children) {
		if(child->DirtySize) {
			child->ComputeMinSize();
		}
		computed += child->ComputedMinSize;
	}

	for(int i = 0; i < 2; ++i){
		auto min = Size.Min.Axes[i].value_or(Px{0});
		if(std::holds_alternative<Px>(min)) {
			computed.Axes[i] = std::max(computed.Axes[i], std::get<Px>(min));
		}
	}

	ComputedMinSize = computed;
}
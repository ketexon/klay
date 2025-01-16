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

	if(std::holds_alternative<FlexLayout>(Layout.Mode)) {
		ComputeFlexLayout(parentRect);
	}
}

void Klay::Element::ComputeFlexLayout(const Klay::PxRect& parentRect) noexcept {
	const auto& flexLayout = std::get<FlexLayout>(Layout.Mode);
	const auto& layoutOptions = Layout.Options;

	auto contentRect = parentRect + layoutOptions.Padding;

	Axis mainAxis = flexLayout.MainAxis;
	Axis crossAxis = CrossAxis(mainAxis);
	Px mainAxisSize;
	float totalGrow = 0;

	// calculate total flex grow
	// and minimum size along the main axis
	for(auto& child : Children) {
		mainAxisSize += child->ComputedMinSize.GetAxis(mainAxis);
		if(auto layoutItem = std::get_if<FlexLayoutItem>(&child->Item.Mode)){
			totalGrow += layoutItem->Grow;
		}
	}

	Px mainAxisGap = std::visit([&](auto&& arg) -> Px {
		if constexpr(std::is_same_v<std::decay_t<decltype(arg)>, Px>){
			return arg;
		}
		else {
			static_assert(false);
		}
	}, layoutOptions.MainGap);

	Px extraSpace = contentRect.GetAxis(mainAxis).Length
		- mainAxisSize
		- mainAxisGap * (NumChildren() - 1);

	Px remainingSpace = extraSpace;

	// apply flex grow and, if align items or align self is stretch,
	// stretch item across cross axis
	for(auto& child : Children) {
		const auto& itemOptions = child->Item.Options;
		float grow = 0;
		float lengthPx = child->ComputedMinSize.GetAxis(mainAxis).Value;
		if(auto layoutItem = std::get_if<FlexLayoutItem>(&child->Item.Mode)){
			float grow = layoutItem->Grow;
			if(grow > 0){
				float growPx = extraSpace.Value * grow / totalGrow;
				lengthPx += growPx;
				remainingSpace -= growPx;
			}
		}
		child->ComputedSize.GetAxis(mainAxis) = lengthPx;

		auto align = itemOptions.AlignSelf.value_or(layoutOptions.AlignItems);
		if(align == Align::Stretch){
			child->ComputedSize.GetAxis(crossAxis) = contentRect.GetAxis(crossAxis).Length;
		}
		// no stretch
		else {
			child->ComputedSize.GetAxis(crossAxis) = child->ComputedMinSize.GetAxis(crossAxis);
		}
	}

	Px mainAxisOffset;
	// justify content doesn't make sense if there is no extra space
	if(totalGrow == 0){
		switch(layoutOptions.JustifyContent) {
			case Justify::End:
				mainAxisOffset = remainingSpace;
				break;
			case Justify::Center:
				mainAxisOffset = remainingSpace / 2;
				break;
			case Justify::SpaceAround: {
				// split remaining space between the leftOffset, gap, and rightOffset
				Px split = remainingSpace / NumChildren();
				mainAxisOffset = split / 2;
				mainAxisGap += split;
				break;
			}
			case Justify::SpaceEvenly: {
				Px split = remainingSpace / (NumChildren() + 1);
				mainAxisOffset = split;
				mainAxisGap += split;
				break;
			}
			case Justify::SpaceBetween:
				mainAxisGap += remainingSpace / (NumChildren() - 1);
				break;
		}
	}

	// compute position
	for(auto& child : Children) {
		child->ComputedPosition.GetAxis(mainAxis) = contentRect.GetAxis(mainAxis).Start + mainAxisOffset;
		mainAxisOffset += child->ComputedSize.Horizontal() + mainAxisGap;

		auto crossAxisExtraSpace = contentRect.GetAxis(crossAxis).Length - child->ComputedSize.GetAxis(crossAxis);
		auto itemOptions = child->Item.Options;
		auto align = itemOptions.AlignSelf.value_or(layoutOptions.AlignItems);

		Px crossAxisOffset;
		switch(align) {
			case Align::Start:
			case Align::Stretch:
				crossAxisOffset = 0;
				break;
			case Align::End:
				crossAxisOffset = crossAxisExtraSpace;
				break;
			case Align::Center:
				crossAxisOffset = crossAxisExtraSpace / 2;
				break;
		}
		child->ComputedPosition.GetAxis(crossAxis) = contentRect.GetAxis(crossAxis).Start + crossAxisOffset;
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
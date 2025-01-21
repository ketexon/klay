#include <klay/Flex.hpp>
#include <klay/Element.hpp>

void Klay::FlexLayoutMode::ComputeLayout(
	std::shared_ptr<Element> el,
	const Klay::PxRect& contentRect
) noexcept {
	const auto& layout_options = el->layout_options;
	const auto& children = el->children;

	Axis cross_axis = CrossAxis(main_axis);
	Px main_axis_size;
	float total_grow = 0;

	// calculate total flex grow
	// and minimum size along the main axis
	for(const auto& child : children) {
		main_axis_size += child->computed_min_size.GetAxis(main_axis);
		total_grow += child->item_options.grow;
	}

	Px main_axis_gap = layout_options.main_gap.CalculatePx(
		contentRect,
		main_axis
	);

	Px extra_space = contentRect.GetAxis(main_axis).length
		- main_axis_size
		- main_axis_gap * (el->NumChildren() - 1);

	Px remaining_space = extra_space;

	// apply flex grow and, if align items or align self is stretch,
	// stretch item across cross axis
	for(const auto& child : children) {
		const auto& item_options = child->item_options;

		float length_px = child->computed_min_size.GetAxis(main_axis).value;
		float grow = item_options.grow;
		if(grow > 0){
			float grow_px = extra_space.value * grow / total_grow;
			length_px += grow_px;
			remaining_space -= grow_px;
		}
		child->computed_size.GetAxis(main_axis) = length_px;

		auto align = item_options.align_self.value_or(layout_options.align_items);
		if(align == Align::Stretch){
			child->computed_size.GetAxis(cross_axis) = contentRect.GetAxis(cross_axis).length;
		}
		// no stretch
		else {
			child->computed_size.GetAxis(cross_axis) = child->computed_min_size.GetAxis(cross_axis);
		}
	}

	Px main_axis_offset;
	// justify content doesn't make sense if there is no extra space
	if(total_grow == 0){
		switch(layout_options.justify_content) {
			case Justify::End:
				main_axis_offset = remaining_space;
				break;
			case Justify::Center:
				main_axis_offset = remaining_space / 2;
				break;
			case Justify::SpaceAround: {
				// split remaining space between the leftOffset, gap, and rightOffset
				Px split = remaining_space / el->NumChildren();
				main_axis_offset = split / 2;
				main_axis_gap += split;
				break;
			}
			case Justify::SpaceEvenly: {
				Px split = remaining_space / (el->NumChildren() + 1);
				main_axis_offset = split;
				main_axis_gap += split;
				break;
			}
			case Justify::SpaceBetween:
				main_axis_gap += remaining_space / (el->NumChildren() - 1);
				break;
		}
	}

	// compute position
	for(auto& child : children) {
		const auto& item_options = child->item_options;

		child->computed_position.GetAxis(main_axis) = (
			contentRect.GetAxis(main_axis).start + main_axis_offset
		);
		main_axis_offset += child->computed_size.GetAxis(main_axis) + main_axis_gap;

		auto cross_axis_extra_space = (
			contentRect.GetAxis(cross_axis).length
			- child->computed_size.GetAxis(cross_axis)
		);
		auto align = item_options.align_self.value_or(
			layout_options.align_items
		);

		Px cross_axis_offset;
		switch(align) {
			case Align::Start:
			case Align::Stretch:
				cross_axis_offset = 0;
				break;
			case Align::End:
				cross_axis_offset = cross_axis_extra_space;
				break;
			case Align::Center:
				cross_axis_offset = cross_axis_extra_space / 2;
				break;
		}
		child->computed_position.GetAxis(cross_axis) = (
			contentRect.GetAxis(cross_axis).start + cross_axis_offset
		);
	}
}
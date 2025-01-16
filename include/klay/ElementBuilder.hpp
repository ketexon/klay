#pragma once

#include <klay/Element.hpp>

namespace Klay {
	class ElementBuilder {
		Element element;

	public:
		constexpr ElementBuilder& Flex(Axis axis = Axis::Horizontal) {
			element.Layout.Mode = FlexLayout { axis };
			return *this;
		}

		constexpr ElementBuilder& JustifyContent(Justify justify) {
			element.Layout.Options.JustifyContent = justify;
			return *this;
		}

		constexpr ElementBuilder& AlignItems(Align align) {
			element.Layout.Options.AlignItems = align;
			return *this;
		}

		constexpr ElementBuilder& AlignSelf(Align align) {
			element.Item.Options.AlignSelf = align;
			return *this;
		}

		constexpr ElementBuilder& Gap(Unit gap) {
			element.Layout.Options.MainGap = gap;
			element.Layout.Options.CrossGap = gap;
			return *this;
		}

		constexpr ElementBuilder& MinWidth(Unit min) {
			element.Size.Min.Horizontal() = min;
			return *this;
		}

		constexpr ElementBuilder& MinHeight(Unit min) {
			element.Size.Min.Vertical() = min;
			return *this;
		}

		constexpr ElementBuilder& MinSize(Unit w, Unit h){
			element.Size.Min = {w, h};
			return *this;
		}

		constexpr ElementBuilder& FlexItem() {
			if(!std::holds_alternative<FlexLayoutItem>(element.Item.Mode)) {
				element.Item.Mode = FlexLayoutItem {};
			}
			return *this;
		}

		constexpr ElementBuilder& FlexGrow(float grow) {
			FlexItem();
			std::get<FlexLayoutItem>(element.Item.Mode).Grow = grow;
			return *this;
		}

		constexpr ElementBuilder& FlexShrink(float shrink) {
			FlexItem();
			std::get<FlexLayoutItem>(element.Item.Mode).Shrink = shrink;
			return *this;
		}

		constexpr ElementBuilder& Padding(PxEdgeArea padding) {
			element.Layout.Options.Padding = padding;
			return *this;
		}

		constexpr ElementBuilder& PaddingLTRB(Px l, Px t, Px r, Px b) {
			PaddingLeft(l);
			PaddingTop(t);
			PaddingRight(r);
			PaddingBottom(b);
			return *this;
		}

		constexpr ElementBuilder& PaddingLeft(Px l) {
			element.Layout.Options.Padding.Horizontal().Start() = l;
			return *this;
		}

		constexpr ElementBuilder& PaddingRight(Px r) {
			element.Layout.Options.Padding.Horizontal().End() = r;
			return *this;
		}

		constexpr ElementBuilder& PaddingTop(Px t) {
			element.Layout.Options.Padding.Vertical().Start() = t;
			return *this;
		}

		constexpr ElementBuilder& PaddingBottom(Px b) {
			element.Layout.Options.Padding.Vertical().End() = b;
			return *this;
		}

		std::shared_ptr<Element> Build() {
			return std::make_shared<Element>(std::move(element));
		}
	};
}
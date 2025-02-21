#pragma once

#include <klay/Element.hpp>
#include <klay/Flex.hpp>
#include <klay/Grid.hpp>
#include <any>
#include <iostream>

namespace Klay {
	class ElementBuilder {
		Element element;

	public:
		inline ElementBuilder& Flex(Axis axis = Axis::Horizontal) {
			element.layout_mode = std::make_unique<FlexLayoutMode>(axis);
			return *this;
		}

		inline ElementBuilder& Grid(int rows, int cols) {
			element.layout_mode = std::make_unique<GridLayoutMode>();
			return NumRows(rows).NumColumns(cols);
		}

		inline ElementBuilder& LayoutMode(std::unique_ptr<LayoutMode> mode) {
			element.layout_mode = std::move(mode);
			return *this;
		}

		inline ElementBuilder& NumRows(int rows) {
			element.layout_options.num_rows = rows;
			return *this;
		}

		inline ElementBuilder& NumColumns(int cols) {
			element.layout_options.num_columns = cols;
			return *this;
		}

		constexpr ElementBuilder& Row(int start) {
			element.item_options.row_start = start;
			return *this;
		}

		constexpr ElementBuilder& Row(int start, int span) {
			element.item_options.row_start = start;
			element.item_options.row_span = span;
			return *this;
		}

		constexpr ElementBuilder& Col(int start) {
			element.item_options.col_start = start;
			return *this;
		}

		constexpr ElementBuilder& Col(int start, int span) {
			element.item_options.col_start = start;
			element.item_options.col_span = span;
			return *this;
		}

		constexpr ElementBuilder& RowSpan(int span) {
			element.item_options.row_span = span;
			return *this;
		}

		constexpr ElementBuilder& ColSpan(int span) {
			element.item_options.col_span = span;
			return *this;
		}

		constexpr ElementBuilder& JustifyContent(Justify justify) {
			element.layout_options.justify_content = justify;
			return *this;
		}

		constexpr ElementBuilder& AlignItems(Align align) {
			element.layout_options.align_items = align;
			return *this;
		}

		constexpr ElementBuilder& AlignSelf(Align align) {
			element.item_options.align_self = align;
			return *this;
		}

		constexpr ElementBuilder& Gap(Unit gap) {
			element.layout_options.main_gap = gap;
			element.layout_options.cross_gap = gap;
			return *this;
		}

		constexpr ElementBuilder& Gap(Unit main, Unit cross) {
			element.layout_options.main_gap = main;
			element.layout_options.cross_gap = cross;
			return *this;
		}

		constexpr ElementBuilder& MainGap(Unit gap) {
			element.layout_options.main_gap = gap;
			return *this;
		}

		constexpr ElementBuilder& CrossGap(Unit gap) {
			element.layout_options.cross_gap = gap;
			return *this;
		}

		constexpr ElementBuilder& MinWidth(Unit min) {
			element.size.min.Horizontal() = min;
			return *this;
		}

		constexpr ElementBuilder& MinHeight(Unit min) {
			element.size.min.Vertical() = min;
			return *this;
		}

		constexpr ElementBuilder& MinSize(Unit w, Unit h){
			element.size.min = {w, h};
			return *this;
		}

		constexpr ElementBuilder& FlexGrow(float grow) {
			element.item_options.grow = grow;
			return *this;
		}

		constexpr ElementBuilder& FlexShrink(float shrink) {
			element.item_options.shrink = shrink;
			return *this;
		}

		constexpr ElementBuilder& PaddingPxLTRB(Px l, Px t, Px r, Px b) {
			PaddingLeft(l);
			PaddingTop(t);
			PaddingRight(r);
			PaddingBottom(b);
			return *this;
		}

		constexpr ElementBuilder& PaddingPercentLTRB(Percent l, Percent t, Percent r, Percent b) {
			PaddingLeft(l);
			PaddingTop(t);
			PaddingRight(r);
			PaddingBottom(b);
			return *this;
		}

		constexpr ElementBuilder& PaddingLeft(Unit l) {
			element.layout_options.padding.Horizontal().Start() = l;
			return *this;
		}

		constexpr ElementBuilder& PaddingRight(Unit r) {
			element.layout_options.padding.Horizontal().End() = r;
			return *this;
		}

		constexpr ElementBuilder& PaddingTop(Unit t) {
			element.layout_options.padding.Vertical().Start() = t;
			return *this;
		}

		constexpr ElementBuilder& PaddingBottom(Unit b) {
			element.layout_options.padding.Vertical().End() = b;
			return *this;
		}

		constexpr ElementBuilder& ID(Element::IDType id) {
			element.id = id;
			return *this;
		}

		constexpr ElementBuilder& UserData(auto data) {
			element.user_data = std::move(data);
			return *this;
		}

		std::shared_ptr<Element> Build() {
			return std::make_shared<Element>(std::move(element));
		}
	};
}

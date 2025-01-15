#pragma once

#include <optional>
#include <variant>
#include <vector>
#include <string>
#include <memory>

#include <klay/Geometry.hpp>

namespace Klay {
	enum class Align {
		Start,
		Center,
		End,
		Stretch,
	};

	struct FlexLayout {
		Axis MainAxis = Axis::Horizontal;
		Align MainAlign = Align::Start;
	};

	struct LayoutItem {

	};

	struct FlexLayoutItem : LayoutItem {
		std::optional<Unit> Basis;
		float Grow = 0;
		float Shrink = 0;
	};

	struct Element : public std::enable_shared_from_this<Element> {
		bool DirtySize = true;
		bool DirtyLayout = true;

		OptionalSizeRange Size;
		std::variant<FlexLayout> Layout;
		std::variant<FlexLayoutItem> LayoutItem;

		std::weak_ptr<Element> Parent;
		std::vector<std::shared_ptr<Element>> Children;

		PxSize ComputedMinSize;
		PxRect ComputedRect;

		constexpr Element()
			: Size{}
			, Layout{FlexLayout{Axis::Horizontal, Align::Start}}
		{}

		void ComputeLayout(const PxRect& parentRect) noexcept;

		std::shared_ptr<Element> AddChild(std::shared_ptr<Element> child) {
			Children.push_back(child);
			child->Reparent(weak_from_this());
			return child;
		}

		void ComputeMinSize() noexcept;

		void Reparent(std::weak_ptr<Element> parent) noexcept {
			Parent = parent;
		}

	private:
		void ComputeFlexLayout(const PxRect& parentRect) noexcept;
	};

	class ElementBuilder {
		Element element;

	public:
		constexpr ElementBuilder& Flex(Axis axis = Axis::Horizontal) {
			element.Layout = FlexLayout { axis, Align::Start };
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
			if(!std::holds_alternative<FlexLayoutItem>(element.LayoutItem)) {
				element.LayoutItem = FlexLayoutItem {};
			}
			return *this;
		}

		constexpr ElementBuilder& FlexGrow(float grow) {
			FlexItem();
			std::get<FlexLayoutItem>(element.LayoutItem).Grow = grow;
			return *this;
		}

		constexpr ElementBuilder& FlexShrink(float shrink) {
			FlexItem();
			std::get<FlexLayoutItem>(element.LayoutItem).Shrink = shrink;
			return *this;
		}

		std::shared_ptr<Element> Build() {
			return std::make_shared<Element>(std::move(element));
		}
	};
}
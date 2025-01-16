#pragma once

#include <klay/Geometry.hpp>

#include <vector>
#include <memory>

namespace Klay {
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

	struct FlexLayout {
		Axis MainAxis = Axis::Horizontal;
		Unit Gap;
		Align MainAlign = Align::Start;
	};

	// https://i.sstatic.net/yedYz.png
	struct LayoutOptions {
		Unit MainGap = Px{0};
		Unit CrossGap = Px{0};
		PxEdgeArea Padding;

		Justify JustifyContent = Justify::Start;

		Align AlignItems = Align::Start;
		Justify JustifyItems = Justify::Start;
	};

	struct ItemOptions {
		std::optional<Align> AlignSelf;
		std::optional<Justify> JustifySelf;
	};

	struct FlexLayoutItem : ItemOptions {
		std::optional<Unit> Basis;
		float Grow = 0;
		float Shrink = 0;
	};

	struct Element : public std::enable_shared_from_this<Element> {
		bool DirtySize = true;
		bool DirtyLayout = true;

		OptionalSizeRange Size;
		struct {
			std::variant<FlexLayout> Mode;
			LayoutOptions Options;
		} Layout;
		struct {
			std::variant<FlexLayoutItem> Mode;
			ItemOptions Options;
		} Item;

		std::weak_ptr<Element> Parent;
		std::vector<std::shared_ptr<Element>> Children;

		PxSize ComputedMinSize;
		PxSize ComputedSize;
		PxPoint ComputedPosition;

		constexpr PxRect ComputedRect() const noexcept {
			return PxRect::FromPointSize(ComputedPosition, ComputedSize);
		}

		constexpr Element()
			: Size{}
			, Layout{FlexLayout{Axis::Horizontal}}
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

		constexpr size_t NumChildren() const noexcept {
			return Children.size();
		}

	private:
		void ComputeFlexLayout(const PxRect& parentRect) noexcept;
	};
}
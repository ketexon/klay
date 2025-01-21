#pragma once

#include <klay/Geometry.hpp>
#include <klay/Layout.hpp>
#include <unordered_map>
#include <any>

#include <kind/Kind.hpp>

#include <vector>
#include <memory>

namespace Klay {
	// forward
	struct Element;

	struct ItemOptions {
		std::optional<Align> align_self;
		std::optional<Justify> justify_self;
		std::optional<Unit> basis;
		float grow = 0;
		float shrink = 0;

		std::optional<int> row_start;
		int row_span = 1;
		std::optional<int> col_start;
		int col_span = 1;
	};

	struct Element : public std::enable_shared_from_this<Element> {
		using IDType = size_t;

		bool dirty_size = true;
		bool dirty_layout = true;

		OptionalSizeRange size;

		std::weak_ptr<Element> parent;
		std::vector<std::shared_ptr<Element>> children;

		std::optional<IDType> id;

		PxSize computed_min_size;
		PxSize computed_size;
		PxPoint computed_position;

		LayoutOptions layout_options;
		std::unique_ptr<LayoutMode> layout_mode;
		ItemOptions item_options;

		inline PxRect ComputedRect() const noexcept {
			return PxRect::FromPointSize(computed_position, computed_size);
		}

		inline Element() : size{} {}

		void ComputeLayout(const PxRect& parent_rect) noexcept;

		std::shared_ptr<Element> AddChild(std::shared_ptr<Element> child) {

			children.push_back(child);
			child->Reparent(weak_from_this());
			return child;
		}

		void ComputeMinSize() noexcept;

		void Reparent(std::weak_ptr<Element> parent) noexcept {
			this->parent = parent;
		}

		constexpr size_t NumChildren() const noexcept {
			return children.size();
		}

	private:
		void AssignDefaultLayoutMode() noexcept;
	};
}
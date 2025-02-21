#pragma once

#include <klay/Geometry.hpp>
#include <klay/Layout.hpp>

#include <kind/Kind.hpp>

#include <vector>
#include <memory>
#include <any>

#define KLAY_DEFINE_ITERATOR_WRAPPER(member) \
		auto begin() noexcept { return member.begin(); } \
		auto begin() const noexcept { return member.begin(); } \
		auto cbegin() const noexcept { return member.cbegin(); } \
		auto end() noexcept { return member.end(); } \
		auto end() const noexcept { return member.end(); } \
		auto cend() const noexcept { return member.cend(); } \
		auto rbegin() noexcept { return member.rbegin(); } \
		auto rbegin() const noexcept { return member.rbegin(); } \
		auto crbegin() const noexcept { return member.crbegin(); } \
		auto rend() noexcept { return member.rend(); } \
		auto rend() const noexcept { return member.rend(); } \
		auto crend() const noexcept { return member.crend(); }


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

		std::any user_data;

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

		KLAY_DEFINE_ITERATOR_WRAPPER(children)

	private:
		void AssignDefaultLayoutMode() noexcept;
	};
}

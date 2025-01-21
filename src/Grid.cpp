#include <klay/Grid.hpp>

#include <klay/Element.hpp>
#include <vector>
#include <unordered_map>
#include <iostream>

// see https://www.w3.org/TR/css-grid-1/#auto-placement-algo
void Klay::GridLayoutMode::ComputeLayout(
	std::shared_ptr<Element> el,
	const Klay::PxRect& content_rect
) noexcept {
	const auto& layout_options = el->layout_options;
	const auto& children = el->children;

	const auto explicit_rows = layout_options.num_rows;
	const auto explicit_cols = layout_options.num_columns;

	this->explicit_grid_size = Vector2<int>{explicit_rows, explicit_cols};

	// we use char to prevent vector bool optimization
	Grid<char> grid { explicit_rows, explicit_cols, 0 };
	std::unordered_map<
		std::shared_ptr<Element>,
		Vector2<Segment<int>>
	> children_positions;

	auto add_to_grid = [&](
		int row_start, int row_span,
		int col_start, int col_span
	) {
		// resize the grid if necessary
		grid.ensure_size(row_start + row_span, col_start + col_span);
		for (int row = row_start; row < row_start + row_span; ++row) {
			for (int col = col_start; col < col_start + col_span; ++col) {
				grid.get_cell(row, col) = 1;
			}
		}
	};

	// this checks if the grid region is free
	// this does include the implicit grid
	const auto check_grid = [&](int row_start, int row_span, int col_start, int col_span) {
		for (int row = row_start; row < row_start + row_span; ++row) {
			for (int col = col_start; col < col_start + col_span; ++col) {
				if (row >= grid.num_rows || col >= grid.num_cols) {
					break;
				}
				if (grid.get_cell(row, col)) {
					return false;
				}
			}
			if(row >= grid.num_rows){
				break;
			}
		}
		return true;
	};

	std::vector<std::shared_ptr<Element>> non_auto_positioned_children;
	std::vector<std::shared_ptr<Element>> row_locked_children;
	std::vector<std::shared_ptr<Element>> remaining_children;

	// classify each child by its positioning
	for (const auto& child : children) {
		const auto& item_options = child->item_options;

		const auto row_start = item_options.row_start;
		const auto col_start = item_options.col_start;

		if (row_start && col_start){
			non_auto_positioned_children.push_back(child);
		}
		else if (row_start) {
			row_locked_children.push_back(child);
		}
		else {
			remaining_children.push_back(child);
		}
	}

	// 1. Position anything that's not auto-positioned
	// place non-auto-positioned children
	// as they are
	for(const auto& child : non_auto_positioned_children) {
		const auto& item_options = child->item_options;

		const auto row_start = item_options.row_start.value();
		const auto col_start = item_options.col_start.value();
		const auto row_span = item_options.row_span;
		const auto col_span = item_options.col_span;

		add_to_grid(row_start, row_span, col_start, col_span);
		children_positions[child] = Vector2<Segment<int>>{
			{col_start, col_span},
			{row_start, row_span},
		};
	}

	// 2. Process the items locked to a given row
	// dense packing
	// Set the column-start line of its placement to the earliest (smallest
	// positive index) line index that ensures this item’s grid area will not
	// overlap any occupied grid cells.
	for(const auto& child : row_locked_children) {
		const auto& item_options = child->item_options;

		const auto row_start = item_options.row_start.value();
		const auto row_span = item_options.row_span;
		const auto col_span = item_options.col_span;

		int col_start;
		for(col_start = 0; ; ++col_start) {
			if(check_grid(row_start, row_span, col_start, col_span)){
				add_to_grid(row_start, row_span, col_start, col_span);
				children_positions[child] = Vector2<Segment<int>>{
					{col_start, col_span},
					{row_start, row_span},
				};
				break;
			}
		}
	}

	// 3. Position the remaining grid items.
	int current_row = 0;
	int current_col = 0;

	for(const auto& child : remaining_children){
		const auto& item_options = child->item_options;
		// INVARIANT: item_options.row_start == std::nullopt

		const auto col_start = item_options.col_start;
		const auto row_span = item_options.row_span;
		const auto col_span = item_options.col_span;

		// If the item has a definite column position:
		if(col_start) {
			// Set the column position of the cursor to the grid item’s
			// column-start line. If this is less than the previous column
			// position of the cursor, increment the row position by 1.
			if(*col_start < current_col) {
				++current_row;
			}
			current_col = col_start.value();

			// Increment the cursor's row position until a value is found where
			// the grid item does not overlap any occupied grid cells (creating
			// new rows in the implicit grid as necessary).
			for(; ; ++current_row) {
				if(check_grid(current_row, row_span, current_col, col_span)){
					add_to_grid(current_row, row_span, current_col, col_span);
					children_positions[child] = Vector2<Segment<int>>{
						{current_col, col_span},
						{current_row, row_span},
					};
					break;
				}
			}
		}
		// col_start == std::nullopt
		// If the item has an automatic grid position in both axes:
		else {
			// Increment the column position of the auto-placement cursor until
			// either this item’s grid area does not overlap any occupied grid
			// cells, or the cursor’s column position, plus the item’s column
			// span, overflow the number of columns in the implicit grid, as
			// determined earlier in this algorithm.
			bool found = false;
			while(!found){
				for(; current_col + col_span <= grid.num_cols; ++current_col){
					if(check_grid(current_row, row_span, current_col, col_span)){
						add_to_grid(current_row, row_span, current_col, col_span);
						children_positions[child] = Vector2<Segment<int>>{
							{current_col, col_span},
							{current_row, row_span},
						};
						found = true;
						break;
					}
				}
				if(!found){
					++current_row;
					current_col = 0;
				}
			}
		}
	}

	// calculate row and column sizes
	const auto& main_gap = layout_options.main_gap.CalculatePx(
		content_rect,
		Axis::Horizontal
	);
	const auto& cross_gap = layout_options.cross_gap.CalculatePx(
		content_rect,
		Axis::Vertical
	);

	// TODO: sizing for implicit grid
	const auto main_space = (
		content_rect.Width()
		- main_gap * (explicit_cols - 1)
	);

	const auto cross_space = (
		content_rect.Height()
		- cross_gap * (explicit_rows - 1)
	);

	// TODO: sizing functions
	const auto col_width = main_space / explicit_cols;
	const auto row_height = cross_space / explicit_rows;

	// set child positions
	for(const auto& child : children) {
		const auto& pos_it = children_positions.find(child);
		if (pos_it == children_positions.end()) {
			continue;
		}
		const auto& grid_pos = pos_it->second;

		child->computed_size = PxSize{
			(col_width + main_gap) * grid_pos.Horizontal().length - main_gap,
			(row_height + cross_gap) * grid_pos.Vertical().length - cross_gap,
		};
		child->computed_position = PxPoint{
			content_rect.Horizontal().start
			+ (col_width + main_gap) * grid_pos.Horizontal().start,
			content_rect.Vertical().start
			+ (row_height + cross_gap) * grid_pos.Vertical().start,
		};
	}

	// set implicit grid size
	this->implicit_grid_size = Vector2<int>{
		grid.num_rows,
		grid.num_cols,
	};
}
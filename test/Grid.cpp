#include <ktest/KTest.hpp>

#include <klay/Grid.hpp>
#include <klay/ElementBuilder.hpp>
#include <klay/ToString.hpp>

TEST_CASE("Grid Data Structure", GridDataStructure) {
	using namespace Klay;

	int rows = 10;
	int cols = 10;
	Grid<int> grid { rows, cols };

	test.AssertEq(grid.num_rows, rows, "Grid number of rows");
	test.AssertEq(grid.num_cols, cols, "Grid number of rows");
	test.Assert(grid.cap_rows >= grid.num_rows, "Grid capacity rows > num rows");
	test.Assert(grid.cap_cols >= grid.num_cols, "Grid capacity cols > num cols");

	// assign data to grid
	for(int r = 0; r < rows; ++r){
		for(int c = 0; c < cols; ++c){
			grid.get_cell(r, c) = r * 1000 + c;
		}
	}

	// ensure data is correct
	for(int r = 0; r < rows; ++r){
		for(int c = 0; c < cols; ++c){
			test.AssertEq(grid.get_cell(r, c), r * 1000 + c, "Grid cell value");
		}
	}

	int new_rows = grid.cap_rows;
	int new_cols = grid.cap_rows;

	// resize grid to capacity
	int default_value = 99999999;
	grid.resize(new_rows, new_cols, default_value);

	test.AssertEq(grid.num_rows, new_rows, "Grid number of rows after resize");
	test.AssertEq(grid.num_cols, new_cols, "Grid number of rows after resize");
	test.Assert(
		grid.cap_rows >= grid.num_rows,
		"Grid capacity rows > num rows after resize"
	);
	test.Assert(
		grid.cap_cols >= grid.num_cols,
		"Grid capacity cols > num cols after resize"
	);

	// ensure original data is correct after resize
	for(int r = 0; r < new_rows; ++r){
		for(int c = 0; c < new_cols; ++c){
			if(r < rows && c < cols){
				test.AssertEq(
					grid.get_cell(r, c), r * 1000 + c,
					"Grid cell value for original cells after resize"
				);
			}
			else {
				test.AssertEq(
					grid.get_cell(r, c),
					default_value,
					"Grid cell value for new cells after resize"
				);
			}
		}
	}

	// resize paste capacity
	new_rows = grid.cap_rows + 1;
	new_cols = grid.cap_cols + 1;

	grid.resize(new_rows, new_cols, default_value);

	test.AssertEq(grid.num_rows, new_rows, "Grid number of rows after resize past capacity");
	test.AssertEq(grid.num_cols, new_cols, "Grid number of rows after resize past capacity");
	test.Assert(
		grid.cap_rows >= grid.num_rows,
		"Grid capacity rows > num rows after resize past capacity"
	);
	test.Assert(
		grid.cap_cols >= grid.num_cols,
		"Grid capacity cols > num cols after resize past capacity"
	);

	// ensure original data is correct after resize past capacity
	for(int r = 0; r < new_rows; ++r){
		for(int c = 0; c < new_cols; ++c){
			if(r < rows && c < cols){
				test.AssertEq(
					grid.get_cell(r, c), r * 1000 + c,
					"Grid cell value for original cells after resize past capacity"
				);
			}
			else {
				test.AssertEq(
					grid.get_cell(r, c),
					default_value,
					"Grid cell value for new cells after resize past capacity"
				);
			}
		}
	}
}

TEST_CASE("Explicit positioned grid elements", ExplicitGridElements) {
	using namespace Klay;

	auto root = ElementBuilder{}
		.Grid(4, 4)
		.Build();

	auto child1 = root->AddChild(
		ElementBuilder{}
			.Row(0)
			.Col(0)
			.Build()
	);

	auto child2 = root->AddChild(
		ElementBuilder{}
			.Row(2, 2)
			.Col(1)
			.Build()
	);

	root->ComputeLayout(PxRect::FromXYWH(0, 0, 100, 100));

	auto child1_rect = child1->ComputedRect();
	auto child2_rect = child2->ComputedRect();

	test.AssertEq(
		child1_rect,
		PxRect::FromXYWH(0, 0, 25, 25),
		"Child 1 position wrong"
	);

	test.AssertEq(
		child2_rect,
		PxRect::FromXYWH(25, 50, 25, 50),
		"Child 2 position wrong"
	);
}

TEST_CASE("Grid Gap", GridGap) {
	using namespace Klay;

	auto root = ElementBuilder{}
		.Grid(2, 2)
		.Gap(Px{10}, Px{20})
		.Build();

	const auto& layout_mode = static_cast<const GridLayoutMode*>(
		root->layout_mode.get()
	);

	auto child1 = root->AddChild(
		ElementBuilder{}
			.Row(0)
			.Col(0)
			.Build()
	);

	auto child2 = root->AddChild(
		ElementBuilder{}
			.Row(0)
			.Col(1)
			.Build()
	);

	auto child3 = root->AddChild(
		ElementBuilder{}
			.Row(1)
			.Col(1)
			.Build()
	);

	//     45  10  45
	// 40  1       2
	// 20
	// 40          3
	root->ComputeLayout(PxRect::FromXYWH(0, 0, 100, 100));

	auto child1_rect = child1->ComputedRect();
	auto child2_rect = child2->ComputedRect();
	auto child3_rect = child3->ComputedRect();

	test.AssertEq(
		layout_mode->GetExplicitGridSize(),
		Vector2<int>{2, 2},
		"Explicit grid size is incorrect"
	);

	test.AssertEq(
		layout_mode->GetImplicitGridSize(),
		Vector2<int>{2, 2},
		"Implicit grid size is incorrect"
	);

	test.AssertEq(
		child1_rect,
		PxRect::FromXYWH(0, 0, 45, 40),
		"Child 1 position wrong"
	);

	test.AssertEq(
		child2_rect,
		PxRect::FromXYWH(55, 0, 45, 40),
		"Child 2 position wrong"
	);

	test.AssertEq(
		child3_rect,
		PxRect::FromXYWH(55, 60, 45, 40),
		"Child 2 position wrong"
	);
}

TEST_CASE("Single-span Auto Positioning", SingleSpanAutoPositioning){
	using namespace Klay;

	auto root = ElementBuilder{}
		.Grid(2, 2)
		.Build();

	auto child1 = root->AddChild(
		ElementBuilder{}
			.Build()
	);

	auto child2 = root->AddChild(
		ElementBuilder{}
			.Build()
	);

	auto child3 = root->AddChild(
		ElementBuilder{}
			.Build()
	);

	auto child4 = root->AddChild(
		ElementBuilder{}
			.Build()
	);

	root->ComputeLayout(PxRect::FromXYWH(0, 0, 100, 100));

	auto child1_rect = child1->ComputedRect();
	auto child2_rect = child2->ComputedRect();
	auto child3_rect = child3->ComputedRect();
	auto child4_rect = child4->ComputedRect();

	test.cout << child1_rect << std::endl;
	test.cout << child2_rect << std::endl;
	test.cout << child3_rect << std::endl;
	test.cout << child4_rect << std::endl;

	test.AssertEq(
		child1_rect,
		PxRect::FromXYWH(0, 0, 50, 50),
		"Child 1 position wrong"
	);

	test.AssertEq(
		child2_rect,
		PxRect::FromXYWH(50, 0, 50, 50),
		"Child 2 position wrong"
	);

	test.AssertEq(
		child3_rect,
		PxRect::FromXYWH(0, 50, 50, 50),
		"Child 3 position wrong"
	);

	test.AssertEq(
		child4_rect,
		PxRect::FromXYWH(50, 50, 50, 50),
		"Child 4 position wrong"
	);
}

TEST_CASE("Grid repeat to track list", GridRepeatToTrackList) {
	using namespace Klay;

	auto track_list = GridTrackList{
		{
			Px{10},
			GridRepeat{3, Px{20}},
			Px{30},
			GridRepeat{2, Px{40}},
		}
	};

	std::vector<float> expected_sizes = {10, 20, 20, 20, 30, 40, 40};

	test.AssertEq(
		track_list.sizes.size(),
		expected_sizes.size(),
		"Track list size"
	);

	for (size_t i = 0; i < expected_sizes.size(); ++i) {
		auto& size = track_list.sizes[i];
		auto expected_size = expected_sizes[i];

		test.AssertEq(
			std::get<Px>(size),
			Px{expected_size},
			"Track list size"
		);
	}
}
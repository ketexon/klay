#include <klay/Klay.hpp>

#include <ktest/KTest.hpp>

using namespace KTest;

TEST_CASE("Minimum size", BasicMinSize) {
	using namespace Klay;

	auto element = ElementBuilder{}.MinWidth(Px { 100 }).Build();
	element->AddChild(
		ElementBuilder{}.Build()
	);
	element->AddChild(
		ElementBuilder{}.Build()
	);
	element->ComputeMinSize();

	auto minWidth = element->computed_min_size.Horizontal();
	auto minHeight = element->computed_min_size.Vertical();
	test.AssertEq(minWidth, 100.0f, "Min width is not 100");
	test.AssertEq(minHeight, 0.0f, "Min height is not 0");
}

TEST_CASE("Minimum size with children", MinSizeWithChildren) {
	using namespace Klay;

	auto element = ElementBuilder{}.MinSize(Px { 100 }, Px { 200 }).Build();
	element->AddChild(
		ElementBuilder{}.MinSize(Px { 80 }, Px { 80 }).Build()
	);
	element->AddChild(
		ElementBuilder{}.MinSize(Px { 70 }, Px { 70 }).Build()
	);
	element->ComputeMinSize();

	auto minWidth = element->computed_min_size.Horizontal();
	auto minHeight = element->computed_min_size.Vertical();
	test.AssertEq(minWidth, 150.0f, "Min size is sum of children min size if children are bigger");
	test.AssertEq(minHeight, 200.0f, "Min size is container min size if children are smaller");
}

TEST_CASE("Minimum size with children", MinSizeWithPadding) {
	using namespace Klay;

	auto element = ElementBuilder{}.PaddingPxLTRB(10, 20, 30, 40).Build();
	element->AddChild(
		ElementBuilder{}.MinSize(Px { 80 }, Px { 80 }).Build()
	);
	element->AddChild(
		ElementBuilder{}.MinSize(Px { 70 }, Px { 70 }).Build()
	);
	element->ComputeMinSize();

	test.AssertEq(
		element->computed_min_size,
		PxSize { 190, 210 },
		"Min size is sum of children min size if children are bigger"
	);
}
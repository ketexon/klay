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

	auto minWidth = element->ComputedMinSize.Horizontal();
	auto minHeight = element->ComputedMinSize.Vertical();
	TestCase::AssertEq(minWidth, 100.0f, "Min width is not 100");
	TestCase::AssertEq(minHeight, 0.0f, "Min height is not 0");
}

TEST_CASE("Minimum size (with children)", MinSizeWithChildren) {
	using namespace Klay;

	auto element = ElementBuilder{}.MinSize(Px { 100 }, Px { 200 }).Build();
	element->AddChild(
		ElementBuilder{}.MinSize(Px { 80 }, Px { 80 }).Build()
	);
	element->AddChild(
		ElementBuilder{}.MinSize(Px { 70 }, Px { 70 }).Build()
	);
	element->ComputeMinSize();

	auto minWidth = element->ComputedMinSize.Horizontal();
	auto minHeight = element->ComputedMinSize.Vertical();
	TestCase::AssertEq(minWidth, 150.0f, "Min size is sum of children min size if children are bigger");
	TestCase::AssertEq(minHeight, 200.0f, "Min size is container min size if children are smaller");
}

TEST_CASE("Basic flexbox without basis", BasicFlex) {
	using namespace Klay;

	auto element = ElementBuilder{}.Flex().Build();
	auto child1 = element->AddChild(
		ElementBuilder{}.FlexGrow(1).Build()
	);
	auto child2 = element->AddChild(
		ElementBuilder{}.FlexGrow(2).Build()
	);
	auto child3 = element->AddChild(
		ElementBuilder{}.FlexGrow(3).Build()
	);

	element->ComputeLayout(PxRect::FromLTRB(0, 0, 600, 0));

	TestCase::AssertEq(
		child1->ComputedRect,
		PxRect::FromXYWH(0, 0, 100, 0),
		"Child 1 has wrong layout"
	);

	TestCase::AssertEq(
		child2->ComputedRect,
		PxRect::FromXYWH(100, 0, 200, 0),
		"Child 2 has wrong layout"
	);

	TestCase::AssertEq(
		child3->ComputedRect,
		PxRect::FromXYWH(300, 0, 300, 0),
		"Child 3 has wrong layout"
	);
}

#include <klay/Klay.hpp>
#include <ktest/KTest.hpp>

#include <array>

using namespace KTest;

TEST_CASE("Basic flexbox", BasicFlex) {
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

	test.AssertEq(
		child1->ComputedRect(),
		PxRect::FromXYWH(0, 0, 100, 0),
		"Child 1 has wrong layout"
	);

	test.AssertEq(
		child2->ComputedRect(),
		PxRect::FromXYWH(100, 0, 200, 0),
		"Child 2 has wrong layout"
	);

	test.AssertEq(
		child3->ComputedRect(),
		PxRect::FromXYWH(300, 0, 300, 0),
		"Child 3 has wrong layout"
	);
}

TEST_CASE("Justify content", JustifyContent) {
	using namespace Klay;

	auto element = ElementBuilder{}.Flex().Build();
	auto child1 = element->AddChild(
		ElementBuilder{}.MinWidth(Px{10}).Build()
	);
	auto child2 = element->AddChild(
		ElementBuilder{}.MinWidth(Px{10}).Build()
	);
	auto child3 = element->AddChild(
		ElementBuilder{}.MinWidth(Px{10}).Build()
	);

	// spaces[i] alternates between the gaps between children and the children
	// starting with the gap
	const auto testJustify = [&](Justify j, std::string name, std::array<float, 7> spaces) {
		element->Layout.Options.JustifyContent = j;
		element->ComputeLayout(PxRect::FromLTRB(0, 0, 100, 0));

		float offset = 0;
		for(int i = 0; i < 3; ++i){
			offset += spaces[2 * i];
			float size = spaces[2 * i + 1];
			const auto rect = element->Children[i]->ComputedRect();
			const auto expected = PxRect::FromXYWH(offset, 0, size, 0);
			const auto equal =  (
				std::abs(rect.Horizontal().Start - expected.Horizontal().Start) < 1e-5f
				&& std::abs(rect.Horizontal().Length - expected.Horizontal().Length) < 1e-5f
				&& std::abs(rect.Vertical().Start - expected.Vertical().Start) < 1e-5f
				&& std::abs(rect.Vertical().Length - expected.Vertical().Length) < 1e-5f
			);
			test.Expect(
				equal,
				rect, expected,
				(
					std::stringstream{}
					<< "Justify " << name
					<< ": Child " << i << " has wrong layout"
				).str()
			);
			offset += size;
		}
	};

	testJustify(Justify::Start, "Start", {0, 10, 0, 10, 0, 10, 70});
	testJustify(Justify::Center, "Center", {35, 10, 0, 10, 0, 10, 35});
	testJustify(Justify::End, "End", {70, 10, 0, 10, 0, 10, 0});
	testJustify(Justify::SpaceBetween, "SpaceBetween", {0, 10, 35, 10, 35, 10, 0 });
	testJustify(Justify::SpaceEvenly, "SpaceEvenly", {70.f/4, 10, 70.f/4, 10, 70.f/4, 10, 70.f/4});
	testJustify(Justify::SpaceAround, "SpaceAround", {70.f/6, 10, 70.f/3, 10, 70.f/3, 10, 70.f/6});
}

TEST_CASE("Align items", AlignItems) {
	using namespace Klay;

	auto element = ElementBuilder{}.Flex().AlignItems(Align::Center).Build();
	auto defaultAlign = element->AddChild(
		ElementBuilder{}.MinSize(Px{10}, Px{10}).Build()
	);
	auto alignStart = element->AddChild(
		ElementBuilder{}.MinSize(Px{10}, Px{10}).AlignSelf(Align::Start).Build()
	);
	auto alignCenter = element->AddChild(
		ElementBuilder{}.MinSize(Px{10}, Px{10}).AlignSelf(Align::Center).Build()
	);
	auto alignEnd = element->AddChild(
		ElementBuilder{}.MinSize(Px{10}, Px{10}).AlignSelf(Align::End).Build()
	);
	auto alignStretch = element->AddChild(
		ElementBuilder{}.MinSize(Px{10}, Px{10}).AlignSelf(Align::Stretch).Build()
	);

	element->ComputeLayout(PxRect::FromXYWH(0, 0, 100, 100));

	test.AssertEq(
		defaultAlign->ComputedRect(),
		PxRect::FromXYWH(0, 45, 10, 10),
		"Default align does not match parent"
	);
	test.AssertEq(
		alignStart->ComputedRect(),
		PxRect::FromXYWH(10, 0, 10, 10),
		"Align start rect incorrect"
	);
	test.AssertEq(
		alignCenter->ComputedRect(),
		PxRect::FromXYWH(20, 45, 10, 10),
		"Align center rect incorrect"
	);
	test.AssertEq(
		alignEnd->ComputedRect(),
		PxRect::FromXYWH(30, 90, 10, 10),
		"Align end rect incorrect"
	);
	test.AssertEq(
		alignStretch->ComputedRect(),
		PxRect::FromXYWH(40, 0, 10, 100),
		"Align stretch rect incorrect"
	);
}

TEST_CASE("Flex Gap", FlexGap) {
	using namespace Klay;

	auto element = ElementBuilder{}.Flex().Gap(Px{10}).AlignItems(Align::Stretch).Build();
	auto child1 = element->AddChild(
		ElementBuilder{}.FlexGrow(1).Build()
	);
	auto child2 = element->AddChild(
		ElementBuilder{}.FlexGrow(1).Build()
	);

	element->ComputeLayout(PxRect::FromXYWH(0, 0, 100, 100));

	test.AssertEq(
		child1->ComputedRect(),
		PxRect::FromXYWH(0, 0, 45, 100),
		"Child 1 has wrong layout"
	);
	test.AssertEq(
		child2->ComputedRect(),
		PxRect::FromXYWH(55, 0, 45, 100),
		"Child 2 has wrong layout"
	);
}

TEST_CASE("Padding", FlexPadding) {
	using namespace Klay;

	auto element = ElementBuilder{}
		.Flex()
		.PaddingLTRB(10, 20, 30, 40)
		.AlignItems(Align::Stretch)
		.Build();

	auto child1 = element->AddChild(
		ElementBuilder{}.FlexGrow(1).Build()
	);

	element->ComputeLayout(PxRect::FromXYWH(0, 0, 100, 100));

	test.AssertEq(
		child1->ComputedRect(),
		PxRect::FromXYWH(10, 20, 60, 40),
		"Child has wrong layout"
	);
}
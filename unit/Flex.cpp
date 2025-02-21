#include "./Unit.hpp"

void UnitTestFlex::Init() {
	root = Klay::ElementBuilder{}
		.Flex()
		.JustifyContent(Klay::Justify::SpaceEvenly)
		.AlignItems(Klay::Align::Center)
		.Gap(Klay::Px{8})
		.Build();
	auto child1 = Klay::ElementBuilder{}.MinWidth(Klay::Px{50}).MinHeight(Klay::Px{50}).UserData(RED).Build();
	auto child2 = Klay::ElementBuilder{}.MinWidth(Klay::Px{50}).MinHeight(Klay::Px{50}).UserData(GREEN).Build();
	auto child3 = Klay::ElementBuilder{}.MinWidth(Klay::Px{50}).MinHeight(Klay::Px{50}).UserData(BLUE).Build();

	root->AddChild(child1);
	root->AddChild(child2);
	root->AddChild(child3);

	OnResize();
}

void UnitTestFlex::OnResize(){
	root->ComputeLayout(Klay::PxRect::FromWH(window->GetWidth(), window->GetHeight()));
}

void UnitTestFlex::Run(){
	for(auto& child : root->children){
		DrawElement(child, std::any_cast<Color>(child->user_data));
	}
}

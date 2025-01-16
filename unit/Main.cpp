#include <raylib-cpp.hpp>
#include <klay/Klay.hpp>

constexpr int screenWidth = 600;
constexpr int screenHeight = 400;

void DrawElement(std::shared_ptr<const Klay::Element> element, Color color){
	auto rect = element->ComputedRect();
	DrawRectangle(
		static_cast<int>(rect.X()),
		static_cast<int>(rect.Y()),
		static_cast<int>(rect.Width()),
		static_cast<int>(rect.Height()),
		color
	);
}

int main(){
	auto root = Klay::ElementBuilder{}
		.Flex()
		.JustifyContent(Klay::Justify::SpaceEvenly)
		.AlignItems(Klay::Align::Center)
		.Gap(Klay::Px{8})
		.Build();
	auto child1 = Klay::ElementBuilder{}.MinWidth(Klay::Px{50}).MinHeight(Klay::Px{50}).Build();
	auto child2 = Klay::ElementBuilder{}.MinWidth(Klay::Px{50}).MinHeight(Klay::Px{50}).Build();
	auto child3 = Klay::ElementBuilder{}.MinWidth(Klay::Px{50}).MinHeight(Klay::Px{50}).Build();

	root->AddChild(child1);
	root->AddChild(child2);
	root->AddChild(child3);

	raylib::Window window {
		screenWidth, screenHeight,
		"Klay Unit Test",
		FLAG_WINDOW_RESIZABLE
	};

	const auto recomputeLayout = [&](){
		root->ComputeLayout(Klay::PxRect::FromWH(window.GetWidth(), window.GetHeight()));
	};
	recomputeLayout();

	while(!window.ShouldClose()){
		if(window.IsResized()){
			recomputeLayout();
		}

		window.BeginDrawing();
		window.ClearBackground(RAYWHITE);

		DrawElement(child1, RED);
		DrawElement(child2, GREEN);
		DrawElement(child3, BLUE);

		window.EndDrawing();
	}

	return 0;
}
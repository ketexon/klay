#include <raylib-cpp.hpp>
#include <klay/Klay.hpp>
#include <vector>

#include "./Unit.hpp"

constexpr int screenWidth = 600;
constexpr int screenHeight = 400;

UnitTest::UnitTest(raylib::Window* window) : window(window) {}

void UnitTest::DrawElement(std::shared_ptr<const Klay::Element> element, Color color){
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
	raylib::Window window {
		screenWidth, screenHeight,
		"Klay Unit Test",
		FLAG_WINDOW_RESIZABLE
	};

	UnitTestFlex flex{&window};
	auto units = std::vector<UnitTest*> {
		&flex,
	};
	size_t unit_index = 0;

	units[unit_index]->Init();

	while(!window.ShouldClose()){
		if(window.IsResized()){
			units[unit_index]->OnResize();
		}

		window.BeginDrawing();
		window.ClearBackground(RAYWHITE);

		units[unit_index]->Run();

		window.EndDrawing();
	}

	return 0;
}

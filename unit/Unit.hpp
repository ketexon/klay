#pragma once
#include <klay/Klay.hpp>
#include <raylib-cpp.hpp>

struct UnitTest {
	raylib::Window* window;

	UnitTest(raylib::Window* window);
	virtual void Init() {}
	virtual void OnResize() {}
	virtual void Run() {}
	virtual void Shutdown() {}

	void DrawElement(std::shared_ptr<const Klay::Element> element, Color color);

	virtual ~UnitTest() = default;
};

struct UnitTestFlex : UnitTest {
	std::shared_ptr<Klay::Element> root;

	using UnitTest::UnitTest;

	void Init() override;
	void Run() override;
	void OnResize() override;
};

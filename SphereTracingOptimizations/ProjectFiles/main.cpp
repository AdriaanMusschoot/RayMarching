#include "SdEngine.h"

int main(int argc, char* args[])
{
	constexpr uint32_t width{ 600 };
	constexpr uint32_t height{ 600 };

	VM::SdEngine Engine{ width, height };

	Engine.Run();
}
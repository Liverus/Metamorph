
#include "console.h"

void Console::Print(Color color, const char* prefix, const char* text, ...)
{
	time_t currentTime;
	time(&currentTime);

	struct tm timeInfo;
	localtime_s(&timeInfo, &currentTime);

	char buffer[11];
	strftime(buffer, 9, XOR("%H:%M:%S"), &timeInfo);

	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(consoleHandle, static_cast<WORD>(Color::White));
	printf(XOR(" [%s]"), buffer);

	SetConsoleTextAttribute(consoleHandle, static_cast<WORD>(color));
	printf(XOR(" [%s] "), prefix);

	SetConsoleTextAttribute(consoleHandle, static_cast<WORD>(Color::DarkWhite));

	va_list args;
	va_start(args, text);
	vprintf(text, args);
	va_end(args);

	printf(XOR("\n"));
}

void Console::NewLine() {
	printf(XOR("\n"));
}

void Console::Clear()
{
	system(XOR("cls"));
}

void Console::Pause() {
	system(XOR("pause"));
}

void Console::Title(const char* title)
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	SetConsoleTextAttribute(consoleHandle, static_cast<WORD>(Color::Purple));
	printf(XOR("%s\n"), title);

	SetConsoleTextAttribute(consoleHandle, static_cast<WORD>(Color::DarkWhite));
	printf(XOR(" Built on %s\n\n"), __DATE__);
}
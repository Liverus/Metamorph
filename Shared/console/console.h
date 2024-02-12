#pragma once

#include <Windows.h>
#include <iostream>

#include "../skcrypter/skcrypter.h"

#define XOR(...) skCrypt(__VA_ARGS__)

#define SetTitle(txt) Console::Title(txt)

#define Log(...) Console::Print(Console::Color::__VA_ARGS__)

#define Success(...) Log(Green, XOR("+"), __VA_ARGS__)
#define Warning(...) Log(Yellow, XOR("!"), __VA_ARGS__)
#define Alert(...) Log(Red, XOR("!"), __VA_ARGS__)
#define Error(...) Log(Red, XOR("-"), __VA_ARGS__)

#define Newline Console::NewLine()
#define SystemPause Console::Pause()

namespace Console
{
	enum class Color
	{
		Default,
		DarkBlue,
		DarkGreen,
		DarkCyan,
		DarkRed,
		DarkPurple,
		DarkYellow,
		DarkWhite,
		DarkGrey,
		Blue,
		Green,
		Cyan,
		Red,
		Purple,
		Yellow,
		White
	};

	void Print(Color color, const char* prefix, const char* text, ...);
	void NewLine();
	void Clear();
	void Pause();
	void Title(const char* title);
}
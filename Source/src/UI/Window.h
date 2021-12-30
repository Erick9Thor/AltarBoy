#pragma once
#include <string>
using namespace std;

class Window
{
public:
	Window(const char* name, bool active);
	virtual void Update() {};

public:
	const char* name;
	bool active = true;
};

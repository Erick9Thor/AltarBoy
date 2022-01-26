#pragma once
#include <string>
using namespace std;

class Window
{
public:
	Window(const char* name, bool active);

	// --- Life cycle --- //
	virtual void Init() {};
	virtual void Update() {};
	virtual void CleanUp() {};

public:
	const char* name;
	bool active = true;
};

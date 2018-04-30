#pragma once
class HookerSwarm
{
	// Constructors
public:
	HookerSwarm(_In_ HMODULE hModule);
	~HookerSwarm();

	// Methods
public:
	bool initializeHooks();
	bool deinitializeHooks();

	// Attributes
public:
	HMODULE m_hModule;
};

//public:
	//functions typedefs

	//attach

	//hook
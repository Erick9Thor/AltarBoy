#include "UID.h"

#include <random>
#include <sstream>
#include <string>

// USED this algorithm to generate uid https://stackoverflow.com/questions/24365331/how-can-i-generate-uuid-in-c-without-using-boost-library

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_int_distribution<UID> dis;
static std::uniform_int_distribution<UID> distribution_hex(0, 15);
static std::uniform_int_distribution<UID> distribution_hex_2(8, 11);

UID GenerateUID()
{
	return dis(gen);
}

std::string GenerateUID128()
{
	std::stringstream ss;

	ss << std::hex;
	ss << std::uppercase;

	for (int i = 0; i < 8; i++)
	{
		ss << distribution_hex(gen);
	}

	ss << "-";
	for (int i = 0; i < 4; i++)
	{
		ss << distribution_hex(gen);
	}

	ss << "-4";
	for (int i = 0; i < 3; i++)
	{
		ss << distribution_hex(gen);
	}

	ss << "-";
	ss << distribution_hex_2(gen);

	for (int i = 0; i < 3; i++)
	{
		ss << distribution_hex(gen);
	}

	ss << "-";

	for (int i = 0; i < 12; i++)
	{
		ss << distribution_hex(gen);
	};

	return ss.str();
}
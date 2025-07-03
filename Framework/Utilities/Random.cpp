#include "Framework.h"
#include "Random.h"

int Random::Range(int p_Min, int p_Max)
{
	std::random_device randDevice;

	std::uniform_int_distribution<int> intRand(p_Min, p_Max);

	int iValue = intRand(randDevice);

	return iValue;
}

float Random::Range(float p_Min, float p_Max)
{
	std::random_device randDevice;

	std::uniform_real_distribution<float> floatRand(p_Min, p_Max);

	float fValue = floatRand(randDevice);

	return fValue;
}

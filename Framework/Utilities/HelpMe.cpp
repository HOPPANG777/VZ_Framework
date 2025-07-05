#include "Framework.h"
#include "HelpMe.h"

HelpMe::HelpMe()
{

}

HelpMe::~HelpMe()
{
	
}

void HelpMe::Pre_Log()
{
	cout << "--------------------------------------------------------------------------" << endl;

	time_t now1 = time(nullptr);
	tm tm_1;

	localtime_s(&tm_1, &now1);

	Log_Count++;

	int Hour_Temp = (tm_1.tm_hour > 12) ? (tm_1.tm_hour - 12) : tm_1.tm_hour;

	string HHour = (Hour_Temp > 9) ? to_string(Hour_Temp) : "0" + to_string(Hour_Temp);
	string MMin = (tm_1.tm_min > 9) ? to_string(tm_1.tm_min) : "0" + to_string(tm_1.tm_min);
	string SSec = (tm_1.tm_sec > 9) ? to_string(tm_1.tm_sec) : "0" + to_string(tm_1.tm_sec);

	cout << "[" << HHour << ":" << MMin << ":" << SSec << "] - " << "(" << Log_Count << ")" << endl;
}

void HelpMe::Log()
{
	Pre_Log();

	cout << "Hello~" << endl;
}

void HelpMe::Log(int p_Value)
{
	Pre_Log();

	cout << p_Value << endl;
}

void HelpMe::Log(float p_Value)
{
	Pre_Log();

	cout << p_Value << endl;
}

void HelpMe::Log(double p_Value)
{
	Pre_Log();

	cout << p_Value << endl;
}

void HelpMe::Log(bool p_Value)
{
	Pre_Log();

	cout << (p_Value ? "True" : "False") << endl;
}

void HelpMe::Log(wstring p_Value)
{
	Pre_Log();

	cout << string(p_Value.begin(), p_Value.end()) << endl;
}

void HelpMe::Log(string p_Value)
{
	Pre_Log();

	cout << p_Value << endl;
}

void HelpMe::Log(char p_Value)
{
	Pre_Log();

	cout << p_Value << endl;
}

void HelpMe::Log(wchar_t p_Value)
{
	Pre_Log();

	wstring Temp(&p_Value);

	cout << string(Temp.begin(), Temp.end()) << endl;
}

void HelpMe::Log(wchar_t& p_Value)
{
	Pre_Log();

	wstring Temp(&p_Value);

	cout << string(Temp.begin(), Temp.end()) << endl;
}

void HelpMe::Log(wchar_t p_Value[])
{
	Pre_Log();

	wstring Temp(p_Value);

	cout << string(Temp.begin(), Temp.end()) << endl;
}

void HelpMe::Log(char& p_Value)
{
	Pre_Log();

	cout << &p_Value << endl;
}

void HelpMe::Log(char p_Value[])
{
	Pre_Log();

	cout << p_Value << endl;
}

//void HelpMe::Log(uint p_Value)
//{
//	Pre_Log();
//
//	cout << p_Value << endl;
//}
//
//void HelpMe::Log(Color p_Value)
//{
//	Pre_Log();
//
//	cout << "(";
//	cout << p_Value.r << ",";
//	cout << p_Value.g << ",";
//	cout << p_Value.b << ",";
//	cout << p_Value.a << ")" << endl;
//}
//
//void HelpMe::Log(Vector2 p_Value)
//{
//	Pre_Log();
//
//	cout << "(";
//	cout << p_Value.x << ",";
//	cout << p_Value.y << ")" << endl;
//}
//
//void HelpMe::Log(Vector3 p_Value)
//{
//	Pre_Log();
//
//	cout << "(";
//	cout << p_Value.x << ",";
//	cout << p_Value.y << ",";
//	cout << p_Value.z << ")" << endl;
//}
//
//void HelpMe::Log(Matrix p_Value)
//{
//	Pre_Log();
//
//	cout << "l " << p_Value._11 << " , " << p_Value._12 << " , " << p_Value._13 << " , " << p_Value._14 << " l" << endl;
//	cout << "l " << p_Value._21 << " , " << p_Value._22 << " , " << p_Value._23 << " , " << p_Value._24 << " l" << endl;
//	cout << "l " << p_Value._31 << " , " << p_Value._32 << " , " << p_Value._33 << " , " << p_Value._34 << " l" << endl;
//	cout << "l " << p_Value._41 << " , " << p_Value._42 << " , " << p_Value._43 << " , " << p_Value._44 << " l" << endl;
//
//
//}
//
//void HelpMe::Log(IObject* p_Value)
//{
//	Pre_Log();
//
//	cout << (p_Value != nullptr ? "True" : "False") << endl;
//}
//
//float HelpMe::Distance(Vector2 p_Start, Vector2 p_End)
//{
//	float X = (p_End.x - p_Start.x);
//	float Y = (p_End.y - p_Start.y);
//
//
//	return (sqrt(X * X) + (Y * Y));
//}
//
//float HelpMe::Distance(Vector3 p_Start, Vector3 p_End)
//{
//	float X = (p_End.x - p_Start.x);
//	float Y = (p_End.y - p_Start.y);
//	float Z = (p_End.z - p_Start.z);
//
//	return (sqrt(X * X) + (Y * Y) + (Z * Z));
//}

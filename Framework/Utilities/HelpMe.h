#pragma once

class HelpMe
{
	DECLARE_SINGLETON(HelpMe);

public:
	int Log_Count = 0;

private:

	void Pre_Log();
public:

	void Log();
	void Log(int p_Value);
	void Log(float p_Value);
	void Log(double p_Value);
	void Log(bool p_Value);
	void Log(std::wstring p_Value);
	void Log(std::string p_Value);
	void Log(char p_Value);
	void Log(char& p_Value);
	void Log(char p_Value[]);
	void Log(wchar_t p_Value);
	void Log(wchar_t& p_Value);
	void Log(wchar_t p_Value[]);
	//void Log(uint p_Value);
	//void Log(Color p_Value);
	//void Log(Vector2 p_Value);
	//void Log(Vector3 p_Value);
	//void Log(Matrix p_Value);
	//void Log(IObject* p_Value);

	//float Distance(Vector2 p_Start, Vector2 p_End);
	//float Distance(Vector3 p_Start, Vector3 p_End);
};


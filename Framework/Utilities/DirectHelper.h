#pragma once
class Values
{
public:
        // Screen related helpers are provided as functions so that
        // they reflect the current resolution at any time.
        static UINT GetWinWidth();
        static UINT GetWinHeight();
        static float GetCenterX();
        static float GetCenterY();
        static Vector3 GetCenter();
        static Vector3 GetScreenSize();

        const static float PI;
        const static float Deg2Rad;

	const static Vector3 ZeroVec3;
	const static Vector3 OneVec3;
	const static Vector3 LeftVec;
	const static Vector3 RightVec;
	const static Vector3 UpVec;
	const static Vector3 DownVec;
	const static Vector3 FwdVec;

	const static Vector2 ZeroVec2;
	const static Vector2 OneVec2;
	const static Vector2 uvLT;
	const static Vector2 uvLB;
	const static Vector2 uvRT;
	const static Vector2 uvRB;

	const static Color Red;
	const static Color Green;
	const static Color Blue;
	const static Color Yellow;
	const static Color Magenta;
	const static Color Cyan;
	const static Color Black;
	const static Color White;
	const static Color ClearColor;
	const static Color Transparent;

	const static Matrix Identity;
};


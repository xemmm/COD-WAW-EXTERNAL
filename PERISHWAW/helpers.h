#pragma once


#include "datatypes.h"
#include "GLFW/glfw3.h"

void showMenu(GLFWwindow* window)
{
	glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, false);
}

void hideMenu(GLFWwindow* window)
{
	glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, true);
}


void ConvertToRange(Vec2& Point)
{
	Point.X /= 1920.0f;
	Point.X *= 2.0f;
	Point.X -= 1.0f;

	Point.Y /= 1080.0f;
	Point.Y *= 2.0f;
	Point.Y -= 1.0f;

}

bool WorldToScreen(const Vec3& vecOrgin, Vec2& VecScreen, float* Matrix)
{
	
	VecScreen.X = vecOrgin.X * Matrix[0] + vecOrgin.Y * Matrix[1] + vecOrgin.Z * Matrix[2] + Matrix[3];
	VecScreen.Y = vecOrgin.X * Matrix[4] + vecOrgin.Y * Matrix[5] + vecOrgin.Z * Matrix[6] + Matrix[7];
	float W = vecOrgin.X * Matrix[12] + vecOrgin.Y * Matrix[13] + vecOrgin.Z * Matrix[14] + Matrix[15];

	if (W < 0.01f)
		return false;

	Vec2 NDC;
	NDC.X = VecScreen.X / W;
	NDC.Y = VecScreen.Y / W;

	VecScreen.X = (1920 / 2 * NDC.X) + (NDC.X + 1920 / 2);
	VecScreen.Y = (1080 / 2 * NDC.Y) + (NDC.Y + 1080 / 2);


	ConvertToRange(VecScreen);

	return true;

}
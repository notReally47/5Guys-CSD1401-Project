#include "cprocessing.h"
#include "math.h"

int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y)
{
	// finding X1,Y1 coords -- top left corner of the rectangle
	float x1 = area_center_x-area_width*0.5f;
	float y1 = area_center_y-area_height*0.5f;
	// finding X2,Y2 coords -- bottom right corner of the rectangle
	float x2 = area_center_x+area_width*0.5f;
	float y2 = area_center_y+area_height*0.5f;
	return ((click_x<x2 && click_x>x1 && click_y<y2 && click_y>y1)?1:0); // return 1 if the mouse is within the rectangle
}
/*
int IsCircleClicked(float circle_center_x, float circle_center_y, float diameter, float click_x, float click_y)
{
	float radius = diameter*0.5f; // finding radius of the circle	
	float distmouse = CP_Math_Distance(circle_center_x,circle_center_y,click_x,click_y); // finding distance from mouse click to the center of the circle
	return ((distmouse<radius)?1:0); // return 1 if distance from the mouse is less than the radius
}
*/
CP_Vector AngleToVector(float radian_angle)
{	// for movement of sprites ?
	float x = cos(CP_Math_Radians(radian_angle)); // finding the x value of a vector by calculating the cosine of the radian angle
	float y = sin(CP_Math_Radians(radian_angle)); // finding the y value of a vector by calculating the sine of the radian angle
    return (CP_Vector_Scale(CP_Vector_Set(x, y), 9.f)); // return the vector with a scalar
}

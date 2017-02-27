#include <GL/glut.h>
#include <ctime>
#include <cstdlib>
#include "General.h"
#include "MyMath.h"


/*
 * Global variables
 *
 * NOTE: const global variables declared with extern in a .h file need to be set in a single .cpp
 *		 file, but will still be visible to other .cpp files. If a global variable declared const
 *		 has the extern keyword modifier, it can't be intialized in the .h file.
 *
 * NOTE: It appears any global variables declared with extern in a .h file need to be set in a single .cpp
 *		 file, but will still be visible to other .cpp files. If any global variable
 *		 has the extern keyword modifier, it can't be intialized in the .h file and must be
 *		 declared in a single .cpp file.
 */
const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 600;
const unsigned int SLEEP_DURATION = 1;
const int MAGIC_GARBAGE = -999;
float *pixelBuffer;




/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Timer
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Timer::Timer(float newDuration, float newElapsedTime, float newStartTime)
{
	mDuration = newDuration;
	mElapsedTime = newElapsedTime;
	mStartTime = newStartTime;
}

/*
* Accessors
*/
float Timer::GetDuration() const
{
	return mDuration;
}

float Timer::GetElapsedTime() const
{
	return mElapsedTime;
}

float Timer::GetStartTime() const
{
	return mStartTime;
}

/*
* Mutators
*/
void Timer::SetDuration(float newDuration)
{
	mDuration = newDuration;
}

void Timer::SetElapsedTime(float newElapsedTime)
{
	mElapsedTime = newElapsedTime;
}

void Timer::SetStartTime(float newStartTime)
{
	mStartTime = newStartTime;
}

/*Reset elapsed time and set start time to current time*/
void Timer::Reset()
{
	mStartTime = (float)clock();
	mElapsedTime = 0.0f;
}






/*
 * Global functions
 */
Color4 GetRandomColor()
{
	float r = (rand() % 256) / 255.0f;
	float g = (rand() % 256) / 255.0f;
	float b = (rand() % 256) / 255.0f;
	float a = (rand() % 256) / 255.0f;
	return Color4(r, g, b, a);
}

/*Returns a non-yellow, non-dark color.*/
Color4 GetRandomCurveColor()
{
	Color4 retColor;
	do
	{
		retColor = GetRandomColor();
	} while ((retColor.GetR() >= 0.8f && retColor.GetG() >= 0.8f) ||
		(retColor.GetR() <= 0.5f && retColor.GetG() <= 0.5f && retColor.GetB() <= 0.5f));

	return retColor;
}

void SetPixel(int x, int y, const Color3 &color)
{
	for (int colorIndex = 0; colorIndex < Color3::Num__RGBParameters; colorIndex++)
	{
		if ((x * (int)Color3::Num__RGBParameters + y * WINDOW_WIDTH * (int)Color3::Num__RGBParameters + colorIndex) < 0 ||
			(x * (int)Color3::Num__RGBParameters + y * WINDOW_WIDTH * (int)Color3::Num__RGBParameters + colorIndex) >= WINDOW_WIDTH * WINDOW_HEIGHT * 3)
			break;
		pixelBuffer[x * (int)Color3::Num__RGBParameters + y * WINDOW_WIDTH * (int)Color3::Num__RGBParameters + colorIndex] = color[colorIndex]; //See pgs. 146-147 to optimize this.
	}
}

unsigned int StringToUInt(const std::string &str)
{
	unsigned int retVal = 0;
	unsigned int weight = 0;

	for (unsigned int i = str.size() - 1; i >= 0; i--)
	{
		retVal += ((int)str[i] - 48) * weight;
		weight += 10;
	}

	return retVal;
}

std::string UIntToString(unsigned int num)
{
	std::string retVal = "";

	do
	{
		retVal.insert(retVal.begin(), (char)((num % 10) + 48));
		num /= 10;
	} while (num > 0);

	return retVal;
}

std::string FloatToString(float num)
{
	std::string retVal = "";

	/*If the float is negative, insert a negative sign into the string retVal. Then
	  make the float positive for the rest of the calculations*/
	if (num < 0)
	{
		retVal = "-";
		num *= -1;
	}

	/*Insert the portion of the float > 0 into the string retVal*/
	unsigned int numAsUInt = (unsigned int)num;
	retVal += UIntToString(numAsUInt);

	/*Remove the portion of the float > 0, leaving only the decimal.
	  Also, insert a decimal point into the string retVal*/
	num -= numAsUInt;
	retVal += ".";

	/*Insert the decimal portion of the float into the string retVal, with a max
	  of 2 decimal places*/
	unsigned int count = 0;
	do
	{
		num *= 10;
		retVal += (char)((int)num + 48);
		num -= (int)num;

		count++;
	} while (count < 2 && num != 0);

	return retVal;
}
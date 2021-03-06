#ifndef HFILE_GENERAL
#define HFILE_GENERAL


#include <vector>
#include <ctime>
#include <cmath>
#include <cstdlib>



/*
* Classes
*/
class Color3
{
public:
	enum RGBParameters
	{
		Red,
		Green,
		Blue,
		Num__RGBParameters,
	};
public:
	//Constructors
	Color3(float red = 1.0f, float green = 1.0f, float blue = 1.0f)
	{
		SetR(red);
		SetG(green);
		SetB(blue);
	}

	//Accessors
	float GetR() const
	{
		return mRGB[(int)Red];
	}
	float GetG() const
	{
		return mRGB[(int)Green];
	}
	float GetB() const
	{
		return mRGB[(int)Blue];
	}

	//Mutators
	void Set(const Color3 &color)
	{
		SetR(color.GetR());
		SetG(color.GetG());
		SetB(color.GetB());
	}
	void Set(float newRed, float newGreen, float newBlue)
	{
		SetR(newRed);
		SetG(newGreen);
		SetB(newBlue);
	}
	void SetR(float newRed)
	{
		newRed = (newRed < 0.0f) ? 0.0f : newRed;
		newRed = (newRed > 1.0f) ? 1.0f : newRed;
		mRGB[(int)Red] = newRed;
	}
	void SetG(float newGreen)
	{
		newGreen = (newGreen < 0.0f) ? 0.0f : newGreen;
		newGreen = (newGreen > 1.0f) ? 1.0f : newGreen;
		mRGB[(int)Green] = newGreen;
	}
	void SetB(float newBlue)
	{
		newBlue = (newBlue < 0.0f) ? 0.0f : newBlue;
		newBlue = (newBlue > 1.0f) ? 1.0f : newBlue;
		mRGB[(int)Blue] = newBlue;
	}

	//Overloaded operators
	float &operator[](int index)
	{
		if (index < 0 || index >= (int)Num__RGBParameters)
			return mRGB[(int)Red]; //If index out of bounds, return the red value by default.
		return mRGB[index];
	}
	const float &operator[](int index) const
	{
		if (index < 0 || index >= (int)Num__RGBParameters)
			return mRGB[(int)Red]; //If index is out of bounds, return the first element by default.
		return mRGB[index];
	}

private:
	float mRGB[Num__RGBParameters];
};


//Color4 is like Color3 but has an alpha value
class Color4
{
public:
	enum RGBAParameters
	{
		Red,
		Green,
		Blue,
		Alpha,
		Num__RGBAParameters,
	};
public:
	//Constructors
	Color4(float red = 1.0f, float green = 1.0f, float blue = 1.0f, float alpha = 1.0f)
	{
		SetR(red);
		SetG(green);
		SetB(blue);
		SetA(alpha);
	}

	//Accessors
	float GetR() const
	{
		return mRGBA[(int)Red];
	}
	float GetG() const
	{
		return mRGBA[(int)Green];
	}
	float GetB() const
	{
		return mRGBA[(int)Blue];
	}
	float GetA() const
	{
		return mRGBA[(int)Alpha];
	}
	Color3 GetColor3() const
	{
		return Color3(mRGBA[(int)Red], mRGBA[(int)Green], mRGBA[(int)Blue]);
	}

	//Mutators
	void Set(const Color4 &color)
	{
		SetR(color.GetR());
		SetG(color.GetG());
		SetB(color.GetB());
		SetA(color.GetA());
	}
	void Set(float newRed, float newGreen, float newBlue, float newAlpha)
	{
		SetR(newRed);
		SetG(newGreen);
		SetB(newBlue);
		SetA(newAlpha);
	}
	void SetR(float newRed)
	{
		newRed = (newRed < 0.0f) ? 0.0f : newRed;
		newRed = (newRed > 1.0f) ? 1.0f : newRed;
		mRGBA[(int)Red] = newRed;
	}
	void SetG(float newGreen)
	{
		newGreen = (newGreen < 0.0f) ? 0.0f : newGreen;
		newGreen = (newGreen > 1.0f) ? 1.0f : newGreen;
		mRGBA[(int)Green] = newGreen;
	}
	void SetB(float newBlue)
	{
		newBlue = (newBlue < 0.0f) ? 0.0f : newBlue;
		newBlue = (newBlue > 1.0f) ? 1.0f : newBlue;
		mRGBA[(int)Blue] = newBlue;
	}
	void SetA(float newAlpha)
	{
		newAlpha = (newAlpha < 0.0f) ? 0.0f : newAlpha;
		newAlpha = (newAlpha > 1.0f) ? 1.0f : newAlpha;
		mRGBA[(int)Alpha] = newAlpha;
	}

	//Overloaded operators
	float &operator[](int index)
	{
		if (index < 0 || index >= (int)Num__RGBAParameters)
			return mRGBA[(int)Red]; //If index out of bounds, return the red value by default.
		return mRGBA[index];
	}
	const float &operator[](int index) const
	{
		if (index < 0 || index >= (int)Num__RGBAParameters)
			return mRGBA[(int)Red]; //If index is out of bounds, return the first element by default.
		return mRGBA[index];
	}

private:
	float mRGBA[Num__RGBAParameters];
};

class Vector2I
{
public:
	enum VectorElements
	{
		X,
		Y,
		Num__Elements,
	};
public:
	//Constructors
	Vector2I(int x = 0, int y = 0)
	{
		mArr[X] = x;
		mArr[Y] = y;
	}

	//Accessors
	int GetX() const
	{
		return mArr[X];
	}
	int GetY() const
	{
		return mArr[Y];
	}

	//Mutators
	void SetX(int newX)
	{
		mArr[X] = newX;
	}
	void SetY(int newY)
	{
		mArr[Y] = newY;
	}

	//Overloaded operators
	int &operator[](int index)
	{
		if (index < 0 || index >= (int)Num__Elements)
			return mArr[X]; //If index is out of bounds, return the first element by default.
		return mArr[index];
	}
	const int &operator[](int index) const
	{
		if (index < 0 || index >= (int)Num__Elements)
			return mArr[X]; //If index is out of bounds, return the first element by default.
		return mArr[index];
	}
	friend bool operator== (const Vector2I &pair1, const Vector2I &pair2)
	{
		return (pair1.GetX() == pair2.GetX() && pair1.GetY() == pair2.GetY());
	}
	friend Vector2I operator+(const Vector2I &left, const Vector2I &right)
	{
		return Vector2I(left[X] + right[X], left[Y] + right[Y]);
	}
	friend Vector2I operator*(const Vector2I &left, const Vector2I &right)
	{
		return Vector2I(left[X] * right[X], left[Y] * right[Y]);
	}
	friend Vector2I operator*(const Vector2I &myVec, float multiplier)
	{
		return Vector2I((int)(myVec[X] * multiplier), (int)(myVec[Y] * multiplier));
	}
	friend Vector2I operator*(float multiplier, const Vector2I &myVec)
	{
		return Vector2I((int)(myVec[X] * multiplier), (int)(myVec[Y] * multiplier));
	}

protected:
	int mArr[Num__Elements];
};


class Vector3I
{
public:
	enum VectorElements
	{
		X,
		Y,
		Z,
		Num__Elements,
	};
public:
	//Constructors
	Vector3I(int x = 0, int y = 0, int z = 0)
	{
		mArr[X] = x;
		mArr[Y] = y;
		mArr[Z] = z;
	}

	//Accessors
	int GetX() const
	{
		return mArr[X];
	}
	int GetY() const
	{
		return mArr[Y];
	}
	int GetZ() const
	{
		return mArr[Z];
	}

	//Mutators
	void SetX(int newX)
	{
		mArr[X] = newX;
	}
	void SetY(int newY)
	{
		mArr[Y] = newY;
	}
	void SetZ(int newZ)
	{
		mArr[Z] = newZ;
	}

	//Overloaded operators
	int &operator[](int index)
	{
		if (index < 0 || index >= (int)Num__Elements)
			return mArr[X]; //If index is out of bounds, return the first element by default.
		return mArr[index];
	}
	const int &operator[](int index) const
	{
		if (index < 0 || index >= (int)Num__Elements)
			return mArr[X]; //If index is out of bounds, return the first element by default.
		return mArr[index];
	}
	friend bool operator== (const Vector3I &coord1, const Vector3I &coord2)
	{
		return (coord1.GetX() == coord2.GetX() && coord1.GetY() == coord2.GetY() && coord1.GetZ() == coord2.GetZ());
	}

protected:
	int mArr[Num__Elements];
};


class Vector2F
{
public:
	enum VectorElements
	{
		X,
		Y,
		Num__Elements,
	};
public:
	//Constructors
	Vector2F(float x = 0, float y = 0)
	{
		mArr[X] = x;
		mArr[Y] = y;
	}

	//Accessors
	float GetX() const
	{
		return mArr[X];
	}
	float GetY() const
	{
		return mArr[Y];
	}

	//Mutators
	void SetX(float newX)
	{
		mArr[X] = newX;
	}
	void SetY(float newY)
	{
		mArr[Y] = newY;
	}

	//Overloaded operators
	float &operator[](int index)
	{
		if (index < 0 || index >= (int)Num__Elements)
			return mArr[X]; //If index is out of bounds, return the first element by default.
		return mArr[index];
	}
	const float &operator[](int index) const
	{
		if (index < 0 || index >= (int)Num__Elements)
			return mArr[X]; //If index is out of bounds, return the first element by default.
		return mArr[index];
	}
	friend bool operator== (const Vector2F &pair1, const Vector2F &pair2)
	{
		return (pair1.GetX() == pair2.GetX() && pair1.GetY() == pair2.GetY());
	}
	friend Vector2F operator+(const Vector2F &left, const Vector2F &right)
	{
		return Vector2F(left[X] + right[X], left[Y] + right[Y]);
	}
	friend Vector2F operator*(const Vector2F &left, const Vector2F &right)
	{
		return Vector2F(left[X] * right[X], left[Y] * right[Y]);
	}
	friend Vector2F operator*(const Vector2F &myVec, float multiplier)
	{
		return Vector2F(myVec[X] * multiplier, myVec[Y] * multiplier);
	}
	friend Vector2F operator*(float multiplier, const Vector2F &myVec)
	{
		return Vector2F(myVec[X] * multiplier, myVec[Y] * multiplier);
	}

protected:
	float mArr[Num__Elements];
};


class Vector3F
{
public:
	enum VectorElements
	{
		X,
		Y,
		Z,
		Num__Elements,
	};
public:
	//Constructors
	Vector3F(float x = 0, float y = 0, float z = 0)
	{
		mArr[X] = x;
		mArr[Y] = y;
		mArr[Z] = z;
	}

	//Accessors
	float GetX() const
	{
		return mArr[X];
	}
	float GetY() const
	{
		return mArr[Y];
	}
	float GetZ() const
	{
		return mArr[Z];
	}

	//Mutators
	void SetX(float newX)
	{
		mArr[X] = newX;
	}
	void SetY(float newY)
	{
		mArr[Y] = newY;
	}
	void SetZ(float newZ)
	{
		mArr[Z] = newZ;
	}

	//Overloaded operators
	float &operator[](int index)
	{
		if (index < 0 || index >= (int)Num__Elements)
			return mArr[X]; //If index is out of bounds, return the first element by default.
		return mArr[index];
	}
	const float &operator[](int index) const
	{
		if (index < 0 || index >= (int)Num__Elements)
			return mArr[X]; //If index is out of bounds, return the first element by default.
		return mArr[index];
	}
	friend bool operator== (const Vector3F &coord1, const Vector3F &coord2)
	{
		return (coord1.GetX() == coord2.GetX() && coord1.GetY() == coord2.GetY() && coord1.GetZ() == coord2.GetZ());
	}

protected:
	float mArr[Num__Elements];
};


/*Generic 2D Vector class*/
template <typename T>
class Vector2	/*2D Vector of Objects*/
{
public:
	enum VectorElements
	{
		X,
		Y,
		Num__Elements,
	};
public:
	/*Constructor(s)*/
	Vector2();
	Vector2(const T &first, const T &second);

	/*The "Big 3"*/
	Vector2(const Vector2 &copyObject); //Copy constructor
	void operator= (const Vector2 &equalObject); //Operator overloader
	~Vector2();

	/*Accessors*/
	T GetX() const;
	T GetY() const;

	/*Mutators*/
	void SetX(T newX);
	void SetY(T newY);
	
	/*Overloaded operators*/
	T &operator[](int index);
	const T &operator[](int index) const;
	/*friend bool operator==(const Vector2 &vec1, const Vector2 &vec2);*/

protected:
	T *mArr;
};

/*Generic 2D Vector implementation*/
template <typename T>
Vector2<T>::Vector2()
{
	mArr = new T[Num__Elements];
	std::cout << "Uninitialized Vector2<T> object!" << "\n";
}

template <typename T>
Vector2<T>::Vector2(const T &first, const T &second)
{
	mArr = new T[Num__Elements];
	mArr[X] = first;
	mArr[Y] = second;
}

template <typename T>
Vector2<T>::Vector2(const Vector2 &copyObject) //Copy constructor
{
	mArr = new T[Num__Elements];
	*mArr = *(copyObject[X]);
}

template <typename T>
void Vector2<T>::operator=(const Vector2 &equalObject) //Operator overloader
{
	delete[] mArr;

	mArr = new T[Num__Elements];
	*mArr = copyObject[X];
}

template <typename T>
Vector2<T>::~Vector2()
{
	delete[] mArr;
}

template <typename T>
T Vector2<T>::GetX() const
{
	return mArr[X];
}

template <typename T>
T Vector2<T>::GetY() const
{
	return mArr[Y];
}

template <typename T>
void Vector2<T>::SetX(T newX)
{
	mArr[X] = newX;
}

template <typename T>
void Vector2<T>::SetY(T newY)
{
	mArr[Y] = newY;
}

template <typename T>
T &Vector2<T>::operator[](int index)
{
	if (index < 0 || index >= (int)Num__Elements)
		return mArr[X]; //If index is out of bounds, return the first element by default.
	return mArr[index];
}

template <typename T>
const T &Vector2<T>::operator[](int index) const
{
	if (index < 0 || index >= (int)Num__Elements)
		return mArr[X]; //If index is out of bounds, return the first element by default.
	return mArr[index];
}

/*
template <typename T>
bool Vector2<T>::operator==(const Vector2 &vec1, const Vector2 &vec2)
{
	return (vec1.GetX() == vec2.GetX() && vec1.GetY() == vec2.GetY());
}
*/


class Timer
{
public:
	/*
	 * Constructor(s)
	 */
	Timer(float newDuration = 1.0f, float newElapsedTime = 0.0f, float newStartTime = 0.0f);

	/*
	* Accessors
	*/
	float GetDuration() const;
	float GetElapsedTime() const;
	float GetStartTime() const;

	/*
	* Mutators
	*/
	void SetDuration(float newDuration);
	void SetElapsedTime(float newElapsedTime);
	void SetStartTime(float newStartTime);
	void Reset();
private:
	float mDuration; /*How long to set the timer for*/
	float mElapsedTime; /*How much time has passed*/
	float mStartTime; /*What time the timer started ticking down*/
};


/*
 * Function prototypes
 */
Color4 GetRandomColor();
Color4 GetRandomCurveColor();
void SetPixel(int x, int y, const Color3 &color);
unsigned int StringToUInt(const std::string &str);
std::string UIntToString(unsigned int num);
std::string FloatToString(float num);

/*
* Global variables
*/
extern const unsigned int WINDOW_WIDTH;
extern const unsigned int WINDOW_HEIGHT;
extern const unsigned int SLEEP_DURATION;
extern const int MAGIC_GARBAGE;
extern float* pixelBuffer;




#endif /*HFILE_GENERAL*/
#ifndef HFILE_GUI
#define HFILE_GUI



#include <iostream>
#include "Rectangle.h"


/*
 * Classes
 */
class Button
{
public:
	enum ButtonState
	{
		Up,
		Down,
		Num__ButtonStates,
	};
public:
	/*
	 * Constructor(s)
	 */
	Button(void *newActionFcn = NULL);

	/*
	 * Accessors
	 */
	ButtonState GetState() const;
private:
	void ButtonFcn();
public:

	/*
	 * Mutators
	 */
	void SetState(ButtonState newState);

private:
	ButtonState mState;
	void *mActionFcn;
};


class Checkbox
{
public:
	enum HoveredState
	{
		Hovered,
		NotHovered,
		Num__HoverStates,
	};
	enum PressedState
	{
		Pressed,
		NotPressed,
		Num__PressedStates,
	};
public:
	const Color3 NORMAL_COLOR = Color3(1.0f, 1.0f, 1.0f);
	const Color3 HOVERED_COLOR = Color3(1.0f, 1.0f, 0.0f);
	const Color3 PRESSED_COLOR = Color3(0.2f, 0.7f, 0.2f);
	const Color3 HOVERED_AND_PRESSED_COLOR = Color3(0.5f * HOVERED_COLOR.GetR() + 0.5f * PRESSED_COLOR.GetR(),
													0.5f * HOVERED_COLOR.GetG() + 0.5f * PRESSED_COLOR.GetG(),
													0.5f * HOVERED_COLOR.GetB() + 0.5f * PRESSED_COLOR.GetB());
public:
	/*
	* Constructor(s)
	*/
	Checkbox(const MyRectangle &newRect = MyRectangle(), void *newCheckedFcn = NULL, void *newUncheckedFcn = NULL);

	/*
	* Accessors
	*/
	HoveredState GetHoveredState() const;
	PressedState GetPressedState() const;
	MyRectangle GetRect() const;

	/*
	* Mutators
	*/
	void SetHoveredState(HoveredState newHoveredState);
	void SetPressedState(PressedState newPressedState);

private:
	MyRectangle *mRect;
	HoveredState mHoveredState;
	PressedState mPressedState;
	void *mCheckedFcn; /*Pointer to function to call when checkbox becomes checked*/
	void *mUncheckedFcn; /*Pointer to function to call when checkbox becomes unchecked*/
};


class CheckboxHandler
{
public:
	/*
	* Constructor(s)
	*/
	CheckboxHandler();

	/*
	* Accessors
	*/
	std::vector<Checkbox*>* GetCheckboxVec();
	void MonitorCheckboxes(int mouseX, int mouseY, int button = MAGIC_GARBAGE, int action = MAGIC_GARBAGE) const;
	void DrawCheckboxes() const;

	/*
	* Mutators
	*/
	void InsertCheckbox(const Checkbox &newCheckbox);

private:
	std::vector<Checkbox*> mCheckboxVec;
};



class Slider
{
public:
	enum Orientation
	{
		Horizontal,
		Vertical,
		Num__Orientations,
	};
public:
	/*
	* Constructor(s)
	*/
	Slider(const std::string &newName,
			MyRectangle &newRect = MyRectangle(),
			const Vector2F &newRange = Vector2F(0.0f, 1.0f),
			const Vector2I &newSliderPos = Vector2I(0, 0),
			void *newSliderMoveFcn = NULL);

	/*
	* Accessors
	*/
	std::string GetName() const;
	MyRectangle GetRect() const;
	Vector2F GetRange() const;
	Orientation GetSliderOrientation() const;
	Vector2I GetSliderPosition() const;
	float GetValue() const; /*Gets value based on slider position and is relative to mRange*/
	bool GetPressedState() const;
	void CallSliderMoveFcn(float t) const;
	void Draw() const;

	/*
	* Mutators
	*/
	void SetName(const std::string &newName);
	void SetRect(const MyRectangle &newRect);
	void SetRange(const Vector2F &newRange);
	void SetSliderPosition(const Vector2I &newSliderPos);
	void SetPressedState(bool newPressedState);
	void SetSliderMoveFcn(void *newSliderMoveFcn);

private:
	std::string mName;
	MyRectangle mRect; /*Bounding rect*/
	Vector2F mRange; /*First value is beginning of range, inclusive, and second value is end of range, inclusive*/
	Orientation mSliderOrientation; /* Slider orientation--that is, whether the slider slides vertically or
									   horizontally--is along the axis of the larger of the two dimensions of
									   mRect.If mRect is a square, one of the dimensions will be made 1 pixel
									   larger.*/
	MyRectangle mSliderTrack; /*A thin rect that the slider knob "slides along"*/
	MyRectangle mSlider; /*A small rect representing the slider knob*/
	bool mIsPressed;
	void *mSliderMoveFcn; /*What action to take when the slider knob moves*/
};


class SliderHandler
{
public:
	/*
	* Constructor(s)
	*/
	SliderHandler();

	/*
	* Accessors
	*/
	std::vector<Slider*>* GetSliderVec();
	void MonitorSliders(int mouseX, int mouseY, int button = MAGIC_GARBAGE, int action = MAGIC_GARBAGE) const;
	void DrawSliders() const;

	/*
	* Mutators
	*/
	void InsertSlider(const Slider &newSlider);

private:
	std::vector<Slider*> mSliderVec;
};



/*
* Global function prototypes
*/
void InitGUI();
void DrawGUI();
Vector2F GetRasterPosition(const Vector2I &desiredPos, int strLen);
void GoToGraphMode();
void GoToCurveMode();
void SetAlgorithmTypeToBernstein();
void SetAlgorithmTypeToDeCasteljau();
void ControlPointVisibilityEnabled(); /*Sets control points visible for all curves*/
void ControlPointVisibilityDisabled(); /*Sets control points NOT visible for all curves*/
void ControlPolygonVisibiliytEnabled(); /*Sets control polygon visible for all curves*/
void ControlPolygonVisibiliytDisabled(); /*Sets control polygon NOT visible for all curves*/
void MBBVisibilityEnabled(); /*Sets MBB (as rect outline) visible for all curves*/
void MBBVisibilityDisabled(); /*Sets MBB (as rect outline) NOT visible for all curves*/
void IntersectionsVisibilityEnabled();
void IntersectionsVisibilityDisabled();


/*
 * Global variables
 */
extern MyRectangle *curveSpace; /*The screen space where the user can draw curves; everywhere
								  on the screen except where panels are and other relevent
								  GUI objects.*/
extern MyRectangle *verticalPanel;
extern CheckboxHandler *checkboxHandler;
extern Checkbox *displayGraphCheckbox;
extern Checkbox *algorithmCheckbox;
extern Checkbox *controlPointCheckbox;
extern Checkbox *controlPolygonCheckbox;
extern Checkbox *MBBCheckbox;
extern Checkbox *displayIntersectionsCheckbox;
extern SliderHandler *sliderHandler;
extern Slider *resolutionSlider;
extern Slider *intermediateLinesSlider;
extern Slider *splitCurveSlider;


#endif /*HFILE_GUI*/
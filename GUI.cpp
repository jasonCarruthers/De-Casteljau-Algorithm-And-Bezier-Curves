#include <GL/glut.h>
#include "Rectangle.h"
#include "Curves.h"
#include "GUI.h"
#include "Graph.h"
#include "Tree.h"




/*
 * Global variables
 */
MyRectangle *curveSpace;
MyRectangle *verticalPanel;
CheckboxHandler *checkboxHandler;
Checkbox *displayGraphCheckbox;
Checkbox *algorithmCheckbox;
Checkbox *controlPointCheckbox;
Checkbox *controlPolygonCheckbox;
Checkbox *MBBCheckbox;
Checkbox *displayIntersectionsCheckbox;
SliderHandler *sliderHandler;
Slider *resolutionSlider;
Slider *intermediateLinesSlider;
Slider *splitCurveSlider;



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Button
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Button::Button(void* newActionFcn)
{
	mState = Button::Up;
	mActionFcn = newActionFcn;
}

/*
* Accessors
*/
Button::ButtonState Button::GetState() const
{
	return mState;
}

void Button::ButtonFcn()
{
	mActionFcn;
}

/*
* Mutators
*/
void Button::SetState(Button::ButtonState newState)
{
	/*If the button was up and is now being pressed down, perform some action by calling
	  ButtonFcn(), which calls the function pointed to by mActionFcn.*/
	if (mState == Button::Up && newState == Button::Down)
		ButtonFcn();

	/*Change the button state*/
	mState = newState;
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Checkbox
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Checkbox::Checkbox(const MyRectangle &newRect, void *newCheckedFcn, void *newUncheckedFcn)
{
	mRect = new MyRectangle(newRect);
	mCheckedFcn = newCheckedFcn;
	mUncheckedFcn = newUncheckedFcn;
	mHoveredState = Checkbox::NotHovered;
	mPressedState = Checkbox::NotPressed;
}

/*
* Accessors
*/
Checkbox::HoveredState Checkbox::GetHoveredState() const
{
	return mHoveredState;
}

Checkbox::PressedState Checkbox::GetPressedState() const
{
	return mPressedState;
}

MyRectangle Checkbox::GetRect() const
{
	return *mRect;
}


/*
* Mutators
*/
void Checkbox::SetHoveredState(Checkbox::HoveredState newHoveredState)
{
	mHoveredState = newHoveredState;

	/*If checkbox is being hovered over and it's not already pressed,
	  set it to Checkbox::HOVERED_COLOR.*/
	if (mPressedState == NotPressed && mHoveredState == Hovered)
		mRect->SetColor(Checkbox::HOVERED_COLOR);

	/*If checkbox is not being hovered over, set checkbox color to Checkbox::NORMAL_COLOR.*/
	else if (mPressedState == NotPressed && mHoveredState == NotHovered)
		mRect->SetColor(Checkbox::NORMAL_COLOR);

	/*If checkbox is in its pressed state and is being hovered over, set its
	  color to Checkbox::HOVERED_AND_PRESSED_COLOR*/
	else if (mPressedState == Pressed && mHoveredState == Hovered)
		mRect->SetColor(Checkbox::HOVERED_AND_PRESSED_COLOR);

	/*If checkbox is in its pressed state and is not being hovered over, set its
	  color to Checkbox::PRESSED_COLOR*/
	else if (mPressedState == Pressed && mHoveredState == NotHovered)
		mRect->SetColor(Checkbox::PRESSED_COLOR);
}

void Checkbox::SetPressedState(Checkbox::PressedState newPressedState)
{
	mPressedState = newPressedState;

	/*If checkbox is being pressed, set checkbox color to Checkbox::PRESSED_COLOR and
	  also call this checkbox's callback function for checkbox becoming checked.*/
	if (mPressedState == Pressed)
	{
		mRect->SetColor(Checkbox::PRESSED_COLOR);
		((void(*)())mCheckedFcn)();
	}

	/*If checkbox is not being pressed, set checkbox color to Checkbox::HOVERED_COLOR since the checkbox
	  must still be hovered over because the user has to mouse hover in order to toggle
	  the checkbox to mPressedState=NotPressed. Also, call this checkbox's callback function
	  for checkbox becoming unchecked.*/
	else if (mPressedState == NotPressed)
	{
		mRect->SetColor(Checkbox::HOVERED_COLOR);
		((void(*)())mUncheckedFcn)();
	}
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class CheckboxHandler
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
CheckboxHandler::CheckboxHandler()
{
}

/*
* Accessors
*/
std::vector<Checkbox*>* CheckboxHandler::GetCheckboxVec()
{
	return &mCheckboxVec;
}

void CheckboxHandler::MonitorCheckboxes(int mouseX, int mouseY, int button, int action) const
{
	for (unsigned int i = 0; i < mCheckboxVec.size(); i++)
	{
		/*Update based on mouse move...*/
		if (button == MAGIC_GARBAGE && action == MAGIC_GARBAGE)
		{
			/*Update hover state appropriately*/
			if (mCheckboxVec[i]->GetRect().ContainsPoint(mouseX, WINDOW_HEIGHT - mouseY))
				mCheckboxVec[i]->SetHoveredState(Checkbox::Hovered);
			else if (mCheckboxVec[i]->GetHoveredState() == Checkbox::Hovered)
				mCheckboxVec[i]->SetHoveredState(Checkbox::NotHovered);
		}

		/*Update based on mouse click...*/
		else
		{
			/*Update pressed state appropriately*/
			if (button == GLUT_LEFT_BUTTON && action == GLUT_UP &&
				mCheckboxVec[i]->GetHoveredState() == Checkbox::Hovered &&
				mCheckboxVec[i]->GetPressedState() == Checkbox::Pressed)
				mCheckboxVec[i]->SetPressedState(Checkbox::NotPressed);
			else if (button == GLUT_LEFT_BUTTON && action == GLUT_UP &&
				mCheckboxVec[i]->GetHoveredState() == Checkbox::Hovered)
				mCheckboxVec[i]->SetPressedState(Checkbox::Pressed);
		}
	}
}

void CheckboxHandler::DrawCheckboxes() const
{
	for (unsigned int i = 0; i < mCheckboxVec.size(); i++)
	{
		mCheckboxVec[i]->GetRect().Draw();
	}
}

/*
* Mutators
*/
void CheckboxHandler::InsertCheckbox(const Checkbox &newCheckbox)
{
	Checkbox *temp = new Checkbox(newCheckbox);
	mCheckboxVec.push_back(temp);
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Slider
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Slider::Slider(const std::string &newName, MyRectangle &newRect, const Vector2F &newRange,
				const Vector2I &newSliderPos, void *newSliderMoveFcn)
{
	/*Set name*/
	mName = newName;

	/*Set bounding rect*/
	mRect.Set(newRect);
	if (mRect.GetDimensions().GetX() == mRect.GetDimensions().GetY())
		mRect.SetDimensions(mRect.GetDimensions().GetX(), mRect.GetDimensions().GetY() + 1);
	
	/*Set orientation*/
	mSliderOrientation = (mRect.GetDimensions().GetX() < mRect.GetDimensions().GetY()) ? Slider::Vertical : Slider::Horizontal;

	/*Set range*/
	mRange.SetX(newRange.GetX());
	mRange.SetY(newRange.GetY());

	/*Set slider knob position*/
	mSlider.SetCenter(newSliderPos);
	mSlider.Set(MyRectangle(newSliderPos, Vector2I(40, 40), Color3(0.0f, 0.0f, 0.0f)));

	/*Set track rect*/
	Vector2I trackDimensions = (mSliderOrientation == Slider::Vertical) ?
								Vector2I(4, mRect.GetDimensions().GetY()) :
								Vector2I(mRect.GetDimensions().GetX(), 4);
	mSliderTrack.SetDimensions(trackDimensions);
	mSliderTrack.SetCenter(mRect.GetCenter());
	mSliderTrack.SetColor(0.0f, 0.0f, 0.0f);

	/*Set slider move function (what action to take when the slider knob moves)*/
	mSliderMoveFcn = newSliderMoveFcn;

	/*Set pressed state to false*/
	mIsPressed = false;
}

/*
* Accessors
*/
std::string Slider::GetName() const
{
	return mName;
}

MyRectangle Slider::GetRect() const
{
	return mRect;
}

Vector2F Slider::GetRange() const
{
	return mRange;
}

Slider::Orientation Slider::GetSliderOrientation() const
{
	return mSliderOrientation;
}

Vector2I Slider::GetSliderPosition() const
{
	return mSlider.GetCenter();
}

/*Gets value based on slider position and is relative to mRange*/
float Slider::GetValue() const
{
	float retVal = mRange.GetX();
	retVal += ((mSliderOrientation == Slider::Vertical) ?
		(float)(mSlider.GetCenter().GetY() - (mRect.GetTopLeft().GetY() - mRect.GetDimensions().GetY())) / mRect.GetDimensions().GetY() :
		(float)(mSlider.GetCenter().GetX() - mRect.GetTopLeft().GetX()) / mRect.GetDimensions().GetX()) *
		(mRange.GetY() - mRange.GetX());

	return retVal;
}

bool Slider::GetPressedState() const
{
	return mIsPressed;
}

void Slider::CallSliderMoveFcn(float t) const
{
	((void(*)(float))mSliderMoveFcn)(t);
}

void Slider::Draw() const
{
	/*
	* ----------------------------------------------------------------------------------------
	* Draw text. Text will always draw on top of everything else, so it's ok to draw it first.
	* NOTE: OpenGL positions text using glRasterPos(...), which takes an x and a y value each
	*		 in the range of [-1, 1], relative to center of screen with origin at bottom left
	*		 corner of screen.
	* ----------------------------------------------------------------------------------------
	*/
	glColor3f(1.0f, 1.0f, 1.0f); /*Set text color*/
	Vector2F rasterPos;
	std::string str;
	/*Draw slider name text*/
	str = mName;
	rasterPos = GetRasterPosition(Vector2I(mRect.GetTopLeft().GetX() + mRect.GetDimensions().GetX() / 2 - 30, mRect.GetTopLeft().GetY() + 50), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Draw end of range text*/
	str = FloatToString(mRange.GetY());
	rasterPos = GetRasterPosition(Vector2I(mRect.GetTopLeft().GetX() + mRect.GetDimensions().GetX() / 2 - 5, mRect.GetTopLeft().GetY() + 25), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Draw beginning of range text*/
	str = FloatToString(mRange.GetX());
	rasterPos = GetRasterPosition(Vector2I(mRect.GetTopLeft().GetX() + mRect.GetDimensions().GetX() / 2 - 5, mRect.GetTopLeft().GetY() - mRect.GetDimensions().GetY() - 25), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Draw t value text*/
	str = FloatToString(GetValue());
	rasterPos = GetRasterPosition(Vector2I(mSlider.GetCenter().GetX() - 10, mSlider.GetCenter().GetY()), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Reposition view frustrum at the "normal" spot.
	NOTE: Do this after displaying any text.*/
	glRasterPos2f(-1, -1);
	glutPostRedisplay();
	/*
	* ----------------------------------------------------------------------------------------
	* End drawing text
	* ----------------------------------------------------------------------------------------
	*/

	/*Draw the rest of the slider components*/
	mRect.Draw();
	mSliderTrack.Draw();
	mSlider.Draw();
}

/*
* Mutators
*/
void Slider::SetName(const std::string &newName)
{
	mName = newName;
}

void Slider::SetRect(const MyRectangle &newRect)
{
	mRect.Set(newRect);
}

void Slider::SetRange(const Vector2F &newRange)
{
	mRange.SetX(newRange.GetX());
	mRange.SetY(newRange.GetY());
}

void Slider::SetSliderPosition(const Vector2I &newSliderPos)
{
	mSlider.SetCenter(newSliderPos);
}

void Slider::SetPressedState(bool newPressedState)
{
	mIsPressed = newPressedState;
}

void Slider::SetSliderMoveFcn(void *newSliderMoveFcn)
{
	mSliderMoveFcn = newSliderMoveFcn;
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class SliderHandler
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
SliderHandler::SliderHandler()
{
}

/*
* Accessors
*/
std::vector<Slider*>* SliderHandler::GetSliderVec()
{
	return &mSliderVec;
}

void SliderHandler::MonitorSliders(int mouseX, int mouseY, int button, int action) const
{
	/*The mouseY is relative to the top left corner of the screen, so first make it
	  relative to the bottom left corner of the screen*/
	mouseY = WINDOW_HEIGHT - mouseY;

	for (unsigned int i = 0; i < mSliderVec.size(); i++)
	{
		/*Update based on mouse move...*/
		if (button == MAGIC_GARBAGE && action == MAGIC_GARBAGE)
		{
			/*If this slider is being pressed, move the slider knob based to the 
			  x or y component of the mouse (bounded by the slider's bounding rect);
			  the chosen component is based on this slider's orientation.*/
			if (mSliderVec[i]->GetRect().ContainsPoint(mouseX, mouseY) &&
				mSliderVec[i]->GetPressedState() == true)
			{
				/*Get slider-mouse position delta*/
				int deltaX = (mSliderVec[i]->GetSliderOrientation() == Slider::Vertical) ?
								0 : mouseX - mSliderVec[i]->GetRect().GetCenter().GetX();
				int deltaY = (mSliderVec[i]->GetSliderOrientation() == Slider::Vertical) ?
					mouseY - mSliderVec[i]->GetRect().GetCenter().GetY() : 0;
				
				/*Bound the slider knob to stay within the slider bounding rect*/
				if (mSliderVec[i]->GetRect().GetCenter().GetX() + deltaX < mSliderVec[i]->GetRect().GetTopLeft().GetX())
					deltaX = mSliderVec[i]->GetRect().GetCenter().GetX() - mSliderVec[i]->GetRect().GetTopLeft().GetX();
				else if (mSliderVec[i]->GetRect().GetCenter().GetX() + deltaX > mSliderVec[i]->GetRect().GetTopLeft().GetX() + mSliderVec[i]->GetRect().GetDimensions().GetX())
					deltaX = (mSliderVec[i]->GetRect().GetTopLeft().GetX() + mSliderVec[i]->GetRect().GetDimensions().GetX()) - mSliderVec[i]->GetRect().GetCenter().GetX();
				else if (mSliderVec[i]->GetRect().GetCenter().GetY() + deltaY > mSliderVec[i]->GetRect().GetTopLeft().GetY())
					deltaY = mSliderVec[i]->GetRect().GetTopLeft().GetY() - mSliderVec[i]->GetRect().GetCenter().GetY();
				else if (mSliderVec[i]->GetRect().GetCenter().GetY() + deltaY < mSliderVec[i]->GetRect().GetTopLeft().GetY() - mSliderVec[i]->GetRect().GetDimensions().GetY())
					deltaY = mSliderVec[i]->GetRect().GetCenter().GetY() - (mSliderVec[i]->GetRect().GetTopLeft().GetY() - mSliderVec[i]->GetRect().GetDimensions().GetY());

				/*Change slider knob position*/
				Vector2I newSliderKnobPos = Vector2I(mSliderVec[i]->GetRect().GetCenter().GetX() + deltaX, mSliderVec[i]->GetRect().GetCenter().GetY() + deltaY);
				mSliderVec[i]->SetSliderPosition(newSliderKnobPos);

				/*Call callback function for when the slider knob moves*/
				mSliderVec[i]->CallSliderMoveFcn(mSliderVec[i]->GetValue());
			}
		}

		/*Update based on mouse click...*/
		else
		{
			/*As long as the mouse is over this slider's knob and the left mouse button is being
			  pressed, then this slider is in its mIsPressed=true state*/
			if (mSliderVec[i]->GetRect().ContainsPoint(mouseX, mouseY) &&
				button == GLUT_LEFT_BUTTON && action == GLUT_DOWN)
				mSliderVec[i]->SetPressedState(true);
			
			/*Otherwise, this slider is in its mIsPressed=false state*/
			else
				mSliderVec[i]->SetPressedState(true);
		}
	}
}

void SliderHandler::DrawSliders() const
{
	for (unsigned int i = 0; i < mSliderVec.size(); i++)
		mSliderVec[i]->Draw();
}

/*
* Mutators
*/
void SliderHandler::InsertSlider(const Slider &newSlider)
{
	Slider *temp = new Slider(newSlider);
	mSliderVec.push_back(temp);
}




/*
 * Global functions
 */
void InitGUI()
{
	/*Init panels*/
	verticalPanel = new MyRectangle(Vector2I(0, WINDOW_HEIGHT), Vector2I(300, WINDOW_HEIGHT),
									Color3(0.4f, 0.4f, 0.4f));

	/*Init curve space*/
	curveSpace = new MyRectangle(Vector2I(verticalPanel->GetWidth(), WINDOW_HEIGHT), Vector2I(WINDOW_WIDTH - verticalPanel->GetWidth(), WINDOW_HEIGHT), Color3(0.0f, 0.0f, 0.0f));

	/*Init checkboxes*/
	displayGraphCheckbox = new Checkbox(MyRectangle(Vector2I(20, 180), Vector2I(20, 20), Color3(1.0f, 1.0f, 1.0f)), GoToGraphMode, GoToCurveMode);
	algorithmCheckbox = new Checkbox(MyRectangle(Vector2I(20, 150), Vector2I(20, 20), Color3(1.0f, 1.0f, 1.0f)), SetAlgorithmTypeToBernstein, SetAlgorithmTypeToDeCasteljau);
	controlPointCheckbox = new Checkbox(MyRectangle(Vector2I(20, 120), Vector2I(20, 20), Color3(1.0f, 1.0f, 1.0f)), ControlPointVisibilityEnabled, ControlPointVisibilityDisabled);
	controlPolygonCheckbox = new Checkbox(MyRectangle(Vector2I(20, 90), Vector2I(20, 20), Color3(1.0f, 1.0f, 1.0f)), ControlPolygonVisibiliytEnabled, ControlPolygonVisibiliytDisabled);
	MBBCheckbox = new Checkbox(MyRectangle(Vector2I(20, 60), Vector2I(20, 20), Color3(1.0f, 1.0f, 1.0f)), MBBVisibilityEnabled, MBBVisibilityDisabled);
	displayIntersectionsCheckbox = new Checkbox(MyRectangle(Vector2I(20, 30), Vector2I(20, 20), Color3(1.0f, 1.0f, 1.0f)), IntersectionsVisibilityEnabled, IntersectionsVisibilityDisabled);

	/*Init checkbox handler*/
	checkboxHandler = new CheckboxHandler();
	checkboxHandler->InsertCheckbox(*displayGraphCheckbox);
	checkboxHandler->InsertCheckbox(*algorithmCheckbox);
	checkboxHandler->InsertCheckbox(*controlPointCheckbox);
	checkboxHandler->InsertCheckbox(*controlPolygonCheckbox);
	checkboxHandler->InsertCheckbox(*MBBCheckbox);
	checkboxHandler->InsertCheckbox(*displayIntersectionsCheckbox);

	/*Init sliders*/
	resolutionSlider = new Slider("Resolution", MyRectangle(Vector2I(20, WINDOW_HEIGHT - 75), Vector2I(60, 300), Color3(0.3f, 0.5f, 1.0f)), Vector2F(10.0f, 500.0f));
	resolutionSlider->SetSliderPosition(resolutionSlider->GetRect().GetCenter());
	resolutionSlider->SetSliderMoveFcn(SetCurveResolution);

	intermediateLinesSlider = new Slider("Construction", MyRectangle(Vector2I(100, WINDOW_HEIGHT - 75), Vector2I(60, 300), Color3(0.3f, 0.5f, 1.0f)), Vector2F(0.0f, 1.0f));
	intermediateLinesSlider->SetSliderPosition(intermediateLinesSlider->GetRect().GetCenter());
	intermediateLinesSlider->SetSliderMoveFcn(SetAndDrawIntermediateLines);

	splitCurveSlider = new Slider("Splitting", MyRectangle(Vector2I(180, WINDOW_HEIGHT - 75), Vector2I(60, 300), Color3(0.3f, 0.5f, 1.0f)), Vector2F(0.0f, 1.0f));
	splitCurveSlider->SetSliderPosition(splitCurveSlider->GetRect().GetCenter());
	splitCurveSlider->SetSliderMoveFcn(SetSplitValue);

	/*Init slider handler*/
	sliderHandler = new SliderHandler();
	sliderHandler->InsertSlider(*resolutionSlider);
	sliderHandler->InsertSlider(*intermediateLinesSlider);
	sliderHandler->InsertSlider(*splitCurveSlider);

	/*Init graph*/
	InitGraph();
}

void DrawGUI()
{
	/*
	 * ----------------------------------------------------------------------------------------
	 * Draw text. Text will always draw on top of everything else, so it's ok to draw it first.
	 * NOTE: OpenGL positions text using glRasterPos(...), which takes an x and a y value each
	 *		 in the range of [-1, 1], relative to center of screen with origin at bottom left
	 *		 corner of screen.
	 * ----------------------------------------------------------------------------------------
	 */
	glColor3f(0.0, 1.0, 0.0); /*Set text color*/

	Vector2F rasterPos;
	std::string str;
	/*
	* displayGraphCheckbox
	*/
	str = "Display Graph";
	rasterPos = GetRasterPosition(Vector2I(50, 170), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*
	* algorithmCheckbox
	*/
	str = "Bernstein Bezier";
	rasterPos = GetRasterPosition(Vector2I(50, 140), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*
	* controlPointCheckbox
	*/
	str = "Control Points";
	rasterPos = GetRasterPosition(Vector2I(50, 110), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*
	 * controlPolygonCheckbox
	 */
	str = "Control Polygon";
	rasterPos = GetRasterPosition(Vector2I(50, 80), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*
	* MBBCheckbox
	*/
	str = "MBB";
	rasterPos = GetRasterPosition(Vector2I(50, 50), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());	
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*
	* displayIntersectionsCheckbox
	*/
	str = "Display Intersections";
	rasterPos = GetRasterPosition(Vector2I(50, 20), str.size());
	glRasterPos2f(rasterPos.GetX(), rasterPos.GetY());
	for (unsigned int i = 0; i < str.size(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, str[i]);

	/*Reposition view frustrum at the "normal" spot.
	  NOTE: Do this after displaying any text.*/
	glRasterPos2f(-1, -1);
	/*
	 * ----------------------------------------------------------------------------------------
	 * End drawing text
	 * ----------------------------------------------------------------------------------------
	 */


	/*Draw vertical panel(s)*/
	verticalPanel->Draw();

	/*Draw checkbox(es) monitored by the checkboxHandler*/
	checkboxHandler->DrawCheckboxes();
	
	/*Draw slider(s) monitored by the sliderHandler*/
	sliderHandler->DrawSliders();
}

/*Takes a desired position and converts it to the actual position where the
 *text should be drawn; text is drawn relative to its center, and with varying
 *string lengths this can be annoying.
 *
 *NOTE: Text is positioned with the glRasterPos(...) function, so the transformation
 *		from desiredPos to actualPos vector will take that into consideration.
 *
 *NOTE: Position vectors are relative to the bottom left corner of
 *		the screen, with positive y up. Also, the desiredPos position vector
 *		is relative to the left edge of the text, centered vertically. Since
 *		OpenGL draws text relative to the center of the string, this function
 *		transforms what the desired vector position of the left-center edge of
 *		text is into what the actual center-center vector position of the text
 *		should be.
 */
Vector2F GetRasterPosition(const Vector2I &desiredPos, int strLen)
{
	const int CHAR_WIDTH = 12; /*Assumes GLUT_BITMAP_TIMES_ROMAN_10 font*/
	const int CHAR_HEIGHT = 7; /*Assumes GLUT_BITMAP_TIMES_ROMAN_10 font*/

	Vector2F retVal;
	retVal = Vector2F(-1.0f + (1.0f / (WINDOW_WIDTH/2)) * (desiredPos.GetX()/* + CHAR_WIDTH * strLen/2.0f*/),
				      -1.0f + (1.0f / (WINDOW_HEIGHT/2)) * (desiredPos.GetY() - CHAR_HEIGHT/2.0f));
	return retVal;
}

void GoToGraphMode()
{
	/*Clear screen*/
	for (unsigned int y = 0; y < WINDOW_HEIGHT; y++)
		for (unsigned int x = 0; x < WINDOW_WIDTH; x++)
			SetPixel(x, y, Color3(0.0f, 0.0f, 0.0f));

	/*Calculate algorithm times. The curves that are being computed should
	  display on the screen briefly as they're made and immediately cleared
	  for the next curve to be ran.*/
	RunCurveTimeTrials();

	/*Set graph mode on*/
	graphModeIsOn = true;
}

void GoToCurveMode()
{
	/*Clear screen*/
	for (unsigned int y = 0; y < WINDOW_HEIGHT; y++)
		for (unsigned int x = 0; x < WINDOW_WIDTH; x++)
			SetPixel(x, y, Color3(0.0f, 0.0f, 0.0f));

	/*Set graph mode off*/
	graphModeIsOn = false;

	/*Display all previously created curves*/
	for (unsigned int i = 0; i < curveVec.size(); i++)
	{
		curveVec[i]->CalcPlotVec();
		HandleCurveDraw(i);
	}
}

void SetAlgorithmTypeToBernstein()
{
	unsigned int vecSize = curveVec.size();
	for (unsigned int i = 0; i < vecSize; i++)
	{
		Curve *newBernsteinCurve = new BernsteinCurve(curveVec[i]->GetColor());

		/*Clear old curve onscreen and any intermediate lines that may be onscreen*/
		if (!graphModeIsOn)
		{
			curveVec[i]->Clear();
			curveVec[i]->ClearIntermediateLines();
		}

		/*Set control points*/
		std::vector<Vector2I*> controlPointVec = (*(curveVec[i]->GetControlPointVec()));
		for (unsigned int j = 0; j < controlPointVec.size(); j++)
		{
			newBernsteinCurve->InsertControlPoint(*controlPointVec[j]);
		}

		/*Set flags*/
		newBernsteinCurve->SetControlPointVisibility(curveVec[i]->GetControlPointVisibility());
		newBernsteinCurve->SetControlPolygonVisibility(curveVec[i]->GetControlPolygonVisibility());
		newBernsteinCurve->SetMBBVisibility(curveVec[i]->GetMBBVisibility());

		/*Clean up*/
		curveVec.erase(curveVec.begin() + i);
		curveVec.insert(curveVec.begin() + i, newBernsteinCurve);
		newBernsteinCurve = NULL;

		/*Force redraw*/
		if (!graphModeIsOn)
			HandleCurveDraw(i);
	}
}

void SetAlgorithmTypeToDeCasteljau()
{
	unsigned int vecSize = curveVec.size();
	for (unsigned int i = 0; i < vecSize; i++)
	{
		DeCasteljauCurve *newDeCasteljauCurve = new DeCasteljauCurve(curveVec[i]->GetColor());

		/*Clear old curve onscreen and any intermediate lines that may be onscreen*/
		if (!graphModeIsOn)
		{
			curveVec[i]->Clear();
			curveVec[i]->ClearIntermediateLines();
		}

		/*Set control points*/
		std::vector<Vector2I*> controlPointVec = (*(curveVec[i]->GetControlPointVec()));
		for (unsigned int j = 0; j < controlPointVec.size(); j++)
		{
			newDeCasteljauCurve->InsertControlPoint(*controlPointVec[j]);
		}

		/*Set flags*/
		newDeCasteljauCurve->SetControlPointVisibility(curveVec[i]->GetControlPointVisibility());
		newDeCasteljauCurve->SetControlPolygonVisibility(curveVec[i]->GetControlPolygonVisibility());
		newDeCasteljauCurve->SetMBBVisibility(curveVec[i]->GetMBBVisibility());

		/*Clean up*/
		curveVec.erase(curveVec.begin() + i);
		curveVec.insert(curveVec.begin() + i, newDeCasteljauCurve);
		newDeCasteljauCurve = NULL;

		/*Force redraw*/
		if ( !graphModeIsOn )
			HandleCurveDraw(i);
	}
}

/*Sets control points visible for all curves*/
void ControlPointVisibilityEnabled()
{
	controlPointsVisible = true;

	for (unsigned int i = 0; i < curveVec.size(); i++)
		curveVec[i]->SetControlPointVisibility(true);

	if (!graphModeIsOn)
		for (unsigned int i = 0; i < curveVec.size(); i++)
			HandleCurveDraw(i);
}

/*Sets control points NOT visible for all curves*/
void ControlPointVisibilityDisabled()
{
	controlPointsVisible = false;

	for (unsigned int i = 0; i < curveVec.size(); i++)
		curveVec[i]->SetControlPointVisibility(false);

	if (!graphModeIsOn)
		for (unsigned int i = 0; i < curveVec.size(); i++)
			HandleCurveDraw(i);
}

/*Sets control polygon visible for all curves*/
void ControlPolygonVisibiliytEnabled()
{
	controlPolygonVisible = true;

	for (unsigned int i = 0; i < curveVec.size(); i++)
		curveVec[i]->SetControlPolygonVisibility(true);

	if (!graphModeIsOn)
		for (unsigned int i = 0; i < curveVec.size(); i++)
			HandleCurveDraw(i);
}

/*Sets control polygon NOT visible for all curves*/
void ControlPolygonVisibiliytDisabled()
{
	controlPolygonVisible = false;

	for (unsigned int i = 0; i < curveVec.size(); i++)
		curveVec[i]->SetControlPolygonVisibility(false);

	if (!graphModeIsOn)
		for (unsigned int i = 0; i < curveVec.size(); i++)
			HandleCurveDraw(i);
}

/*Sets MBB (as rect outline) visible for all curves*/
void MBBVisibilityEnabled()
{
	MBBVisible = true;

	for (unsigned int i = 0; i < curveVec.size(); i++)
		curveVec[i]->SetMBBVisibility(true);

	if ( !graphModeIsOn )
		for (unsigned int i = 0; i < curveVec.size(); i++)
			HandleCurveDraw(i);
}

/*Sets MBB (as rect outline) NOT visible for all curves*/
void MBBVisibilityDisabled()
{
	MBBVisible = false;

	for (unsigned int i = 0; i < curveVec.size(); i++)
		curveVec[i]->SetMBBVisibility(false);
	
	if ( !graphModeIsOn )
		for (unsigned int i = 0; i < curveVec.size(); i++)
			HandleCurveDraw(i);
}

void IntersectionsVisibilityEnabled()
{
	/*
	* Find intersections
	*/
	/*For each curve, get the intersections between that curve and each other curve.*/
	for (unsigned int i = 0; i < curveVec.size(); i++)
	{
		for (unsigned int j = i + 1; j < curveVec.size(); j++)
		{
			/*Get the intersections of both curves.*/
			GetIntersections(curveVec[i], curveVec[j]);
		}
	}

	/*Clear screen*/
	for (unsigned int y = 0; y < WINDOW_HEIGHT; y++)
		for (unsigned int x = 0; x < WINDOW_WIDTH; x++)
			SetPixel(x, y, Color3(0.0f, 0.0f, 0.0f));

	/*Force redraw.*/
	for (unsigned int i = 0; i < curveVec.size(); i++)
	{
		HandleCurveDraw(i);
	}

	/*Display intersections*/
	for (unsigned int i = 0; i < intersectionsVec.size(); i++)
	{
		MyRectangle rect = MyRectangle();
		rect.SetDimensions(Vector2I(11, 11));
		rect.SetCenter(*(intersectionsVec[i]));
		rect.DrawOutline(Color3(1.0f, 1.0f, 1.0f));
	}
}

void IntersectionsVisibilityDisabled()
{
	/*Clear screen*/
	for (unsigned int y = 0; y < WINDOW_HEIGHT; y++)
		for (unsigned int x = 0; x < WINDOW_WIDTH; x++)
			SetPixel(x, y, Color3(0.0f, 0.0f, 0.0f));

	/*Clear the intersectionsVec*/
	intersectionsVec.clear();

	/*Force redraw of all curves*/
	for (unsigned int i = 0; i < curveVec.size(); i++)
		curveVec[i]->Draw();
}
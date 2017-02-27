#include <vector>
#include "Main.h"
#include "MyMath.h"
#include "Line.h"
#include "Curves.h"
#include "GUI.h"
#include "Graph.h"
#include "Tree.h"


/*
*Global variables
*/
int curveIndex;
std::vector<Curve*> curveVec;
unsigned int curveResolution;
float splitT;
bool isUsingBernstein;
bool controlPointsVisible; /*True if the controlPointCheckbox is checked*/
bool controlPolygonVisible; /*True if the controlPolygonCheckbox is checked*/
bool MBBVisible; /*True if the MBBCheckbox is checked*/
std::vector<Vector2I*> intersectionsVec;





/* 
 * -------------------------------------------------------------------------------------------------------
 * Implementation of class ControlPoints
 * -------------------------------------------------------------------------------------------------------
 */
/*
* Constructor(s)
*/
ControlPoints::ControlPoints(const Color3 &newColor)
{
	mColor.Set(newColor);
}

/*
* Accessors
*/
std::vector<Vector2I*>* ControlPoints::GetControlPointVec()
{
	return &mCPVec;
}

Color3 ControlPoints::GetColor() const
{
	return mColor;
}

MyRectangle ControlPoints::GetMBB() const
{
	if (mCPVec.size() == 0)
		return MyRectangle();

	int leftMostX;
	int rightMostX;
	int bottomMostY;
	int topMostY;


	/*Find the MBB x and y values.*/
	for (unsigned int i = 0; i < mCPVec.size(); i++)
	{
		if (i == 0)
		{
			leftMostX = mCPVec[i]->GetX();
			rightMostX = mCPVec[i]->GetX();
			bottomMostY = mCPVec[i]->GetY();
			topMostY = mCPVec[i]->GetY();
		}
		else
		{
			if (mCPVec[i]->GetX() < leftMostX)
				leftMostX = mCPVec[i]->GetX();
			else if (mCPVec[i]->GetX() > rightMostX)
				rightMostX = mCPVec[i]->GetX();

			if (mCPVec[i]->GetY() < bottomMostY)
				bottomMostY = mCPVec[i]->GetY();
			else if (mCPVec[i]->GetY() > topMostY)
				topMostY = mCPVec[i]->GetY();
		}
	}

	/*Create the MBB, then return it.*/
	MyRectangle *mbb = new MyRectangle(Vector2I(leftMostX, topMostY), Vector2I(rightMostX - leftMostX, topMostY - bottomMostY), mColor);
	return *mbb;
}

void ControlPoints::Draw() const
{
	for (unsigned int controlPoint = 0; controlPoint < mCPVec.size(); controlPoint++)
	{
		for (int relativeY = -5; relativeY <= 5; relativeY++)
			for (int relativeX = -5; relativeX <= 5; relativeX++)
				SetPixel(mCPVec[controlPoint]->GetX() + relativeX, mCPVec[controlPoint]->GetY() + relativeY, mColor);
	}
}

/*Connect all adjacent control points*/
void ControlPoints::DrawControlPolygon() const
{
	for (unsigned int controlPoint = 1; controlPoint < mCPVec.size(); controlPoint++)
	{
		Line *tempLine = new Line(*mCPVec[controlPoint - 1], *mCPVec[controlPoint], Color3(1.0f, 1.0f, 1.0f));
		tempLine->Draw();
	}
}

/*Draw convex hull*/
void ControlPoints::DrawConvexHull() const
{

}

/*Draw Minimum Bounding Box*/
void ControlPoints::DrawMBB() const
{
	GetMBB().DrawOutline();
}

/*Erase all control point pixels from pixel buffer*/
void ControlPoints::Clear() const
{
	for (unsigned int controlPoint = 0; controlPoint < mCPVec.size(); controlPoint++)
	{
		for (int relativeY = -5; relativeY <= 5; relativeY++)
			for (int relativeX = -5; relativeX <= 5; relativeX++)
				SetPixel(mCPVec[controlPoint]->GetX() + relativeX, mCPVec[controlPoint]->GetY() + relativeY, Color3(0.0f, 0.0f, 0.0f));
	}
}

/*Erase the given control point's pixels from pixel buffer.*/
void ControlPoints::ClearControlPoint(int index) const
{
	for (int relativeY = -5; relativeY <= 5; relativeY++)
		for (int relativeX = -5; relativeX <= 5; relativeX++)
			SetPixel(mCPVec[index]->GetX() + relativeX, mCPVec[index]->GetY() + relativeY, Color3(0.0f, 0.0f, 0.0f));
}

/*Erase control polygon pixels from pixel buffer*/
void ControlPoints::ClearControlPolygon() const
{
	for (unsigned int controlPoint = 1; controlPoint < mCPVec.size(); controlPoint++)
	{
		Line *tempLine = new Line(*mCPVec[controlPoint - 1], *mCPVec[controlPoint], Color3(0.0f, 0.0f, 0.0f));
		tempLine->Draw();
	}
}

/*Erase convex hull pixels from pixel buffer*/
void ControlPoints::ClearConvexHull() const
{

}

/*Erase MBB pixels from pixel buffer*/
void ControlPoints::ClearMBB() const
{
	GetMBB().DrawOutline(Color3(0.0f, 0.0f, 0.0f));
}


/*
* Mutators
*/
void ControlPoints::SetColor(const Color3 &newColor)
{
	mColor.Set(newColor);
}

void ControlPoints::PushBack(const Vector2I &newControlPoint)
{
	Vector2I *cpPtr = new Vector2I(newControlPoint);
	mCPVec.push_back(cpPtr);
}

void ControlPoints::EraseAll()
{
	Clear();
	mCPVec.clear();
}

void ControlPoints::Erase(int index)
{
	if (index < 0 || (unsigned int)index >= mCPVec.size())
		return;

	ClearControlPoint(index);
	mCPVec.erase(mCPVec.begin() + index);
}


/*
* Overloaded operators
*/
Vector2I& ControlPoints::operator[](int index)
{
	if (index < 0 || index >= (int)mCPVec.size())
		return *mCPVec[0]; /*If index is out of bounds, return the first element by default.*/
	return *mCPVec[index];
}
const Vector2I& ControlPoints::operator[](int index) const
{
	if (index < 0 || index >= (int)mCPVec.size())
		return *mCPVec[0]; /*If index is out of bounds, return the first element by default.*/
	return *mCPVec[index];
}






/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Curve
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
Curve::Curve(const Color3 &newColor)
{
	mColor.Set(newColor);
	mControlPoints.SetColor(mColor);

	mControlPointVisible = false;
	mControlPolygonVisible = false;
	mMBBVisible = false;
}

/*
* Accessors
*/
std::vector<Vector2I*>* Curve::GetControlPointVec()
{
	return mControlPoints.GetControlPointVec();
}

std::vector<Vector2I*>* Curve::GetPlottedPointsVec()
{
	return &mPlotVec;
}

Color3 Curve::GetColor() const
{
	return mColor;
}

bool Curve::GetControlPointVisibility() const
{
	return mControlPointVisible;
}

bool Curve::GetControlPolygonVisibility() const
{
	return mControlPolygonVisible;
}

bool Curve::GetMBBVisibility() const
{
	return mMBBVisible;
}

MyRectangle Curve::GetMBB()
{
	MyRectangle *returnRect = new MyRectangle();
	returnRect->Set(mControlPoints.GetMBB());
	return *returnRect;
}

void Curve::Draw() const
{
	Draw(mColor);
}

void Curve::Draw(const Color3 &drawColor) const
{
	Line tempLine;
	for (unsigned int i = 0; i < mPlotVec.size(); i++)
	{
		/*Draw a point within the vec holding the points of the curve*/
		SetPixel(mPlotVec[i]->GetX(), mPlotVec[i]->GetY(), drawColor);

		/*Draw lines connecting two adjecent points in mPlotVec, in case the resolution
		of the parametric range isn't sufficiently large to draw a continuous curve by
		plotting individual points alone*/
		if (i > 0)
		{
			tempLine = Line(*mPlotVec[i - 1], *mPlotVec[i], drawColor);
			tempLine.Draw();
		}

		/*Draw intermediate lines*/
		//if (mGenerationLines.size() > 0)
		//{
		//	if (i > 0)
		//	{
		//		/*Erase previous generation's itermediate lines*/
		//		for (unsigned int gen = 0; gen < mGenerationLines[i - 1].size(); gen++)
		//			mGenerationLines[i - 1][gen]->Draw(Color3(0.0f, 0.0f, 0.0f));
		//	}

		//	/*Draw current generation's intermediate lines*/
		//	for (unsigned int gen = 0; gen < mGenerationLines[i].size(); gen++)
		//		mGenerationLines[i][gen]->Draw();
		//}
	}

	if (mControlPointVisible)
		mControlPoints.Draw();

	if (mControlPolygonVisible)
		mControlPoints.DrawControlPolygon();

	if (mMBBVisible)
		mControlPoints.DrawMBB();
}

void Curve::DrawIntermediateLines() const
{
	for (unsigned int i = 0; i < mIntermediateLines.size(); i++)
		mIntermediateLines[i]->Draw();
}

/*
 * Mutators
 */
/*Whenever a new control point is inserted, (re)calculate all the plot points of the curve*/
void Curve::InsertControlPoint(const Vector2I &newControlPoint)
{
	/*First, clear previously drawn curve*/
	Clear();

	/*Also clear any intermediate lines that may be in the pixelBuffer*/
	ClearIntermediateLines();

	/*Make a copy of the newControlPoint*/
	Vector2I *temp = new Vector2I(newControlPoint.GetX(), newControlPoint.GetY());

	/*Insert the new control point and recalculate the curve*/
	mControlPoints.PushBack(*temp);
	CalcPlotVec();

	/*
	* NOTE: I don't like how this is implemented. It would be better if none of the curve functions
	*		depended on the graphModeIsOn flag, or any other global flags for that matter, so that
	*		this file stays very portable to other projects.
	*/
	//if (!graphModeIsOn)
	//{
	//	int index = MAGIC_GARBAGE;
	//	for (unsigned int i = 0; i < curveVec.size(); i++)
	//	{
	//		if (&(*this) == &(*(curveVec[i])))
	//		{
	//			index = i;
	//			break;
	//		}
	//	}
	//	HandleCurveDraw(index);
	//}
}

void Curve::EraseAllControlPoints()
{
	mControlPoints.EraseAll();
}

void Curve::EraseControlPoint(int index)
{
	mControlPoints.Erase(index);
}

void Curve::SetIntermediateLines(float t)
{
	/*If there are no control points, return*/
	if (mControlPoints.GetControlPointVec()->size() < 1)
		return;

	/*Ensure that t is normalized*/
	t = (t < 0.0f) ? 0.0f : t;
	t = (t > 1.0f) ? 1.0f : t;

	/*Allocate space for the 2D array generation table,
	used for keeping track of children in generations from
	zero to n*/
	const unsigned int n = mControlPoints.GetControlPointVec()->size() - 1;
	Vector2I *generationTable = new Vector2I[(n + 1)*(n + 1)]; /*Each row represents a generation,
															   starting with generation 0 at the top.
															   Each column within a row represents
															   a child of that generation, in
															   increasing order from left to right.*/

	/*Populate the generation table*/
	for (unsigned int j = 0; j <= n; j++)
	{
		for (unsigned int i = 0; i <= n - j; i++)
		{
			if (j == 0)
			{
				generationTable[i].SetX(mControlPoints[i].GetX());
				generationTable[i].SetY(mControlPoints[i].GetY());
			}
			else
			{
				Vector2I temp = Vector2I((1.0f - t) * generationTable[(j - 1) * (n + 1) + i] +
					t * generationTable[(j - 1) * (n + 1) + i + 1]);

				generationTable[j * (n + 1) + i].SetX(temp.GetX());
				generationTable[j * (n + 1) + i].SetY(temp.GetY());
			}

			/*Calculate intermediate lines*/
			if (i > 0)
			{
				/*
				* Store line connecting two adjacent children of the same generation
				*/
				/*Calculate which point on the curve this is*/
				float tStep = 1.0f / curveResolution;
				int s = (int)(t / tStep); /*Counter of which pixel is being calculated in the curve*/
				/*Store a generation line*/
				Line* tempLine = new Line(generationTable[j * (n + 1) + i - 1],
					generationTable[j * (n + 1) + i],
					Color3(mColor.GetR() / 2.0f, mColor.GetG() / 2.0f, mColor.GetB() / 2.0f));
				mIntermediateLines.push_back(tempLine);
			}
		}
	}
}

void Curve::Clear()
{
	/*Clear pixels from pixelBuffer*/
	Line tempLine;
	for (unsigned int i = 0; i < mPlotVec.size(); i++)
	{
		SetPixel(mPlotVec[i]->GetX(), mPlotVec[i]->GetY(), Color3(0.0f, 0.0f, 0.0f));
		if (i > 0)
		{
			tempLine = Line(*mPlotVec[i - 1], *mPlotVec[i], Color3(0.0f, 0.0f, 0.0f));
			tempLine.Draw();
		}
	}

	/*Clear stored pixel locations within mPlotVec and mGenerationLines*/
	mPlotVec.clear();
	//mGenerationLines.clear();
}

void Curve::ClearIntermediateLines()
{
	/*Clear pixels from pixelBuffer*/
	Line tempLine;
	for (unsigned int i = 0; i < mIntermediateLines.size(); i++)
		mIntermediateLines[i]->Draw(Color3(0.0f, 0.0f, 0.0f));

	/*Clear stored pixel locations within mIntermediateLines*/
	mIntermediateLines.clear();
}

/*Clear the MBB lines from the pixelBuffer.*/
void Curve::ClearMBB()
{
	mControlPoints.ClearMBB();
}

/*Clear curve, intermediate lines, control points, control polygon, MBB*/
void Curve::ClearEverything()
{
	Clear();
	ClearIntermediateLines();
	mControlPoints.Clear();
	mControlPoints.ClearControlPolygon();
	/*mControlPoints.ClearConvexHull();*/
	mControlPoints.ClearMBB();
}

void Curve::SetColor(const Color3 &newColor)
{
	mColor.Set(newColor);
	mControlPoints.SetColor(newColor);
}

/*Splits this curve at t. This curve becomes the left child and the right child is returned.*/
Curve* Curve::Split(float t)
{
	/*If there are no control points, return*/
	if (mControlPoints.GetControlPointVec()->size() < 1)
		return NULL;

	/*Ensure that t is normalized*/
	t = (t < 0.0f) ? 0.0f : t;
	t = (t > 1.0f) ? 1.0f : t;

	/*Allocate space for the 2D array generation table,
	used for keeping track of children in generations from
	zero to n*/
	const unsigned int n = mControlPoints.GetControlPointVec()->size() - 1;
	Vector2I *generationTable = new Vector2I[(n + 1)*(n + 1)]; /*Each row represents a generation,
															   starting with generation 0 at the top.
															   Each column within a row represents
															   a child of that generation, in
															   increasing order from left to right.*/

	/*Populate the generation table*/
	for (unsigned int j = 0; j <= n; j++)
	{
		for (unsigned int i = 0; i <= n - j; i++)
		{
			if (j == 0)
			{
				generationTable[i].SetX(mControlPoints[i].GetX());
				generationTable[i].SetY(mControlPoints[i].GetY());
			}
			else
			{
				Vector2I temp = Vector2I((1.0f - t) * generationTable[(j - 1) * (n + 1) + i] +
					t * generationTable[(j - 1) * (n + 1) + i + 1]);

				generationTable[j * (n + 1) + i].SetX(temp.GetX());
				generationTable[j * (n + 1) + i].SetY(temp.GetY());
			}
		}
	}

	/*Clear this curve's control points, then set its new control points to be those
	  of the left child*/
	EraseAllControlPoints();
	for (unsigned int i = 0; i < n + 1; i++)
		InsertControlPoint(generationTable[i * (n + 1)]);

	/*Create a pointer to a curve with the right child's control points.*/
	Curve *rightChild;
	if(isUsingBernstein)
		rightChild = new BernsteinCurve(GetRandomCurveColor().GetColor3()); 
	else /*if(!isUsingBernstein)*/
		rightChild = new DeCasteljauCurve(GetRandomCurveColor().GetColor3());
	for (unsigned int i = 0; i < n + 1; i++)
		rightChild->InsertControlPoint(generationTable[i * (n + 1) + n - i]);

	/*Handle flags for right child*/
	rightChild->SetControlPointVisibility(controlPointsVisible);
	rightChild->SetControlPolygonVisibility(controlPolygonVisible);
	rightChild->SetMBBVisibility(MBBVisible);

	/*return right child curve*/
	return rightChild;
}

void Curve::SetControlPointVisibility(bool newVisibility)
{
	mControlPointVisible = newVisibility;

	/*If control point visibility has just been turned off, clear all control points that may
	  be onscreen.*/
	if (newVisibility == false)
		mControlPoints.Clear();

	/*If control point visibility has just been turned on, draw control points*/
	else
		if ( !graphModeIsOn )
			mControlPoints.Draw();
}

void Curve::SetControlPolygonVisibility(bool newVisibility)
{
	mControlPolygonVisible = newVisibility;

	/*If control polygon visibility has just been turned off, clear all lines from the
	  control polygon that may be onscreen.*/
	if (newVisibility == false)
		mControlPoints.ClearControlPolygon();
	

	/*If control polygon visibility has just been turned on, draw lines that make up
	  the control polygon*/
	else
		if (!graphModeIsOn)
			mControlPoints.DrawControlPolygon();
}

void Curve::SetMBBVisibility(bool newVisibility)
{
	mMBBVisible = newVisibility;

	/*If MBB visibility has just been turned off, clear all lines from the
	MBB that may be onscreen.*/
	if (newVisibility == false)
		mControlPoints.ClearMBB();


	/*If control polygon visibility has just been turned on, draw lines that make up
	the control polygon*/
	else
		if (!graphModeIsOn)
			mControlPoints.DrawMBB();
}



/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class DeCasteljauCurve
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
DeCasteljauCurve::DeCasteljauCurve(const Color3 &newColor) : Curve(newColor)
{
}

/*
* Accessors
*/
Vector2I DeCasteljauCurve::GetPlotPoint(float t)
{
	/*If there are no control points, return some arbitrary coordinate*/
	if (mControlPoints.GetControlPointVec()->size() < 1)
		return Vector2I(0, 0);

	/*Ensure that t is normalized*/
	t = (t < 0.0f) ? 0.0f : t;
	t = (t > 1.0f) ? 1.0f : t;

	/*Allocate space for the 2D array generation table,
	used for keeping track of children in generations from
	zero to n*/
	const unsigned int n = mControlPoints.GetControlPointVec()->size() - 1;
	Vector2F *generationTable = new Vector2F[(n + 1)*(n + 1)]; /*Each row represents a generation,
																starting with generation 0 at the top.
																Each column within a row represents
																a child of that generation, in
																increasing order from left to right.*/

	/*Populate the generation table, then return the single child of the last generation*/
	for (unsigned int j = 0; j <= n; j++)
	{
		for (unsigned int i = 0; i <= n - j; i++)
		{
			if (j == 0)
			{
				generationTable[i].SetX((float)mControlPoints[i].GetX());
				generationTable[i].SetY((float)mControlPoints[i].GetY());
			}
			else
			{
				Vector2F temp = Vector2F((1.0f - t) * generationTable[(j - 1) * (n + 1) + i] +
					t * generationTable[(j - 1) * (n + 1) + i + 1]);

				generationTable[j * (n + 1) + i].SetX(temp.GetX());
				generationTable[j * (n + 1) + i].SetY(temp.GetY());
			}

			/*Calculate intermediate lines*/
			//if (i > 0)
			//{
			//	/*
			//	* Store line connecting two adjacent children of the same generation
			//	*/
			//	/*Calculate which point on the curve this is*/
			//	float tStep = 1.0f / curveResolution;
			//	int s = (int)(t / tStep); /*Counter of which pixel is being calculated in the curve*/
			//	/*Store a generation line*/
			//	Line* tempLine = new Line(generationTable[j * (n + 1) + i - 1],
			//		generationTable[j * (n + 1) + i], mColor);
			//	std::vector<Line*> tempGenLine;
			//	tempGenLine.push_back(tempLine);
			//	if (j == 0)
			//		mGenerationLines.push_back(tempGenLine);
			//	else
			//		mGenerationLines[s].push_back(tempLine);
			//}
		}
	}

	Vector2I roundedPlotPoint = Vector2I(MyRound(generationTable[n * (n + 1)].GetX()), MyRound(generationTable[n * (n + 1)].GetY()));
	return roundedPlotPoint; /*Return the first (and only) child of the last generation.*/
}

/*
* Mutators
*/
void DeCasteljauCurve::CalcPlotVec()
{
	/*Since this function will calculate every point on the curve, we can clear any previous
	contents in the mPlotVec and mGenerationLines*/
	mPlotVec.clear();
	//mGenerationLines.clear();

	/*Populate the mPlotVec (and, implicitly, mGenerationLines)*/
	Vector2I *temp;
	float tStep = 1.0f / curveResolution;
	for (float t = 0.0f; t <= 1.0f; t += tStep)
	{
		temp = new Vector2I(GetPlotPoint(t));
		mPlotVec.push_back(temp);
	}

	/*Ensure that the point when t=1.0f is drawn*/
	mPlotVec.push_back(new Vector2I(GetPlotPoint(1.0f)));
}


/*
* -------------------------------------------------------------------------------------------------------
* Implementation of class Bernstein
* -------------------------------------------------------------------------------------------------------
*/
/*
* Constructor(s)
*/
BernsteinCurve::BernsteinCurve(const Color3 &newColor) : Curve(newColor)
{
}

/*
* Accessors
*/
/*
* This function has odd behavior when the number of control points is greater than
* or equal to 22, i.e. when nPlusOne >= 22
*/
Vector2I BernsteinCurve::GetPlotPoint(float t)
{
	/*If there are no control points, return some arbitrary coordinate*/
	if (mControlPoints.GetControlPointVec()->size() < 1)
		return Vector2I(0, 0);

	/*Ensure that t is normalized*/
	t = (t < 0.0f) ? 0.0f : t;
	t = (t > 1.0f) ? 1.0f : t;

	/*Use the Bernstein algorithm to get the plot point determined by t*/
	float x = 0;
	float y = 0;
	float coeff = 0;
	unsigned int nPlusOne = mControlPoints.GetControlPointVec()->size();
	for (unsigned int i = 0; i < nPlusOne; i++)
	{
		coeff = (float)(pascalsTriangle[nPlusOne - 1][i] * MyPow(1.0f - t, (float)(nPlusOne - 1 - i)) * MyPow(t, (float)i));
		x += coeff * (*mControlPoints.GetControlPointVec())[i]->GetX();
		y += coeff * (*mControlPoints.GetControlPointVec())[i]->GetY();
	}

	Vector2I roundedPlotPoint = Vector2I(MyRound(x), MyRound(y));
	return roundedPlotPoint; /*Return the first (and only) child of the last generation.*/
}

/*
* Mutators
*/
void BernsteinCurve::CalcPlotVec()
{
	/*Since this function will calculate every point on the curve, we can clear any previous
	contents in the mPlotVec and mGenerationLines*/
	mPlotVec.clear();
	//mGenerationLines.clear();

	/*Populate the mPlotVec (and, implicitly, mGenerationLines)*/
	Vector2I *temp;
	float tStep = 1.0f / curveResolution;
	for (float t = 0.0f; t <= 1.0f; t += tStep)
	{
		temp = new Vector2I(GetPlotPoint(t));
		mPlotVec.push_back(temp);
	}

	/*Ensure that the point when t=1.0f is drawn*/
	mPlotVec.push_back(new Vector2I(GetPlotPoint(1.0f)));
}



/*
* Global functions
*/
/*Takes a float param because all functions linked to a slider must have the form:
*			void funcName(float paramName)
*/
void SetCurveResolution(float newCurveResolution)
{
	/*Set new curve resolution*/
	curveResolution = (unsigned int)newCurveResolution;

	/*Move the other slider bar slightly so that the intermediate lines are cleared,
	recalculated, and redrawn to fit the new curve*/
	if (intermediateLinesSlider != NULL)
	{
		Vector2I sliderPos = intermediateLinesSlider->GetSliderPosition();
		if (intermediateLinesSlider->GetSliderOrientation() == Slider::Vertical)
		{
			sliderPos = (sliderPos.GetY() < intermediateLinesSlider->GetRect().GetTopLeft().GetY()) ?
				Vector2I(sliderPos.GetX(), sliderPos.GetY() + 1) :
				Vector2I(sliderPos.GetX(), sliderPos.GetY() - 1);
		}
		else /*if (intermediateLinesSlider->GetSliderOrientation() == Slider::Horizontal)*/
		{
			sliderPos = (sliderPos.GetX() > intermediateLinesSlider->GetRect().GetTopLeft().GetX()) ?
				Vector2I(sliderPos.GetX() - 1, sliderPos.GetY()) :
				Vector2I(sliderPos.GetX() + 1, sliderPos.GetY());
		}
		intermediateLinesSlider->SetSliderPosition(sliderPos);
	}

	/*Set new resolution for curves and redraw them.*/
	for (unsigned int i = 0; i < curveVec.size(); i++)
	{
		/*Clear all curves onscreen, as they are about to have a new resolution*/
		curveVec[i]->Clear();

		/*Recalculate all the points for the curves based on the new curve resolution*/
		curveVec[i]->CalcPlotVec();
		HandleCurveDraw(i);
	}
}

void SetAndDrawIntermediateLines(float t)
{
	for (unsigned int i = 0; i < curveVec.size(); i++)
	{
		/*Clear old intermediate lines*/
		curveVec[i]->ClearIntermediateLines();

		/*Set new intermediate lines*/
		curveVec[i]->SetIntermediateLines(t);

		/*Draw intermediate lines*/
		curveVec[i]->DrawIntermediateLines();

		/*Since clearing the old intermediate lines might have cleared some pixels representing the
		  actual curve in the pixelBuffer, redraw the curve. Since intermediate lines are only drawn
		  for the line described by the deCasteljau algorithm, that is the line to redraw*/
		HandleCurveDraw(i);
	}
}

void SetSplitValue(float t)
{
	if (t > 1.0f)
		splitT = 1.0f;
	else if (t < 0.0f)
		splitT = 0.0f;
	else
		splitT = t;
}

void HandleNewControlPoint(const Vector2I &temp)
{
	/*If curveIndex is out of bounds, set it to MAGIC_GARBAGE so that the new control point
	  will go towards making a new curve.*/
	if (curveIndex < 0 || (unsigned int)curveIndex >= curveVec.size())
		curveIndex = MAGIC_GARBAGE;

	/*When the user places a new control point, clear any intermediate lines that may have
	been onscreen.*/
	for (unsigned int i = 0; i < curveVec.size(); i++)
		curveVec[i]->ClearIntermediateLines();

	/*Clear the MBB of the curve in focus that may have been onscreen, as placing a new control point
	  may change the MBB of the curve in focus. In any case, after forcing a redraw, the MBB of the
	  curve in focsu will be redisplayed if the MBBCheckbox is checked.*/
	if (curveIndex >= 0 && (unsigned int)curveIndex < curveVec.size())
		curveVec[curveIndex]->ClearMBB();

	/*Make a deep copy of the new control point's position*/
	Vector2I *newControlPointPos = new Vector2I();
	newControlPointPos->SetX(temp.GetX());
	newControlPointPos->SetY(temp.GetY());

	/*If no curve is in focus, then create a new curve (using the Bernstein algorithm
	  if the respective checkbox is checked, or the deCasteljau otherwise).*/
	if (curveIndex == MAGIC_GARBAGE)
	{
		Curve *newCurve = NULL;
		
		/*Create a curve using with the underlying algorithm either being the deCasteljau
		  or the Bernstein.*/
		if (isUsingBernstein)
			newCurve = new BernsteinCurve();
		else /*(!isUsingBernstein)*/
			newCurve = new DeCasteljauCurve();

		/*Give curve a random, non-yellow color, non-dark color.*/
		do
		{
			newCurve->SetColor(GetRandomCurveColor().GetColor3());
		} while (newCurve->GetColor().GetR() >= 0.8f && newCurve->GetColor().GetG() >= 0.8f);
		
		/*Handle flags for new curve*/
		newCurve->SetControlPointVisibility(controlPointsVisible);
		newCurve->SetControlPolygonVisibility(controlPolygonVisible);
		newCurve->SetMBBVisibility(MBBVisible);

		/*Change curveIndex and insert curve into curveVec*/
		curveIndex = curveVec.size();
		newCurve->InsertControlPoint(*newControlPointPos);
		curveVec.push_back(newCurve);
	}
	else
	{
		if ((curveVec[curveIndex]->GetControlPointVec())->size() < pascalsTriangle.size())
			curveVec[curveIndex]->InsertControlPoint(*newControlPointPos);
	}

	/*Force redraw of curve that just had a new control point inserted into it.*/
	HandleCurveDraw(curveIndex);
}

void HandleCurveDraw(int index)
{
	if (index < 0 || (unsigned int)index >= curveVec.size())
		return;

	if (index == curveIndex)
		curveVec[index]->Draw(Color3(1.0f, 1.0f, 0.0f));
	else
		curveVec[index]->Draw();
}

void GetIntersections(Curve *firstCurve, Curve *secondCurve)
{
	const unsigned int EPSILON = 1;
	MyRectangle firstCurveMBB = firstCurve->GetMBB();
	MyRectangle secondCurveMBB = secondCurve->GetMBB();
	Curve *firstCurveFirstChild = NULL;
	Curve *firstCurveSecondChild = NULL;
	Curve *secondCurveFirstChild = NULL;
	Curve *secondCurveSecondChild = NULL;


	/*The MBBs of the two curves overlap. Check to see if the MBBs of both curves are small enough.
	  If they are, then update intersectionsVec. If not, split both curves and make a recursive
	  call to this function to check if the children MBBs overlap.*/
	if (firstCurveMBB.ContainsRect(secondCurveMBB))
	{
		/*Create a copy of firstCurve*/
		firstCurveFirstChild = new BernsteinCurve();
		for (unsigned int controlPoint = 0; controlPoint < firstCurve->GetControlPointVec()->size(); controlPoint++)
			firstCurveFirstChild->InsertControlPoint(*((*(firstCurve->GetControlPointVec()))[controlPoint]));

		/*Create a copy of secondCurve.*/
		secondCurveFirstChild = new BernsteinCurve();
		for (unsigned int controlPoint = 0; controlPoint < secondCurve->GetControlPointVec()->size(); controlPoint++)
			secondCurveFirstChild->InsertControlPoint(*((*(secondCurve->GetControlPointVec()))[controlPoint]));

		/*Split firstCurve.*/
		firstCurveSecondChild = firstCurveFirstChild->Split(0.5f);

		/*Split secondCurve.*/
		secondCurveSecondChild = secondCurveFirstChild->Split(0.5f);



		/*If both MBBs are small enough, update intersectionsVec and return.*/
		if ((firstCurveMBB.GetDimensions().GetX() < EPSILON && firstCurveMBB.GetDimensions().GetY() < EPSILON) ||
			(secondCurveMBB.GetDimensions().GetX() < EPSILON && secondCurveMBB.GetDimensions().GetY() < EPSILON) ||
			(firstCurveMBB.GetDimensions() == firstCurveFirstChild->GetMBB().GetDimensions() || firstCurveMBB.GetDimensions() == firstCurveSecondChild->GetMBB().GetDimensions()) &&
			(secondCurveMBB.GetDimensions() == secondCurveFirstChild->GetMBB().GetDimensions() || secondCurveMBB.GetDimensions() == secondCurveSecondChild->GetMBB().GetDimensions()))
		{
			Vector2I *newIntersection = new Vector2I(firstCurveMBB.GetCenter());
			intersectionsVec.push_back(newIntersection);
			return;
		}


		/*Force a redisplay of the pixel buffer, to display any MBBs of children curves created.*/
		for (unsigned int i = 0; i < curveVec.size(); i++)
			HandleCurveDraw(i);
		Display();

		/*Check for intersections between children curves belonging to different parent curves.*/
		GetIntersections(firstCurveFirstChild, secondCurveFirstChild);
		GetIntersections(firstCurveFirstChild, secondCurveSecondChild);
		GetIntersections(firstCurveSecondChild, secondCurveFirstChild);
		GetIntersections(firstCurveSecondChild, secondCurveSecondChild);
	}
}
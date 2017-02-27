#ifndef HFILE_CURVES
#define HFILE_CURVES


#include <vector>
#include "General.h"
#include "Line.h"
#include "Rectangle.h"




class ControlPoints
{
public:
	/*
	 * Constructor(s)
	 */
	ControlPoints(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	 * Accessors
	 */
	std::vector<Vector2I*> *GetControlPointVec();
	Color3 GetColor() const;
	MyRectangle GetMBB() const;
	void Draw() const;
	void DrawControlPolygon() const; /*Connect all adjacent control points*/
	void DrawConvexHull() const; /*Draw convex hull*/
	void DrawMBB() const; /*Draw Minimum Bounding Box*/
	void Clear() const; /*Erase all control point pixels from pixel buffer*/
	void ClearControlPoint(int index) const; /*Erase the given control point's pixels from pixel buffer.*/
	void ClearControlPolygon() const; /*Erase control polygon pixels from pixel buffer*/
	void ClearConvexHull() const; /*Erase convex hull pixels from pixel buffer*/
	void ClearMBB() const; /*Erase MBB pixels from pixel buffer*/

	/*
	 * Mutators
	 */
	void SetColor(const Color3 &newColor);
	void PushBack(const Vector2I &newControlPoint);
	void EraseAll();
	void Erase(int index);

	/*
	 * Overloaded operators
	 */
	Vector2I &operator[](int index);
	const Vector2I &operator[](int index) const;
	
private:
	std::vector<Vector2I*> mCPVec; /*Control point vec*/
	Color3 mColor;
};


class Curve
{
public:
	/*
	 * Constructor(s)
	 */
	Curve(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	 * Accessors
	 */
private:
	virtual Vector2I GetPlotPoint(float t) = 0;
public:
	std::vector<Vector2I*> *GetControlPointVec();
	std::vector<Vector2I*> *GetPlottedPointsVec();
	Color3 GetColor() const;
	bool GetControlPointVisibility() const;
	bool GetControlPolygonVisibility() const;
	bool GetMBBVisibility() const;
	MyRectangle GetMBB();
	virtual void Draw() const;
	virtual void Draw(const Color3 &drawColor) const;
	void DrawIntermediateLines() const;

	/*
	 * Mutators
	 */
	void InsertControlPoint(const Vector2I &newControlPoint); /* Whenever a new control point is inserted,
															  * (re)calculate all the plot points of the
															  * curve*/
	void EraseAllControlPoints();
	void EraseControlPoint(int index);
	virtual void Clear(); /*Clear the points on the curve from the pixelBuffer*/
	void ClearIntermediateLines(); /*Clear the intermediate lines from the pixelBuffer*/
	void ClearMBB(); /*Clear the MBB lines from the pixelBuffer.*/
	void ClearEverything(); /*Clear curve, intermediate lines, control points, control polygon, MBB*/
	virtual void CalcPlotVec() = 0;
	void SetColor(const Color3 &newColor);
	void SetIntermediateLines(float t);
	Curve* Split(float t); /*Splits this curve at t. This curve becomes the left child
						     and the right child is returned.*/
	void SetControlPointVisibility(bool newVisibility);
	void SetControlPolygonVisibility(bool newVisibility);
	void SetMBBVisibility(bool newVisibility);

protected:
	ControlPoints mControlPoints;
	std::vector<Vector2I*> mPlotVec; /*A vec of plotted points on the curve*/
	Color3 mColor;

	bool mControlPointVisible; /*If true, display control points*/
	bool mControlPolygonVisible; /*If true, display control polygon*/
	bool mMBBVisible; /*If true, display MBB as a line rect*/


	/*std::vector<std::vector<Line*>> mGenerationLines;*/ /*All the intermediate lines (drawn between two adjecent children
														  of the same generation) for all points on the curve. This
														  variable keeps track of such groups of
														  lines.*/
	std::vector<Line*> mIntermediateLines; /*Lines used in the deCasteljau algorithm to plot a point on the curve*/
};


/*Bezier curve implemented with the deCasteljau algorithm*/
class DeCasteljauCurve : public Curve
{
public:
	/*
	 * Constructor(s)
	 */
	DeCasteljauCurve(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	 * Accessors
	 */
private:
	Vector2I GetPlotPoint(float t);
public:
	/*
	 * Mutators
	 */
	void CalcPlotVec();
};


/*Bezier curve implemented with the Bernstein algorithm*/
class BernsteinCurve : public Curve
{
public:
	/*
	* Constructor(s)
	*/
	BernsteinCurve(const Color3 &newColor = Color3(1.0f, 0.0f, 0.0f));

	/*
	* Accessors
	*/
private:
	Vector2I GetPlotPoint(float t);
public:
	/*
	* Mutators
	*/
	void CalcPlotVec();
};



/*
* Global function prototypes
*/
void SetCurveResolution(float newCurveResolution);
void SetAndDrawIntermediateLines(float t);
void SetSplitValue(float t);
void HandleNewControlPoint(const Vector2I &newControlPoint);
void HandleCurveDraw(int index);
void GetIntersections(Curve *firstCurve, Curve *secondCurve);

/*
* Global variables
*/
extern int curveIndex;
extern std::vector<Curve*> curveVec;
extern unsigned int curveResolution; /*How many chunks to break the parametric range zero to one
									 into. A higher number here will yield smoother curves, since
									 more points will be used to draw them and thus any line
									 segments between points will be shorter.*/
extern float splitT; /*Where to split the curveInFocus; has a value between 0.0f and 1.0f, inclusive.*/
extern bool isUsingBernstein;
extern bool controlPointsVisible; /*True if the controlPointCheckbox is checked*/
extern bool controlPolygonVisible; /*True if the controlPolygonCheckbox is checked*/
extern bool MBBVisible; /*True if the MBBCheckbox is checked*/
extern std::vector<Vector2I*> intersectionsVec;

#endif /*HFILE_CURVES*/
#pragma once
#include "CoordinateMapping.h"

class CoordinateMappingFlipX :
	public CoordinateMapping
{
public:
	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		x = image.getWidth() - x;

		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;

		return true;
	}
};

class CoordinateMappingFlipY :
	public CoordinateMapping
{
public:
	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		y = image.getHeight() - y;

		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;

		return true;
	}
};

class CoordinateMappingFlipXY :
	public CoordinateMapping
{
public:
	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		x = image.getWidth() - x;
		y = image.getHeight() - y;

		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;

		return true;
	}
};

//A version of the image flip used for sprite sheets specifically - this takes the width and height of each sprite, along with the row that is being animated, and flips each sprite individually
class CoordinateMappingFlipSheet :
	public CoordinateMapping
{
public:
	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{
		//Only flip on the row currently being animated
		/*
		if (y > offsetY&& y <= offsetY + sprH) {
			int xw = std::floor(x / sprW) * sprW; //find the frame number that this pixel resides in
			x = xw + 1 - (x - xw); //flip the sprite within its frame
		}
		*/

		//flip sprite sheet and apply offset in reverse
		x = (double)image.getWidth() - x;

		x += (double)offsetX + (double)sprW - (double)image.getWidth(); //not sure why I need to add sprW but it's the only way I can make it work properly
		y += (double)offsetY;
		
		if (image.getPixelColour((int)x, (int)y) == transCol) return false;
		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (double)(image.getWidth() - 0.5)) return false;
		if (y >= (double)(image.getHeight() - 0.5)) return false;

		return true;
	}

	//Wrapper for mapCoordinates that takes the additional values needed
	void setSheetValues(int offsetX, int offsetY, int sprW, int sprH, int transCol)
	{
		this->offsetX = offsetX;
		this->offsetY = offsetY;
		this->sprW = sprW;
		this->sprH = sprH;
		this->transCol = transCol;
	}

private:
	int offsetX, offsetY;
	int sprW, sprH;
	int transCol;

};

class CoordinateMappingRotateTransparent :
	public CoordinateMapping
{
public:
	CoordinateMappingRotateTransparent(double r, int transCol)
		: rotation(r), transCol(transCol)
	{
	}

	virtual bool mapCoordinates(double& x, double& y, const SimpleImage& image) override
	{

		// Shift offset to the centre of the image, so we can rotate around centre
		x -= image.getWidth() / 2;
		y -= image.getHeight() / 2;

		// Rotate it
		double dAngle = atan(y / (x + 0.0001));
		if (x < 0)
			dAngle += M_PI;
		double hyp = ::sqrt(x * x + y * y);
		dAngle += (double)rotation;

		x = hyp * ::cos(dAngle);
		y = hyp * ::sin(dAngle);

		// Shift offset back to the corner
		x += image.getWidth() / 2;
		y += image.getHeight() / 2;

		if (image.getPixelColour((int)x, (int)y) == transCol) return false;
		if (x < 0) return false;
		if (y < 0) return false;
		if (x >= (image.getWidth() - 0.5)) return false;
		if (y >= (image.getHeight() - 0.5)) return false;

		return true;
	}

	double getRotation() { return rotation; }
	void setRotation(double r) { rotation = r; }

protected:
	double rotation;
	int transCol;

};
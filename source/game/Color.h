#ifndef GB_COLOR_H
#define GB_COLOR_H

struct Color
{
	int redness;
	int greenness;
	int blueness;
	int alphaness;

	Color() : redness(0), greenness(0), blueness(0), alphaness(255)
	{
	}

	Color(int red, int green, int blue, int alpha = 255) : 
		redness(red),
		greenness(green),
		blueness(blue),
		alphaness(alpha)
	{
	}

	//TODO: a < b  should mean that b can't be less than a, but this code allows it.
	bool operator<(const Color & other) const
	{
		return ((this->redness < other.redness) ||
			(this->greenness < other.greenness) ||
			(this->blueness < other.blueness) ||
			(this->alphaness < other.alphaness));
	}
};

#endif

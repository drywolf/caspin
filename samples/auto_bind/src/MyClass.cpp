#include "MyClass.h"
#include "auto_classes.cpp"


namespace flash {namespace geom {

	int ColorTransform::get_color()
	{
		return mColor;
	}

	void ColorTransform::set_color(int value)
	{
		std::cout << "color = " << value << std::endl;
		mColor = value;
	}

}}
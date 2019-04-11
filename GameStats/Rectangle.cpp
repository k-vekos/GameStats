#include "Rectangle.h"

#include <iostream>

Rectangle::Rectangle(int centerX, int centerY, int width, int depth)
{
	center.first = centerX;
	center.second = centerY;
	this->width = width;
	this->depth = depth;
}

void Rectangle::print() const
{
	std::cout << "---Rectangle---" << std::endl;
	std::cout << "center=(" << center.first << ", " << center.second << ")" << std::endl;
	std::cout << "width=" << width << std::endl;
	std::cout << "depth=" << depth << std::endl;
}

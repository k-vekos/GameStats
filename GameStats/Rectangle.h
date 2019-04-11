#pragma once

#include "GeometricShape.h"

#include <cstdint>
#include <utility>

/* Represents a geometric shape that is a rectangle. */
class Rectangle :
	public GeometricShape
{
public:
	Rectangle() = default;
	Rectangle(int centerX, int centerY, int width, int depth);
	virtual void print() const;
	std::pair<int32_t, int32_t> center;
	int32_t width;
	int32_t depth;
};


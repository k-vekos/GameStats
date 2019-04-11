#pragma once

#include "GeometricShape.h"

#include <cstdint>
#include <utility>

/* Represents a geometric shape that is a circle. */
class Circle :
	public GeometricShape
{
public:
	Circle() = default;
	Circle(int centerX, int centerY, int radius);
	virtual void print() const;
	std::pair<int32_t, int32_t> center;
	int32_t radius;
};

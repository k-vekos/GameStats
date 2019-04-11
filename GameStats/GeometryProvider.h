#pragma once

#include <string>
#include <vector>

#include "GeometricShape.h"
#include "Circle.h"
#include "Rectangle.h"

/* Helper class for geometry-related operations including loading geometry from file. */
class GeometryProvider
{
public:
	GeometryProvider() = default;
	void loadStaticGeometry();
	void loadGeometryFile(std::string fileName);
	std::vector<GeometricShape*> & getLevelGeometry();
	std::vector<Circle> getCircles();
	std::vector<Rectangle> getRectangles();
private:
	std::vector<GeometricShape*> levelGeometry;
	void readToInt32(int32_t & buffer, std::ifstream & stream, bool & bigEndian);
};


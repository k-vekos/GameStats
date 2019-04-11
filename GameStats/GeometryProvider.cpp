#include "GeometryProvider.h"
#include "Circle.h"
#include "Rectangle.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <cstdint>
#include <boost/endian/conversion.hpp>

/* Loads geometry data from given file into memory. */
void GeometryProvider::loadGeometryFile(std::string fileName)
{
	// clear level geometry first
	levelGeometry.clear();

	char geoTypeBuffer;
	char buffer[4];
	int32_t int32buffer = 0;

	std::ifstream input(fileName, std::ios::binary);

	// read in ABCD header
	input.read(buffer, 4);

	// check endianness
	bool bigEndian = false;
	if (buffer[0] == 'D') {
		bigEndian = true;
	}

	while (true) {
		// read in first byte ("Geo Type")
		input.read(&geoTypeBuffer, 1);

		// check if we've hit EoF yet...
		if (input.eof())
			break;

		/*
		Byte geoType			\  \
		Int32 x					 |--|-- Circle
		Int32 y					 |  |
		int32 radius / width	/   |-- Rectangle
		int32 depth                /
		*/

		if (geoTypeBuffer == 0) {
			// It's a Circle
			Circle * circle = new Circle();

			// x
			readToInt32(int32buffer, input, bigEndian);
			circle->center.first = int32buffer;
			// y
			readToInt32(int32buffer, input, bigEndian);
			circle->center.second = int32buffer;
			// radius
			readToInt32(int32buffer, input, bigEndian);
			circle->radius = int32buffer;

			levelGeometry.push_back(circle);
		}
		else if (geoTypeBuffer == 1) {
			// It's a Rectangle
			Rectangle * rectangle = new Rectangle();

			// x
			readToInt32(int32buffer, input, bigEndian);
			rectangle->center.first = int32buffer;
			// y
			readToInt32(int32buffer, input, bigEndian);
			rectangle->center.second = int32buffer;
			// width
			readToInt32(int32buffer, input, bigEndian);
			rectangle->width = int32buffer;
			// depth
			readToInt32(int32buffer, input, bigEndian);
			rectangle->depth = int32buffer;

			levelGeometry.push_back(rectangle);
		}
		else {
			std::cout << "UNKNOWN SHAPE!" << std::endl;
		}
	}
}

/* Returns collection of geometric shapes loaded into memory. */
std::vector<GeometricShape*>& GeometryProvider::getLevelGeometry()
{
	return levelGeometry;
}

/* Returns collection of all Circle shapes loaded into memory. */
std::vector<Circle> GeometryProvider::getCircles()
{
	std::vector<Circle> circles;

	for (auto s : levelGeometry) {
		Circle *ptr = dynamic_cast<Circle*>(s);
		if (ptr) {
			circles.push_back(*ptr);
		}
	}

	return circles;
}

/* Returns collection of all Rectangle shapes loaded into memory. */
std::vector<Rectangle> GeometryProvider::getRectangles()
{
	std::vector<Rectangle> rectangles;

	for (auto s : levelGeometry) {
		Rectangle *ptr = dynamic_cast<Rectangle*>(s);
		if (ptr) {
			rectangles.push_back(*ptr);
		}
	}

	return rectangles;
}

/* Reads an int32 value from the given stream into the given buffer and handles endianness conversion. */
void GeometryProvider::readToInt32(int32_t & buffer, std::ifstream & stream, bool & bigEndian)
{
	stream.read((char*)&buffer, sizeof(buffer));

	// reverse endianness to make it little-endian
	if (bigEndian) {
		boost::endian::endian_reverse_inplace(buffer);
	}
}

void GeometryProvider::loadStaticGeometry()
{
	// clear geometry first
	levelGeometry.clear();

	// load static data (circles)
	levelGeometry.push_back(new Circle(8800, -3300, 200));
	levelGeometry.push_back(new Circle(-5800, 8900, 600));
	levelGeometry.push_back(new Circle(-6600, 1000, 900));
	levelGeometry.push_back(new Circle(-100, -1400, 700));
	levelGeometry.push_back(new Circle(-6900, 800, 700));
	levelGeometry.push_back(new Circle(7800, 600, 400));
	levelGeometry.push_back(new Circle(3700, -9700, 200));
	levelGeometry.push_back(new Circle(3400, -2700, 800));
	levelGeometry.push_back(new Circle(-6500, -3300, 600));
	levelGeometry.push_back(new Circle(-400, -2700, 300));
	levelGeometry.push_back(new Circle(-2500, -4800, 200));
	levelGeometry.push_back(new Circle(5800, -8000, 300));
	levelGeometry.push_back(new Circle(-5000, 7800, 900));
	levelGeometry.push_back(new Circle(-4100, 8100, 200));
	levelGeometry.push_back(new Circle(5700, -200, 200));
	levelGeometry.push_back(new Circle(500, 2700, 500));
	levelGeometry.push_back(new Circle(-8300, 5800, 700));
	levelGeometry.push_back(new Circle(-700, 7700, 600));
	levelGeometry.push_back(new Circle(-1100, 5700, 900));
	levelGeometry.push_back(new Circle(4900, 900, 800));
	levelGeometry.push_back(new Circle(-3800, -8800, 800));
	levelGeometry.push_back(new Circle(-3500, -2400, 200));
	levelGeometry.push_back(new Circle(1700, 700, 600));
	levelGeometry.push_back(new Circle(2000, 3500, 300));
	levelGeometry.push_back(new Circle(3900, 700, 800));

	// load static data (rectangles)
	levelGeometry.push_back(new Rectangle(-8700, -3700, 800, 900));
	levelGeometry.push_back(new Rectangle(1800, 7300, 1000, 800));
	levelGeometry.push_back(new Rectangle(-2300, 6900, 600, 800));
	levelGeometry.push_back(new Rectangle(-6400, -4600, 1100, 700));
	levelGeometry.push_back(new Rectangle(5800, 2500, 700, 1100));
	levelGeometry.push_back(new Rectangle(-3200, -2800, 1100, 500));
	levelGeometry.push_back(new Rectangle(-7400, 8900, 500, 900));
	levelGeometry.push_back(new Rectangle(8200, 1900, 400, 600));
	levelGeometry.push_back(new Rectangle(-9900, -6900, 400, 700));
	levelGeometry.push_back(new Rectangle(-10000, 7700, 800, 1100));
	levelGeometry.push_back(new Rectangle(1600, 1800, 700, 600));
	levelGeometry.push_back(new Rectangle(-10000, -9500, 700, 400));
}

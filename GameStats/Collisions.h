#pragma once

#include "FoundRecord.h"
#include "GeometryProvider.h"
#include "Circle.h"
#include "Rectangle.h"

#include <boost/geometry/geometry.hpp>

typedef boost::geometry::model::d2::point_xy<double> point;
typedef boost::geometry::model::polygon<point> polygon;
typedef boost::geometry::model::multi_polygon<polygon> multipoly;

/* Helper class for collision detection operations. */
class Collisions
{
public:
	Collisions() = default;
	bool inPartialCover(FoundRecord kill, std::vector<polygon> polygons);
	polygon getVisionCone(FoundRecord kill);
	polygon getPolygon(Circle circle);
	polygon getPolygon(Rectangle rectangle);
	static bool overlapCheck(polygon p1, polygon p2);
};


#include "Collisions.h"

#include <boost/geometry/geometries/segment.hpp> 
#include <boost/geometry/algorithms/intersection.hpp>
#include <boost/geometry/io/io.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/algorithms/overlaps.hpp>

namespace bg = boost::geometry;

/* Returns true if the hider was in partial or full cover from seeker. */
bool Collisions::inPartialCover(FoundRecord record, std::vector<polygon> polygons)
{
	auto visionCone = getVisionCone(record);

	int objectsInWay = 0;
	for (auto p : polygons) {
		objectsInWay += overlapCheck(visionCone, p);
	}

	return objectsInWay > 0;
}

/* Returns a triangular polygon representing the cone of sight from the seeker's center to the "left side" and "right side" of the found hider with
respect to the seeker's position. */
polygon Collisions::getVisionCone(FoundRecord record) {

	// set v1 (vertex 1 of triangle)
	point v1{ (double) record.seekerPos.first, (double) record.seekerPos.second };

	// get vector from hider to seeker
	bg::model::point<double, 2, bg::cs::cartesian> hiderPos(record.foundHiderPos.first, record.foundHiderPos.second);
	bg::model::point<double, 2, bg::cs::cartesian> seekerPos(record.seekerPos.first, record.seekerPos.second);
	auto vectorToSeeker = seekerPos; // copy seekerPos because subtract_point modifies first parameter
	bg::subtract_point(vectorToSeeker, hiderPos);
	bg::detail::vec_normalize(vectorToSeeker);

	// find vector perpendicular to vectorToSeeker
	auto holdY = vectorToSeeker.get<1>();					// store old Y
	vectorToSeeker.set<1>(0.0 - vectorToSeeker.get<0>());	// set Y to X and negate it
	vectorToSeeker.set<0>(holdY);							// set X to Y
	
	//multiply by 50 (cm radii, the "mass" of the found player)
	bg::multiply_value(vectorToSeeker, 50);

	// set v2 (vertex 2 of triangle, the seeker's "left" side)
	point v2{
		vectorToSeeker.get<0>() + hiderPos.get<0>(),
		vectorToSeeker.get<1>() + hiderPos.get<1>()
	};

	// get opposite-facing vector
	bg::multiply_value(vectorToSeeker, -1);

	// set v3 (vertex 3 of triangle, the seeker's "right" side)
	point v3{
		vectorToSeeker.get<0>() + hiderPos.get<0>(),
		vectorToSeeker.get<1>() + hiderPos.get<1>()
	};

	polygon triangle;
	boost::geometry::append(triangle, v1);
	boost::geometry::append(triangle, v2);
	boost::geometry::append(triangle, v3);
	boost::geometry::append(triangle, v1);

	return triangle;
}

/* Returns a polygon representing the given Circle. */
polygon Collisions::getPolygon(Circle circle)
{
	const double buffer_distance = circle.radius; // radius of circle
	const int points_per_circle = 36;
	boost::geometry::strategy::buffer::distance_symmetric<double> distance_strategy(buffer_distance);
	boost::geometry::strategy::buffer::join_round join_strategy(points_per_circle);
	boost::geometry::strategy::buffer::end_round end_strategy(points_per_circle);
	boost::geometry::strategy::buffer::point_circle circle_strategy(points_per_circle);
	boost::geometry::strategy::buffer::side_straight side_strategy;

	boost::geometry::model::multi_polygon<polygon> result;

	point pt{ (double)circle.center.first, (double)circle.center.second }; // center of circle

	boost::geometry::buffer(pt, result,
		distance_strategy, side_strategy,
		join_strategy, end_strategy, circle_strategy);

	return result.front();
}

/* Returns a polygon representing the given Rectangle. */
polygon Collisions::getPolygon(Rectangle rectangle)
{
	double centerX = rectangle.center.first * 1.0;
	double centerY = rectangle.center.second * 1.0;

	double offsetX = rectangle.width / 2.0;
	double offsetY = rectangle.depth / 2.0;

	// start with bottom-left
	point v1{ centerX - offsetX, centerY - offsetY };
	// clockwise to top-left
	point v2{ centerX - offsetX, centerY + offsetY };
	// top-right
	point v3{ centerX + offsetX, centerY + offsetY };
	// bottom-right
	point v4{ centerX + offsetX, centerY - offsetY };

	polygon poly;
	boost::geometry::append(poly, v1);
	boost::geometry::append(poly, v2);
	boost::geometry::append(poly, v3);
	boost::geometry::append(poly, v4);
	boost::geometry::append(poly, v1);

	return poly;
}

/* Returns true if the two polygon shapes overlap each other. */
bool Collisions::overlapCheck(polygon p1, polygon p2)
{
	return boost::geometry::overlaps(p1, p2);
}

#include "FoundRecord.h"

#include <iostream>
#include <boost/geometry.hpp>
#include <boost/geometry/arithmetic/arithmetic.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/qvm/vec.hpp>
#include <boost/qvm/vec_operations.hpp>

namespace bg = boost::geometry;

FoundRecord::FoundRecord(int seekerX, int seekerY, int seekerRotation, int hiderX, int hiderY, int hiderRotation, int secondsUntilFound)
{
	seekerPos.first = seekerX;
	seekerPos.second = seekerY;
	this->seekerRotation = seekerRotation;
	foundHiderPos.first = hiderX;
	foundHiderPos.second = hiderY;
	foundHiderRotation = hiderRotation;
	this->secondsUntilFound = secondsUntilFound;
}

void FoundRecord::print()
{
	std::cout << "---FoundRecord---" << std::endl;
	std::cout << "seekerPos=(" << seekerPos.first << ", " << seekerPos.second << ")" << std::endl;
	std::cout << "seekerRotation=" << seekerRotation << std::endl;
	std::cout << "foundHiderPos=(" << foundHiderPos.first << ", " << foundHiderPos.second << ")" << std::endl;
	std::cout << "foundHiderRotation=" << foundHiderRotation << std::endl;
	std::cout << "secondsUntilFound=" << secondsUntilFound << std::endl;
}

/* Returns true if the hider was facing the seeker within a 90 degree window. */
bool FoundRecord::foundFacingSeeker() const
{
	// get vector from hider to seeker
	bg::model::point<double, 2, bg::cs::cartesian> hiderPos(foundHiderPos.first, foundHiderPos.second);
	bg::model::point<double, 2, bg::cs::cartesian> seekerPos(seekerPos.first, seekerPos.second);

	auto vectorToSeeker = seekerPos; // copy seekerPos because subtract_point modifies first parameter
	bg::subtract_point(vectorToSeeker, hiderPos);
	bg::detail::vec_normalize(vectorToSeeker);

	// find vector representing hider's heading
	auto unit = getUnitVectorFromRotation(foundHiderRotation);
	bg::model::point<double, 2, bg::cs::cartesian> hiderFacing(unit.first, unit.second);

	// get dot product of vector from hider to seeker and facing direction of the hider
	auto dot = bg::dot_product(vectorToSeeker, hiderFacing);

	// if dot <= (1 / sqrt(2)) then its in sight
	bool seekerInSight = dot <= (1 / sqrt(2));

	return seekerInSight;
}

/* Returns a unit vector representation of the given rotation (where 0 is 0 degrees and 1024 is 360 degrees). */
std::pair<double, double> FoundRecord::getUnitVectorFromRotation(int32_t rotation) const
{
	double radians = rotation * 2 * boost::math::constants::pi<double>() / 1024;

	std::pair<double, double> unitVector(cos(radians), sin(radians));

	return unitVector;
}

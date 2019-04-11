#pragma once

#include <cstdint>
#include <utility>

/* Represents an incident of when a Hider was found. */
class FoundRecord
{
public:
	FoundRecord() = default;
	FoundRecord(int seekerX, int seekerY, int seekerRotation, int hiderX, int hiderY, int hiderRotation, int secondsUntilFound);
	std::pair<int32_t, int32_t> seekerPos;
	int32_t seekerRotation;
	std::pair<int32_t, int32_t> foundHiderPos;
	int32_t foundHiderRotation;
	int32_t secondsUntilFound;
	void print();
	bool foundFacingSeeker() const;
private:
	std::pair<double, double> getUnitVectorFromRotation(int32_t rotation) const;
};
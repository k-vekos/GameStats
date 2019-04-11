#include <iostream>

#include "FoundRecordProvider.h"
#include "GeometryProvider.h"
#include "Collisions.h"

long sampleCenterX;
long sampleCenterY;
long sampleWidth;
long sampleHeight;
long outputResX;
long outputResY;

std::vector<int> *foundGrid = nullptr;
std::vector<int> *sawSeekerGrid = nullptr;
std::vector<int> *inCoverGrid = nullptr;

int foundFacingSeekerCount = 0;
int inPartialCoverCount = 0;

/* Prints a given heatmap grid to the console window. */
void printGrid(std::vector<int> * grid) {
	for (int y = (outputResY - 1); y >= 0; y--) {
		for (int x = 0; x < outputResX; x++) {
			std::cout << (*grid)[y * outputResX + x];

			// if not last in row, put a ","
			if (x != outputResX - 1)
				std::cout << ", ";
		}
		std::cout << std::endl;
	}
}

/* Populates all three heatmaps using the given found records, polygon cache, and collisions helper. */
void generateHeatmaps(std::vector<FoundRecord> foundRecords, std::vector<polygon> polygonCache, Collisions collisions) {
	int fromBoundLeft		= sampleCenterX - (sampleWidth / 2);
	int fromBoundRight		= sampleCenterX + (sampleWidth / 2);
	int fromBoundTop		= sampleCenterY + (sampleHeight / 2);
	int fromBoundBottom		= sampleCenterY - (sampleHeight / 2);
	
	int toBoundRight		= outputResX - 1;
	int toBoundTop			= outputResY - 1;

	// calculate the X/Y slopes
	double slopeX = 1.0 * (toBoundRight - 0) / (fromBoundRight - fromBoundLeft);
	double slopeY = 1.0 * (toBoundTop - 0) / (fromBoundTop - fromBoundBottom);

	for (auto & r : foundRecords) {
		// use the slopes to map dead position into output grid range
		double x = 0 + round(slopeX * (r.foundHiderPos.first - fromBoundLeft));
		double y = 0 + round(slopeY * (r.foundHiderPos.second - fromBoundBottom));

		// add the finding to the found grid
		(*foundGrid)[(int)roundf(y) * outputResX + (int)roundf(x)] += 1;

		bool foundFacingSeeker = r.foundFacingSeeker();
		// add to "facing seeker" count
		foundFacingSeekerCount += foundFacingSeeker;
		// add finding to the sawSeekerGrid (only increments if they WERE facing seeker)
		(*sawSeekerGrid)[(int)roundf(y) * outputResX + (int)roundf(x)] += foundFacingSeeker;

		bool inPartialCover = collisions.inPartialCover(r, polygonCache);
		// add to "in partial cover" count
		inPartialCoverCount += inPartialCover;
		// add finding to the inCoverGrid (only increments if they WERE in partial cover)
		(*inCoverGrid)[(int)roundf(y) * outputResX + (int)roundf(x)] += inPartialCover;
	}
}

int main(int argc, char *argv[])
{
	// Example input
	// "0 0 5000 5000 5 5"
	// All-encompassing example input
	// "-100 -100 13400 13800 24 24"

	// capture input args
	char *p;
	sampleCenterX	= strtol(argv[1], &p, 10);
	sampleCenterY	= strtol(argv[2], &p, 10);
	sampleWidth		= strtol(argv[3], &p, 10);
	sampleHeight	= strtol(argv[4], &p, 10);
	outputResX		= strtol(argv[5], &p, 10);
	outputResY		= strtol(argv[6], &p, 10);

	std::cout << "--- START ---" << std::endl;

	FoundRecordProvider findings;
	GeometryProvider geometry;
	Collisions collisions;

	// load found records data
	findings.loadStaticRecords();

	// get found records from specified sample area ONLY
	auto records = findings.getFoundRecords(sampleCenterX, sampleCenterY, sampleWidth, sampleHeight);

	// load static geometry data
	geometry.loadStaticGeometry();

	// initialize output grids
	foundGrid = new std::vector<int>(outputResX * outputResY, 0);
	sawSeekerGrid = new std::vector<int>(outputResX * outputResY, 0);
	inCoverGrid = new std::vector<int>(outputResX * outputResY, 0);

	// create polygon cache
	std::vector<polygon> polygonCache;
	auto circles = geometry.getCircles();
	auto rectangles = geometry.getRectangles();
	for (auto & c : circles) {
		polygonCache.push_back(collisions.getPolygon(c));
	}
	for (auto & r : rectangles) {
		polygonCache.push_back(collisions.getPolygon(r));
	}

	// generate the heatmaps (populates the output grids)
	generateHeatmaps(records, polygonCache, collisions);

	// print the counts
	std::cout << foundFacingSeekerCount << std::endl;
	std::cout << std::endl;
	std::cout << inPartialCoverCount << std::endl;
	std::cout << std::endl;
	// print the grids
	printGrid(foundGrid);
	std::cout << std::endl;
	printGrid(sawSeekerGrid);
	std::cout << std::endl;
	printGrid(inCoverGrid);
}
#include "FoundRecordProvider.h"
#include "FoundRecord.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <cstdint>
#include <boost/endian/conversion.hpp>

/* Loads geometry data from given file into memory. */
void FoundRecordProvider::loadFoundRecordsFile(std::string fileName)
{
	// clear records first
	foundRecords.clear();

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
		// read in first int32
		readToInt32(int32buffer, input, bigEndian);
		
		// check if we've hit EoF yet...
		if (input.eof())
			break;

		FoundRecord record;

		// #2 seekerPos X
		record.seekerPos.first = int32buffer;

		// #2 seekerPos Y
		readToInt32(int32buffer, input, bigEndian);
		record.seekerPos.second = int32buffer;

		// #3 seekerRotation
		readToInt32(int32buffer, input, bigEndian);
		record.seekerRotation = int32buffer;

		// #4 foundHiderPos X
		readToInt32(int32buffer, input, bigEndian);
		record.foundHiderPos.first = int32buffer;

		// #5 foundHiderPos Y
		readToInt32(int32buffer, input, bigEndian);
		record.foundHiderPos.second = int32buffer;

		// #6 foundHiderRotation
		readToInt32(int32buffer, input, bigEndian);
		record.foundHiderRotation = int32buffer;

		// #7 secondsUntilFound
		readToInt32(int32buffer, input, bigEndian);
		record.secondsUntilFound = int32buffer;

		foundRecords.push_back(record);
	}	
}

/* Returns all found records loaded into memory. */
std::vector<FoundRecord> FoundRecordProvider::getFoundRecords()
{
	std::vector<FoundRecord> inSampleRecords;

	for (auto & r : foundRecords) {
		inSampleRecords.push_back(r);
	}

	return inSampleRecords;
}

/* Returns all found records loaded into memory within the given sample area. */
std::vector<FoundRecord> FoundRecordProvider::getFoundRecords(long centerX, long centerY, long width, long height)
{
	std::vector<FoundRecord> inSampleRecords;

	long boundLeft		= centerX - (width	/ 2);
	long boundRight		= centerX + (width / 2);
	long boundTop		= centerY + (height / 2);
	long boundBottom	= centerY - (height / 2);

	for (auto r : foundRecords) {
		// if player was found inside the sample area
		if (
			r.foundHiderPos.first	>= boundLeft	&&
			r.foundHiderPos.first	<= boundRight	&&
			r.foundHiderPos.second	<= boundTop		&&
			r.foundHiderPos.second	>= boundBottom
		) {
			inSampleRecords.push_back(r);
		}
	}

	return inSampleRecords;
}

/* Reads an int32 value from the given stream into the given buffer and handles endianness conversion. */
void FoundRecordProvider::readToInt32(int32_t & buffer, std::ifstream & stream, bool & bigEndian)
{
	stream.read((char*)&buffer, sizeof(buffer));

	// reverse endianness to make it little-endian
	if (bigEndian) {		
		boost::endian::endian_reverse_inplace(buffer);
	}
}

void FoundRecordProvider::loadStaticRecords()
{
	// clear records first
	foundRecords.clear();

	// load static data
	foundRecords.push_back(FoundRecord(-4000, -2000, 805, -5700, -1600, 555, 548));
	foundRecords.push_back(FoundRecord(1500, 8500, 540, 1200, 6800, 889, 80));
	foundRecords.push_back(FoundRecord(4000, 7400, 551, 3600, 5800, 297, 479));
	foundRecords.push_back(FoundRecord(200, 200, 95, 400, 500, 210, 551));
	foundRecords.push_back(FoundRecord(800, 1400, 939, 0, 2800, 39, 147));
	foundRecords.push_back(FoundRecord(3900, 3000, 881, 3300, 3500, 607, 328));
	foundRecords.push_back(FoundRecord(800, -3300, 132, 2800, -1400, 262, 312));
	foundRecords.push_back(FoundRecord(2500, -4000, 890, 900, -2500, 565, 174));
	foundRecords.push_back(FoundRecord(-1600, -3000, 216, 0, -2600, 963, 272));
	foundRecords.push_back(FoundRecord(3300, 100, 961, 2900, 1100, 198, 348));
	foundRecords.push_back(FoundRecord(2300, -3200, 900, 600, -1400, 684, 545));
	foundRecords.push_back(FoundRecord(3900, 5100, 9, 4000, 6800, 604, 474));
	foundRecords.push_back(FoundRecord(-3200, 3700, 616, -3800, 2900, 963, 262));
	foundRecords.push_back(FoundRecord(-3200, 200, 61, -2800, 1200, 826, 268));
	foundRecords.push_back(FoundRecord(-5500, 4300, 907, -6800, 5800, 426, 34));
	foundRecords.push_back(FoundRecord(4900, 4300, 715, 3700, 3900, 339, 199));
	foundRecords.push_back(FoundRecord(-3200, -4000, 152, -1300, -2600, 919, 415));
	foundRecords.push_back(FoundRecord(-6400, -4000, 538, -6600, -5200, 823, 151));
	foundRecords.push_back(FoundRecord(-3000, -1800, 735, -4000, -2000, 1017, 443));
	foundRecords.push_back(FoundRecord(-4600, -4600, 668, -5600, -5300, 266, 313));
	foundRecords.push_back(FoundRecord(4600, 2400, 672, 3700, 1800, 973, 67));
	foundRecords.push_back(FoundRecord(4900, -2200, 377, 6200, -3400, 914, 423));
	foundRecords.push_back(FoundRecord(-1500, -3900, 991, -1900, -1900, 831, 92));
	foundRecords.push_back(FoundRecord(-800, 200, 715, -2600, -400, 700, 237));
	foundRecords.push_back(FoundRecord(-1400, 3400, 820, -2900, 3900, 382, 564));
	foundRecords.push_back(FoundRecord(-100, 3600, 331, 1300, 2900, 916, 168));
	foundRecords.push_back(FoundRecord(-3600, -6400, 43, -3100, -4600, 625, 37));
	foundRecords.push_back(FoundRecord(-6500, 3400, 128, -6100, 3800, 20, 351));
	foundRecords.push_back(FoundRecord(-5600, 1200, 877, -6600, 2000, 720, 31));
	foundRecords.push_back(FoundRecord(-6000, 5300, 173, -5100, 5800, 909, 89));
	foundRecords.push_back(FoundRecord(800, -6000, 184, 2500, -5200, 183, 176));
	foundRecords.push_back(FoundRecord(2300, 4800, 879, 1200, 5700, 631, 171));
	foundRecords.push_back(FoundRecord(-500, -2300, 813, -1900, -1900, 174, 329));
	foundRecords.push_back(FoundRecord(4400, 4100, 128, 5100, 4800, 1005, 50));
	foundRecords.push_back(FoundRecord(400, 7400, 425, 1100, 6200, 5, 60));
	foundRecords.push_back(FoundRecord(-800, 6400, 603, -2000, 4500, 23, 436));
	foundRecords.push_back(FoundRecord(-600, -3000, 778, -2100, -2900, 970, 567));
	foundRecords.push_back(FoundRecord(-4500, -2000, 914, -4900, -1500, 141, 222));
	foundRecords.push_back(FoundRecord(2200, 4200, 256, 2500, 4200, 871, 328));
	foundRecords.push_back(FoundRecord(-1600, -6400, 402, -1200, -6900, 141, 46));
	foundRecords.push_back(FoundRecord(2800, -5400, 227, 4500, -5100, 852, 298));
	foundRecords.push_back(FoundRecord(8300, -5900, 862, 6500, -4700, 312, 187));
	foundRecords.push_back(FoundRecord(-3800, 1600, 896, -5400, 3200, 74, 181));
	foundRecords.push_back(FoundRecord(-6100, 900, 176, -4400, 1800, 892, 266));
	foundRecords.push_back(FoundRecord(4800, 3000, 389, 6200, 1500, 813, 401));
	foundRecords.push_back(FoundRecord(-5100, -900, 433, -4100, -2800, 212, 210));
	foundRecords.push_back(FoundRecord(4200, -6900, 143, 5800, -5600, 883, 444));
	foundRecords.push_back(FoundRecord(6800, 700, 942, 6200, 1800, 87, 400));
	foundRecords.push_back(FoundRecord(-1300, -1200, 672, -1600, -1400, 84, 192));
	foundRecords.push_back(FoundRecord(2000, -4000, 991, 1600, -2000, 308, 440));
	foundRecords.push_back(FoundRecord(-100, -6000, 934, -1200, -4200, 901, 182));
	foundRecords.push_back(FoundRecord(4000, -5800, 54, 4600, -4100, 69, 377));
	foundRecords.push_back(FoundRecord(-6100, 5700, 436, -5900, 5300, 901, 391));
	foundRecords.push_back(FoundRecord(1900, 1400, 207, 3200, 1800, 53, 562));
	foundRecords.push_back(FoundRecord(5700, 1600, 37, 6100, 3300, 519, 281));
	foundRecords.push_back(FoundRecord(-3800, -6900, 265, -2000, -7000, 235, 145));
	foundRecords.push_back(FoundRecord(6400, -200, 948, 6000, 600, 113, 404));
	foundRecords.push_back(FoundRecord(-7200, 4100, 244, -5600, 4200, 352, 141));
	foundRecords.push_back(FoundRecord(-4700, -600, 987, -5000, 700, 67, 80));
	foundRecords.push_back(FoundRecord(2200, 2400, 132, 4100, 4200, 243, 60));
	foundRecords.push_back(FoundRecord(4900, -6700, 167, 6400, -5800, 77, 272));
	foundRecords.push_back(FoundRecord(3300, -100, 331, 3900, -400, 105, 198));
	foundRecords.push_back(FoundRecord(-5000, 3400, 375, -4000, 2500, 515, 352));
	foundRecords.push_back(FoundRecord(6800, -5200, 888, 5600, -4100, 452, 371));
	foundRecords.push_back(FoundRecord(-7100, -1200, 77, -6100, 700, 929, 279));
	foundRecords.push_back(FoundRecord(6600, 5400, 0, 6600, 5800, 518, 144));
	foundRecords.push_back(FoundRecord(100, -2500, 407, 700, -3300, 332, 262));
	foundRecords.push_back(FoundRecord(4500, -3500, 207, 5800, -3100, 653, 181));
	foundRecords.push_back(FoundRecord(-3200, 2200, 948, -4200, 4200, 431, 555));
	foundRecords.push_back(FoundRecord(5200, 4400, 427, 5600, 3700, 1019, 19));
	foundRecords.push_back(FoundRecord(-700, -4500, 308, -400, -4600, 6, 409));
	foundRecords.push_back(FoundRecord(1300, -1700, 862, 100, -900, 578, 588));
	foundRecords.push_back(FoundRecord(600, -1200, 399, 2000, -2900, 725, 317));
	foundRecords.push_back(FoundRecord(-6300, 2100, 967, -6700, 3200, 828, 381));
	foundRecords.push_back(FoundRecord(-900, 4100, 851, -2500, 5000, 9, 533));
	foundRecords.push_back(FoundRecord(2400, -2200, 616, 1800, -3000, 123, 118));
	foundRecords.push_back(FoundRecord(-4100, -4600, 472, -3800, -5800, 917, 48));
	foundRecords.push_back(FoundRecord(-3200, -7300, 909, -4900, -5300, 440, 229));
	foundRecords.push_back(FoundRecord(-500, -6300, 922, -1800, -4500, 1015, 50));
	foundRecords.push_back(FoundRecord(3300, -4400, 256, 4400, -4400, 312, 117));
}
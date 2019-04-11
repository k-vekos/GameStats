#pragma once

#include <string>
#include <vector>

#include "FoundRecord.h"

/* Helper class for found record related operations including loading records from file. */
class FoundRecordProvider
{
public:
	FoundRecordProvider() = default;
	void loadStaticRecords();
	void loadFoundRecordsFile(std::string fileName);
	std::vector<FoundRecord> getFoundRecords();
	std::vector<FoundRecord> getFoundRecords(long centerX, long centerY, long width, long height);

private:
	std::vector<FoundRecord> foundRecords;
	void readToInt32(int32_t & buffer, std::ifstream & stream, bool & bigEndian);
};


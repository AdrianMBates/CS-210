/*
 * AnimalRecordData.h
 *
 *  Created on: Feb 17, 2021
 *      Author: 1757275_snhu
 */

#ifndef ANIMALRECORDDATA_H_
#define ANIMALRECORDDATA_H_

#include <string>

class AnimalRecordData {
public:
	AnimalRecordData();
	AnimalRecordData(std::string trackNumber, std::string name, std::string type, std::string subType, int eggs, int nurse);
	virtual ~AnimalRecordData();

	std::string trackNumber;
	std::string name;
	std::string type;
	std::string subType;
	int eggs;
	int nurse;
};

#endif /* ANIMALRECORDDATA_H_ */

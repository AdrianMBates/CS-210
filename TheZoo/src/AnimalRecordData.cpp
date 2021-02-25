/*
 * AnimalRecordData.cpp
 *
 *  Created on: Feb 17, 2021
 *      Author: 1757275_snhu
 */

#include "AnimalRecordData.h"

AnimalRecordData::AnimalRecordData(){
	this->trackNumber = -1;
		this->name = "";
		this->type = "";
		this->subType = "";
		this->eggs = -1;
		this->nurse = -1;
}

AnimalRecordData::AnimalRecordData(std::string trackNumber, std::string name, std::string type, std::string subType, int eggs, int nurse) {
	this->trackNumber = trackNumber;
	this->name = name;
	this->type = type;
	this->subType = subType;
	this->eggs = eggs;
	this->nurse = nurse;
}

AnimalRecordData::~AnimalRecordData() {

}



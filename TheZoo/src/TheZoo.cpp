#include <iostream>
#include <iomanip>
#include <jni.h>
#include <fstream>
#include <vector>
#include <conio.h>

#include "AnimalRecordData.h"

using namespace std;

/*
 * Utility functions
 */

//Waits for user input to continue.
void waitForInput() {
	cin.ignore();

	do {
		cout << "Press enter to continue..." << endl;
	} while (cin.get() != '\n');
}

//Returns paddedString without spaces following content
string getStringWithoutPadding(string paddedString) {
	string stringWithoutPadding;
	//									  from 0 to first space character
	stringWithoutPadding = paddedString.substr(0, paddedString.find_first_of(' ', 0));
	return stringWithoutPadding;
}

//Gets input repeatedly until input is properly verified as int
int getVerifiedInt() {
	int verifiedInt;
	//assign to out variable
	cin >> verifiedInt;
	//get state of stream
	bool success = (bool) cin;
	//while the type of input is not correct
	while (!success) {
		//clear the input
		std::cin.clear();
		std::cin.ignore(100000, '\n');

		std::cout << "Invalid Input. Try again.";
		cout << endl;

		//retry getting input
		std::cin >> verifiedInt;
		//get the state of stream
		success = (bool) cin;
	}
	return verifiedInt;
}

//Gets input repeatedly until input is properly verified as either 0 or 1
int getVerifiedIntZeroOne() {
	int verifiedInt;
	//assign to out variable
	cin >> verifiedInt;
	//get state of stream
	bool success = (bool) cin;
	//get whether int is one or zero
	bool oneOrZero = (verifiedInt == 0 || verifiedInt == 1);
	//while the type of input is not correct
	while (!success || !oneOrZero) {
		//clear the input
		std::cin.clear();
		std::cin.ignore(100000, '\n');

		std::cout << "Invalid Input. Try again.";
		cout << endl;

		//retry getting input
		std::cin >> verifiedInt;
		//get the state of stream
		success = (bool) cin;
		//get whether int is one or zero
		oneOrZero = (verifiedInt == 0 || verifiedInt == 1);
	}
	return verifiedInt;
}

//Gets input repeatedly until input is properly verified as string smaller or equal to maxLength characters
string getVerifiedString(int maxLength) {
	string verifiedString;
	//assign to out variable
	cin >> verifiedString;
	//get state of stream
	bool success = (bool) cin;
	//get whether string is too long
	bool tooLong = verifiedString.length() > (size_t) maxLength;
	//while the type of input is not correct
	while (!success || tooLong) {
		//clear the input
		std::cin.clear();
		std::cin.ignore(100000, '\n');

		//output either invalid or too long
		std::cout << (!success ? "Invalid Input. Try again. \n" : "Input too long. Try again. \n");
		cout << endl;

		//retry getting input
		std::cin >> verifiedString;
		//get the state of stream
		success = (bool) cin;
		//get whether string is too long
		tooLong = verifiedString.length() > (size_t) maxLength;
	}
	return verifiedString;
}

//writes animal data in columns to specified ostream, padding as required by data format
void writeSingleAnimalData(AnimalRecordData data, ostream& stream) {
	stream << data.trackNumber << " ";
	stream << setfill(' ') << setw(17) << data.name;
	stream << setw(17) << data.type;
	stream << setw(17) << data.subType;
	stream << data.eggs << " ";
	stream << data.nurse;
	stream << endl;
}

void GenerateData()               //DO NOT TOUCH CODE IN THIS METHOD
{
	JavaVM *jvm;                    // Pointer to the JVM (Java Virtual Machine)
	JNIEnv *env;                      // Pointer to native interface
									  //================== prepare loading of Java VM ============================
	JavaVMInitArgs vm_args;                        // Initialization arguments
	JavaVMOption* options = new JavaVMOption[1];   // JVM invocation options
	options[0].optionString = (char*) "-Djava.class.path="; // where to find java .class
	vm_args.version = JNI_VERSION_1_6;             // minimum Java version
	vm_args.nOptions = 1;                          // number of options
	vm_args.options = options;
	vm_args.ignoreUnrecognized = false; // invalid options make the JVM init fail
										//=============== load and initialize Java VM and JNI interface =============
	jint rc = JNI_CreateJavaVM(&jvm, (void**) &env, &vm_args);  // YES !!
	delete options;    // we then no longer need the initialisation options.
	if (rc != JNI_OK) {
		// TO DO: error processing...
		cin.get();
		exit(EXIT_FAILURE);
	}
	//=============== Display JVM version =======================================
	cout << "JVM load succeeded: Version ";
	jint ver = env->GetVersion();
	cout << ((ver >> 16) & 0x0f) << "." << (ver & 0x0f) << endl;

	jclass cls2 = env->FindClass("ZooFileWriter");  // try to find the class
	if (cls2 == nullptr) {
		cerr << "ERROR: class not found !";
	} else {                                  // if class found, continue
		cout << "Class MyTest found" << endl;
		jmethodID mid = env->GetStaticMethodID(cls2, "createZooFile", "()V"); // find method
		if (mid == nullptr)
			cerr << "ERROR: method void createZooFile() not found !" << endl;
		else {
			env->CallStaticVoidMethod(cls2, mid);                 // call method
			cout << endl;
		}
	}

	jvm->DestroyJavaVM();
	cin.get();
}

//Gets verified user input (as specified by data format) specifying a new AnimalRecordData and adds it to dataToBeModified
void AddAnimal(vector<AnimalRecordData>& dataToBeModifed) {
	AnimalRecordData newRecord;

	cout << "Track Number:" << endl;
	newRecord.trackNumber = getVerifiedString(7);

	//if the user inputs a number less than 7 characters long (without leading zeros)
	if (newRecord.trackNumber.length() < 7) {
		//pad the string with leading zeros until length = 7
		newRecord.trackNumber.insert(0, 7 - newRecord.trackNumber.length(), '0');
	}

	cout << "Animal Name:" << endl;
	newRecord.name = getVerifiedString(15);

	cout << "Animal Type:" << endl;
	newRecord.type = getVerifiedString(15);

	cout << "Animal Sub-Type:" << endl;
	newRecord.subType = getVerifiedString(15);

	cout << "Number of Eggs:" << endl;
	newRecord.eggs = getVerifiedInt();

	cout << "Enter 1 if Nursing, 0 if not:" << endl;
	newRecord.nurse = getVerifiedIntZeroOne();

	writeSingleAnimalData(newRecord, cout);
	cout << "Save this data? (1 for yes, 0 for no)" << endl;

	//if user inputs 1, add record
	if (getVerifiedIntZeroOne() == 1) {
		dataToBeModifed.push_back(newRecord);
		cout << "Record added successfully." << endl;
	//if user inputs 0, cancel addition
	} else {
		cout << "Record addition canceled." << endl;
	}

	waitForInput();
}

//Removes AnimalRecordData with track number specified by user from dataToBeModifed
void RemoveAnimal(vector<AnimalRecordData>& dataToBeModified) {
	string trackNumberForDeletion;
	cout << "Enter track number for deletion: " << endl;
	trackNumberForDeletion = getVerifiedString(7);

	//if the user inputs a number less than 7 characters long (without leading zeros)
	if (trackNumberForDeletion.length() < 7) {
		//pad the string with leading zeros until length = 7
		trackNumberForDeletion.insert(0, 7 - trackNumberForDeletion.length(), '0');
	}

	//for every individual record
	for (int i = 0; (size_t) i < dataToBeModified.size(); i++) {
		//if the trackNumber matches the user specification
		if (dataToBeModified.at(i).trackNumber == trackNumberForDeletion) {
			//output selected record
			writeSingleAnimalData(dataToBeModified.at(i), cout);
			cout << "Are you sure you want to delete this record? (1 for yes, 0 for no)" << endl;

			//if the user enters 1, delete the record
			if (getVerifiedIntZeroOne() == 1) {
				dataToBeModified.erase(dataToBeModified.begin() + i);
				cout << "Record deleted successfully." << endl;
			//if the user enters 0, cancel deletion
			} else {
				cout << "Record deletion canceled." << endl;
			}

			waitForInput();
			return;
		}
	}

	//if the loop never finds the track number
	cout << "Track number not found." << endl;
	waitForInput();
}

//Loads individual lines(individual records) from zoodata.txt into returnData, a collection of records.
void LoadDataFromFile(vector<AnimalRecordData>& returnData) {
	ifstream inFS;
	//for getting a full line of data
	string line;
	//for splitting that line up into chunks
	string subLine;
	//for finding the space after the egg integer field, since it could be of any length
	int spaceIndex;

	AnimalRecordData tempData;
	vector<AnimalRecordData> fileData;

	inFS.open("zoodata.txt");

	//if file doesn't exist
	if (!inFS.is_open()) {
		cout << "zoodata.txt could not be found." << endl;
		inFS.close();
		return;
	}

	//read entire file, line by line
	while (getline(inFS, line)) {
		//first column
		subLine = line.substr(0, 8);
		tempData.trackNumber = getStringWithoutPadding(subLine);
		//second column
		subLine = line.substr(8, 17);
		tempData.name = getStringWithoutPadding(subLine);
		//third column
		subLine = line.substr(25, 17);
		tempData.type = getStringWithoutPadding(subLine);
		//fourth column
		subLine = line.substr(42, 17);
		tempData.subType = getStringWithoutPadding(subLine);
		//fifth column
		//find first space after integer
		spaceIndex = line.find_first_of(' ', 59);
		//get entire integer
		subLine = line.substr(59, spaceIndex - 59);
		tempData.eggs = stoi(getStringWithoutPadding(subLine));
		//sixth column
		subLine = line.substr(spaceIndex + 1, 1);
		tempData.nurse = stoi(getStringWithoutPadding(subLine));

		fileData.push_back(tempData);
	}

	//if there was an issue reading the file
	if (inFS.bad()) {
		cout << "Error reading file." << endl;
		inFS.close();
		return;
	}

	inFS.close();

	//set successfully read data to return data
	returnData = fileData;
	cout << "Load complete." << endl;

	waitForInput();
}

//Saves data from saveData to zoodata.txt, according to data format
void SaveDataToFile(vector<AnimalRecordData>& saveData) {
	//if there's no data in saveData
	if (saveData.size() <= 0) {
		cout << "No data to save. Are you sure you want to delete all saved data? (1 for yes, 0 for no)" << endl;
		//if user enters 1, continue with function and save empty data.
		if (getVerifiedIntZeroOne() == 1) {
			cout << "Deleting data..." << endl;
		//if user enters 0, abort saving/return
		} else {
			cout << "Data deletion cancelled." << endl;
			waitForInput();
			return;
		}
	}

	ofstream outFS;

	outFS.open("zoodata.txt");

	//if file doesn't exist
	if (!outFS.is_open()) {
		cout << "zoodata.txt could not be found." << endl;
		outFS.close();
		return;
	}

	//align content to the left
	outFS << left;

	//write all data to outFS
	for (int i = 0; (size_t) i < saveData.size(); i++) {
		writeSingleAnimalData(saveData.at(i), outFS);
	}

	outFS.close();

	cout << "Save successfully completed." << endl;

	waitForInput();
}

//Displays all records in data line by line as per data format
void DisplayAnimalData(vector<AnimalRecordData> data) {
	//if there's no data
	if (data.size() <= 0) {
		cout << "Data not currently loaded. Try loading data first." << endl;
		waitForInput();
		return;
	}

	//align to the left
	cout << left;

	//write all data to console
	for (int i = 0; (size_t) i < data.size(); i++) {
		writeSingleAnimalData(data.at(i), cout);
	}

	waitForInput();
}

//Displays menu options
void DisplayMenu() {
	printf("--------------------\n");
	printf("%-20s (1) \n", "Load Animal Data ");
	printf("%-20s (2) \n", "Generate Data");
	printf("%-20s (3) \n", "Display Animal Data");
	printf("%-20s (4) \n", "Add Record");
	printf("%-20s (5) \n", "Delete Record");
	printf("%-20s (6) \n", "Save Animal Data");
	printf("%-20s (7) \n", "Quit");
}

int main() {
	int userInput;
	bool quit = false;
	vector<AnimalRecordData> savedData;

	while (!quit) {
		DisplayMenu();

		//Input validation
		userInput = getVerifiedInt();

		//User selection options
		switch (userInput) {
		case 1:
			LoadDataFromFile(savedData);
			break;
		case 2:
			GenerateData();
			break;
		case 3:
			DisplayAnimalData(savedData);
			break;
		case 4:
			AddAnimal(savedData);
			break;
		case 5:
			RemoveAnimal(savedData);
			break;
		case 6:
			SaveDataToFile(savedData);
			break;
		case 7:
			quit=true;
			break;
		default:
			printf("Invalid menu option. Please try again.");
			continue;
		}
	}
	return 1;
}

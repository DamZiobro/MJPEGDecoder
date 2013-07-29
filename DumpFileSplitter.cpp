//--------------------------------------------------------------------------------------------------

/*
 * DumpFileSplitter.cpp
 *
 *  Created on: Sep 8, 2011
 *      Author: Ziobro Damian
 */

//--------------------------------------------------------------------------------------------------

#include "DumpFileSplitter.h"
#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <vector>
#include <ctype.h>

using namespace std;

//--------------------------------------------------------------------------------------------------

DumpFileSplitter::DumpFileSplitter() {
	this->extensionForOutputFiles = (char*)"txt";
	this->isSplitterAtTheBeginning = false;
	this->directoryName = (char*)DEFAULT_DIRECTORY_NAME;
	this->numberOfSplittedParts = 0;
}

//--------------------------------------------------------------------------------------------------

DumpFileSplitter::~DumpFileSplitter() {
	if(extensionForOutputFiles != NULL){
		delete extensionForOutputFiles;
		extensionForOutputFiles = NULL;
	}
	if(fileToAnalyze != NULL){
		delete fileToAnalyze;
		fileToAnalyze = NULL;
	}
}

//--------------------------------------------------------------------------------------------------

void DumpFileSplitter::SetExtensionForOutputFiles(char* extension){
	this->extensionForOutputFiles = extension;
}

//--------------------------------------------------------------------------------------------------

void DumpFileSplitter::SetFileToSplit(char* fileToSplit){
	this->fileToAnalyze = fileToSplit;
}

//--------------------------------------------------------------------------------------------------

void DumpFileSplitter::SetSplitterAtTheBeginning(bool atTheBeginning){
	this->isSplitterAtTheBeginning = atTheBeginning;
}

//--------------------------------------------------------------------------------------------------

void DumpFileSplitter::SetDirectoryNameForSplittedParts(char* directoryName){
	this->directoryName = directoryName;
}

//--------------------------------------------------------------------------------------------------

char* DumpFileSplitter::GetDirectoryNameForSplittedParts(){
	return this->directoryName;
}

//--------------------------------------------------------------------------------------------------

char* DumpFileSplitter::ConvertHexStringToString(char* hexToConvert){
	//function return NULL if hex format is inappropriate

	char* convertedText = new char();
	int numberOfCharacter = 0;
	unsigned char byte[2];

	//ommiting 0x at the beginning of hex string
	if((hexToConvert[0] == '0') && (hexToConvert[1] == 'x')){
		hexToConvert+=2;
	}

	int sizeOfHex = strlen(hexToConvert);
	if(!(sizeOfHex % 2)){
		for(int i=0;i<sizeOfHex;i+=2){
			byte[0] = tolower(hexToConvert[i]);
			byte[1] = tolower(hexToConvert[i+1]);
			//cout << "byte[0] before: " << (int)byte[0] << endl;
			//cout << "byte[1] before: " << (int)byte[1] << endl;
			for(int i=0;i<2;i++){
				if(byte[i] >= '0' && byte[i] <='9'){
					byte[i] = byte[i] - 48; // 48 is ASCII code of '0'
				} else if(byte[i] >= 'a' && byte[i] <='f'){
					byte[i] = byte[i] - 97 + 10; // 97 is ASCII code of 'a'
				} else {
					return NULL;
				}
			}

			//cout << "byte[0] after: " << (int)byte[0] << endl;
			//cout << "byte[1] after: " << (int)byte[1] << endl;
			//cout << "char to assign: " << (char)((byte[0] << 4) + byte[1]) << "; integer value: " << (int)((byte[0] << 4) + byte[1]) << endl;

			convertedText[numberOfCharacter] = (char)((byte[0] << 4) + byte[1]);
			numberOfCharacter++;
		}
	} else {
		return NULL;
	}
	return convertedText;
}

//--------------------------------------------------------------------------------------------------

int DumpFileSplitter::SplitFileByASCII(char* asciiSplitter){
	//checking if parameters set is appropriate
	if(fileToAnalyze == NULL){
		cout << "DumpFileSplitter::splitFileByASCII => File which should be split is not set. Program terminate...\n";
		return FILE_NOT_SET;
	}
	if(!(asciiSplitter != NULL)){
		cout << "DumpFileSplitter::splitFileByASCII => Splitter not defined. Program terminate\n";
		return EXIT_FAILURE;
	}

	//opening the file
	  ifstream analyzedFile;
	  analyzedFile.open(fileToAnalyze, ios::in | ios::binary);
	  if (analyzedFile.is_open())
	  {
			char* dataFromFile;
			analyzedFile.seekg(0,ifstream::end);
			int size=analyzedFile.tellg();
			analyzedFile.seekg(0);
			//cout << "Size of analyzed file: " << size << endl;

			dataFromFile = new char[ size ];
			analyzedFile.read( dataFromFile, size );

			int numberOfFile = 1;
			bool isFirstFile = true;
			std::string* splitterBuffer = new std::string();
			std::string* stringToWrite = new std::string();
			std::string* splitterString = new std::string();

			if(asciiSplitter != NULL){
				splitterString= new std::string(asciiSplitter);
			} else {
				cout << "DumpFileSplitter::splitFileByASCII => Neither splitting string nor splitting hex defined. Use -s or -h options. Program terminate\n";
				return EXIT_FAILURE;
			}

			//creating the directory
			if(strcmp(directoryName,DEFAULT_DIRECTORY_NAME)!=0){
				try{
					string* createDirCommand = new string("mkdir ");
					createDirCommand->append(directoryName);
					system(createDirCommand->c_str());
				} catch(char* str){
					cout << "Cannot create directory for splitted files... Program terminate...";
					return EXIT_FAILURE;
				}
			}

			//splitting file
			for ( int i = 0; i < size; i++ )
			{
				splitterBuffer->push_back(dataFromFile[i]);
				stringToWrite->push_back(dataFromFile[i]);
				if(splitterBuffer->size() > splitterString->size()){
					splitterBuffer->erase(splitterBuffer->begin());
				}

				//cout << "splitterBuffer: " << splitterBuffer->c_str() << endl;
				//cout << "spllitterString: " << splitterString->c_str() << endl;
				//cout << "stringToWrite: " << stringToWrite->c_str() << endl;
				if(splitterBuffer->compare(splitterString->c_str())== 0  || (i == size-1)){
					//ommiting creation first file which would have only splitterString inside
					if(isSplitterAtTheBeginning && isFirstFile){
						isFirstFile = false;
					} else {
						char* buffer = new char();
						sprintf(buffer,"%d",numberOfFile);

						//creating output filename
						string* outputFileName = new string();
						outputFileName->append(directoryName);
						outputFileName->push_back('/');
						outputFileName->append(buffer);
						outputFileName->push_back('.');
						outputFileName->append(extensionForOutputFiles);
						//cout << "OutputFile: " << outputFileName->c_str() << endl;

						ofstream* writtenFile = new ofstream(outputFileName->c_str(), ios::out | ios::binary);

						if(writtenFile->is_open()){
							if(isSplitterAtTheBeginning){
								writtenFile->write(splitterString->c_str(), splitterString->size());
								if(splitterString->size() <= stringToWrite->size()){
									stringToWrite->erase(stringToWrite->end()-splitterString->size(), stringToWrite->end());
								}
							}
							writtenFile->write(stringToWrite->c_str(),stringToWrite->size());
							writtenFile->close();
							stringToWrite->clear();

							this->numberOfSplittedParts = numberOfFile;
							numberOfFile++;

						} else {
							cout << "DumpFileSplitter::splitFileByASCII => Error during creating file... Cannot create and write file to the chosen directory... Program terminate...\n";
							return EXIT_FAILURE;
						}
					}

				}
			}
			//close the file
			analyzedFile.close();
	  }
	  else cout << "DumpFileSplitter::splitFileByASCII => Unable to open file";

  return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------------------------

int DumpFileSplitter::SplitFileByHex(char* hexSplitter){
	char* convertedHex = this->ConvertHexStringToString(hexSplitter);
	if (convertedHex != NULL) {
		if(SplitFileByASCII(convertedHex) == EXIT_FAILURE){
			return EXIT_FAILURE;
		}
		//cout << "Converted hexToSplit: " << splitterString->c_str() << endl;
	} else {
		cout << "DumpFileSplitter::splitFileByASCII => Inappropriate format of splitting hex... Program terminate\n";
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------------------------

int DumpFileSplitter::GetNumberOfSplittedParts(){
	return numberOfSplittedParts;
}

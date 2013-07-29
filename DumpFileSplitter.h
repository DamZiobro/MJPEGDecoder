/*
 * DumpFileSplitter.h
 *
 *  Created on: Sep 8, 2011
 *      Author: Ziobro Damian
 */

#ifndef DUMPFILESPLITTER_H_
#define DUMPFILESPLITTER_H_

//--------------------------------------------------------------------------------------------------

enum Errors{
	FILE_NOT_SET = 1,
};
#define DEFAULT_DIRECTORY_NAME "."
//--------------------------------------------------------------------------------------------------

class DumpFileSplitter {
private:
	char* extensionForOutputFiles;
	char* fileToAnalyze;
	bool isSplitterAtTheBeginning;
	char* directoryName;
	int numberOfSplittedParts;

	char* ConvertHexStringToString(char* hexToConvert);

public:
	//splitters
	int SplitFileByASCII(char* asciiString);
	int SplitFileByHex(char* hexString);

	//setters
	void SetExtensionForOutputFiles(char* extension);
	void SetFileToSplit(char* fileToSplit);
	void SetSplitterAtTheBeginning(bool atTheBeginning);
	void SetDirectoryNameForSplittedParts(char* directoryName);
	char* GetDirectoryNameForSplittedParts();

	//other
	int GetNumberOfSplittedParts();

	DumpFileSplitter();
	virtual ~DumpFileSplitter();
};

#endif /* DUMPFILESPLITTER_H_ */

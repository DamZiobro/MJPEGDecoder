/*
 * mjpegDecoder.cpp
 *
 *  Created on: Sep 8, 2011
 *      Author: Ziobro Damian
 */

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "DumpFileSplitter.h"
#include "JpegToAvi.h"

using namespace std;

//================================================================================================================

#define JPEG_FILES_EXTENSION "tmp.jpg"

//================================================================================================================

int GetFilesFromDirectory (char* dir, vector<string> &files, bool directoryNameIncluded);
bool isSubstringInString(string MainString, string SubString);

//================================================================================================================

int main(int argc, char **argv) {

	if(argv[1] == NULL || argv[2] == NULL){
		cout << "Inappropriate parameters... USAGE: mjpegDecoder <infile> <outfile>" << endl;
		return EXIT_FAILURE;
	}

	char* inputFile = argv[1];
	char* outputFile = argv[2];
	char* JPEG_SOI = (char*)"0xffd8";

	DumpFileSplitter* splitter = new DumpFileSplitter();
	splitter->SetExtensionForOutputFiles((char*)JPEG_FILES_EXTENSION);
	splitter->SetFileToSplit(inputFile);
	splitter->SetSplitterAtTheBeginning(true);

	// splitting dump file for JPEG framesNumber
	splitter->SplitFileByHex(JPEG_SOI);

	//DECODE P FRAMES HERE!!!!!!!!
	//NEED TO ADD CODE FOR DECODING P FRAMES

	//creating MJPEG Avi from decoded JPEG framesNumber
	JPEGToAVI jpegToAviGenerator;
	jpegToAviGenerator.CreateMJPEGAviFromJPGs(outputFile,splitter->GetNumberOfSplittedParts(), (char*)JPEG_FILES_EXTENSION);

	//removing temporary JPEG files
	vector<string> temporaryJPEGFiles;
	GetFilesFromDirectory(splitter->GetDirectoryNameForSplittedParts(),temporaryJPEGFiles, true);
	for (unsigned int i = 0;i < temporaryJPEGFiles.size();i++) {
		if(isSubstringInString(temporaryJPEGFiles[i], string(JPEG_FILES_EXTENSION))){
			remove(temporaryJPEGFiles[i].c_str());
		}
	}

	return EXIT_SUCCESS;
}

//================================================================================================================

int GetFilesFromDirectory (char* dir, vector<string> &files, bool directoryNameIncluded)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir)) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
    	string filename;
    	if(directoryNameIncluded){
    		filename.append(dir);
    		filename.append("/");
    	}
    	filename.append(dirp->d_name);
        files.push_back(filename);
    }
    closedir(dp);
    return 0;
}

//================================================================================================================

bool isSubstringInString(string MainString, string SubString){
 return (MainString.find(SubString) != string::npos);
}

//================================================================================================================

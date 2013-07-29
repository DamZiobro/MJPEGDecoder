/*
 * JpegToAvi.h
 *
 *  Created on: Sep 14, 2011
 *      Author: Ziobro Damian
 */

//--------------------------------------------------------------------------------------------------


#ifndef JPEGTOAVI_H_
#define JPEGTOAVI_H_

//--------------------------------------------------------------------------------------------------


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include "AviFormat.h"
#include "MyList.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstddef>
using namespace std;

//--------------------------------------------------------------------------------------------------

typedef struct JPEGDataStruct JPEGData;

//--------------------------------------------------------------------------------------------------

struct JPEGDataStruct
{
  unsigned int size;
  unsigned int offset;
  char name[0]; // ex. variable length structure
};

//--------------------------------------------------------------------------------------------------

#define JPEG_DATA_SZ (sizeof(unsigned int) * 2)

//--------------------------------------------------------------------------------------------------

class JPEGToAVI {
private:
	unsigned long long GetFileSize(char *fn);
	unsigned long long GetFramesSizes(MyList *myList);
	MyList *GetFrameList(int numberOfFrames, char* jpegFilesExtension);
	void PrintJPEGQuartet(unsigned int i, FILE* fileToWrite);
	bool GetJPEGSize(unsigned char* imageData, unsigned int imageDataSize, unsigned int *jpegWidth, unsigned int *jpegHeight);
	string GetFileData(char* filename);

	unsigned int frameWidth;
	unsigned int frameHeight;
	string frameData;
	unsigned int frameDataSize;

public:
	JPEGToAVI();
	virtual ~JPEGToAVI();
	int CreateMJPEGAviFromJPGs(char* outputAviFile, int numberOfFrames, char* jpegFilesExtension);

};
//--------------------------------------------------------------------------------------------------


#endif /* JPEGTOAVI_H_ */

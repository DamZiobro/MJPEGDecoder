#include "JpegToAvi.h"
#include <string.h>
#include <sstream>

#define DEFAULT_FRAME_WIDTH 360
#define DEFAULT_FRAME_HEIGHT 240
#define DEFAULT_FRAME_DATA_SIZE 0

//--------------------------------------------------------------------------------------------------

JPEGToAVI::JPEGToAVI(){
	  this->frameWidth = DEFAULT_FRAME_WIDTH;
	  this->frameHeight = DEFAULT_FRAME_HEIGHT;
	  this->frameDataSize = DEFAULT_FRAME_DATA_SIZE;
}

//--------------------------------------------------------------------------------------------------

JPEGToAVI::~JPEGToAVI(){

}

//--------------------------------------------------------------------------------------------------

unsigned long long JPEGToAVI::GetFileSize(char *fn)
{
  struct stat s;
  if (stat(fn, &s) == -1)
    return -1;
  return s.st_size;
}

//--------------------------------------------------------------------------------------------------

string JPEGToAVI::GetFileData(char* filename){
	string fileData;

		ifstream inFile;
		size_t size = 0; // here

		//cout << "Filename: " << filename << endl;
		inFile.open( filename, ios::in|ios::binary|ios::ate );
		char* oData = 0;

		inFile.seekg(0, ios::end);
		size = inFile.tellg() ;
		//cout << "Size of file: " << size << endl;
		inFile.seekg(0, ios::beg);

		oData = new char[ size ];
		inFile.read( oData, size );
		fileData.append(oData);

		this->frameDataSize = size;
		//cout << "Size of data: " << fileData.size() << endl;
		inFile.close();

	return fileData;
}

//--------------------------------------------------------------------------------------------------

unsigned long long JPEGToAVI::GetFramesSizes(MyList *myList)
{
  unsigned long long tmp, ret = 0;

  //for each list element
  for(; (myList); myList = myList->next) {
	this->frameData = GetFileData(((JPEGData *)myList->data)->name);
	if(this->frameDataSize == 0){
		return -1;
	}
	tmp = this->frameDataSize;
    ((JPEGData *)myList->data)->size = tmp;

    //get frame width and height (if it is possible)
    unsigned int* height = new unsigned int();
    unsigned int* width = new unsigned int();
    if(GetJPEGSize((unsigned char*)frameData.c_str(),this->frameDataSize,height, width)){
    	this->frameHeight = *height;
    	this->frameWidth = *width;
    	//cout << "FrameHeight: " << frameHeight << endl;
    	//cout << "FrameWidth: " << frameWidth << endl;
    }

    tmp += ((4-(this->frameDataSize%4)) % 4);
    ret += tmp;
  }

  return ret;
}

//--------------------------------------------------------------------------------------------------

MyList *JPEGToAVI::GetFrameList(int numberOfFrames, char* jpegFilesExtension)
{

	  MyList *ret = (MyList *) malloc(sizeof(MyList)),
	       *myList = ret;
	  JPEGData *tmp;
	  int i;

	  ret->data = 0;
	  ret->prev = 0;
	  ret->next = 0;

    for (i = 1; i <= numberOfFrames; i++) {
      stringstream tmpFileName;
      tmpFileName << i << "." << jpegFilesExtension; 
      cout << "JpegFile: " << tmpFileName.str() << endl;

      tmp = (JPEGData *) malloc(strlen(tmpFileName.str().c_str()) + 1 + JPEG_DATA_SZ);
      strcpy(tmp->name, tmpFileName.str().c_str());
      if (myList->data == 0){
        myList->data = tmp;
      } else {
        myList = list_push_back(myList, tmp);
      }
    }

	  return ret;
}

//--------------------------------------------------------------------------------------------------

void JPEGToAVI::PrintJPEGQuartet(unsigned int i, FILE* fileToWrite)
{
  fprintf(fileToWrite,"%c",i % 0x100);  i /= 0x100;
  fprintf(fileToWrite,"%c",i % 0x100);  i /= 0x100;
  fprintf(fileToWrite,"%c",i % 0x100);  i /= 0x100;
  fprintf(fileToWrite,"%c",i % 0x100);
}

//--------------------------------------------------------------------------------------------------

bool JPEGToAVI::GetJPEGSize(unsigned char* imageData, unsigned int imageDataSize, unsigned int *jpegWidth, unsigned int *jpegHeight) {

   unsigned int i=0;
   if(imageData[i] == 0xFF && imageData[i+1] == 0xD8 && imageData[i+2] == 0xFF && imageData[i+3] == 0xE0) {
      i += 4;
         for(i=0;i<imageDataSize;i++){
            if(imageData[i] == 0xFF && imageData[i+1] == 0xC0) {
               *jpegWidth = imageData[i+5]*256 + imageData[i+6];
               *jpegHeight = imageData[i+7]*256 + imageData[i+8];
               return true;
            }
         }
   } else {
	   return false;
   }                     //Not a valid SOI header
   return false;
}

//--------------------------------------------------------------------------------------------------

int JPEGToAVI::CreateMJPEGAviFromJPGs(char* outputAviFile, int numberOfFrames, char* jpegFilesExtension)
{
  unsigned int perMicrosecond = 1;
  unsigned int framesNumber = 1;

  const unsigned int framesPerSecond = 25;
  long long jpegSize64, riffSize64;
  long jpegSize = 1;
  const long long MAX_RIFF_SZ = 2147483648LL; //limit of 2GB for file
  unsigned int riffSize;

  int fd;
  long nbr, nbw, tnbw = 0;
  char buff[512];
  unsigned long long mfsz, remnant;
  MyList *framesNumberList = 0,
       *f = 0;

  FILE * fileToWrite;
  fileToWrite = fopen ( outputAviFile , "wb" );
  cout << "File to write..."<< outputAviFile << endl;


  perMicrosecond = 1000000 / framesPerSecond;

   //==================================================================================================
   //Filling header
   //==================================================================================================
			   struct AVIListHeaderLong aviFileHeader = {
				/* header */
				{
				  {'L', 'I', 'S', 'T'},
				  LITTLE_ENDIAN_4_BYTES(sizeof(struct AVIListHeaderLong) - 8),
				  {'h', 'd', 'r', 'l'}
				},

				/* chunk avih */
				{'a', 'v', 'i', 'h'},
				LITTLE_ENDIAN_4_BYTES(sizeof(struct AVIHeader)),
				{
				  LITTLE_ENDIAN_4_BYTES(perMicrosecond),
				  LITTLE_ENDIAN_4_BYTES(1000000 * (jpegSize/framesNumber) / perMicrosecond),
				  LITTLE_ENDIAN_4_BYTES(0),
				  LITTLE_ENDIAN_4_BYTES(AVIF_HASINDEX),
				  LITTLE_ENDIAN_4_BYTES(framesNumber),
				  LITTLE_ENDIAN_4_BYTES(0),
				  LITTLE_ENDIAN_4_BYTES(1),
				  LITTLE_ENDIAN_4_BYTES(0),
				  LITTLE_ENDIAN_4_BYTES(frameWidth),
				  LITTLE_ENDIAN_4_BYTES(frameHeight),
				  {LITTLE_ENDIAN_4_BYTES(0), LITTLE_ENDIAN_4_BYTES(0), LITTLE_ENDIAN_4_BYTES(0), LITTLE_ENDIAN_4_BYTES(0)}
				},

				/* list strl */
				{
				  {
				{'L', 'I', 'S', 'T'},
				LITTLE_ENDIAN_4_BYTES(sizeof(struct AVIListSTRL) - 8),
				{'s', 't', 'r', 'l'}
				  },

				  /* chunk strh */
				  {'s', 't', 'r', 'h'},
				  LITTLE_ENDIAN_4_BYTES(sizeof(struct AVIStreamHeader)),
				  {
				{'v', 'i', 'd', 's'},
				{'M', 'J', 'P', 'G'},
				LITTLE_ENDIAN_4_BYTES(0),
				LITTLE_ENDIAN_4_BYTES(0),
				LITTLE_ENDIAN_4_BYTES(0),
				LITTLE_ENDIAN_4_BYTES(perMicrosecond),
				LITTLE_ENDIAN_4_BYTES(1000000),
				LITTLE_ENDIAN_4_BYTES(0),
				LITTLE_ENDIAN_4_BYTES(framesNumber),
				LITTLE_ENDIAN_4_BYTES(0),
				LITTLE_ENDIAN_4_BYTES(0),
				LITTLE_ENDIAN_4_BYTES(0)
				  },

				  /* chunk strf */
				  {'s', 't', 'r', 'f'},
				  sizeof(struct AVIFrameHeader),
				  {
				LITTLE_ENDIAN_4_BYTES(sizeof(struct AVIFrameHeader)),
				LITTLE_ENDIAN_4_BYTES(frameWidth),
				LITTLE_ENDIAN_4_BYTES(frameHeight),
				LITTLE_ENDIAN_4_BYTES(1 + 24*256*256),
				{'M', 'J', 'P', 'G'},
				LITTLE_ENDIAN_4_BYTES(frameWidth * frameHeight * 3),
				LITTLE_ENDIAN_4_BYTES(0),
				LITTLE_ENDIAN_4_BYTES(0),
				LITTLE_ENDIAN_4_BYTES(0),
				LITTLE_ENDIAN_4_BYTES(0)
				  },

				  /* list odml */
				  {
				{
				  {'L', 'I', 'S', 'T'},
				  LITTLE_ENDIAN_4_BYTES(16),
				  {'o', 'd', 'm', 'l'}
				},
				{'d', 'm', 'l', 'h'},
				LITTLE_ENDIAN_4_BYTES(4),
				LITTLE_ENDIAN_4_BYTES(framesNumber)
				  }
				}
			  };

   //==================================================================================================

  cout << "Getting frames - it may take few minutes. Please wait ... " << endl;
  framesNumberList = GetFrameList(numberOfFrames, jpegFilesExtension);
  cout << "Getting frames - DONE " << endl;
  framesNumber = list_size(framesNumberList);
  cout << "Frames number: " << framesNumber << endl;

  /* getting image, and hence, riff sizes */
  jpegSize64 = GetFramesSizes(framesNumberList);

  if (jpegSize64 == -1) {
    fprintf(stderr, "couldn't determine size of images\n");
    return -2;
  }

  riffSize64 = sizeof(struct AVIListHeaderLong) + 4 + 4 + jpegSize64
    + 8*framesNumber + 8 + 8 + 16*framesNumber;

  if (riffSize64 >= MAX_RIFF_SZ) {
    fprintf(stderr,"RIFF would exceed 2 Gb limit\n");
    return -3;
  }

  jpegSize = (long) jpegSize64;
  riffSize = (unsigned int) riffSize64;

  /* printing AVI riff hdr */
  fprintf(fileToWrite,"RIFF");
  PrintJPEGQuartet(riffSize, fileToWrite);
  fprintf(fileToWrite,"AVI ");

  // fulfill avi file header
  aviFileHeader.aviHeader.microsecondsPerFrame = LITTLE_ENDIAN_4_BYTES(perMicrosecond);
  aviFileHeader.aviHeader.maxBytesPerSecond = LITTLE_ENDIAN_4_BYTES(1000000 * (jpegSize/framesNumber)
				      / perMicrosecond);
  aviFileHeader.aviHeader.totalFramesNumber = LITTLE_ENDIAN_4_BYTES(framesNumber);
  aviFileHeader.aviHeader.frameWidth = LITTLE_ENDIAN_4_BYTES(frameWidth);
  aviFileHeader.aviHeader.frameHeight = LITTLE_ENDIAN_4_BYTES(frameHeight);
  aviFileHeader.aviListSTRL.streamHeader.scale = LITTLE_ENDIAN_4_BYTES(perMicrosecond);
  aviFileHeader.aviListSTRL.streamHeader.rate = LITTLE_ENDIAN_4_BYTES(1000000);
  aviFileHeader.aviListSTRL.streamHeader.length = LITTLE_ENDIAN_4_BYTES(framesNumber);
  aviFileHeader.aviListSTRL.frameHeader.frameWidth = LITTLE_ENDIAN_4_BYTES(frameWidth);
  aviFileHeader.aviListSTRL.frameHeader.frameHeight = LITTLE_ENDIAN_4_BYTES(frameHeight);
  aviFileHeader.aviListSTRL.frameHeader.imageSize = LITTLE_ENDIAN_4_BYTES(frameWidth * frameHeight * 3);
  aviFileHeader.aviListSTRL.listODML.framesNumber = LITTLE_ENDIAN_4_BYTES(framesNumber);
  fwrite(&aviFileHeader, sizeof(aviFileHeader), 1, fileToWrite);

  /* list movi */
  fprintf(fileToWrite,"LIST");
  PrintJPEGQuartet(jpegSize + 8*framesNumber + 4, fileToWrite);
  fprintf(fileToWrite,"movi");

  //==================================================================================================
  // Writing each frame and headers for AVI file
  //==================================================================================================
	  for (f = framesNumberList; (f); f = f->next) {
		fprintf(fileToWrite,"00db");
		//cout << "FileJPG: " << ((JPEGData *) f->data)->name << "; FileSize: " << ((JPEGData *) f->data)->size << endl;
		mfsz = ((JPEGData *) f->data)->size;
		remnant = (4-(mfsz%4)) % 4;
		PrintJPEGQuartet(mfsz + remnant, fileToWrite);
		((JPEGData *) f->data)->size += remnant;

		if (f == framesNumberList) {
		  ((JPEGData *) f->data)->offset = 4;
		} else {
		  ((JPEGData *) f->data)->offset =
		((JPEGData *) f->prev->data)->offset +
		((JPEGData *) f->prev->data)->size + 8;
		}

		if ((fd = open(((JPEGData *) f->data)->name, O_RDONLY)) < 0) {
		  fprintf(stderr, "couldn't open file!\n");
		  list_rerased(framesNumberList);
		  return -6;
		}
		nbw = 0;

		if ((nbr = read(fd, buff, 6)) != 6) {
		  fprintf(stderr, "error: nbr == %d != 6!!!\n",(int)nbr);
		  list_rerased(framesNumberList);
		  return -7;
		}
		fwrite(buff, nbr, 1, fileToWrite);
		read(fd, buff, 4);
		fwrite("AVI1", 4, 1, fileToWrite);
		nbw = 10;

		while ((nbr = read(fd, buff, 512)) > 0) {
		  fwrite(buff, nbr, 1, fileToWrite);
		  nbw += nbr;
		}
		if (remnant > 0) {
		  fwrite(buff, remnant, 1, fileToWrite);
		  nbw += remnant;
		}
		tnbw += nbw;
		close(fd);
	  }

	  if (tnbw != jpegSize) {
		fprintf(stderr, "error writing images (wrote %ld bytes, expected %ld bytes)\n",
			tnbw, jpegSize);
		list_rerased(framesNumberList);
		return -8;
	  }

	  /* indices */
	  fprintf(fileToWrite,"idx1");
	  PrintJPEGQuartet(16 * framesNumber, fileToWrite);
	  for (f = framesNumberList; (f); f = f->next) {
		fprintf(fileToWrite,"00db");
		PrintJPEGQuartet(18, fileToWrite);
		PrintJPEGQuartet(((JPEGData *) f->data)->offset, fileToWrite);
		PrintJPEGQuartet(((JPEGData *) f->data)->size, fileToWrite);
	  }

	  fclose(fileToWrite);
	  close(fd);
	  list_rerased(framesNumberList);
  
  //==================================================================================================

  return 0;
}

//--------------------------------------------------------------------------------------------------

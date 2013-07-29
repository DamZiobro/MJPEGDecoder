//===================================================================================================================

#ifndef _AVIFMT_H_
#define _AVIFMT_H_

//===================================================================================================================

// Byte type definition
typedef unsigned char BYTE;

//===================================================================================================================

// AVIHeader flags
const unsigned int AVIF_HASINDEX = 0x00000010;
const unsigned int AVIF_MUSTUSEINDEX = 0x00000020;
const unsigned int AVIF_ISINTERLEAVED = 0x00000100;
const unsigned int AVIF_TRUSTCKTYPE = 0x00000800;
const unsigned int AVIF_WASCAPTUREFILE = 0x00010000;
const unsigned int AVIF_COPYRIGHTED = 0x00020000;

//===================================================================================================================

struct AVIHeader 
{
  unsigned int	microsecondsPerFrame;
  unsigned int maxBytesPerSecond;
  unsigned int padding;
  unsigned int flags;
  unsigned int totalFramesNumber;
  unsigned int initialframesNumber;
  unsigned int streams;
  unsigned int bufferSize;  
  unsigned int frameWidth;
  unsigned int frameHeight;
  unsigned int reserved[4];
};

//===================================================================================================================

struct AVIStreamHeader
{
  unsigned char streamType[4];
  unsigned char handler[4];
  unsigned int flags;
  unsigned int priority;
  unsigned int initialFramesNumber;
  unsigned int scale;
  unsigned int rate;
  unsigned int start;
  unsigned int length;
  unsigned int bufferSize;
  unsigned int quality;
  unsigned int sampleSize;
};

//===================================================================================================================

struct AVIFrameHeader
{       
  unsigned int size;
  unsigned int frameWidth;
  unsigned int frameHeight;
  unsigned int planesBitCounter;
  unsigned char compression[4];
  unsigned int imageSize;
  unsigned int xPelsMetter;
  unsigned int yPelsMeter;
  unsigned int numberOfColors;
  unsigned int impColours;
};

//===================================================================================================================

struct AVIListHeader
{
  unsigned char aviListHeaderId[4];
  unsigned int aviListHeaderSize;
  unsigned char aviListHeaderType[4];
};

//===================================================================================================================

struct AVIListODML
{
  struct AVIListHeader aviListHeader;

  unsigned char id[4];
  unsigned int size;
  unsigned int framesNumber;
};

//===================================================================================================================

struct AVIListSTRL
{
  struct AVIListHeader aviListHeader;
  
  unsigned char streamHeaderId[4];
  unsigned int streamHeaderSize;
  struct AVIStreamHeader streamHeader;

  unsigned char frameHeaderId[4];
  unsigned int frameHeaderSize;
  struct AVIFrameHeader frameHeader;

  struct AVIListODML listODML;
};


struct AVIListHeaderLong
{
  struct AVIListHeader aviListHeader;

  unsigned char aviHeaderId[4];
  unsigned int aviHeaderSize;
  struct AVIHeader aviHeader;
  
  struct AVIListSTRL aviListSTRL;
};

//===================================================================================================================
//BYTE SWAP SECTION
//===================================================================================================================

#include <endian.h>
#include <sys/types.h>


#ifndef __BYTE_ORDER
	#error "Byte order ERROR: __BYTE_ORDER is not defined\n";
#endif


#define SWAP_2_BYTES(x) (((x>>8) & 0x00ff) |\
                  ((x<<8) & 0xff00))


#define SWAP_4_BYTES(x) (((x>>24) & 0x000000ff) |\
                  ((x>>8)  & 0x0000ff00) |\
                  ((x<<8)  & 0x00ff0000) |\
                  ((x<<24) & 0xff000000))


#if __BYTE_ORDER == __BIG_ENDIAN
# define LITTLE_ENDIAN_2_BYTES(a) SWAP_2_BYTES((a))
# define LITTLE_ENDIAN_4_BYTES(a) SWAP_4_BYTES((a))
# define BIG_ENDIAN_2_BYTES(a) (a)
# define BIG_ENDIAN_4_BYTES(a) (a)
#else
# define LITTLE_ENDIAN_2_BYTES(a) (a)
# define LITTLE_ENDIAN_4_BYTES(a) (a)
# define BIG_ENDIAN_2_BYTES(a) SWAP_2_BYTES((a))
# define BIG_ENDIAN_4_BYTES(a) SWAP_4_BYTES((a))
#endif

//===================================================================================================================

#endif

//===================================================================================================================

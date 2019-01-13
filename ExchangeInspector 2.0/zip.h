#pragma once

#pragma pack(push, 1)
typedef struct _ZLocalHeader
{
	unsigned int header;
	unsigned short version;
	unsigned short bitFlag;
	unsigned short compMethod;
	unsigned short lastModTime;
	unsigned short lastModDate;
	unsigned int crc32;
	unsigned int compSize;
	unsigned int unCompSize;
	unsigned short fileNameLength;
	unsigned short extraFieldLength;
} ZLocalHeader, * PZLocalHeader;
#pragma pack(pop)

#ifndef ZHEADER
#define ZHEADER 0x04034B50
#endif

#ifndef CHUNK_SIZE
#define CHUNK_SIZE 4096
#endif

#include "FileReadAndChunk.h"





FileReadAndChunk::~FileReadAndChunk()
{
	free(data);
}

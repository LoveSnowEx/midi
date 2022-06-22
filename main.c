#include "basic.h"
#include "midi.h"

sMthdChunk readMthd(FILE *stream) {
	sMthdChunk chunk;
	fread((void*)&chunk, 1, sizeof(sMthdChunk), stream);
	return chunk;
}

// sMtrkChunk readMtrk(FILE *stream) {

// }

int main() {
	// char buf[BUFSIZ];
	char *filename = "HotelCalifornia.mid";

	FILE *pFile = fopen(filename, "r");

	sMthdChunk chunk = readMthd(pFile);

	printf("mthd: %s\n", chunk.mthd);
	printf("size: %d\n", chunk.size);
	printf("format: %d\n", chunk.format);
	printf("numTracks: %d\n", chunk.numTracks);
	printf("division: %d\n", chunk.division);


	fclose(pFile);


	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include "ice.h"

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		printf("BDO_encrypt <source file> <output file>");
		return 1;
	}

	char* srcFileName = argv[1];
	char* outFileName = argv[2];

	FILE* srcFile = fopen(srcFileName, "rb");
	if (srcFile == NULL){
		printf("File not found: %s", srcFileName);
		return -1;
	}
	FILE* outFile = fopen(outFileName, "wb");

	size_t fileSize, dataSize;
	fseek(srcFile, 0, SEEK_END);
	fileSize = ftell(srcFile);
	fseek(srcFile, 0, SEEK_SET);

	dataSize = (fileSize % 8) == 0 ? fileSize : fileSize + 8 - (fileSize % 8);
	
	unsigned char *ptext = (unsigned char *) calloc(dataSize, sizeof(unsigned char));
	unsigned char *ctext = (unsigned char *) calloc(dataSize, sizeof(unsigned char));

	fread(ptext, 1, fileSize, srcFile);

	const unsigned char *bdo_ice_key = (const unsigned char*)"\x51\xF3\x0F\x11\x04\x24\x6A\x00";

	ICE_KEY *ik = ice_key_create(0);
	ice_key_set(ik, bdo_ice_key);

	size_t cuts = dataSize/8;
	while (cuts--){
		ice_key_encrypt(ik, ptext, ctext); // key, in (decrypted text), out (encrypted text)

		ptext +=8;
		ctext +=8;
	}

	ctext -= dataSize; // reset the pointer back to the beginning.

	fwrite(ctext, 1, dataSize, outFile);

	fclose(srcFile);
	fclose(outFile);

	return 0;
}

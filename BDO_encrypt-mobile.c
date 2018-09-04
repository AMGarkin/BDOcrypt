#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "zlib.h"

const unsigned char CHAR_NULL = 0x00;
const unsigned char CHAR_CR = 0x0D;
const unsigned char CHAR_LF = 0x0A;
const unsigned char BOM_UTF8[3] = {0xEF, 0xBB, 0xBF};
const unsigned long MAX_BUFF_SIZE = 4096;


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
	FILE* tmpFile = tmpfile();
	FILE* outFile = fopen(outFileName, "wb");


	///Convert .txt source file into temporary .bss file
	unsigned long strSize;
	unsigned long strType;
	unsigned long strID1;
	unsigned long strID2; //for some reason "short" doesn't work well with fwscanf()
	unsigned char strID3;
	unsigned char strID4;
	int a;
	char strBuff[MAX_BUFF_SIZE];

	//Handle BOM (if present, skip it)
	fread(&strBuff[0], 1, 1, srcFile);
	fread(&strBuff[1], 1, 1, srcFile);
	fread(&strBuff[2], 1, 1, srcFile);
	if (!(strBuff[0] == BOM_UTF8[0]) && (strBuff[1] == BOM_UTF8[1]) && (strBuff[2] == BOM_UTF8[2])) {
		rewind(srcFile);
	}

	while (1){
		memset(strBuff, CHAR_NULL, MAX_BUFF_SIZE);

		if (fscanf(srcFile, "%u\t%u\t%u\t%u\t%u\t", &strType, &strID1, &strID2, &strID3, &strID4) < 5) break; //this pattern "eats" leading white space from the next string, so I had to enclose strings in double quotes (in bss -> txt conversion)

		fseek(srcFile, 1, SEEK_CUR); //skip leading double quotes

		for (a = 0; a < MAX_BUFF_SIZE; a++) {
			fread(&strBuff[a], 1, 1, srcFile);
			if (a > 0) {
				if (strBuff[a] == CHAR_LF || strBuff[a] == CHAR_CR) {
					if (strBuff[a-1] == '"') {
						strBuff[a-1] = CHAR_NULL;
					}
					strBuff[a] = CHAR_NULL;
					break;
				}
				if (strBuff[a] == 'n' && strBuff[a-1] == '\\') {
					strBuff[a-1] = CHAR_CR;
					strBuff[a] = CHAR_LF;
				}
			}
		}

		strSize = strlen(strBuff);

		fwrite(&strSize, 4, 1, tmpFile);
		fwrite(&strType, 4, 1, tmpFile);
		fwrite(&strID1,  4, 1, tmpFile);
		fwrite(&strID2,  2, 1, tmpFile);
		fwrite(&strID3,  1, 1, tmpFile);
		fwrite(&strID4,  1, 1, tmpFile);
		fwrite(&strBuff, 1, strSize, tmpFile);
		fputc(CHAR_NULL, tmpFile);
	}


	///Compress temporary .bss file to .loc
	unsigned long compressedSize = 0;
	unsigned long uncompressedSize = 0;

	fseek(tmpFile, 0, SEEK_END);
	uncompressedSize = ftell(tmpFile);
	rewind(tmpFile);

	compressedSize = compressBound(uncompressedSize);

	unsigned char *pCompressedData = (unsigned char *) calloc(compressedSize, sizeof(unsigned char));
	unsigned char *pUncompressedData = (unsigned char *) calloc(uncompressedSize, sizeof(unsigned char));

	fread(pUncompressedData, 1, uncompressedSize, tmpFile);

	int result = compress2(pCompressedData, &compressedSize, pUncompressedData, uncompressedSize, Z_BEST_SPEED);

	if (result == Z_OK) {
		fwrite(&uncompressedSize, 1, 4, outFile);
		fwrite(pCompressedData, 1, compressedSize, outFile);
	} else if (result == Z_MEM_ERROR) {
		printf("ERROR: Not enough memory.");
	} else if (result == Z_BUF_ERROR) {
		printf("ERROR: Output buffer is too small.");
	}

	free(pCompressedData);
	free(pUncompressedData);
	fclose(srcFile);
	fclose(tmpFile);
	fclose(outFile);

	return 0;
}

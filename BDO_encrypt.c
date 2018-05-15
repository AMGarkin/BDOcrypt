#include <stdio.h>
#include <stdlib.h>
#include <cwchar>
#include "zlib.h"

const wchar_t CHAR_NULL = 0x0000;
const wchar_t CHAR_CR = 0x000D;
const wchar_t CHAR_LF = 0x000A;
const wchar_t BOM_UTF16LE = 0xFEFF;
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
	wchar_t strBuff[MAX_BUFF_SIZE];

	//Handle BOM (if present, skip it)
	fread(&strBuff[0], 2, 1, srcFile);
	if (strBuff[0] != BOM_UTF16LE) {
		rewind(srcFile);
	}

	while (1){
		wmemset(strBuff, CHAR_NULL, MAX_BUFF_SIZE);

		if (fwscanf(srcFile, L"%u\t%u\t%u\t%u\t%u\t", &strType, &strID1, &strID2, &strID3, &strID4) < 5) break; //this pattern "eats" leading white space from next string, so I had to enclose strings in double quotes (in bss -> txt conversion)

		fseek(srcFile, 2, SEEK_CUR); //skip leading double quotes

		for (a = 0; a < MAX_BUFF_SIZE; a++) {
			fread(&strBuff[a], 2, 1, srcFile);
			if (a > 0) {
				if (strBuff[a] == CHAR_LF || strBuff[a] == CHAR_CR) {
					if (strBuff[a-1] == L'"') {
						strBuff[a-1] = CHAR_NULL;
					}
					strBuff[a] = CHAR_NULL;
					break;
				}
				if (strBuff[a] == L'n' && strBuff[a-1] == L'\\') {
					a--;
					strBuff[a] = CHAR_LF;
				}
			}
		}

		strSize = wcslen(strBuff);

		fwrite(&strSize, 4, 1, tmpFile);
		fwrite(&strType, 4, 1, tmpFile);
		fwrite(&strID1,  4, 1, tmpFile);
		fwrite(&strID2,  2, 1, tmpFile);
		fwrite(&strID3,  1, 1, tmpFile);
		fwrite(&strID4,  1, 1, tmpFile);
		fwrite(&strBuff, 2, strSize, tmpFile);
		fputwc(CHAR_NULL, tmpFile);
		fputwc(CHAR_NULL, tmpFile);
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

	fclose(srcFile);
	fclose(tmpFile);
	fclose(outFile);

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <cwchar>
#include "zlib.h"

const wchar_t CHAR_LF = 0x000A;
const unsigned long MAX_BUFF_SIZE = 4096;


int main(int argc, char **argv)
{
	if (argc != 3){
		printf("BDO_decrypt <source file> <output file>");
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

	///Decompress source to temporary file
	unsigned long compressedSize = 0;
	unsigned long uncompressedSize = 0;

	fseek(srcFile, 0, SEEK_END);
	compressedSize = ftell(srcFile) - 4; // 1st 4 bytes holds information about uncompressed data size
	rewind(srcFile);
	
	fread(&uncompressedSize, 4, 1, srcFile);

	unsigned char *pCompressedData = (unsigned char *) calloc(compressedSize, sizeof(unsigned char));
	unsigned char *pUncompressedData = (unsigned char *) calloc(uncompressedSize, sizeof(unsigned char));

	fread(pCompressedData, 1, compressedSize, srcFile);

	int result = uncompress(pUncompressedData, &uncompressedSize, pCompressedData, compressedSize);

	if (result == Z_OK) {
		fwrite(pUncompressedData, 1, uncompressedSize, tmpFile);

		///Convert .bss data from temporary file to text file
		unsigned long strSize;
		unsigned long strType;
		unsigned long strID1;
		unsigned short strID2;
		unsigned char strID3;
		unsigned char strID4;
		int a, b;
		wchar_t tmp;
		wchar_t strBuff[MAX_BUFF_SIZE];

		rewind(tmpFile);

		while (1){
			if (fread(&strSize, 4, 1, tmpFile) != 1) break;
			if (fread(&strType, 4, 1, tmpFile) != 1) break;
			if (fread(&strID1, 4, 1, tmpFile) != 1) break;
			if (fread(&strID2, 2, 1, tmpFile) != 1) break;
			if (fread(&strID3, 1, 1, tmpFile) != 1) break;
			if (fread(&strID4, 1, 1, tmpFile) != 1) break;
			b = 0;
			for (a = 0; a < strSize + 2; a++) {
				if (fread(&tmp, 2, 1, tmpFile) != 1) break;
				if (tmp == CHAR_LF) {
					strBuff[b] = L'\\';
					b++;
					strBuff[b] = L'n';
					b++;
				} else {
					strBuff[b] = tmp;
					b++;
				}
			}

			fwprintf(outFile, L"%u\t%u\t%u\t%u\t%u\t\"%s\"\r\n", strType, strID1, strID2, strID3, strID4, strBuff);
		}
	} else if (result == Z_MEM_ERROR) {
		printf("ERROR: Not enough memory.");
	} else if (result == Z_BUF_ERROR) {
		printf("ERROR: Output buffer is too small.");
	} else if (result == Z_DATA_ERROR) {
		printf("ERROR: Input data are corrupted or incomplete.");
	}

	fclose(srcFile);
	fclose(tmpFile);
	fclose(outFile);

	return 0;
}

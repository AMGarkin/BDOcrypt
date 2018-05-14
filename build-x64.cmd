@mkdir obj\x64 2>nul
x86_64-w64-mingw32-g++.exe -O3 -c BDO_encrypt.c -o obj/x64/BDO_encrypt.o -lz
x86_64-w64-mingw32-g++.exe -O3 -c BDO_decrypt.c -o obj/x64/BDO_decrypt.o -lz
@mkdir bin\x64 2>nul
x86_64-w64-mingw32-g++.exe -o bin/x64/BDO_encrypt.exe obj/x64/BDO_encrypt.o -s -static -lz
x86_64-w64-mingw32-g++.exe -o bin/x64/BDO_decrypt.exe obj/x64/BDO_decrypt.o -s -static -lz

@mkdir obj\x64 2>nul
x86_64-w64-mingw32-g++.exe -O3 -c BDO_encrypt-mobile.c -o obj/x64/BDO_encrypt-mobile.o -lz
x86_64-w64-mingw32-g++.exe -O3 -c BDO_decrypt-mobile.c -o obj/x64/BDO_decrypt-mobile.o -lz
@mkdir bin\x64 2>nul
x86_64-w64-mingw32-g++.exe -o bin/x64/BDO_encrypt-mobile.exe obj/x64/BDO_encrypt-mobile.o -s -static -lz
x86_64-w64-mingw32-g++.exe -o bin/x64/BDO_decrypt-mobile.exe obj/x64/BDO_decrypt-mobile.o -s -static -lz

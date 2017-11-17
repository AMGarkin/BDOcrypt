@mkdir obj\x64 2>nul
x86_64-w64-mingw32-g++.exe -O3 -c ice.c -o obj/x64/ice.o
x86_64-w64-mingw32-g++.exe -O3 -c BDO_encrypt.c -o obj/x64/BDO_encrypt.o
x86_64-w64-mingw32-g++.exe -O3 -c BDO_decrypt.c -o obj/x64/BDO_decrypt.o
@mkdir bin\x64 2>nul
x86_64-w64-mingw32-g++.exe -o bin/x64/BDO_encrypt.exe obj/x64/BDO_encrypt.o obj/x64/ice.o -s -static
x86_64-w64-mingw32-g++.exe -o bin/x64/BDO_decrypt.exe obj/x64/BDO_decrypt.o obj/x64/ice.o -s -static

@mkdir obj\x86 2>nul
i686-w64-mingw32-g++.exe -O3 -c BDO_encrypt.c -o obj/x86/BDO_encrypt.o -lz
i686-w64-mingw32-g++.exe -O3 -c BDO_decrypt.c -o obj/x86/BDO_decrypt.o -lz
@mkdir bin\x86 2>nul
i686-w64-mingw32-g++.exe -o bin/x86/BDO_encrypt.exe obj/x86/BDO_encrypt.o -s -static -lz
i686-w64-mingw32-g++.exe -o bin/x86/BDO_decrypt.exe obj/x86/BDO_decrypt.o -s -static -lz

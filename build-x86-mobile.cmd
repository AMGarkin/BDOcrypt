@mkdir obj\x86 2>nul
i686-w64-mingw32-g++.exe -O3 -c BDO_encrypt-mobile.c -o obj/x86/BDO_encrypt-mobile.o -lz
i686-w64-mingw32-g++.exe -O3 -c BDO_decrypt-mobile.c -o obj/x86/BDO_decrypt-mobile.o -lz
@mkdir bin\x86 2>nul
i686-w64-mingw32-g++.exe -o bin/x86/BDO_encrypt-mobile.exe obj/x86/BDO_encrypt-mobile.o -s -static -lz
i686-w64-mingw32-g++.exe -o bin/x86/BDO_decrypt-mobile.exe obj/x86/BDO_decrypt-mobile.o -s -static -lz

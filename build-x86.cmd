@mkdir obj\x86 2>nul
i686-w64-mingw32-g++.exe -O3 -c ice.c -o obj/x86/ice.o
i686-w64-mingw32-g++.exe -O3 -c BDO_encrypt.c -o obj/x86/BDO_encrypt.o
i686-w64-mingw32-g++.exe -O3 -c BDO_decrypt.c -o obj/x86/BDO_decrypt.o
@mkdir bin\x86 2>nul
i686-w64-mingw32-g++.exe -o bin/x86/BDO_encrypt.exe obj/x86/BDO_encrypt.o obj/x86/ice.o -s -static
i686-w64-mingw32-g++.exe -o bin/x86/BDO_decrypt.exe obj/x86/BDO_decrypt.o obj/x86/ice.o -s -static

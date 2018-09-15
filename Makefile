CFLAGS	+= -s -O3
CLIBS	+= -lz

ifeq ($(shell uname -s), Darwin)
  CC	= gcc-8
  CXX	= g++-8
else
  CLIBS	+= -static
endif

all: obj bin BDO_encrypt BDO_decrypt

BDO_encrypt: BDO_encrypt.o
	 $(CXX) $(CFLAGS) -o bin/BDO_encrypt obj/BDO_encrypt.o $(CLIBS)
BDO_encrypt.o: BDO_encrypt.c
	 $(CXX) $(CFLAGS) -c BDO_encrypt.c -o obj/BDO_encrypt.o $(CLIBS)

BDO_decrypt: BDO_decrypt.o
	 $(CXX) $(CFLAGS) -o bin/BDO_decrypt obj/BDO_decrypt.o $(CLIBS)
BDO_decrypt.o: BDO_decrypt.c
	 $(CXX) $(CFLAGS) -c BDO_decrypt.c -o obj/BDO_decrypt.o $(CLIBS)

obj:
	 mkdir -p obj
bin:
	 mkdir -p bin

clean:
	 rm obj/BDO_encrypt.o bin/BDO_encrypt obj/BDO_decrypt.o bin/BDO_decrypt

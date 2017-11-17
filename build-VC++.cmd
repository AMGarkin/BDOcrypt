@rem x86/x64 depends on which environment you use
@rem x64 = Start > Visual Studio 2017 > x64 Native Tools Command prompt for VS 2017
@rem x86 = Start > Visual Studio 2017 > x64_x86 Cross Tools Command prompt for VS 2017

@mkdir obj\VCC 2>nul
cl -Ox -W2 -GL -Gy -MT -EHsc -Foobj\VCC\ -c BDO_encrypt.c BDO_decrypt.c ice.c
@mkdir bin\VCC 2>nul
link -LTCG -OUT:bin\VCC\BDO_encrypt.exe obj/VCC/BDO_encrypt.obj obj/VCC/ice.obj
link -LTCG -OUT:bin\VCC\BDO_decrypt.exe obj/VCC/BDO_decrypt.obj obj/VCC/ice.obj

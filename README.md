Utilities can be used to decrypt/encrypt localization files for Black Desert Online

BDO_decrypt \<encrypted file\> \<output file\><br>
BDO_encrypt \<decrypted file\> \<output file\>

WARNING! Output file will be overwritten without asking.
<br>
<br>
Example of use:

- Decrypt localization file:

    bdo_decrypt.exe ads\languagedata_en.loc languagedata_en.txt

- Do some stuff with that file (modify some texts)
- Encrypt it back to the original location:

    bdo_encrypt.exe languagedata_en.txt ads\languagedata_en.loc

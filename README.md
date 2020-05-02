# console BMP-Parser
### Authors 
##### Rafał Stachowiak - creator of the printable headers, histogram creation and grayscale image alghorytm in \<nazwac>.c and \<nazwah>.h
##### Franciszek Olejnik - creator of the steganography part and  \<others>  in \<nazwa>.\<ext>

## Simplicity in usage
Few steps how to use our application:
1. compile file \<nazwac>.c 
```bash
	gcc -o <executable-file-name> <nazwac>.c -c
```
2.  execute with the following to obtain informations about headers, and histogram with percentage of colours:
```bash
./<executable-file-name> <file-name.bmp> 
```
3.  If you want to change your bmp file into greyscale (using mean colour method):
```bash
./<executable-file-name> <file-name.bmp> <result-file-name.bmp>
```
4. If you want to encrypt a message in the bmp file please execute as following:
```bash
./<executable-file-name> <file-name.bmp> <result-file-name.bmp> <"message-to-encrypt">
```
### \<nazwah>.h 
It contains defines directives crucial for program execution and definitions for functions used in \<nazwac>.c

# Please show mercy.

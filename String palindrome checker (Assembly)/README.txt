The program receives the start address of the string in the code memory, checks
whether the string at the given address is palindrome and sets the CY flag to
either 0 or 1 depending on the result. The subroutine
STR_CODE2IRAM copies the string in the code
memory to the internal memory and STR_PALINDROME checkss if the copied string is palindrome.
STR_LENGTH_DETECT detects the length of the string as well as the end address 
onto which the string is to be copied. 
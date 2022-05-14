; Definitions
; -------------------------------------------------------------------

; Address symbols for creating pointers

STR_ADDR_IRAM  EQU 0x40

ORG 0x0070 
STR_ADDR_CODE:
DB "TestForPalindromeSentence!!ecnetneSemordnilaProFtseT" 
DB 0

; Interrupt jump table
ORG 0x0000;
    SJMP  MAIN                  ; Reset vector



; Beginning of the user program
ORG 0x0010

; -------------------------------------------------------------------
; MAIN program
; -------------------------------------------------------------------
; Purpose: Prepare the inputs and call the subroutines
; -------------------------------------------------------------------

MAIN:

    ; Prepare input parameters for the subroutine
	MOV DPTR,#STR_ADDR_CODE    ; DPTR points to the base address in the code memory.
	MOV R7,#STR_ADDR_IRAM      ; R7 points to the base address onto which 
	                           ; the string is to be written in internal memory.   
	CALL STR_CODE2IRAM         ; Copy the string from code memory to internal memory
	
	MOV R7, #STR_ADDR_IRAM
; Infinite loop: Call the subroutine repeatedly
LOOP:

    CALL STR_PALINDROME ; Call palindrome check subroutine

    SJMP  LOOP




; ===================================================================           
;                           SUBROUTINE(S)
; ===================================================================           


; -------------------------------------------------------------------
; STR_CODE2IRAM
; -------------------------------------------------------------------
; Purpose: Copy the string from code memory to internal memory
; -------------------------------------------------------------------
; INPUT(S):
;   DPTR - Base address of string in code memory (16 bits)
;   R7 - Base address of string in the internal memory
;   R3 - the length of string set by the STR_LENGTH_DETECT subroutine
; OUTPUT(S): 
;   -
; MODIFIES:
;   R2 - stores the length of the string 
;        and is decremented in each iteration of the loop
;   R1 - stores the base address of the string in the internal memory
;        and is incremented in each iteration of the loop
;   A  - used for indirect addressing and addressing between Ri registers
 ; -------------------------------------------------------------------


STR_CODE2IRAM:

CALL STR_LENGTH_DETECT     ; detects the length of the string and the end address
	                       ; of the string in internal memory.
MOV DPTR,#STR_ADDR_CODE 

MOV A, R7 ;      ; R7 register cannot be used for indirect addressing.
MOV R1, A        ; Hence, the base address onto which the string is to be written is held by R1.
MOV A, R3        ; R3 holds the value of the length of the string and R3 should not be modified.
MOV R2, A        ; Hence, the length is addressed to R2.
BACK: CLR A      ; ensures the Accumulator is empty.
MOVC A, @A+DPTR  ; sends the string character to Acc. 
MOV @R1, A       ; idirectly addresses the received string character to the internal memory to which R1 is pointing.
INC DPTR         ; points to the next character.
INC R1           ; points to the next memory cell in the internal memory.
DJNZ R2, BACK    ; the previous operation is held as many times as the number of characters of the string.
    RET          ; return to the main.





; -------------------------------------------------------------------
; STR_PALINDROME
; -------------------------------------------------------------------
; Purpose: Check whether an ASCII string is palindrome
; -------------------------------------------------------------------
; INPUT(S):
;   R7 - Base address of the string in internal memory
;   R4 - Address of the end of the string in internal memory
; OUTPUT(S): 
;   CY=0 if plaindrome 
;   CY=1 if not
; MODIFIES:
;   R0 - temporarily holds the base address of the string
;   R1 - temporarily holds the end address of the string
;   A  - used for addressing betweek Ri registers and indirect addressing for comparisons
;   B  - used for indirect addressing for comparisons and other operations
; -------------------------------------------------------------------


STR_PALINDROME:

MOV A, R3           ; sends the length value of the string to Acc.
MOV B, #2           ; addresses the constant 2 to B.
DIV AB              ; the string length is divided by 2 and the quotient is addressed back to Acc. 
                    ; the value in Acc defines the maximum number of comparisons to operate.
MOV R2, A           ; addressing from Acc to R2.
MOV A, R4           ; R4 points to the last character of the string.
MOV R1, A           ; the content of R4 is sent to R1 in order not to modify R4.
MOV A, R7           ; R7 points to the first character of the string.
MOV R0, A           ; the content of R7 is sent to R0 in order not to modify R7.
LOOP1: MOV A, @R0   ; for the first iteration of this loop, The first character is loaded to Acc.
MOV B, @R1          ; for the first iteration of this loop, The last character is loaded to register B.
CJNE A, B, NOTEQUAL ; compares the two characters and jumps to NOTEQUAL if not equal.
INC R0              ; incrementing R0 makes it point to the next character in the forwards direction.
DEC R1              ; decrementing R1 makes it point to the next character in the backwards direction.
DJNZ R2, LOOP1      ; the maximum number of loops for comparison is set by R2.
CLR C               ; if the program gets to this line, the string is palindrome, hence CY = 0.
SJMP SKIP           ; skips the enabling of CY flag.
NOTEQUAL: SETB C    ; if the program jumps to this line, the string is not palindrome, hence CY = 1.
SKIP: RET           ; return to the main.


; Extra subroutines necessary for the program.


; -------------------------------------------------------------------
; STR_LENGTH_DETECT
; -------------------------------------------------------------------
; Purpose: Find the length of the string and the end address of the string
;          in the internal memory
; -------------------------------------------------------------------
; INPUT(S):
;   DPTR - the base address of the string in code memory
;   R7   - the base address of the string in internal memory
; OUTPUT(S): 
;   R3   - the value of the length of the string
;   R4   - the address of the end of the string
; MODIFIES:
;   A    - used for indirect addressing and storing the result of an arithmetic operation
; -------------------------------------------------------------------

STR_LENGTH_DETECT:
MOV R3, #0            ; R3 is initialized with value 0.
LOOP2: CLR A          ; ensures the Acc is empty.
MOVC A, @A+DPTR       ; indirect addressing for loading Acc with the string char.
INC DPTR              ; DPTR points to the next char of the string.
INC R3                ; increments the count number of the length.
CJNE A, #0x00, LOOP2  ; if Acc is loaded with NULL, it indicates that the loop has finished counting.
DEC R3                ; R3 should be decremented as it has counted the NULL char.
CLR A                 ; ensures the Acc is empty.
MOV A, R7             ; loads the Acc with the base address of the string in internal memory.
ADD A, R3             ; result of the addition is the end address of the string and is loaded to Acc.
MOV R4, A             ; the end address is saved in R4 register which will be used by other subroutines.
DEC R4                ; due to addition, R4 points to the next cell to the end address, hence should be decremented.
CLR A                 ; ensures the Acc is empty.
RET                   ; return to the main.


; End of the source file
END


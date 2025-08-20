; In MP2 I was taked to create a stack calculator using various subroutines with usage of JSR system calls. 
; My program begins by jumping to the EVALUATE section which gets input and echoes it via GETC and OUT. It then checks the input and pushes if it is a number and pops if it is
; an  operator. There are several test cases that are made to ensure that the code functions properly, such as underflow checks and so on. After that, I have a PRINT_HEX
; routine that actually prints out the output in hexadecimal, which is similar to my code from MP1, slightly modified albeit. While debugging, I had a major error of my output
; have an infinite amount of 0's at the end, indicating that my program was faulty. In order to fix this, I ensure that I added various HALT's and ended up doing a lot of callee
; saves locally so my program does not mess up whenever I use various registers. If there is a logic error in the expression, my code will produce invalid expression.

.ORIG x3000
    JSR EVALUATE        ; Start the main program
    HALT                ; Halt when EVALUATE is finished


EVALUATE
    ST R7, EVAL_SaveR7  ; CRITICAL: Save R7 at the start of the subroutine.

EVALUATE_LOOP
    GETC                ; R0 gets character from keyboard
    OUT                 ; Echo the character to the console so the user sees it

    ; --- Save the original input character before it gets overwritten ---
    AND R6, R6, #0
    ADD R6, R0, #0      ; R6 now holds the input character

    ; --- Check 1: Is the input an equals sign ('=')? ---
    LD R1, NEG_EQUAL
    ADD R1, R6, R1
    BRz HANDLE_EQUAL

    ; --- Check 2: Is the input a space (' ')? ---
    LD R1, NEG_SPACE
    ADD R1, R6, R1
    BRz EVALUATE_LOOP   ; If space, ignore and get next char

    ; --- Check 3: Is the input a digit ('0' through '9')? ---
    LD R1, NEG_ZERO
    ADD R2, R6, R1
    BRn CHECK_OPERATOR  ; If less than '0', not a digit

    LD R1, NEG_NINE
    ADD R1, R6, R1
    BRp CHECK_OPERATOR  ; If greater than '9', not a digit

    BRnzp HANDLE_DIGIT  ; It is a digit

CHECK_OPERATOR
    ; --- Check 4: Is the input an operator? ---
    LD R1, NEG_PLUS
    ADD R1, R6, R1
    BRz HANDLE_OPERATOR

    LD R1, NEG_MINUS
    ADD R1, R6, R1
    BRz HANDLE_OPERATOR

    LD R1, NEG_MULTIPLY
    ADD R1, R6, R1
    BRz HANDLE_OPERATOR

    LD R1, NEG_DIVIDE
    ADD R1, R6, R1
    BRz HANDLE_OPERATOR

    LD R1, NEG_POWER
    ADD R1, R6, R1
    BRz HANDLE_OPERATOR

    BRnzp PRINT_INVALID ; If it's not a recognized character, error out.

;handler sections

HANDLE_EQUAL
    ; Check if there is exactly one value on the stack.
    LD R1, STACK_START
    LD R2, STACK_TOP
    NOT R2, R2
    ADD R2, R2, #1
    ADD R1, R1, R2      ; R1 = STACK_START - STACK_TOP
    ADD R1, R1, #-1     ; If 1 item, STACK_TOP is STACK_START-1, so result is 0
    BRnp PRINT_INVALID  ; If not zero, error.

    JSR POP
    AND R5, R5, #0
    ADD R5, R0, #0 ; Store R0 into R5 per instruction.

    ST R0, FINAL_RESULT_SAVE ; Save R0 before JSR clobbers it

    ADD R3, R0, #0      ; Move result to R3 for PRINT_HEX
    JSR PRINT_HEX       ; Call print subroutine

    LEA R0, NEWLINE     ; Print a newline for clean output
    PUTS

    LD R7, EVAL_SaveR7  ;  Restore R7 before returning.
    RET                 ; Return to the initial JSR

HANDLE_DIGIT
    LD R1, NEG_ZERO
    ADD R0, R6, R1      ; Use saved character in R6 for conversion (ASCII -> number)
    JSR PUSH
    BRnzp EVALUATE_LOOP

HANDLE_OPERATOR
    ; Check if there are at least two values on the stack to operate on.
    LD R1, STACK_START
    LD R2, STACK_TOP
    NOT R2, R2
    ADD R2, R2, #1
    ADD R1, R1, R2      ; R1 = STACK_START - STACK_TOP
    ADD R1, R1, #-2     ; Check if difference is >= 2
    BRn PRINT_INVALID   ; If less than 2 items, error.

    JSR POP             ; Pop second operand (op2)
    ADD R5, R5, #0      ; Check for underflow
    BRp PRINT_INVALID
    ADD R4, R0, #0      ; Move operand to R4

    JSR POP             ; Pop first operand (op1)
    ADD R5, R5, #0      ; Check for underflow
    BRp PRINT_INVALID
    ADD R3, R0, #0      ; Move operand to R3

    ; Find which operator was entered
    LD R1, NEG_PLUS
    ADD R1, R6, R1
    BRz CALL_PLUS

    LD R1, NEG_MINUS
    ADD R1, R6, R1
    BRz CALL_MINUS

    LD R1, NEG_MULTIPLY
    ADD R1, R6, R1
    BRz CALL_MUL

    LD R1, NEG_DIVIDE
    ADD R1, R6, R1
    BRz CALL_DIV

    LD R1, NEG_POWER
    ADD R1, R6, R1
    BRz CALL_EXP

    BRnzp PRINT_INVALID ; Should not be reached, but just incase.

CALL_PLUS
    JSR PLUS
    BRnzp PUSH_RESULT
CALL_MINUS
    JSR MIN
    BRnzp PUSH_RESULT
CALL_MUL
    JSR MUL
    BRnzp PUSH_RESULT
CALL_DIV
    JSR DIV
    BRnzp PUSH_RESULT
CALL_EXP
    JSR EXP
    BRnzp PUSH_RESULT

PUSH_RESULT
    JSR PUSH
    ADD R5, R5, #0      ; Check for overflow
    BRp PRINT_INVALID
    BRnzp EVALUATE_LOOP

PRINT_INVALID
    LEA R0, INVALID_MSG
    PUTS
    HALT

; --- DATA FOR EVALUATE ROUTINE ---
EVAL_SaveR7         .BLKW #1
NEG_EQUAL           .FILL xFFC3 ; -'='
NEG_SPACE           .FILL xFFE0 ; -' '
NEG_ZERO            .FILL xFFD0 ; -'0'
NEG_NINE            .FILL xFFC7 ; -'9'
NEG_PLUS            .FILL xFFD5 ; -'+'
NEG_MINUS           .FILL xFFD3 ; -'-'
NEG_MULTIPLY        .FILL xFFD6 ; -'*'
NEG_DIVIDE          .FILL xFFD1 ; -'/'
NEG_POWER           .FILL xFFA2 ; -'^'
INVALID_MSG         .STRINGZ "Invalid Expression"
NEWLINE             .STRINGZ "\n"
FINAL_RESULT_SAVE   .BLKW #1
STACK_END           .FILL x3FF0
STACK_START         .FILL x4000
STACK_TOP           .FILL x4000


PRINT_HEX
    ST R3, PRINT_SaveR3 ; Save registers
    ST R7, PRINT_SaveR7

    ; Initialize for the loop.
    ADD R1, R3, #0      ; R1 has the number to print. We will shift it left.
    AND R4, R4, #0
    ADD R4, R4, #4      ; R4 is loop counter for 4 digits.

PRINT_DIGIT_LOOP
    
    AND R0, R0, #0      ; R0 will hold the 4-bit nibble value.
    
    ADD R1, R1, #0      ; Check MSB of R1
    BRzp SHIFT_1
    ADD R0, R0, #8      ; Add 8 to nibble if MSB is 1
SHIFT_1
    ADD R1, R1, R1      ; Shift R1 left
    
    ADD R1, R1, #0
    BRzp SHIFT_2
    ADD R0, R0, #4      ; Add 4 to nibble
SHIFT_2
    ADD R1, R1, R1
    
    ADD R1, R1, #0
    BRzp SHIFT_3
    ADD R0, R0, #2      ; Add 2 to nibble
SHIFT_3
    ADD R1, R1, R1
    
    ADD R1, R1, #0
    BRzp SHIFT_4
    ADD R0, R0, #1      ; Add 1 to nibble
SHIFT_4
    ADD R1, R1, R1

    ; Now R0 has the 4-bit value (0-15). Convert to ASCII and print.
    ADD R2, R0, #-10
    BRn IS_DIGIT
    
    ; It's a letter A-F. R0 has value 10-15.
    LD R2, ASCII_A_OFFSET ; R2 = 'A' - 10
    ADD R0, R0, R2      ; R0 = nibble + ('A' - 10), which is the correct ASCII char
    OUT
    BRnzp NEXT_DIGIT

IS_DIGIT
    ; It's a digit 0-9. The nibble value is still in R0.
    LD R2, ASCII_0      ; Load '0' into a temporary register.
    ADD R0, R0, R2      ; Add the nibble value to '0' to get the ASCII character.
    OUT
    
NEXT_DIGIT
    ADD R4, R4, #-1
    BRp PRINT_DIGIT_LOOP

    LD R7, PRINT_SaveR7 
    LD R3, PRINT_SaveR3
    RET


PRINT_SaveR3 .BLKW #1
PRINT_SaveR7 .BLKW #1
ASCII_0      .FILL x30
ASCII_A_OFFSET .FILL x37 ; 'A' - 10 = x41 - x0A = x37

;arithmetic subroutines, input r3 r4 output r0
PLUS
    ADD R0, R3, R4
    RET

MIN
    NOT R4, R4
    ADD R4, R4, #1
    ADD R0, R3, R4
    RET

MUL
    ST R1, MUL_SaveR1 ;callee saves
    ST R2, MUL_SaveR2
    ST R3, MUL_SaveR3
    ST R4, MUL_SaveR4

    AND R0, R0, #0      ; Clear result R0
    AND R2, R2, #0      ; R2 is the sign flag (0 for pos, 1 for neg)

    ; Check sign of R3 (op1)
    ADD R3, R3, #0
    BRzp R3_POS
    NOT R2, R2          ; Flip sign flag
    NOT R3, R3
    ADD R3, R3, #1      ; R3 = |R3|
R3_POS
    ; Check sign of R4 (op2)
    ADD R4, R4, #0
    BRzp R4_POS
    NOT R2, R2          ; Flip sign flag
    NOT R4, R4
    ADD R4, R4, #1      ; R4 = |R4|
R4_POS
    ADD R1, R4, #0      ; Use R1 as loop counter
    BRz APPLY_SIGN      ; If multiplier is 0, result is 0
MUL_LOOP
    ADD R0, R0, R3
    ADD R1, R1, #-1
    BRp MUL_LOOP
APPLY_SIGN
    ADD R2, R2, #0      ; Check sign flag
    BRz MUL_DONE        ; If positive, we are done
    NOT R0, R0          ; Otherwise, negate the result
    ADD R0, R0, #1
MUL_DONE
    LD R4, MUL_SaveR4
    LD R3, MUL_SaveR3
    LD R2, MUL_SaveR2
    LD R1, MUL_SaveR1
    RET
MUL_SaveR1 .BLKW #1
MUL_SaveR2 .BLKW #1
MUL_SaveR3 .BLKW #1
MUL_SaveR4 .BLKW #1

DIV
    ST R1, DIV_SaveR1 ;callee saves
    ST R2, DIV_SaveR2
    ST R3, DIV_SaveR3
    ST R4, DIV_SaveR4

    AND R0, R0, #0      ; Clear result R0 (quotient)
    AND R2, R2, #0      ; R2 is the sign flag
    
    ADD R4, R4, #0      ; Check for divide by zero
    BRz DIV_DONE        ; If divisor is 0, result is 0

    ; Check sign of R3 (dividend)
    ADD R3, R3, #0
    BRzp DIV_R3_POS
    NOT R2, R2
    NOT R3, R3
    ADD R3, R3, #1
DIV_R3_POS
    ; Check sign of R4 (divisor)
    ADD R4, R4, #0
    BRzp DIV_R4_POS
    NOT R2, R2
    NOT R4, R4
    ADD R4, R4, #1
DIV_R4_POS
    NOT R1, R4
    ADD R1, R1, #1      ; R1 = -|divisor|
DIV_LOOP
    ADD R3, R3, R1      ; Subtract |divisor| from |dividend|
    BRn DIV_APPLY_SIGN
    ADD R0, R0, #1
    BRp DIV_LOOP
DIV_APPLY_SIGN
    ADD R2, R2, #0
    BRz DIV_DONE
    NOT R0, R0
    ADD R0, R0, #1
DIV_DONE
    LD R4, DIV_SaveR4
    LD R3, DIV_SaveR3
    LD R2, DIV_SaveR2
    LD R1, DIV_SaveR1
    RET
DIV_SaveR1 .BLKW #1
DIV_SaveR2 .BLKW #1
DIV_SaveR3 .BLKW #1
DIV_SaveR4 .BLKW #1

EXP
;callee saves    
    ST R1, EXP_SaveR1
    ST R3, EXP_SaveR3
    ST R4, EXP_SaveR4
    ST R7, EXP_SaveR7
    
    ADD R1, R4, #0      ; R1 is loop counter (exponent)
    AND R0, R0, #0
    ADD R0, R0, #1      ; Result is 1 for exponent = 0
    BRz EXP_DONE        ; If exponent is 0, done.

    ; Main loop for exponentiation
EXP_LOOP
    ST R0, EXP_TEMP_RES ; Save current result
    ST R1, EXP_TEMP_CTR ; Save exponent counter
    ADD R4, R3, #0      ; Set R4 = base for MUL
    ADD R3, R0, #0      ; Set R3 = current result for MUL
    JSR MUL             ; R0 = current_result * base
    LD R3, EXP_SaveR3   ; Restore original base
    LD R1, EXP_TEMP_CTR ; Restore exponent counter
    ADD R1, R1, #-1
    BRp EXP_LOOP
    
EXP_DONE
    LD R7, EXP_SaveR7
    LD R4, EXP_SaveR4
    LD R3, EXP_SaveR3
    LD R1, EXP_SaveR1
    RET

EXP_SaveR1 .BLKW #1
EXP_SaveR3 .BLKW #1
EXP_SaveR4 .BLKW #1
EXP_SaveR7 .BLKW #1
EXP_TEMP_RES .BLKW #1
EXP_TEMP_CTR .BLKW #1


PUSH
    ST R3, PUSH_SaveR3
    ST R4, PUSH_SaveR4
    AND R5, R5, #0
    LD R3, STACK_END
    LD R4, STACK_TOP
    ADD R3, R3, #-1
    NOT R3, R3
    ADD R3, R3, #1
    ADD R3, R3, R4
    BRz OVERFLOW
    STR R0, R4, #0
    ADD R4, R4, #-1
    ST R4, STACK_TOP
    BRnzp DONE_PUSH
OVERFLOW
    ADD R5, R5, #1
DONE_PUSH
    LD R3, PUSH_SaveR3
    LD R4, PUSH_SaveR4
    RET
PUSH_SaveR3 .BLKW #1
PUSH_SaveR4 .BLKW #1

POP
    ST R3, POP_SaveR3
    ST R4, POP_SaveR4
    AND R5, R5, #0
    LD R3, STACK_START
    LD R4, STACK_TOP
    NOT R3, R3
    ADD R3, R3, #1
    ADD R3, R3, R4
    BRz UNDERFLOW
    ADD R4, R4, #1
    LDR R0, R4, #0
    ST R4, STACK_TOP
    BRnzp DONE_POP
UNDERFLOW
    ADD R5, R5, #1
DONE_POP
    LD R3, POP_SaveR3
    LD R4, POP_SaveR4
    RET
POP_SaveR3 .BLKW #1
POP_SaveR4 .BLKW #1

.END


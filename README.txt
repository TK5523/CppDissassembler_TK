Readme: C++ Dissassembler for SIC/XE
External variables/data structures and functions
-	Ops and mnemonics keep track of opcodes and corresponding instructions
-	Regs and regMnems keep track of names and indexes for each register
-	hexStr_Num and HexNum_Str convert hex strings to decimal numbers and back to hex strings, for arithmetic operations
-	hexToDec converts hex numbers to decimal. This is a redundant function.
-	Subst is a substring function for character variables
-	Hex_val and dec_val are maps matching dec values to hex values
-	Add_hex is a function to add two hexadecimal numbers. I ended up not using this very often.
-	createbinhexMap is a map that helps convert decimal to binary numbers
-	binToHex converts binary numbers to hex
-	toBinary converts an input string from hex to binary. This was used mostly to separate object codes to bits so I could find the opcodes and address bits for each instruction

main
-	first opens the two input files
-	I declare many variables here to hold values for the current instruction the program is working with
-	I create 2D arrays to hold the symbol table and literal table from the input symbol file, as well as a 2D array to hold the contents of the listing file, which will be used to make the output file.
-	When reading in the symbol file, I skip some lines that I know won’t contain symbols or literals because they would cause issues if I didn’t skip them
-	I use increment variables to keep track of the number of symbols and literals
-	Next I start reading the object file
-	I use a variable ‘inc’ to keep track of the current row of the list file array that is being edited
-	Header records and end records are handled by checking the first character of the object file current line
-	When a text record is found in the object file, I handle the beginning section that holds the object length and starting address of the record first
-	Before reading instructions, it checks for RESW functions by comparing the Program counter to the start address of the current text record
-	Then I have a loop that handles the instructions in the text record file
-	The ‘z’ variable keeps track of the current character in the text record, and increments by 8,4, or 2 for each normal instruction depending on if they are format 4,3, or 2 respectively
-	The loop also uses the length of the current text record to make sure the program doesn’t run off the end and cause errors
-	First checks to see if the instruction is format 2 by comparing the opcode to the opcode for ‘CLEAR’, the only format 2 instruction the disassembler needs to handle.
-	Otherwise, it moves on to record the opcode, and address bits to start determining the format, and addressing modes for target address and operand
-	First checks the extended bit to see if the instruction is format 4
o	In this case, is always direct addressing but the program also checks for indexed addressing
o	Sets the ‘address’ portion of the object code with the last 5 characters of the object code section
o	Increments PC and z accordingly
-	If the instruction is neither format 4 or 2, it must be format 3
o	First checks for PC relative addressing , then base relative, then direct addressing
	Note: the pc relative section will sometimes subtract 1000 in hex from the target address if it falls outside of the range for PC relative. This ensures the operand is always calculated correctly
o	If indexed addressing is detected, the current value of the X register is added to the target address for the instruction
-	Next it handles the operand addressing mode for format 3 and 4 instructions
o	Checks for immediate first, and adds the # symbol to the instruction for the listing file array
o	It compares the displacement or address for format 3 and 4 respectively with the values for symbols in the symbol table to check if the operand is an address
o	Indirect addressing and simple addressing are handled pretty much the same way, except for differing symbols(@ for indirect and nothing for simple)
o	Sets operand to the target address value, then compares this value with values from the symbol and literal tables
o	The operand is set accordingly if a match is found in either table
o	Finally, if the instruction is indexed, a ‘,X’ is added to the instruction in the listing file array
-	Next handles format 2 clear instruction
o	clears the regVals array index corresponding to the appropriate register
o	determines the register that is being cleared by comparing the first register portion of the object code to the value of each index in the register values array
-	after handling these instructions, several rows of the listing file array are updated no matter what format the instruction was: current address, instruction mnemonic, and object code from the text record
-	next the program checks for literals and will overwrite previous changes to the listing file array if literals are found
-	adds a ‘LTORG’ line to the listing file array as well if a literal definition is found
-	another special condition handles the LDB instruction at the beginning of an object file by checking the opcode 
o	alters the base register value according to the address of the symbol that is specified by LDB
-	another special case handles the RSUB instruction, which simply changes the object code to 4F0000 which is assumed to be the object code for all rsub instructions
-	after all of this, several of the variables that hold current values for instructions are cleared for use on the next instruction
finally, the program moves the entries from the listing file array to the output file, inserting white space in place of empty entries

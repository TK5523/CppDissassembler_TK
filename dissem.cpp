//Thomas Kestler
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <fstream>
#include <string>
#include <bitset>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <exception>
#include "math.h"
#include <stdlib.h>
//#include <stdafx.h>
//#include <bits/stdc++.h>
//#include <bits/stdc++.h>


#include <cstdlib> // for exit function
using namespace std;
using std::ifstream;
using std::bitset;
using std::string;
using std::map;
using std::unordered_map;
//using std::encode;

const static string ops[] = {//keeps track of all opcodes
"18", "58", "90", "40", "B4", "28",
"88", "A0", "24", "64", "9C", "C4",
"C0", "F4", "3C", "30", "34", "38",
"48", "00", "68", "50", "70", "08",
"6C", "74", "04", "D0", "20", "60",
"98", "C8", "44", "D8", "AC", "4C",
"A4", "A8", "F0", "EC", "0C", "78",
"54", "80", "D4", "14", "7C", "E8",
"84", "10", "1C", "5C", "94", "B0",
"E0", "F8", "2C", "B8", "DC"
};
const static string mnemonics[] = {//mnemonics for each opcode
"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP",
"COMPF", "COMPR", "DIV", "DIVF", "DIVR", "FIX",
"FLOAT", "HIO", "J", "JEQ", "JGT", "JLT",
"JSUB", "LDA", "LDB", "LDCH", "LDF", "LDL",
"LDS", "LDT", "LDX", "LPS", "MUL", "MULF",
"MULR", "NORM", "OR", "RD", "RMO", "RSUB",
"SHIFTL", "SHIFTR", "SIO", "SSK", "STA", "STB",
"STCH", "STF", "STI", "STL","STS", "STSW",
"STT", "STX", "SUB", "SUBF", "SUBR", "SVC",
"TD", "TIO", "TIX", "TIXR", "WD"
};

const static string regs[]={
"0","1","2","3","4","5","6"

};

const static string regMnems[]={//names for each register
"A","X","L","B","S","T","F"
};

//convert hex string to decimal number
int hexStr_Num (string s){
    return stoul(s,nullptr,16);
}
//convert dec number to hex string
string hexNum_Str(int x){
    stringstream sx;
    sx<<hex<<x;
    return sx.str();
}


//function to convert hex numbers to decimal
unsigned long hexToDec(string hex)
{
    unsigned long result = 0;
    for (int i=0; i<hex.length(); i++) {
        if (hex[i]>=48 && hex[i]<=57)
        {
            result += (hex[i]-48)*pow(16,hex.length()-i-1);
        } else if (hex[i]>=65 && hex[i]<=70) {
            result += (hex[i]-55)*pow(16,hex.length( )-i-1);
        } else if (hex[i]>=97 && hex[i]<=102) {
            result += (hex[i]-87)*pow(16,hex.length()-i-1);
        }
    }
    return result;
}

char* subst(char* arr, int start, int lngth)//substring function for character
{//arguments are character array, starting index and length of the desired substring
    char* st = new char[lngth + 1];
    for (int i = 0; i < lngth; i++)
        st[i] = *(arr + start + i);
    st[lngth] = 0;
    return st;
}

// map that matches hex with decimal values
map<char, int> hex_val(void)
{
    map<char, int> hexmap{ { '0', 0 }, { '1', 1 },
                      { '2', 2 }, { '3', 3 },
                      { '4', 4 }, { '5', 5 },
                      { '6', 6 }, { '7', 7 },
                      { '8', 8 }, { '9', 9 },
                      { 'A', 10 }, { 'B', 11 },
                      { 'C', 12 }, { 'D', 13 },
                      { 'E', 14 }, { 'F', 15 } };
 
    return hexmap;
}
 
// map that matches decimal with hex values
map<int, char> dec_val(void)
{
    map<int, char> decmap{ { 0, '0' }, { 1, '1' },
                      { 2, '2' }, { 3, '3' },
                      { 4, '4' }, { 5, '5' },
                      { 6, '6' }, { 7, '7' },
                      { 8, '8' }, { 9, '9' },
                      { 10, 'A' }, { 11, 'B' },
                      { 12, 'C' }, { 13, 'D' },
                      { 14, 'E' }, { 15, 'F' } };
 
    return decmap;
}
 
// this function adds two hex numbers
string Add_Hex(string a, string b)
{
    map<char, int> m = hex_val();
    map<int, char> k = dec_val();
 
    //swap if second input bigger than first
    if (a.length() < b.length())
        swap(a, b);
 
    // Store length of both strings
    int len = a.length(), len2 = b.length();
    string answer = "";
    // start carry at zero
    int crry = 0, i, j;
 
    // these loops traverse through each input string
    for (i = len - 1, j = len2 - 1;
         j >= 0; i--, j--) {
        //take dec value of numbers from a[i] and b[i]
        int result = m[a[i]] + m[b[j]] + crry;
        // use mod to find addition bit
        int addbit = k[result % 16];
        // combine addbit and answer
        answer.push_back(addbit);
        // update value of crry accordingly
        crry = result /16;
    }
    //iterate thru leftover element of a
    while (i >= 0) {
        //dec value of a[i] value
        int result = m[a[i]] + crry;
        // use modulus to find addition bit
        int addbit = k[result % 16];
        // combine addbit and answer
        answer.push_back(addbit);
        // update value of crry accordingly
        crry = result/16;
        i--;
    }
    // determine if carry still has a value
    if (crry) {
        answer.push_back(k[crry]);
    }
    // output = reversed string
    reverse(answer.begin(), answer.end());
    return answer;
}
//this maps binary # to appropriate hex value
void createbinhexMap(unordered_map<string, char> *binmap)
{
    (*binmap)["0000"] = '0';
    (*binmap)["0001"] = '1';
    (*binmap)["0010"] = '2';
    (*binmap)["0011"] = '3';
    (*binmap)["0100"] = '4';
    (*binmap)["0101"] = '5';
    (*binmap)["0110"] = '6';
    (*binmap)["0111"] = '7';
    (*binmap)["1000"] = '8';
    (*binmap)["1001"] = '9';
    (*binmap)["1010"] = 'A';
    (*binmap)["1011"] = 'B';
    (*binmap)["1100"] = 'C';
    (*binmap)["1101"] = 'D';
    (*binmap)["1110"] = 'E';
    (*binmap)["1111"] = 'F';
}
// convert binary input to hex
string BinToHex(string binst)
{
    int l = binst.size();
    int t = binst.find_first_of('.');
    //length between beginning of string and '.'
    int leftlength = t != -1 ? t : l;
    // add zeros to beginning to make %4 work
    for (int i = 1; i <= (4 - leftlength % 4) % 4; i++)
        binst = '0' + binst;
     
    // check if input has decimal point 
    if (t != -1)   
    {
        // string length after dec point '.'
        int rightlength = l - leftlength - 1;
        // for rightlength %4 to work, add zeros at end
        for (int i = 1; i <= (4 - rightlength % 4) % 4; i++)
            binst = binst + '0';
    }
     
    // mapping binary to hex code
    unordered_map<string, char> bin_hex_map;
    createbinhexMap(&bin_hex_map);
     
    int i = 0;
    string hex = "";
     
    while (1)
    {
        //start from left, take size 4 substring and add that hex code
        hex += bin_hex_map[binst.substr(i, 4)];
        i += 4;
        if (i == binst.size())
            break;
             
        // add dec point to result if detected
        if (binst.at(i) == '.')   
        {
            hex += '.';
            i++;
        }
    }
    // return the hex result
    return hex;   
}
string toBinary(string inp){//converts input string from hex to binary
    string result = "";
			for (int i = 0; i < inp.length (); ++i)
			{
				switch (inp [i])
				{
					case '0': result.append ("0000"); break;
					case '1': result.append ("0001"); break;
					case '2': result.append ("0010"); break;
					case '3': result.append ("0011"); break;
					case '4': result.append ("0100"); break;
					case '5': result.append ("0101"); break;
					case '6': result.append ("0110"); break;
					case '7': result.append ("0111"); break;
					case '8': result.append ("1000"); break;
					case '9': result.append ("1001"); break;
					case 'A': result.append ("1010"); break;
					case 'B': result.append ("1011"); break;
					case 'C': result.append ("1100"); break;
					case 'D': result.append ("1101"); break;
					case 'E': result.append ("1110"); break;
					case 'F': result.append ("1111"); break;
				}
			}
			return result;


}


int main() {


    try//this will catch exceptions
    {
        
        ifstream indata; // will hold values from input object file
   
    char wrd[100];//input value for object file
   
    indata.open("test.obj"); // opens object code file
    if(!indata) { // error if processing the input file fails
        cerr << "Error: Could not open test obj file" << endl;
        exit(1);
    }
    ifstream symTable;//same idea here for symbol input file
    symTable.open("test.sym");
    if(!symTable) { 
      cerr << "Error: Could not open test sym file" << endl;
      exit(1);
   }
    char* progName;//hold name of prog from header
    char* stAdr;//start addr from header
    char* objLen;//length of program from header
    char* firstInstAdr;//address of first instruction in hex, from end record
    string bincode;//will hold binary value of  object code for current instruction
    string opcode; //opcode for current instruction
    string adrBits; //address section(6 bits) for current instruction
    string disp; //displacement for format 3 instructions
    string addr; //address for format 4 instructions
    char* txtRecord; //holds current line of text record
    string PC=BinToHex("0"); //keeps track of current Program counter value
    //variables for values of registers
    string regVals[7]={"0","0","0","0","0","0","0"};
    string TA; //holds target address value
    string operand;//operand value for current instruction
    int format;//holds value of format 2,3,or 4 for current instr
    string opAdrMode;//stores addressing mode of current section of object code(immediate,simple or indirect)
    bool indexed=false;//if current instruction has indexed addressing or not
    //variables holding flag bits below: nixpbe
    string fn;
    string fi;
    string fx;
    string fb;
    string fp;
    string fe;
    //variables for output file by column
    string curAdr;//col 1, current address
    string Instr; //col 3, the instruction
    string keyword2; //col 4: the the operand for the current instruction
    string objCode;//col 5: object code of cur instruction
    //variables for symbol table
    string SymTableArr[10][3];
    string LitTableArr[20][4];//variables for literal section of symbol table
    string curLLength;//this is not used for anything, but removing it causes a seg fault for some unknown reason
    string curLAdr;//same as above curllength

    int ltaRows=0; //keeps track of number of filled rows in above array
    char symInput[15];//holds current line of sym table input file
    char junk[100]; //this holds skipped lines of the sym table file
    bool breakLp=false;//this tells program when to stop reading from sym file
    bool ifRSUB=false;//this tells program whether to use special RSUB case
    string ListFileArr[100][5];//array holding contents of output listing file
    int LFileRows=0;//keeps track of # of filled rows of above array
      for(int x=0;x<2;x++){//this loop skips the first two lines of the sym table file
      //this is done because those lines would cause issues
            symTable.getline(junk,100,'\n');
        }

    symTable >> symInput;//read next line of sym file

    for (int j=0; !symTable.eof(); j++) {//read symbol table section of sym file. j keeps track of current line of array

        for(int i=0;i<=2;i++){
            SymTableArr[j][i]=symInput;
            if( (SymTableArr[j][i]) == "Name"){//this indicates start of literal table
                breakLp=true;
                SymTableArr[j][i] = "";
                break;
            }
            symTable >> symInput;
        }
        if(breakLp){//stop adding to sym table array
               symTable >> symInput;
               break;
           }
    }
    for(int x=0;x<1;x++){//skip next two lines to get to literal table
            symTable.getline(junk,100,'\n');
        }
    symTable>>symInput;
    symTable>>symInput;//skips lines

    for(int k=0;!symTable.eof();k++){//adds remaining lines of sym file to literal table
        for(int g=0;g<=3;g++){
            if(g==0){
                LitTableArr[k][g]="   ";
            }
            else{
            LitTableArr[k][g]=symInput;
            ltaRows++;
            symTable>>symInput;
            }
        }
    }
    int rows= sizeof SymTableArr / sizeof SymTableArr[0];
    int columns=3;//these two variables are for the # rows and cols of symbol table
    int symRows=0;
            for(int i=0; i<rows; i++){
                if(SymTableArr[i][2]=="R"){//set R flag for all lines in symbol table bc assuming they are all relative
                    symRows++;
                }
            }

    rows=sizeof LitTableArr / sizeof LitTableArr[0];
    columns = 4;
int inc;//keeps track of current line of output listing file
indata >> wrd;//take in current line of input obj file
inc=0;//initialize inc to zero
   while (!indata.eof()) { // stops when the end of the file is reached

        if(wrd[0]=='H'){//see if Header record
            //take substring of next 6 values after H
            //store prog name, start address, and object length and record in current line of listing file array
           
            cout<<"found header file: \n";
            
            progName=subst(wrd,1,6);
            stAdr=subst(wrd,7,6);
            objLen=subst(wrd,13,6);
            ListFileArr[inc][0]=subst(stAdr,1,4);
            ListFileArr[inc][1]=progName;
            ListFileArr[inc][3]="START";
            ListFileArr[inc][4]="0";
            inc++;
            LFileRows++;
        }
        else if(wrd[0]=='E'){
            //if end record found, then we should be at the end of the obj file
            firstInstAdr=subst(wrd,1,6);
            ListFileArr[inc][2]="END";
            for(int i=0; i<symRows; i++){//refer back to # rows in SymTableArr
                if(SymTableArr[i][1]==firstInstAdr){//check for equivalent address in 'value' column of symbol table
                    ListFileArr[inc][3]=SymTableArr[i][0];//record symbol corresponding to 'first inst adr' in listing file           
                }
            }
            LFileRows++;
        }
        else if(wrd[0]=='T'){//this section handles text records
            
            int i = 0;//placeholder value
            txtRecord=subst(wrd,1,strlen(wrd)-1);//take current text record
            string recStAdr=subst(txtRecord,0,6);
            string recObjLen=subst(txtRecord,5,2);//this is used how much of the current line to read before stopping
            curAdr=recStAdr.substr(2,4);
            //the above statements handle the first section of each text record containing object code length and 
            //the start address of the first instruction of the current text record
            
            //this handles RESW functions
            if(curAdr!=PC){//if cur address is larger than the PC, this means a RESW function was found
            int decPC = hexToDec(PC);//decimal version of current PC value
            for(int i=0; i<symRows; i++){//refer back to # rows in SymTableArr
            int symValDec = hexToDec(SymTableArr[i][1].substr(2,4));//current symbol value from sym table
            int caDec = hexToDec(curAdr);//hold decimal value of current address
           
                if((caDec>(symValDec-decPC)) && (decPC <= symValDec)&& (caDec>symValDec)){//these conditions find the symbol from the sym table that matches the change in current address
                    ListFileArr[inc][0]=SymTableArr[i][1].substr(2,4);
                    ListFileArr[inc][1]=SymTableArr[i][0];
                    ListFileArr[inc][2]="RESW";
                    
                    int reswValue=(caDec-symValDec)/3;//divide by 3 to get appropriate number of bytes
                    if(ListFileArr[inc-1][2]=="RESW"){//this checks to see if there is a second RESW above this one
                        ListFileArr[inc-1][3]=to_string(stoi(ListFileArr[inc-1][3])-reswValue);
                    }
                    ListFileArr[inc][3]=to_string(reswValue);
                    inc++;//increment inc so lines are not skipped
                    LFileRows++;//increment this so all lines of the listing file array will be given to output file
                }
            }
        }
            PC=curAdr;//update PC so it matches the start address of the current text record
            int z=8;//this will be for recording instructions after the beginning section of the text record
            while( z<strlen(txtRecord)){//length of text record is different every time, this prevents program from running off end of it
            bincode = toBinary(subst(txtRecord,z,2));//only stores opcode until the format is determined
            opcode = bincode.substr(0,6)+"00";//take first 6 bits of curr obj code and appends 2 zeros at the end to get op code
            for(int w=0;w<59;w++){//iterate through ops array to find appropriate opcode
                if(opcode==toBinary(ops[w])){
                    Instr=mnemonics[w];
                }
            }
            string opcodef2=bincode.substr(0,8);//this only exists to check if the instruction is format 2
    
            if(opcodef2!=(toBinary(ops[4]))){//check if format 2 or not

                bincode = toBinary(subst(txtRecord,z,3));//store opcode and address bits until format 3 vs 4 is determined
                
                adrBits=bincode.substr(6,6);//nixbpe bits
              
                fn=adrBits.substr(0,1);//keeping track of addr bits
                fi=adrBits.substr(1,1);
                fx=adrBits.substr(2,1);
                fb=adrBits.substr(3,1);
                fp=adrBits.substr(4,1);

                if((adrBits.substr(5,1))=="1"){//check: if 'e' is set, then format 4
                    bincode = toBinary(subst(txtRecord,z,8));
                    addr=subst(txtRecord,z+3,5);//holds address portion of object code
                    curAdr=PC;

                    PC=Add_Hex(PC,"4");//add 4 to PC bc format 4
                    Instr="+"+Instr;//format 4 instructions have '+' at beginning so this takes care of that
                    format=4;//this is used when checking addressing mode for operand
                    if(fx=="1"){//check for indexed addressing:x=1
                    TA=BinToHex(toBinary(addr)+toBinary(regVals[1]));//add value of X register to the address
                    indexed=true;
                    }
                    TA=addr;//since format 4 is direct addressing, set target address to address portion
                    objCode=subst(txtRecord,z,8);
                    ListFileArr[inc][4]= objCode;
                    z+=8;//move z up 8 characters to match start of next instruction
                }
                else{//this else part handles format 3 instructions
                bincode = toBinary(subst(txtRecord,z,6));
                disp=subst(txtRecord,z+3,3);//displacement portion: last 3 hex characters of current object code from txt record
                curAdr=PC;

                PC=Add_Hex(PC,"3");//add 3 bc format 3
                format=3;
                objCode=subst(txtRecord,z,6);
                ListFileArr[inc][4]= objCode;
                
                //determine addressing mode for target address
                if(fb=="0"&&fp=="1"){//check if PC relative: b=0,p=1
                    int iPC=hexStr_Num(PC);
                    int idsp=hexStr_Num(disp);
                    int calcres = iPC+idsp;
                    //these above vars are converted from hex to decimal so operations can be performed on them properly
                    if(idsp>2048){//correct value of displacement if it falls outside of range
                        TA=hexNum_Str(calcres-4096);
                    }
                    else{
                    TA=hexNum_Str(calcres); //PC + disp
                    }//calculate target address
                }
                else if(fb=="1"&&fp=="0"){//if base relative, b=1,p=0
                    int iBse=hexStr_Num(regVals[3]);//fetches current value of base register
                    int idsp=hexStr_Num(disp);
                    TA=hexNum_Str(iBse+idsp);//calculate target address: B + disp
                }
                else if(fb=="0"&&fp=="0"){//if b,p=0 then direct addressing
                    TA=disp;
                }

                if(fx=="1"){//check for indexed addressing:x=1          
                    indexed=true;
                    if(regVals[1]!="0"){
                    TA=hexNum_Str(hexStr_Num(disp)+hexStr_Num(regVals[1]));//add value of X register to the address if the value is not zero
                    }
                    
                }
                z+=6;//move z up 6 characters to match start of next instruction
            }
                for(int i=0; i<symRows; i++){//this sets col 1 of current line of listing file to the appropriate symbol if the current address matches the symbol value
                        if(SymTableArr[i][1].substr(2,4)==curAdr){//check for equivalent address in 'value' column of symbol table
                            ListFileArr[inc][1]=SymTableArr[i][0];
                        }
                    }
                //determine addressing mode for operand
                if(fi=="1"&&fn=="0"){//check for immediate addressing
                opAdrMode="immediate";
                //format 3 is for 'op #c' or op #m instructions: sets the fourth column of listing file array to appropriate value
                if(format==3){
                    operand=TA;
                    keyword2="#";//immediate instructions start with this in listing file
                    keyword2+=to_string(hexToDec(objCode.substr(3,3)));
                    ListFileArr[inc][3]=keyword2;  

                    for(int i=1; i<symRows; i++){//this handles 'op #m' instructions for format 3
                        if(SymTableArr[i][1].substr(3,3)==objCode.substr(3,3)){//check for equivalent address in 'value' column of symbol table
                            keyword2="#";
                            keyword2+=SymTableArr[i][0];
                            ListFileArr[inc][3]=keyword2;//sets operand to appropriate symbol if value matches address in object code
                        }
                    }
                }
                else if(format==4){//'op #m' instructions: finds appropriate symbol 
                    for(int i=0; i<symRows; i++){//refer back to # rows in SymTableArr
                        if(SymTableArr[i][1].substr(1,5)==objCode.substr(3,5)){//check for equivalent address in 'value' column of symbol table
                            keyword2="#";
                            keyword2+=SymTableArr[i][0];
                            ListFileArr[inc][3]=keyword2;//sets operand to appropriate symbol if value matches address in object code
                            

                        }
                    }
                }
                }
                else if(fi=="0"&&fn=="1"){//check for indirect addressing: "op @m" case for format 3 or 4
                    opAdrMode="indirect";
                    keyword2="@"+keyword2;
                    //case is same for format 3 or 4
                    if(format==3){
                        int iTA=hexStr_Num(TA);
                        operand=hexNum_Str(iTA);
                    }
                    else if(format==4){
                        int iTA=hexStr_Num(TA);
                        operand=hexNum_Str(iTA);
                    }
                    ListFileArr[inc][3]+=keyword2;
                }
                else if(fi=="1"&&fn=="1"){//check for simple addressing: "op m or op m,X"
                    opAdrMode="simple";
                    //case is same with format 3 or 4
                    //finding the operand is same as for indirect
                    if(format==3){
                        int iTA=hexStr_Num(TA);
                        operand=hexNum_Str(iTA);
                    }
                    else if(format==4){
                        int iTA=hexStr_Num(TA);
                        operand=hexNum_Str(iTA);
                    }
                }
                if(opAdrMode=="simple"|| opAdrMode=="indirect"){//since operand found same way for simple/indirect, only need one case here
                for(int i=0; i<symRows; i++){//refer back to # rows in SymTableArr
                if(operand!=""){//avoid empty operands to prevent stoul exception
                        if(hexStr_Num(SymTableArr[i][1])==hexStr_Num(operand)){//check for equivalent address in 'value' column of symbol table
                            keyword2+=SymTableArr[i][0];
                            ListFileArr[inc][3]=keyword2;//change operand column in listing file to the appropriate symbol for matching address value
                        }
                }
                    }
                     for(int r=0;r<ltaRows;r++){
                        if(LitTableArr[r][3]!=""){//avoid empty entries to prevent stoul exception
                        if(hexStr_Num(LitTableArr[r][3])==hexStr_Num(operand)){
                            keyword2+=LitTableArr[r][1];
                            ListFileArr[inc][3]=keyword2;//if the address instead matches a literal, set that as the operand
                            
                        }
                        }
                }
                }
                if(indexed==true){
                    ListFileArr[inc][3]+=",X";//this is done to indiciate indexed instruction in listing file
                }

            }
            else{//case if working with CLEAR, format 2 instruction
                format=2;
                bincode = toBinary(subst(txtRecord,z,4));
                objCode=subst(txtRecord,z,4);
                ListFileArr[inc][4]= objCode;
                string curReg=objCode.substr(2,1);//sets this to first half byte in instruction, the register CLEAR will be altering
                curAdr=PC;
                for(int u=0;u<7;u++){//iterate through register array and alter the appropriate one according to first register specified by object code
                
                    if(curReg==to_string(u)){
                        regVals[u]=toBinary("0");//clear appropriate register
                        ListFileArr[inc][3]=regMnems[u];//record register that was cleared in listing file
                    }
                }
               PC=Add_Hex(PC,"2");//add 2 to PC because format 2
                z+=4;//move up 4 characters to next instruction
            }
            LFileRows++;
            //these instructions increment # rows in listing file array accordingly and record the appropriate information: address, instruction and object code
            ListFileArr[inc][0]=curAdr;
            ListFileArr[inc][2]+=Instr;
            ListFileArr[inc][4]= objCode;

            //this will handle literals: if a literal is detected in the obj file, overrite previous changes to ListFileArr accordingly
            int lastZ=z-(format*2);//use format variable to find address of z for instruction before it was incremented
            string tempBCode = subst(txtRecord,lastZ,6);//takes bincode from instruction that was evaluated in this loop cycle
            
            for(int h=0;h<ltaRows;h++){//find the literal corresponding to the appropriate value
                if(LitTableArr[h][1]!=""){
                    
                    if((LitTableArr[h][1].substr(3,6))==tempBCode){
                        //add LTORG before each literal definition
                        ListFileArr[inc][0].clear();
                        ListFileArr[inc][1].clear();
                        ListFileArr[inc][2]="LTORG";
                        ListFileArr[inc][3].clear();
                        ListFileArr[inc][4].clear();//set line in listing file to LTORG and then set next line to the appropriate literal
                        inc++;
                        LFileRows++;

                        ListFileArr[inc][2]='*';//indicates literal definition in listing file
                        ListFileArr[inc][1].clear();
                        ListFileArr[inc][3]=LitTableArr[h][1];
                        ListFileArr[inc][0]=LitTableArr[h][3].substr(2,4);
                        ListFileArr[inc][4]=objCode;

                    }
                }
            }

            //condition for LDB instruction at beginning of file
            if(opcode=="01101000"){
                inc++;
                LFileRows++;
                ListFileArr[inc][2]="BASE";
                ListFileArr[inc][3]=ListFileArr[inc-1][3].substr(1,ListFileArr[inc-1][3].length());//uses same operand from previous LDB instruction
                for(int i=0; i<symRows; i++){//refer back to # rows in SymTableArr
                if((SymTableArr[i][1])!=""&&(ListFileArr[inc][3])!=""){//this is to avoid stoul exceptions by converting empty values
                        if(SymTableArr[i][0]==ListFileArr[inc][3]){//check for equivalent address in 'value' column of symbol table
                            regVals[3]=SymTableArr[i][1];
                        }
                }
                    }
            }
            else if(opcode=="01001100"){//special case: check for RSUB instruction
                inc++;
                LFileRows++;
                //overwrite previous changes if RSUB is found
                ListFileArr[inc][1].clear();
                ListFileArr[inc][2]="RSUB";
                ListFileArr[inc][3].clear();
                ListFileArr[inc][4]="4F0000";//assuming that object code for RSUB is always 4F0000

            }
            keyword2.clear();
            Instr.clear();
            indexed=false;
            ifRSUB=false;
            //this clears and resets variables used for current loop cycle so they don't mess up the next instruction
            inc++;
            }//end of for loop for going through text record instructions

        }
       
        indata >> wrd;
        
   }
   indata.close();
  
   int Lrows= sizeof ListFileArr / sizeof ListFileArr[0];
    
   int Lcols=5;
    

    //begin writing listing file array to output file
    ofstream outLFile;
    outLFile.open("out.lst");

    for(int r=0;r<=LFileRows;r++){
        for(int c=0;c<Lcols;c++){
            if(ListFileArr[r][c]==""){
                ListFileArr[r][c]="     ";//add empty space to empty entries in array for formatting purposes
                
            }
            cout << ListFileArr[r][c]<<"     ";
            outLFile<<ListFileArr[r][c]<<"     ";

            
        }
        cout << endl;
        outLFile<<endl;
        
    }

    outLFile.close();
    //outLFile.open


    }
    catch (std::exception const &exc)//catch exceptions if encountered
    {
        std::cerr << "Exception caught " << exc.what() << "\n";
    }
    catch (...)
    {
        std::cerr << "Unknown exception caught\n";
    }


    


   return 0;
}
/**
 * @file   main.cpp
 * @author Jarrod Brunson
 * @date   05.19.16
 * @brief  Simple MIPS Assebmler
 *
 * @description
 * This program implements an assembler for a subset
 * of the MIPS assembly language.
 *****************************************************/

#ifndef main_CPP
#define main_CPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <list>
#include <boost/tokenizer.hpp>
#include <vector>


/********************************************
 *               Command Class              *
 ********************************************/

struct Command
{
  int                       lineNum;                  //asmFile line number
  std::vector<std::string>  tokens_vec;               //vector to store tokens
  std::string               type = "empty";           //instruction type
  int                       machine = 0;              //machine code
 
  Command(std::string, int);                          //constructor
  bool IsLabel();                                     //return true iff label
  std::string GetRIJType();                           //determine R, I or J type instructions
  int GetOp();                                        //returns op code value
  int GetReg(std::string);                            //returns register value
  int GetFunct();                                     //returns funct code value
  void ResolveMachine();                              //assembly instruction -> machine code
};

/******************************************
 *               Label Class              *
 ******************************************/

struct Label
{
  int                       lineNum;                  //asmFile line number
  std::vector<std::string>  tokens_vec;               //vector to store tokens
  std::string               type = "empty";           //instruction type
  int                       machine = 0;              //machine code
 
  Label(std::string, int);                            //constructor
};




/***********************************************
 *               Command Methods               *
 ***********************************************/

Command::Command(std::string command, int lineNum) : lineNum(lineNum)
{
  //creates tokens from assembly instruction, stores in vector
  boost::char_separator<char> delimeter(", ()");
  boost::tokenizer< boost::char_separator< char > > tokens(command,delimeter);
  for(boost::tokenizer< boost::char_separator<char> >::iterator it = tokens.begin();
      it != tokens.end(); it++)
  {
    tokens_vec.push_back(*it);
  }
}



bool Command::IsLabel()
{
  //only labels contain ':'
  size_t result = tokens_vec[0].find(':');
  if (result == std::string::npos)
    return false;
  return true;
}



std::string Command::GetRIJType()
{
  std::string result = "0";

  //search for R type instruction
  if(tokens_vec[0] == "addu")
  {
    result = "r";
  }
  else if(tokens_vec[0] == "and")
  {
    result = "r";
  }
  else if(tokens_vec[0] == "div")
  {
    result = "r";
  }
  else if(tokens_vec[0] == "mfhi")
  {
    result = "r";
  }
  else if(tokens_vec[0] == "mflo")
  {
    result = "r";
  }
  else if(tokens_vec[0] == "mult")
  {
    result = "r";
  }
  else if(tokens_vec[0] == "or")
  {
    result = "r";
  }
  else if(tokens_vec[0] == "slt")
  {
    result = "r";
  }
  else if(tokens_vec[0] == "subu")
  {
    result = "r";
  }
  else if(tokens_vec[0] == "syscall")
  {
    result = "r";
  } 
  //search for I type instruction
  else if(tokens_vec[0] == "addiu")
  {
    result = "i";
  }
  else if(tokens_vec[0] == "beq")
  {
    result = "i";
  }
  else if(tokens_vec[0] == "bne")
  {
    result = "i";
  }
  else if(tokens_vec[0] == "lw")
  {
    result = "i";
  }
  else if(tokens_vec[0] == "sw")
  {
    result = "i";
  }
  //search for J type instruction
  else if(tokens_vec[0] == "j")
  {
    result = "j";
  }
  
  return result;
}



int Command::GetOp()
{
  if(tokens_vec[0] == "addiu")
    return 9;
  else if(tokens_vec[0] == "beq")
    return 4;
  else if(tokens_vec[0] == "bne")
    return 5;
  else if(tokens_vec[0] == "j")
    return 2;
  else if(tokens_vec[0] == "lw")
    return 35;
  else if(tokens_vec[0] == "sw")
    return 43;
  else
    return 0;
}



int Command::GetReg(std::string reg)
{
  if(reg == "$zero" || reg == "$0")
    return 0;
  else if (reg == "$at")
    return 1;
  else if (reg == "$v0")
    return 2;
  else if (reg == "$v1")
    return 3;
  else if (reg == "$a0")
    return 4;
  else if (reg == "$a1")
    return 5;
  else if (reg == "$a2")
    return 6;
  else if (reg == "$a3")
    return 7;
  else if (reg == "$t0")
    return 8;
  else if (reg == "$t1")
    return 9;
  else if (reg == "$t2")
    return 10;
  else if (reg == "$t3")
    return 11;
  else if (reg == "$t4")
    return 12;
  else if (reg == "$t5")
    return 13;
  else if (reg == "$t6")
    return 14;
  else if (reg == "$t7")
    return 15;
  else if (reg == "$s0")
    return 16;
  else if (reg == "$s1")
    return 17;
  else if (reg == "$s2")
    return 18;
  else if (reg == "$s3")
    return 19;
  else if (reg == "$s4")
    return 20;
  else if (reg == "$s5")
    return 21;
  else if (reg == "$s6")
    return 22;
  else if (reg == "$s7")
    return 23;
  else if (reg == "$t8")
    return 24;
  else if (reg == "$t9")
    return 25;
  else if (reg == "$k0")
    return 26;
  else if (reg == "$k1")
    return 27;
  else if (reg == "$gp")
    return 28;
  else if (reg == "$sp")
    return 29;
  else if (reg == "$fp")
    return 30;
  else if (reg == "$ra")
    return 31;
  
  //argument not a register
  else
    return 99;
}



int Command::GetFunct()
{ 
  if(tokens_vec[0] == "addu")
    return 33;
  else if(tokens_vec[0] == "and")
    return 36;
  else if(tokens_vec[0] == "div")
    return 26;
  else if(tokens_vec[0] == "mfhi")
    return 16;
  else if(tokens_vec[0] == "mflo")
    return 18;
  else if(tokens_vec[0] == "mult")
    return 24;
  else if(tokens_vec[0] == "or")
    return 37;
  else if(tokens_vec[0] == "slt")
    return 42;
  else if(tokens_vec[0] == "subu")
    return 35;
  else if(tokens_vec[0] == "syscall")
    return 12;
  else
    return 0;
}

void Command::ResolveMachine()
{
  //J type instructions, label and address
  if (tokens_vec[0] == "j")
  {
    int op = GetOp();
    op = op << 26;
    int targetLocation = stoi(tokens_vec[1]);
    machine = op + targetLocation;
    return;
    }

  //I type arithmetic instruction, two registers and immediate
  if (tokens_vec[0] == "addiu")
  {
    int op = GetOp();
    op = op << 26;
    int rs = GetReg(tokens_vec[2]);
    rs = rs << 21;
    int rt = GetReg(tokens_vec[1]);
    rt = rt << 16;
    int immediate;
    immediate = stoi(tokens_vec[3]);
    machine = op + rs + rt + immediate;
    return;
  }

  
  //I type branch instruction, two registers and loocation
  //16 bit unsigned int for immediate used to get unsigned value of 2's
  //compliment offset
  if (tokens_vec[0] == "beq" || tokens_vec[0] == "bne")
  {
    int op = GetOp();
    op = op << 26;
    int rs = GetReg(tokens_vec[1]);
    rs = rs << 21;
    int rt = GetReg(tokens_vec[2]);
    rt = rt << 16;
    int targetLocation = stoi(tokens_vec[3]);
    int programCounter = lineNum + 1;
    short unsigned int immediate = targetLocation - programCounter;
    machine = op + rs + rt + immediate;
    return;
    }

  //I type memory instruction, two registers and offset
  if (tokens_vec[0] == "lw" || tokens_vec[0] == "sw")
  {
    int op = GetOp();
    op = op << 26;
    int rs = GetReg(tokens_vec[3]);
    rs = rs << 21;
    int rt = GetReg(tokens_vec[1]);
    rt = rt << 16;
    int immediate;
    immediate = stoi(tokens_vec[2]);
    machine = op + rs + rt + immediate;
    return;
  }
  
  //R type instruction w/3 operands
  else if (tokens_vec[0] == "addu" || tokens_vec[0] == "and" || tokens_vec[0] == "or" ||
           tokens_vec[0] == "slt" || tokens_vec[0] == "subu")
  {   
    int op = GetOp();
    op = op << 26;
    int rs = GetReg(tokens_vec[2]);
    rs = rs << 21;
    int rt = GetReg(tokens_vec[3]);
    rt = rt << 16;
    int rd = GetReg(tokens_vec[1]);
    rd = rd << 11;
    int funct = GetFunct();
    machine = op + rs + rt + rd + funct;
    return;
  }

  //R type instruction w/2 operands
  else if (tokens_vec[0] == "div" || tokens_vec[0] == "mult")
  {
    int op = GetOp();
    op = op << 26;
    int rs = GetReg(tokens_vec[1]);
    rs = rs << 21;
    int rt = GetReg(tokens_vec[2]);
    rt = rt << 16;
    int funct = GetFunct();
    machine = op + rs + rt + funct;
    return;
  }

  //R type instruction w/1 operand
  else if (tokens_vec[0] == "mfhi" || tokens_vec[0] == "mflo")
  {
    int op = GetOp();
    op = op << 26;
    int rd = GetReg(tokens_vec[1]);
    rd = rd << 11;
    int funct = GetFunct();
    machine = op + rd + funct;
    return;
  }

  //R type instruction w/0 operands
  else if (tokens_vec[0] == "syscall")
  {
    int op = GetOp();
    op = op << 26;
    int funct = GetFunct();
    machine = op + funct;
    return;
  }
  
  else
  {
    return;
  }
}



/*********************************************
 *               Label Methods               *
 *********************************************/

Label::Label(std::string command, int lineNum) : lineNum(lineNum)
{
  //creates tokens from assembly instruction, stores in vector
  boost::char_separator<char> delimeter(", ():");
  boost::tokenizer< boost::char_separator< char > > tokens(command,delimeter);
  for(boost::tokenizer< boost::char_separator<char> >::iterator it = tokens.begin();
      it != tokens.end(); it++)
  {
    tokens_vec.push_back(*it);
  }
}



/**************************************************
 *               Non-member Methods               *
 *************************************************/

std::ostream& operator << (std::ostream& os, Command command)
{
  //for debugging
  //output Command data mambers
  std::cout << "Tokens: ";
  for (int i = 0; i < command.tokens_vec.size(); ++i)
    std::cout << command.tokens_vec[i] << " ";
  std::cout << std::endl;

  std::cout << "Line Number:      " << command.lineNum << std::endl;

  std::cout << "Instruciton Type: " << command.type << std::endl;

  std::cout << "Machine: " << std::setw(8) << std::hex << std::setfill('0') << command.machine << std::endl;

  std::cout << std::dec;
}



std::ostream& operator << (std::ostream& os, Label command)
{
  //for debugging
  //output label data mambers
  std::cout << "Tokens: ";
  for (int i = 0; i < command.tokens_vec.size(); ++i)
    std::cout << command.tokens_vec[i] << " ";
  std::cout << std::endl;

  std::cout << "Line Number:      " << command.lineNum << std::endl;

  std::cout << "Instruciton Type: " << command.type << std::endl;

  std::cout << "Machine: " << std::setw(8) << std::hex << std::setfill('0') << command.machine << std::endl;

  std::cout << std::dec;
}



void ResolveLabels(std::list<Command>& cmdList, std::list<Label> lblList)
{
  std::list<Command>::iterator cmdIter = cmdList.begin();

  for (size_t i = 0; i < cmdList.size(); ++i)
  {
    //for lw and sw instructions
    if (cmdIter->tokens_vec[0] == "lw" || cmdIter->tokens_vec[0] == "sw")
    {
      std::string label = cmdIter->tokens_vec[2];
      std::list<Label>::iterator lblIter = lblList.begin();
      for (size_t j = 0; j < lblList.size(); ++j)
      {
        if (label == lblIter->tokens_vec[0])
        {
          cmdIter->tokens_vec[2] = lblIter->tokens_vec[2]; 
          break;
        }
        ++lblIter;
      }
    }

    //for beq and bne instructions
    if(cmdIter->tokens_vec[0] == "beq" || cmdIter->tokens_vec[0] == "bne")
    {
      std::string label = cmdIter->tokens_vec[3];
      std::list<Label>::iterator lblIter = lblList.begin();
      for (size_t j = 0; j < lblList.size(); ++j)
      {
        if (label == lblIter->tokens_vec[0])
        {
          cmdIter->tokens_vec[3] = std::to_string(lblIter->lineNum); 
          break;
        }
        ++lblIter;
      }
    }

    //for j instructions
    if(cmdIter->tokens_vec[0] == "j")
    {
      std::string label = cmdIter->tokens_vec[1];
      std::list<Label>::iterator lblIter = lblList.begin();
      for (size_t j = 0; j < lblList.size(); ++j)
      {
        if (label == lblIter->tokens_vec[0])
        {
          cmdIter->tokens_vec[1] = std::to_string(lblIter->lineNum); 
          break;
        }
        ++lblIter;
      }
    }
    
    ++cmdIter;
  }  
}



/***********************************
 *               Main              *
 ***********************************/

int main(int argc, char* argv[])
{
  //open assembly file from command line
  //check for opening error
  std::ifstream asmFile;
  asmFile.open(argv[1]);
  if (!asmFile.is_open())
  {
    std::cerr << "Error opening file." << std::endl;
    std::cerr << "Quitting assembler." << std::endl;
    return 1;
  }

  //read/decipher/store source code
  std::string lineIn;
  std::list<Command> cmdList;
  std::list<Label> lblList;
  int index = 0;
  bool inData = 0;
  while (!asmFile.eof())
  {
    std::ws(asmFile);
    std::getline(asmFile,lineIn);
    if(!asmFile.good())
      break;

    //.text section - process .text instructions
    if (!inData)
    {
      //if .data section is entered, break out of .text loop
      if (lineIn == ".data")
      {
        inData = 1;
        continue;
      }
      //ignore .text instruction
      if(lineIn == ".text")
      {
        continue;
      }

      //only labels contain ':'
      //store labels in seperate list
      size_t result = lineIn.find(':');
      if (result != std::string::npos)
      {
        Label newLabel(lineIn,index);
        newLabel.type = "tl";
        lblList.push_back(newLabel);
        //++index; not used, text labels should  have same lineNum as next command
        continue;
      }

      //if not a label...
      //determine type of instruction, push onto command list
      Command newCommand(lineIn,index);  
      newCommand.type = newCommand.GetRIJType();
      cmdList.push_back(newCommand);
      
    } //while(!inData)

    
    //.data section - process .data instructions
    else
    {
     Label newLabel(lineIn,index);
      
      //process .word allocations
      if (newLabel.tokens_vec[1] == ".word")
      {
        for (int i = 2; i < newLabel.tokens_vec.size(); ++i)
        {
          Label wordLabel(lineIn,index);
          wordLabel.type = ".word";
          wordLabel.machine = stoi(newLabel.tokens_vec[i]);
          lblList.push_back(wordLabel);
          ++index;
        }
        continue;

      }

      //process .space allocations
      else
      {
        int numBytes = stoi(newLabel.tokens_vec[2]);
        
        for (int i = 0; i < numBytes; ++i)
        {
          Label spaceLabel(lineIn,index);
          spaceLabel.type = ".space";
          spaceLabel.machine = 0;
          lblList.push_back(spaceLabel);
          ++index;
        }
      }      
    } //else

    //.data process increments index for itself
    if (!inData)
      ++index;

  } //while(!asmFile.eof())

  //resolve labels in cmdList
  ResolveLabels(cmdList, lblList);
  
  //resolve machine codes in cmdList
  std::list<Command>::iterator cmdIter = cmdList.begin();
  for (int i = 0; i < cmdList.size(); ++i)
  {
    cmdIter->ResolveMachine();
    ++cmdIter;
  }

  //output cmdList machine code
  std::cout << std::endl;
  cmdIter = cmdList.begin();
  for (int i = 0; i < cmdList.size(); ++i)
  {
    std::cout << std::setw(8) << std::hex << std::setfill('0') << cmdIter->machine << std::endl;
    ++cmdIter;
  }

  //output lblList machine code
  std::list<Label>::iterator lblIter = lblList.begin();
  for (int i = 0; i < lblList.size(); ++i)
  {
    //exclude text labels
    if(lblIter->type != "tl")
    {
      std::cout << std::setw(8) << std::hex << std::setfill('0') << lblIter->machine << std::endl;
    }
    ++lblIter;
  }

  
  #ifdef DEBUG
  
  //read contents of cmdList for debug
  std::cout << std::endl;
  std::cout << "COMMAND LIST CONTENTS:" << std::endl;
  std::cout << "COMMAND LIST SIZE: " << cmdList.size() << std::endl;
  cmdIter = cmdList.begin();
  for (size_t i = 0; i < cmdList.size(); ++i)
  {
    std::cout << i << ": " << *cmdIter << std::endl;
    ++cmdIter;
  }
  
  //read contents of lblList for debug
  std::cout << "LABEL LIST CONTENTS:" << std::endl;
  std::cout << "LABEL LIST SIZE: " << lblList.size() << std::endl;
  lblIter = lblList.begin();
  for (size_t i = 0; i < lblList.size(); ++i)
  {
    std::cout << i << ": " << *lblIter << std::endl;
    ++lblIter;
  }

  #endif

  return 0;
}

#endif

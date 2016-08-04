/**
 * @file   main.cpp
 * @author Jarrod Brunson
 * @date   071116
 * @brief  Basic cache simulator
 *
 * @description
 * This program simulates the work of cache memory
 *****************************************************/

#ifndef main_CPP
#define main_CPP

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/tokenizer.hpp>
#include <vector>
#include <list>


/******************************************
 *          Class Declarations            *
 *****************************************/
struct Cache;
struct Access;
struct Set;
struct Line;


/************************************
 *          Cache  Class            *
 ***********************************/

struct Cache
{
  int maxLines;                                             //number of lines in each set 
  int maxBytes;                                             //number of B  in  each line
  int cacheSize;                                            //total B in cache
  int setNum;                                               //number of sets in cache
  std::vector<Set> sets;                                    //sets in cache
  
  int indexBits;                                            //number of index bits
  int offsetBits;                                           //number of offset bits
  int tagBits;                                              //number of tag bits

  int hits;                                                 //hit counter
  int misses;                                               //miss counter
  
  Cache(int maxLines, int maxBytes, int cacheSize);                   //default constructor
  void EditCache(int index, int line, int offset, int newTagValue);   //edit cache at location
  void ShowCache();                                         //display cache contents
  void ShowConfiguration();                                 //display cache configuration info
  bool HitOrMiss(Access access);                            //1 = hit, 0 = miss
  void ShowSummary();                                      //display summary data
};


/**********************************
 *          Set  Class            *
 *********************************/

struct Set
{
  int maxLines;                                            //max lines in set
  std::vector<Line> lines;                                 //lines stored in set
  std::list<int> nextLineToEdit;                          //tracks least recently used line
  
  Set(int maxLines, int maxBytes);                         //default constructor,
                                                           //creates max number of lines,
                                                           //each with max number of bytes
  void EditSet(int line, int offset, int newTagValue);     //edit set at location
  bool IsTagInSet(int tag);                                //return offset of tag
                                                           //if in set, -1 if not
  int GetLine(int tag);                                    //find line where tag
                                                           //is stored 
};


/***********************************
 *          Line  Class            *
 **********************************/

struct Line
{
  int maxBytes;                                   //max bytes in line
  std::vector <int> bytes;                        //bytes stored in line,
                                                  //contain tags
                                                  
  Line(int maxBytes);                             //default constructor,
                                                  //creates max number of bytes,
                                                  //with default value -1
  void EditByte(int offset, int newTagValue);     //edit tag at byte offset
  int GetTag(int offset);                         //return tag at byte offset
};


/*************************************
 *          Access  Class            *
 ************************************/

struct Access
{
  int referenceNum;                               //reference number
  std::string accessType;                         //type of memory access
  int size;                                       //size of reference (in Bytes)
  unsigned int address;                           //address of reference, shifts must be unsigned
  int tag;                                        //cache tag
  int index;                                      //cache index
  int offset;                                     //byte offset
  std::string hitOrMiss;                          //memory hit or miss status

  Access(int r, std::string aT, int s, int a);    //default constructor
};


/***************************************************
 *          Access Non-Member Operators            *
 **************************************************/

std::ostream& operator << (std::ostream& os, Access access)
{
  std::cout << std::left << "   "
            << std::setw(5) << access.referenceNum
            << std::right << std::setw(5) << access.accessType
            << std::setw(5) << " "
            << std::hex << std::setfill('0') << std::setw(8) << access.address
            << std::setfill(' ')
            << std::setw(7) << access.tag
            << std::setw(8) << std::dec << access.index
            << std::setw(8) << access.offset
            << std::setw(10) << access.hitOrMiss
            << std::endl;
}


/*********************************************
 *            Function Prototypes            *
 ********************************************/

//create Cache object from configuration file
Cache ReadConfig(std::ifstream& configFile);

//create Access object from string
Access parseAccess(std::string accessString, int referenceNum);

//parse memory trace file
void ReadMemTrace(std::ifstream&, std::vector<Access>& accessVec);

//get tag, index & offset for accesses   
void ResolveAccessBits(std::vector<Access>& accessVec, Cache cache);

//display cache memory access log
void ShowAccesses(std::vector<Access> accessVec);

//determine hit/miss of each access    
void ProcessAccesses(std::vector<Access>& accessVec, Cache& cache);


/******************************
 *            Main            *
 *****************************/

int main(int argc, char* argv[])
{
  //open configuration and memory trace files from command line
  //check for errors
  std::ifstream configFile;
  configFile.open(argv[1]);
  if (!configFile.is_open())
  {
    std::cerr << "Error opening configuation file." << std::endl;
    std::cerr << "Exiting cache simulation." << std::endl;
    return 1;
  }
  
  std::ifstream memFile;
  memFile.open(argv[2]);
  if (!memFile.is_open())
  {
    std::cerr << "Error opening memory trace file." << std::endl;
    std::cerr << "Exiting cache simulation." << std::endl;
    return 1;
  }

  //read configuration data, create cache object  
  Cache newCache = ReadConfig(configFile);  
  
  //read memory access data to access vector
  std::vector<Access> accessVec;
  ReadMemTrace(memFile, accessVec);

  //resolve tag, index and offset bit values for accesses in cache
  ResolveAccessBits(accessVec, newCache);

  //process memory access, detemine if hit or miss
  ProcessAccesses(accessVec,newCache);

  newCache.ShowConfiguration();
  ShowAccesses(accessVec);
  newCache.ShowSummary();
  
  #ifdef DEBUG

  //Shows contents of each set's nextLineToEdit Vector
  //run through sets
  for (int i = 0; i < cache.sets.size(); ++i)
  {
    std::cout << "Set: " << i << std::endl;
    //run through lines
    for (int j = 0; j < cache.sets[i].nextLineToEdit.size(); ++j)
    {  
      std::cout << "\tLine: " << j <<std::endl;;
      //run through bytes
      for (std::list<int>::iterator k = cache.sets[i].nextLineToEdit.begin();
           k != cache.sets[i].nextLineToEdit.end(); ++k)
        std::cout << "\t\t" << *k << " ";
      std::cout << std::endl;
    }
  }
  std::cout << std::endl;
  
  
  #endif

  return 0;
}


/**************************************************
 *            Cache Member Definitions            *
 *************************************************/

Cache::Cache(int maxLines, int maxBytes, int cacheSize) : maxLines(maxLines), maxBytes(maxBytes),
                                                          cacheSize(cacheSize), hits(0), misses(0)
{ 
  //caclulate number of sets
  setNum = cacheSize / (maxLines * maxBytes);
  
  //calculate number of indexBits
  //number of bits needed to select between number of sets
  indexBits = std::log2(float(setNum));
    
  //calculate number of offsetBits
  //number of bits needed to select between number of bytes in a line
  offsetBits = std::log2(float(maxBytes));
    
  //calculate number of tagBits
  //assuming 32b address
  tagBits = 32 - indexBits - offsetBits;

  //create cache structure
  for (int i = 0; i < setNum; ++i)
  {
    Set newSet(maxLines,maxBytes);
    sets.push_back(newSet);
  }  
}

void Cache::EditCache(int index, int line, int offset, int newTagValue)
{
  sets[index].EditSet(line,offset,newTagValue);
  return;
}

void Cache::ShowCache()
{
  std::cout << "sets:" << std::endl;
  for (int i = 0; i < sets.size(); ++i)
  {
    std::cout << i << std::endl;
    std::cout << "\tlines:" << std::endl;
    for (int j = 0; j < sets[i].lines.size(); ++j)
    {
      std::cout << "\t\t" << j << std::endl;
      std::cout << "\t\tbytes:" << std::endl;
      for (int k = 0; k < sets[i].lines[j].bytes.size(); ++k)
      {
        std::cout << "\t\t\t" << k << std::endl;
        std::cout << "\t\t\ttag: " << sets[i].lines[j].bytes[k] << std::endl;
      }
    }
  }
}

void Cache::ShowConfiguration()
{
  std::cout << std::endl;
  std::cout << "Total Cache Size:  " << cacheSize << "B" << std::endl;
  std::cout << "Line Size:  " << maxBytes << "B" << std::endl;
  std::cout << "Set Size:  " << maxLines << std::endl;
  std::cout << "Number of Sets:  " << setNum << std::endl;
  
  return;
}

bool Cache::HitOrMiss(Access access)
{
  return sets[access.index].IsTagInSet(access.tag);
}

void Cache::ShowSummary()
{
  std::cout << std::endl;
  std::cout << "    Simulation Summary" << std::endl;
  std::cout << "**************************" << std::endl;
  std::cout << "Total Hits:\t" << hits << std::endl;
  std::cout << "Total Misses:\t" << misses << std::endl;
  std::cout << "Hit Rate:\t" << std::setprecision(5) << float(hits) / float((hits + misses)) << std::endl;
  std::cout << "Miss Rate:\t" << std::setprecision(5) << float(misses) / float((hits + misses)) << std::endl;
}


/************************************************
 *            Set Member Definitions            *
 ***********************************************/

Set::Set(int maxLines, int maxBytes) : maxLines(maxLines), nextLineToEdit(0)
{
  for (int i = 0; i < maxLines; ++i)
  {
    Line newLine(maxBytes);
    lines.push_back(newLine);
    nextLineToEdit.push_back(i);
  }
}

void Set::EditSet(int line, int offset, int newTagValue)
{
  lines[line].EditByte(offset,newTagValue);
  return;
}

bool Set::IsTagInSet(int tag)
{
  for (int i = 0; i < lines.size(); ++i)
  {
    for (int j = 0; j < lines[i].bytes.size(); ++j)
    {
      if(lines[i].GetTag(j) == tag)
        return true;
    }
  }
  return false;
}

int Set::GetLine(int tag)
{
  for (int i = 0; i < lines.size(); ++i)
  {
    for (int j = 0; j < lines[i].bytes.size(); ++j)
    {
      if (lines[i].GetTag(j) == tag)
        return i;
    }
  }
  return -1;
}

/***************************************************
 *            Line Member Definitions            *
 **************************************************/

Line::Line(int maxBytes) : maxBytes(maxBytes)
{
  for (int i = 0; i < maxBytes; ++i)
  {
    int newTag = -1;
    bytes.push_back(newTag);
  }
}

void Line::EditByte(int offset, int newTagValue)
{
  bytes[offset] = newTagValue;
  return;
}

int Line::GetTag(int offset)
{
  return bytes[offset];
}


/***************************************************
 *            Access Member Definitions            *
 **************************************************/

Access::Access(int r, std::string aT, int s, int a) : referenceNum(r), size(s), address(a), tag(0), index(0), offset(0),
                                                      hitOrMiss("Miss")
{
  if(aT == "R" || aT == "r")
  {
    accessType = "Read";
  }
  else
  {
    accessType = "Write";
  }
}


/**********************************************
 *            Function Definitions            *
 *********************************************/

Access parseAccess(std::string accessString, int referenceNum)
{
  boost::char_separator<char> delimeter(": ");
  boost::tokenizer < boost::char_separator < char > > tokens(accessString,delimeter);
  boost::tokenizer < boost::char_separator < char > >::iterator it = tokens.begin();

  std::stringstream strs;
  
  while (it != tokens.end())
  {
    //get Access values
    std::string accessType = *it;
    ++it;
    std::string sizeStr = *it;
    ++it;
    std::string addressStr = *it;
    ++it;
    
    //convert numerical data to int's
    int size = stoi(sizeStr);
    strs << addressStr << std::hex;
    int address;
    strs >> address;

    //create and return Access
    Access newAccess = Access(referenceNum,accessType,size,address);
    return newAccess;
  }
  
}

Cache ReadConfig(std::ifstream& configFile)
{
  //holds data from input file
  std::string lineIn;
  int maxLines;
  int maxBytes;
  int cacheSize;
  
  //read/store configuration file  
  std::ws(configFile);
  std::getline(configFile, lineIn);
  maxLines = stoi(lineIn);
  std::ws(configFile);
  std::getline(configFile, lineIn);
  maxBytes = stoi(lineIn);
  std::ws(configFile);
  std::getline(configFile, lineIn);
  cacheSize = stoi(lineIn);

  Cache newCache = Cache(maxLines,maxBytes,cacheSize);
  
  return newCache;
}

void ReadMemTrace(std::ifstream& memFile, std::vector<Access>& accessVec)
{
  //holds data from input file
  std::string lineIn;

  //tracks reference number of memory access
  int referenceNum = 0;
  
  //read each line of input file, create Access objects, push onto vector
  while (!memFile.eof())
  {
    std::ws(memFile);
    std::getline(memFile,lineIn);
    if (!memFile.good())
      break;
    
    Access newAccess = parseAccess(lineIn,referenceNum);

    accessVec.push_back(newAccess);
    ++referenceNum;
  }
  
  return;
}

void ResolveAccessBits(std::vector<Access>& accessVec, Cache cache)
{

  //for each access in accessVec
  for (int i = 0; i < accessVec.size(); ++i)
  {
    //assuming 32b address for all calculations

    //tag = value of bits left after shifting off index and offset bits
    accessVec[i].tag = accessVec[i].address >> (32 - cache.tagBits);

    //shift left to clear tag bits, shift right to undo left shift + clear
    //offset bits, leaves index bits 
    unsigned int indexShift = accessVec[i].address << cache.tagBits;
    accessVec[i].index = indexShift >> (cache.tagBits + cache.offsetBits);

    //shift left to clear tag and index bits, shift right to undo left shift,
    //leaves offset bits
    unsigned int offsetShift = accessVec[i].address << (cache.tagBits + cache.indexBits);
    accessVec[i].offset = offsetShift >> (cache.tagBits + cache.indexBits);
    }
  
  return;
}

void ShowAccesses(std::vector<Access> accessVec)
{
  
  std::cout << std::endl;
  std::cout << std::left
            << std::setw(10) << "RefNum"
            << std::setw(8) << "R/W"
            << std::setw(13) << "Address"
            << std::setw(6) << "Tag"
            << std::setw(8) << "Index"
            << std::setw(10) << "Offset"
            << std::setw(8) << "H/M"
            << std::endl;
  std::cout << "***************************************************************" << std::endl;

  for (int i = 0; i < accessVec.size(); ++i)
  {
    std::cout << accessVec[i];
  }
  
  return;
}

void ProcessAccesses(std::vector<Access>& accessVec, Cache& cache)
{
  int maxLines = cache.maxLines;
  
  for(int i = 0; i < accessVec.size(); ++i)
  {
    //determines if Access is already in cache,
    //marks Access with result
    bool hit;
    hit = cache.HitOrMiss(accessVec[i]);
    if(hit)
    {
      ++cache.hits;
      accessVec[i].hitOrMiss = "Hit";
    }
    else
      ++cache.misses;

    //if - direct mapped cache (single line in set); no use for LRU logic
    if (maxLines == 1)
    {
      cache.EditCache(accessVec[i].index,0,accessVec[i].offset,accessVec[i].tag);
      continue;
    }

    //else - associative cache; use least recently used logic to determine which
    //lines to update
    else
    {
      if(hit)
      {
        //get line where tag is found, remove it from list, add it to the end
        int tagFoundAt = cache.sets[accessVec[i].index].GetLine(accessVec[i].tag);
        cache.sets[accessVec[i].index].nextLineToEdit.remove(tagFoundAt);
        cache.sets[accessVec[i].index].nextLineToEdit.push_back(tagFoundAt);
      }

      else
      {
        int front = cache.sets[accessVec[i].index].nextLineToEdit.front();
        cache.EditCache(accessVec[i].index,front,accessVec[i].offset,accessVec[i].tag);
        cache.sets[accessVec[i].index].nextLineToEdit.pop_front();
        cache.sets[accessVec[i].index].nextLineToEdit.push_back(front);
      }
    }
  }
  
  return;
}

#endif

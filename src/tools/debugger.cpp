#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

#include <sys/ptrace.h>
#include <sys/wait.h>

#include "debugger.hpp"
#include "memory.hpp"

using namespace tools;

// TODO: choice between const functions or object debugger.
Debugger::Debugger()
{
  std::cout << "hello debugger" << std::endl;
}

Debugger::~Debugger() {}

void Debugger::attach(pid_t target)
{
  int ret = ptrace(PTRACE_ATTACH, target, nullptr, nullptr);

  if (ret < 0)
    throw "Cannot attach to the process!\n";

  pid = target;
}

void Debugger::detach()
{
  int ret = ptrace(PTRACE_DETACH, pid, nullptr, nullptr);

  if (ret < 0)
    throw "Cannot detach the process!\n";
}

// TODO: add sending signals
void Debugger::stopTheProcess()
{
}

void Debugger::readRegisters()
{
  int ret = ptrace(PTRACE_GETREGS, pid, nullptr, &regs);

  if (ret < 0)
    throw "Cannot update registers\n";
}

void Debugger::setRegisters()
{
  int ret = ptrace(PTRACE_SETREGS, pid, nullptr, &regs);

  if (ret < 0)
    throw "Cannot set registers\n";
}

void Debugger::setRegisters(user_regs_struct &regs_struct)
{
  int ret = ptrace(PTRACE_SETREGS, pid, nullptr, &regs_struct);

  if (ret < 0)
    throw "Cannot set registers\n";
}


// TODO: debug function
void Debugger::printRegisters()
{
  readRegisters();
  std::cout<<std::hex;
  std::cout<<"RAX: 0x"<< regs.rax <<std::endl;
  std::cout<<"RBX: 0x"<< regs.rbx <<std::endl;
  std::cout<<"RCX: 0x"<< regs.rcx <<std::endl;
  std::cout<<"RDX: 0x"<< regs.rdx <<std::endl;
  std::cout<<"RBP: 0x"<< regs.rbp <<std::endl;
  std::cout<<"RSP: 0x"<< regs.rsp <<std::endl;
  std::cout<<"RIP: 0x"<< regs.rip <<std::endl;
  std::cout<<std::endl;
}

void Debugger::writeWord(uint64_t addr, uint64_t data)
{
  uint64_t ret = ptrace(PTRACE_POKEDATA, pid, addr, data);

  if (ret > 0)
    throw "Cannot write data to memory!\n";

}

uint64_t Debugger::readWord(uint64_t addr)
{
  uint64_t ret = ptrace(PTRACE_PEEKDATA, pid, addr, nullptr);

  // if (ret > 0)
    // throw "Cannot read data from memory!\n";

  // std::cout<<std::hex<<"0x"<<ret<<std::endl;
  return ret;
}

// TODO: remove debug printing and return byte array
void Debugger::read(uint64_t addr, uint8_t num)
{
  uint64_t value;
  for(auto i = 0; i < num; ++i)
  {
    value = readWord(addr);
    printHexCString(value);
    addr += 0x8;
  }
}

uint64_t Debugger::swapEndian(uint64_t value)
{
  uint64_to_arr_byte src, dst;
  
  src.u_int64 = value;

  for(auto i = 0; i < 8; i++)
  {
    dst.arr[i] = src.arr[8 - i - 1];
  }

  return dst.u_int64;
}

void Debugger::printHexCString(uint64_t value)
{
  uint64_to_arr_byte data;
  data.u_int64 = value;

  std::cout<<std::hex;
  for(auto i = 0; i < 8; ++i)
    std::cout<<"\\x"<<(data.arr[i] & 0xff); 
}


// TODO: doesen't work need more test
void Debugger::continueExecution()
{
  int ret = ptrace(PTRACE_CONT, pid, nullptr, nullptr);

  if (ret < 0)
    throw "Cannot resume process!\n";
}

void Debugger::continueExecutionUntilHitBreakpoint()
{
  while(1)
  {
    singleStep();
    wait();
    readRegisters();
    if (isSoftBreakpointExistInMap(regs.rip))
    {
      std::cout<<"Breakpoint hit at address: 0x"<<std::hex<<regs.rip<<std::endl;
      break;
    }
  }
}

void Debugger::singleStep()
{
  int ret = ptrace(PTRACE_SINGLESTEP, pid, nullptr, nullptr);

  if (ret < 0)
    throw "Cannot continue execution!\n";
}

// TODO: Implement
void Debugger::singleSourceCodeStep()
{

}

// TODO: more test more behavior
void Debugger::syscallStep()
{
  int ret = ptrace(PTRACE_SYSCALL, pid, nullptr, nullptr);

  if (ret < 0)
    throw "cannot syscall breakpoints\n";
}

void Debugger::wait()
{
  waitpid(pid, nullptr, 0);
}

// TODO: Maybe throw to another file
void Debugger::loadMemoryMap()
{ 
  mem_mapping.reserve(64);
  std::string buffer;
  std::string word;
  std::string addr;

  MemoryObject tmp;

  std::string path("/proc/");
  path += std::to_string(pid);
  path += "/maps";
  std::ifstream processMap(path);

  if (processMap.is_open())
  {
    
    // parse line
    while(std::getline(processMap, buffer))
    {
      auto i = 0;
      std::stringstream sstream(buffer);
      while(sstream>>word)
      {
        switch(i)
        {
          case 0: {
            std::istringstream inpstream(word);
            std::getline(inpstream, addr, '-');
            tmp.lower_address = std::stoull(addr, nullptr, 16);
            std::getline(inpstream, addr, '-');
            tmp.upper_address = std::stoull(addr, nullptr, 16);
            break;
          }
          case 1:
            tmp.permission = std::move(word);
            break;
          case 5:
            tmp.file_mapped_name = std::move(word);
            break;
        }
        i++;
      }
      i=0;
      mem_mapping.push_back(std::move(tmp));
    }
  } 
  else
  {
    throw "Cannot open memory map file!\n";
  }
  // for(auto &el : mem_mapping)
    // el.print();
}


void Debugger::moduleLocation()
{
  uint64_t rip = regs.rip;
  auto itr = std::find_if(mem_mapping.begin(), mem_mapping.end(), [rip](MemoryObject &el){
    return (el.lower_address <= rip) && (el.upper_address >= rip);
  });

  if (itr != mem_mapping.end())
  {
    itr->print();
  }
}

// TODO: Change name
void Debugger::breakpointSoftware(uint64_t addr)
{
  addBreakpoint(addr);
}

// TODO: is prabobly better to readRegister in other place in middleware
void Debugger::breakpointHit()
{
  readRegisters();
  if(!isSoftBreakpointExistInMap(regs.rip))
    return;
  
  removeBreakpoint(regs.rip);
}

void Debugger::addBreakpoint(uint64_t addr)
{
  if (isSoftBreakpointExistInMap(addr))
    return;

  uint8_t opcode = '\xc3';
  uint64_t instr = readWord(addr);
  if (instr == 0)
    return;

  breakpoint_list[addr] = instr;
  instr &= ~0xFF;
  instr |= opcode;
  writeWord(addr, instr);
}

void Debugger::removeBreakpoint(uint64_t addr)
{
  if (!isSoftBreakpointExistInMap(addr))
    return;

  uint64_t bp = breakpoint_list.at(addr);
  writeWord(addr, bp);
  breakpoint_list.erase(addr);
}

bool Debugger::isSoftBreakpointExistInMap(uint64_t addr)
{
  return breakpoint_list.find(addr) != breakpoint_list.end();
}
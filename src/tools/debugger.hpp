#include <sys/user.h>
#ifndef __TOOLS_DEBUGGER_HPP
#define __TOOLS_DEBUGGER_HPP
#include <vector>
#include "memory.hpp"

namespace tools
{
  class Debugger
  {
  private:
    std::vector<MemoryObject> mem_mapping;

  public:
    user_regs_struct regs;
    pid_t pid;

    Debugger();
    ~Debugger();
    Debugger(Debugger &) = delete;
    Debugger(Debugger &&) = delete;
    Debugger &operator=(Debugger &) = delete;
    Debugger &operator=(Debugger &&) = delete;

    /**
     * @brief Attach to process using pid
     *
     * @param[in] target process id
     */
    void attach(pid_t);
    void detach();
    void stopTheProcess();

    void readRegisters();
    void setRegisters();
    void setRegisters(user_regs_struct &);
    void printRegisters();
    void writeWord(uint64_t, uint64_t);
    uint64_t readWord(uint64_t);

    /**
     * @brief Read number of words from address
     * 
     * param addr count
     * 
     */
    void read(uint64_t, uint8_t);
    void write(uint64_t);

    void printHex(uint64_t);

    uint64_t swapEndian(uint64_t value);

    void continueExecution();
    void singleStep();
    void syscallStep();
    void wait();

    /**
     * @brief Return actually location in memory map based on RIP
     * 
     */
    void loadMemoryMap();
    void moduleLocation();
    void functionLocation();
    // MemoryObject parseLine(std::ifstream &);

    void breakpointSoftware(uint64_t addr);
  };
}
#endif
#include <sys/user.h>
#ifndef __TOOLS_DEBUGGER_HPP
#define __TOOLS_DEBUGGER_HPP
#include <vector>
#include <map>
#include "memory.hpp"

namespace tools
{

  union uint64_to_arr_byte
  {
    uint64_t u_int64;
    unsigned char arr[sizeof(u_int64)];
  };

  // TODO: add debugger builder/factory
  
  // TODO: sort method by category and add docs comments
  class Debugger
  {
  private:
    std::vector<MemoryObject> mem_mapping;
    std::map<uint64_t, u_int64_t> breakpoint_list;

  public:
    user_regs_struct regs;
    pid_t pid;
    int status;

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
    void setOptionsTraceExec();

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
    std::vector<uint64_t> read(uint64_t, uint8_t);
    void write(uint64_t, std::vector<uint64_t> &);

    void printHexCString(uint64_t);

    uint64_t swapEndian(uint64_t value);

    void continueExecutionUntilHitBreakpoint();
    void continueExecution();
    void singleStep();
    void singleSourceCodeStep();
    void syscallStep();
    int wait();

    /**
     * @brief Return actually location in memory map based on RIP
     * 
     */
    void loadMemoryMap();
    void moduleLocation();
    void functionLocation();
    void printMemoryMaps();
    // MemoryObject parseLine(std::ifstream &);

    void breakpointSoftware(uint64_t addr);
    void addBreakpoint(uint64_t addr);
    void removeBreakpoint(uint64_t addr);
    bool isSoftBreakpointExistInMap(uint64_t addr);
    void breakpointHit();

    void executeOpcode(std::vector<char> &code);
    std::vector<uint64_t> convert8To64ByteVector(std::vector<char> &code);
  };
}
#endif
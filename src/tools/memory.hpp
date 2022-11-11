#ifndef _TOOLS_MEMORY_HPP
#define _TOOLS_MEMORY_HPP

namespace tools
{
  class MemoryObject
  {
    public:
    uint64_t lower_address;
    // std::string address;
    uint64_t upper_address;
    std::string permission;
    std::string file_mapped_name;

    MemoryObject() {};
    ~MemoryObject() {};

    void print() 
    {
      std::cout<<std::hex<<lower_address<<":"<<upper_address<<" "<<permission<<" "<<file_mapped_name<<std::endl;
    }
    // MemoryObject parseLine(std::string_view)
    private:
  };
}

#endif
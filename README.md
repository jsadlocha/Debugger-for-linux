# Debugger-for-linux
Debugger and code injector for linux system


building in progress...

Probably project will be converted into the hacking framework, as universal C++ high level module.

# Dependencies
C++17, CMake, GoogleTest

External library used:
  - Elfio - elf parser

# Features:
  - Running binary ❌
  - Attach to the process by pid ☑️
  - Reading/Writing to the registers ☑️
  - Reading/Writing to arbitrary memory ☑️
  - Software breakpoints ☑️
  - Hardware breakpoints ❌
  - Syscall Entry/Exit info ❌
  - Elf Parser ❌
  - Disassembler ❌
  - Running assembly snippets ☑️
  - Running arbitrary binary ❌
  - Dll injection ❌
  - Running python scripts ❌
  - Memory module map ☑️
  - Function locations and reference scrapping ❌
  - Memory scanning ❌
  - Source code mapping with dwarf symbols ❌
  - Remote debugging ❌
  - Reference name demangling C++ ☑️
  - Mapping arbitrary memory objects to python binds ❌
  - JIT compiling arbitrary code ❌
  - Graph execution modeling ❌
  - SMT solving execution branches ❌
  - Reverse execution/backtracking ❌
  - Simple GUI ❌

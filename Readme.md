# CadDB

## Compiling boost

### MinGW-W64

Tested with MinGW-W64 with gcc 6.2

add mingw-w64 to path: 
`export PATH="/c/Program Files (x86)/mingw-w64/i686-6.2.0-posix-dwarf-rt_v5-rev0/mingw32/bin:$PATH"`
`bootstrap.sh mingw`
`b2 -j8 toolset=gcc`

### Visual Studio 2013

start a "VS2013 x86 Native Tools Command Prompt"
in boost dir:
`boostrap.bat`
`b2 -j8 toolset=msvc`

### Clang
Clang 3.8.1
Based on boost 1.61: 
Change boost/typeof/typeof.hpp:52 from
    #elif defined(__GNUC__)
to
    #elif defined(__GNUC__) || defined(__clang__)
(see [bug report](https://svn.boost.org/trac/boost/ticket/11821))

Ensure that `CMAKE_CXX_COMPILER` and `CMAKE_C_COMPILER` are set 
to the `clang-cl.exe` executable (which emulates the msvc compiler), 
e.g. in `C:/Program Files (x86)/LLVM/bin/clang-cl.exe`
Since clang-cl uses the MSVC ABI, we can link to the boost libs generated
by MSVC. 
Also ensure that `CMAKE_CXX_FLAGS_RELEASE` does not contain `/O2 /Ob2` 
optimization settings, but e.g. `/MD /O1 /Ob1 /D NDEBUG`, in order not 
to run into a CLang bug. 




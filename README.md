# OS_Dynamic-memory-management
Soongsil University. Operating System 2020-2 Assignment #5 : Dynamic Memory Management

 implementation of _alloc_ and _dealloc_ functions.
 
**mmap** obtains pages of memory from the OS, and allocate chunks from these pages dynamically when requested.

**init alloc()** initialize memory manager.

**cleanup()** clean up any state of memory manager.

**alloc(int)** take an integer buffer size that must be allocated, and return a char * pointer to the buffer on a success.

**dealloc(char * )** take a pointer, and frees up the entire memory.

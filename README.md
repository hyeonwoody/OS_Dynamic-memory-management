# OS_Dynamic-memory-management  
## 숭실대학교 운영체제 2020 가을학기 - 과제5 : 동적 메모리 관리  
  
본 과제의 목표는 프로그램에서 메모리를 동적으로 할당하는 사용자 지정 메모리 관리자를 구현하는 것입니다.   

malloc(3) 및 free(3)와 같은 함수를 대체하여 메모리를 할당하고 해제하는 함수 구현을 통해 메모리 관리 기법을 이해할 수 있습니다.   

Mmap(2)과 munmap(2)등의 시스템 호출 함수를 사용하여 메모리 페이지를 가져오고  

메모리 요청 시 메모리 페이지들의 chunk를 동적으로 할당합니다.    
 
<br>
  
## Soongsil University. Operating System 2020-2 Assignment #5 : Dynamic Memory Management  
  
implementation of _alloc_ and _dealloc_ functions.  
  
**mmap** obtains pages of memory from the OS, and allocate chunks from these pages dynamically when requested.

**init alloc()** initialize memory manager.

**cleanup()** clean up any state of memory manager.

**alloc(int)** take an integer buffer size that must be allocated, and return a char * pointer to the buffer on a success.

**dealloc(char * )** take a pointer, and frees up the entire memory.

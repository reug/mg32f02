#include <stdint.h>

#define APP_ORIGIN      0x20000000
#define APP_SIGNATURE   0x46c046c0

#define SVC1(no,arg0)                 asm("mov r0,%0\n" "svc %1\n" :: "r"(arg0), "I"(no) : "r0")
#define SVC2(no,arg0,arg1)            asm("mov r0,%0\n" "mov r1,%1\n" "svc %2\n" :: "r"(arg0), "r"(arg1), "I"(no) : "r0","r1")
#define SVC3(no,arg0,arg1,arg2)       asm("mov r0,%0\n" "mov r1,%1\n" "mov r2,%2\n" "svc %3\n" :: "r"(arg0), "r"(arg1), "r"(arg2), "I"(no) : "r0","r1","r2")
#define SVC4(no,arg0,arg1,arg2,arg3)  asm("mov r0,%0\n" "mov r1,%1\n" "mov r2,%2\n" "mov r3,%3\n" "svc %4\n" :: "r"(arg0), "r"(arg1), "r"(arg2), "r"(arg3), "I"(no) : "r0","r1","r2","r3")


/// syscalls
enum IOSystemCalls {
  SVC_HANDLER_UNSET =                  0,
  SVC_HANDLER_SET   =                  1
};


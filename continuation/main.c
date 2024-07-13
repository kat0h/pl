#include <stdio.h>

void func2() {
  puts("func2");
}

void func() {
  asm volatile (
      "lea -8(%%rsp), %%rsp;"
      "lea 16(%%rip), %%rax;"
      "mov %%rax, (%%rsp);"
      "jmp func2"
      :
      :
      : "%rax"
      );
  puts("UNKO");
}

int main(int argc, char **argv) {
  puts("main");
  func();
  puts("main_2");
}

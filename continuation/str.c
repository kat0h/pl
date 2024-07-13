#include <stdio.h>

typedef struct {
  int x;
  int y;
} a[1];

void func(a A) {
  A->x = 1;
  A->y = 2;
}

int main() {
  a A;
  func(A);
  printf("%d %d\n", A->x, A->y);
}

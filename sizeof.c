#include <stdio.h>
#include <limits.h>

int main() {
  long max_sec;

  //printf("double: %lu Bytes\n", sizeof(double));
  printf("short: %lu Bytes\n", sizeof(short));
  //printf("int: %lu Bytes\n", sizeof(int));
  printf("long: %lu Bytes\n", sizeof(long));
  printf("long max: %ld\n\n", LONG_MAX);

  printf("long long: %lu Bytes\n", sizeof(long long));
  printf("long long max: %lld\n", LLONG_MAX);

  //seekできる最大の秒数
  max_sec = LONG_MAX / (44100 * 2);
  printf("max: %ld[sec]\n", max_sec);

  return 0;
}

int syscall(int *a, int len) {
  a[0] = 114514;
  a[1] = 1919;
  a[2] = 13;
  a[3] = 998244353;
  a[4] = 1000000007;
  a[5] = 810;
  a[6] = 2341;
  a[7] = 17;
  int max = -2147483647 - 1;
  for (int i = 0; i < len; ++i) {
    if (a[i] > max) {
      max = a[i];
    }
  }
  return max;
}

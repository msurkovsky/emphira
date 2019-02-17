
double foo();

double bar();

double baz(double x) {
  // double res;
  if (x != 0.000000) {
    return foo();
  } else {
    return bar();
  }
}

double foo() {
  while (true) {
    if (0) {
      return 2.0;
    }
  }
  return -1.0;
}

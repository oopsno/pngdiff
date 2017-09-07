#include "pngdiff.hpp"

int main(int argc, char *argv[]) {
  auto lhs = pngdiff::read_png_file(argv[argc - 2]);
  auto rhs = pngdiff::read_png_file(argv[argc - 1]);
  lhs.diff(rhs);
  return 0;
}
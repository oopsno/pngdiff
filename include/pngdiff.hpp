#pragma once

#include <cstdint>
#include <png.h>
#include <stdexcept>

namespace pngdiff {

struct LibPNGError: std::runtime_error {
  explicit LibPNGError(const std::string& what) : std::runtime_error(what) {}
};

struct IHDR {
  #define IHDR_U32_FIELDS width, height
  #define IHDR_I32_FIELDS bit_depth, color_type, interlace_method, compression_method, filter_method
  uint32_t IHDR_U32_FIELDS;
  int32_t IHDR_I32_FIELDS;

  bool operator==(const IHDR& other) const;
  void diff(const IHDR& other) const;
  void show() const;
};

struct PLTE {
  png_colorp colors;
  int32_t num = 0;

  bool operator==(const PLTE& other) const;
  void diff(const PLTE& other) const;
  void show() const;
};

struct PNG {
  IHDR ihdr;
  PLTE plte;
  png_bytepp pixels = nullptr;

  ~PNG();

  struct {
    png_structp ptr;
    png_infop info;
    png_infop info_end;
    size_t rowbytes;
  } internel;

  bool operator==(const PNG& other) const;
  void diff(const PNG& other) const;
  void show() const;
};

PNG read_png_file(const char *) throw(std::runtime_error, LibPNGError);

}
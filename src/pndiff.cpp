#include <iostream>

#include "mmpl.hpp"
#include "pngdiff.hpp"

namespace pngdiff {

#define _JOIN_CONJ_(x, y) ((x) and ((y) == other.y))
#define FIELDS_EQUAL(...) FOLDL(_JOIN_CONJ_, true, __VA_ARGS__)
#define _DIFF_(field) do { \
    if ((field) != other.field) { \
      std::cout << typeid(*this).name() << "::" #field ":\t\"" \
                << (field) << "\"\tvs\t\"" << other.field << "\"" << std::endl; \
    } \
  } while(false);
#define FIELDS_DIFF(...) APPLY(_DIFF_, __VA_ARGS__)
#define _SHOW_(field) do { \
    std::cout << "  " << #field " = " << (field) << ";" << std::endl; \
  } while(false);
#define FIELDS_SHOW(st, ...) \
  std::cout << st << " {" << std::endl; \
  APPLY(_SHOW_, __VA_ARGS__); \
  std::cout << "}" << std::endl;

bool IHDR::operator==(const IHDR& other) const {
  return FIELDS_EQUAL(IHDR_U32_FIELDS, IHDR_I32_FIELDS);
}

void IHDR::diff(const IHDR& other) const {
  if (not this->operator==(other)) {
    FIELDS_DIFF(IHDR_U32_FIELDS, IHDR_I32_FIELDS);
  }
};

void IHDR::show() const {
  FIELDS_SHOW("IHDR", IHDR_U32_FIELDS, IHDR_I32_FIELDS);
}

static bool is_same_color(const png_color& lhs, const png_color& rhs) {
  return lhs.blue == rhs.blue and lhs.green == rhs.green and lhs.red == rhs.red;
}

static std::string to_string(const png_color& color) {
  static char buffer[2048];
  std::snprintf(buffer, 2048, "#%02X%02X%02X", color.red, color.blue, color.green);
  return buffer;
}

bool PLTE::operator==(const PLTE& other) const {
  if (num != other.num) {
    return false;
  }
  for (auto i = 0; i < num; ++i) {
    if (not is_same_color(colors[i], other.colors[i])) {
      return false;
    }
  }
  return true;
}

void PLTE::diff(const PLTE& other) const {
  if (not this->operator==(other)) {
    for (auto i = 0; i < std::min(num, other.num); ++i) {
      if (not is_same_color(colors[i], other.colors[i])) {
        std::cout << "  PLTE::colors[" << i << "]:\t"
                  << to_string(colors[i]) << " vs " << to_string(other.colors[i]) << std::endl;
      }
    }
  }
}

void PLTE::show() const {
  std::cout << "PLTE {" << std::endl
            << "  " << "num = " << num << ";" << std::endl
            << "  " << "colors = {" << std::endl;
  for (auto i = 0; i < num; ++i) {
    std::cout << "    /** " << i << ": " << to_string(colors[i]) << " */," << std::endl;
  }
  std::cout << std::dec;
  std::cout << "  }" << std::endl << "}" << std::endl;
}

bool PNG::operator==(const PNG& other) const {
  return ihdr == other.ihdr and plte == other.plte;
}

void PNG::diff(const PNG& other) const {
  ihdr.diff(other.ihdr);
  plte.diff(other.plte);
  if (ihdr == other.ihdr and plte == other.plte) {
    for (auto y = 0; y < ihdr.height; ++y) {
      for (auto x = 0; x < ihdr.width; ++x) {
        const auto lhs = pixels[y][x];
        const auto rhs = other.pixels[y][x];
        if (lhs != rhs) {
          std::cout << "(" << x << ", " << y << "):\t" << lhs << " vs " << rhs << std::endl;
        }
      }
    }
  }
}

void PNG::show() const {
  ihdr.show();
  plte.show();
}

void abort(const char *s) {
  std::cerr << s << std::endl;
  std::abort();
}

void abort(const char *s, const char *p) {
  std::cerr << s << " ";
  abort(p);
}

PNG read_png_file(const char *file_name) {
  /* open file and test for it being a png */
  PNG png;
  png_byte header[8];  // 8 is the maximum size that can be checked
  auto *fp = fopen(file_name, "rb");
  if (fp == nullptr) {
    abort("[read_png_file] File %s could not be opened for reading", file_name);
  }
  fread(header, 1, 8, fp);
  if (png_sig_cmp(header, 0, 8) != 0) {
    abort("[read_png_file] File %s is not recognized as a PNG file", file_name);
  }

  /* initialize stuff */
  png.internel.ptr = png_create_read_struct("1.6.32", nullptr, nullptr, nullptr);
  if (png.internel.ptr == nullptr) {
    abort("[read_png_file] png_create_read_struct failed");
  }

  png.internel.info = png_create_info_struct(png.internel.ptr);
  if (png.internel.info == nullptr) {
    abort("[read_png_file] png_create_info_struct failed");
  }

  if (setjmp(png_jmpbuf(png.internel.ptr)) != 0) {
    abort("[read_png_file] Error during init_io");
  }

  png_init_io(png.internel.ptr, fp);
  png_set_sig_bytes(png.internel.ptr, 8);
  png_read_info(png.internel.ptr, png.internel.info);
  png_read_update_info(png.internel.ptr, png.internel.info);

  /* read file */
  if (setjmp(png_jmpbuf(png.internel.ptr)) != 0) {
    abort("[read_png_file] Error during read_image");
  }

  const auto height = png_get_image_height(png.internel.ptr, png.internel.info);
  png.internel.rowbytes = png_get_rowbytes(png.internel.ptr, png.internel.info);
  png.pixels = new png_bytep[height];
  for (auto y = 0; y < height; y++) {
    png.pixels[y] = new png_byte[png.internel.rowbytes];
  }

  png_read_image(png.internel.ptr, png.pixels);

  png.internel.info_end = png_create_info_struct(png.internel.ptr);
  if (png.internel.info_end == nullptr) {
    abort("[read_png_file] png_create_info_struct failed");
  }
  png_read_end(png.internel.ptr, png.internel.info_end);

  png.ihdr.width = png_get_image_width(png.internel.ptr, png.internel.info);
  png.ihdr.height = png_get_image_height(png.internel.ptr, png.internel.info);
  png.ihdr.bit_depth = png_get_bit_depth(png.internel.ptr, png.internel.info);
  png.ihdr.color_type = png_get_color_type(png.internel.ptr, png.internel.info);
  png.ihdr.filter_method = png_get_filter_type(png.internel.ptr, png.internel.info);
  png.ihdr.interlace_method = png_get_interlace_type(png.internel.ptr, png.internel.info);
  png.ihdr.compression_method = png_get_compression_type(png.internel.ptr, png.internel.info);

  if (png.ihdr.color_type == PNG_COLOR_TYPE_PALETTE) {
    auto success = png_get_PLTE(png.internel.ptr, png.internel.info, &png.plte.colors, &png.plte.num);
    if (success == 0) {
      abort("png_get_PLTE", file_name);
    }
  }

  fclose(fp);
  return png;
}

PNG::~PNG() {
  png_destroy_read_struct(&internel.ptr, &internel.info, &internel.info_end);
  if (pixels != nullptr) {
    for (auto i = 0; i < ihdr.height; ++i) {
      delete[](pixels[i]);
    }
    delete[](pixels);
  }
}

}

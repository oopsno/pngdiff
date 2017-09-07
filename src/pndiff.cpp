#include <iostream>
#include <sstream>
#include <iomanip>

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
  std::stringstream ss;
  ss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2);
  ss << int(color.red) << int(color.green) << int(color.blue);
  return ss.str();
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

namespace png {

template<typename Function>
struct function_wrapper;

template<typename T>
struct handler {
  template<typename Fn, typename ...Args>
  static std::enable_if_t<std::is_integral<T>::value, T>
  call(Fn fn, const std::string& name, Args ...args) {
    auto rtv = fn(args...);
    if (rtv != 0) {
      throw LibPNGError(name + " returned nullptr");
    }
    return rtv;
  }
};

template<>
struct handler<void> {
  static inline void set_long_jump(const std::string &name, png_structp png_ptr) {
    if (setjmp(png_jmpbuf(png_ptr)) != 0) {
      throw LibPNGError(name + " triggered long jump");
    }
  }
  template <typename T>
  static inline std::enable_if_t<not std::is_same<T, png_structp>::value>
  set_long_jump(const std::string &name, png_structp png_ptr) { }

  template<typename Fn, typename FirstArg, typename ...Args>
  static auto call(Fn fn, const std::string& name, FirstArg firstArg, Args ...args) {
    set_long_jump(name, firstArg);
    return fn(firstArg, args...);
  }
};

template<typename T>
struct handler<T *> {
  template<typename Fn, typename ...Args>
  static auto call(Fn fn, const std::string& name, Args ...args) {
    auto rtv = fn(args...);
    if (rtv == nullptr) {
      throw LibPNGError(name + " returned nullptr");
    }
    return rtv;
  }
};

template<typename ReturnType, typename ...Args>
struct function_wrapper<ReturnType(Args...)> {
  using function_t = std::function<ReturnType(Args...)>;
  function_t fn;
  const std::string name;

  explicit function_wrapper(std::string name, function_t fn) : name{std::move(name)}, fn{fn} {}

  inline ReturnType operator()(Args ...args) const {
    return handler<ReturnType>::call(fn, name, args...);
  }
};

#define WRAP(fn) static auto fn = function_wrapper<decltype(png_##fn)>("png_"#fn, png_##fn)

WRAP(sig_cmp);
WRAP(create_read_struct);
WRAP(create_info_struct);
WRAP(init_io);
WRAP(set_sig_bytes);
WRAP(read_image);
WRAP(read_info);
WRAP(read_update_info);
WRAP(read_end);
WRAP(get_PLTE);

#undef WRAP

} // namespace png

PNG read_png_file(const char *file_name) throw(std::runtime_error, LibPNGError) {
  /* open file and test for it being a png */
  PNG png;
  png_byte header[8];  // 8 is the maximum size that can be checked
  auto *fp = fopen(file_name, "rb");
  if (fp == nullptr) {
    throw std::runtime_error(std::string("File could not be opened for reading: ") + file_name);
  }
  fread(header, 1, 8, fp);
  png::sig_cmp(header, 0, 8);

  /* initialize stuff */
  png.internel.ptr = png::create_read_struct("1.6.32", nullptr, nullptr, nullptr);
  png.internel.info = png::create_info_struct(png.internel.ptr);
  png.internel.info_end = png::create_info_struct(png.internel.ptr);

  png::init_io(png.internel.ptr, fp);
  png::set_sig_bytes(png.internel.ptr, 8);
  png::read_info(png.internel.ptr, png.internel.info);
  png::read_update_info(png.internel.ptr, png.internel.info);

  /* read file */
  const auto height = png_get_image_height(png.internel.ptr, png.internel.info);
  png.internel.rowbytes = png_get_rowbytes(png.internel.ptr, png.internel.info);
  png.pixels = new png_bytep[height];
  for (auto y = 0; y < height; y++) {
    png.pixels[y] = new png_byte[png.internel.rowbytes];
  }

  png::read_image(png.internel.ptr, png.pixels);
  png::read_end(png.internel.ptr, png.internel.info_end);

  png.ihdr.width = png_get_image_width(png.internel.ptr, png.internel.info);
  png.ihdr.height = png_get_image_height(png.internel.ptr, png.internel.info);
  png.ihdr.bit_depth = png_get_bit_depth(png.internel.ptr, png.internel.info);
  png.ihdr.color_type = png_get_color_type(png.internel.ptr, png.internel.info);
  png.ihdr.filter_method = png_get_filter_type(png.internel.ptr, png.internel.info);
  png.ihdr.interlace_method = png_get_interlace_type(png.internel.ptr, png.internel.info);
  png.ihdr.compression_method = png_get_compression_type(png.internel.ptr, png.internel.info);

  if (png.ihdr.color_type == PNG_COLOR_TYPE_PALETTE) {
    png::get_PLTE(png.internel.ptr, png.internel.info, &png.plte.colors, &png.plte.num);
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

} // namespace pngdiff

#include <cstddef>
#include <cstdint>
#include <vector>

#if defined(ESP32) || defined(ESP8266)
#include <FS.h>
// #elif defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_RP2040) ||
// defined(ARDUINO_ARCH_SAMD) #include <SD.h>
#else
#include <stdio.h>
#define TINY_SJIS2UTF8_USE_MOCK_FILE_CLASS
#endif

namespace tinysjis2utf8 {

#ifdef TINY_SJIS2UTF8_USE_MOCK_FILE_CLASS
class MockFileRead {
public:
  MockFileRead(const char *filename) {
    f = fopen(filename, "rb");
    if (fseek(f, 0, SEEK_END) == 0) {
      _size = ftell(f);
      fseek(f, 0, SEEK_SET);
    }
  }
  ~MockFileRead() {
    if (f != nullptr) {
      close();
    }
  }
  int available() { return size() - position(); }
  void close() {
    fclose(f);
    f = nullptr;
  }
  int peek() {
    const auto pos = ftell(f);
    uint8_t buffer[1] = {0};
    const size_t size = fread(buffer, 1, 1, f);
    fseek(f, pos, SEEK_SET);
    return (size >= 1) ? -1 : buffer[0];
  }
  size_t position() const { return ftell(f); }
  bool seek(uint32_t pos) { return fseek(f, pos, SEEK_SET) == 0; }
  size_t size() const { return _size; }
  size_t read(uint8_t *buffer, size_t size) {
    return fread(buffer, 1, size, f);
  }

protected:
  FILE *f = nullptr;
  size_t _size = -1;
};
typedef MockFileRead File;
#else
typedef ::File File;
#endif

std::vector<uint8_t> sjis2utf8(File *tbl_file, const char *sjis_cstr,
                               size_t max_sjis_len);

} // namespace tinysjis2utf8

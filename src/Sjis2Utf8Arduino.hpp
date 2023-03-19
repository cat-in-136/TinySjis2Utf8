#include <cstddef>
#include <cstdint>
#include <vector>

namespace sjis2utf8arduino {

std::vector<uint8_t> sjis2utf8(const char *sjis_cstr, size_t max_sjis_len);

#if 0
size_t sjis2utf8_(const char *sjis, size_t sjis_len, char *utf8,
                  size_t max_utf8_len);
#endif

} // namespace sjis2utf8arduino

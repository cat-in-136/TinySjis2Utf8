#include <cstddef>
#include <cstdint>
#include <vector>

namespace tinysjis2utf8 {

std::vector<uint8_t> sjis2utf8(const char *sjis_cstr, size_t max_sjis_len);

} // namespace tinysjis2utf8

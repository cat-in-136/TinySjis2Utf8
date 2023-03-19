#include "Sjis2Utf8Arduino.hpp"
#include <cstring>

static size_t append_to_char_from_unicode(const uint32_t unic,
                                          std::vector<uint8_t> &out) {
  if (unic < 128) {
    out.push_back(unic);
    return 1;
  } else if (unic < 2048) {
    out.push_back(0xC0 | (unic >> 6));
    out.push_back(0x80 | (unic & 0x3F));
    return 2;
  } else if (unic < 65536) {
    out.push_back(0xE0 | (unic >> 12));
    out.push_back(0x80 | ((unic >> 6) & 0x3F));
    out.push_back(0x80 | (unic & 0x3F));
    return 3;
  } else if (unic < 0x110000) {
    out.push_back(0xF0 | (unic >> 18));
    out.push_back(0x80 | ((unic >> 12) & 0x3F));
    out.push_back(0x80 | ((unic >> 6) & 0x3F));
    out.push_back(0x80 | (unic & 0x3F));
    return 4;
  } else {
    return 0;
  }
}

static const struct {
  uint16_t sjis_start;
  uint16_t sjis_end;
  uint32_t unicode_start;
} SJIS2UNICODE_SEQUENTIAL_CONV_TABLE[] = {
    {0x829F, 0x82F1, 0x3041}, // ひらがな (ぁ...ん)
    {0x8340, 0x837E, 0x30A1}, // カタカナ (ァ...ミ)
    {0x8380, 0x8396, 0x30E0}  // カタカナ (ム...ヶ)
};

std::vector<uint8_t> sjis2utf8arduino::sjis2utf8(const char *sjis_cstr,
                                                 size_t max_sjis_len) {
  const size_t sjis_len = strnlen(sjis_cstr, max_sjis_len);
  const uint8_t *sjis = reinterpret_cast<const uint8_t *>(sjis_cstr);

  std::vector<uint8_t> utf8;
  utf8.reserve(sjis_len * 3 + 1);

  for (size_t sjis_p = 0; sjis_p < sjis_len; sjis_p++) {
    const uint8_t c = sjis[sjis_p];

    if (c == 0) {
      break;
    } else if ((c & 0xF0) <= 0x70) {
      utf8.push_back(c);
    } else if (((0xF0 & c) == 0xA0) || ((0xF0 & c) == 0xB0) ||
               ((0xF0 & c) == 0xC0) || ((0xF0 & c) == 0xD0)) {
      utf8.push_back('_'); // TODO 半角カナと半角記号
    } else {
      if (sjis_p + 1 >= sjis_len) {
        utf8.push_back('?'); // XXX エラー
        break;
      }

      const uint8_t c2 = sjis[++sjis_p];
      const uint16_t c_sjis = ((uint16_t)c) << 8 | c2;
      uint32_t unicode = 0;
      for (auto conv : SJIS2UNICODE_SEQUENTIAL_CONV_TABLE) {
        if ((conv.sjis_start <= c_sjis) && (c_sjis <= conv.sjis_end)) {
          unicode = conv.unicode_start + (c_sjis - conv.sjis_start);
        }
      }

      if (unicode != 0) {
        append_to_char_from_unicode(unicode, utf8);
      } else {
        utf8.push_back('?'); // XXX エラー
      }
    }
  }
  utf8.push_back(0);

  return utf8;
}

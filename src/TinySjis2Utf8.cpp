#include "TinySjis2Utf8.hpp"
#include <cstdint>
#include <cstring>

static size_t append_to_char_from_unicode(const uint32_t unic,
                                          std::string &out) {
  if (unic < 128) {
    out += (unic);
    return 1;
  } else if (unic < 2048) {
    out += (0xC0 | (unic >> 6));
    out += (0x80 | (unic & 0x3F));
    return 2;
  } else if (unic < 65536) {
    out += (0xE0 | (unic >> 12));
    out += (0x80 | ((unic >> 6) & 0x3F));
    out += (0x80 | (unic & 0x3F));
    return 3;
  } else if (unic < 0x110000) {
    out += (0xF0 | (unic >> 18));
    out += (0x80 | ((unic >> 12) & 0x3F));
    out += (0x80 | ((unic >> 6) & 0x3F));
    out += (0x80 | (unic & 0x3F));
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
    {0x8140, 0x8142, 0x3000}, // 全角空白句読点 (　、。)
    {0x824F, 0x8258, 0xFF10}, // 全角数字 (０...９)
    {0x8260, 0x8279, 0xFF21}, // 全角ラテン大文字 (Ａ...Ｚ)
    {0x8281, 0x829A, 0xFF41}, // 全角ラテン小文字 (ａ...ｚ)
    {0x829F, 0x82F1, 0x3041}, // ひらがな (ぁ...ん)
    {0x8340, 0x837E, 0x30A1}, // カタカナ (ァ...ミ)
    {0x8380, 0x8396, 0x30E0}, // カタカナ (ム...ヶ)
    {0x839F, 0x83AF, 0x0391}, // ギリシャ大文字 (Α...Ρ)
    {0x83B0, 0x83B6, 0x03A3}, // ギリシャ大文字 (Σ...Ω)
    {0x83BF, 0x83CF, 0x03B1}, // ギリシャ小文字 (α...ρ)
    {0x83D0, 0x83D6, 0x03C3}, // ギリシャ小文字 (σ...ω)
    {0x8440, 0x8445, 0x0410}, // キリル大文字 (А...Е)
    {0x8446, 0x8446, 0x0401}, // キリル大文字 (Ё...Ё)
    {0x8447, 0x8460, 0x0416}, // キリル大文字 (Ж...Я)
    {0x8470, 0x8475, 0x0430}, // キリル小文字 (а...е)
    {0x8476, 0x8476, 0x0451}, // キリル小文字 (ё...ё)
    {0x8477, 0x847E, 0x0436}, // キリル小文字 (ж...н)
    {0x8480, 0x8491, 0x043E}, // キリル小文字 (о...я)
    {0x8740, 0x8753, 0x2460}, // 丸囲み数字(機種依存文字) (①...⑳)
    {0x8754, 0x875D, 0x2160}, // ローマ数字(機種依存文字) (Ⅰ...Ⅹ)
};

static const uint32_t UNICODE_REPLACEMENT_CHARACTER =
    0xFFFD; // U+FFFD replacement character

std::string tinysjis2utf8::sjis2utf8(File *tbl_file, const char *sjis_cstr,
                                     size_t max_sjis_len) {
  const size_t sjis_len = strnlen(sjis_cstr, max_sjis_len);
  const uint8_t *sjis = reinterpret_cast<const uint8_t *>(sjis_cstr);

  std::string utf8;
  utf8.reserve(sjis_len * 3 + 1);

  for (size_t sjis_p = 0; sjis_p < sjis_len; sjis_p++) {
    const uint8_t c = sjis[sjis_p];

    if (c == 0) {
      break;
    } else if ((c & 0xF0) <= 0x70) {
      utf8 += c;
    } else if ((0xA1 <= c) && (c <= 0xDF)) {
      // 半角カナ・半角カナ記号
      const uint32_t unicode = 0xFF61 + (c - 0xA1);
      append_to_char_from_unicode(unicode, utf8);
    } else {
      if (sjis_p + 1 >= sjis_len) {
        append_to_char_from_unicode(UNICODE_REPLACEMENT_CHARACTER, utf8);
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

      static const uint16_t TBL_START_SJIS = 0x8140;
      static const uint16_t TBL_HEADER_LEN = 0xB0;
      if ((unicode == 0) && (TBL_START_SJIS <= c_sjis) &&
          (tbl_file != nullptr)) {
        if (tbl_file->seek(((c_sjis - TBL_START_SJIS) * 2 + TBL_HEADER_LEN))) {
          uint8_t tbl_data[2];
          if (tbl_file->read(tbl_data, sizeof(tbl_data)) == sizeof(tbl_data)) {
            unicode = ((uint16_t)tbl_data[1] << 8) | (uint16_t)tbl_data[0];
          }
        }
      }

      if (unicode == 0) {
        unicode = UNICODE_REPLACEMENT_CHARACTER;
      }

      append_to_char_from_unicode(unicode, utf8);
    }
  }
  utf8 += '\0';

  return utf8;
}

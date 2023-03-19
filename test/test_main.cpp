#include <Sjis2Utf8Arduino.h>
#include <unity.h>

#include <cstring>

#ifdef ARDUINO
#include <Arduino.h>
#endif

void test_sjis2utf8_ascii() {
  char input[0x80] = {0};

  for (auto i = 0; i < sizeof(input) - 1; i++) {
    input[i] = i + 1;
  }

  const auto out = sjis2utf8arduino::sjis2utf8(input, strlen(input));
  const char *const out_cstr = reinterpret_cast<const char *>(out.data());

  TEST_ASSERT_EQUAL_STRING_LEN(input, out_cstr, strlen(input));
}

void test_sjis2utf8_fullwidth_space_punctuation() {
  const char expected[] = "　、。";
  const uint8_t input[] = {0x81, 0x40, 0x81, 0x41, 0x81, 0x42, 0x00};

  const auto out = sjis2utf8arduino::sjis2utf8(
      reinterpret_cast<const char *>(input), sizeof(input));
  const char *const out_cstr = reinterpret_cast<const char *>(out.data());

  TEST_ASSERT_EQUAL_STRING_LEN(expected, out_cstr, strlen(expected));
}

void test_sjis2utf8_fullwidth_digit() {
  const char expected[] = "０１２３４５６７８９";
  const uint8_t input[] = {
      0x82, 0x4F, 0x82, 0x50, 0x82, 0x51, 0x82, 0x52, 0x82, 0x53, 0x82,
      0x54, 0x82, 0x55, 0x82, 0x56, 0x82, 0x57, 0x82, 0x58, 0x00,
  };

  const auto out = sjis2utf8arduino::sjis2utf8(
      reinterpret_cast<const char *>(input), sizeof(input));
  const char *const out_cstr = reinterpret_cast<const char *>(out.data());

  TEST_ASSERT_EQUAL_STRING_LEN(expected, out_cstr, strlen(expected));
}

void test_sjis2utf8_fullwidth_latin() {
  const char expected[] = "ＡＺａｚ";
  const uint8_t input[] = {
      0x82, 0x60, 0x82, 0x79, 0x82, 0x81, 0x82, 0x9A, 0x00,
  };

  const auto out = sjis2utf8arduino::sjis2utf8(
      reinterpret_cast<const char *>(input), sizeof(input));
  const char *const out_cstr = reinterpret_cast<const char *>(out.data());

  TEST_ASSERT_EQUAL_STRING_LEN(expected, out_cstr, strlen(expected));
}

void test_sjis2utf8_hiragana() {
  const char expected[] = "ぁあぃをん";
  const uint8_t input[] = {0x82, 0x9F, 0x82, 0xA0, 0x82, 0xA1,
                           0x82, 0xF0, 0x82, 0xF1, 0x00};

  const auto out = sjis2utf8arduino::sjis2utf8(
      reinterpret_cast<const char *>(input), sizeof(input));
  const char *const out_cstr = reinterpret_cast<const char *>(out.data());

  TEST_ASSERT_EQUAL_STRING_LEN(expected, out_cstr, strlen(expected));
}

void test_sjis2utf8_katakana() {
  const char expected[] = "ァアミムヲンヴヵヶ";
  const uint8_t input[] = {
      0x83, 0x40, 0x83, 0x41, 0x83, 0x7E, 0x83, 0x80, 0x83, 0x92,
      0x83, 0x93, 0x83, 0x94, 0x83, 0x95, 0x83, 0x96, 0x00,
  };

  const auto out = sjis2utf8arduino::sjis2utf8(
      reinterpret_cast<const char *>(input), sizeof(input));
  const char *const out_cstr = reinterpret_cast<const char *>(out.data());

  TEST_ASSERT_EQUAL_STRING_LEN(expected, out_cstr, strlen(expected));
}

void test_sjis2utf8_greek() {
  const char expected[] = "ΑΡΣΩαρσω";
  const uint8_t input[] = {0x83, 0x9F, 0x83, 0xAF, 0x83, 0xB0, 0x83, 0xB6, 0x83,
                           0xBF, 0x83, 0xCF, 0x83, 0xD0, 0x83, 0xD6, 0x00};

  const auto out = sjis2utf8arduino::sjis2utf8(
      reinterpret_cast<const char *>(input), sizeof(input));
  const char *const out_cstr = reinterpret_cast<const char *>(out.data());

  TEST_ASSERT_EQUAL_STRING_LEN(expected, out_cstr, strlen(expected));
}

void test_sjis2utf8_cyrillic() {
  const char expected[] = "АЕЁЖЯаеёжноя";
  const uint8_t input[] = {0x84, 0x40, 0x84, 0x45, 0x84, 0x46, 0x84, 0x47, 0x84,
                           0x60, 0x84, 0x70, 0x84, 0x75, 0x84, 0x76, 0x84, 0x77,
                           0x84, 0x7E, 0x84, 0x80, 0x84, 0x91, 0x00};

  const auto out = sjis2utf8arduino::sjis2utf8(
      reinterpret_cast<const char *>(input), sizeof(input));
  const char *const out_cstr = reinterpret_cast<const char *>(out.data());

  TEST_ASSERT_EQUAL_STRING_LEN(expected, out_cstr, strlen(expected));
}

void test_sjis2utf8_circled_number() {
  const char expected[] = "①⑳";
  const uint8_t input[] = {0x87, 0x40, 0x87, 0x53, 0x00};

  const auto out = sjis2utf8arduino::sjis2utf8(
      reinterpret_cast<const char *>(input), sizeof(input));
  const char *const out_cstr = reinterpret_cast<const char *>(out.data());

  TEST_ASSERT_EQUAL_STRING_LEN(expected, out_cstr, strlen(expected));
}

void test_sjis2utf8_roman_number() {
  const char expected[] = "①⑳";
  const uint8_t input[] = {0x87, 0x40, 0x87, 0x53, 0x00};

  const auto out = sjis2utf8arduino::sjis2utf8(
      reinterpret_cast<const char *>(input), sizeof(input));
  const char *const out_cstr = reinterpret_cast<const char *>(out.data());

  TEST_ASSERT_EQUAL_STRING_LEN(expected, out_cstr, strlen(expected));
}

#ifdef ARDUINO
void setup() {
  delay(2000); // add 2-sec wait for the board w/o software resetting via
               // Serial.DTR/RTS
#else
int main(int argc, char *argv[]) {
#endif
  UNITY_BEGIN();
  RUN_TEST(test_sjis2utf8_ascii);
  RUN_TEST(test_sjis2utf8_fullwidth_space_punctuation);
  RUN_TEST(test_sjis2utf8_fullwidth_digit);
  RUN_TEST(test_sjis2utf8_fullwidth_latin);
  RUN_TEST(test_sjis2utf8_hiragana);
  RUN_TEST(test_sjis2utf8_katakana);
  RUN_TEST(test_sjis2utf8_greek);
  RUN_TEST(test_sjis2utf8_cyrillic);
  RUN_TEST(test_sjis2utf8_circled_number);
  RUN_TEST(test_sjis2utf8_roman_number);
  UNITY_END();

#ifndef ARDUINO
  return 0;
#endif
}

#ifdef ARDUINO
void loop() {}
#endif

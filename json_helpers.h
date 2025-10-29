#pragma once

#include <Arduino.h>

struct JsonFieldSpec {
  const char* key;
  String value;
  bool raw;
};

inline JsonFieldSpec jsonStringField(const char* key, const String& value) {
  return {key, value, false};
}

inline JsonFieldSpec jsonStringField(const char* key, const char* value) {
  return {key, value ? String(value) : String(), false};
}

inline JsonFieldSpec jsonBoolField(const char* key, bool value) {
  return {key, value ? String(F("true")) : String(F("false")), true};
}

template <typename T>
inline JsonFieldSpec jsonNumberField(const char* key, T value) {
  return {key, String(value), true};
}

inline JsonFieldSpec jsonFloatField(const char* key, double value, uint8_t decimals = 2) {
  return {key, String(value, static_cast<unsigned int>(decimals)), true};
}


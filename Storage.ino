/**
 * @file Storage.cpp
 * @brief Implementation of EEPROM-based storage management for Stairled AP
 */

#include "Storage.h"

bool Storage::begin() {
  EEPROM.begin(EEPROM_SIZE);
  return true;
}

String Storage::readString(const String& key, const String& defaultValue) {
  int addr = findKeyAddress(key);
  if (addr == -1) {
    return defaultValue;
  }

  int valueLength = EEPROM.read(addr + MAX_KEY_LENGTH);
  String value = "";
  for (int i = 0; i < valueLength; i++) {
    value += char(EEPROM.read(addr + MAX_KEY_LENGTH + 1 + i));
  }
  return value;
}

bool Storage::writeString(const String& key, const String& value) {
  if (key.length() > MAX_KEY_LENGTH || value.length() > MAX_VALUE_LENGTH) {
    return false;
  }

  int addr = findKeyAddress(key);
  if (addr == -1) {
    addr = findFreeSpace(MAX_KEY_LENGTH + 1 + value.length());
    if (addr == -1) {
      return false;
    }
  }

  for (unsigned int i = 0; i < key.length(); i++) {
    EEPROM.write(addr + i, key[i]);
  }
  for (unsigned int i = key.length(); i < MAX_KEY_LENGTH; i++) {
    EEPROM.write(addr + i, 0);
  }

  EEPROM.write(addr + MAX_KEY_LENGTH, value.length());
  for (unsigned int i = 0; i < value.length(); i++) {
    EEPROM.write(addr + MAX_KEY_LENGTH + 1 + i, value[i]);
  }

  return true;
}

bool Storage::commit() {
  return EEPROM.commit();
}

bool Storage::clear() {
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0);
  }
  return commit();
}

int Storage::findKeyAddress(const String& key) {
  for (int addr = 0; addr < EEPROM_SIZE; addr += MAX_KEY_LENGTH + 1 + MAX_VALUE_LENGTH) {
    bool match = true;
    for (unsigned int i = 0; i < key.length(); i++) {
      if (EEPROM.read(addr + i) != key[i]) {
        match = false;
        break;
      }
    }
    if (match) {
      return addr;
    }
  }
  return -1;
}

int Storage::findFreeSpace(int dataSize) {
  for (int addr = 0; addr < EEPROM_SIZE - dataSize; addr += MAX_KEY_LENGTH + 1 + MAX_VALUE_LENGTH) {
    bool free = true;
    for (int i = 0; i < MAX_KEY_LENGTH; i++) {
      if (EEPROM.read(addr + i) != 0) {
        free = false;
        break;
      }
    }
    if (free) {
      return addr;
    }
  }
  return -1;
}

Storage storage;
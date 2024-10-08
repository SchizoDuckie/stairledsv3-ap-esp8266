/**
 * @file Storage.h
 * @brief EEPROM-based storage management for Stairled AP
 * 
 * This class is responsible for:
 * - Providing a key-value storage system using EEPROM
 * - Handling read and write operations for configuration data
 * - Managing EEPROM space efficiently
 */

#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>

class Storage {
public:
  /**
   * @brief Initialize the storage system
   * @return bool True if initialization was successful, false otherwise
   */
  bool begin();

  /**
   * @brief Read a string value from storage
   * @param key The key to read
   * @param defaultValue The default value if key is not found
   * @return String The value associated with the key
   */
  String readString(const String& key, const String& defaultValue = "");

  /**
   * @brief Write a string value to storage
   * @param key The key to write
   * @param value The value to store
   * @return bool True if write was successful, false otherwise
   */
  bool writeString(const String& key, const String& value);

  /**
   * @brief Commit changes to EEPROM
   * @return bool True if commit was successful, false otherwise
   */
  bool commit();

  /**
   * @brief Clear all stored data
   * @return bool True if clear was successful, false otherwise
   */
  bool clear();

private:
  static const int EEPROM_SIZE = 512; // Size of EEPROM in bytes
  static const int MAX_KEY_LENGTH = 16; // Maximum length of a key
  static const int MAX_VALUE_LENGTH = 64; // Maximum length of a value

  /**
   * @brief Find the address of a key in EEPROM
   * @param key The key to find
   * @return int The address of the key, or -1 if not found
   */
  int findKeyAddress(const String& key);

  /**
   * @brief Find free space in EEPROM for a new key-value pair
   * @param dataSize The size of the data to store
   * @return int The address of free space, or -1 if not enough space
   */
  int findFreeSpace(int dataSize);
};

extern Storage storage;

#endif // STORAGE_H
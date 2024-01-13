# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/esp/esp-idf/components/bootloader/subproject"
  "D:/VScode/LORA/ESP32_LORA/build/bootloader"
  "D:/VScode/LORA/ESP32_LORA/build/bootloader-prefix"
  "D:/VScode/LORA/ESP32_LORA/build/bootloader-prefix/tmp"
  "D:/VScode/LORA/ESP32_LORA/build/bootloader-prefix/src/bootloader-stamp"
  "D:/VScode/LORA/ESP32_LORA/build/bootloader-prefix/src"
  "D:/VScode/LORA/ESP32_LORA/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/VScode/LORA/ESP32_LORA/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/VScode/LORA/ESP32_LORA/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()

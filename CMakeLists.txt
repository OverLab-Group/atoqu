cmakeminimumrequired(VERSION 3.18)
project(atoqu LANGUAGES CXX)

set(CMAKECXXSTANDARD 17)
set(CMAKECXXSTANDARD_REQUIRED ON)

option(ATOQUENABLEASAN "Enable AddressSanitizer" OFF)
option(ATOQUENABLETSAN "Enable ThreadSanitizer" OFF)
option(ATOQUENABLEUBSAN "Enable UndefinedBehaviorSanitizer" OFF)
option(ATOQUENABLECOVERAGE "Enable coverage flags" OFF)
option(ATOQUENABLECUDA "Enable CUDA backend" OFF)
option(ATOQUBUILDDOCS "Build documentation (Doxygen + Sphinx)" OFF)

if (ATOQUENABLEASAN)
  addcompileoptions(-fsanitize=address -fno-omit-frame-pointer)
  addlinkoptions(-fsanitize=address)
endif()

if (ATOQUENABLETSAN)
  addcompileoptions(-fsanitize=thread -fno-omit-frame-pointer)
  addlinkoptions(-fsanitize=thread)
endif()

if (ATOQUENABLEUBSAN)
  addcompileoptions(-fsanitize=undefined -fno-omit-frame-pointer)
  addlinkoptions(-fsanitize=undefined)
endif()

if (ATOQUENABLECOVERAGE)
  addcompileoptions(--coverage)
  addlinkoptions(--coverage)
endif()

if (ATOQUENABLECUDA)
  enable_language(CUDA)
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(docs)

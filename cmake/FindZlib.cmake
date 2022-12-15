set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH FALSE)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
endif()

# Поиск пути к заголовочным файлам
find_path(ZLIB_INCLUDE_DIR zlib.h PATHS ${CMAKE_SOURCE_DIR}/third_party/include/zlib NO_DEFAULT_PATH)
# Поиск библиотеки ZLib
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    find_library(ZLIB_LIBRARY NAMES zlibstatic PATHS ${CMAKE_SOURCE_DIR}/third_party/lib NO_DEFAULT_PATH)
else()
    find_library(ZLIB_LIBRARY NAMES z PATHS ${CMAKE_SOURCE_DIR}/third_party/lib NO_DEFAULT_PATH)
endif()

# Подключаем 'FindPackageHandle' для использования модуля поиска (find_package(<PackageName>))
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Zlib REQUIRED_VARS
    ZLIB_LIBRARY
    ZLIB_INCLUDE_DIR

    FAIL_MESSAGE "Missing ZLib. Run ./build_third_party.sh first"
)

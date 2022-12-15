set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH FALSE)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
endif()

# Поиск пути к заголовочным файлам
find_path(PYTHON_INCLUDE_DIR Python.h PATHS ${CMAKE_SOURCE_DIR}/third_party/include/python3.11 NO_DEFAULT_PATH)
# Поиск библиотеки Python
find_library(PYTHON_LIBRARY NAMES python3.11 PATHS ${CMAKE_SOURCE_DIR}/third_party/lib NO_DEFAULT_PATH)

# Подключаем 'FindPackageHandle' для использования модуля поиска (find_package(<PackageName>))
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Python REQUIRED_VARS
    PYTHON_LIBRARY
    PYTHON_INCLUDE_DIR

    FAIL_MESSAGE "Missing Python. Run ./build_third_party.sh first"
)

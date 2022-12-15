set(CMAKE_FIND_USE_SYSTEM_ENVIRONMENT_PATH FALSE)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
endif()

# Поиск пути к заголовочным файлам
find_path(OPENSSL_INCLUDE_DIR openssl/opensslconf.h PATHS ${CMAKE_SOURCE_DIR}/third_party/include NO_DEFAULT_PATH)
# Поиск библиотек OpenSSL
find_library(OPENSSL_SSL_LIBRARY NAMES ssl PATHS ${CMAKE_SOURCE_DIR}/third_party/lib NO_DEFAULT_PATH)
find_library(OPENSSL_CRYPTO_LIBRARY NAMES crypto PATHS ${CMAKE_SOURCE_DIR}/third_party/lib NO_DEFAULT_PATH)

# Подключаем 'FindPackageHandle' для использования модуля поиска (find_package(<PackageName>))
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OpenSSL REQUIRED_VARS
    OPENSSL_SSL_LIBRARY
    OPENSSL_CRYPTO_LIBRARY
    OPENSSL_INCLUDE_DIR

    FAIL_MESSAGE "Missing OpenSSL. Run ./build_third_party.sh first"
)

set(OPENSSL_LIBRARIES
    ${OPENSSL_SSL_LIBRARY}
    ${OPENSSL_CRYPTO_LIBRARY}
)

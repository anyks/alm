# Подключаем версию CMake
cmake_minimum_required(VERSION 3.15)

# TODO: вписать рабочее название проекта
project(alm LANGUAGES CXX)

set(PROJECT_VENDOR_LONG "ANYKS Language Model")

# Опции сборки
# Сборка тестового ПО
option(ENABLE_CONSOLE_APP "Build test app" TRUE)

set(PROJECT_COPYRIGHT "Copyright (c) ${CURRENT_YEAR} ${PROJECT_VENDOR_LONG}")

# Тип сборки
# cmake -DCMAKE_BUILD_TYPE=YES ..
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}\n")

# Получаем архитектуру
EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
message( STATUS "Architecture: ${ARCHITECTURE}" )

# Проверка версии OS
string(REGEX MATCH "Linux" PROJECT_OS_LINUX ${CMAKE_SYSTEM_NAME})
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    message(STATUS "Detected OS: Linux")
    set(DEBUGGER "-ggdb3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed -ldl")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    message(STATUS "Detected OS: MacOS X")
    set(DEBUGGER "-glldb")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    message(STATUS "Detected OS: FreeBSD")
    set(DEBUGGER "-ggdb3")
else()
    set(DEBUGGER "")
endif()

# Enable C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Устанавливаем опции компилятора
set(CMAKE_CXX_COMPILER_ID "clang++")
message(STATUS "Using C compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Using C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}\n")

# Информация и времени компиляции
string(TIMESTAMP CURRENT_YEAR "%Y")
string(TIMESTAMP CURRENT_TIME "%d.%m.%Y %H:%M:%S")

# Опции компилятора
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(TON_PARAMS "-Wno-cpp -Wno-deprecated")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(TON_PARAMS "-Wno-cpp -Wno-deprecated ")
else()
    set(TON_PARAMS "-Wno-unknown-attributes")
endif()

if (${ARCHITECTURE} MATCHES "aarch64" OR ${ARCHITECTURE} MATCHES "arm*")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -pipe -DNDEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fno-permissive -Wno-pedantic -Wno-narrowing -Wno-deprecated-declarations ${TON_PARAMS}")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -pipe -mrdrnd -march=core2 -DNDEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fno-permissive -Wno-pedantic -Wno-narrowing -Wno-deprecated-declarations ${TON_PARAMS}")
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -DDEBUG_MODE ${DEBUGGER}")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "C Flags: ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}\n")
else()
    message(STATUS "C Flags: ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}\n")
endif()

# Добавляем модули cmake
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Выполняем поиск нужных библиотек
find_package(Zlib REQUIRED)
find_package(Python REQUIRED)
find_package(OpenSSL REQUIRED)

# Провекра OpenSSL
if (OpenSSL_FOUND)
    message(STATUS "OpenSSL libs found: " ${OPENSSL_SSL_LIBRARY} ${OPENSSL_CRYPTO_LIBRARY})
    message(STATUS "OpenSSL include dir found: " ${OPENSSL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "OpenSSL not found")
endif()

# Провекра ZLib
if (Zlib_FOUND)
    message(STATUS "ZLib lib found: " ${ZLIB_LIBRARY})
    message(STATUS "ZLib include dir found: " ${ZLIB_INCLUDE_DIR})
else()
    message(FATAL_ERROR "ZLib not found")
endif()

# Провекра Python
if (Python_FOUND)
    message(STATUS "Python lib found: " ${PYTHON_LIBRARY})
    message(STATUS "Python include dir found: " ${PYTHON_INCLUDE_DIR})
else()
    message(FATAL_ERROR "Python not found")
endif()

# Выполняем подключение хидеров
include_directories(${ZLIB_INCLUDE_DIR})
include_directories(${PYTHON_INCLUDE_DIR})
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/contrib/include)

# Выполняем подключение исходников
set(SOURCE_FILES
    "${CMAKE_SOURCE_DIR}/src/idw.cpp"
    "${CMAKE_SOURCE_DIR}/src/nwt.cpp"
    "${CMAKE_SOURCE_DIR}/src/env.cpp"
    "${CMAKE_SOURCE_DIR}/src/alm.cpp"
    "${CMAKE_SOURCE_DIR}/src/alm1.cpp"
    "${CMAKE_SOURCE_DIR}/src/alm2.cpp"
    "${CMAKE_SOURCE_DIR}/src/arpa.cpp"
    "${CMAKE_SOURCE_DIR}/src/ablm.cpp"
    "${CMAKE_SOURCE_DIR}/src/python.cpp"
    "${CMAKE_SOURCE_DIR}/src/toolkit.cpp"
    "${CMAKE_SOURCE_DIR}/src/alphabet.cpp"
    "${CMAKE_SOURCE_DIR}/src/progress.cpp"
    "${CMAKE_SOURCE_DIR}/src/tokenizer.cpp"
    "${CMAKE_SOURCE_DIR}/src/collector.cpp"
    "${CMAKE_SOURCE_DIR}/src/levenshtein.cpp"
    "${CMAKE_SOURCE_DIR}/contrib/src/cityhash/city.cc"
    "${CMAKE_SOURCE_DIR}/contrib/src/bigint/BigInteger.cc"
    "${CMAKE_SOURCE_DIR}/contrib/src/bigint/BigUnsigned.cc"
    "${CMAKE_SOURCE_DIR}/contrib/src/bigint/BigIntegerUtils.cc"
    "${CMAKE_SOURCE_DIR}/contrib/src/bigint/BigUnsignedInABase.cc"
    "${CMAKE_SOURCE_DIR}/contrib/src/bigint/BigIntegerAlgorithms.cc"
)

set(ALM_LIB_NAME ${PROJECT_NAME})
set(ALM_APP_NAME ${PROJECT_NAME}_bin)

# Делаем либу
add_library(${ALM_LIB_NAME} STATIC ${SOURCE_FILES})

# Если установлен флаг ENABLE_CONSOLE_APP собираем консольное приложение
if (ENABLE_CONSOLE_APP)

    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set(WINFLAGS -lws2_32 -lgdi32 -lcrypt32 -lIphlpapi)
    else()
        set(WINFLAGS "")
    endif()

    # Устанавливаем иконку для windows под MinGW.
    set(RES_FILES "")
    if(MINGW)
        set(RES_FILES "alm.rc")
        set(CMAKE_RC_COMPILER_INIT windres)
        ENABLE_LANGUAGE(RC)
        SET(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    endif(MINGW)

    add_executable(${ALM_APP_NAME} app/alm.cpp ${RES_FILES})

    # Если операционной системой является Windows
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        target_link_libraries(${ALM_APP_NAME}
            ${ALM_LIB_NAME}
            ${ZLIB_LIBRARY}
            ${PYTHON_LIBRARY}
            ${OPENSSL_LIBRARIES}
            ${WINFLAGS}
        )
    # Для всех остальных операционных систем
    else()
        target_link_libraries(${ALM_APP_NAME}
            ${ALM_LIB_NAME}
            ${ZLIB_LIBRARY}
            ${PYTHON_LIBRARY}
            ${OPENSSL_LIBRARIES}
        )
    endif()

    set_target_properties(${ALM_APP_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
endif()

# Устанавливаем адрес установки, запрет установки в /usr/local
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local" OR "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local/")
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")
endif()

# Устанавливаем хидеры
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/app" DESTINATION "${CMAKE_INSTALL_PREFIX}/include/app/${CMAKE_PROJECT_NAME}" FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include" DESTINATION "${CMAKE_INSTALL_PREFIX}/include/${CMAKE_PROJECT_NAME}" FILES_MATCHING PATTERN "*.hpp")
# Устанавливаем библиотеку
install(TARGETS ${ALM_LIB_NAME} DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
# Устанавливаем исполнительные файлы
install(TARGETS ${ALM_APP_NAME} DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/${CMAKE_PROJECT_NAME}")

# Устанавливаем хидеры сторонних библиотек
install(DIRECTORY "${ZLIB_INCLUDE_DIR}" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${PYTHON_INCLUDE_DIR}" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${OPENSSL_INCLUDE_DIR}/openssl" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" FILES_MATCHING PATTERN "*.h")

# Устанавливаем заголовочные файлы зависимостей
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/contrib/include" DESTINATION "${CMAKE_INSTALL_PREFIX}/include" FILES_MATCHING PATTERN "*.h")

# Устанавливаем стороние библиотеки
install(FILES ${ZLIB_LIBRARY} DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
install(FILES ${PYTHON_LIBRARY} DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")
install(FILES ${OPENSSL_LIBRARIES} DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

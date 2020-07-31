# Example:
# cmake \
# -DOPENSSL_ROOT_DIR=/usr/local/Cellar/openssl@1.1/1.1.1g \
# -DOPENSSL_LIBRARIES=/usr/local/Cellar/openssl@1.1/1.1.1g/include/lib \
# -DPYTHON_INCLUDE_DIR='/System/Volumes/Data/usr/local/Cellar/python@3.8/3.8.3_2/Frameworks/Python.framework/Versions/3.8/include/python3.8' \
# -DPYTHON_LIBRARY='/System/Volumes/Data/usr/local/Cellar/python@3.8/3.8.3_2/Frameworks/Python.framework/Versions/3.8/lib/libpython3.8.dylib' \
# ..

# Проверка версии CMake.
cmake_minimum_required(VERSION 2.8)

# Название проекта
project(ANYKS LANGUAGES CXX)

# Устанавливаем название вендора
set(PROJECT_VENDOR_LONG "ANYKS")
set(ORG_WEBSITE "https://anyks.com")
set(APP_NAME "Smart Language Model")

# Устанавливаем адрес установки
if("${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local" OR "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local/")
	set(INSTALL_TO_USR TRUE)
else()
	set(INSTALL_TO_USR FALSE)
endif()

# Устанавливаем флаг использования потоков
set(THREADS_PREFER_PTHREAD_FLAG ON)
# Ищем библиотеку Threads
find_package(Threads REQUIRED)

# Ищем библиотеку ZLib
find_package(ZLIB REQUIRED)

# Выводим сообщение
message("Zlib include dir: ${ZLIB_INCLUDE_DIRS}")
message("Zlib libraries: ${ZLIB_LIBRARIES}")

# Подключаем заголовочные файлы
include_directories(${ZLIB_INCLUDE_DIRS})
# Добавляем библиотеки в список
list(APPEND LIB_LIST ${ZLIB_LIBRARIES})

# Ищем библиотеку Python
find_package(PythonLibs 3.7 REQUIRED)

# Выводим сообщение
message("Python include dir: ${PYTHON_INCLUDE_DIRS}")
message("Python libraries: ${PYTHON_LIBRARIES}")

# Подключаем заголовочные файлы
include_directories(${PYTHON_INCLUDE_DIRS})
# Добавляем библиотеки в список
list(APPEND LIB_LIST ${PYTHON_LIBRARIES})

# Ищем зависимости OpenSSL
# find_library(SSL_LIBRARY ssl)
# find_library(CRYPTO_LIBRARY crypto)
find_package(OpenSSL REQUIRED)

# Выводим сообщение
message("OpenSSL libraries: ${OPENSSL_LIBRARIES}")
message("OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")

# Формируем время для копирайта
string(TIMESTAMP CURRENT_YEAR "%Y")
string(TIMESTAMP CURRENT_TIME "%d.%m.%Y %H:%M:%S")
# Формируем копирайт
set(PROJECT_COPYRIGHT "Copyright (c) ${CURRENT_YEAR} ${PROJECT_VENDOR_LONG}")
# Выводим сообщение о начале работы
message(STATUS "Creating \"${APP_NAME}\" application")

# Set build type
# if(NOT CMAKE_BUILD_TYPE)
#	set(CMAKE_BUILD_TYPE Debug)
# endif()
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# Подключаем зависимости
set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "-stdlib=libc++")
# Устанавливаем опции компилятора
message(STATUS "Using C++ compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Using C compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

# Способ первый: затребовать от компилятора фичу cxx_std_17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Подстраиваемся под компилятор
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -fPIC -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CC_OPT_FLAGS} ${CXX_OPT_FLAGS}")

# Если режим отладки включён
if(CMAKE_BUILD_TYPE MATCHES Debug)
	message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
	message(STATUS "C Flags: ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}")
else()
	message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
	message(STATUS "C Flags: ${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}")
endif()

# Устанавливаем путь установки приложения
set(CMAKE_PREFIX_PATH $ENV{ALM_PREFIX_PATH} ${CMAKE_PREFIX_PATH})

# Если установка требуется
if(INSTALL_TO_USR)
	set(CONFIG_INSTALL_DEST "/${CONFIG_INSTALL_DEST}")
endif()

# Устанавливаем адрес каталога установки
set(CONFIG_INSTALL_DEST "etc/${PROJECT_NAME}")

# Подключаем заголовочные файлы
include_directories(${OPENSSL_INCLUDE_DIR})
# Добавляем библиотеки в список
list(APPEND LIB_LIST ${OPENSSL_LIBRARIES})

# Устанавливаем каталог заголовков
include_directories(${PROJECT_BINARY_DIR}/../)
include_directories(${PROJECT_BINARY_DIR}/../include)
include_directories(${PROJECT_BINARY_DIR}/../contrib/include)

# Установка переменной со списком исходников для исполняемого файла
set(ALM_EXE ${PROJECT_BINARY_DIR}/../app/alm.cpp)

# Добавляем исходники
set(ENV_LIB ${PROJECT_BINARY_DIR}/../src/env.cpp)
set(IDW_LIB ${PROJECT_BINARY_DIR}/../src/idw.cpp)
set(NWT_LIB ${PROJECT_BINARY_DIR}/../src/nwt.cpp)
set(ALM_LIB ${PROJECT_BINARY_DIR}/../src/alm.cpp)
set(ALM1_LIB ${PROJECT_BINARY_DIR}/../src/alm1.cpp)
set(ALM2_LIB ${PROJECT_BINARY_DIR}/../src/alm2.cpp)
set(ARPA_LIB ${PROJECT_BINARY_DIR}/../src/arpa.cpp)
set(ABLM_LIB ${PROJECT_BINARY_DIR}/../src/ablm.cpp)
set(PYTHON_LIB ${PROJECT_BINARY_DIR}/../src/python.cpp)
set(TOOLKIT_LIB ${PROJECT_BINARY_DIR}/../src/toolkit.cpp)
set(PROGRESS_LIB ${PROJECT_BINARY_DIR}/../src/progress.cpp)
set(ALPHABET_LIB ${PROJECT_BINARY_DIR}/../src/alphabet.cpp)
set(COLLECTOR_LIB ${PROJECT_BINARY_DIR}/../src/collector.cpp)
set(TOKENIZER_LIB ${PROJECT_BINARY_DIR}/../src/tokenizer.cpp)
set(LEVENSHTEIN_LIB ${PROJECT_BINARY_DIR}/../src/levenshtein.cpp)
set(CITYHASH_LIB ${PROJECT_BINARY_DIR}/../contrib/src/cityhash/city.cc)
set(BIGINTEGER_LIB ${PROJECT_BINARY_DIR}/../contrib/src/bigint/BigInteger.cc)
set(BIGUNSIGNED_LIB ${PROJECT_BINARY_DIR}/../contrib/src/bigint/BigUnsigned.cc)
set(BIGINTEGERUTILS_LIB ${PROJECT_BINARY_DIR}/../contrib/src/bigint/BigIntegerUtils.cc)
set(BIGUNSIGNEDINBASE_LIB ${PROJECT_BINARY_DIR}/../contrib/src/bigint/BigUnsignedInABase.cc)
set(BIGINTEGERALGORITHMS_LIB ${PROJECT_BINARY_DIR}/../contrib/src/bigint/BigIntegerAlgorithms.cc)

# Определяем операционную систему
if(APPLE)
	# Выводим сообщение
	message("Select MacOS X")
	# Добавляем библиотеку Gperftools
	set(GPERFTOOLS_LIB /usr/local/lib/libtcmalloc.a)
elseif(UNIX)
	# Если это Linux
	if(CMAKE_SYSTEM_NAME STREQUAL Linux)
		# Выводим сообщение
		message("Select Linux")
		# Добавляем библиотеку Gperftools
		set(GPERFTOOLS_LIB tcmalloc)
	endif()
	# Если это FreeBSD
	if(CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
		# Выводим сообщение
		message("Select FreeBSD")
		# Добавляем библиотеку Gperftools
		set(GPERFTOOLS_LIB /usr/local/lib/libtcmalloc.a)
	endif()
endif()

# Создаем список модулей
list(APPEND LIB_LISTS ${IDW_LIB} ${NWT_LIB} ${ALPHABET_LIB} ${ENV_LIB} ${LEVENSHTEIN_LIB} ${CITYHASH_LIB} ${COLLECTOR_LIB} ${TOKENIZER_LIB} ${ALM_LIB} ${ALM1_LIB} ${ALM2_LIB} ${ARPA_LIB} ${PYTHON_LIB} ${ABLM_LIB} ${PROGRESS_LIB} ${TOOLKIT_LIB} ${BIGINTEGER_LIB} ${BIGUNSIGNED_LIB} ${BIGINTEGERUTILS_LIB} ${BIGUNSIGNEDINBASE_LIB} ${BIGINTEGERALGORITHMS_LIB})

# Создание динамической библиотеки с именем alm
add_library(lm STATIC ${LIB_LISTS})

# Создает исполняемый файл с именем alm
add_executable(alm ${ALM_EXE})

# Выполняем линковку приложения
target_link_libraries(alm lm Threads::Threads ${GPERFTOOLS_LIB} ${BIGINT_LIB} ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${PYTHON_LIBRARIES})

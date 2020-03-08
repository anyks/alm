# Example:
# cmake -DOPENSSL_ROOT_DIR=/usr/local/Cellar/openssl@1.1/1.1.1d -DOPENSSL_LIBRARIES=/usr/local/Cellar/openssl@1.1/1.1.1d/include/lib ..

# Проверка версии CMake.
cmake_minimum_required(VERSION 2.8)

# Название проекта
project(ANYKS LANGUAGES CXX)

# Устанавливаем параметры сборки
set(CMAKE_CXX_FLAGS "-fPIC -g")

# Способ первый: затребовать от компилятора фичу cxx_std_17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

# Подключаем зависимости
set_target_properties(${TARGET} PROPERTIES COMPILE_FLAGS "-stdlib=libc++")

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
find_package(PythonLibs REQUIRED)

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

# Подключаем заголовочные файлы
include_directories(${OPENSSL_INCLUDE_DIR})
# Добавляем библиотеки в список
list(APPEND LIB_LIST ${OPENSSL_LIBRARIES})

# Устанавливаем каталог заголовков
include_directories(${PROJECT_BINARY_DIR}/../include)
include_directories(${PROJECT_BINARY_DIR}/../json/include)
include_directories(${PROJECT_BINARY_DIR}/../lib/BigInt/include)

# Установка переменной со списком исходников для исполняемого файла
set(ALM_EXE ${PROJECT_BINARY_DIR}/../app/alm.cpp)

# Добавляем исходники
set(ENV_LIB ${PROJECT_BINARY_DIR}/../src/env.cpp)
set(IDW_LIB ${PROJECT_BINARY_DIR}/../src/idw.cpp)
set(NWT_LIB ${PROJECT_BINARY_DIR}/../src/nwt.cpp)
set(ARPA_LIB ${PROJECT_BINARY_DIR}/../src/arpa.cpp)
set(ABLM_LIB ${PROJECT_BINARY_DIR}/../src/ablm.cpp)
set(PYTHON_LIB ${PROJECT_BINARY_DIR}/../src/python.cpp)
set(TOOLKIT_LIB ${PROJECT_BINARY_DIR}/../src/toolkit.cpp)
set(PROGRESS_LIB ${PROJECT_BINARY_DIR}/../src/progress.cpp)
set(ALPHABET_LIB ${PROJECT_BINARY_DIR}/../src/alphabet.cpp)
set(COLLECTOR_LIB ${PROJECT_BINARY_DIR}/../src/collector.cpp)
set(TOKENIZER_LIB ${PROJECT_BINARY_DIR}/../src/tokenizer.cpp)
set(LEVENSHTEIN_LIB ${PROJECT_BINARY_DIR}/../src/levenshtein.cpp)

# Определяем операционную систему
if(APPLE)
	# Выводим сообщение
	message("Select MacOS X")
	# Добавляем библиотеку Gperftools
	set(GPERFTOOLS_LIB /usr/local/lib/libtcmalloc.a)
	# Добавляем исходник
	set(BIGINT_LIB ${PROJECT_BINARY_DIR}/../lib/BigInt/mac/libbint.a)
elseif(UNIX)
	# Если это Linux
	if(CMAKE_SYSTEM_NAME STREQUAL Linux)
		# Выводим сообщение
		message("Select Linux")
		# Добавляем библиотеку Gperftools
		set(GPERFTOOLS_LIB tcmalloc)
		# Добавляем исходник
		set(BIGINT_LIB ${PROJECT_BINARY_DIR}/../lib/BigInt/linux/libbint.a)
	endif()
	# Если это FreeBSD
	if(CMAKE_SYSTEM_NAME STREQUAL FreeBSD)
		# Выводим сообщение
		message("Select FreeBSD")
		# Добавляем библиотеку Gperftools
		set(GPERFTOOLS_LIB /usr/local/lib/libtcmalloc.a)
		# Добавляем исходник
		set(BIGINT_LIB ${PROJECT_BINARY_DIR}/../lib/BigInt/freebsd/libbint.a)
	endif()
endif()

# Создаем список модулей
list(APPEND LIB_LISTS ${IDW_LIB} ${NWT_LIB} ${ALPHABET_LIB} ${ENV_LIB} ${LEVENSHTEIN_LIB} ${COLLECTOR_LIB} ${TOKENIZER_LIB} ${ARPA_LIB} ${PYTHON_LIB} ${ABLM_LIB} ${PROGRESS_LIB} ${TOOLKIT_LIB})

# Создание динамической библиотеки с именем alm
add_library(lm STATIC ${LIB_LISTS})

# Создает исполняемый файл с именем alm
add_executable(alm ${ALM_EXE})

# Выполняем линковку приложения
target_link_libraries(alm lm Threads::Threads ${GPERFTOOLS_LIB} ${BIGINT_LIB} ${ZLIB_LIBRARIES} ${OPENSSL_LIBRARIES} ${PYTHON_LIBRARIES})

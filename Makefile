# Определяем тип операционной системы
OS := $(shell uname -s)

# Название приложения
NAME = alm

# Если это MacOS X (Сборка через make)
ifeq ($(OS), Darwin)
	# Компилятор
	CC = clang++

	# Версия
	PYVER = 7
	# Питон
	PYTHON = python3.$(PYVER)m
	# Путь питона
	PYPATH = /usr/local/Frameworks/Python.framework/Versions/3.$(PYVER)

	# Сторонние модули
	LIBS = \
		-lm \
		-lz \
		-lssl \
		-lstdc++ \
		-lcrypto \
		-lpthread \
		-ljemalloc \
		-l$(PYTHON) \
		-L$(PYPATH)/lib \
		-L/usr/local/Cellar/openssl@1.1/1.1.1d/lib

	# Расширение библиотеки
	EXT = dylib

	# Библиотеки для тестовой сборки
	UNITLIBS = \
		./gtest/mac/libgtest.a \
		./gtest/mac/libgtest_main.a

	# Заголовочные файлы
	INCLUDE = \
		-I./ \
		-I./include \
		-I./json/include \
		-I./contrib/include \
		-I/usr/include \
		-I/usr/local/include \
		-I./gtest/include \
		-I$(PYPATH)/include/$(PYTHON) \
		-I/usr/local/Cellar/openssl@1.1/1.1.1d/include

	# brew install google-perftools

	# Отладчик
	DEBUG = -glldb
endif

# Если это FreeBSD (Сборка через gmake)
ifeq ($(OS), FreeBSD)
	# Компилятор
	CC = clang++

	# Версия
	PYVER = 7
	# Питон
	PYTHON = python3.$(PYVER)m

	# Сторонние модули
	LIBS = \
		-lm \
		-lz \
		-lssl \
		-lstdc++ \
		-lcrypto \
		-lpthread \
		-l$(PYTHON) \
		-ljemalloc \
		-L/usr/local/lib

	# Расширение библиотеки
	EXT = so

	# Библиотеки для тестовой сборки
	UNITLIBS = \
		./gtest/freebsd/libgtest.a \
		./gtest/freebsd/libgtest_main.a

	# Заголовочные файлы
	INCLUDE = \
		-I./ \
		-I./include \
		-I./json/include \
		-I./contrib/include \
		-I/usr/include \
		-I/usr/local/include \
		-I./gtest/include \
		-I/usr/local/include/$(PYTHON)

	# pkg install google-perftools-2.7_1

	# Отладчик
	DEBUG = -glldb
endif

# Если это Linux
ifeq ($(OS), Linux)
	# Компилятор
	CC = clang++

	# Версия
	PYVER = 7
	# Питон
	PYTHON = python3.$(PYVER)m

	# Сторонние модули
	LIBS = \
		-lm \
		-lz \
		-lssl \
		-lstdc++ \
		-lcrypto \
		-lpthread \
		-ljemalloc \
		-l$(PYTHON)

	# Расширение библиотеки
	EXT = so

	# Библиотеки для тестовой сборки
	UNITLIBS = \
		./gtest/linux/libgtest.a \
		./gtest/linux/libgtest_main.a

	# Заголовочные файлы
	INCLUDE = \
		-I./ \
		-I./include \
		-I./json/include \
		-I./contrib/include \
		-I/usr/include \
		-I/usr/local/include \
		-I./gtest/include \
		-I/usr/include/$(PYTHON)

	# apt install libgoogle-perftools-dev

	# Отладчик
	DEBUG = -ggdb3
endif

# Бинарный файл
BIN = ./bin
# Каталог сборки
BUILD = ./build

# Конфиг для сборки отладочной версии
CONFDEV = -std=c++11 -O0 -fno-permissive -Wno-pedantic -Wno-unknown-attributes
# Конфиг для стандартной сборки
CONFIG = -std=c++11 -O2 -pipe -mfma -mrdrnd -mf16c -fno-permissive -Wno-pedantic -Wno-unknown-attributes

# ./app/alm.cpp

# Правило сборки
all:
	mkdir -p $(BUILD) && \
	mkdir -p $(BIN)/libs && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./app/alm.cpp -o $(BUILD)/main.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/alm.cpp -o $(BUILD)/alm.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/idw.cpp -o $(BUILD)/idw.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/nwt.cpp -o $(BUILD)/nwt.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/env.cpp -o $(BUILD)/env.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/arpa.cpp -o $(BUILD)/arpa.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/ablm.cpp -o $(BUILD)/ablm.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/python.cpp -o $(BUILD)/python.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/toolkit.cpp -o $(BUILD)/toolkit.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/alphabet.cpp -o $(BUILD)/alphabet.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/progress.cpp -o $(BUILD)/progress.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/tokenizer.cpp -o $(BUILD)/tokenizer.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/collector.cpp -o $(BUILD)/collector.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/levenshtein.cpp -o $(BUILD)/levenshtein.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/cityhash/city.cc -o $(BUILD)/cityhash.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigInteger.cc -o $(BUILD)/BigInteger.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigUnsigned.cc -o $(BUILD)/BigUnsigned.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigIntegerUtils.cc -o $(BUILD)/BigIntegerUtils.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigUnsignedInABase.cc -o $(BUILD)/BigUnsignedInABase.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigIntegerAlgorithms.cc -o $(BUILD)/BigIntegerAlgorithms.o && \
	$(CC) $(LIBS) -shared -fPIC -o $(BIN)/libs/liblm.$(EXT) $(BUILD)/idw.o $(BUILD)/nwt.o $(BUILD)/env.o \
	$(BUILD)/cityhash.o $(BUILD)/arpa.o $(BUILD)/ablm.o $(BUILD)/python.o $(BUILD)/toolkit.o $(BUILD)/alm.o \
	$(BUILD)/tokenizer.o $(BUILD)/alphabet.o $(BUILD)/levenshtein.o $(BUILD)/collector.o $(BUILD)/progress.o \
	$(BUILD)/BigInteger.o $(BUILD)/BigUnsigned.o $(BUILD)/BigIntegerUtils.o $(BUILD)/BigUnsignedInABase.o $(BUILD)/BigIntegerAlgorithms.o && \
	ar rcs $(BIN)/libs/liblm.a $(BUILD)/idw.o $(BUILD)/nwt.o $(BUILD)/env.o $(BUILD)/alm.o $(BUILD)/arpa.o $(BUILD)/cityhash.o \
	$(BUILD)/ablm.o $(BUILD)/python.o $(BUILD)/toolkit.o $(BUILD)/collector.o $(BUILD)/tokenizer.o $(BUILD)/alphabet.o \
	$(BUILD)/levenshtein.o $(BUILD)/progress.o $(BUILD)/BigInteger.o $(BUILD)/BigUnsigned.o $(BUILD)/BigIntegerUtils.o \
	$(BUILD)/BigUnsignedInABase.o $(BUILD)/BigIntegerAlgorithms.o && \
	$(CC) $(BUILD)/main.o -L$(BIN)/libs -Wl,-rpath,$(BIN)/libs -llm $(LIBS) -o $(BIN)/$(NAME) && \
	rm -rf $(BUILD)

# Правила сборки под Dev
dev:
	mkdir -p $(BUILD) && \
	mkdir -p $(BIN)/libs && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./app/alm.cpp -o $(BUILD)/main.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/alm.cpp -o $(BUILD)/alm.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/idw.cpp -o $(BUILD)/idw.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/nwt.cpp -o $(BUILD)/nwt.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/env.cpp -o $(BUILD)/env.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/arpa.cpp -o $(BUILD)/arpa.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/ablm.cpp -o $(BUILD)/ablm.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/python.cpp -o $(BUILD)/python.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/toolkit.cpp -o $(BUILD)/toolkit.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/alphabet.cpp -o $(BUILD)/alphabet.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/progress.cpp -o $(BUILD)/progress.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/tokenizer.cpp -o $(BUILD)/tokenizer.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/collector.cpp -o $(BUILD)/collector.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./src/levenshtein.cpp -o $(BUILD)/levenshtein.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./contrib/src/cityhash/city.cc -o $(BUILD)/cityhash.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigInteger.cc -o $(BUILD)/BigInteger.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigUnsigned.cc -o $(BUILD)/BigUnsigned.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigIntegerUtils.cc -o $(BUILD)/BigIntegerUtils.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigUnsignedInABase.cc -o $(BUILD)/BigUnsignedInABase.o && \
	$(CC) $(CONFDEV) $(DEBUG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigIntegerAlgorithms.cc -o $(BUILD)/BigIntegerAlgorithms.o && \
	ar rcs $(BIN)/libs/liblm.a $(BUILD)/idw.o $(BUILD)/nwt.o $(BUILD)/env.o $(BUILD)/alm.o $(BUILD)/arpa.o $(BUILD)/cityhash.o \
	$(BUILD)/ablm.o $(BUILD)/python.o $(BUILD)/toolkit.o $(BUILD)/collector.o $(BUILD)/tokenizer.o $(BUILD)/alphabet.o \
	$(BUILD)/levenshtein.o $(BUILD)/progress.o $(BUILD)/BigInteger.o $(BUILD)/BigUnsigned.o $(BUILD)/BigIntegerUtils.o \
	$(BUILD)/BigUnsignedInABase.o $(BUILD)/BigIntegerAlgorithms.o && \
	$(CC) $(DEBUG) $(BUILD)/main.o -L$(BIN)/libs -Wl,-rpath,$(BIN)/libs -L/usr/lib -L/usr/local/lib -llm $(LIBS) -o $(BIN)/$(NAME) && \
	rm -rf $(BUILD)

# Правила статической сборки
static:
	mkdir -p $(BUILD) && \
	mkdir -p $(BIN)/libs && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./app/alm.cpp -o $(BUILD)/main.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/alm.cpp -o $(BUILD)/alm.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/idw.cpp -o $(BUILD)/idw.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/nwt.cpp -o $(BUILD)/nwt.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/env.cpp -o $(BUILD)/env.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/arpa.cpp -o $(BUILD)/arpa.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/ablm.cpp -o $(BUILD)/ablm.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/python.cpp -o $(BUILD)/python.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/toolkit.cpp -o $(BUILD)/toolkit.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/alphabet.cpp -o $(BUILD)/alphabet.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/progress.cpp -o $(BUILD)/progress.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/tokenizer.cpp -o $(BUILD)/tokenizer.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/collector.cpp -o $(BUILD)/collector.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./src/levenshtein.cpp -o $(BUILD)/levenshtein.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/cityhash/city.cc -o $(BUILD)/cityhash.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigInteger.cc -o $(BUILD)/BigInteger.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigUnsigned.cc -o $(BUILD)/BigUnsigned.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigIntegerUtils.cc -o $(BUILD)/BigIntegerUtils.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigUnsignedInABase.cc -o $(BUILD)/BigUnsignedInABase.o && \
	$(CC) $(CONFIG) $(INCLUDE) -fPIC -c ./contrib/src/bigint/BigIntegerAlgorithms.cc -o $(BUILD)/BigIntegerAlgorithms.o && \
	ar rcs $(BIN)/libs/liblm.a $(BUILD)/idw.o $(BUILD)/nwt.o $(BUILD)/env.o $(BUILD)/alm.o $(BUILD)/arpa.o $(BUILD)/cityhash.o \
	$(BUILD)/ablm.o $(BUILD)/python.o $(BUILD)/toolkit.o $(BUILD)/collector.o $(BUILD)/tokenizer.o $(BUILD)/alphabet.o \
	$(BUILD)/levenshtein.o $(BUILD)/progress.o $(BUILD)/BigInteger.o $(BUILD)/BigUnsigned.o $(BUILD)/BigIntegerUtils.o \
	$(BUILD)/BigUnsignedInABase.o $(BUILD)/BigIntegerAlgorithms.o && \
	$(CC) $(BUILD)/main.o -L$(BIN)/libs -Wl,-rpath,$(BIN)/libs -llm $(LIBS) -o $(BIN)/$(NAME) && \
	rm -rf $(BUILD)

# Правило очистки
clean:
	rm -rf $(BIN) && rm -rf $(BUILD)

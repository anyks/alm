#!/usr/bin/env bash

# Получаем корневую дирректорию
readonly ROOT=$(cd "$(dirname "$0")" && pwd)

# Получаем версию OS
OS=$(uname -a | awk '{print $1}')

if [[ $OS =~ "MINGW64" ]]; then
	OS="Windows"
fi

if [ -n "$1" ]; then
	if [ $1 = "--clean" ]; then
		# Очистка подпроекта
		clean_submodule(){
			cd "$ROOT/submodules/$1" || exit 1
			git clean -dfx
			git stash
			cd "$ROOT" || exit 1
		}

		# Очистка директории
		clean_directory(){
			git clean -dfx "$1"
		}

		# Очищаем подпроекты
		clean_submodule "zlib"
		clean_submodule "cpython"
		clean_submodule "openssl"

		# Удаляем сборочную директорию
		rm -rf "$ROOT/third_party"

		printf "\n****************************************"
		printf "\n************   Success!!!   ************"
		printf "\n****************************************"
		printf "\n\n\n"

		exit 0
	else
		printf "Usage: config [options]\n"
		printf " --clean - Cleaning all submodules and build directory\n"

		exit 1
	fi
fi

# Устанавливаем систему сборки
if [[ $OS = "Windows" ]]; then
	export BUILD="mingw32-make"
else
	export BUILD="make"
fi

# Каталог для установки собранных библиотек
PREFIX="$ROOT/third_party"
export PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig"

# Устанавливаем флаги глобального использования
# export CPPFLAGS=""
export CXXFLAGS="$CXXFLAGS -fPIC"
export CFLAGS="-I$PREFIX/include -fPIC"

export LDFLAGS="-L$PREFIX/lib"
export LD_LIBRARY_PATH="$PREFIX/lib"

# Создаём каталоги
mkdir -p "$PREFIX/bin"
mkdir -p "$PREFIX/lib"
mkdir -p "$PREFIX/include"

# Определяем количество логических ядер
if [ $OS = "Darwin" ]; then
	numproc=$(sysctl -n hw.logicalcpu)
elif [ $OS = "FreeBSD" ]; then
	numproc=$(sysctl -n hw.ncpu)
else
	numproc=$(nproc)
fi

if [ $OS = "Darwin" ]; then # MacOS
	INSTALL_CMD="ditto -v"
elif [ $OS = "FreeBSD" ]; then # FreeBSD
	INSTALL_CMD="install -m 0644"
	# Создаём каталон назначения заголовочных файлов
	mkdir -p "$PREFIX/include/brotli"
elif [ $OS = "Windows" ]; then # Windows
	INSTALL_CMD="install -D -m 0644"
else # Linux
	INSTALL_CMD="install -D -m 0644"
fi

# Применяем патчи
apply_patch(){
	patch="$ROOT/patches/$1/$2"
	if ! git apply --reverse --check "$patch" 2> /dev/null; then
		echo "applaying patch $patch"
		git apply "$patch" || exit 1
	else
		echo "patch $patch already applied"
	fi
}

# Инициализируем подпроекты
git submodule update --init --recursive --remote

# Сборка OpenSSL
src="$ROOT/submodules/openssl"
if [ ! -f "$src/.stamp_done" ]; then
	printf "\n****** OpenSSL ******\n"
	cd "$src" || exit 1

	# Версия OpenSSL
	ver="p"

	# Переключаемся на master
	git checkout master
	# Закачиваем все теги
	git fetch --all --tags
	# Удаляем старую ветку
	git branch -D v1.1.1${ver}-branch
	# Выполняем переключение на указанную версию
	git checkout tags/OpenSSL_1_1_1${ver} -b v1.1.1${ver}-branch

	# Выполняем конфигурацию проекта под Linux или FreeBSD
	if [[ $OS = "Linux" ]] || [[ $OS = "FreeBSD" ]]; then
		# Выполняем конфигурацию проекта
		./config \
		 sctp \
		 no-async \
		 no-shared \
		 --prefix="$PREFIX" \
		 --openssldir="$PREFIX" \
		 -Wl,-rpath,"$PREFIX/lib" || exit 1
	# Выполняем конфигурацию проекта под все остальные операционные системы
	else
		# Выполняем конфигурацию проекта
		./config \
		 no-async \
		 no-shared \
		 --prefix="$PREFIX" \
		 --openssldir="$PREFIX" \
		 -Wl,-rpath,"$PREFIX/lib" || exit 1
	fi

	# Выполняем сборку на всех логических ядрах
	$BUILD -j"$numproc" || exit 1
	# Выполняем установку проекта без документации
	$BUILD install_sw || exit 1
	$BUILD install_ssldirs || exit 1

	# Помечаем флагом, что сборка и установка произведена
	touch "$src/.stamp_done"
	cd "$ROOT" || exit 1
fi

# Устанавливаем переменные окружения
export OPENSSL_CFLAGS="-I$BUILD/openssl/include"
export OPENSSL_LIBS="-L$BUILD/openssl/lib -lssl -lcrypto"

# Сборка Zlib
src="$ROOT/submodules/zlib"
if [ ! -f "$src/.stamp_done" ]; then
	printf "\n****** ZLib ******\n"
	cd "$src" || exit 1

	# Версия Zlib
	ver="1.2.11"

	# Переключаемся на master
	git checkout master
	# Закачиваем все теги
	git fetch --all --tags
	# Удаляем старую ветку
	git branch -D v${ver}-branch
	# Выполняем переключение на указанную версию
	git checkout tags/v${ver} -b v${ver}-branch

	# Выполняем конфигурацию проекта
	if [[ $OS = "Windows" ]]; then
		# Создаём каталог сборки
		mkdir -p "build" || exit 1
		# Переходим в каталог
		cd "build" || exit 1

		# Удаляем старый файл кэша
		rm -rf ./CMakeCache.txt

		cmake \
		 -DCMAKE_BUILD_TYPE="Release" \
		 -DCMAKE_SYSTEM_NAME="Windows" \
		 -DCMAKE_INSTALL_PREFIX="$PREFIX" \
		 -DINSTALL_INC_DIR="$PREFIX/include/zlib" \
		 -DBUILD_SHARED_LIBS="NO" \
		 -G "MinGW Makefiles" \
		 .. || exit 1
	else
		./configure \
		 --prefix="$PREFIX" \
		 --libdir="$PREFIX/lib" \
		 --includedir="$PREFIX/include/zlib" \
		 --static || exit 1
	fi

	# Выполняем сборку на всех логических ядрах
	$BUILD -j"$numproc" || exit 1
	# Выполняем установку проекта
	$BUILD install || exit 1

	# Помечаем флагом, что сборка и установка произведена
	touch "$src/.stamp_done"
	cd "$ROOT" || exit 1
fi

# Сборка CPython
src="$ROOT/submodules/cpython"
if [ ! -f "$src/.stamp_done" ]; then
	printf "\n****** Python ******\n"
	cd "$src" || exit 1

	# Версия Python
	ver="3.11.1"

	# Переключаемся на main
	git checkout main
	# Закачиваем все теги
	git fetch --all --tags
	# Удаляем старую ветку
	git branch -D v${ver}-branch
	# Выполняем переключение на указанную версию
	git checkout tags/v${ver} -b v${ver}-branch

	# Выполняем сборку
	./configure \
	 --prefix="$PREFIX" || exit 1

	# Выполняем сборку на всех логических ядрах
	$BUILD -j"$numproc" || exit 1
	# Выполняем установку проекта без документации
	$BUILD install || exit 1

	# Помечаем флагом, что сборка и установка произведена
	touch "$src/.stamp_done"
	cd "$ROOT" || exit 1
fi

# Переименовываем расширение библиотек для Windows
if [ $OS = "Windows" ]; then # Windows
	for i in $(ls "$PREFIX/lib" | grep .a$);
	do
		mv "$PREFIX/lib/$i" "$PREFIX/lib/$(basename "$i" .a).lib"
	done
fi

printf "\n****************************************"
printf "\n************   Success!!!   ************"
printf "\n****************************************"
printf "\n\n\n"

# ANYKS Language Model (ALM) C++11

- [Design goals](#design-goals)
- [Sponsors](#sponsors)
- [Integration](#integration)
  - [CMake](#cmake)
  - [Package Managers](#package-managers)
- [Examples](#examples)
  - [JSON as first-class data type](#json-as-first-class-data-type)
  - [Serialization / Deserialization](#serialization--deserialization)
  - [STL-like access](#stl-like-access)
  - [Conversion from STL containers](#conversion-from-stl-containers)
  - [JSON Pointer and JSON Patch](#json-pointer-and-json-patch)
  - [JSON Merge Patch](#json-merge-patch)
  - [Implicit conversions](#implicit-conversions)
  - [Conversions to/from arbitrary types](#arbitrary-types-conversions)
  - [Specializing enum conversion](#specializing-enum-conversion)
  - [Binary formats (BSON, CBOR, MessagePack, and UBJSON)](#binary-formats-bson-cbor-messagepack-and-ubjson)
- [Supported compilers](#supported-compilers)
- [License](#license)
- [Contact](#contact)
- [Thanks](#thanks)
- [Used third-party tools](#used-third-party-tools)
- [Projects using JSON for Modern C++](#projects-using-json-for-modern-c)
- [Notes](#notes)
- [Execute unit tests](#execute-unit-tests)

## Цели проекта

Существует множество библиотек языковых моделей ([KenLM](https://github.com/kpu/kenlm), [SriLM](https://github.com/BitMindLab/SRILM), [IRSTLM](https://github.com/irstlm-team/irstlm)), и у каждой из них может быть причина для существования. Наша языковая модель преследовала следующие цели:

- **Поддержка UTF-8**. Полная поддержка UTF-8 без сторонних зависимостей.

- **Поддержка форматов данных**. Arpa, Vocab, Map Sequence, N-grams, Binary alm dictionary.

- **Поддержка алгоритмов сглаживания**. Kneser-Nay, Modified Kneser-Nay, Witten-Bell, Additive, Good-Turing, Absolute discounting.

- **Нормализация входных корпусов**. Приведение слов к нижнему регистру, умная токенизация, поддержка чёрного и белого списков.

- **Модификация arpa**. Замена частот, замена n-грамм, добавление новых n-грамм с частотами, удаление n-грамм.

- **Прунинг**. Сокращение числа n-грамм которые не соответствуют указанным критериям качества.

- **Чистка плохих n-грамм**. Удаление n-грамм у которых обратная частота backoff выше основной частоты.

- **Восстановление arpa**. Восстановление повреждённых n-грамм в arpa с последующим перерасчётом их backoff частот.

- **Поддержка дополнительных признаков слов**. Определение в тексте признаков: (чисел, римских чисел, диапазонов чисел, числовых аббривиатур, любых других пользовательских признаков с помощью скриптов написанных на языке Python3).

- **Обработка грязных текстов**. В отличие от всех остальных языковых моделей, здесь мы умеем извлекать правильный контекст из текстовых файлов с грязными текстами.

- **Полноценный учёт <unk> признака**. Учёт <unk> признака как полноценной n-граммы.

- **Переопределение <unk> признака**. Возможность переопределения признака неизвестного слова.

- **Препроцессинг обрабатываемых n-грамм**. Возможность предобрабатывать n-граммы перед добавлением в arpa с помощью пользовательских скриптов на языке Python3.

- **Бинарный контейнер языковой модели**. Бинарный bALM контейнер поддерживает сжатие, шифрование и установку копирайтов.

- **Удобная визуализация хода процесса**. В ALM реализовано несколько видов визуализаций: текстовая, графическая в виде индикатора процесса, логирование в файлы или консоль.

- **Гарантированная сборка всех n-грамм**. В отличие от остальных языковых моделей, ALM гарантированно собирает все n-граммы из текста в независимости от их длины (кроме Modified Kneser-Nay), также возможно принудительно учитывать все n-граммы даже если они встретились всего 1 раз.

## Внешние зависимости

- [Zlib](http://www.zlib.net)

- [OpenSSL](https://www.openssl.org)

- [GperfTools](https://github.com/gperftools/gperftools)

- [Python3](https://www.python.org/download/releases/3.0)

- [NLohmann::json](https://github.com/nlohmann/json)

- [BigInteger](http://mattmccutchen.net/bigint)

## Сборка проекта

### Linux и FreeBSD

```bash
$ mkdir ./build
$ cd ./build

$ cmake ..
$ make
```

### MacOS X

```bash
$ mkdir ./build
$ cd ./build

$ cmake -DOPENSSL_ROOT_DIR=/usr/local/Cellar/openssl@1.1/1.1.1d -DOPENSSL_LIBRARIES=/usr/local/Cellar/openssl@1.1/1.1.1d/include/lib ..
$ make
```

## Примеры

...

## License

![MIT License](http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png "MIT License")

The class is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright © 2020 [Yuriy Lobarev](https://anyks.com)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

* * *

## Contact

If you have questions regarding the library, I would like to invite you to [open an issue at GitHub](https://github.com/nlohmann/json/issues/new/choose). Please describe your request, problem, or question as detailed as possible, and also mention the version of the library you are using as well as the version of your compiler and operating system. Opening an issue at GitHub allows other users and contributors to this library to collaborate.

---

[Yuriy Lobarev](https://anyks.com) <forman@anyks.com>

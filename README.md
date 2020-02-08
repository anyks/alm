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

## Описание форматов файлов

**Arpa**
```
\data\
ngram 1=52
ngram 2=68
ngram 3=15

\1-grams:
-1.807052	1-й	-0.30103
-1.807052	2	-0.30103
-1.807052	3~4	-0.30103
-1.150508	</s>
-99	<s>	-0.3309932
-2.112406	<unk>
-1.807052	T358	-0.30103
-1.807052	VII	-0.30103
-1.503878	Грека	-0.39794
-1.807052	Греку	-0.30103
-1.62953	Ехал	-0.30103
...

\2-grams:
-0.29431	1-й передал
-0.29431	2 ложки
-0.29431	3~4 дня
-0.8407791	<s> Ехал
...

\3-grams:
-0.09521468	рак на руке
...

\end\
```

---

**Ngrams**
```
\data\
ad=1
cw=23832
unq=9390

ngram 1=9905
ngram 2=21907
ngram 3=306

\1-grams:
<s>	2022 | 1
<num>	117 | 1
<unk>	19 | 1
<rng>	7 | 1
<abr>	16 | 1
</s>	2022 | 1
А	244 | 1
а	244 | 1
б	11 | 1
в	762 | 1
ыборах	112 | 1
обзорах	224 | 1
половозрелые	1 | 1
небесах	86 | 1
изобретали	978 | 1
яблочную	396 | 1
джинсах	108 | 1
классах	77 | 1
трассах	32 | 1

\2-grams:
<s> <num>	7 | 1
<s> <unk>	1 | 1
<s> а	84 | 1
<s> в	83 | 1
<s> и	57 | 1
и классные	82 | 1
и валютные	11 | 1
и несправедливости	24 | 1
снилось являлось	18 | 1
нашлось никого	31 | 1
соответственно вы	45 | 1
соответственно дома	97 | 1
соответственно наша	71 | 1

\3-grams:
<s> <num> </s>	3 | 1
<s> а в	6 | 1
<s> а я	4 | 1
<s> а на	2 | 1
<s> а то	3 | 1
можно и нужно	2 | 1
будет хорошо </s>	2 | 1
пейзажи за окном	2 | 1
статусы для одноклассников	2 | 1
только в одном	2 | 1
работа связана с	2 | 1
говоря про то	2 | 1
отбеливания зубов </s>	2 | 1
продолжение следует </s>	3 | 1
препараты от варикоза	2 | 1

\end\
```

- **ad** Количество документов в корпусе
- **cw** Количество слов во всех документах корпуса
- **unq** Количество уникальных собранных слов

---

**Мета-данные бинарного контейнера**
```json
{
	"aes": 128,                 // размер шифрования AES (128, 192, 256)
	"name": "Name dictionary",  // название словаря
	"author": "Name author",    // автор словаря
	"lictype": "License type",  // тип лицензии
	"lictext": "License text",  // текст лицензии
	"contacts": "...",          // контактные данные автора
	"password": "...",          // пароль шифрования (если требуется), шифрование производится только при установки пароля
	"copyright": "..."          // копирайт владельца словаря
}
```

---

## Примеры

### Пример обучения языковой модели

**Алгоритм сглаживания: Witten-Bell, сборка из одного файла**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./text.txt
```

**Алгоритм сглаживания: Absolute discounting, сборка из группы файлов**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing cdiscount -discount 0.3 -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -path ./corpus -ext txt
```

**Алгоритм сглаживания: Additive, сборка из группы файлов**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing addsmooth -delta 0.3 -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -path ./corpus -ext txt
```

**Алгоритм сглаживания: Kneser-Nay, сборка из группы файлов**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing kneserney -kneserney-modified -kneserney-prepares -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -path ./corpus -ext txt
```

**Алгоритм сглаживания: Good-Turing, сборка из группы файлов бинарного контейнера**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing goodturing -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -path ./corpus -ext txt -w-bin ./lm.alm -w-bin-meta ./meta.json -w-bin-arpa -w-bin-usigns -w-bin-options -w-bin-preword -w-bin-badwords -w-bin-goodwords
```

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

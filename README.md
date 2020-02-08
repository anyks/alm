# ANYKS Language Model (ALM) C++11

- [Цели проекта](#markdown-header-)
- [Внешние зависимости](#sponsors)
- [Сборка проекта](#sponsors)
  - [Linux и FreeBSD](#cmake)
  - [MacOS X](#package-managers)
- [Описание форматов файлов](#examples)
  - [Arpa](#json-as-first-class-data-type)
  - [Ngrams](#serialization--deserialization)
  - [Vocab](#stl-like-access)
  - [Map](#conversion-from-stl-containers)
  - [Файл добавления n-грамм в существующий файл arpa](#json-pointer-and-json-patch)
  - [Файл изменения частот n-грамм в существующем файле arpa](#json-merge-patch)
  - [Файл замены n-грамм в существующем файле arpa](#implicit-conversions)
  - [Файл удаления n-грамм в существующем файле arpa](#arbitrary-types-conversions)
  - [Мета-данные бинарного контейнера](#specializing-enum-conversion)
  - [Формат скрипта python предобработки полученных слов](#binary-formats-bson-cbor-messagepack-and-ubjson)
  - [Формат скрипта python определения собственных признаков слов](#binary-formats-bson-cbor-messagepack-and-ubjson)s
- [Примеры](#supported-compilers)
  - [Пример обучения языковой модели](#supported-compilers)
  - [Пример исправления arpa](#supported-compilers)
  - [Пример удаления n-грамм у которых частота ниже backoff](#supported-compilers)
  - [Пример пруннинга arpa](#supported-compilers)
  - [Пример модификации arpa](#supported-compilers)
- [License](#license)
- [Contact](#contact)

## Цели проекта

Существует множество библиотек языковых моделей ([KenLM](https://github.com/kpu/kenlm), [SriLM](https://github.com/BitMindLab/SRILM), [IRSTLM](https://github.com/irstlm-team/irstlm)), и у каждой из них может быть причина для существования. Наша языковая модель преследовала следующие цели:

- **Поддержка UTF-8**: Полная поддержка UTF-8 без сторонних зависимостей.

- **Поддержка форматов данных**: Arpa, Vocab, Map Sequence, N-grams, Binary alm dictionary.

- **Поддержка алгоритмов сглаживания**: Kneser-Nay, Modified Kneser-Nay, Witten-Bell, Additive, Good-Turing, Absolute discounting.

- **Нормализация входных корпусов**: Приведение слов к нижнему регистру, умная токенизация, поддержка чёрного и белого списков.

- **Модификация arpa**: Замена частот, замена n-грамм, добавление новых n-грамм с частотами, удаление n-грамм.

- **Прунинг**: Сокращение числа n-грамм которые не соответствуют указанным критериям качества.

- **Чистка плохих n-грамм**: Удаление n-грамм у которых обратная частота backoff выше основной частоты.

- **Восстановление arpa**: Восстановление повреждённых n-грамм в arpa с последующим перерасчётом их backoff частот.

- **Поддержка дополнительных признаков слов**: Определение в тексте признаков: (чисел, римских чисел, диапазонов чисел, числовых аббривиатур, любых других пользовательских признаков с помощью скриптов написанных на языке Python3).

- **Обработка грязных текстов**: В отличие от всех остальных языковых моделей, здесь мы умеем извлекать правильный контекст из текстовых файлов с грязными текстами.

- **Полноценный учёт <unk> признака**: Учёт <unk> признака как полноценной n-граммы.

- **Переопределение <unk> признака**: Возможность переопределения признака неизвестного слова.

- **Препроцессинг обрабатываемых n-грамм**: Возможность предобрабатывать n-граммы перед добавлением в arpa с помощью пользовательских скриптов на языке Python3.

- **Бинарный контейнер языковой модели**: Бинарный bALM контейнер поддерживает сжатие, шифрование и установку копирайтов.

- **Удобная визуализация хода процесса**: В ALM реализовано несколько видов визуализаций: текстовая, графическая в виде индикатора процесса, логирование в файлы или консоль.

- **Гарантированная сборка всех n-грамм**: В отличие от остальных языковых моделей, ALM гарантированно собирает все n-граммы из текста в независимости от их длины (кроме Modified Kneser-Nay), также возможно принудительно учитывать все n-граммы даже если они встретились всего 1 раз.

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

### Arpa
```
\data\
ngram 1=52
ngram 2=68
ngram 3=15

\1-grams:
-1.807052	1-й	-0.30103
-1.807052	2	-0.30103
-1.807052	3~4	-0.30103
-2.332414	как	-0.394770
-3.185530	после	-0.311249
-3.055896	того	-0.441649
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
-1.328447	после того	-0.477121
...

\3-grams:
-0.09521468	рак на руке
-0.166590	после того как
...

\end\
```

| Частота               | N-грамма                     | Обратная частота           |
|-----------------------|------------------------------|----------------------------|
| -1.328447             | после того                   | -0.477121                  |

- **<s>** Начало предложения
- **</s>** Конец предложения
- **<num>** Число (арабское или римское)
- **<unk>** Неизвестное слово
- **<rng>** Диапазон чисел (1-2, 100-200, 300-400 ...)
- **<abr>** Аббривиатура (1-й, 2-е, 20-я ...)

---

### Ngrams
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

| N-грамма              | Встречаемость в корпусе      | Встречаемость в документах |
|-----------------------|------------------------------|----------------------------|
| только в одном        | 2                            | 1                          |

- **ad** Количество документов в корпусе
- **cw** Количество слов во всех документах корпуса
- **unq** Количество уникальных собранных слов

---

### Vocab
```
\data\
ad=1
cw=23832
unq=9390

\words:
33	а	244 | 1 | 0.010238 | 0.000000 | -3.581616
34	б	11 | 1 | 0.000462 | 0.000000 | -6.680889
35	в	762 | 1 | 0.031974 | 0.000000 | -2.442838
40	ж	12 | 1 | 0.000504 | 0.000000 | -6.593878
330344	был	47 | 1 | 0.001972 | 0.000000 | -5.228637
335190	вам	17 | 1 | 0.000713 | 0.000000 | -6.245571
335192	дам	1 | 1 | 0.000042 | 0.000000 | -9.078785
335202	нам	22 | 1 | 0.000923 | 0.000000 | -5.987742
335206	сам	7 | 1 | 0.000294 | 0.000000 | -7.132874
335207	там	29 | 1 | 0.001217 | 0.000000 | -5.711489
2282019644	похожесть	1 | 1 | 0.000042 | 0.000000 | -9.078785
2282345502	новый	10 | 1 | 0.000420 | 0.000000 | -6.776199
2282416889	белый	2 | 1 | 0.000084 | 0.000000 | -8.385637
3009239976	гражданский	1 | 1 | 0.000042 | 0.000000 | -9.078785
3009763109	банкиры	1 | 1 | 0.000042 | 0.000000 | -9.078785
3013240091	геныч	1 | 1 | 0.000042 | 0.000000 | -9.078785
3014009989	преступлениях	1 | 1 | 0.000042 | 0.000000 | -9.078785
3015727462	тысяч	2 | 1 | 0.000084 | 0.000000 | -8.385637
3025113549	позаботьтесь	1 | 1 | 0.000042 | 0.000000 | -9.078785
3049820849	комментарием	1 | 1 | 0.000042 | 0.000000 | -9.078785
3061388599	компьютерная	1 | 1 | 0.000042 | 0.000000 | -9.078785
3063804798	шаблонов	1 | 1 | 0.000042 | 0.000000 | -9.078785
3071212736	завидной	1 | 1 | 0.000042 | 0.000000 | -9.078785
3074971025	холодной	1 | 1 | 0.000042 | 0.000000 | -9.078785
3075044360	выходной	1 | 1 | 0.000042 | 0.000000 | -9.078785
3123271427	делаешь	1 | 1 | 0.000042 | 0.000000 | -9.078785
3123322362	читаешь	1 | 1 | 0.000042 | 0.000000 | -9.078785
3126399411	готовится	1 | 1 | 0.000042 | 0.000000 | -9.078785
...
```

| Идентификатор слова   | Слово     | Встречаемость в корпусе    | Встречаемость в документах | tf       | tf-idf   | wltf      |
|-----------------------|-----------|----------------------------|----------------------------|----------|----------|-----------|
| 2282345502            | новый     | 10                         | 1                          | 0.000420 | 0.000000 | -6.776199 |

- **oc** Встречаемость в корпусе
- **dc** Встречаемость в документах
- **tf** (term frequency — частота слова) — отношение числа вхождений некоторого слова к общему числу слов документа. Таким образом, оценивается важность слова в пределах отдельного документа, расчитывается как: [tf = oc / cw]
- **idf** (inverse document frequency — обратная частота документа) — инверсия частоты, с которой некоторое слово встречается в документах коллекции, расчитывается как: [idf = log(ad / dc)]
- **tf-idf** Расчитывается как: [tf-idf = tf * idf]
- **wltf** Рейтинг слова, расчитывается как: [wltf = 1 + log(tf * dc)]

---

### Map
```
1:{2022,1,0}|42:{57,1,0}|279603:{2,1,0}
1:{2022,1,0}|42:{57,1,0}|320749:{2,1,0}
1:{2022,1,0}|42:{57,1,0}|351283:{2,1,0}
1:{2022,1,0}|42:{57,1,0}|379815:{3,1,0}
1:{2022,1,0}|42:{57,1,0}|26122748:{3,1,0}
1:{2022,1,0}|44:{6,1,0}
1:{2022,1,0}|48:{1,1,0}
1:{2022,1,0}|51:{11,1,0}|335967:{3,1,0}
1:{2022,1,0}|53:{14,1,0}|371327:{3,1,0}
1:{2022,1,0}|53:{14,1,0}|40260976:{7,1,0}
1:{2022,1,0}|65:{68,1,0}|34:{2,1,0}
1:{2022,1,0}|65:{68,1,0}|3277:{3,1,0}
1:{2022,1,0}|65:{68,1,0}|278003:{2,1,0}
1:{2022,1,0}|65:{68,1,0}|320749:{2,1,0}
1:{2022,1,0}|65:{68,1,0}|11353430797:{2,1,0}
1:{2022,1,0}|65:{68,1,0}|34270133320:{2,1,0}
1:{2022,1,0}|65:{68,1,0}|51652356484:{2,1,0}
1:{2022,1,0}|65:{68,1,0}|66967237546:{2,1,0}
1:{2022,1,0}|2842:{11,1,0}|42:{7,1,0}
...
```

> Содержимое этого файла имеет чисто техническое значение. В сочетании с файлом **vocab** можно объединять несколько языковых моделей, модифицировать, хранить, распространять и извлекать любые форматы (arpa, ngrams, vocab, alm).

---

### Файл добавления n-грамм в существующий файл arpa
```
-3.002006	США
-1.365296	границ США
-0.988534	у границ США
-1.759398	замуж за
-0.092796	собираюсь замуж за
-0.474876	и тоже
-19.18453	можно и тоже
...
```
| Частота n-граммы      | Разделитель | N-грамма     |
|-----------------------|-------------|--------------|
| -0.988534             | \t          | у границ США |

---

### Файл изменения частот n-грамм в существующем файле arpa
```
-0.6588787	получайте удовольствие </s>
-0.6588787	только в одном
-0.6588787	работа связана с
-0.6588787	мужчины и женщины
-0.6588787	говоря про то
-0.6588787	потому что я
-0.6588787	потому что это
-0.6588787	работу потому что
-0.6588787	пейзажи за окном
-0.6588787	статусы для одноклассников
-0.6588787	вообще не хочу
...
```
| Частота n-граммы      | Разделитель | N-грамма          |
|-----------------------|-------------|-------------------|
| -0.6588787            | \t          | мужчины и женщины |

---

### Файл замены n-грамм в существующем файле arpa
```
коем случае нельзя	там да тут
но тем не	да ты что
неожиданный у	ожидаемый к
в СМИ	в ФСБ
Шах	Мат
```
| Существующая n-грамма | Разделитель | Новая n-грамма    |
|-----------------------|-------------|-------------------|
| но тем не             | \t          | да ты что         |

---

### Файл удаления n-грамм в существующем файле arpa
```
ну то есть
ну очень большой
бы было если
мы с ней
ты смеешься над
два года назад
над тем что
или еще что-то
как я понял
как ни удивительно
как вы знаете
так и не
все-таки права
все-таки болят
все-таки сдохло
все-таки встала
все-таки решился
уже
мне
мое
все
```

---

### Мета-данные бинарного контейнера
```json
{
	"aes": 128,
	"name": "Name dictionary",
	"author": "Name author",
	"lictype": "License type",
	"lictext": "License text",
	"contacts": "...",
	"password": "...",
	"copyright": "..."
}
```

- **aes** Размер шифрования AES (128, 192, 256) бит
- **name** Название словаря
- **author** Автор словаря
- **lictype** Тип лицензии
- **lictext** Текст лицензии
- **contacts** Контактные данные автора
- **password** Пароль шифрования (если требуется), шифрование производится только при установки пароля
- **copyright** Копирайт владельца словаря

---

### Формат скрипта python предобработки полученных слов
```python
# -*- coding: utf-8 -*-

def init():
    """
    Метод инициализации: выполняется только один раз при запуске приложения
    """

def run(word, context):
    """
    Метод запуска обработки: запускается при извлечении слова из текста
    @word    обрабатываемое слово
    @context последовательность предыдущих слов в виде массива
    """
    return word
```

---

### Формат скрипта python определения собственных признаков слов
```python
# -*- coding: utf-8 -*-

def init():
    """
    Метод инициализации: выполняется только один раз при запуске приложения
    """

def run(using, word):
    """
    Метод запуска обработки: запускается при извлечении слова из текста
    @using название признака слова
    @word  обрабатываемое слово
    """
    if using and (using == "<usa>"):
        if word and (word.lower() == "сша"): return "ok"
    elif using and (using == "<russia>"):
        if word and (word.lower() == "россия"): return "ok"
    return "no"
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

**Алгоритм сглаживания: Witten-Bell, сборка из бинарного контейнера**
```bash
$ ./alm -r-bin ./lm.alm -r-bin-meta ./meta.json -method train -debug 1 -size 3 -smoothing wittenbell -w-arpa ./lm.arpa
```

### Пример исправления arpa

```bash
./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method repair -debug 1 -w-arpa ./lm2.arpa -allow-unk -interpolate -r-arpa ./lm1.arpa
```

### Пример удаления n-грамм у которых частота ниже backoff
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method sweep -debug 1 -w-arpa ./lm2.arpa -allow-unk -interpolate -r-arpa ./lm1.arpa
```

### Пример пруннинга arpa
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method prune -debug 1 -w-arpa ./lm2.arpa -allow-unk -interpolate -r-map ./lm.map -r-vocab ./lm.vocab -prune-threshold 0.003 -prune-max-gram 2
```

### Пример модификации arpa

**Добавление в arpa n-грамм**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method modify -modify emplace -modify-file ./app.txt -debug 1 -w-arpa ./lm.arpa -allow-unk -interpolate -r-map ./lm.map -r-vocab ./lm.vocab
```

**Изменение частот n-грамм в arpa**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method modify -modify change -modify-file ./chg.txt -debug 1 -w-arpa ./lm.arpa -allow-unk -interpolate -r-map ./lm.map -r-vocab ./lm.vocab
```

**Удаление n-грамм в arpa**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method modify -modify remove -modify-file ./rm.txt -debug 1 -w-arpa ./lm.arpa -allow-unk -interpolate -r-map ./lm.map -r-vocab ./lm.vocab
```

**Изменение n-грамм в arpa**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method modify -modify replace -modify-file ./rep.txt -debug 1 -w-arpa ./lm.arpa -allow-unk -interpolate -r-map ./lm.map -r-vocab ./lm.vocab
```

* * *

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

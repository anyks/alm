[![ANYKS Smart language model](https://raw.githubusercontent.com/anyks/alm/master/site/img/banner.jpg)](https://anyks.com)

# ANYKS LM (ALM) C++11

- [Цели проекта](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-цели-проекта)
- [Внешние зависимости](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-внешние-зависимости)
- [Сборка проекта](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-сборка-проекта)
  - [Клонирование проекта](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-клонирование-проекта-модификатор---recursive-обязателен)
  - [Linux и FreeBSD](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-linux-и-freebsd)
  - [MacOS X](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-macos-x)
- [Описание форматов файлов](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-описание-форматов-файлов)
  - [Arpa](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-arpa)
  - [Ngrams](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-ngrams)
  - [Vocab](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-vocab)
  - [Map](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-map)
  - [Файл добавления n-грамм в существующий файл arpa](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-файл-добавления-n-грамм-в-существующий-файл-arpa)
  - [Файл изменения частот n-грамм в существующем файле arpa](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-файл-изменения-частот-n-грамм-в-существующем-файле-arpa)
  - [Файл замены n-грамм в существующем файле arpa](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-файл-замены-n-грамм-в-существующем-файле-arpa)
  - [Файл удаления n-грамм в существующем файле arpa](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-файл-удаления-n-грамм-в-существующем-файле-arpa)
  - [Мета-данные бинарного контейнера](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-мета-данные-бинарного-контейнера)
  - [Формат скрипта python предобработки полученных слов](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-формат-скрипта-python-предобработки-полученных-слов)
  - [Формат скрипта python определения собственных токенов слов](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-формат-скрипта-python-определения-собственных-токенов-слов)
- [Переменные окружения](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-переменные-окружения)
- [Примеры](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-примеры)
  - [Пример обучения языковой модели](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-пример-обучения-языковой-модели)
  - [Пример исправления arpa](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-пример-исправления-arpa)
  - [Пример удаления n-грамм у которых частота ниже backoff](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-пример-удаления-n-грамм-у-которых-частота-ниже-backoff)
  - [Пример прунинга arpa](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-пример-пруннинга-arpa)
  - [Информация о бинарном контейнере](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-информация-о-бинарном-контейнере)
  - [Пример модификации arpa](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-пример-модификации-arpa)
  - [Обучение с предобработкой полученных слов](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-обучение-с-предобработкой-полученных-слов)
  - [Обучение с использованием своих токенов](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-обучение-с-использованием-своих-токенов)
  - [Обучение с белым списком](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-обучение-с-белым-списком)
  - [Обучение с чёрным списком](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-обучение-с-чёрным-списком)
  - [Обучение с неизвестным словом](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-обучение-с-неизвестным-словом)
  - [Токенизация текста](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-токенизация-текста)
- [Лицензия](https://github.com/anyks/alm/blob/master/site/RU.md/#user-content-лицензия)
- [Контакты](https://github.com/anyks/alm/blob/master/site/RU.md/#markdown-header-контакты)

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

- **Поддержка дополнительных токенов слов**: Определение в тексте токенов: (чисел, римских чисел, диапазонов чисел, числовых аббривиатур, любых других пользовательских токенов с помощью скриптов написанных на языке Python3).

- **Обработка грязных текстов**: В отличие от всех остальных языковых моделей, здесь мы умеем извлекать правильный контекст из текстовых файлов с грязными текстами.

- **Полноценный учёт 〈unk〉 токена**: Учёт 〈unk〉 токена как полноценной n-граммы.

- **Переопределение 〈unk〉 токена**: Возможность переопределения токена неизвестного слова.

- **Препроцессинг обрабатываемых n-грамм**: Возможность предобрабатывать n-граммы перед добавлением в arpa с помощью пользовательских скриптов на языке Python3.

- **Бинарный контейнер языковой модели**: Бинарный контейнер поддерживает сжатие, шифрование и установку копирайтов.

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

### Клонирование проекта, флаг [--recursive] обязателен

```bash
$ git clone --recursive https://github.com/anyks/alm.git
```

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

#### Описание:

- **〈s〉** - Токен начала предложения
- **〈/s〉** - Токен конеца предложения
- **〈url〉** - Токен url адреса
- **〈num〉** - Токен чисел (арабское или римское)
- **〈unk〉** - Токен неизвестного слова
- **〈date〉** - Токен даты (**18.07.2004** | **07/18/2004**)
- **〈time〉** - Токен времени (**15:44:56**)
- **〈abbr〉** - Токен аббревиатуры (**1-й** | **2-е** | **20-я**)
- **〈anum〉** - Токен псевдо-числа (**T34** | **895-M-86** | **39km**)
- **〈math〉** - Токен математической операции (**+** | **-** | **=** | **/** | ***** | **^**)
- **〈range〉** - Токен диапазона чисел (**1-2** | **100-200** | **300-400**)
- **〈aprox〉** - Токен приблизительного числа (**~93** | **~95.86** | **10~20**)
- **〈score〉** - Токен числового счёта (**4:3** | **01:04**)
- **〈dimen〉** - Токен габаритных размеров (**200x300** | **1920x1080**)
- **〈fract〉** - Токен числовой дроби (**5/20** | **192/864**)
- **〈punct〉** - Токен знаков пунктуации (**.** | **...** | **,** | **!** | **?** | **:** | **;**)
- **〈isolat〉** - Токен символов изоляции (**"** | **'** | **«** | **»** | **„** | **“** | **`** | **(** | **)** | **[** | **]** | **{** | **}**)
- **〈specs〉** - Токен спец-символа (**~** | **@** | **#** | **№** | **%** | **&** | **$** | **§** | **©** | **〈** | **〉**)

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
<abbr>	16 | 1
<range>	7 | 1
</s>	2022 | 1
А	244 | 1
а	244 | 1
б	11 | 1
в	762 | 1
выборах	112 | 1
обзорах	224 | 1
половозрелые	1 | 1
небесах	86 | 1
изобретали	978 | 1
яблочную	396 | 1
джинсах	108 | 1
классах	77 | 1
трассах	32 | 1
...

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
...

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
...

\end\
```

| N-грамма              | Встречаемость в корпусе      | Встречаемость в документах |
|-----------------------|------------------------------|----------------------------|
| только в одном        | 2                            | 1                          |

#### Описание:

- **ad** - Количество документов в корпусе
- **cw** - Количество слов во всех документах корпуса
- **unq** - Количество уникальных собранных слов

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

#### Описание:

- **oc** - Встречаемость в корпусе
- **dc** - Встречаемость в документах
- **tf** - (term frequency — частота слова) — отношение числа вхождений некоторого слова к общему числу слов документа. Таким образом, оценивается важность слова в пределах отдельного документа, расчитывается как: [tf = oc / cw]
- **idf** - (inverse document frequency — обратная частота документа) — инверсия частоты, с которой некоторое слово встречается в документах коллекции, расчитывается как: [idf = log(ad / dc)]
- **tf-idf** - Расчитывается как: [tf-idf = tf * idf]
- **wltf** - Рейтинг слова, расчитывается как: [wltf = 1 + log(tf * dc)]

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
...
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
...
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
	"contacts": "Contacts data",
	"password": "Password if needed",
	"copyright": "Copyright author"
}
```

#### Описание:

- **aes** - Размер шифрования AES (128, 192, 256) бит
- **name** - Название словаря
- **author** - Автор словаря
- **lictype** - Тип лицензии
- **lictext** - Текст лицензии
- **contacts** - Контактные данные автора
- **password** - Пароль шифрования (если требуется), шифрование производится только при установки пароля
- **copyright** - Копирайт владельца словаря

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

### Формат скрипта python определения собственных токенов слов
```python
# -*- coding: utf-8 -*-

def init():
    """
    Метод инициализации: выполняется только один раз при запуске приложения
    """

def run(token, word):
    """
    Метод запуска обработки: запускается при извлечении слова из текста
    @token название токена слова
    @word  обрабатываемое слово
    """
    if token and (token == "<usa>"):
        if word and (word.lower() == "сша"): return "ok"
    elif token and (token == "<russia>"):
        if word and (word.lower() == "россия"): return "ok"
    return "no"
```

---

### Переменные окружения

- Все параметры можно передавать через переменные окружения. Переменные начинаются с префикса **ALM_** и должны записываться в верхнем регистре, в остальном названия их соответствуют параметрам приложения.
- Если одновременно указаны и параметры приложения и переменные окружения, приоритет будут иметь параметры приложения.

```bash
$ export $ALM_SMOOTHING=wittenbell

$ export $ALM_W-ARPA=./lm.arpa
```

---

## Примеры

![Пример работы программы](https://raw.githubusercontent.com/anyks/alm/master/site/img/screen1.png "Пример работы программы")

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
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing goodturing -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -path ./corpus -ext txt -w-bin ./lm.alm -w-bin-meta ./meta.json -w-bin-arpa -w-bin-utokens -w-bin-options -w-bin-preword -w-bin-badwords -w-bin-goodwords
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

### Пример прунинга arpa
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method prune -debug 1 -w-arpa ./lm2.arpa -allow-unk -interpolate -r-map ./lm.map -r-vocab ./lm.vocab -prune-threshold 0.003 -prune-max-gram 2
```

### Информация о бинарном контейнере
```bash
$ ./alm -r-bin ./lm.alm -r-bin-meta ./meta.json -method info
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

### Обучение с предобработкой полученных слов

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -path ./corpus -ext txt -word-script ./wordTest.py
```

> Иногда требуется изменить слово прежде чем оно будет добавлено в arpa - это можно сделать с помощью скрипта [**wordTest.py**](#user-content-формат-скрипта-python-предобработки-полученных-слов) в который будет передано слово и его контекст.

### Обучение с использованием своих токенов

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -path ./corpus -ext txt -utokens "usa|russia" -utoken-script ./utokenTest.py
```

> В примере добавлены свои токены **usa** и **russia**, при обработки текста все слова которые скрипт [**utokenTest.py**](#user-content-формат-скрипта-python-определения-собственных-токенов-слов) подтвердит как указанный токен, будут добавлены в arpa с названием токена.

### Обучение с белым списком

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -path ./corpus -ext txt -goodwords ./goodwords.txt
```

> Если указать белый список при обучении, все слова указанные в белом списке будут принудительно добавлены в arpa.

### Обучение с чёрным списком

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -path ./corpus -ext txt -badwords ./badwords.txt
```

> Если указать чёрный список при обучении, все слова указанные в чёрном списке будут приравнены к токену **〈unk〉**.

### Обучение с неизвестным словом

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -path ./corpus -ext txt -unknown-word goga
```

> В данном примере, токен **〈unk〉** в arpa будет заменён на слово указанное в параметре [-unknown-word | --unknown-word=〈value〉], в нашем случае на слово **goga**.

### Токенизация текста

**Формирование json файла из текста**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens -debug 1 -r-tokens-text ./text.txt -w-tokens-json ./tokens.json
```

**Исправление текстовых файлов**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens -debug 1 -r-tokens-text ./text.txt -w-tokens-text ./text.txt
```

**Формирование текста из json файла**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens -debug 1 -r-tokens-json ./tokens.json -w-tokens-text ./text.txt
```

**Формирование json файлов из группы текстов**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens -debug 1 -r-tokens-path ./path_text -w-tokens-path ./path_json -ext txt
```

**Формирование текстов из группы json файлов**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens -debug 1 -r-tokens-path ./path_json -w-tokens-path ./path_text -ext json
```

**Формирование json из строки**
```bash
$ echo 'Hello World?' | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens
```

**Формирование строки из json**
```bash
$ echo '[["Hello","World","?"]]' | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens
```

* * *

## Лицензия

![MIT License](http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png "MIT License")

Приложение лицензируется под [MIT License](http://opensource.org/licenses/MIT):

Авторские права © 2020 [Лобарев Юрий](https://anyks.com)

Данная лицензия разрешает лицам, получившим копию данного программного обеспечения и сопутствующей документации (в дальнейшем именуемыми «Программное Обеспечение»), безвозмездно использовать Программное Обеспечение без ограничений, включая неограниченное право на использование, копирование, изменение, слияние, публикацию, распространение, сублицензирование и/или продажу копий Программного Обеспечения, а также лицам, которым предоставляется данное Программное Обеспечение, при соблюдении следующих условий:

Указанное выше уведомление об авторском праве и данные условия должны быть включены во все копии или значимые части данного Программного Обеспечения.

ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ «КАК ЕСТЬ», БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ, ЯВНО ВЫРАЖЕННЫХ ИЛИ ПОДРАЗУМЕВАЕМЫХ, ВКЛЮЧАЯ ГАРАНТИИ ТОВАРНОЙ ПРИГОДНОСТИ, СООТВЕТСТВИЯ ПО ЕГО КОНКРЕТНОМУ НАЗНАЧЕНИЮ И ОТСУТСТВИЯ НАРУШЕНИЙ, НО НЕ ОГРАНИЧИВАЯСЬ ИМИ. НИ В КАКОМ СЛУЧАЕ АВТОРЫ ИЛИ ПРАВООБЛАДАТЕЛИ НЕ НЕСУТ ОТВЕТСТВЕННОСТИ ПО КАКИМ-ЛИБО ИСКАМ, ЗА УЩЕРБ ИЛИ ПО ИНЫМ ТРЕБОВАНИЯМ, В ТОМ ЧИСЛЕ, ПРИ ДЕЙСТВИИ КОНТРАКТА, ДЕЛИКТЕ ИЛИ ИНОЙ СИТУАЦИИ, ВОЗНИКШИМ ИЗ-ЗА ИСПОЛЬЗОВАНИЯ ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ ИЛИ ИНЫХ ДЕЙСТВИЙ С ПРОГРАММНЫМ ОБЕСПЕЧЕНИЕМ.

* * *

## Контакты

Если у вас есть вопросы относительно приложения, я бы хотел пригласить вас [на GitHub](https://github.com/anyks/alm/issues/new/choose). Пожалуйста, опишите ваш запрос, проблему или вопрос как можно более подробно, а также укажите версию библиотеки, которую вы используете, а также версию вашего компилятора и операционной системы. Открытие проблемы на GitHub позволяет другим пользователям и участникам этой библиотеки сотрудничать.

---

[Лобарев Юрий](https://anyks.com) <forman@anyks.com>
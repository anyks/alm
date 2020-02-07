# ANYKS Language Model (ALM)

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

## Основные зависимости

...

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

## Лицензия

**Edit a file, create a new file, and clone from Bitbucket in under 2 minutes**

When you're done, you can delete the content in this README and update the file with details for others getting started with your repository.

*We recommend that you open this README in another tab as you perform the tasks below. You can [watch our video](https://youtu.be/0ocf7u76WSo) for a full demo of all the steps in this tutorial. Open the video in a new tab to avoid leaving Bitbucket.*

---

## Edit a file

You’ll start by editing this README file to learn how to edit a file in Bitbucket.

1. Click **Source** on the left side.
2. Click the README.md link from the list of files.
3. Click the **Edit** button.
4. Delete the following text: *Delete this line to make a change to the README from Bitbucket.*
5. After making your change, click **Commit** and then **Commit** again in the dialog. The commit page will open and you’ll see the change you just made.
6. Go back to the **Source** page.

---

## Create a file

Next, you’ll add a new file to this repository.

1. Click the **New file** button at the top of the **Source** page.
2. Give the file a filename of **contributors.txt**.
3. Enter your name in the empty file space.
4. Click **Commit** and then **Commit** again in the dialog.
5. Go back to the **Source** page.

Before you move on, go ahead and explore the repository. You've already seen the **Source** page, but check out the **Commits**, **Branches**, and **Settings** pages.

---

## Clone a repository

Use these steps to clone from SourceTree, our client for using the repository command-line free. Cloning allows you to work on your files locally. If you don't yet have SourceTree, [download and install first](https://www.sourcetreeapp.com/). If you prefer to clone from the command line, see [Clone a repository](https://confluence.atlassian.com/x/4whODQ).

1. You’ll see the clone button under the **Source** heading. Click that button.
2. Now click **Check out in SourceTree**. You may need to create a SourceTree account or log in.
3. When you see the **Clone New** dialog in SourceTree, update the destination path and name if you’d like to and then click **Clone**.
4. Open the directory you just created to see your repository’s files.

Now that you're more familiar with your Bitbucket repository, go ahead and add a new file locally. You can [push your change back to Bitbucket with SourceTree](https://confluence.atlassian.com/x/iqyBMg), or you can [add, commit,](https://confluence.atlassian.com/x/8QhODQ) and [push from the command line](https://confluence.atlassian.com/x/NQ0zDQ).
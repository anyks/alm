[![ANYKS Smart language model](https://raw.githubusercontent.com/anyks/alm/master/site/img/banner.jpg)](https://anyks.com)

# ANYKS LM (ALM) C++11

- [Project goals and features](https://github.com/anyks/alm/#project-goals-and-features)
- [Requirements](https://github.com/anyks/alm/#requirements)
- [To build and launch the project](https://github.com/anyks/alm/#to-build-and-launch-the-project)
  - [Python version ALM](https://github.com/anyks/alm/#python-version-alm)
  - [To clone the project](https://github.com/anyks/alm/#to-clone-the-project)
  - [Build on Linux and FreeBSD](https://github.com/anyks/alm/#build-on-linux-and-freebsd)
  - [Build on MacOS X](https://github.com/anyks/alm/#build-on-macos-x)
- [Files formats](https://github.com/anyks/alm/#file-formats)
  - [Arpa](https://github.com/anyks/alm/#arpa)
  - [Ngrams](https://github.com/anyks/alm/#ngrams)
  - [Vocab](https://github.com/anyks/alm/#vocab)
  - [Map](https://github.com/anyks/alm/#map)
  - [File of adding n-gram into existing arpa file](https://github.com/anyks/alm/#file-of-adding-n-gram-into-existing-arpa-file)
  - [File of changing n-gram frequency in existing arpa file](https://github.com/anyks/alm/#file-of-changing-n-gram-frequency-in-existing-arpa-file)
  - [File of replacing n-gram in existing arpa file](https://github.com/anyks/alm/#file-of-replacing-n-gram-in-existing-arpa-file)
  - [File of similar letters in different dictionaries](https://github.com/anyks/alm/#file-of-similar-letters-in-different-dictionaries)
  - [File of removing n-gram from existing arpa file](https://github.com/anyks/alm/#file-of-removing-n-gram-from-existing-arpa-file)
  - [File of abbreviations list words](https://github.com/anyks/alm/#file-of-abbreviations-list-words)
  - [File of domain zones list](https://github.com/anyks/alm/#file-of-domain-zones-list)
  - [Binary container metadata](https://github.com/anyks/alm/#binary-container-metadata)
  - [The python script format to preprocess the received words](https://github.com/anyks/alm/#the-python-script-format-to-preprocess-the-received-words)
  - [The python script format to define the word features](https://github.com/anyks/alm/#the-python-script-format-to-define-the-word-features)
- [Environment variables](https://github.com/anyks/alm/#environment-variables)
- [Examples](https://github.com/anyks/alm/#examples)
  - [Language Model training example](https://github.com/anyks/alm/#language-model-training-example)
  - [Arpa patch example](https://github.com/anyks/alm/#arpa-patch-example)
  - [Example of removing n-grams with a frequency lower than backoff](https://github.com/anyks/alm/#example-of-removing-n-grams-with-a-frequency-lower-than-backoff)
  - [Example of merge raw data](https://github.com/anyks/alm/#example-of-merge-raw-data)
  - [Arpa pruning example](https://github.com/anyks/alm/#arpa-pruning-example)
  - [Vocab pruning example](https://github.com/anyks/alm/#vocab-pruning-example)
  - [An example of detecting and correcting words consisting of mixed dictionaries](https://github.com/anyks/alm/#an-example-of-detecting-and-correcting-words-consisting-of-mixed-dictionaries)
  - [Binary container information](https://github.com/anyks/alm/#binary-container-information)
  - [Arpa modification example](https://github.com/anyks/alm/#arpa-modification-example)
  - [Training with preprocessing of received words](https://github.com/anyks/alm/#training-with-preprocessing-of-received-words)
  - [Training using your own features](https://github.com/anyks/alm/#training-using-your-own-features)
  - [Example of disabling token identification](https://github.com/anyks/alm/#example-of-disabling-token-identification)
  - [An example of identifying tokens as 〈unk〉](https://github.com/anyks/alm/#an-example-of-identifying-tokens-as-unk)
  - [Training using whitelist](https://github.com/anyks/alm/#training-using-whitelist)
  - [Training using blacklist](https://github.com/anyks/alm/#training-using-blacklist)
  - [Training with an unknown word](https://github.com/anyks/alm/#training-with-an-unknown-word)
  - [Text tokenization](https://github.com/anyks/alm/#text-tokenization)
  - [Perplexity calculation](https://github.com/anyks/alm/#perplexity-calculation)
  - [Checking context in text](https://github.com/anyks/alm/#checking-context-in-text)
  - [Fix words case](https://github.com/anyks/alm/#fix-words-case)
  - [Check counts ngrams](https://github.com/anyks/alm/#check-counts-ngrams)
  - [Search ngrams by text](https://github.com/anyks/alm/#search-ngrams-by-text)
  - [Sentences generation](https://github.com/anyks/alm/#sentences-generation)
  - [Mixing language models](https://github.com/anyks/alm/#mixing-language-models)
- [License](https://github.com/anyks/alm/#license)
- [Contact](https://github.com/anyks/alm/#contact)

## Project goals and features

The are many toolkits capable of creating language models: ([KenLM](https://github.com/kpu/kenlm), [SriLM](https://github.com/BitMindLab/SRILM), [IRSTLM](https://github.com/irstlm-team/irstlm)), and each of those toolkits may have a reason to exist. But our language model creation toolkit has the following goals and features:

- **UTF-8 support**: Full UTF-8 support without third-party dependencies.
- **Support of many data formats**: Arpa, Vocab, Map Sequence, N-grams, Binary alm dictionary.
- **Smoothing algorithms**: Kneser-Nay, Modified Kneser-Nay, Witten-Bell, Additive, Good-Turing, Absolute discounting.
- **Normalisation and preprocessing for corpora**: Transferring corpus to lowercase, smart tokenization, ability to create black - and white - lists for n-grams.
- **Arpa modification**: Frequencies and n-grams replacing, adding new n-grams with frequencies, removing n-grams.
- **Pruning**: N-gram removal based on specified criteria.
- **Removal of low-probability n-grams**: Removal of n-grams which backoff probability is higher than standard probability.
- **Arpa recovery**: Recovery of damaged n-grams in arpa with subsequent recalculation of their backoff probabilities.
- **Support of additional word features**: Feature extraction: (numbers, roman numbers, ranges of numbers, numeric abbreviations, any other custom attributes) using scripts written in Python3.
- **Text preprocessing**: Unlike all other language model toolkits, ALM can extract correct context from files with unnormalized texts.
- **Unknown word token accounting**: Accounting of 〈unk〉 token as full n-gram.
- **Redefinition of 〈unk〉 token**: Ability to redefine an attribute of an unknown token.
- **N-grams preprocessing**: Ability to pre-process n-grams before adding them to arpa using custom Python3 scripts.
- **Binary container for Language Models**: The binary container supports compression, encryption and installation of copyrights.
- **Convenient visualization of the Language model assembly process**: ALM implements several types of visualizations: textual, graphic, process indicator, and logging to files or console.
- **Collection of all n-grams**: Unlike other language model toolkits, ALM is guaranteed to extract all possible n-grams from the corpus, regardless of their length (except for Modified Kneser-Nay); you can also force all n-grams to be taken into account even if they occured only once.

## Requirements

- [Zlib](http://www.zlib.net)
- [OpenSSL](https://www.openssl.org)
- [GperfTools](https://github.com/gperftools/gperftools)
- [Python3](https://www.python.org/download/releases/3.0)
- [NLohmann::json](https://github.com/nlohmann/json)
- [BigInteger](http://mattmccutchen.net/bigint)

## To build and launch the project

### Python version ALM
```bash
$ python3 -m pip install pybind11
$ python3 -m pip install anyks-lm
```

[Documentation pip](https://pypi.org/project/anyks-lm)

### To clone the project

```bash
$ git clone https://github.com/anyks/alm.git
```

### Build on Linux and FreeBSD

```bash
$ mkdir ./build
$ cd ./build

$ cmake ..
$ make
```

### Build on MacOS X

```bash
$ mkdir ./build
$ cd ./build

$ cmake -DOPENSSL_ROOT_DIR=/usr/local/Cellar/openssl@1.1/1.1.1g -DOPENSSL_LIBRARIES=/usr/local/Cellar/openssl@1.1/1.1.1g/include/lib ..
$ make
```

## File formats

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

| Frequency             | N-gram                       | Reverse frequency          |
|-----------------------|------------------------------|----------------------------|
| -1.328447             | после того                   | -0.477121                  |

#### Description:
 - **〈s〉** - Sentence beginning token
 - **〈/s〉** - Sentence end token
 - **〈url〉** - URL-address token
 - **〈num〉** - Number (arabic or roman) token
 - **〈unk〉** - Unknown word token
 - **〈time〉** - Time token **(15:44:56)**
 - **〈score〉** - Score count token **(4:3 | 01:04)**
 - **〈fract〉** - Fraction token **(5/20 | 192/864)**
 - **〈date〉** - Date token **(18.07.2004 | 07/18/2004)**
 - **〈abbr〉** - Abbreviation token **(1-й | 2-е | 20-я)**
 - **〈dimen〉** - Dimensions token **(200x300 | 1920x1080)**
 - **〈range〉** - Range of numbers token **(1-2 | 100-200 | 300-400)**
 - **〈aprox〉** - Approximate number token (**~93** | **~95.86** | **10~20**)
 - **〈anum〉** - Pseudo-number token (combination of numbers and other symbols) **(T34 | 895-M-86 | 39km)**
 - **〈pcards〉** - Symbols of the play cards **(♠ | ♣ | ♥ | ♦ )**
 - **〈punct〉** - Punctuation token **(. | , | ? | ! | : | ; | … | ¡ | ¿)**
 - **〈route〉** - Direction symbols (arrows) **(← | ↑ | ↓ | ↔ | ↵ | ⇐ | ⇑ | ⇒ | ⇓ | ⇔ | ◄ | ▲ | ► | ▼)**
 - **〈greek〉** - Symbols of the Greek alphabet **(Α | Β | Γ | Δ | Ε | Ζ | Η | Θ | Ι | Κ | Λ | Μ | Ν | Ξ | Ο | Π | Ρ | Σ | Τ | Υ | Φ | Χ | Ψ | Ω)**
 - **〈isolat〉** - Isolation/quotation token **(( | ) | [ | ] | { | } | " | « | » | „ | “ | ` | ⌈ | ⌉ | ⌊ | ⌋ | ‹ | › | ‚ | ’ | ′ | ‛ | ″ | ‘ | ” | ‟ | ' |〈 | 〉)**
 - **〈specl〉** - Special character token **(_ | @ | # | № | © | ® | & | ¦ | § | æ | ø | Þ | – | ‾ | ‑ | — | ¯ | ¶ | ˆ | ˜ | † | ‡ | • | ‰ | ⁄ | ℑ | ℘ | ℜ | ℵ | ◊ | \ )**
 - **〈currency〉** - Symbols of world currencies **($ | € | ₽ | ¢ | £ | ₤ | ¤ | ¥ | ℳ | ₣ | ₴ | ₸ | ₹ | ₩ | ₦ | ₭ | ₪ | ৳ | ƒ | ₨ | ฿ | ₫ | ៛ | ₮ | ₱ | ﷼ | ₡ | ₲ | ؋ | ₵ | ₺ | ₼ | ₾ | ₠ | ₧ | ₯ | ₢ | ₳ | ₥ | ₰ | ₿ | ұ)**
 - **〈math〉** - Mathematical operation token **(+ | - | = | / | * | ^ | × | ÷ | − | ∕ | ∖ | ∗ | √ | ∝ | ∞ | ∠ | ± | ¹ | ² | ³ | ½ | ⅓ | ¼ | ¾ | % | ~ | · | ⋅ | ° | º | ¬ | ƒ | ∀ | ∂ | ∃ | ∅ | ∇ | ∈ | ∉ | ∋ | ∏ | ∑ | ∧ | ∨ | ∩ | ∪ | ∫ | ∴ | ∼ | ≅ | ≈ | ≠ | ≡ | ≤ | ≥ | ª | ⊂ | ⊃ | ⊄ | ⊆ | ⊇ | ⊕ | ⊗ | ⊥ | ¨)**

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

| N-gram                | Occurrence in corpus         | Occurrence in documents    |
|-----------------------|------------------------------|----------------------------|
| только в одном        | 2                            | 1                          |

#### Description:

- **ad** - The number of documents in corpus
- **cw** - The number of words in all documents
- **unq** - The number of unique words collected in corpus

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

| Word Id               | Word      | Occurrence in corpus       | Occurrence in documents    | tf       | tf-idf   | wltf      |
|-----------------------|-----------|----------------------------|----------------------------|----------|----------|-----------|
| 2282345502            | новый     | 10                         | 1                          | 0.000420 | 0.000000 | -6.776199 |

#### Description:

- **oc** - Occurrence in corpus
- **dc** - Occurrence in documents
- **tf** - Term frequency — the ratio of a word occurrence to the total number of words in a document. Thus, the importance of a word is evaluated within a single document, calculation formula is: [tf = oc / cw]
- **idf** - Inverse document frequency for word, calculation formula: [idf = log(ad / dc)]
- **tf-idf** - It's calculated by the formula: [tf-idf = tf * idf]
- **wltf** - Word rating, calculation formula: [wltf = 1 + log(tf * dc)]

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

> This file is for technical use only. In combination with the **[vocab](https://github.com/anyks/alm#vocab)** file, you can combine several language models, modify, store, distribute and extract any formats ([arpa](https://github.com/anyks/alm#arpa), [ngrams](https://github.com/anyks/alm#ngrams), [vocab](https://github.com/anyks/alm#vocab), [alm](https://github.com/anyks/alm#binary-container-metadata)).

---

### File of adding n-gram into existing arpa file
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

| N-gram frequency      | Separator   | N-gram       |
|-----------------------|-------------|--------------|
| -0.988534             | \t          | у границ США |

---

### File of changing n-gram frequency in existing arpa file
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

| N-gram frequency      | Separator   | N-gram            |
|-----------------------|-------------|-------------------|
| -0.6588787            | \t          | мужчины и женщины |

---

### File of replacing n-gram in existing arpa file
```
коем случае нельзя	там да тут
но тем не	да ты что
неожиданный у	ожидаемый к
в СМИ	в ФСБ
Шах	Мат
...
```

| Existing N-gram       | Separator   | New N-gram        |
|-----------------------|-------------|-------------------|
| но тем не             | \t          | да ты что         |

---

### File of similar letters in different dictionaries
```
p  р
c  с
o  о
t  т
k  к
e  е
a  а
h  н
x  х
b  в
m  м
...
```

| Letter for search | Separator | Letter for replace |
|-------------------|-----------|--------------------|
| t                 | \t        | т                  |

---

### File of removing n-gram from existing arpa file
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

### File of abbreviations list words
```
г
р
США
ул
руб
рус
чел
...
```

> All words from this list will be identificate as an unknown word **〈abbr〉**.

---

### File of domain zones list
```
ru
su
cc
net
com
org
info
...
```

> For more accurate identification of the **〈url〉** token, you should add your own domain zones (all domain zones in the example are already pre-installed).

---

### The python script format to preprocess the received words
```python
# -*- coding: utf-8 -*-

def init():
    """
    Initialization Method: Runs only once at application startup
    """

def run(word, context):
    """
    Processing start method: starts when a word is extracted from text
    @word    word for processing
    @context sequence of previous words as an array
    """
    return word
```

---

### The python script format to define the word features
```python
# -*- coding: utf-8 -*-

def init():
    """
    Initialization Method: Runs only once at application startup
    """

def run(token, word):
    """
    Processing start method: starts when a word is extracted from text
    @token word token name
    @word  word for processing
    """
    if token and (token == "<usa>"):
        if word and (word.lower() == "usa"): return "ok"
    elif token and (token == "<russia>"):
        if word and (word.lower() == "russia"): return "ok"
    return "no"
```

---

### Environment variables

- All parameters can be passed through environment variables. Variables should begin with the prefix **ALM_** and must be written in upper case, their names should correspond to the application parameters.
- If both application parameters and environment variables are specified at the same time, application parameters will take precedence.

```bash
$ export ALM_SMOOTHING=wittenbell

$ export ALM_W-ARPA=./lm.arpa
```

- Example JSON format file

```json
{
  "alphabet": "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя",
  "smoothing": "wittenbell",
  "method": "train",
  "size": "3",
  "debug": "1",
  "w-arpa": "./lm.arpa",
  "w-map": "./lm.map",
  "w-vocab": "./lm.vocab",
  "w-ngram": "./lm.ngrams",
  "allow-unk": "true",
  "interpolate": "true",
  "corpus": "./text.txt"
}
```

---

## Examples

![Program operation example](https://raw.githubusercontent.com/anyks/alm/master/site/img/screen1.png "Program operation example")

### Language Model training example

**Smoothing Algorithm: Witten-Bell, single-file build by JSON**
```bash
$ ./alm -r-json ./meta.json
```

**Smoothing Algorithm: Witten-Bell, single-file build**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./text.txt
```

**Smoothing Algorithm: Absolute discounting, build from a group of files**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing cdiscount -discount 0.3 -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./corpus -ext txt
```

**Smoothing Algorithm: Additive, build from a group of files**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing addsmooth -delta 0.3 -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./corpus -ext txt
```

**Smoothing Algorithm: Kneser-Nay, build from a group of files**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing kneserney -kneserney-modified -kneserney-prepares -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./corpus -ext txt
```

**Smoothing Algorithm: Good-Turing, build from a group of files from binary container**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing goodturing -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./corpus -ext txt -w-bin ./lm.alm -bin-aes 128 -bin-password 911 -bin-name test -bin-lictype MIT -w-bin-arpa -w-bin-utokens -w-bin-options -w-bin-preword -w-bin-badwords -w-bin-goodwords
```

**Smoothing Algorithm: Witten-Bell, build from binary container**
```bash
$ ./alm -r-bin ./lm.alm -bin-aes 128 -bin-password 911 -method train -debug 1 -size 3 -smoothing wittenbell -w-arpa ./lm.arpa
```

### Arpa patch example

```bash
./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method repair -debug 1 -w-arpa ./lm2.arpa -allow-unk -interpolate -r-arpa ./lm1.arpa
```

### Example of removing n-grams with a frequency lower than backoff
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method sweep -debug 1 -w-arpa ./lm2.arpa -allow-unk -interpolate -r-arpa ./lm1.arpa
```

### Example of merge raw data
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method merge -debug 1 -r-map ./path -r-vocab ./path -w-map ./lm.map -w-vocab ./lm.vocab
```

### Arpa pruning example
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method aprune -debug 1 -w-arpa ./lm2.arpa -allow-unk -r-map ./lm.map -r-vocab ./lm.vocab -aprune-threshold 0.003 -aprune-max-gram 2
```

### Vocab pruning example

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method vprune -debug 1 -w-arpa ./lm2.arpa -allow-unk -w-vocab ./lm2.vocab -r-map ./lm.map -r-vocab ./lm.vocab -vprune-wltf -9.11
```

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method vprune -debug 1 -w-arpa ./lm2.arpa -allow-unk -w-vocab ./lm2.vocab -r-map ./lm.map -r-vocab ./lm.vocab -vprune-oc 5892
```

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method vprune -debug 1 -w-arpa ./lm2.arpa -allow-unk -w-vocab ./lm2.vocab -r-map ./lm.map -r-vocab ./lm.vocab -vprune-dc 624
```

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method vprune -debug 1 -w-arpa ./lm2.arpa -allow-unk -w-vocab ./lm2.vocab -r-map ./lm.map -r-vocab ./lm.vocab -vprune-oc 5892 -vprune-dc 624
```

> **Vocabulary pruning** - removes low-frequency words that are supposed to contain **errors/typos**. Pruning is done according to the threshold of the **wltf**, **oc** or **dc** parameters.

### An example of detecting and correcting words consisting of mixed dictionaries

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -reset-unk -interpolate -mixed-dicts -corpus ./text.txt -r-mix-restwords ./restwords.txt
```

> Words in the text that contain typos in the form of similar letters of the alphabet of another language will be corrected if there are letters to replace in [restwords.txt](https://github.com/anyks/alm/#file-of-similar-letters-in-different-dictionaries).

### Binary container information
```bash
$ ./alm -r-bin ./lm.alm -bin-aes 128 -bin-password 911 -method info
```

### Arpa modification example

**Adding n-gram to arpa**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method modify -modify emplace -modify-file ./app.txt -debug 1 -w-arpa ./lm.arpa -allow-unk -interpolate -r-map ./lm.map -r-vocab ./lm.vocab
```

**Changing n-gram frequencies in arpa**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method modify -modify change -modify-file ./chg.txt -debug 1 -w-arpa ./lm.arpa -allow-unk -interpolate -r-map ./lm.map -r-vocab ./lm.vocab
```

**Removing n-gram from arpa**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method modify -modify remove -modify-file ./rm.txt -debug 1 -w-arpa ./lm.arpa -allow-unk -interpolate -r-map ./lm.map -r-vocab ./lm.vocab
```

**Changing n-gram in arpa**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method modify -modify replace -modify-file ./rep.txt -debug 1 -w-arpa ./lm.arpa -allow-unk -interpolate -r-map ./lm.map -r-vocab ./lm.vocab
```

### Training with preprocessing of received words

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./corpus -ext txt -word-script ./wordTest.py
```

> Sometimes it is necessary to change a word before it is added to arpa - this can be done using the script [**wordTest.py**](https://github.com/anyks/alm#the-python-script-format-to-preprocess-the-received-words) the word and its context will be passed into script.

### Training using your own features

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./corpus -ext txt -utokens "usa|russia" -utoken-script ./utokenTest.py
```

> The example adds its own features **usa** and **russia**, when processing text all words, that script [**utokenTest.py**](https://github.com/anyks/alm#the-python-script-format-to-define-the-word-features) marks as feature, will be added to arpa with feature name.

### Example of disabling token identification

**Smoothing algorithm: Witten-Bell, assembly with disabled tokens**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -reset-unk -interpolate -tokens-disable "num|url|abbr|date|time|anum|math|rnum|specl|range|aprox|score|dimen|fract|punct|isolat" -corpus ./text.txt
```

> Here is the **rnum** token, which is a Roman number, but is not used as an independent token.

**Smoothing algorithm: Witten-Bell, assembly with all disabled tokens**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -reset-unk -interpolate -tokens-all-disable -corpus ./text.txt
```

> In the example, the identification of all tokens is disabled, disabled tokens will be added to arpa as separate words.

### An example of identifying tokens as 〈unk〉

**Smoothing algorithm: Witten-Bell, assembly with identification of tokens as 〈unk〉**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -reset-unk -interpolate -tokens-unknown "num|url|abbr|date|time|anum|math|rnum|specl|range|aprox|score|dimen|fract|punct|isolat" -corpus ./text.txt
```

> Here is the **rnum** token, which is a Roman number, but is not used as an independent token.

**Smoothing algorithm: Witten-Bell, assembly with identification of all tokens as 〈unk〉**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -reset-unk -interpolate -tokens-all-unknown -corpus ./text.txt
```

> The example identifies all tokens as как 〈unk〉.

### Training using whitelist

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./corpus -ext txt -goodwords ./goodwords.txt
```

> If you specify a whitelist during training, all words specified in the white list will be forcibly added to arpa.

### Training using blacklist

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./corpus -ext txt -badwords ./badwords.txt
```

> If you specify a black list during training, all the words indicated in the black list will be equated with the token **〈unk〉**.

### Training with an unknown word

```bash
./bin/alm.exe -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -size 3 -smoothing wittenbell -method train -debug 1 -w-arpa ./lm.arpa -w-map ./lm.map -w-vocab ./lm.vocab -w-ngram ./lm.ngrams -allow-unk -interpolate -corpus ./corpus -ext txt -unknown-word goga
```

> In this example the token **〈unk〉** in arpa will be replaced by the word specified in the parameter [-unknown-word | --unknown-word=〈value〉], in our case it's word **goga**.

### Text tokenization

**Generating json file from text**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens -debug 1 -r-tokens-text ./text.txt -w-tokens-json ./tokens.json
```

**Correction of text files**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens -debug 1 -r-tokens-text ./text.txt -w-tokens-text ./text.txt
```

**Generating text from json file**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens -debug 1 -r-tokens-json ./tokens.json -w-tokens-text ./text.txt
```

**Generating json files from a group of texts**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens -debug 1 -r-tokens-path ./path_text -w-tokens-path ./path_json -ext txt
```

**Generating texts from a group of json files**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens -debug 1 -r-tokens-path ./path_json -w-tokens-path ./path_text -ext json
```

**Generating json from text string**
```bash
$ echo 'Hello World?' | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens
```

**Generating text string from json**
```bash
$ echo '[["Hello","World","?"]]' | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method tokens
```

### Perplexity calculation
```bash
$ echo "неожиданно из подворотни в Олега ударил яркий прожектор патрульный трактор???с лязгом выкатился и остановился возле мальчика...." | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method ppl -debug 1 -r-arpa ./lm.arpa -confidence
```

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method ppl -debug 1 -r-arpa ./lm.arpa -confidence -r-text ./text.txt -threads 0
```

### Checking context in text
**Smart checking**
```bash
$ echo "<s> Сегодня сыграл и в Олега ударил яркий прожектор патрульный трактор с корпоративным сектором </s>" | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method checktext -debug 1 -r-arpa ./lm.arpa -confidence
```

**Accurate checking**
```bash
$ echo "<s> в Олега ударил яркий </s>" | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method checktext -debug 1 -r-arpa ./lm.arpa -confidence -accurate
```

**Checking by file**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method checktext -debug 1 -r-arpa ./lm.arpa -confidence -r-text ./text.txt -w-text ./checks.txt -threads 0
```

### Fix words case
```bash
$ echo "неожиданно из подворотни в Олега ударил яркий прожектор патрульный трактор???с лязгом выкатился и остановился возле мальчика...." | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method fixcase -debug 1 -r-arpa ./lm.arpa -confidence
```

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method fixcase -debug 1 -r-arpa ./lm.arpa -confidence -r-text ./text.txt -w-text ./fix.txt -threads 0
```

### Check counts ngrams
```bash
$ echo "неожиданно из подворотни в Олега ударил яркий прожектор патрульный трактор???с лязгом выкатился и остановился возле мальчика...." | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method counts -debug 1 -r-arpa ./lm.arpa -confidence
```

```bash
$ echo "неожиданно из подворотни в Олега ударил яркий прожектор патрульный трактор???с лязгом выкатился и остановился возле мальчика...." | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method counts -ngrams bigram -debug 1 -r-arpa ./lm.arpa -confidence
```

```bash
$ echo "неожиданно из подворотни в Олега ударил яркий прожектор патрульный трактор???с лязгом выкатился и остановился возле мальчика...." | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method counts -ngrams trigram -debug 1 -r-arpa ./lm.arpa -confidence
```

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method counts -ngrams bigram -debug 1 -r-arpa ./lm.arpa -confidence -r-text ./text.txt -w-text ./counts.txt -threads 0
```

### Search ngrams by text

```bash
$ echo "Особое место занимает чудотворная икона Лобзание Христа Иудою" | ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method find -debug 1 -r-arpa ./lm.arpa -confidence
```

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method find -debug 1 -r-arpa ./lm.arpa -confidence -r-text ./text.txt -w-text ./found.txt -threads 0
```

### Sentences generation

```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method sentences -gen 5 -debug 1 -r-arpa ./lm.arpa -confidence -w-text ./sentences.txt
```

### Mixing language models

**Static mixing**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method mix -mix static -debug 1 -r-arpa ./lm1.arpa -mix-arpa1 ./lm2.arpa -mix-lambda1 0.5 -w-arpa ./lm.arpa -confidence -mix-backward
```

**Bayes mixing**
```bash
$ ./alm -alphabet "abcdefghijklmnopqrstuvwxyzабвгдеёжзийклмнопрстуфхцчшщъыьэюя" -method mix -mix bayes -debug 1 -r-arpa ./lm1.arpa -mix-arpa1 ./lm2.arpa -mix-lambda1 0.5 -w-arpa ./lm.arpa -confidence -mix-bayes-scale 0.5 -mix-bayes-length 3
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

If you have questions regarding the library, I would like to invite you to [open an issue at GitHub](https://github.com/anyks/alm/issues/new/choose). Please describe your request, problem, or question as detailed as possible, and also mention the version of the library you are using as well as the version of your compiler and operating system. Opening an issue at GitHub allows other users and contributors to this library to collaborate.

---

[Yuriy Lobarev](https://anyks.com) <forman@anyks.com>

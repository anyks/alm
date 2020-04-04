/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_LANGUAGE_MODEL_
#define _ANYKS_LANGUAGE_MODEL_

/**
 * Стандартная библиотека
 */
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <bitset>
#include <vector>
#include <limits>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <sys/stat.h>
/**
 * Наши модули
 */
#include <fsys.hpp>
#include <word.hpp>
#include <python.hpp>
#include <general.hpp>
#include <alphabet.hpp>
#include <tokenizer.hpp>

// Устанавливаем область видимости
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Alm Класс модуля языковой модели
	 */
	typedef class Alm {
		private:
			/**
			 * UserToken Структура пользовательского токена
			 */
			typedef struct UserToken {
				/*
				 * Идентификатор слова
				 */
				size_t idw;
				/*
				 * Название токена
				 */
				word_t name;
				/*
				 * Функция проверки
				 */
				function <bool (const string &, const string &)> test;
				/**
				 * UserToken Конструктор
				 */
				UserToken() : idw(idw_t::NIDW), name("") {}
			} utoken_t;
			/**
			 * Структура словаря языковой модели
			 */
			typedef struct Arpa: std::map <size_t, Arpa> {
				/**
				 * Регистры слова
				 */
				size_t uppers;
				/**
				 * Частота последовательности и
				 * обратная частота последовательности
				 */
				float weight, backoff;
				/**
				 * Arpa Конструктор
				 */
				Arpa() : uppers(0), weight(log(0)), backoff(0.0f) {}
			} arpa_t;
			/**
			 * Структура слова для карты последовательности
			 */
			typedef struct Seq {
				/**
				 * Частота n-граммы и обратная частота
				 */
				float weight, backoff;
				/**
				 * Идентификатор слова, встречаемость n-граммы,
				 * количество документов где встретилась n-грамма,
				 * значение регистров слова
				 */
				size_t idw, oc, dc, ups;
				/**
				 * Seq Конструктор
				 */
				Seq() : weight(log(0)), backoff(0.0f), idw(idw_t::NIDW), oc(0), dc(0), ups(0) {}
			} __attribute__((packed)) seq_t;
		public:
			// Основные опции
			enum class options_t : u_short {
				debug,     // Флаг режима отладки
				onlyGood,  // Флаг использования только слов из белого списка
				mixdicts,  // Флаг разрешающий детектировать слова из смешанных словарей
				confidence // Флаг разрешающий загружать n-граммы из arpa так-как они есть
			};
			/**
			 * Perplexity Структура результатов перплексии
			 */
			typedef struct Perplexity {
				size_t oovs;              // Количество неизвестных слов
				size_t words;             // Количество слов в расчёте
				size_t sentences;         // Количество предложений
				size_t zeroprobs;         // Количество нулевых проб
				float logprob, ppl, ppl1; // Значение перплексии
				/**
				 * Perplexity Конструктор
				 */
				Perplexity() : oovs(0), words(0), sentences(0), zeroprobs(0), logprob(0.0f), ppl(idw_t::NIDW), ppl1(idw_t::NIDW) {}
			} ppl_t;
			// Упрощаем тип функции для получения слова
			typedef function <const word_t * (const size_t)> words_t;
		private:
			// Нулевое значение логорифма
			const float zero = log(0);
			// Максимальное значение 10-го логорифма
			const float mln10 = 2.30258509299404568402;
		private:
			// Максимальный размер n-граммы
			u_short size = 1;
			// Замена неизвестному слову
			size_t unknown = 0;
			// Флаг запрещающий очистку объект питона
			bool notCleanPython = false;
		private:
			// Флаги параметров
			bitset <4> options;
			// Список плохих слов
			std::set <size_t> badwords;
			// Список хороших слов
			std::set <size_t> goodwords;
			// Список токенов приводимых к <unk>
			std::set <token_t> tokenUnknown;
			// Список запрещённых токенов
			std::set <token_t> tokenDisable;
			// Список пользовательских токенов
			std::map <size_t, utoken_t> utokens;
			// Список скриптов python
			std::map <u_short, pair <string, size_t>> scripts;
		private:
			// Словарь языковой модели
			mutable arpa_t arpa;
			// Мютекс блокировки потока
			mutable recursive_mutex locker;
			// Словарь всех слов в системе
			mutable std::map <size_t, word_t> vocab;
		private:
			// Функция извлечения слова по его идентификатору
			words_t getWord = nullptr;
			// Объект работы с python
			python_t * python = nullptr;
			// Объект log файла
			const char * logfile = nullptr;
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
			// Объект токенизатора
			const tokenizer_t * tokenizer = nullptr;
		private:
			/**
			 * event Метод проверки на спец-слово
			 * @param idw идентификатор слова для проверки
			 * @return    результат проверки
			 */
			const bool event(const size_t idw) const noexcept;
			/**
			 * findWord Метод поиска слова в словаре
			 * @param idw идентификатор слова
			 * @return    найденное слово в словаре
			 */
			const word_t * findWord(const size_t idw) const noexcept;
			/**
			 * word Метод извлечения слова по его идентификатору
			 * @param idw идентификатор слова
			 * @param ups регистры слова
			 * @return    слово соответствующее идентификатору
			 */
			const word_t word(const size_t idw, const size_t ups = 0) const noexcept;
		private:
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq последовательность слов для установки
			 */
			void set(const vector <seq_t> & seq) const noexcept;
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq     список идентификаторов слов которые нужно добавить
			 * @param weight  вес n-граммы из файла arpa
			 * @param backoff обратная частота документа из файла arpa
			 */
			void set(const vector <pair_t> & seq, const float weight, const float backoff) const noexcept;
			/**
			 * clearShielding Функция удаления экранирования
			 * @param word  слово в котором следует удалить экранирование
			 * @param front первый символ экранирования
			 * @param back  последний символ экранирования
			 */
			void clearShielding(const string & word, const string & front = "<", const string & back = ">") const noexcept;
		private:
			/**
			 * weight Метод извлечения веса последовательности
			 * @param seq последовательность для извлечения веса
			 * @return    вес последовательноси и n-грамма для которой она получена
			 */
			const pair <u_short, float> weight(const vector <size_t> & seq) const noexcept;
			/**
			 * frequency Метод извлечения частоты n-граммы
			 * @param seq список слов последовательности
			 * @return    частота и обратная частота n-граммы
			 */
			const pair <float, float> frequency(const vector <size_t> & seq) const noexcept;
			/**
			 * pplCalculate Метод расчёта значения перплексии по весу
			 * @param logprob вес последовательности n-грамм полученный из расчётов
			 * @param wirds   список слов используемых при расчёте
			 * @param oovs    список неизвестных слов используемых при расчётах
			 * @return        значение перплексии полученное при расчётах
			 */
			const pair <float, float> pplCalculate(const float logprob, const size_t words, const size_t oovs) const noexcept;
		private:
			/**
			 * isOption Метод проверки наличия опции
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isOption(const options_t option) const noexcept;
			/**
			 * backoff Метод извлечения обратной частоты последовательности
			 * @param seq последовательность для извлечения обратной частоты
			 * @return    обратная частота последовательности
			 */
			const float backoff(const vector <size_t> & seq) const noexcept;
			/**
			 * getIdw Метод генерирования идентификатора слова
			 * @param  word  слово для генерации
			 * @param  check нужно выполнить дополнительную проверку слова
			 * @return       идентификатор слова
			 */
			const size_t getIdw(const wstring & word, const bool check = true) const noexcept;
		public:
			/**
			 * perplexity Метод расчёта перплексии текста
			 * @param  text текст для расчёта
			 * @return      результат расчёта
			 */
			const ppl_t perplexity(const string & text) const noexcept;
			/**
			 * perplexity Метод расчёта перплексии текста
			 * @param  text текст для расчёта
			 * @return      результат расчёта
			 */
			const ppl_t perplexity(const wstring & text) const noexcept;
			/**
			 * perplexity Метод расчёта перплексии
			 * @param  seq список последовательностей
			 * @return     результат расчёта
			 */
			const ppl_t perplexity(const vector <size_t> & seq) const noexcept;
			/**
			 * pplConcatenate Метод объединения перплексий
			 * @param ppl1 первая перплексия
			 * @param ppl2 вторая перплексия
			 * @return     объединённая перплексия
			 */
			const ppl_t pplConcatenate(const ppl_t & ppl1, const ppl_t & ppl2) const noexcept;
			/**
			 * pplByFiles Метод чтения расчёта перплексии по файлу или группе файлов
			 * @param path   адрес каталога или файла для расчёта перплексии
			 * @param status функция вывода статуса чтения
			 * @param ext    расширение файлов в каталоге (если адрес передан каталога)
			 * @return       результат расчёта
			 */
			const ppl_t pplByFiles(const string & path, function <void (const u_short)> status = nullptr, const string & ext = "txt") const noexcept;
		public:
			/**
			 * check Метод проверки существования последовательности
			 * @param text текст для проверки существования
			 * @return     результат проверки
			 */
			const pair <bool, size_t> check(const string & text) const noexcept;
			/**
			 * check Метод проверки существования последовательности
			 * @param text текст для проверки существования
			 * @return     результат проверки
			 */
			const pair <bool, size_t> check(const wstring & text) const noexcept;
			/**
			 * check Метод проверки существования последовательности
			 * @param seq список слов последовательности
			 * @return    результат проверки
			 */
			const pair <bool, size_t> check(const vector <size_t> & seq) const noexcept;
		public:
			/**
			 * fixUppers Метод исправления регистров в тексте
			 * @param text текст для исправления регистров
			 * @return     текст с исправленными регистрами слов
			 */
			const string fixUppers(const string & text) const noexcept;
			/**
			 * fixUppers Метод исправления регистров в тексте
			 * @param text текст для исправления регистров
			 * @return     текст с исправленными регистрами слов
			 */
			const wstring fixUppers(const wstring & text) const noexcept;
			/**
			 * context Метод сборки текстового контекста из последовательности
			 * @param seq  последовательность слов для сборки контекста
			 * @param nwrd флаг разрешающий вывод системных токенов
			 * @return     собранный текстовый контекст
			 */
			const wstring context(const vector <size_t> & seq, const bool nwrd = false) const noexcept;
		public:
			/**
			 * clear Метод очистки
			 */
			void clear();
			/**
			 * initPython Метод инициализации скриптов питона
			 */
			void initPython();
			/**
			 * clearBadwords Метод очистки списка плохих слов
			 */
			void clearBadwords() noexcept;
			/**
			 * clearGoodwords Метод очистки списка хороших слов
			 */
			void clearGoodwords() noexcept;
			/**
			 * clearUserTokens Метод очистки пользовательских токенов
			 */
			void clearUserTokens() noexcept;
		public:
			/**
			 * setAllTokenUnknown Метод установки всех токенов идентифицируемых как <unk>
			 */
			void setAllTokenUnknown() noexcept;
			/**
			 * setAllTokenDisable Метод установки всех токенов как не идентифицируемых
			 */
			void setAllTokenDisable() noexcept;
		public:
			/**
			 * addBadword Метод добавления идентификатора похого слова в список
			 * @param idw идентификатор слова
			 */
			void addBadword(const size_t idw) noexcept;
			/**
			 * addGoodword Метод добавления идентификатора хорошего слова в список
			 * @param idw идентификатор слова
			 */
			void addGoodword(const size_t idw) noexcept;
			/**
			 * addBadword Метод добавления похого слова в список
			 * @param word слово для добавления
			 */
			void addBadword(const string & word) noexcept;
			/**
			 * addGoodword Метод добавления хорошего слова в список
			 * @param word слово для добавления
			 */
			void addGoodword(const string & word) noexcept;
		public:
			/**
			 * setWordMethod Метод установки функции получения слова
			 * @param word функция получения слова
			 */
			void setWordMethod(words_t word) noexcept;
			/**
			 * setUnknown Метод установки неизвестного слова
			 * @param word слово для добавления
			 */
			void setUnknown(const string & word) noexcept;
			/**
			 * setOptions Метод установки опций
			 * @param options опции для установки
			 */
			void setOptions(const u_int options) noexcept;
			/**
			 * setPythonObj Метод установки внешнего объекта питона
			 * @param python внешний объект питона
			 */
			void setPythonObj(python_t * python) noexcept;
			/**
			 * setLogfile Метод установка файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile) noexcept;
			/**
			 * setUserToken Метод добавления токена пользователя
			 * @param name слово - обозначение токена
			 */
			void setUserToken(const string & name) noexcept;
			/**
			 * setOption Метод установки опций модуля
			 * @param option опция для установки
			 */
			void setOption(const options_t option) noexcept;
			/**
			 * unsetOption Метод отключения опции модуля
			 * @param option опция для отключения
			 */
			void unsetOption(const options_t option) noexcept;
			/**
			 * setWordScript Метод установки скрипта обработки слов
			 * @param script скрипт python обработки слов
			 */
			void setWordScript(const string & script) noexcept;
			/**
			 * setVocab Метод загрузки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 * @param arpa   нужно добавить только данные arpa
			 */
			void setVocab(const vector <char> & buffer) noexcept;
			/**
			 * setTokenUnknown Метод установки списка токенов которых нужно идентифицировать как <unk>
			 * @param options список токенов которых нужно идентифицировать как <unk>
			 */
			void setTokenUnknown(const string & options) noexcept;
			/**
			 * setTokenDisable Метод установки списка не идентифицируемых токенов
			 * @param options список не идентифицируемых токенов
			 */
			void setTokenDisable(const string & options) noexcept;
			/**
			 * setBin Метод установки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 */
			void setBin(const vector <char> & buffer) const noexcept;
			/**
			 * setUserTokenScript Метод установки скрипта обработки пользовательских токенов
			 * @param script скрипт python обработки пользовательских токенов
			 */
			void setUserTokenScript(const string & script) noexcept;
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet) noexcept;
			/**
			 * setTokenizer Метод установки токенизатора
			 * @param tokenizer объект токенизатора
			 */
			void setTokenizer(const tokenizer_t * tokenizer) noexcept;
			/**
			 * setTokensUnknown Метод установки списка токенов приводимых к <unk>
			 * @param tokens список токенов для установки
			 */
			void setTokensUnknown(const std::set <token_t> & tokens) noexcept;
			/**
			 * setTokensDisable Метод установки списка запрещённых токенов
			 * @param tokens список токенов для установки
			 */
			void setTokensDisable(const std::set <token_t> & tokens) noexcept;
			/**
			 * setBadwords Метод установки списка идентификаторов плохих слов в список
			 * @param badwords список идентификаторов плохих слов
			 */
			void setBadwords(const std::set <size_t> & badwords) noexcept;
			/**
			 * setBadwords Метод установки списка плохих слов в список
			 * @param badwords список плохих слов
			 */
			void setBadwords(const vector <string> & badwords) noexcept;
			/**
			 * setGoodwords Метод установки списка идентификаторов хороших слов в список
			 * @param goodwords список идентификаторов хороших слов
			 */
			void setGoodwords(const std::set <size_t> & goodwords) noexcept;
			/**
			 * setGoodwords Метод установки списка хороших слов в список
			 * @param goodwords список хороших слов
			 */
			void setGoodwords(const vector <string> & goodwords) noexcept;
			/**
			 * getUppers Метод извлечения регистров для каждого слова
			 * @param seq  последовательность слов для сборки контекста
			 * @param upps список извлечённых последовательностей
			 */
			void getUppers(const vector <size_t> & seq, vector <size_t> & upps) const noexcept;
			/**
			 * getBin Метод извлечения данных arpa в бинарном виде
			 * @param callback функция обратного вызова
			 */
			void getBin(function <void (const vector <char> &, const u_short)> callback) const noexcept;
			/**
			 * getVocab Метод извлечения данных словаря в бинарном виде
			 * @param callback функция обратного вызова
			 */
			void getVocab(function <void (const vector <char> &, const u_short)> callback) const noexcept;
			/**
			 * read Метод чтения данных из файла arpa
			 * @param filename адрес файла для чтения
			 * @param status   функция вывода статуса чтения
			 */
			void read(const string & filename, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * setUserTokenMethod Метод добавления функции обработки пользовательского токена
			 * @param name слово - обозначение токена
			 * @param fn   внешняя функция обрабатывающая пользовательский токен
			 */
			void setUserTokenMethod(const string & name, function <bool (const string &, const string &)> fn) noexcept;
		public:
			/**
			 * getSize Метод получения размера n-грамы
			 * @return длина n-граммы в языковой моделе
			 */
			const u_short getSize() const;
			/**
			 * getOptions Метод извлечения установленных опций
			 * @return установленные опции
			 */
			const u_int getOptions() const noexcept;
			/**
			 * getUnknown Метод извлечения неизвестного слова
			 * @return установленное неизвестное слово
			 */
			const string & getUnknown() const noexcept;
			/**
			 * getWordScript Метод извлечения скрипта обработки слов
			 * @return адрес скрипта python обработки слов
			 */
			const string & getWordScript() const noexcept;
			/**
			 * getUserTokenScript Метод извлечения скрипта обработки пользовательских токенов
			 * @return адрес скрипта python обработки пользовательских токенов
			 */
			const string & getUserTokenScript() const noexcept;
			/**
			 * getBadwords Метод извлечения чёрного списка
			 * @return чёрный список слов
			 */
			const std::set <size_t> & getBadwords() const noexcept;
			/**
			 * getGoodwords Метод извлечения белого списка
			 * @return белый список слов
			 */
			const std::set <size_t> & getGoodwords() const noexcept;
			/**
			 * getTokensUnknown Метод извлечения списка токенов приводимых к <unk>
			 * @return список токенов
			 */
			const std::set <token_t> & getTokensUnknown() const noexcept;
			/**
			 * getTokensDisable Метод извлечения списка запрещённых токенов
			 * @return список токенов
			 */
			const std::set <token_t> & getTokensDisable() const noexcept;
			/**
			 * getUserTokens Метод извлечения списка пользовательских токенов
			 * @return список пользовательских токенов
			 */
			const vector <string> & getUserTokens() const noexcept;
			/**
			 * grams Метод проверки количества найденных в тексте n-грамм
			 * @param text текст для расчёта
			 * @return     количество найденных n-грамм
			 */
			const size_t grams(const string & text) const noexcept;
			/**
			 * grams Метод проверки количества найденных n-грамм
			 * @param seq список последовательностей
			 * @return    количество найденных n-грамм
			 */
			const size_t grams(const vector <size_t> & seq) const noexcept;
			/**
			 * bigrams Метод проверки количества найденных в тексте биграмм
			 * @param text текст для расчёта
			 * @return     количество найденных биграмм
			 */
			const size_t bigrams(const string & text) const noexcept;
			/**
			 * bigrams Метод проверки количества найденных биграмм
			 * @param seq список последовательностей
			 * @return    количество найденных биграмм
			 */
			const size_t bigrams(const vector <size_t> & seq) const noexcept;
			/**
			 * trigrams Метод проверки количества найденных в тексте триграмм
			 * @param text текст для расчёта
			 * @return     количество найденных триграмм
			 */
			const size_t trigrams(const string & text) const noexcept;
			/**
			 * trigrams Метод проверки количества найденных последовательных триграмм
			 * @param seq список последовательностей
			 * @return    количество найденных триграмм
			 */
			const size_t trigrams(const vector <size_t> & seq) const noexcept;
			/**
			 * getUserTokenWord Метод получения пользовательского токена по его идентификатору
			 * @param idw идентификатор пользовательского токена
			 * @return    пользовательский токен соответствующий идентификатору
			 */
			const string getUserTokenWord(const size_t idw) const noexcept;
			/**
			 * getUserTokenId Метод получения идентификатора пользовательского токена
			 * @param name слово для которого нужно получить идентификатор
			 * @return     идентификатор пользовательского токена соответствующий слову
			 */
			const size_t getUserTokenId(const string & name) const noexcept;
		public:
			/**
			 * Alm Конструктор
			 */
			Alm() noexcept {};
			/**
			 * Alm Конструктор
			 * @param alphabet объект алфавита
			 */
			Alm(const alphabet_t * alphabet) noexcept;
			/**
			 * Alm Конструктор
			 * @param tokenizer объект токенизатора
			 */
			Alm(const tokenizer_t * tokenizer) noexcept;
			/**
			 * Alm Конструктор
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 */
			Alm(const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept;
			/**
			 * ~Alm Деструктор
			 */
			~Alm() noexcept;
	} alm_t;
};

#endif // _ANYKS_LANGUAGE_MODEL_

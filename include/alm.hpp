/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_LANGUAGE_MODEL__
#define __ANYKS_LANGUAGE_MODEL__

/**
 * Стандартная библиотека
 */
#include <map>
#include <set>
#include <list>
#include <mutex>
#include <cmath>
#include <ctime>
#include <memory>
#include <atomic>
#include <bitset>
#include <vector>
#include <limits>
#include <fstream>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#include <sys/stat.h>
/**
 * Наши модули
 */
#include <fsys.hpp>
#include <word.hpp>
#include <python.hpp>
#include <alphabet.hpp>
#include <tokenizer.hpp>
#include <threadpool.hpp>
#include <app/alm.hpp>

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
			// Объявляем дружественные классы
			friend class Alm1;
			friend class Alm2;
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
				function <const bool (const string &, const string &)> test;
				/**
				 * UserToken Конструктор
				 */
				UserToken() : idw(idw_t::NIDW), name("") {}
			} utoken_t;
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
				size_t oovs;               // Количество неизвестных слов
				size_t words;              // Количество слов в расчёте
				size_t sentences;          // Количество предложений
				size_t zeroprobs;          // Количество нулевых проб
				double logprob, ppl, ppl1; // Значение перплексии
				/**
				 * Perplexity Конструктор
				 */
				Perplexity() : oovs(0), words(0), sentences(0), zeroprobs(0), logprob(0.0), ppl(idw_t::NIDW), ppl1(idw_t::NIDW) {}
			} ppl_t;
			/**
			 * Структура слова для карты последовательности
			 */
			typedef struct Seq {
				/**
				 * Частота n-граммы и обратная частота
				 */
				double weight, backoff;
				/**
				 * Идентификатор слова, встречаемость n-граммы,
				 * количество документов где встретилась n-грамма,
				 * значение регистров слова
				 */
				size_t idw, oc, dc, ups;
				/**
				 * Seq Конструктор
				 */
				Seq() : weight(log(0)), backoff(0.0), idw(idw_t::NIDW), oc(0), dc(0), ups(0) {}
			} seq_t;
			// Упрощаем тип функции для получения слова
			typedef function <const word_t * (const size_t)> words_t;
			// Упрощаем тип функции для добавления слова
			typedef function <void (const size_t, const word_t &)> addw_t;
			// Упрощаем тип функции предобработки слов
			typedef function <const string (const string &, const vector <string> &)> wpres_t;
		private:
			// Нулевое значение логорифма
			const double zero = log(0);
			// Максимальное значение 10-го логорифма
			const double mln10 = 2.30258509299404568402;
		private:
			// Максимальный размер n-граммы
			u_short size = 1;
			// Замена неизвестному слову
			size_t unknown = 0;
			// Флаг запрещающий очистку объект питона
			bool notCleanPython = false;
			// Количество потоков для работы
			size_t threads = thread::hardware_concurrency();
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
			// Создаем тредпул
			mutable tpool_t tpool;
			// Мютекс блокировки потока
			mutable recursive_mutex locker;
			// Словарь всех слов в системе
			mutable std::map <size_t, word_t> vocab;
		private:
			// Функция добавления слова в словарь
			addw_t addWord = nullptr;
			// Функция извлечения слова по его идентификатору
			words_t getWord = nullptr;
			// Функция предобработки слов
			wpres_t wordPress = nullptr;
			// Объект работы с python
			python_t * python = nullptr;
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
			// Объект токенизатора
			const tokenizer_t * tokenizer = nullptr;
		private:
			// Объект файла с oov словами
			const char * oovfile = nullptr;
			// Объект log файла
			mutable const char * logfile = nullptr;
		private:
			/**
			 * isOption Метод проверки наличия опции
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isOption(const options_t option) const noexcept;
			/**
			 * exist Метод проверки существования последовательности
			 * @param seq список слов последовательности
			 * @return    результат проверки
			 */
			virtual const bool exist(const vector <size_t> & seq) const noexcept;
		private:
			/**
			 * backoff Метод извлечения обратной частоты последовательности
			 * @param seq последовательность для извлечения обратной частоты
			 * @return    обратная частота последовательности
			 */
			virtual const double backoff(const vector <size_t> & seq) const noexcept;
			/**
			 * weight Метод извлечения веса последовательности
			 * @param seq последовательность для извлечения веса
			 * @return    вес последовательноси и n-грамма для которой она получена
			 */
			virtual const pair <u_short, double> weight(const vector <size_t> & seq) const noexcept;
			/**
			 * frequency Метод извлечения частоты n-граммы
			 * @param seq список слов последовательности
			 * @return    частота и обратная частота n-граммы
			 */
			virtual const pair <double, double> frequency(const vector <size_t> & seq) const noexcept;
		private:
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq последовательность слов для установки
			 */
			virtual void set(const vector <seq_t> & seq) const noexcept;
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq     список идентификаторов слов которые нужно добавить
			 * @param uppers  список верхних регистров последнего слова последовательности
			 * @param weight  вес n-граммы из файла arpa
			 * @param backoff обратная частота документа из файла arpa
			 */
			virtual void set(const vector <size_t> & seq, const size_t uppers, const double weight, const double backoff) const noexcept;
		private:
			/**
			 * clearShielding Функция удаления экранирования
			 * @param word  слово в котором следует удалить экранирование
			 * @param front первый символ экранирования
			 * @param back  последний символ экранирования
			 */
			void clearShielding(const string & word, const string & front = "<", const string & back = ">") const noexcept;
			/**
			 * pplCalculate Метод расчёта значения перплексии по весу
			 * @param logprob вес последовательности n-грамм полученный из расчётов
			 * @param wirds   список слов используемых при расчёте
			 * @param oovs    список неизвестных слов используемых при расчётах
			 * @return        значение перплексии полученное при расчётах
			 */
			const pair <double, double> pplCalculate(const double logprob, const size_t words, const size_t oovs) const noexcept;
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
			 * perplexity Метод расчёта перплексии текста
			 * @param  words список слов для расчёта
			 * @return       результат расчёта
			 */
			const ppl_t perplexity(const vector <wstring> & words) const noexcept;
			/**
			 * perplexity Метод расчёта перплексии
			 * @param  seq список последовательностей
			 * @return     результат расчёта
			 */
			virtual const ppl_t perplexity(const vector <size_t> & seq) const noexcept;
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
			 * @param status функция вывода статуса
			 * @param ext    расширение файлов в каталоге (если адрес передан каталога)
			 * @return       результат расчёта
			 */
			const ppl_t pplByFiles(const string & path, function <void (const u_short)> status = nullptr, const string & ext = "txt") const noexcept;
		public:
			/**
			 * check Метод проверки существования последовательности, с указанным шагом
			 * @param text текст для проверки существования
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			const bool check(const string & text, const u_short step) const noexcept;
			/**
			 * check Метод проверки существования последовательности, с указанным шагом
			 * @param text текст для проверки существования
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			const bool check(const wstring & text, const u_short step) const noexcept;
			/**
			 * check Метод проверки существования последовательности, с указанным шагом
			 * @param seq  список слов последовательности
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			const bool check(const vector <string> & seq, const u_short step) const noexcept;
			/**
			 * check Метод проверки существования последовательности, с указанным шагом
			 * @param seq  список слов последовательности
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			const bool check(const vector <wstring> & seq, const u_short step) const noexcept;
			/**
			 * check Метод проверки существования последовательности, с указанным шагом
			 * @param seq  список слов последовательности
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			virtual const bool check(const vector <size_t> & seq, const u_short step) const noexcept;
			/**
			 * check Метод проверки существования последовательности
			 * @param text     текст для проверки существования
			 * @param accurate режим точной проверки
			 * @return         результат проверки
			 */
			const pair <bool, size_t> check(const string & text, const bool accurate = false) const noexcept;
			/**
			 * check Метод проверки существования последовательности
			 * @param text     текст для проверки существования
			 * @param accurate режим точной проверки
			 * @return         результат проверки
			 */
			const pair <bool, size_t> check(const wstring & text, const bool accurate = false) const noexcept;
			/**
			 * check Метод проверки существования последовательности
			 * @param seq      список слов последовательности
			 * @param accurate режим точной проверки
			 * @return         результат проверки
			 */
			const pair <bool, size_t> check(const vector <string> & seq, const bool accurate = false) const noexcept;
			/**
			 * check Метод проверки существования последовательности
			 * @param seq      список слов последовательности
			 * @param accurate режим точной проверки
			 * @return         результат проверки
			 */
			const pair <bool, size_t> check(const vector <wstring> & seq, const bool accurate = false) const noexcept;
			/**
			 * check Метод проверки существования последовательности
			 * @param seq      список слов последовательности
			 * @param accurate режим точной проверки
			 * @return         результат проверки
			 */
			virtual const pair <bool, size_t> check(const vector <size_t> & seq, const bool accurate = false) const noexcept;
		public:
			/**
			 * exist Метод проверки существования последовательности
			 * @param text текст для проверки существования
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			const pair <bool, size_t> exist(const string & text, const u_short step) const noexcept;
			/**
			 * exist Метод проверки существования последовательности
			 * @param text текст для проверки существования
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			const pair <bool, size_t> exist(const wstring & text, const u_short step) const noexcept;
			/**
			 * exist Метод проверки существования последовательности
			 * @param seq  список слов последовательности
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			const pair <bool, size_t> exist(const vector <string> & seq, const u_short step) const noexcept;
			/**
			 * exist Метод проверки существования последовательности
			 * @param seq  список слов последовательности
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			const pair <bool, size_t> exist(const vector <wstring> & seq, const u_short step) const noexcept;
			/**
			 * exist Метод проверки существования последовательности
			 * @param seq  список слов последовательности
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			virtual const pair <bool, size_t> exist(const vector <size_t> & seq, const u_short step) const noexcept;
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
		public:
			/**
			 * setBin Метод установки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 */
			virtual void setBin(const vector <char> & buffer) const noexcept;
			/**
			 * getBin Метод извлечения данных arpa в бинарном виде
			 * @param callback функция обратного вызова
			 */
			virtual void getBin(function <void (const vector <char> &, const size_t, const u_short)> callback) const noexcept;
		public:
			/**
			 * clear Метод очистки всех данных
			 */
			void clear();
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
			 * setAllTokenUnknown Метод установки всех токенов идентифицируемых как <unk>
			 */
			void setAllTokenUnknown() noexcept;
			/**
			 * setAllTokenDisable Метод установки всех токенов как не идентифицируемых
			 */
			void setAllTokenDisable() noexcept;
		public:
			/**
			 * initPython Метод инициализации скриптов питона
			 */
			void initPython();
		public:
			/**
			 * setSize Метод установки размера n-граммы
			 * @param size размер n-граммы
			 */
			void setSize(const u_short size) noexcept;
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
			 * setLogfile Метод установки файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile) noexcept;
			/**
			 * setOOvFile Метод установки файла для сохранения OOV слов
			 * @param oovfile адрес файла для сохранения oov слов
			 */
			void setOOvFile(const char * oovfile) noexcept;
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
			 * setWordFn Метод установки функций получения и добавления слов
			 * @param fn1 функция получения слова
			 * @param fn2 функция добавления слова
			 */
			void setWordFn(words_t fn1, addw_t fn2) noexcept;
			/**
			 * unsetOption Метод отключения опции модуля
			 * @param option опция для отключения
			 */
			void unsetOption(const options_t option) noexcept;
			/**
			 * setThreads Метод установки количества потоков
			 * @param threads количество потоков для работы
			 */
			void setThreads(const size_t threads = 0) noexcept;
			/**
			 * setWordScript Метод установки скрипта обработки слов
			 * @param script скрипт python обработки слов
			 */
			void setWordScript(const string & script) noexcept;
			/**
			 * setWordPreprocessingMethod Метод установки функции препроцессинга слова
			 * @param fn внешняя функция препроцессинга слова
			 */
			void setWordPreprocessingMethod(wpres_t fn) noexcept;
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
			 * setUserTokenMethod Метод установки функции обработки пользовательского токена
			 * @param name слово - обозначение токена
			 * @param fn   внешняя функция обрабатывающая пользовательский токен
			 */
			void setUserTokenMethod(const string & name, function <const bool (const string &, const string &)> fn) noexcept;
		public:
			/**
			 * setVocab Метод загрузки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 * @param arpa   нужно добавить только данные arpa
			 */
			void setVocab(const vector <char> & buffer) noexcept;
			/**
			 * getVocab Метод извлечения данных словаря в бинарном виде
			 * @param callback функция обратного вызова
			 */
			void getVocab(function <void (const vector <char> &, const u_short)> callback) const noexcept;
		public:
			/**
			 * sentences Метод генерации предложений
			 * @param callback функция обратного вызова
			 */
			virtual void sentences(function <const bool (const wstring &)> callback) const noexcept;
			/**
			 * getUppers Метод извлечения регистров для каждого слова
			 * @param seq  последовательность слов для сборки контекста
			 * @param upps список извлечённых последовательностей
			 */
			virtual void getUppers(const vector <size_t> & seq, vector <size_t> & upps) const noexcept;
			/**
			 * context Метод сборки текстового контекста из последовательности
			 * @param seq  последовательность слов для сборки контекста
			 * @param nwrd флаг разрешающий вывод системных токенов
			 * @return     собранный текстовый контекст
			 */
			virtual const wstring context(const vector <size_t> & seq, const bool nwrd = false) const noexcept;
		public:
			/**
			 * find Метод поиска n-грамм в тексте
			 * @param text     текст в котором необходимо найти n-граммы
			 * @param callback функция обратного вызова
			 */
			void find(const string & text, function <void (const string &)> callback) const noexcept;
			/**
			 * find Метод поиска n-грамм в тексте
			 * @param text     текст в котором необходимо найти n-граммы
			 * @param callback функция обратного вызова
			 */
			virtual void find(const wstring & text, function <void (const wstring &)> callback) const noexcept;
		public:
			/**
			 * read Метод чтения данных из файла arpa
			 * @param filename адрес файла для чтения
			 * @param status   функция вывода статуса
			 */
			void read(const string & filename, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * sentencesToFile Метод сборки указанного количества предложений и записи в файл
			 * @param counts   количество предложений для сборки
			 * @param filename адрес файла для записи результата
			 * @param status   функция вывода статуса
			 */
			void sentencesToFile(const u_short counts, const string & filename, function <void (const u_short)> status = nullptr) const noexcept;
			/**
			 * findByFiles Метод поиска n-грамм в текстовом файле
			 * @param path     адрес каталога или файла для обработки
			 * @param filename адрес файла для записи результата
			 * @param status   функция вывода статуса
			 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
			 */
			void findByFiles(const string & path, const string & filename, function <void (const string &, const u_short)> status = nullptr, const string & ext = "txt") const noexcept;
			/**
			 * fixUppersByFiles Метод исправления регистров текста в текстовом файле
			 * @param path     адрес каталога или файла для обработки
			 * @param filename адрес файла для записи результата
			 * @param status   функция вывода статуса
			 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
			 */
			void fixUppersByFiles(const string & path, const string & filename, function <void (const string &, const u_short)> status = nullptr, const string & ext = "txt") const noexcept;
			/**
			 * countsByFiles Метод подсчёта количества n-грамм в текстовом файле
			 * @param path     адрес каталога или файла для обработки
			 * @param filename адрес файла для записи результата
			 * @param ngrams   размер n-граммы для подсчёта
			 * @param status   функция вывода статуса
			 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
			 */
			void countsByFiles(const string & path, const string & filename, const u_short ngrams = 0, function <void (const string &, const u_short)> status = nullptr, const string & ext = "txt") const noexcept;
		public:
			/**
			 * checkByFiles Метод проверки существования последовательности в текстовом файле
			 * @param path     адрес каталога или файла для обработки
			 * @param filename адрес файла для записи результата
			 * @param step     шаг размера N-граммы для перебора текста
			 * @param status   функция вывода статуса
			 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
			 */
			void checkByFiles(const string & path, const string & filename, const u_short step, function <void (const string &, const u_short)> status = nullptr, const string & ext = "txt") const noexcept;
			/**
			 * checkByFiles Метод проверки существования последовательности в текстовом файле
			 * @param path     адрес каталога или файла для обработки
			 * @param filename адрес файла для записи результата
			 * @param accurate режим точной проверки
			 * @param status   функция вывода статуса
			 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
			 */
			void checkByFiles(const string & path, const string & filename, const bool accurate = false, function <void (const string &, const u_short)> status = nullptr, const string & ext = "txt") const noexcept;
		public:
			/**
			 * getSize Метод получения размера n-грамы
			 * @return длина n-граммы в языковой моделе
			 */
			const u_short getSize() const noexcept;
			/**
			 * getOptions Метод извлечения установленных опций
			 * @return установленные опции
			 */
			const u_short getOptions() const noexcept;
			/**
			 * getUserTokenId Метод получения идентификатора пользовательского токена
			 * @param name слово для которого нужно получить идентификатор
			 * @return     идентификатор пользовательского токена соответствующий слову
			 */
			const size_t getUserTokenId(const string & name) const noexcept;
			/**
			 * getIdw Метод генерирования идентификатора слова
			 * @param  word  слово для генерации
			 * @param  check нужно выполнить дополнительную проверку слова
			 * @return       идентификатор слова
			 */
			const size_t getIdw(const wstring & word, const bool check = true) const noexcept;
		public:
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
			 * getUserTokenWord Метод получения пользовательского токена по его идентификатору
			 * @param idw идентификатор пользовательского токена
			 * @return    пользовательский токен соответствующий идентификатору
			 */
			const string getUserTokenWord(const size_t idw) const noexcept;
		public:
			/**
			 * word Метод извлечения слова по его идентификатору
			 * @param idw идентификатор слова
			 * @param ups регистры слова
			 * @return    слово соответствующее идентификатору
			 */
			const word_t word(const size_t idw, const size_t ups = 0) const noexcept;
		public:
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
		public:
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
			 * grams Метод проверки количества найденных n-грамм
			 * @param words список слов для расчёта
			 * @return      количество найденных n-грамм
			 */
			const size_t grams(const vector <wstring> & words) const noexcept;
		public:
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
			 * bigrams Метод проверки количества найденных биграмм
			 * @param words список слов для расчёта
			 * @return      количество найденных биграмм
			 */
			const size_t bigrams(const vector <wstring> & words) const noexcept;
		public:
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
			 * trigrams Метод проверки количества найденных последовательных триграмм
			 * @param words список слов для расчёта
			 * @return      количество найденных триграмм
			 */
			const size_t trigrams(const vector <wstring> & words) const noexcept;
		public:
			/**
			 * Alm Конструктор
			 */
			Alm() noexcept;
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
			virtual ~Alm() noexcept;
	} alm_t;
	/**
	 * Alm1 Класс alm первого типа
	 */
	typedef class Alm1 : public Alm {
		private:
			/**
			 * Структура словаря языковой модели
			 */
			typedef struct Arpa : std::map <size_t, Arpa> {
				/**
				 * Регистры слова
				 */
				size_t uppers;
				/**
				 * Частота последовательности и
				 * обратная частота последовательности
				 */
				double weight, backoff;
				/**
				 * Arpa Конструктор
				 */
				Arpa() : uppers(0), weight(log(0)), backoff(0.0) {}
			} arpa_t;
			// Словарь языковой модели
			mutable arpa_t arpa;
		private:
			/**
			 * exist Метод проверки существования последовательности
			 * @param seq список слов последовательности
			 * @return    результат проверки
			 */
			virtual const bool exist(const vector <size_t> & seq) const noexcept;
			/**
			 * backoff Метод извлечения обратной частоты последовательности
			 * @param seq последовательность для извлечения обратной частоты
			 * @return    обратная частота последовательности
			 */
			virtual const double backoff(const vector <size_t> & seq) const noexcept;
			/**
			 * weight Метод извлечения веса последовательности
			 * @param seq последовательность для извлечения веса
			 * @return    вес последовательноси и n-грамма для которой она получена
			 */
			virtual const pair <u_short, double> weight(const vector <size_t> & seq) const noexcept;
			/**
			 * frequency Метод извлечения частоты n-граммы
			 * @param seq список слов последовательности
			 * @return    частота и обратная частота n-граммы
			 */
			virtual const pair <double, double> frequency(const vector <size_t> & seq) const noexcept;
		private:
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq последовательность слов для установки
			 */
			virtual void set(const vector <alm_t::seq_t> & seq) const noexcept;
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq     список идентификаторов слов которые нужно добавить
			 * @param uppers  список верхних регистров последнего слова последовательности
			 * @param weight  вес n-граммы из файла arpa
			 * @param backoff обратная частота документа из файла arpa
			 */
			virtual void set(const vector <size_t> & seq, const size_t uppers, const double weight, const double backoff) const noexcept;
		public:
			/**
			 * clear Метод очистки всех данных
			 */
			virtual void clear();
		public:
			/**
			 * perplexity Метод расчёта перплексии
			 * @param  seq список последовательностей
			 * @return     результат расчёта
			 */
			virtual const ppl_t perplexity(const vector <size_t> & seq) const noexcept;
			/**
			 * check Метод проверки существования последовательности, с указанным шагом
			 * @param seq  список слов последовательности
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			virtual const bool check(const vector <size_t> & seq, const u_short step) const noexcept;
			/**
			 * exist Метод проверки существования последовательности
			 * @param seq  список слов последовательности
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			virtual const pair <bool, size_t> exist(const vector <size_t> & seq, const u_short step) const noexcept;
			/**
			 * check Метод проверки существования последовательности
			 * @param seq      список слов последовательности
			 * @param accurate режим точной проверки
			 * @return         результат проверки
			 */
			virtual const pair <bool, size_t> check(const vector <size_t> & seq, const bool accurate = false) const noexcept;
		public:
			/**
			 * setBin Метод установки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 */
			virtual void setBin(const vector <char> & buffer) const noexcept;
			/**
			 * getBin Метод извлечения данных arpa в бинарном виде
			 * @param callback функция обратного вызова
			 */
			virtual void getBin(function <void (const vector <char> &, const size_t, const u_short)> callback) const noexcept;
		public:
			/**
			 * sentences Метод генерации предложений
			 * @param callback функция обратного вызова
			 */
			virtual void sentences(function <const bool (const wstring &)> callback) const noexcept;
			/**
			 * getUppers Метод извлечения регистров для каждого слова
			 * @param seq  последовательность слов для сборки контекста
			 * @param upps список извлечённых последовательностей
			 */
			virtual void getUppers(const vector <size_t> & seq, vector <size_t> & upps) const noexcept;
			/**
			 * find Метод поиска n-грамм в тексте
			 * @param text     текст в котором необходимо найти n-граммы
			 * @param callback функция обратного вызова
			 */
			virtual void find(const wstring & text, function <void (const wstring &)> callback) const noexcept;
		public:
			/**
			 * context Метод сборки текстового контекста из последовательности
			 * @param seq  последовательность слов для сборки контекста
			 * @param nwrd флаг разрешающий вывод системных токенов
			 * @return     собранный текстовый контекст
			 */
			virtual const wstring context(const vector <size_t> & seq, const bool nwrd = false) const noexcept;
		public:
			/**
			 * Alm1 Конструктор
			 */
			Alm1() noexcept : alm_t() {};
			/**
			 * Alm1 Конструктор
			 * @param alphabet объект алфавита
			 */
			Alm1(const alphabet_t * alphabet) noexcept : alm_t(alphabet) {};
			/**
			 * Alm1 Конструктор
			 * @param tokenizer объект токенизатора
			 */
			Alm1(const tokenizer_t * tokenizer) noexcept : alm_t(tokenizer) {};
			/**
			 * Alm1 Конструктор
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 */
			Alm1(const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept : alm_t(alphabet, tokenizer) {};
			/**
			 * ~Alm1 Деструктор
			 */
			virtual ~Alm1() noexcept;
	} alm1_t;
	/**
	 * Alm2 Класс alm второго типа
	 */
	typedef class Alm2 : public Alm1 {
		private:
			/**
			 * Ngram Структура N-граммы
			 */
			typedef struct Ngram {
				/**
				 * Регистры слова
				 */
				size_t uppers;
				/**
				 * Частота последовательности и
				 * обратная частота последовательности
				 */
				double weight, backoff;
				/**
				 * Ngram Конструктор
				 */
				Ngram() : uppers(0), weight(log(0)), backoff(0.0) {}
			} ngram_t;
			// Создаём тип данных структуры Лес, для данных arpa
			typedef std::map <u_short, std::map <size_t, ngram_t>> arpa_t;
			// Словарь языковой модели
			mutable arpa_t arpa;
		private:
			/**
			 * exist Метод проверки существования последовательности
			 * @param seq список слов последовательности
			 * @return    результат проверки
			 */
			const bool exist(const vector <size_t> & seq) const noexcept;
			/**
			 * backoff Метод извлечения обратной частоты последовательности
			 * @param seq последовательность для извлечения обратной частоты
			 * @return    обратная частота последовательности
			 */
			const double backoff(const vector <size_t> & seq) const noexcept;
			/**
			 * weight Метод извлечения веса последовательности
			 * @param seq последовательность для извлечения веса
			 * @return    вес последовательноси и n-грамма для которой она получена
			 */
			const pair <u_short, double> weight(const vector <size_t> & seq) const noexcept;
			/**
			 * frequency Метод извлечения частоты n-граммы
			 * @param seq список слов последовательности
			 * @return    частота и обратная частота n-граммы
			 */
			const pair <double, double> frequency(const vector <size_t> & seq) const noexcept;
		private:
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq последовательность слов для установки
			 */
			void set(const vector <alm_t::seq_t> & seq) const noexcept;
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq     список идентификаторов слов которые нужно добавить
			 * @param uppers  список верхних регистров последнего слова последовательности
			 * @param weight  вес n-граммы из файла arpa
			 * @param backoff обратная частота документа из файла arpa
			 */
			void set(const vector <size_t> & seq, const size_t uppers, const double weight, const double backoff) const noexcept;
		public:
			/**
			 * clear Метод очистки всех данных
			 */
			void clear();
		public:
			/**
			 * perplexity Метод расчёта перплексии
			 * @param  seq список последовательностей
			 * @return     результат расчёта
			 */
			const ppl_t perplexity(const vector <size_t> & seq) const noexcept;
			/**
			 * check Метод проверки существования последовательности, с указанным шагом
			 * @param seq  список слов последовательности
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			const bool check(const vector <size_t> & seq, const u_short step) const noexcept;
			/**
			 * exist Метод проверки существования последовательности
			 * @param seq  список слов последовательности
			 * @param step размер шага проверки последовательности
			 * @return     результат проверки
			 */
			const pair <bool, size_t> exist(const vector <size_t> & seq, const u_short step) const noexcept;
			/**
			 * check Метод проверки существования последовательности
			 * @param seq      список слов последовательности
			 * @param accurate режим точной проверки
			 * @return         результат проверки
			 */
			const pair <bool, size_t> check(const vector <size_t> & seq, const bool accurate = false) const noexcept;
		public:
			/**
			 * setBin Метод установки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 */
			void setBin(const vector <char> & buffer) const noexcept;
			/**
			 * setBin2 Метод установки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 */
			void setBin2(const vector <char> & buffer) const noexcept;
			/**
			 * getBin Метод извлечения данных arpa в бинарном виде
			 * @param callback функция обратного вызова
			 */
			void getBin(function <void (const vector <char> &, const size_t, const u_short)> callback) const noexcept;
		public:
			/**
			 * sentences Метод генерации предложений
			 * @param callback функция обратного вызова
			 */
			void sentences(function <const bool (const wstring &)> callback) const noexcept;
			/**
			 * getUppers Метод извлечения регистров для каждого слова
			 * @param seq  последовательность слов для сборки контекста
			 * @param upps список извлечённых последовательностей
			 */
			void getUppers(const vector <size_t> & seq, vector <size_t> & upps) const noexcept;
			/**
			 * find Метод поиска n-грамм в тексте
			 * @param text     текст в котором необходимо найти n-граммы
			 * @param callback функция обратного вызова
			 */
			void find(const wstring & text, function <void (const wstring &)> callback) const noexcept;
		public:
			/**
			 * context Метод сборки текстового контекста из последовательности
			 * @param seq  последовательность слов для сборки контекста
			 * @param nwrd флаг разрешающий вывод системных токенов
			 * @return     собранный текстовый контекст
			 */
			const wstring context(const vector <size_t> & seq, const bool nwrd = false) const noexcept;
		public:
			/**
			 * Alm2 Конструктор
			 */
			Alm2() noexcept : Alm1() {};
			/**
			 * Alm2 Конструктор
			 * @param alphabet объект алфавита
			 */
			Alm2(const alphabet_t * alphabet) noexcept : Alm1(alphabet) {};
			/**
			 * Alm2 Конструктор
			 * @param tokenizer объект токенизатора
			 */
			Alm2(const tokenizer_t * tokenizer) noexcept : Alm1(tokenizer) {};
			/**
			 * Alm2 Конструктор
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 */
			Alm2(const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept : Alm1(alphabet, tokenizer) {};
			/**
			 * ~Alm2 Деструктор
			 */
			~Alm2() noexcept;
	} alm2_t;
};

#endif // __ANYKS_LANGUAGE_MODEL__

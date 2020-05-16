/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_TOOLKIT_
#define _ANYKS_TOOLKIT_

/**
 * Стандартная библиотека
 */
#include <set>
#include <map>
#include <mutex>
#include <limits>
#include <bitset>
#include <vector>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <sys/stat.h>
/**
 * Наши модули
 */
#include <fsys.hpp>
#include <arpa.hpp>
#include <word.hpp>
#include <python.hpp>
#include <alphabet.hpp>
#include <app/alm.hpp>

// Устанавливаем область видимости
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Toolkit Класс модуля сборки языковой модели
	 */
	typedef class Toolkit {
		public:
			// Типы алгоритмов расчёта языковой модели
			enum class algorithm_t : u_short {
				addSmooth,
				kneserNey,
				wittenBell,
				goodTuring,
				modKneserNey,
				constDiscount,
				naturalDiscount
			};
			// Основные опции
			enum class options_t : u_short {
				debug,      // Флаг режима отладки
				resetUnk,   // Флаг сброса частоты неизвестного слова
				onlyGood,   // Флаг использования только слов из белого списка
				allowUnk,   // Флаг разрешаюний использовать токен неизвестного слова
				allGrams,   // Флаг предписывающий использовать все полученные n-граммы
				mixdicts,   // Флаг разрешающий детектировать слова из смешанных словарей
				lowerCase,  // Флаг предписывающий использовать слова в нижнем регистре
				confidence, // Флаг разрешающий загружать n-граммы из arpa так-как они есть
				interpolate // Флаг предписывающий выполнять интерполяцию при расчёте частот
			};
			// Флаги модификации
			enum class modify_t : u_short {emplace, remove, change, replace};
			/**
			 * Params Параметры алгоритма сглаживания
			 */
			typedef struct Params {
				double mod;        // Дополнительный параметр дельты
				bool prepares;     // Необходимость изменения счёта, после вычисления
				bool modified;     // Количество уже изменённых младших заказов
				u_short algorithm; // Алгоритм сглаживания
				/**
				 * Params Конструктор
				 */
				Params() : mod(0.0), prepares(false), modified(false), algorithm(0) {}
			} params_t;
			// Упрощаем тип функции предобработки слов
			typedef function <const string (const string &, const vector <string> &)> wpres_t;
		private:
			/**
			 * Info Структура общей статистики
			 */
			typedef struct Info {
				size_t ad;  // Общее количество документов
				size_t cw;  // Общее количество слов
				size_t unq; // Количество уникальных слов
				size_t idd; // Идентификатор последнего документа
				/**
				 * Info Конструктор
				 */
				Info() : ad(0), cw(0), unq(0), idd(idw_t::NIDW) {};
			} __attribute__((packed)) info_t;
			/**
			 * UserToken Структура пользовательского токена
			 */
			typedef struct UserToken {
				size_t idw;                                            // Идентификатор слова
				word_t name;                                           // Название токена
				function <bool (const string &, const string &)> test; // Функция проверки
				/**
				 * UserToken Конструктор
				 */
				UserToken() : idw(idw_t::NIDW), name("") {}
			} utoken_t;
		private:
			// Замена неизвестному слову
			size_t unknown = 0;
			// Размер n-грамм
			u_short size = DEFNGRAM;
			// Флаг запрещающий очистку объект питона
			bool notCleanPython = false;
		private:
			// Общая статистика
			info_t info;
			// Параметры алгоритма сглаживания
			params_t params;
			// Флаги параметров
			bitset <9> options;
			// Список плохих слов
			set <size_t> badwords;
			// Список хороших слов
			set <size_t> goodwords;
			// Мютекс блокировки потока
			recursive_mutex locker;
			// Список токенов приводимых к <unk>
			set <token_t> tokenUnknown;
			// Список запрещённых токенов
			set <token_t> tokenDisable;
			// Список пользовательских токенов
			map <size_t, utoken_t> utokens;
			// Словарь всех слов в системе
			mutable map <size_t, word_t> vocab;
			// Список скриптов python
			map <u_short, pair <string, size_t>> scripts;
		private:
			// Объект arpa модуля
			arpa_t * arpa = nullptr;
			// Объект работы с python
			python_t * python = nullptr;
			// Функция предобработки слов
			wpres_t wordPress = nullptr;
			// Объект log файла
			const char * logfile = nullptr;
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
			// Объект токенизатора
			const tokenizer_t * tokenizer = nullptr;
		private:
			/**
			 * word Метод получения данных слова
			 * @param idw идентификатор слова
			 * @return    запрашиваемое слово
			 */
			const word_t * word(const size_t idw) const noexcept;
			/**
			 * isOption Метод проверки наличия опции
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isOption(const options_t option) const noexcept;
			/**
			 * getIdw Метод генерирования идентификатора слова
			 * @param  word  слово для генерации
			 * @param  check нужно выполнить дополнительную проверку слова
			 * @return       идентификатор слова
			 */
			const size_t getIdw(const wstring & word, const bool check = true) const noexcept;
			/**
			 * clearShielding Функция удаления экранирования
			 * @param word  слово в котором следует удалить экранирование
			 * @param front первый символ экранирования
			 * @param back  последний символ экранирования
			 */
			void clearShielding(const string & word, const string & front = "<", const string & back = ">") const noexcept;
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
			const u_int getOptions() const noexcept;
			/**
			 * getParams Метод извлечения параметров алгоритма сжатия
			 * @return параметры алгоритма сжатия
			 */
			const params_t & getParams() const noexcept;
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
			const set <size_t> & getBadwords() const noexcept;
			/**
			 * getGoodwords Метод извлечения белого списка
			 * @return белый список слов
			 */
			const set <size_t> & getGoodwords() const noexcept;
			/**
			 * getTokensUnknown Метод извлечения списка токенов приводимых к <unk>
			 * @return список токенов
			 */
			const set <token_t> & getTokensUnknown() const noexcept;
			/**
			 * getTokensDisable Метод извлечения списка запрещённых токенов
			 * @return список токенов
			 */
			const set <token_t> & getTokensDisable() const noexcept;
			/**
			 * getStatistic Метод извлечения общей статистики
			 * @return общее количество документов и слов в корпусах при обучении
			 */
			const pair <size_t, size_t> getStatistic() const noexcept;
			/**
			 * getUserTokens Метод извлечения списка пользовательских токенов
			 * @return список пользовательских токенов
			 */
			const vector <string> & getUserTokens() const noexcept;
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
			 * clear Метод очистки
			 */
			void clear() noexcept;
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
			 * setUserToken Метод добавления токена пользователя
			 * @param name слово - обозначение токена
			 */
			void setUserToken(const string & name) noexcept;
			/**
			 * setOption Метод подключения опции модуля
			 * @param option опция для подключения
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
			void setTokensUnknown(const set <token_t> & tokens) noexcept;
			/**
			 * setTokensDisable Метод установки списка запрещённых токенов
			 * @param tokens список токенов для установки
			 */
			void setTokensDisable(const set <token_t> & tokens) noexcept;
			/**
			 * getStatistic Метод установки общей статистики
			 * @param ad общее количество документов при обучении
			 * @param cw общее количество слов при обучении
			 */
			void getStatistic(const size_t ad, const size_t cw) noexcept;
			/**
			 * setBadwords Метод установки списка идентификаторов плохих слов в список
			 * @param badwords список идентификаторов плохих слов
			 */
			void setBadwords(const set <size_t> & badwords) noexcept;
			/**
			 * setBadwords Метод установки списка плохих слов в список
			 * @param badwords список плохих слов
			 */
			void setBadwords(const vector <string> & badwords) noexcept;
			/**
			 * setGoodwords Метод установки списка идентификаторов хороших слов в список
			 * @param goodwords список идентификаторов хороших слов
			 */
			void setGoodwords(const set <size_t> & goodwords) noexcept;
			/**
			 * setGoodwords Метод установки списка хороших слов в список
			 * @param goodwords список хороших слов
			 */
			void setGoodwords(const vector <string> & goodwords) noexcept;
			/**
			 * addText Метод добавления текста для расчёта
			 * @param text текст который нужно добавить
			 * @param idd  идентификатор документа
			 */
			void addText(const string & text, const size_t idd = 0) noexcept;
			/**
			 * addWord Метод добавления слова в словарь
			 * @param word слово для добавления
			 * @param idw  идентификатор слова, если нужно
			 * @param idd  идентификатор документа
			 */
			void addWord(const word_t & word, const size_t idw = 0, const size_t idd = 0) noexcept;
			/**
			 * addWord Метод добавления слова в словарь
			 * @param word слово для добавления
			 * @param idw  идентификатор слова, если нужно
			 * @param idd  идентификатор документа
			 */
			void addWord(const wstring & word, const size_t idw = 0, const size_t idd = 0) noexcept;
		public:
			/**
			 * setWordPreprocessingMethod Метод установки функции препроцессинга слова
			 * @param fn внешняя функция препроцессинга слова
			 */
			void setWordPreprocessingMethod(wpres_t fn) noexcept;
			/**
			 * setUserTokenMethod Метод установки функции обработки пользовательского токена
			 * @param name слово - обозначение токена
			 * @param fn   внешняя функция обрабатывающая пользовательский токен
			 */
			void setUserTokenMethod(const string & name, function <bool (const string &, const string &)> fn) noexcept;
			/**
			 * init Метод инициализации языковой модели
			 * @param algorithm алгоритм расчёта языковой модели
			 * @param modified  количество уже изменённых младших заказов
			 * @param prepares  необходимость изменения счёта, после вычисления
			 * @param mod       дополнительный параметр дельты
			 */
			void init(const algorithm_t algorithm, const bool modified = false, const bool prepares = false, const double mod = 0.0);
		public:
			/**
			 * sweep Метод удаления низкочастотных n-грамм arpa
			 * @param status статус расёта
			 */
			void sweep(function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * train Метод запуска обучения языковой модели
			 * @param status функция вывода статуса обучения
			 */
			void train(function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * repair Метод ремонта уже расчитанной ранее arpa
			 * @param status статус расёта
			 */
			void repair(function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * pruneVocab Метод прунинга словаря
			 * @param wltf   пороговый вес слова для прунинга
			 * @param status статус прунинга словаря
			 */
			void pruneVocab(const double wltf, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * modify Метод модификации arpa
			 * @param filename адрес файла для чтения
			 * @param flag     флаг модификации arpa
			 * @param status   функция вывода статуса модификации
			 */
			void modify(const string & filename, modify_t flag, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * prune Метод прунинга языковой модели
			 * @param threshold порог частоты прунинга
			 * @param mingram   значение минимальной n-граммы за которую нельзя прунить
			 * @param status    функция вывода статуса обучения
			 */
			void prune(const double threshold, const u_short mingram, function <void (const u_short)> status = nullptr) const noexcept;
			/**
			 * mix Метод интерполяции нескольких arpa
			 * @param filenames список файлов arpa для объединения
			 * @param lambdas   список весов первой модели при интерполяции
			 * @param backward  необходимо выполнить интерполяцию в обратном направлении
			 * @param status    функция вывода статуса чтения
			 */
			void mix(const vector <string> & filenames, const vector <double> & lambdas, const bool backward = false, function <void (const string &, const u_short)> status = nullptr) noexcept;
			/**
			 * mix Метод интерполяции нескольких arpa алгоритмами (Баеса и Логарифмическо-линейным)
			 * @param filenames список файлов arpa для объединения
			 * @param lambdas   список весов первой модели при интерполяции
			 * @param length    байесовская интерполяция с учетом контекста длины length
			 * @param scale     логарифмическая шкала вероятности для алгоритма Баеса
			 * @param status    функция вывода статуса чтения
			 */
			void mix(const vector <string> & filenames, const vector <double> & lambdas, const size_t length = 0, const double scale = 0.0, function <void (const string &, const u_short)> status = nullptr) noexcept;
		public:
			/**
			 * loadVocab Метод загрузки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 * @param arpa   нужно добавить только данные arpa
			 */
			void loadVocab(const vector <char> & buffer) noexcept;
			/**
			 * loadInfoVocab Метод загрузки бинарных информационных данных словаря
			 * @param buffer буфер бинарных информационных данных словаря
			 */
			void loadInfoVocab(const vector <char> & buffer) noexcept;
			/**
			 * loadArpa Метод загрузки бинарных данных n-грамм в словарь
			 * @param buffer буфер с бинарными данными
			 * @param arpa   нужно добавить только данные arpa
			 */
			void loadArpa(const vector <char> & buffer, const bool arpa = false) noexcept;
			/**
			 * appendArpa Метод добавления бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 * @param idd    идентификатор документа в котором получена n-грамма
			 */
			void appendArpa(const vector <char> & buffer, const size_t idd = 0) const noexcept;
		public:
			/**
			 * saveInfoVocab Метод сохранения бинарных информационных данных словаря
			 * @param buffer буфер бинарных информационных данных словаря
			 */
			void saveInfoVocab(vector <char> & buffer) const noexcept;
			/**
			 * saveVocab Метод извлечения данных словаря в бинарном виде
			 * @param callback функция обратного вызова
			 */
			void saveVocab(function <void (const vector <char> &, const u_short)> callback) const noexcept;
			/**
			 * writeArpa Метод записи данных в файл arpa
			 * @param filename адрес файла для записи
			 * @param status   функция вывода статуса записи
			 */
			void writeArpa(const string & filename, function <void (const u_short)> status = nullptr) const noexcept;
			/**
			 * writeVocab Метод записи данных словаря в файл
			 * @param filename адрес файла для записи
			 * @param status   функция вывода статуса записи
			 */
			void writeVocab(const string & filename, function <void (const u_short)> status = nullptr) const noexcept;
			/**
			 * writeWords Метод записи данных слов в файл
			 * @param filename адрес файла для записи
			 * @param status   функция вывода статуса записи
			 */
			void writeWords(const string & filename, function <void (const u_short)> status = nullptr) const noexcept;
			/**
			 * writeNgrams Метод записи данных в файлы ngrams
			 * @param filename адрес файла для записи
			 * @param status   функция вывода статуса записи
			 */
			void writeNgrams(const string & filename, function <void (const u_short)> status = nullptr) const noexcept;
			/**
			 * saveArpa Метод извлечения данных n-грамм в бинарном виде
			 * @param callback функция обратного вызова
			 * @param arpa     нужно добавить только данные arpa
			 */
			void saveArpa(function <void (const vector <char> &, const u_short)> callback, const bool arpa = false) const noexcept;
			/**
			 * writeMap Метод записи карты последовательности в файл
			 * @param filename адрес map файла карты последовательности
			 * @param status   функция вывода статуса записи
			 * @param delim    разделитель последовательностей
			 */
			void writeMap(const string & filename, function <void (const u_short)> status = nullptr, const string & delim = "|") const noexcept;
		public:
			/**
			 * words Метод извлечения слов из словаря
			 * @param callback функция обратного вызова (word, idw, count words)
			 */
			void words(function <const bool (const word_t &, const size_t, const size_t)> callback) const noexcept;
			/**
			 * words Метод извлечения слов из словаря
			 * @param callback функция обратного вызова (word, idw, oc, dc, count words)
			 */
			void words(function <const bool (const wstring &, const size_t, const size_t, const size_t, const size_t)> callback) const noexcept;
		public:
			/**
			 * readArpa Метод чтения данных из файла arpa
			 * @param filename адрес файла для чтения
			 * @param status   функция вывода статуса чтения
			 */
			void readArpa(const string & filename, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * readVocab Метод чтения словаря из файла
			 * @param filename файл для чтения словаря
			 * @param status   функция вывода статуса чтения
			 */
			void readVocab(const string & filename, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * readNgram Метод чтения данных из файла ngrams
			 * @param filename адрес файла для чтения
			 * @param status   функция вывода статуса чтения
			 */
			void readNgram(const string & filename, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * readMap Метод чтения карты последовательности из файла
			 * @param filename адрес map файла карты последовательности
			 * @param status   функция вывода статуса чтения
			 * @param delim    разделитель последовательностей
			 */
			void readMap(const string & filename, function <void (const u_short)> status = nullptr, const string & delim = "|") noexcept;
		public:
			/**
			 * readNgrams Метод чтения данных из каталога файлов ngrams
			 * @param path   адрес где лежат ngrams файлы
			 * @param ext    расширение файлов в каталоге
			 * @param status функция вывода статуса чтения
			 */
			void readNgrams(const string & path, const string & ext, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * readMaps Метод добавления - объединения карт последовательностей
			 * @param path   адрес где лежат map файлы
			 * @param ext    расширение файлов в каталоге
			 * @param status функция вывода статуса чтения
			 * @param delim  разделитель последовательностей
			 */
			void readMaps(const string & path, const string & ext, function <void (const u_short)> status = nullptr, const string & delim = "|") noexcept;
		public:
			/**
			 * Toolkit Конструктор
			 */
			Toolkit() noexcept {};
			/**
			 * Toolkit Конструктор
			 * @param tokenizer объект токенизатора
			 */
			Toolkit(const tokenizer_t * tokenizer) noexcept;
			/**
			 * Toolkit Конструктор
			 * @param size размерность n-грамм
			 */
			Toolkit(const u_short size = DEFNGRAM) noexcept;
			/**
			 * Toolkit Конструктор
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 */
			Toolkit(const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept;
			/**
			 * Toolkit Конструктор
			 * @param alphabet объект алфавита
			 * @param size     размерность n-грамм
			 */
			Toolkit(const alphabet_t * alphabet, const u_short size = DEFNGRAM) noexcept;
			/**
			 * Toolkit Конструктор
			 * @param tokenizer объект токенизатора
			 * @param size      размерность n-грамм
			 */
			Toolkit(const tokenizer_t * tokenizer, const u_short size = DEFNGRAM) noexcept;
			/**
			 * Toolkit Конструктор
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 * @param size      размерность n-грамм
			 */
			Toolkit(const alphabet_t * alphabet, const tokenizer_t * tokenizer, const u_short size = DEFNGRAM) noexcept;
			/**
			 * ~Toolkit Деструктор
			 */
			~Toolkit() noexcept;
	} toolkit_t;
};

#endif // _ANYKS_TOOLKIT_

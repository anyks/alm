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
// #include <mutex>
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
#include <general.hpp>
#include <alphabet.hpp>

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
		private:
			// Общая статистика
			info_t info;
			// Мютекс для блокировки потока
			// mutex locker;
			// Параметры алгоритма сглаживания
			params_t params;
			// Флаги параметров
			bitset <8> options;
			// Список плохих слов
			set <size_t> badwords;
			// Список хороших слов
			set <size_t> goodwords;
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
			// Объект log файла
			const char * logfile = nullptr;
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
			// Объект токенизатора
			const tokenizer_t * tokenizer = nullptr;
		private:
			/**
			 * getIdw Метод генерирования идентификатора слова
			 * @param  word слово для генерации
			 * @return      идентификатор слова
			 */
			const size_t getIdw(const wstring & word) const;
			/**
			 * isOption Метод проверки наличия опции
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isOption(const options_t option) const;
			/**
			 * clearShielding Функция удаления экранирования
			 * @param word  слово в котором следует удалить экранирование
			 * @param front первый символ экранирования
			 * @param back  последний символ экранирования
			 */
			void clearShielding(const string & word, const string & front = "<", const string & back = ">") const;
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
			const u_int getOptions() const;
			/**
			 * getParams Метод извлечения параметров алгоритма сжатия
			 * @return параметры алгоритма сжатия
			 */
			const params_t & getParams() const;
			/**
			 * getUnknown Метод извлечения неизвестного слова
			 * @return установленное неизвестное слово
			 */
			const string getUnknown() const;
			/**
			 * getWordScript Метод извлечения скрипта обработки слов
			 * @return адрес скрипта python обработки слов
			 */
			const string getWordScript() const;
			/**
			 * getUserTokenScript Метод извлечения скрипта обработки пользовательских токенов
			 * @return адрес скрипта python обработки пользовательских токенов
			 */
			const string getUserTokenScript() const;
			/**
			 * getBadwords Метод извлечения чёрного списка
			 * @return чёрный список слов
			 */
			const set <size_t> & getBadwords() const;
			/**
			 * getGoodwords Метод извлечения белого списка
			 * @return белый список слов
			 */
			const set <size_t> & getGoodwords() const;
			/**
			 * getUserTokens Метод извлечения списка пользовательских токенов
			 * @return список пользовательских токенов
			 */
			const vector <string> getUserTokens() const;
			/**
			 * getUserTokenWord Метод получения пользовательского токена по его идентификатору
			 * @param idw идентификатор пользовательского токена
			 * @return    пользовательский токен соответствующий идентификатору
			 */
			const string getUserTokenWord(const size_t idw) const;
			/**
			 * getUserTokenId Метод получения идентификатора пользовательского токена
			 * @param name слово для которого нужно получить идентификатор
			 * @return     идентификатор пользовательского токена соответствующий слову
			 */
			const size_t getUserTokenId(const string & name) const;
		public:
			/**
			 * clear Метод очистки
			 */
			void clear();
			/**
			 * clearBadwords Метод очистки списка плохих слов
			 */
			void clearBadwords();
			/**
			 * clearGoodwords Метод очистки списка хороших слов
			 */
			void clearGoodwords();
			/**
			 * clearUserTokens Метод очистки пользовательских токенов
			 */
			void clearUserTokens();
		public:
			/**
			 * setAllTokenUnknown Метод установки всех токенов идентифицируемых как <unk>
			 */
			void setAllTokenUnknown();
			/**
			 * setAllTokenDisable Метод установки всех токенов как не идентифицируемых
			 */
			void setAllTokenDisable();
		public:
			/**
			 * addBadword Метод добавления идентификатора похого слова в список
			 * @param idw идентификатор слова
			 */
			void addBadword(const size_t idw);
			/**
			 * addGoodword Метод добавления идентификатора хорошего слова в список
			 * @param idw идентификатор слова
			 */
			void addGoodword(const size_t idw);
			/**
			 * addBadword Метод добавления похого слова в список
			 * @param word слово для добавления
			 */
			void addBadword(const string & word);
			/**
			 * addGoodword Метод добавления хорошего слова в список
			 * @param word слово для добавления
			 */
			void addGoodword(const string & word);
		public:
			/**
			 * setSize Метод установки размера n-граммы
			 * @param size размер n-граммы
			 */
			void setSize(const u_short size);
			/**
			 * setUnknown Метод установки неизвестного слова
			 * @param word слово для добавления
			 */
			void setUnknown(const string & word);
			/**
			 * setOptions Метод установки опций
			 * @param options опции для установки
			 */
			void setOptions(const u_int options);
			/**
			 * setLogfile Метод установка файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile);
			/**
			 * setUserToken Метод добавления токена пользователя
			 * @param name слово - обозначение токена
			 */
			void setUserToken(const string & name);
			/**
			 * setOption Метод подключения опции модуля
			 * @param option опция для подключения
			 */
			void setOption(const options_t option);
			/**
			 * unsetOption Метод отключения опции модуля
			 * @param option опция для отключения
			 */
			void unsetOption(const options_t option);
			/**
			 * setWordScript Метод установки скрипта обработки слов
			 * @param script скрипт python обработки слов
			 */
			void setWordScript(const string & script);
			/**
			 * setTokenUnknown Метод установки списка токенов которых нужно идентифицировать как <unk>
			 * @param options список токенов которых нужно идентифицировать как <unk>
			 */
			void setTokenUnknown(const string & options);
			/**
			 * setTokenDisable Метод установки списка не идентифицируемых токенов
			 * @param options список не идентифицируемых токенов
			 */
			void setTokenDisable(const string & options);
			/**
			 * setUserTokenScript Метод установки скрипта обработки пользовательских токенов
			 * @param script скрипт python обработки пользовательских токенов
			 */
			void setUserTokenScript(const string & script);
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet);
			/**
			 * setTokenizer Метод установки токенизатора
			 * @param tokenizer объект токенизатора
			 */
			void setTokenizer(const tokenizer_t * tokenizer);
			/**
			 * addBadwords Метод добавления списка идентификаторов плохих слов в список
			 * @param badwords список идентификаторов плохих слов
			 */
			void addBadwords(const vector <size_t> & badwords);
			/**
			 * addBadwords Метод добавления списка плохих слов в список
			 * @param badwords список плохих слов
			 */
			void addBadwords(const vector <string> & badwords);
			/**
			 * addGoodwords Метод добавления списка идентификаторов хороших слов в список
			 * @param goodwords список идентификаторов хороших слов
			 */
			void addGoodwords(const vector <size_t> & goodwords);
			/**
			 * addGoodwords Метод добавления списка хороших слов в список
			 * @param goodwords список хороших слов
			 */
			void addGoodwords(const vector <string> & goodwords);
			/**
			 * addText Метод добавления текста для расчёта
			 * @param text текст который нужно добавить
			 * @param idd  идентификатор документа
			 */
			void addText(const string & text, const size_t idd = 0);
			/**
			 * addWord Метод добавления слова в словарь
			 * @param word слово для добавления
			 * @param idw  идентификатор слова, если нужно
			 * @param idd  идентификатор документа
			 */
			void addWord(const wstring & word, const size_t idw = 0, const size_t idd = 0);
			/**
			 * setUserTokenMethod Метод добавления функции обработки пользовательского токена
			 * @param name слово - обозначение токена
			 * @param fn   внешняя функция обрабатывающая пользовательский токен
			 */
			void setUserTokenMethod(const string & name, function <bool (const string &, const string &)> fn);
		public:
			/**
			 * sweep Метод удаления низкочастотных n-грамм arpa
			 * @param status статус расёта
			 */
			void sweep(function <void (const u_short)> status = nullptr);
			/**
			 * train Метод запуска обучения языковой модели
			 * @param status функция вывода статуса обучения
			 */
			void train(function <void (const u_short)> status = nullptr);
			/**
			 * repair Метод ремонта уже расчитанной ранее arpa
			 * @param status статус расёта
			 */
			void repair(function <void (const u_short)> status = nullptr);
			/**
			 * pruneVocab Метод прунинга словаря
			 * @param wltf   пороговый вес слова для прунинга
			 * @param status статус прунинга словаря
			 */
			void pruneVocab(const float wltf, function <void (const u_short)> status = nullptr);
			/**
			 * modify Метод модификации arpa
			 * @param filename адрес файла для чтения
			 * @param flag     флаг модификации arpa
			 * @param status   функция вывода статуса модификации
			 */
			void modify(const string & filename, modify_t flag, function <void (const u_short)> status = nullptr);
			/**
			 * prune Метод прунинга языковой модели
			 * @param threshold порог частоты прунинга
			 * @param mingram   значение минимальной n-граммы за которую нельзя прунить
			 * @param status    функция вывода статуса обучения
			 */
			void prune(const double threshold, const u_short mingram, function <void (const u_short)> status = nullptr) const;
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
			 * loadInfoVocab Метод загрузки бинарных информационных данных словаря
			 * @param buffer буфер бинарных информационных данных словаря
			 */
			void loadInfoVocab(const vector <char> & buffer);
			/**
			 * loadVocab Метод загрузки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 * @param arpa   нужно добавить только данные arpa
			 */
			void loadVocab(const vector <char> & buffer) const;
			/**
			 * loadArpa Метод загрузки бинарных данных n-грамм в словарь
			 * @param buffer буфер с бинарными данными
			 * @param arpa   нужно добавить только данные arpa
			 */
			void loadArpa(const vector <char> & buffer, const bool arpa = false) const;
		public:
			/**
			 * saveInfoVocab Метод сохранения бинарных информационных данных словаря
			 * @param buffer буфер бинарных информационных данных словаря
			 */
			void saveInfoVocab(vector <char> & buffer) const;
			/**
			 * saveVocab Метод извлечения данных словаря в бинарном виде
			 * @param callback функция обратного вызова
			 */
			void saveVocab(function <void (const vector <char> &, const u_short)> callback) const;
			/**
			 * writeArpa Метод записи данных в файл arpa
			 * @param filename адрес файла для записи
			 * @param status   функция вывода статуса записи
			 */
			void writeArpa(const string & filename, function <void (const u_short)> status = nullptr) const;
			/**
			 * writeVocab Метод записи данных словаря в файл
			 * @param filename адрес файла для записи
			 * @param status   функция вывода статуса записи
			 */
			void writeVocab(const string & filename, function <void (const u_short)> status = nullptr) const;
			/**
			 * writeNgrams Метод записи данных в файлы ngrams
			 * @param filename адрес файла для записи
			 * @param status   функция вывода статуса записи
			 */
			void writeNgrams(const string & filename, function <void (const u_short)> status = nullptr) const;
			/**
			 * saveArpa Метод извлечения данных n-грамм в бинарном виде
			 * @param callback функция обратного вызова
			 * @param arpa     нужно добавить только данные arpa
			 */
			void saveArpa(function <void (const vector <char> &, const u_short)> callback, const bool arpa = false) const;
			/**
			 * writeMap Метод записи карты последовательности в файл
			 * @param filename адрес map файла карты последовательности
			 * @param status   функция вывода статуса записи
			 * @param delim    разделитель последовательностей
			 */
			void writeMap(const string & filename, function <void (const u_short)> status = nullptr, const string & delim = "|") const;
		public:
			/**
			 * readArpas Метод чтения данных из каталога файлов arpa
			 * @param path   адрес где лежат arpa файлы
			 * @param status функция вывода статуса чтения
			 */
			void readArpas(const string & path, function <void (const u_short)> status = nullptr);
			/**
			 * readNgrams Метод чтения данных из каталога файлов ngrams
			 * @param path   адрес где лежат ngrams файлы
			 * @param status функция вывода статуса чтения
			 */
			void readNgrams(const string & path, function <void (const u_short)> status = nullptr);
			/**
			 * readArpa Метод чтения данных из файла arpa
			 * @param filename адрес файла для чтения
			 * @param status   функция вывода статуса чтения
			 */
			void readArpa(const string & filename, function <void (const u_short)> status = nullptr);
			/**
			 * readVocab Метод чтения словаря из файла
			 * @param filename файл для чтения словаря
			 * @param status   функция вывода статуса чтения
			 */
			void readVocab(const string & filename, function <void (const u_short)> status = nullptr);
			/**
			 * readNgram Метод чтения данных из файла ngrams
			 * @param filename адрес файла для чтения
			 * @param status   функция вывода статуса чтения
			 */
			void readNgram(const string & filename, function <void (const u_short)> status = nullptr);
			/**
			 * readMaps Метод добавления - объединения карт последовательностей
			 * @param path   адрес где лежат map файлы
			 * @param status функция вывода статуса чтения
			 * @param delim  разделитель последовательностей
			 */
			void readMaps(const string & path, function <void (const u_short)> status = nullptr, const string & delim = "|");
			/**
			 * readMap Метод чтения карты последовательности из файла
			 * @param filename адрес map файла карты последовательности
			 * @param status   функция вывода статуса чтения
			 * @param delim    разделитель последовательностей
			 */
			void readMap(const string & filename, function <void (const u_short)> status = nullptr, const string & delim = "|");
		public:
			/**
			 * Toolkit Конструктор
			 */
			Toolkit(){};
			/**
			 * Toolkit Конструктор
			 * @param tokenizer объект токенизатора
			 */
			Toolkit(const tokenizer_t * tokenizer);
			/**
			 * Toolkit Конструктор
			 * @param size размерность n-грамм
			 */
			Toolkit(const u_short size = DEFNGRAM);
			/**
			 * Toolkit Конструктор
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 */
			Toolkit(const alphabet_t * alphabet, const tokenizer_t * tokenizer);
			/**
			 * Toolkit Конструктор
			 * @param alphabet объект алфавита
			 * @param size     размерность n-грамм
			 */
			Toolkit(const alphabet_t * alphabet, const u_short size = DEFNGRAM);
			/**
			 * Toolkit Конструктор
			 * @param tokenizer объект токенизатора
			 * @param size      размерность n-грамм
			 */
			Toolkit(const tokenizer_t * tokenizer, const u_short size = DEFNGRAM);
			/**
			 * Toolkit Конструктор
			 * @param alphabet  объект алфавита
			 * @param tokenizer объект токенизатора
			 * @param size      размерность n-грамм
			 */
			Toolkit(const alphabet_t * alphabet, const tokenizer_t * tokenizer, const u_short size = DEFNGRAM);
			/**
			 * ~Toolkit Деструктор
			 */
			~Toolkit();
	} toolkit_t;
};

#endif // _ANYKS_TOOLKIT_

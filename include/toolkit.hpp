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
#include <idw.hpp>
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
			enum class options_t : u_int {
				debug,       // Флаг режима отладки
				onlyGood,    // Флаг использования только слов из белого списка
				allowUnk,    // Флаг разрешаюний использовать признак неизвестного слова
				allGrams,    // Флаг предписывающий использовать все полученные n-граммы
				lowerCase,   // Флаг предписывающий использовать слова в нижнем регистре
				interpolate, // Флаг предписывающий выполнять интерполяцию при расчёте частот
				notUserSign, // Флаг запрещающий использовать пользовательский признак
				notAbbr,     // Флаг запрещающий детектировать аббривиатуры чисел
				notDate,     // Флаг запрещающий детектировать дату
				notTime,     // Флаг запрещающий детектировать время
				notRange,    // Флаг запрещающий детектировать диапазоны арабских чисел
				notPunct,    // Флаг запрещающий детекстировать знак пунктуации
				notScore,    // Флаг запрещающий детектировать числовой счёт
				notDimen,    // Флаг запрещающий детектировать габаритные размеры
				notFract,    // Флаг запрещающий детектировать числовые дроби
				notRoman,    // Флаг запрещающий детектировать римские числа
				notAprox,    // Флаг запрещающий детектировать приблизительные значения чисел
				notNumber,   // Флаг запрещающий детектировать арабские числа
				notANumber   // Флаг запрещающий детектировать псевдо-арабские числа
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
				Info() : ad(0), cw(0), unq(0), idd(noID) {};
			} __attribute__((packed)) info_t;
			/**
			 * UserSign Структура пользовательского признака
			 */
			typedef struct Usersign {
				size_t idw;                                            // Идентификатор слова
				word_t name;                                           // Название признака
				function <bool (const string &, const string &)> test; // Функция проверки
				/**
				 * Usersign Конструктор
				 */
				Usersign() : idw(noID), name("") {}
			} usign_t;
		private:
			// Замена неизвестному слову
			size_t unknown = 0;
			// Размер n-грамм
			u_short size = DEFNGRAM;
		private:
			// Объект идентификатора
			idw_t idw;
			// Общая статистика
			info_t info;
			// Параметры алгоритма сглаживания
			params_t params;
			// Флаги параметров
			bitset <19> options;
			// Список плохих слов
			set <size_t> badwords;
			// Список хороших слов
			set <size_t> goodwords;
			// Список пользовательских признаков
			map <size_t, usign_t> usigns;
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
			const u_short getOptions() const;
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
			 * getUsignScript Метод извлечения скрипта обработки пользовательских признаков
			 * @return адрес скрипта python обработки пользовательских признаков
			 */
			const string getUsignScript() const;
			/**
			 * getParams Метод извлечения параметров алгоритма сжатия
			 * @return параметры алгоритма сжатия
			 */
			const params_t & getParams() const;
			/**
			 * getUsigns Метод извлечения списка пользовательских признаков
			 * @return список пользовательских признаков
			 */
			const vector <string> getUsigns() const;
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
			 * getUsignWord Метод получения пользовательского признака по его идентификатору
			 * @param idw идентификатор пользовательского признака
			 * @return    пользовательский признак соответствующий идентификатору
			 */
			const string getUsignWord(const size_t idw) const;
			/**
			 * getUsignId Метод получения идентификатора пользовательского признака
			 * @param name слово для которого нужно получить идентификатор
			 * @return     идентификатор пользовательского признака соответствующий слову
			 */
			const size_t getUsignId(const string & name) const;
		public:
			/**
			 * clear Метод очистки
			 */
			void clear();
			/**
			 * Метод очистки пользовательских признаков
			 */
			void clearUsigns();
			/**
			 * clearBadwords Метод очистки списка плохих слов
			 */
			void clearBadwords();
			/**
			 * clearGoodwords Метод очистки списка хороших слов
			 */
			void clearGoodwords();
		public:
			/**
			 * addBadword Метод добавления идентификатора похого слова в список
			 * @param idw идентификатор слова
			 */
			void addBadword(const size_t idw);
			/**
			 * addBadword Метод добавления похого слова в список
			 * @param word слово для добавления
			 */
			void addBadword(const string & word);
			/**
			 * addGoodword Метод добавления идентификатора хорошего слова в список
			 * @param idw идентификатор слова
			 */
			void addGoodword(const size_t idw);
			/**
			 * addGoodword Метод добавления хорошего слова в список
			 * @param word слово для добавления
			 */
			void addGoodword(const string & word);
			/**
			 * setOptions Метод установки опций
			 * @param options опции для установки
			 */
			void setOptions(const u_short options);
			/**
			 * setLogfile Метод установка файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile);
			/**
			 * setUsign Метод добавления признака пользователя
			 * @param name слово - обозначение признака
			 */
			void setUsign(const string & name);
			/**
			 * setUnknown Метод установки неизвестного слова
			 * @param word слово для добавления
			 */
			void setUnknown(const string & word);
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
			 * setUsignScript Метод установки скрипта обработки пользовательских признаков
			 * @param script скрипт python обработки пользовательских признаков
			 */
			void setUsignScript(const string & script);
			/**
			 * setSize Метод установки размера n-граммы
			 * @param size размер n-граммы
			 */
			void setSize(const u_short size = DEFNGRAM);
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet);
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
			 * addWord Метод добавления слова в словарь
			 * @param word слово для добавления
			 * @param idd  идентификатор документа
			 */
			void addWord(const wstring & word, const size_t idd = 0);
			/**
			 * addText Метод добавления текста для расчёта
			 * @param text текст который нужно добавить
			 * @param idd  идентификатор документа
			 */
			void addText(const string & text, const size_t idd = 0);
			/**
			 * setUsignMethod Метод добавления функции обработки пользовательского признака
			 * @param name слово - обозначение признака
			 * @param fn   внешняя функция обрабатывающая пользовательский признак
			 */
			void setUsignMethod(const string & name, function <bool (const string &, const string &)> fn);
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
			 * loadInfoVocab Метод загрузки бинарных информационных данных словаря
			 * @param buffer буфер бинарных информационных данных словаря
			 */
			void loadInfoVocab(const vector <char> & buffer);
			/**
			 * saveInfoVocab Метод сохранения бинарных информационных данных словаря
			 * @param buffer буфер бинарных информационных данных словаря
			 */
			void saveInfoVocab(vector <char> & buffer) const;
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
			/**
			 * saveVocab Метод извлечения данных словаря в бинарном виде
			 * @param callback функция обратного вызова
			 */
			void saveVocab(function <void (const vector <char> &, const u_short)> callback) const;
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
			 * modifyArpa Метод модификации arpa
			 * @param filename адрес файла для чтения
			 * @param flag     флаг модификации arpa
			 * @param status   функция вывода статуса модификации
			 */
			void modifyArpa(const string & filename, modify_t flag, function <void (const u_short)> status = nullptr);
			/**
			 * saveArpa Метод извлечения данных n-грамм в бинарном виде
			 * @param callback функция обратного вызова
			 * @param arpa     нужно добавить только данные arpa
			 */
			void saveArpa(function <void (const vector <char> &, const u_short)> callback, const bool arpa = false) const;
			/**
			 * prune Метод прунинга языковой модели
			 * @param threshold порог частоты прунинга
			 * @param mingram   значение минимальной n-граммы за которую нельзя прунить
			 * @param status    функция вывода статуса обучения
			 */
			void prune(const double threshold, const u_short mingram, function <void (const u_short)> status = nullptr) const;
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
			/**
			 * writeMap Метод записи карты последовательности в файл
			 * @param filename адрес map файла карты последовательности
			 * @param status   функция вывода статуса записи
			 * @param delim    разделитель последовательностей
			 */
			void writeMap(const string & filename, function <void (const u_short)> status = nullptr, const string & delim = "|") const;
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
			 * Toolkit Конструктор
			 */
			Toolkit(){};
			/**
			 * Toolkit Конструктор
			 * @param size размерность n-грамм
			 */
			Toolkit(const u_short size = DEFNGRAM);
			/**
			 * Toolkit Конструктор
			 * @param alphabet объект алфавита
			 * @param size     размерность n-грамм
			 */
			Toolkit(const alphabet_t * alphabet, const u_short size = DEFNGRAM);
			/**
			 * ~Toolkit Деструктор
			 */
			~Toolkit();
	} toolkit_t;
};

#endif // _ANYKS_TOOLKIT_

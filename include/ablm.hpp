/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_ABLM_
#define _ANYKS_ABLM_

/**
 * Стандартная библиотека
 */
#include <ctime>
#include <string>
#include <vector>
#include <bitset>
#include <cstring>
#include <functional>
/**
 * Наши модули
 */
#include <aspl.hpp>
#include <fsys.hpp>
#include <toolkit.hpp>
#include <alphabet.hpp>
#include <nlohmann/json.hpp>

// Устанавливаем область видимости
using namespace std;
// Активируем пространство имён json
using json = nlohmann::json;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * AbLM Класс бинарной языковой модели
	 */
	typedef class AbLM {
		public:
			// Основные флаги
			enum class flag_t : u_short {
				debug,       // Флаг режима отладки
				onlyArpa,    // Флаг хранения только данных arpa (без информации о встречаемости)
				expUsigns,   // Флаг хранения пользовательских признаков
				expOptions,  // Флаг хранения всех опций тулкита
				expPreword,  // Флаг хранения скрипта препроцессинга слов
				expBadwords, // Флаг хранения чёрного списка слов
				expGoodwords // Флаг хранения белого списка слов
			};
		private:
			/**
			 * Meta структура метаданных
			 */
			typedef struct Meta {
				size_t date;         // Дата генерации словаря
				string name;         // Название словаря
				string author;       // Автор словаря
				string lictype;      // Тип лицензии под которой распространяется словарь
				string lictext;      // Текст лицензии под которой распространяется словарь
				string contacts;     // Контактные данные автора
				string password;     // Пароль шифрования словаря
				string copyright;    // Копирайт владельца словаря
				aspl_t::types_t aes; // Тип шифрования словаря
				/**
				 * Meta Конструктор
				 */
				Meta() : date(time(nullptr)), name(""), author(""), lictype(""), lictext(""), contacts(""), password(""), copyright(""), aes(aspl_t::types_t::aes128) {}
			} meta_t;
		private:
			// Метаданные словаря
			meta_t meta = {};
			// Флаги параметров
			bitset <7> flags;
			// Адрес файла словаря
			string filename = "";
			// Объект бинарного контейнера
			aspl_t * aspl = nullptr;
			// Объект тулкита
			toolkit_t * toolkit = nullptr;
			// Объект log файла
			const char * logfile = nullptr;
			// Объект словаря
			alphabet_t * alphabet = nullptr;
		private:
			/**
			 * flag_t Метод проверки наличия флага
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isFlag(const flag_t flag) const;
		public:
			/**
			 * write Метод записи данных словаря в файл
			 * @param status статус расёта
			 */
			const bool write(function <void (const u_short)> status = nullptr);
			/**
			 * read Метод чтения данных словаря из файла
			 * @param status статус расёта
			 * @param info   выводить только информацию о словаре
			 */
			const bool read(function <void (const u_short)> status = nullptr, const bool info = false);
		public:
			/**
			 * Метод инициализации
			 */
			void init();
			/**
			 * clear Метод очистки данных словаря
			 */
			void clear();
			/**
			 * info Метод вывода инормационных данных словаря
			 */
			void info() const;
			/**
			 * setMeta Метод установки метаданных в формате json
			 * @param meta метаданные в формате json
			 */
			void setMeta(const json & meta);
			/**
			 * setFlag Метод установки флага модуля
			 * @param flag флаг для установки
			 */
			void setFlag(const flag_t flag);
			/**
			 * unsetFlag Метод отключения флага модуля
			 * @param flag флаг для отключения
			 */
			void unsetFlag(const flag_t flag);
			/**
			 * setLogfile Метод установка файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile);
			/**
			 * setToolkit Метод установки объекта тулкита
			 * @param toolkit объект тукита для установки
			 */
			void setToolkit(toolkit_t * toolkit);
			/**
			 * setAlphabet Метод установки объекта словаря
			 * @param alphabet объект словаря для установки
			 */
			void setAlphabet(alphabet_t * alphabet);
			/**
			 * setFilename Метод установки адреса файла словаря
			 * @param filename адрес файла словаря
			 */
			void setFilename(const string & filename);
		public:
			/**
			 * AbLM Конструктор
			 */
			AbLM(){};
			/**
			 * AbLM Конструктор
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			AbLM(const char * logfile);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 */
			AbLM(const string & filename);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param meta     метаданные в формате json
			 */
			AbLM(const string & filename, const json & meta);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param toolkit  объект тукита для установки
			 * @param alphabet объект словаря для установки
			 */
			AbLM(toolkit_t * toolkit, alphabet_t * alphabet);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param toolkit  объект тукита для установки
			 */
			AbLM(const string & filename, toolkit_t * toolkit);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param logifle  адрес файла для вывода отладочной информации
			 */
			AbLM(const string & filename, const char * logfile);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param alphabet объект словаря для установки
			 */
			AbLM(const string & filename, alphabet_t * alphabet);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param meta     метаданные в формате json
			 * @param toolkit  объект тукита для установки
			 */
			AbLM(const string & filename, const json & meta, toolkit_t * toolkit);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param meta     метаданные в формате json
			 * @param alphabet объект словаря для установки
			 */
			AbLM(const string & filename, const json & meta, alphabet_t * alphabet);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param toolkit  объект тукита для установки
			 * @param alphabet объект словаря для установки
			 */
			AbLM(const string & filename, toolkit_t * toolkit, alphabet_t * alphabet);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param meta     метаданные в формате json
			 * @param toolkit  объект тукита для установки
			 * @param logifle  адрес файла для вывода отладочной информации
			 */
			AbLM(const string & filename, const json & meta, toolkit_t * toolkit, const char * logfile);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param meta     метаданные в формате json
			 * @param toolkit  объект тукита для установки
			 * @param alphabet объект словаря для установки
			 */
			AbLM(const string & filename, const json & meta, toolkit_t * toolkit, alphabet_t * alphabet);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param toolkit  объект тукита для установки
			 * @param alphabet объект словаря для установки
			 * @param logifle  адрес файла для вывода отладочной информации
			 */
			AbLM(const string & filename, toolkit_t * toolkit, alphabet_t * alphabet, const char * logfile);
			/**
			 * AbLM Конструктор
			 * @param filename адрес файла словаря
			 * @param meta     метаданные в формате json
			 * @param toolkit  объект тукита для установки
			 * @param alphabet объект словаря для установки
			 * @param logifle  адрес файла для вывода отладочной информации
			 */
			AbLM(const string & filename, const json & meta, toolkit_t * toolkit, alphabet_t * alphabet, const char * logfile);
			/**
			 * ~AbLM Деструктор
			 */
			~AbLM();
	} ablm_t;
};

#endif // _ANYKS_ABLM_

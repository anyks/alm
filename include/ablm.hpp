/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_ABLM__
#define __ANYKS_ABLM__

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
#include <alm.hpp>
#include <aspl.hpp>
#include <fsys.hpp>
#include <toolkit.hpp>
#include <alphabet.hpp>
#include <tokenizer.hpp>

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
				debug,        // Флаг режима отладки
				onlyArpa,     // Флаг экспорта только данных arpa (без информации о встречаемости)
				expAllData,   // Флаг экспорта всех данных словаря
				expOptions,   // Флаг экспорта всех опций тулкита
				expPreword,   // Флаг экспорта скрипта препроцессинга слов
				expAbbrevs,   // Флаг экспорта списка аббревиатур
				expDomZones,  // Флаг экспорта списка доменных имён
				expBadwords,  // Флаг экспорта чёрного списка слов
				expGoodwords, // Флаг экспорта белого списка слов
				expUserTokens // Флаг экспорта пользовательских токенов
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
			meta_t meta;
			// Флаги параметров
			bitset <10> flags;
			// Адрес файла словаря
			string filename = "";
		private:
			// Объект языковой модели
			alm_t * alm = nullptr;
			// Объект бинарного контейнера
			aspl_t * aspl = nullptr;
			// Объект тулкита
			toolkit_t * toolkit = nullptr;
			// Объект словаря
			alphabet_t * alphabet = nullptr;
			// Объект токенизатора
			tokenizer_t * tokenizer = nullptr;
		private:
			// Объект log файла
			const char * logfile = nullptr;
		private:
			/**
			 * flag_t Метод проверки наличия флага
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isFlag(const flag_t flag) const noexcept;
		public:
			/**
			 * write Метод записи данных словаря в файл
			 * @param status статус записи данных в бинарный контейнер
			 */
			const bool write(function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * readAlm Метод чтения данных из файла в объект alm
			 * @param status статус чтения данных из бинарного контейнера
			 */
			const bool readAlm(function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * readToolkit Метод чтения данных из файла в объект toolkit
			 * @param status статус чтения данных из бинарного контейнера
			 */
			const bool readToolkit(function <void (const u_short)> status = nullptr) noexcept;
		public:
			/**
			 * Метод инициализации
			 */
			void init();
			/**
			 * clear Метод очистки данных словаря
			 */
			void clear() noexcept;
			/**
			 * info Метод вывода инормационных данных словаря
			 */
			void info() const noexcept;
		public:
			/**
			 * setAlm Метод установки объекта языковой модели
			 * @param alm объект языковой модели для установки
			 */
			void setAlm(alm_t * alm) noexcept;
			/**
			 * setAES Метод установки размера шифрования
			 * @param aes размер шифрования для установки
			 */
			void setAES(aspl_t::types_t aes) noexcept;
			/**
			 * setFlag Метод установки флага модуля
			 * @param flag флаг для установки
			 */
			void setFlag(const flag_t flag) noexcept;
			/**
			 * unsetFlag Метод отключения флага модуля
			 * @param flag флаг для отключения
			 */
			void unsetFlag(const flag_t flag) noexcept;
			/**
			 * setName Метод установки названия словаря
			 * @param name название словаря для установки
			 */
			void setName(const string & name) noexcept;
			/**
			 * setAuthor Метод установки автора словаря
			 * @param author автор словаря для установки
			 */
			void setAuthor(const string & author) noexcept;
			/**
			 * setLogfile Метод установки файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile) noexcept;
			/**
			 * setToolkit Метод установки объекта тулкита
			 * @param toolkit объект тукита для установки
			 */
			void setToolkit(toolkit_t * toolkit) noexcept;
			/**
			 * setLictype Метод установки типа лицензии
			 * @param lictype тип лицензии для установки
			 */
			void setLictype(const string & lictype) noexcept;
			/**
			 * setLictext Метод установки текста лицензии
			 * @param lictext текст лицензии для установки
			 */
			void setLictext(const string & lictext) noexcept;
			/**
			 * setAlphabet Метод установки объекта словаря
			 * @param alphabet объект словаря для установки
			 */
			void setAlphabet(alphabet_t * alphabet) noexcept;
			/**
			 * setContacts Метод установки контактов автора
			 * @param contacts контакты автора для установки
			 */
			void setContacts(const string & contacts) noexcept;
			/**
			 * setPassword Метод установки пароля словаря
			 * @param password пароль словаря для установки
			 */
			void setPassword(const string & password) noexcept;
			/**
			 * setTokenizer Метод установки токенизатора
			 * @param tokenizer объект токенизатора
			 */
			void setTokenizer(tokenizer_t * tokenizer) noexcept;
			/**
			 * setFilename Метод установки адреса файла словаря
			 * @param filename адрес файла словаря
			 */
			void setFilename(const string & filename) noexcept;
			/**
			 * setCopyright Метод установки копирайта автора
			 * @param copyright копирайт автора для установки
			 */
			void setCopyright(const string & copyright) noexcept;
		public:
			/**
			 * AbLM Конструктор
			 */
			AbLM() noexcept {};
			/**
			 * AbLM Конструктор
			 * @param filename  адрес файла словаря
			 * @param alm       объект языковой модели для установки
			 * @param alphabet  объект словаря для установки
			 * @param tokenizer объект токенизатора
			 * @param logifle   адрес файла для вывода отладочной информации
			 */
			AbLM(const string & filename, alm_t * alm, alphabet_t * alphabet, tokenizer_t * tokenizer, const char * logfile) noexcept;
			/**
			 * AbLM Конструктор
			 * @param filename  адрес файла словаря
			 * @param toolkit   объект тукита для установки
			 * @param alphabet  объект словаря для установки
			 * @param tokenizer объект токенизатора
			 * @param logifle   адрес файла для вывода отладочной информации
			 */
			AbLM(const string & filename, toolkit_t * toolkit, alphabet_t * alphabet, tokenizer_t * tokenizer, const char * logfile) noexcept;
			/**
			 * ~AbLM Деструктор
			 */
			~AbLM() noexcept;
	} ablm_t;
};

#endif // __ANYKS_ABLM__

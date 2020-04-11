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
#include <alm.hpp>
#include <aspl.hpp>
#include <fsys.hpp>
#include <toolkit.hpp>
#include <alphabet.hpp>
#include <tokenizer.hpp>
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
			meta_t meta = {};
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
			 * @param status статус расёта
			 */
			const bool write(function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * readAlm Метод чтения данных словаря из файла в объект toolkit
			 * @param status статус расёта
			 * @param info   выводить только информацию о словаре
			 */
			const bool readAlm(function <void (const u_short)> status = nullptr, const bool info = false) noexcept;
			/**
			 * readToolkit Метод чтения данных словаря из файла в объект toolkit
			 * @param status статус расёта
			 * @param info   выводить только информацию о словаре
			 */
			const bool readToolkit(function <void (const u_short)> status = nullptr, const bool info = false) noexcept;
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
			/**
			 * setAlm Метод установки объекта языковой модели
			 * @param alm объект языковой модели для установки
			 */
			void setAlm(alm_t * alm) noexcept;
			/**
			 * setMeta Метод установки метаданных в формате json
			 * @param meta метаданные в формате json
			 */
			void setMeta(const json & meta) noexcept;
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
			 * setAlphabet Метод установки объекта словаря
			 * @param alphabet объект словаря для установки
			 */
			void setAlphabet(alphabet_t * alphabet) noexcept;
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

#endif // _ANYKS_ABLM_

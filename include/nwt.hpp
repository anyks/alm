/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_NWT_
#define _ANYKS_NWT_

/**
 * Стандартная библиотека
 */
#include <set>
#include <regex>
#include <iostream>
#include <sys/types.h>
/**
 * Наши модули
 */
#include <app/alm.hpp>

// Устанавливаем область видимости
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Uri Структура списка параметров URI
	 */
	typedef class Uri {
		public:
			// Типы n-грамм
			enum class types_t : u_short {
				mac = 0x0,    // MAC адрес
				ipv4 = 0x1,   // IPv4 адрес
				ipv6 = 0x2,   // IPv6 адрес
				null = 0x3,   // Тип не определён
				email = 0x4,  // Электронная почта
				wrong = 0x5,  // Сломанный адрес
				domain = 0x6, // Доменное имя
				network = 0x7 // Параметры сети
			};
		public:
			/**
			 * Data Структура ответа
			 */
			typedef struct Data {
				// Основные параметры
				types_t type;     // Тип URI
				wstring uri;      // Полный URI
				wstring port;     // Порт запроса если существует
				wstring data;     // Полное доменное имя, мак адрес, ip адрес или сеть
				wstring path;     // Путь запроса если существует
				wstring user;     // Ник пользователя (для электронной почты)
				wstring domain;   // Домен верхнего уровня если существует
				wstring params;   // Параметры запроса если существуют
				wstring protocol; // Протокол запроса если существует
				/**
				 * Data Конструктор
				 */
				Data() : type(types_t::null), uri(L""), data(L""), path(L""), domain(L""), params(L""), protocol(L"") {}
			} data_t;
		private:
			// Список букв разрешенных в последовательности
			wstring letters = L"";
		private:
			// Распарсенные данные
			data_t data;
			// Списки доменных зон интернета
			set <wstring> general, national, user;
		public:
			/**
			 * get Метод получения распарсенных данных
			 * @return параметры полученные в результате парсинга
			 */
			const data_t & get() noexcept;
			/**
			 * getZones Метод извлечения списка пользовательских зон интернета
			 */
			const set <wstring> & getZones() const noexcept;
		public:
			/**
			 * clear Метод очистки результатов парсинга
			 */
			void clear() noexcept;
			/**
			 * parse Метод парсинга URI строки
			 * @param text текст для парсинга
			 */
			void parse(const wstring & text) noexcept;
			/**
			 * setZone Метод установки пользовательской зоны
			 * @param zone пользовательская зона
			 */
			void setZone(const wstring & zone) noexcept;
			/**
			 * setLetters Метод добавления букв алфавита
			 * @param letters список букв алфавита
			 */
			void setLetters(const wstring & letters) noexcept;
			/**
			 * setZones Метод установки списка пользовательских зон
			 * @param zones список доменных зон интернета
			 */
			void setZones(const set <wstring> & zones) noexcept;
			/**
			 * Uri Конструктор
			 * @param letters список букв алфавита
			 * @param text    текст для парсинга
			 */
			Uri(const wstring & letters = L"", const wstring & text = L"") noexcept;
	} uri_t;
};

#endif // _ANYKS_NWT_

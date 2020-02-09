/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_ID_WORD_
#define _ANYKS_ID_WORD_

/**
 * Стандартная библиотека
 */
#include <cmath>
#include <string>
#include <vector>
#include <limits>
#include <sys/types.h>
/**
 * Наши модули
 */
#include <general.hpp>
#include <alphabet.hpp>
#include <BigIntegerLibrary.hh>

// Устанавливаем область видимости
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Idw Класс модуля идентификатора языка
	 */
	typedef class Idw {
		private:
			// Список модулей для каждой буквы алфавита
			vector <size_t> xs;
		private:
			// Модуль вектора
			size_t modulus = 0;
			// Смещение в алфавите
			u_short offset = 0;
			// Список букв разрешенных в последовательности
			wstring letters = L"¶";
			// Объект основного алфавита
			const alphabet_t * alphabet = nullptr;
		public:
			/**
			 * setOffset Метод установки смещения в алфавите
			 * @param offset смещение в алфавите
			 */
			void setOffset(const u_short offset);
			/**
			 * get Метод генерирования идентификатора слова
			 * @param  word слово для генерации
			 * @return      идентификатор слова
			 */
			const size_t get(const wstring & word) const;
			/**
			 * set Метод установки алфавита
			 * @param alphabet объект алфавита
			 * @param offset   смещение в алфавите
			 */
			void set(const alphabet_t * alphabet, const u_short offset = 0);
		public:
			/**
			 * Idw Конструктор
			 */
			Idw(){};
			/**
			 * Idw Конструктор
			 * @param offset смещение в алфавите
			 */
			Idw(const u_short offset);
			/**
			 * Idw Конструктор
			 * @param alphabet объект алфавита
			 */
			Idw(const alphabet_t * alphabet);
			/**
			 * Idw Конструктор
			 * @param alphabet объект алфавита
			 * @param offset   смещение в алфавите
			 */
			Idw(const alphabet_t * alphabet, const u_short offset);
	} idw_t;
};

#endif // _ANYKS_ID_WORD_

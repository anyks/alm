/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_ID_WORD__
#define __ANYKS_ID_WORD__

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
#include <app/alm.hpp>
#include <alphabet.hpp>
#include <cityhash/city.h>
#include <bigint/BigIntegerLibrary.hh>

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
			// Модули векторов
			size_t mod;
			// Смещение в алфавите
			u_short offset;
			// Список букв разрешенных в последовательности
			wstring letters;
			// Список модулей для каждой буквы алфавита
			vector <size_t> xs;
			// Объект основного алфавита
			const alphabet_t * alphabet;
		public:
			// Получаем максимальное значение идентификатора
			static constexpr size_t NIDW = numeric_limits <size_t>::max();
		public:
			/**
			 * setOffset Метод установки смещения в алфавите
			 * @param offset смещение в алфавите
			 */
			void setOffset(const u_short offset) noexcept;
			/**
			 * get Метод генерирования идентификатора слова
			 * @param  word слово для генерации
			 * @return      идентификатор слова
			 */
			const size_t get(const wstring & word) const noexcept;
			/**
			 * get Метод генерирования идентификатора последовательности
			 * @param  seq последовательность для генерации
			 * @return     идентификатор последовательности
			 */
			const size_t get(const vector <size_t> & seq) const noexcept;
			/**
			 * set Метод установки алфавита
			 * @param alphabet объект алфавита
			 * @param offset   смещение в алфавите
			 */
			void set(const alphabet_t * alphabet, const u_short offset = 0) noexcept;
		public:
			/**
			 * Idw Конструктор
			 */
			Idw() noexcept;
			/**
			 * Idw Конструктор
			 * @param offset смещение в алфавите
			 */
			Idw(const u_short offset) noexcept;
			/**
			 * Idw Конструктор
			 * @param alphabet объект алфавита
			 */
			Idw(const alphabet_t * alphabet) noexcept;
			/**
			 * Idw Конструктор
			 * @param alphabet объект алфавита
			 * @param offset   смещение в алфавите
			 */
			Idw(const alphabet_t * alphabet, const u_short offset) noexcept;
	} idw_t;
};

#endif // __ANYKS_ID_WORD__

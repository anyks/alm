/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_PROGRESS__
#define __ANYKS_PROGRESS__

/**
 * Стандартная библиотека
 */
#include <set>
#include <ctime>
#include <string>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>

// Устанавливаем область видимости
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Progress Класс работы с прогресс баром
	 */
	typedef class Progress {
		private:
			// Время начала
			time_t startTime = 0;
			// Текущее значение прогресса
			u_short progress = 101;
			// Текущее значение индикатора
			const char * litem = "|";
			// Заголовки и описание при работе прогресса
			string title1 = "", title2 = "", desc = "";
		private:
			/**
			 * date Метод вывода даты и времени в текстовом виде
			 * @param seconds количество секунд из которых нужно сформировать дату
			 * @return        строка с датой и временем
			 */
			const string date(const time_t seconds) const noexcept;
		public:
			/**
			 * clear Метод сброса данных
			 */
			void clear() noexcept;
			/**
			 * status Метод вывод статуса в текстовом виде
			 * @param status текущий статус
			 */
			void status(const u_short status = 0) noexcept;
			/**
			 * update Метод обновления прогресса
			 * @param status текущий статус
			 */
			void update(const u_short status = 0) noexcept;
			/**
			 * description Метод установки описания работы
			 * @param text текст описания работы
			 */
			void description(const string & text) noexcept;
			/**
			 * title Метод установки заголовка
			 * @param start  заголовок при работе прогресса
			 * @param finish заголовок при завершении прогресса
			 */
			void title(const string & start, const string & finish = "") noexcept;
			/**
			 * dimension Метод получения типа оставшегося времени
			 * @param sec время для расчёта в секундах
			 * @return    значение оставшегося времени
			 */
			const pair <time_t, string> dimension(const time_t sec) const noexcept;
	} progress_t;
};

#endif // __ANYKS_PROGRESS__

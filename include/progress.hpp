/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_PROGRESS_
#define _ANYKS_PROGRESS_

/**
 * Стандартная библиотека
 */
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
	} progress_t;
};

#endif // _ANYKS_PROGRESS_

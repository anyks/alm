/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <progress.hpp>

/**
 * dimension Метод получения типа оставшегося времени
 * @param sec время для расчёта в секундах
 * @return    значение оставшегося времени
 */
const pair <time_t, string> anyks::Progress::dimension(const time_t sec) const noexcept {
	// Диапазон единичных значений
	set <time_t> dipaz = {
		1, 21, 31, 41, 51, 61, 71, 81, 91, 101, 121, 131, 141,
		151, 161, 171, 181, 191, 201, 221, 231, 241, 251, 161,
		271, 281, 291, 301, 321, 331, 341, 351, 361, 371, 381, 391
	};
	// Получаем описание времени
	const string description = (dipaz.count(sec) > 0 ? "second" : "seconds");
	// Результат работы функции
	pair <time_t, string> result = {sec, description};
	// Если секунды переданы
	if(result.first > 0){
		// Если это минуты, переводим секунды в минуты
		if(result.first >= 60){
			// Переводим секунды в минуты
			result.first /= 60;
			// Меняем индикатор размерности
			result.second = (dipaz.count(result.first) > 0 ? "minute" : "minutes");
			// Если это часы
			if(result.first >= 60){
				// Переводим минуты в часы
				result.first /= 60;
				// Меняем индикатор размерности
				result.second = (dipaz.count(result.first) > 0 ? "hour" : "hours");
				// Если это дни
				if(result.first >= 24){
					// Переводим минуты в дни
					result.first /= 24;
					// Меняем индикатор размерности
					result.second = (dipaz.count(result.first) > 0 ? "day" : "days");
					// Если это месяцы
					if(result.first >= 31){
						// Переводим минуты в месяцы
						result.first /= 31;
						// Меняем индикатор размерности
						result.second = (dipaz.count(result.first) > 0 ? "month" : "months");
						// Если это годы
						if(result.first >= 12){
							// Переводим минуты в годы
							result.first /= 12;
							// Меняем индикатор размерности
							result.second = (dipaz.count(result.first) > 0 ? "year" : "years");
						}
					}
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * clear Метод сброса данных
 */
void anyks::Progress::clear() noexcept {
	// Сбрасываем индикатор процесса
	this->litem = "|";
	// Обнуляем оставшееся время
	this->startTime = 0;
	// Сбрасываем текущее значение процесса
	this->progress = 101;
	// Очищаем описание прогресса
	this->desc.clear();
	// Очищаем заголовки
	this->title1.clear();
	this->title2.clear();
}
/**
 * status Метод вывод статуса в текстовом виде
 * @param status текущий статус
 */
void anyks::Progress::status(const u_short status) noexcept {
	// Если статус не 100%
	if((status < 100) && (this->progress != status)){
		// Пишем процент загрузки
		if(!this->title1.empty()){
			// Выводим заголовок индикатора загрузки
			if((status == 0) || (this->startTime == 0)) printf("\x1B[36m\x1B[1m%s:\x1B[0m %u%%\r\n", this->title1.c_str(), status);
			// Выводим примерное время завершения работы
			else if(this->startTime > 0) {
				// Выполняем расчёт оставшихся секунд
				// Получаем оставшееся время
				auto dimension = this->dimension((100 - status) * ((time(nullptr) - this->startTime) / double(status)));
				// Если описание передано
				if(!this->desc.empty()){
					// Выводим заголовок индикатора загрузки с описанием
					printf("\x1B[36m\x1B[1m%s:\x1B[0m %u%% [%ld %s]: %s\r\n", this->title1.c_str(), status, dimension.first, dimension.second.c_str(), this->desc.c_str());
				// Выводим заголовок индикатора загрузки без описания
				} else printf("\x1B[36m\x1B[1m%s:\x1B[0m %u%% [%ld %s]\r\n", this->title1.c_str(), status, dimension.first, dimension.second.c_str());
			}
		}
		// Запоминаем текущий статус
		this->progress = status;
		// Запоминаем время начала
		if(status == 0) this->startTime = time(nullptr);
	// Если статус перешёл 100%
	} else if((status >= 100) && !this->title2.empty()) {
		// Выводим сообщение результата
		printf("\r\n\x1B[36m\x1B[1mResult:\x1B[0m %s\r\n\r\n", this->title2.c_str());
		// Сбрасываем параметры прогресс-бара
		this->clear();
	}
}
/**
 * update Метод обновления прогресса
 * @param status текущий статус
 */
void anyks::Progress::update(const u_short status) noexcept {
	// Если статус не 100%
	if((status < 100) && (this->progress != status)){
		// Структура размеров экрана консоли
		struct winsize w;
		// Очищаем консоль
		printf("\033c\x1B[0m");
		// Получаем текущий размер консоли
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
		// Пишем процент загрузки
		if(!this->title1.empty()){
			// Выводим заголовок индикатора загрузки
			if((status == 0) || (this->startTime == 0)) printf("\r\n \x1B[36m\x1B[1m%s:\x1B[0m \x1B[32m\x1B[1m%u%%\x1B[0m\r\n ", this->title1.c_str(), status);
			// Выводим примерное время завершения работы
			else if(this->startTime > 0) {
				// Получаем оставшееся время
				auto dimension = this->dimension((100 - status) * ((time(nullptr) - this->startTime) / double(status)));
				// Выводим заголовок индикатора загрузки
				printf("\r\n \x1B[36m\x1B[1m%s:\x1B[0m \x1B[32m\x1B[1m%u%%\x1B[0m \x1B[33m\x1B[1m[%ld %s]\x1B[0m\r\n ", this->title1.c_str(), status, dimension.first, dimension.second.c_str());
			}
		}
		// Запоминаем текущий статус
		this->progress = status;
		// Запоминаем время начала
		if(status == 0){
			// Запоминаем текущее время
			this->startTime = time(nullptr);
			// Отображаем оставшиеся знаки загрузки
			for(size_t i = 0; i < size_t(w.ws_col - 4); i++) printf("\e[47m \x1B[0m");
			// Отображаем индикатор процесса
			printf(" \x1B[33m\x1B[1m%s\x1B[0m\e[0m\r\n\r\n", this->litem);
		// Если статус больше 0
		} else {
			// Выполняем расчёт текущей позиции
			const size_t pos = (((status <= 100 ? status * w.ws_col : 100 * w.ws_col) / double(100.0)) - 2);
			// Отображаем индикатор загрузки
			for(size_t i = 0; i < pos; i++) printf("\e[42m \x1B[0m");
			// Отображаем оставшиеся знаки загрузки
			for(size_t i = 0; i < ((w.ws_col - pos) - 4); i++) printf("\e[47m \x1B[0m");
			// Отображаем индикатор процесса
			printf(" \x1B[33m\x1B[1m%s\x1B[0m\e[0m\r\n", this->litem);
			// Меняем значение индикатора
			if(strcmp(this->litem, "|") == 0) this->litem = "/";
			else if(strcmp(this->litem, "/") == 0) this->litem = "--";
			else if(strcmp(this->litem, "--") == 0) this->litem = "\\";
			else if(strcmp(this->litem, "\\") == 0) this->litem = "|";
			// Если описание передано
			if(!this->desc.empty()) printf(" \x1B[34m\x1B[1m%s\x1B[0m\r\n\r\n", this->desc.c_str());
			// Иначе просто выводим перенос строки
			else printf("%s", "\r\n");
		}
	// Если статус перешёл 100%
	} else if((status >= 100) && !this->title2.empty()) {
		// Выводим сообщение результата
		printf("\033c\x1B[0m\r\n \x1B[36m\x1B[1mResult:\x1B[0m %s\r\n\r\n", this->title2.c_str());
		// Сбрасываем параметры прогресс-бара
		this->clear();
	}
}
/**
 * description Метод установки описания работы
 * @param text текст описания работы
 */
void anyks::Progress::description(const string & text) noexcept {
	// Если текст передан, устанавливаем описание работы
	this->desc = text;
}
/**
 * title Метод установки заголовка
 * @param start  заголовок при работе прогресса
 * @param finish заголовок при завершении прогресса
 */
void anyks::Progress::title(const string & start, const string & finish) noexcept {
	// Если заголовок прогресса передан
	if(!start.empty()) this->title1 = start;
	// Если заголовок завершения прогресса передан
	if(!finish.empty()) this->title2 = finish;
}

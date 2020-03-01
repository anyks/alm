/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_COLLECTOR_
#define _ANYKS_COLLECTOR_

/**
 * Стандартная библиотека
 */
#include <cmath>
#include <atomic>
/**
 * Наши модули
 */
#include <toolkit.hpp>
#include <progress.hpp>
#include <threadpool.hpp>

using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Collector Класс для с общучением в многопоточном режиме
	 */
	typedef class Collector {
		private:
			// Создаем тредпул
			tpool_t tpool;
			// Объект тулкита
			toolkit_t * toolkit = nullptr;
			// Количество потоков для работы
			size_t threads = thread::hardware_concurrency();
		public:
			/**
			 * setToolkit Метод установки объекта тулкита
			 * @param toolkit объект тулкита
			 */
			void setToolkit(toolkit_t * toolkit);
			/**
			 * setThreads Метод установки количества потоков
			 * @param threads количество потоков для работы
			 */
			void setThreads(const size_t threads = 0);
		public:
			/**
			 * readFile Метод запуска чтение текстового корпуса из одного файла
			 * @param filename адрес файла для чтения
			 * @param status   функция вывода статуса обучения
			 */
			void readFile(const string & filename, function <void (const u_short)> status = nullptr);
			/**
			 * readDir Метод запуска чтение текстового корпуса из каталога
			 * @param path   адрес каталога для чтения
			 * @param ext    расширение файла для поиска в каталоге
			 * @param status функция вывода статуса обучения
			 */
			void readDir(const string & path, const string & ext, function <void (const string &, const u_short)> status = nullptr);
		public:
			/**
			 * Конструктор
			 */
			Collector(){}
			/**
			 * Конструктор
			 * @param toolkit объект тулкита
			 */
			Collector(toolkit_t * toolkit);
			/**
			 * Конструктор
			 * @param toolkit объект тулкита
			 */
			Collector(const size_t threads);
			/**
			 * Конструктор
			 * @param threads количество потоков для работы
			 * @param toolkit объект тулкита
			 */
			Collector(const size_t threads, toolkit_t * toolkit);
	} collector_t;
};

#endif // _ANYKS_COLLECTOR_

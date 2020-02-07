/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_PYTHON_
#define _ANYKS_PYTHON_

/**
 * Стандартная библиотека
 */
#include <map>
#include <string>
#include <vector>
#include <Python.h>
#include <sys/stat.h>
#include <sys/types.h>
/**
 * Наши модули
 */
#include <idw.hpp>
#include <fsys.hpp>
#include <alphabet.hpp>

using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Python Класс для работы с питоном
	 */
	typedef class Python {
		private:
			/**
			 * Script Структура параметров скрипта
			 */
			typedef struct Script {
				u_short count;		// Количество аргументов функции
				PyObject * run;		// Функция запуска работы скрипта
				PyObject * init;	// Функция инициализации скрипта
				PyObject * module;	// Рабочий модуль
				/**
				 * Script конструктор
				 */
				Script() : count(0), run(nullptr), init(nullptr), module(nullptr) {}
			} script_t;
		private:
			// Флаг режима отладки
			bool debug;
			// Генератор идентификаторов
			const idw_t * idw;
			// Список скриптов
			map <size_t, script_t> scripts;
		public:
			/**
			 * clear Метод очистки списка скриптов
			 */
			void clear();
			/**
			 * setDebug Метод установки режима отладки
			 */
			void setDebug();
			/**
			 * unsetDebug Метод отключения режима отладки
			 */
			void unsetDebug();
			/**
			 * setIdw Метод установки генератора идентификаторов
			 * @param idw модуль генератора идентификаторов
			 */
			void setIdw(const idw_t * idw);
			/**
			 * remove Метод удаления добавленного скрипта по его имени
			 * @param sid идентификатор скрипта
			 */
			void remove(const size_t sid);
		public:
			/**
			 * empty Проверка на пустое количество скриптов
			 * @return результат проверки
			 */
			const bool empty() const;
			/**
			 * count Метод определения количества добавленных скриптов
			 * @return количество добавленных скриптов
			 */
			const size_t count() const;
			/**
			 * add Метод добавления скрипта
			 * @param script адрес скрипта
			 * @param args   количество аргументов передаваемых в функцию запуска
			 * @return       идентификатор добавленного скрипта
			 */
			const size_t add(const string & script, const u_short args = 0);
			/**
			 * run Метод запуска скрипта
			 * @param  sid  идентификатор скрипта
			 * @param  args список аргументов передаваемых в скрипт
			 * @param  arr  массив аргументов для передачи в скрипт
			 * @return      результат выполнения скрипта
			 */
			const wstring run(const size_t sid, const vector <string> & args = {}, const vector <string> & arr = {}) const;
		public:
			/**
			 * Python Конструктор
			 */
			Python();
			/**
			 * Python Конструктор
			 * @param idw указатель на генератор идентификаторов
			 */
			Python(const idw_t * idw);
			/**
			 * ~Python Деструктор
			 */
			~Python();
	} python_t;
};

#endif // _ANYKS_PYTHON_

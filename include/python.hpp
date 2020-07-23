/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_PYTHON__
#define __ANYKS_PYTHON__

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
#include <tokenizer.hpp>

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
			bool debug = false;
			// Список скриптов
			map <size_t, script_t> scripts;
			// Генератор идентификаторов
			const tokenizer_t * tokenizer = nullptr;
		public:
			/**
			 * clear Метод очистки списка скриптов
			 */
			void clear() noexcept;
			/**
			 * setDebug Метод установки режима отладки
			 */
			void setDebug() noexcept;
			/**
			 * unsetDebug Метод отключения режима отладки
			 */
			void unsetDebug() noexcept;
			/**
			 * remove Метод удаления добавленного скрипта по его имени
			 * @param sid идентификатор скрипта
			 */
			void remove(const size_t sid) noexcept;
			/**
			 * setTokenizer Метод установки токенизатора
			 * @param tokenizer указатель на токенизатор
			 */
			void setTokenizer(const tokenizer_t * tokenizer) noexcept;
		public:
			/**
			 * empty Проверка на пустое количество скриптов
			 * @return результат проверки
			 */
			const bool empty() const noexcept;
			/**
			 * count Метод определения количества добавленных скриптов
			 * @return количество добавленных скриптов
			 */
			const size_t count() const noexcept;
			/**
			 * add Метод добавления скрипта
			 * @param script адрес скрипта
			 * @param args   количество аргументов передаваемых в функцию запуска
			 * @return       идентификатор добавленного скрипта
			 */
			const size_t add(const string & script, const u_short args = 0) noexcept;
			/**
			 * run Метод запуска скрипта
			 * @param  sid  идентификатор скрипта
			 * @param  args список аргументов передаваемых в скрипт
			 * @param  arr  массив аргументов для передачи в скрипт
			 * @return      результат выполнения скрипта
			 */
			const wstring run(const size_t sid, const vector <string> & args = {}, const vector <string> & arr = {}) const noexcept;
		public:
			/**
			 * Python Конструктор
			 */
			Python() noexcept;
			/**
			 * Python Конструктор
			 * @param tokenizer указатель на токенизатор
			 */
			Python(const tokenizer_t * tokenizer) noexcept;
			/**
			 * ~Python Деструктор
			 */
			~Python() noexcept;
	} python_t;
};

#endif // __ANYKS_PYTHON__

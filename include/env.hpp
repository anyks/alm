/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_ENV__
#define __ANYKS_ENV__

/**
 * Стандартная библиотека
 */
#include <vector>
#include <string>
#include <limits>
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <unistd.h>
/**
 * Наши модули
 */
#include <alphabet.hpp>
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
	 * Env Класс модуля работы с переменными окружения
	 */
	typedef class Env {
		private:
			// Флаг автоматического чтения текстовой переменной
			bool aread = false;
			/**
			 * Название переменной содержащей текст
			 * и префикс переменной окружения
			 */
			string text, prefix;
			// Переменная окружения
			mutable string variable;
			// Список полученных переменных окружения
			unordered_map <string, string> data;
		private:
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
		private:
			/**
			 * env Метод извлечения переменной окружения
			 * @param  key переменная окружения
			 * @return     значение переменной
			 */
			const string env(const string & key) const noexcept;
		public:
			/**
			 * getJson Метод получения конфигурационных данных в формате JSON
			 * @return конфигурационные данные
			 */
			const json getJson() const noexcept;
			/**
			 * count Метод извлечения количества аргументов
			 * @return количество полученных аргументов
			 */
			const u_short count() const noexcept;
			/**
			 * get Метод извлечения содержимого переменной окружения
			 * @param  name название переменной окружения
			 * @return      значение переменной окружения
			 */
			const char * get(const string & name) const noexcept;
			/**
			 * is Метод проверки наличия и значения переменной
			 * @param key ключ - название переменной для проверки
			 * @param val значение переменной для проверки (если требуется)
			 */
			const bool is(const string & key, const string & val = "") const noexcept;
		public:
			/**
			 * autoRead Метод разрешения или запрещения автоматического чтения текстовой переменной
			 * @param flag значение флага
			 */
			void autoRead(const bool flag) noexcept;
			/**
			 * setJson Метод добавления данных в формате json
			 * @param data данные в формате json
			 */
			void setJson(const json & data) noexcept;
			/**
			 * textEnv Метод установки списка имён которые нужно извлечь
			 * @param text название переменной для извлечения текстовой информации из потока (если параметром не передана)
			 */
			void textEnv(const string & text) noexcept;
			/**
			 * prefixEnv Метод установки префикса переменной окружения
			 * @param prefix префикс переменной окружения
			 */
			void prefixEnv(const string & prefix) noexcept;
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet) noexcept;
			/**
			 * read Метод чтения данных переменных окружения
			 * @param args  список переменных окружения
			 * @param count количество переменных окружения
			 */
			void read(const char * args[], const u_short count) noexcept;
		public:
			/**
			 * Env Конструктор
			 * @param prefix префикс переменной окружения
			 */
			Env(const string & prefix) noexcept;
			/**
			 * Env Конструктор
			 * @param alphabet объект алфавита
			 */
			Env(const alphabet_t * alphabet = nullptr) noexcept;
			/**
			 * Env Конструктор
			 * @param prefix префикс переменной окружения
			 * @param text   название переменной для извлечения текстовой информации из потока (если параметром не передана)
			 */
			Env(const string & prefix, const string & text) noexcept;
			/**
			 * Env Конструктор
			 * @param prefix   префикс переменной окружения
			 * @param alphabet объект алфавита
			 */
			Env(const string & prefix, const alphabet_t * alphabet) noexcept;
			/**
			 * Env Конструктор
			 * @param prefix   префикс переменной окружения
			 * @param text     название переменной для извлечения текстовой информации из потока (если параметром не передана)
			 * @param alphabet объект алфавита
			 */
			Env(const string & prefix, const string & text, const alphabet_t * alphabet) noexcept;
	} env_t;
};

#endif // __ANYKS_ENV__

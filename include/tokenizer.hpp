/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_TOKENIZER_
#define _ANYKS_TOKENIZER_

/**
 * Стандартная библиотека
 */
#include <string>
#include <vector>
#include <fstream>
#include <functional>
#include <string.h>
#include <sys/types.h>
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
	 * Tokenizer Класс работы с токенизацией
	 */
	typedef struct Tokenizer {
		private:
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
		public:
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet);
		public:
			/**
			 * readline Метод извлечения строки из текста
			 * @param  is  файловый поток для чтения данных
			 * @param  str строка для извлечения текста
			 * @return     файловый поток с текущий позиции
			 */
			istream & readline(istream & is, string & str) const;
			/**
			 * restore Метод восстановления текста из контекста
			 * @param context токенизированный контекст
			 * @return        результирующий текст
			 */
			const string restore(const vector <string> & context) const;
			/**
			 * restore Метод восстановления текста из контекста
			 * @param context токенизированный контекст
			 * @return        результирующий текст
			 */
			const wstring restore(const vector <wstring> & context) const;
			/**
			 * jsonToText Метод преобразования текста в формате json в текст
			 * @param text     текст для преобразования в формате json
			 * @param callback функция обратного вызова, на каждой итерации
			 */
			void jsonToText(const string & text, function <void (const string &)> callback) const;
			/**
			 * textToJson Метод преобразования текста в json
			 * @param text     текст для преобразования
			 * @param callback функция обратного вызова, на каждой итерации
			 */
			void textToJson(const string & text, function <void (const string &)> callback) const;
			/**
			 * run Метод разбивки текста на токены
			 * @param text     входной текст для обработки
			 * @param callback функция обратного вызова, на каждой итерации
			 */
			void run(const string & text, function <const bool (const wstring &, const vector <string> &, const bool, const bool)> callback) const;
		public:
			/**
			 * Tokenizer Конструктор
			 * @param alphabet объект алфавита
			 */
			Tokenizer(const alphabet_t * alphabet = nullptr);
	} tokenizer_t;
};

#endif // _ANYKS_TOKENIZER_

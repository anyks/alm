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
#include <cmath>
#include <limits>
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
	// Токены n-грамм
	enum class token_t : u_int {
		num = 2,     // токен арабского или римского числа <num>
		unk = 3,     // токен неизвестного слова <unk>
		url = 4,     // токен url адреса <url>
		abbr = 5,    // токен аббревиатуры <abbr>
		date = 6,    // токен даты <date>
		time = 7,    // токен времени <time>
		anum = 8,    // токен псевдо-числа <anum>
		math = 9,    // токен математической операции <math>
		specs = 10,  // токен спец-символа <specs>
		start = 1,   // токен начала предложения <s>
		aprox = 11,  // токен приблизительного числа <aprox>
		range = 12,  // токен диапазона чисел <range>
		score = 13,  // токен числового счёта <score>
		dimen = 14,  // токен габаритных размеров <dimen>
		fract = 15,  // токен числовой дроби <fract>
		punct = 16,  // токен знака пунктуации <punct>
		isolat = 17, // токен знака изоляции <isolat>
		finish = 18  // токен конеца предложения </s>
	};
	// Тип данных пары целых значений
	typedef pair <size_t, size_t> pair_t;
	// Получаем максимальное значение идентификатора
	constexpr size_t noID = numeric_limits <size_t>::max();
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
			 * idw Метод извлечения идентификатора токена
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const u_short idw(const wstring & word) const;
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
		public:
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

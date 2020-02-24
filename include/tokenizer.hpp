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
#include <idw.hpp>
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
	enum class token_t : u_short {
		num = 2,     // токен арабского или римского числа <num>
		unk = 3,     // токен неизвестного слова <unk>
		url = 4,     // токен url адреса <url>
		null = 0,    // токен неопределённого значения
		abbr = 5,    // токен аббревиатуры <abbr>
		date = 6,    // токен даты <date>
		time = 7,    // токен времени <time>
		anum = 8,    // токен псевдо-числа <anum>
		math = 9,    // токен математической операции <math>
		start = 1,   // токен начала предложения <s>
		rnum = 10,   // токен римского числа (не используется в чистом виде)
		specl = 11,  // токен спец-символа <specl>
		aprox = 12,  // токен приблизительного числа <aprox>
		range = 13,  // токен диапазона чисел <range>
		score = 14,  // токен числового счёта <score>
		dimen = 15,  // токен габаритных размеров <dimen>
		fract = 16,  // токен числовой дроби <fract>
		punct = 17,  // токен знака пунктуации <punct>
		isolat = 18, // токен знака изоляции <isolat>
		finish = 19  // токен конеца предложения </s>
	};
	// Тип данных пары целых значений
	typedef pair <size_t, size_t> pair_t;
	/**
	 * Tokenizer Класс работы с токенизацией
	 */
	typedef struct Tokenizer {
		private:
			// Объект идентификатора
			idw_t idWord;
			// Список аббревиатур
			set <size_t> abbrs;
		private:
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
		public:
			/**
			 * setAbbr Метод добавления аббревиатуры
			 * @param word слово для добавления
			 */
			void setAbbr(const string & word);
			/**
			 * setAbbr Метод добавления аббревиатуры
			 * @param word слово для добавления
			 */
			void setAbbr(const wstring & word);
			/**
			 * setAbbrs Метод установки списка аббревиатур
			 * @param abbrs список аббревиатур
			 */
			void setAbbrs(const set <size_t> & abbrs);
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet);
		public:
			/**
			 * getAbbrs Метод извлечения списка аббревиатур
			 * @return список аббревиатур
			 */
			const set <size_t> & getAbbrs() const;
		public:
			/**
			 * idw Метод извлечения идентификатора слова
			 * @param  word слово для получения идентификатора
			 * @return      идентификатор слова
			 */
			const size_t idw(const wstring & word) const;
			/**
			 * idt Метод извлечения идентификатора токена
			 * @param  word слово для получения идентификатора
			 * @return      идентификатор токена
			 */
			const token_t idt(const wstring & word) const;
			/**
			 * isAbbr Метод проверки слова на соответствие аббревиатуры
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isAbbr(const wstring & word) const;
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
			 * clear Метод очистки собранных данных
			 */
			void clear();
			/**
			 * update Метод обновления параметров
			 */
			void update();
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
			/**
			 * ~Tokenizer Деструктор
			 */
			~Tokenizer();
	} tokenizer_t;
};

#endif // _ANYKS_TOKENIZER_

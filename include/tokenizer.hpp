/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_TOKENIZER__
#define __ANYKS_TOKENIZER__

/**
 * Стандартная библиотека
 */
#include <stack>
#include <cmath>
#include <bitset>
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
#include <fsys.hpp>
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
		num = 2,       // токен арабского или римского числа <num>
		unk = 3,       // токен неизвестного слова <unk>
		url = 4,       // токен url адреса <url>
		null = 0,      // токен неопределённого значения
		abbr = 5,      // токен аббревиатуры <abbr>
		date = 6,      // токен даты <date>
		time = 7,      // токен времени <time>
		anum = 8,      // токен псевдо-числа <anum>
		math = 9,      // токен математической операции <math>
		start = 1,     // токен начала предложения <s>
		rnum = 10,     // токен римского числа (не используется в чистом виде)
		greek = 11,    // токен символа греческого алфавита <greek>
		route = 12,    // токен символа направления (стрелок) <route>
		specl = 13,    // токен спец-символа <specl>
		aprox = 14,    // токен приблизительного числа <aprox>
		range = 15,    // токен диапазона чисел <range>
		score = 16,    // токен числового счёта <score>
		dimen = 17,    // токен габаритных размеров <dimen>
		fract = 18,    // токен числовой дроби <fract>
		punct = 19,    // токен знака пунктуации <punct>
		pcards = 20,   // токен символа игральных карт <pcards>
		isolat = 21,   // токен знака изоляции <isolat>
		finish = 22,   // токен конеца предложения </s>
		currency = 23, // токен символа мировой валюты <currency>
		endtoken       // Финальное значение токена
	};
	// Тип данных пары целых значений
	typedef pair <size_t, size_t> pair_t;
	/**
	 * Tokenizer Класс работы с токенизацией
	 */
	typedef struct Tokenizer {
		private:
			// Типы флагов
			enum class type_t : u_short {
				end,     // Конец контекста
				num,     // Символ числа в тексте
				null,    // Не определено
				word,    // нормальное слово
				math,    // Математическая операция
				open,    // Открытие изоляционного символа
				greek,   // Символ греческого алфавита
				route,   // Символ направления (стрелок)
				close,   // Закрытие изоляционного символа
				specl,   // Спец-символ в тексте
				space,   // Символ пробела в тексте
				allow,   // Разрешённый символ
				punct,   // Знак препинания
				pcards,  // Символ игральных карт
				isolat,  // Изоляционный символ в строке
				currency // Символ мировой валюты
			};
		public:
			// Основные опции
			enum class options_t : u_short {
				debug,  // Флаг режима отладки
				stress, // Флаг разрешения использовать символы ударения
				uppers, // Флаг разрешения проставлять регистры букв в словах
				collect // Флаг разрешения сборку суффиксов цифровых аббревиатур
			};
			// Тип функции внешнего токенизатора
			typedef function <void (const wstring &, function <const bool (const wstring &, const vector <string> &, const bool, const bool)>)> tokenz_t;
		private:
			// Объект идентификатора
			idw_t wrdId;
			// Флаги параметров
			bitset <4> options;
			// Списки суффиксов цифровых и буквенных аббревиатур
			mutable set <size_t> abbrs, suffix;
		private:
			// Внешняя функция токенизатора
			tokenz_t extFn;
			// Объект алфавита
			const alphabet_t * alphabet;
			// Объект log файла
			mutable const char * logfile;
		private:
			/**
			 * isOption Метод проверки наличия опции
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isOption(const options_t option) const noexcept;
			/**
			 * restore Метод восстановления текста из контекста
			 * @param first   текущее слово для проверки
			 * @param second  следующее слово для проверки
			 * @param text    текст для сборки контекста
			 * @param context контекст типов токенов
			 */
			void restore(const wstring & first, const wstring & second, wstring & text, stack <type_t> & context) const noexcept;
		public:
			/**
			 * addAbbr Метод добавления аббревиатуры
			 * @param idw идентификатор слова для добавления
			 */
			void addAbbr(const size_t idw) noexcept;
			/**
			 * addAbbr Метод добавления аббревиатуры
			 * @param word слово для добавления
			 */
			void addAbbr(const string & word) noexcept;
			/**
			 * addAbbr Метод добавления аббревиатуры
			 * @param word слово для добавления
			 */
			void addAbbr(const wstring & word) noexcept;
			/**
			 * setAbbrs Метод установки списка аббревиатур
			 * @param abbrs список аббревиатур
			 */
			void setAbbrs(const set <size_t> & abbrs) noexcept;
		public:
			/**
			 * setLogfile Метод установки файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile) noexcept;
			/**
			 * setOption Метод установки опций модуля
			 * @param option опция для установки
			 */
			void setOption(const options_t option) noexcept;
			/**
			 * unsetOption Метод отключения опции модуля
			 * @param option опция для отключения
			 */
			void unsetOption(const options_t option) noexcept;
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet) noexcept;
		public:
			/**
			 * addSuffix Метод установки суффикса цифровой аббревиатуры
			 * @param idw идентификатор суффикса цифровой аббревиатуры
			 */
			void addSuffix(const size_t idw) const noexcept;
			/**
			 * setSuffixes Метод установки списка суффиксов цифровых аббревиатур
			 * @param suffix список суффиксов цифровых аббревиатур
			 */
			void setSuffixes(const set <size_t> & suffix) const noexcept;
			/**
			 * addSuffix Метод извлечения суффикса из цифровой аббревиатуры
			 * @param word слово для извлечения суффикса аббревиатуры
			 * @param idw  идентификатор обрабатываемого слова
			 */
			void addSuffix(const wstring & word, const size_t idw = idw_t::NIDW) const noexcept;
		public:
			/**
			 * getAbbrs Метод извлечения списка аббревиатур
			 * @return список аббревиатур
			 */
			const set <size_t> & getAbbrs() const noexcept;
			/**
			 * getSuffixes Метод извлечения списка суффиксов цифровых аббревиатур
			 * @return список цифровых аббревиатур
			 */
			const set <size_t> & getSuffixes() const noexcept;
		public:
			/**
			 * fti Метод удаления дробной части числа
			 * @param  num   число для обработки
			 * @param  count количество символов после запятой
			 * @return       число без дробной части
			 */
			const size_t fti(const double num, const u_short count = 0) const noexcept;
		public:
			/**
			 * abbrEmpty Метод проверки на существование буквенных аббревиатур
			 * @return результат проверки
			 */
			const bool abbrEmpty() const noexcept;
			/**
			 * suffixEmpty Метод проверки на существование суффиксов цифровых аббревиатур
			 * @return результат проверки
			 */
			const bool suffixEmpty() const noexcept;
		public:
			/**
			 * isToken Метод проверки идентификатора на токен
			 * @param idw идентификатор слова для проверки
			 * @return    результат проверки
			 */
			const bool isToken(const size_t idw) const noexcept;
			/**
			 * isIdWord Метод проверки на соответствие идентификатора слову
			 * @param idw идентификатор слова для проверки
			 * @return    результат проверки идентификатора
			 */
			const bool isIdWord(const size_t idw) const noexcept;
		public:
			/**
			 * isAbbr Метод проверки слова на соответствие аббревиатуры
			 * @param idw идентификатор слова для проверки
			 * @return    результат проверки
			 */
			const bool isAbbr(const size_t idw) const noexcept;
			/**
			 * isAbbr Метод проверки слова на соответствие аббревиатуры
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isAbbr(const wstring & word) const noexcept;
			/**
			 * isSuffix Метод проверки слова на суффикс цифровой аббревиатуры
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isSuffix(const wstring & word) const noexcept;
		public:
			/**
			 * idt Метод извлечения идентификатора токена
			 * @param  word слово для получения идентификатора
			 * @return      идентификатор токена
			 */
			const token_t idt(const wstring & word) const noexcept;
			/**
			 * idw Метод извлечения идентификатора слова
			 * @param  word слово для получения идентификатора
			 * @return      идентификатор слова
			 */
			const size_t idw(const wstring & word) const noexcept;
			/**
			 * ids Метод извлечения идентификатора последовательности
			 * @param  seq последовательность для получения идентификатора
			 * @return     идентификатор последовательности
			 */
			const size_t ids(const vector <size_t> & seq) const noexcept;
		public:
			/**
			 * readline Метод извлечения строки из текста
			 * @param  is  файловый поток для чтения данных
			 * @param  str строка для извлечения текста
			 * @return     файловый поток с текущий позиции
			 */
			istream & readline(istream & is, string & str) const noexcept;
			/**
			 * restore Метод восстановления текста из контекста
			 * @param context токенизированный контекст
			 * @return        результирующий текст
			 */
			const string restore(const vector <string> & context) const noexcept;
			/**
			 * restore Метод восстановления текста из контекста
			 * @param context токенизированный контекст
			 * @return        результирующий текст
			 */
			const wstring restore(const vector <wstring> & context) const noexcept;
		public:
			/**
			 * clear Метод очистки собранных данных
			 */
			void clear() noexcept;
			/**
			 * update Метод обновления параметров
			 */
			void update() noexcept;
			/**
			 * setExternal Метод установки внешней функции токенизатора
			 * @param fn внешняя функция токенизатора
			 */
			void setExternal(tokenz_t fn) noexcept;
		public:
			/**
			 * jsonToText Метод преобразования текста в формате json в текст
			 * @param text     текст для преобразования в формате json
			 * @param callback функция обратного вызова, на каждой итерации
			 */
			void jsonToText(const string & text, function <void (const string &)> callback) const noexcept;
			/**
			 * textToJson Метод преобразования текста в json
			 * @param text     текст для преобразования
			 * @param callback функция обратного вызова, на каждой итерации
			 */
			void textToJson(const string & text, function <void (const string &)> callback) const noexcept;
		public:
			/**
			 * writeSuffix Метод записи данных в файл суффиксов цифровых аббревиатур
			 * @param filename адрес файла для записи
			 * @param status   функция вывода статуса
			 */
			void writeSuffix(const string & filename, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * readSuffix Метод чтения данных из файла суффиксов цифровых аббревиатур
			 * @param filename адрес файла для чтения
			 * @param status   функция вывода статуса
			 */
			void readSuffix(const string & filename, function <void (const string &, const u_short)> status = nullptr) noexcept;
		public:
			/**
			 * run Метод разбивки текста на токены
			 * @param text     входной текст для обработки
			 * @param callback функция обратного вызова, на каждой итерации
			 */
			void run(const string & text, function <const bool (const wstring &, const vector <string> &, const bool, const bool)> callback) const noexcept;
			/**
			 * run Метод разбивки текста на токены
			 * @param text     входной текст для обработки
			 * @param callback функция обратного вызова, на каждой итерации
			 */
			void run(const wstring & text, function <const bool (const wstring &, const vector <string> &, const bool, const bool)> callback) const noexcept;
		public:
			/**
			 * Tokenizer Конструктор
			 * @param alphabet объект алфавита
			 */
			Tokenizer(const alphabet_t * alphabet = nullptr) noexcept;
			/**
			 * ~Tokenizer Деструктор
			 */
			~Tokenizer() noexcept;
	} tokenizer_t;
};

#endif // __ANYKS_TOKENIZER__

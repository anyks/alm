/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_ALPHABET_
#define _ANYKS_ALPHABET_

/**
 * Стандартная библиотека
 */
#include <set>
#include <map>
#include <list>
#include <stack>
#include <bitset>
#include <locale>
#include <string>
#include <vector>
#include <chrono>
#include <cstdarg>
#include <fstream>
#include <string.h>
#include <sys/types.h>
/**
 * Наши модули
 */
#include <nwt.hpp>
#include <word.hpp>
#include <general.hpp>

using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Alphabet Класс для работы с алфавитом
	 */
	typedef class Alphabet {
		private:
			// Латинский тип алфавита
			bool typeLatian = true;
			// Локализация приложения
			string encoding = "en_US.UTF-8";
			// Алфавит словаря
			wstring alphabet = L"abcdefghijklmnopqrstuvwxyz";
		public:
			// log_t Флаги логирования
			enum class log_t : u_short {info, warning, error};
		private:
			// Объект парсинга uri адреса
			mutable uri_t uri;
			// Латинский алфавит
			std::set <wchar_t> latian;
			// Алфавит для проверки
			std::set <wchar_t> letters;
			// Список альтернативных букв
			std::map <wchar_t, wchar_t> alters;
			// Список похожих букв в разных алфавитах
			std::map <wchar_t, wchar_t> substitutes;
		public:
			/**
			 * cbegin Метод итератор начала списка
			 * @return итератор
			 */
			const std::set <wchar_t>::const_iterator cbegin() const;
			/**
			 * cend Метод итератор конца списка
			 * @return итератор
			 */
			const std::set <wchar_t>::const_iterator cend() const;
			/**
			 * crbegin Метод обратный итератор начала списка
			 * @return итератор
			 */
			const std::set <wchar_t>::const_reverse_iterator crbegin() const;
			/**
			 * crend Метод обратный итератор конца списка
			 * @return итератор
			 */
			const std::set <wchar_t>::const_reverse_iterator crend() const;
		public:
			/**
			 * alts Метод получения списка альтернативных букв
			 * return список альтернативных букв
			 */
			const std::map <wchar_t, wchar_t> & alts() const;
		public:
			/**
			 * get Метод получения алфавита языка
			 * @return алфавит языка
			 */
			const string get() const;
			/**
			 * trim Метод удаления пробелов вначале и конце текста
			 * @param  text текст для удаления пробелов
			 * @return      текст без пробелов
			 */
			const string trim(const string & text) const;
			/**
			 * toLower Метод перевода русских букв в нижний регистр
			 * @param str строка для перевода
			 * @return    строка в нижнем регистре
			 */
			const string toLower(const string & str) const;
			/**
			 * toUpper Метод перевода русских букв в верхний регистр
			 * @param str строка для перевода
			 * @return    строка в верхнем регистре
			 */
			const string toUpper(const string & str) const;
			/**
			 * convert Метод конвертирования строки utf-8 в строку
			 * @param  str строка utf-8 для конвертирования
			 * @return     обычная строка
			 */
			const string convert(const wstring & str) const;
			/**
			 * format Метод реализации функции формирования форматированной строки
			 * @param format формат строки вывода
			 * @param args   передаваемые аргументы
			 * @return       сформированная строка
			 */
			const string format(const char * format, ...) const;
		public:
			/**
			 * toLower Метод перевода русских букв в нижний регистр
			 * @param str строка для перевода
			 * @return    строка в нижнем регистре
			 */
			const char toLower(const char letter) const;
			/**
			 * toUpper Метод перевода русских букв в верхний регистр
			 * @param str строка для перевода
			 * @return    строка в верхнем регистре
			 */
			const char toUpper(const char letter) const;
			/**
			 * alt Метод получения альтернативной буквы
			 * @param  lid буква для проверки
			 * @return     альтернативная буква
			 */
			const wchar_t alt(const wchar_t lid) const;
			/**
			 * rel Метод получения реальной буквы из альтернативной
			 * @param  lid альтернативная буква
			 * @return     реальная буква
			 */
			const wchar_t rel(const wchar_t lid) const;
			/**
			 * toLower Метод перевода русских букв в нижний регистр
			 * @param str строка для перевода
			 * @return    строка в нижнем регистре
			 */
			const wchar_t toLower(const wchar_t letter) const;
			/**
			 * toUpper Метод перевода русских букв в верхний регистр
			 * @param str строка для перевода
			 * @return    строка в верхнем регистре
			 */
			const wchar_t toUpper(const wchar_t letter) const;
		public:
			/**
			 * trim Метод удаления пробелов вначале и конце текста
			 * @param  text текст для удаления пробелов
			 * @return      текст без пробелов
			 */
			const wstring trim(const wstring & text) const;
			/**
			 * convert Метод конвертирования строки в строку utf-8
			 * @param  str строка для конвертирования
			 * @return     строка в utf-8
			 */
			const wstring convert(const string & str) const;
			/**
			 * toLower Метод перевода русских букв в нижний регистр
			 * @param str строка для перевода
			 * @return    строка в нижнем регистре
			 */
			const wstring toLower(const wstring & str) const;
			/**
			 * toUpper Метод перевода русских букв в верхний регистр
			 * @param str строка для перевода
			 * @return    строка в верхнем регистре
			 */
			const wstring toUpper(const wstring & str) const;
			/**
			 * arabic2Roman Метод перевода арабских чисел в римские
			 * @param  number арабское число от 1 до 4999
			 * @return        римское число
			 */
			const wstring arabic2Roman(const u_int number) const;
			/**
			 * arabic2Roman Метод перевода арабских чисел в римские
			 * @param  word арабское число от 1 до 4999
			 * @return      римское число
			 */
			const wstring arabic2Roman(const wstring & word) const;
			/**
			 * delAltInWord Метод удаления альтернативных букв в слове
			 * @param word слово в котором нужно удалить альтернативную букву
			 * @return     слово без альтернативной буквы
			 */
			const wstring delAltInWord(const wstring & word) const;
			/**
			 * delPunctInWord Метод очистки текста от всех знаков препинаний
			 * @param word слово для очистки
			 * @return     текст без запрещенных символов
			 */
			const wstring delPunctInWord(const wstring & word) const;
			/**
			 * delBrokenInWord Метод очистки текста от всех символов кроме разрешенных
			 * @param word слово для очистки
			 * @return     текст без запрещенных символов
			 */
			const wstring delBrokenInWord(const wstring & word) const;
			/**
			 * delHyphenInWord Метод удаления дефиса из слова
			 * @param word слово в котором нужно удалить дефис
			 * @return     слово без дефиса
			 */
			const wstring delHyphenInWord(const wstring & word) const;
			/**
			 * replace Метод замены в тексте слово на другое слово
			 * @param  text текст в котором нужно произвести замену
			 * @param  word слово для поиска
			 * @param  alt  слово на которое нужно произвести замену
			 * @return      результирующий текст
			 */
			const wstring replace(const wstring & text, const wstring & word, const wstring & alt = L"") const;
		public:
			/**
			 * errors Метод подсчета максимально-возможного количества ошибок в слове
			 * @param  word слово для расчёта
			 * @return      результат расчёта
			 */
			const u_short errors(const wstring & word) const;
			/**
			 * roman2Arabic Метод перевода римских цифр в арабские
			 * @param  word римское число
			 * @return      арабское число
			 */
			const u_short roman2Arabic(const wstring & word) const;
		public:
			/**
			 * count Метод получения количества букв в словаре
			 * @return количество букв в словаре
			 */
			const size_t count() const;
			/**
			 * setCase Метод запоминания регистра слова
			 * @param pos позиция для установки регистра
			 * @param cur текущее значение регистра в бинарном виде
			 * @return    позиция верхнего регистра в бинарном виде
			 */
			const size_t setCase(const size_t pos, const size_t cur = 0) const;
			/**
			 * countLetter Метод подсчета количества указанной буквы в слове
			 * @param word   слово в котором нужно подсчитать букву
			 * @param letter букву которую нужно подсчитать
			 * @return       результат подсчёта
			 */
			const size_t countLetter(const wstring & word, const wchar_t letter) const;
		public:
			/**
			 * altemp Метод проверки на сущестования альтернативных букв
			 * @return результат проверки
			 */
			const bool altemp() const;
			/**
			 * isUrl Метод проверки соответствия слова url адресу
			 * @param word слово для проверки
			 * @return     результат проверки
			 */
			const bool isUrl(const wstring & word) const;
			/**
			 * isAlt Метод проверки существования альтернативной буквы
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool isAlt(const wchar_t letter) const;
			/**
			 * isMath Метод определения математических операий
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			const bool isMath(const wchar_t letter) const;
			/**
			 * isAbbr Метод проверки слова на соответствие аббревиатуры
			 * @param word слово для проверки
			 * @return     результат проверки
			 */
			const bool isAbbr(const wstring & word) const;
			/**
			 * isUpper Метод проверки символ на верхний регистр
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			const bool isUpper(const wchar_t letter) const;
			/**
			 * isLatian Метод проверки является ли строка латиницей
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const bool isLatian(const wstring & str) const;
			/**
			 * isPunct Метод проверки является ли буква, знаком препинания
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool isPunct(const wchar_t letter) const;
			/**
			 * isSpace Метод проверки является ли буква, пробелом
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool isSpace(const wchar_t letter) const;
			/**
			 * isNumber Метод проверки является ли слово числом
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isNumber(const wstring & word) const;
			/**
			 * isDecimal Метод проверки является ли слово дробным числом
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isDecimal(const wstring & word) const;
			/**
			 * isANumber Метод проверки является ли косвенно слово числом
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isANumber(const wstring & word) const;
			/**
			 * isAllowed Метод проверки соответствия слова словарю
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isAllowed(const wstring & word) const;
			/**
			 * isSpecial Метод определения спец-символа
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			const bool isSpecial(const wchar_t letter) const;
			/**
			 * isIsolation Метод определения знака изоляции (кавычки, скобки)
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool isIsolation(const wchar_t letter) const;
		public:
			/**
			 * rest Метод исправления и детектирования слов со смешенными алфавитами
			 * @param  word слово для проверки и исправления
			 * @return      результат проверки
			 */
			const bool rest(wstring & word) const;
			/**
			 * check Метод проверки соответствии буквы
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool check(const wchar_t letter) const;
			/**
			 * checkHome2 Метод проверки слова на Дом-2
			 * @param  word слово для проверки
			 * @return      результат работы метода
			 */
			const bool checkHome2(const wstring & word) const;
			/**
			 * checkLatian Метод проверки наличия латинских символов в строке
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const bool checkLatian(const wstring & str) const;
			/**
			 * checkHyphen Метод проверки наличия дефиса в строке
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const bool checkHyphen(const wstring & str) const;
			/**
			 * checkSimilars Метод проверки на симиляции букв с другими языками
			 * @param  str строка для проверки
			 * @return     результат проверки
			 */
			const bool checkSimilars(const wstring & str) const;
		public:
			/**
			 * getzones Метод извлечения списка пользовательских зон интернета
			 */
			const std::set <wstring> & getzones() const;
			/**
			 * getSubstitutes Метод извлечения букв для исправления слов из смешанных алфавитов
			 * @param return список букв разных алфавитов соответствующих друг-другу
			 */
			const std::map <string, string> & getSubstitutes() const;
			/**
			 * urls Метод извлечения координат url адресов в строке
			 * @param text текст для извлечения url адресов
			 * @return     список координат с url адресами
			 */
			const std::map <size_t, size_t> urls(const wstring & text) const;
			/**
			 * checkHypLat Метод поиска дефиса и латинского символа
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const pair <bool, bool> checkHypLat(const wstring & str) const;
		public:
			/**
			 * clear Метод очистки собранных данных
			 */
			void clear();
			/**
			 * log Метод вывода текстовой информации в консоль или файл
			 * @param format   формат строки вывода
			 * @param flag     флаг типа логирования
			 * @param filename адрес файла для вывода
			 */
			void log(const string & format, log_t flag, const char * filename, ...) const;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const word_t & str, const word_t & delim, list <word_t> & v) const;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const word_t & str, const word_t & delim, vector <word_t> & v) const;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const wstring & str, const wstring & delim, list <wstring> & v) const;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const wstring & str, const wstring & delim, vector <wstring> & v) const;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const string & str, const string & delim, list <wstring> & v) const;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const string & str, const string & delim, vector <wstring> & v) const;
			/**
			 * add Метод добавления буквы в алфавит
			 * @param lid идентификатор буквы для добавления
			 */
			void add(const wchar_t lid);
			/**
			 * set Метод добавления алфавита в словарь
			 * @param alphabet алфавит символов для текущего языка
			 */
			void set(const string & alphabet = "");
			/**
			 * rmalt Метод удаления альтернативной буквы
			 * @param letter буква у которой есть альтернативная буква
			 */
			void rmalt(const wchar_t letter);
			/**
			 * setzone Метод установки пользовательской зоны
			 * @param zone пользовательская зона
			 */
			void setzone(const string & zone);
			/**
			 * setzone Метод установки пользовательской зоны
			 * @param zone пользовательская зона
			 */
			void setzone(const wstring & zone);
			/**
			 * setlocale Метод установки локали
			 * @param locale локализация приложения
			 */
			void setlocale(const string & locale);
			/**
			 * setzones Метод установки списка пользовательских зон
			 * @param zones список доменных зон интернета
			 */
			void setzones(const std::set <string> & zones);
			/**
			 * setzones Метод установки списка пользовательских зон
			 * @param zones список доменных зон интернета
			 */
			void setzones(const std::set <wstring> & zones);
			/**
			 * setalt Метод добавления альтернативной буквы
			 * @param lid буква у которой есть альтернатива
			 * @param alt альтернативная буква
			 */
			void setalt(const wchar_t lid, const wchar_t alt);
			/**
			 * setSubstitutes Метод установки букв для исправления слов из смешанных алфавитов
			 * @param letters список букв разных алфавитов соответствующих друг-другу
			 */
			void setSubstitutes(const std::map <string, string> & letters);
		public:
			/**
			 * Alphabet Конструктор
			 */
			Alphabet(){};
			/**
			 * Alphabet Конструктор
			 * @param alphabet алфавит символов для текущего языка
			 * @param locale   локализация приложения
			 */
			Alphabet(const string & alphabet, const string & locale = "");
	} alphabet_t;
};

#endif // _ANYKS_ALPHABET_

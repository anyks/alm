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
#include <cstring>
#include <algorithm>
#include <sys/types.h>
/**
 * Наши модули
 */
#include <nwt.hpp>
#include <word.hpp>
#include <app/alm.hpp>

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
			/**
			 * Nums структура параметров чисел
			 */
			typedef struct Nums {
				// Названия римских цифр
				const set <wchar_t> roman = {L'm', L'd', L'c', L'l', L'x', L'i', L'v'};
				// Список арабских цифр
				const set <wchar_t> arabs = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9'};
				// Шаблоны римских форматов
				const vector <wstring> i = {L"", L"I", L"II", L"III", L"IV", L"V", L"VI", L"VII", L"VIII", L"IX"};
				const vector <wstring> x = {L"", L"X", L"XX", L"XXX", L"XL", L"L", L"LX", L"LXX", L"LXXX", L"XC"};
				const vector <wstring> c = {L"", L"C", L"CC", L"CCC", L"CD", L"D", L"DC", L"DCC", L"DCCC", L"CM"};
				const vector <wstring> m = {L"", L"M", L"MM", L"MMM", L"MMMM"};
			} nums_t;
		private:
			// Латинский тип алфавита
			bool typeLatian = true;
			// Разрешить апостроф как часть слова
			bool apostrophe = false;
			// Алфавит словаря
			wstring alphabet = L"abcdefghijklmnopqrstuvwxyz";
		private:
			// Числовые параметры
			const nums_t numsSymbols;
			// Список разрешённых спец-символом
			const std::set <wchar_t> allowedSymbols = {
				L'~', L'-', L'+', L'=',
				L'*', L'/', L':', L'%',
				L'|', L'^', L'&', L'#', L'\\'
			};
			// Список спец-символом
			const std::set <wchar_t> specialSymbols = {
				L'~', L'_', L'@', L'#', L'№', L'©',
				L'%', L'&', L'$', L'|', L'§', L'\\', L'±'
			};
			// Список символов изоляции
			const std::set <wchar_t> isolationSymbols = {
				L'(', L')', L'[', L']', L'{',
				L'}', L'"', L'«', L'»', L'„',
				L'“', L'`', L'\'',
			};
			// Список символов симиляции
			const std::set <wchar_t> similarsSymbols = {
				L'a', L'b', L'c', L'e', L'h', L'k',
				L'm', L'o', L'p', L't', L'u', L'x'
			};
			// Список математических символов
			const std::set <wchar_t> mathSymbols = {L'+', L'-', L'=', L'/', L'*', L'^'};
			// Список знаков пунктуации
			const std::set <wchar_t> punctsSymbols = {L'.', L',', L'?', L'!', L':', L';'};
		public:
			// log_t Флаги логирования
			enum class log_t : u_short {info, warning, error, null};
		private:
			// Объект парсинга uri адреса
			mutable uri_t uri;
			// Латинский алфавит
			std::set <wchar_t> latian;
			// Алфавит для проверки
			std::set <wchar_t> letters;
			// Список похожих букв в разных алфавитах
			std::map <wchar_t, wchar_t> substitutes;
		public:
			/**
			 * cbegin Метод итератор начала списка
			 * @return итератор
			 */
			const std::set <wchar_t>::const_iterator cbegin() const noexcept;
			/**
			 * cend Метод итератор конца списка
			 * @return итератор
			 */
			const std::set <wchar_t>::const_iterator cend() const noexcept;
			/**
			 * crbegin Метод обратный итератор начала списка
			 * @return итератор
			 */
			const std::set <wchar_t>::const_reverse_iterator crbegin() const noexcept;
			/**
			 * crend Метод обратный итератор конца списка
			 * @return итератор
			 */
			const std::set <wchar_t>::const_reverse_iterator crend() const noexcept;
		public:
			/**
			 * get Метод получения алфавита языка
			 * @return алфавит языка
			 */
			const string get() const noexcept;
			/**
			 * get Метод получения алфавита языка
			 * @return алфавит языка
			 */
			const wstring & wget() const noexcept;
			/**
			 * trim Метод удаления пробелов вначале и конце текста
			 * @param  text текст для удаления пробелов
			 * @return      результат работы функции
			 */
			const string trim(const string & text) const noexcept;
			/**
			 * toLower Метод перевода русских букв в нижний регистр
			 * @param str строка для перевода
			 * @return    строка в нижнем регистре
			 */
			const string toLower(const string & str) const noexcept;
			/**
			 * toUpper Метод перевода русских букв в верхний регистр
			 * @param str строка для перевода
			 * @return    строка в верхнем регистре
			 */
			const string toUpper(const string & str) const noexcept;
			/**
			 * convert Метод конвертирования строки utf-8 в строку
			 * @param  str строка utf-8 для конвертирования
			 * @return     обычная строка
			 */
			const string convert(const wstring & str) const noexcept;
			/**
			 * format Метод реализации функции формирования форматированной строки
			 * @param format формат строки вывода
			 * @param args   передаваемые аргументы
			 * @return       сформированная строка
			 */
			const string format(const char * format, ...) const noexcept;
		public:
			/**
			 * toLower Метод перевода русских букв в нижний регистр
			 * @param str строка для перевода
			 * @return    строка в нижнем регистре
			 */
			const char toLower(const char letter) const noexcept;
			/**
			 * toUpper Метод перевода русских букв в верхний регистр
			 * @param str строка для перевода
			 * @return    строка в верхнем регистре
			 */
			const char toUpper(const char letter) const noexcept;
			/**
			 * toLower Метод перевода русских букв в нижний регистр
			 * @param str строка для перевода
			 * @return    строка в нижнем регистре
			 */
			const wchar_t toLower(const wchar_t letter) const noexcept;
			/**
			 * toUpper Метод перевода русских букв в верхний регистр
			 * @param str строка для перевода
			 * @return    строка в верхнем регистре
			 */
			const wchar_t toUpper(const wchar_t letter) const noexcept;
		public:
			/**
			 * trim Метод удаления пробелов вначале и конце текста
			 * @param  text текст для удаления пробелов
			 * @return      результат работы функции
			 */
			const wstring trim(const wstring & text) const noexcept;
			/**
			 * convert Метод конвертирования строки в строку utf-8
			 * @param  str строка для конвертирования
			 * @return     строка в utf-8
			 */
			const wstring convert(const string & str) const noexcept;
			/**
			 * toLower Метод перевода русских букв в нижний регистр
			 * @param str строка для перевода
			 * @return    строка в нижнем регистре
			 */
			const wstring toLower(const wstring & str) const noexcept;
			/**
			 * toUpper Метод перевода русских букв в верхний регистр
			 * @param str строка для перевода
			 * @return    строка в верхнем регистре
			 */
			const wstring toUpper(const wstring & str) const noexcept;
			/**
			 * arabic2Roman Метод перевода арабских чисел в римские
			 * @param  number арабское число от 1 до 4999
			 * @return        римское число
			 */
			const wstring arabic2Roman(const u_int number) const noexcept;
			/**
			 * arabic2Roman Метод перевода арабских чисел в римские
			 * @param  word арабское число от 1 до 4999
			 * @return      римское число
			 */
			const wstring arabic2Roman(const wstring & word) const noexcept;
			/**
			 * delPunctInWord Метод очистки текста от всех знаков препинаний
			 * @param word слово для очистки
			 * @return     текст без запрещенных символов
			 */
			const wstring delPunctInWord(const wstring & word) const noexcept;
			/**
			 * delBrokenInWord Метод очистки текста от всех символов кроме разрешенных
			 * @param word слово для очистки
			 * @return     текст без запрещенных символов
			 */
			const wstring delBrokenInWord(const wstring & word) const noexcept;
			/**
			 * delHyphenInWord Метод удаления дефиса из слова
			 * @param word слово в котором нужно удалить дефис
			 * @return     слово без дефиса
			 */
			const wstring delHyphenInWord(const wstring & word) const noexcept;
			/**
			 * replace Метод замены в тексте слово на другое слово
			 * @param  text текст в котором нужно произвести замену
			 * @param  word слово для поиска
			 * @param  alt  слово на которое нужно произвести замену
			 * @return      результирующий текст
			 */
			const wstring replace(const wstring & text, const wstring & word, const wstring & alt = L"") const noexcept;
		public:
			/**
			 * errors Метод подсчета максимально-возможного количества ошибок в слове
			 * @param  word слово для расчёта
			 * @return      результат расчёта
			 */
			const u_short errors(const wstring & word) const noexcept;
			/**
			 * roman2Arabic Метод перевода римских цифр в арабские
			 * @param  word римское число
			 * @return      арабское число
			 */
			const u_short roman2Arabic(const wstring & word) const noexcept;
		public:
			/**
			 * count Метод получения количества букв в словаре
			 * @return количество букв в словаре
			 */
			const size_t count() const noexcept;
			/**
			 * setCase Метод запоминания регистра слова
			 * @param pos позиция для установки регистра
			 * @param cur текущее значение регистра в бинарном виде
			 * @return    позиция верхнего регистра в бинарном виде
			 */
			const size_t setCase(const size_t pos, const size_t cur = 0) const noexcept;
			/**
			 * countLetter Метод подсчета количества указанной буквы в слове
			 * @param word   слово в котором нужно подсчитать букву
			 * @param letter букву которую нужно подсчитать
			 * @return       результат подсчёта
			 */
			const size_t countLetter(const wstring & word, const wchar_t letter) const noexcept;
		public:
			/**
			 * isAllowApostrophe Метод проверки разрешения апострофа
			 * @return результат проверки
			 */
			const bool isAllowApostrophe() const noexcept;
			/**
			 * isUrl Метод проверки соответствия слова url адресу
			 * @param word слово для проверки
			 * @return     результат проверки
			 */
			const bool isUrl(const wstring & word) const noexcept;
			/**
			 * isMath Метод определения математических операий
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			const bool isMath(const wchar_t letter) const noexcept;
			/**
			 * isAbbr Метод проверки слова на соответствие аббревиатуры
			 * @param word слово для проверки
			 * @return     результат проверки
			 */
			const bool isAbbr(const wstring & word) const noexcept;
			/**
			 * isUpper Метод проверки символ на верхний регистр
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			const bool isUpper(const wchar_t letter) const noexcept;
			/**
			 * isLatian Метод проверки является ли строка латиницей
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const bool isLatian(const wstring & str) const noexcept;
			/**
			 * isPunct Метод проверки является ли буква, знаком препинания
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool isPunct(const wchar_t letter) const noexcept;
			/**
			 * isSpace Метод проверки является ли буква, пробелом
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool isSpace(const wchar_t letter) const noexcept;
			/**
			 * isNumber Метод проверки является ли слово числом
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isNumber(const wstring & word) const noexcept;
			/**
			 * isDecimal Метод проверки является ли слово дробным числом
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isDecimal(const wstring & word) const noexcept;
			/**
			 * isANumber Метод проверки является ли косвенно слово числом
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isANumber(const wstring & word) const noexcept;
			/**
			 * isAllowed Метод проверки соответствия слова словарю
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isAllowed(const wstring & word) const noexcept;
			/**
			 * isSpecial Метод определения спец-символа
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			const bool isSpecial(const wchar_t letter) const noexcept;
			/**
			 * isIsolation Метод определения знака изоляции (кавычки, скобки)
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool isIsolation(const wchar_t letter) const noexcept;
		public:
			/**
			 * rest Метод исправления и детектирования слов со смешенными алфавитами
			 * @param  word слово для проверки и исправления
			 * @return      результат проверки
			 */
			const bool rest(wstring & word) const noexcept;
			/**
			 * check Метод проверки соответствии буквы
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool check(const wchar_t letter) const noexcept;
			/**
			 * checkHome2 Метод проверки слова на Дом-2
			 * @param  word слово для проверки
			 * @return      результат работы метода
			 */
			const bool checkHome2(const wstring & word) const noexcept;
			/**
			 * checkLatian Метод проверки наличия латинских символов в строке
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const bool checkLatian(const wstring & str) const noexcept;
			/**
			 * checkHyphen Метод проверки наличия дефиса в строке
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const bool checkHyphen(const wstring & str) const noexcept;
			/**
			 * checkSimilars Метод проверки на симиляции букв с другими языками
			 * @param  str строка для проверки
			 * @return     результат проверки
			 */
			const bool checkSimilars(const wstring & str) const noexcept;
		public:
			/**
			 * getzones Метод извлечения списка пользовательских зон интернета
			 */
			const std::set <wstring> & getzones() const noexcept;
			/**
			 * getSubstitutes Метод извлечения букв для исправления слов из смешанных алфавитов
			 * @param return список букв разных алфавитов соответствующих друг-другу
			 */
			const std::map <string, string> & getSubstitutes() const noexcept;
			/**
			 * urls Метод извлечения координат url адресов в строке
			 * @param text текст для извлечения url адресов
			 * @return     список координат с url адресами
			 */
			const std::map <size_t, size_t> urls(const wstring & text) const noexcept;
			/**
			 * checkHypLat Метод поиска дефиса и латинского символа
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const pair <bool, bool> checkHypLat(const wstring & str) const noexcept;
		public:
			/**
			 * clear Метод очистки собранных данных
			 */
			void clear() noexcept;
			/**
			 * switchAllowApostrophe Метод разрешения или запрещения апострофа как части слова
			 */
			void switchAllowApostrophe() noexcept;
			/**
			 * log Метод вывода текстовой информации в консоль или файл
			 * @param format   формат строки вывода
			 * @param flag     флаг типа логирования
			 * @param filename адрес файла для вывода
			 */
			void log(const string & format, log_t flag, const char * filename, ...) const noexcept;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const word_t & str, const word_t & delim, list <word_t> & v) const noexcept;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const word_t & str, const word_t & delim, vector <word_t> & v) const noexcept;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const wstring & str, const wstring & delim, list <wstring> & v) const noexcept;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const wstring & str, const wstring & delim, vector <wstring> & v) const noexcept;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const string & str, const string & delim, list <wstring> & v) const noexcept;
			/**
			 * split Метод разделения строк на составляющие
			 * @param str   строка для поиска
			 * @param delim разделитель
			 * @param v     результирующий вектор
			 */
			void split(const string & str, const string & delim, vector <wstring> & v) const noexcept;
			/**
			 * add Метод добавления буквы в алфавит
			 * @param letter буква для добавления
			 */
			void add(const wchar_t letter) noexcept;
			/**
			 * set Метод добавления алфавита в словарь
			 * @param alphabet алфавит символов для текущего языка
			 */
			void set(const string & alphabet = "") noexcept;
			/**
			 * setzone Метод установки пользовательской зоны
			 * @param zone пользовательская зона
			 */
			void setzone(const string & zone) noexcept;
			/**
			 * setzone Метод установки пользовательской зоны
			 * @param zone пользовательская зона
			 */
			void setzone(const wstring & zone) noexcept;
			/**
			 * setzones Метод установки списка пользовательских зон
			 * @param zones список доменных зон интернета
			 */
			void setzones(const std::set <string> & zones) noexcept;
			/**
			 * setzones Метод установки списка пользовательских зон
			 * @param zones список доменных зон интернета
			 */
			void setzones(const std::set <wstring> & zones) noexcept;
			/**
			 * setlocale Метод установки локали
			 * @param locale локализация приложения
			 */
			void setlocale(const string & locale = "en_US.UTF-8") noexcept;
			/**
			 * setSubstitutes Метод установки букв для исправления слов из смешанных алфавитов
			 * @param letters список букв разных алфавитов соответствующих друг-другу
			 */
			void setSubstitutes(const std::map <string, string> & letters) noexcept;
		public:
			/**
			 * Alphabet Конструктор
			 */
			Alphabet() noexcept;
			/**
			 * Alphabet Конструктор
			 * @param locale локализация приложения
			 */
			Alphabet(const string & locale) noexcept;
			/**
			 * Alphabet Конструктор
			 * @param alphabet алфавит символов для текущего языка
			 * @param locale   локализация приложения
			 */
			Alphabet(const string & alphabet, const string & locale) noexcept;
	} alphabet_t;
	/**
	 * Оператор чтения из потока
	 * @param is   поток для чтения
	 * @param word слово куда нужно считать данные из потока
	 */
	istream & operator >> (istream & is, wstring & word) noexcept;
	/**
	 * Оператор вывода в поток из слова
	 * @param os   поток куда нужно вывести данные
	 * @param word слово из которого нужно вывести данные
	 */
	ostream & operator << (ostream & os, const wstring & word) noexcept;
};

#endif // _ANYKS_ALPHABET_

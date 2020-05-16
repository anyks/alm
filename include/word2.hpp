/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */
#ifndef _ANYKS_WORD2_
#define _ANYKS_WORD2_

/**
 * Стандартная библиотека
 */
#include <cmath>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <cstring>
#include <iostream>
#include <algorithm>
#include <sys/types.h>

using namespace std;

/**
 * anyks пространство имен
 */
namespace anyks {
	/**
	 * Word2 Класс слова в словаре
	 */
	typedef class Word2 : public wstring {
		private:
			/**
			 * trim Метод удаления начальных и конечных пробелов
			 * @param str строка для обработки
			 * @param loc локаль
			 * @return    результат работы функции
			 */
			static const Word2 trim(const wstring & str, const locale & loc = locale::classic()) noexcept {
				// Запоминаем итератор на первый левый символ
				auto begin = str.begin();
				// Переходим по всем символам в слове и проверяем является ли символ - символом пробела, если нашли то смещаем итератор
				while((begin != str.end()) && isspace(* begin, loc)) ++begin;
				// Если прошли все слово целиком значит пробелов нет и мы выходим
				if(begin == str.end()) return {};
				// Запоминаем итератор на первый правый символ
				auto rbegin = str.rbegin();
				// Переходим по всем символам в слове и проверяем является ли символ - символом пробела, если нашли то смещаем итератор
				while(rbegin != str.rend() && isspace(* rbegin, loc)) ++rbegin;
				// Создаем результирующую строку
				Word2 result(wstring{begin, rbegin.base()});
				// Выводим результат
				return result;
			}
		private:
			// Строка в обычных байтах
			mutable string current = "";
		public:
			// Строка не найдена
			static const size_t npos = -1;
		private:
			/**
			 * isUpCase Метод проверки на верхний регистр
			 * @param c буква для проверки
			 * @return  результат проверки
			 */
			const bool isUpCase(wchar_t c) noexcept {
				// Если буквы не равны значит это верхний регистр
				return (towlower(c) != c);
			}
		private:
			/**
			 * str Метод конвертирования строки utf-8 в строку
			 * @param  str строка utf-8 для конвертирования
			 * @return     обычная строка
			 */
			const string str(const wstring & str) const noexcept {
				// Результат работы функции
				string result = "";
				// Если строка передана
				if(!str.empty()){
					// Объявляем конвертер
					wstring_convert <codecvt_utf8 <wchar_t>> conv;
					// Выполняем конвертирование в utf-8 строку
					result = conv.to_bytes(str);
				}
				// Выводим результат
				return result;
			}
			/**
			 * wstr Метод конвертирования строки в строку utf-8
			 * @param  str строка для конвертирования
			 * @return     строка в utf-8
			 */
			const wstring wstr(const string & str) const noexcept {
				// Результат работы функции
				wstring result = L"";
				// Если строка передана
				if(!str.empty()){
					// Объявляем конвертер
					wstring_convert <codecvt_utf8 <wchar_t>> conv;
					// Выполняем конвертирование в utf-8 строку
					result = conv.from_bytes(str);
				}
				// Выводим результат
				return result;
			}
			/**
			 * lower Метод приведения строки к нижнему регистру
			 * @param  str строка для приведения к нижнему регистру
			 * @return     строка в нижнем регистре
			 */
			const wstring & lower(const wstring & str) const noexcept {
				// Получаем временную строку
				wstring * tmp = const_cast <wstring *> (&str);
				// Переходим по всем символам
				for(auto & c : * tmp) c = towlower(c);
				// Выводим результат
				return str;
			}
			/**
			 * upper Метод приведения строки к верхнему регистру
			 * @param  str строка для приведения к верхнему регистру
			 * @return     строка в верхнем регистре
			 */
			const wstring & upper(const wstring & str) const noexcept {
				// Получаем временную строку
				wstring * tmp = const_cast <wstring *> (&str);
				// Переходим по всем символам
				for(auto & c : * tmp) c = towupper(c);
				// Выводим результат
				return str;
			}
		public:
			/**
			 * clear Метод очистки строки
			 */
			void clear() noexcept {
				// Очищаем текущее слово
				this->current.clear();
				// Очищаем полученную строку
				reinterpret_cast <wstring *> (this)->clear();
			}
		public:
			/**
			 * push_back Метод добавления символа в конец строки
			 * @param c символ для добавления
			 */
			template <typename Value>
			void push_back(Value c) noexcept {
				// Изменяем строку
				this->append(1, c);
			}
		public:
			/**
			 * compare Метод сравнения строк
			 * @param  s указатель на символы в памяти
			 * @return   результат сравнения
			 */
			const int compare(const char * s) const noexcept {
				// Выводим результат
				return this->str().compare(s);
			}
			/**
			 * compare Метод сравнения строк
			 * @param  s указатель на символы в памяти
			 * @return   результат сравнения
			 */
			const int compare(const wchar_t * s) const noexcept {
				// Выводим результат
				return this->wstr().compare(s);
			}
			/**
			 * compare Метод сравнения двух слов
			 * @param word слово для сравнения
			 * @return     результат сравнения
			 */
			const int compare(const Word2 & word) const noexcept {
				// Выводим результат сравнения
				return this->compare(word.wstr());
			}
			/**
			 * compare Метод сравнения строк
			 * @param  str строка для сравнения
			 * @return     результат сравнения
			 */
			const int compare(const string & str) const noexcept {
				// Выводим результат
				return this->str().compare(str);
			}
			/**
			 * compare Метод сравнения строк
			 * @param  str строка для сравнения
			 * @return     результат сравнения
			 */
			const int compare(const wstring & str) const noexcept {
				// Выводим результат
				return this->wstr().compare(str);
			}
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  s   указатель на символы в памяти
			 * @return     результат сравнения
			 */
			const int compare(size_t pos, size_t len, const char * s) const noexcept {
				// Выводим результат
				return this->str().compare(pos, len, s);
			}
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  s   указатель на символы в памяти
			 * @return     результат сравнения
			 */
			const int compare(size_t pos, size_t len, const wchar_t * s) const noexcept {
				// Выводим результат
				return this->wstr().compare(pos, len, s);
			}
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  str строка для сравнения
			 * @return     результат сравнения
			 */
			const int compare(size_t pos, size_t len, const string & str) const noexcept {
				// Выводим результат
				return this->str().compare(pos, len, str);
			}
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  str строка для сравнения
			 * @return     результат сравнения
			 */
			const int compare(size_t pos, size_t len, const wstring & str) const noexcept {
				// Выводим результат
				return this->wstr().compare(pos, len, str);
			}
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  s   указатель на символы в памяти
			 * @param  n   количество символов
			 * @return     результат сравнения
			 */
			const int compare(size_t pos, size_t len, const char * s, size_t n) const noexcept {
				// Выводим результат
				return this->str().compare(pos, len, s, n);
			}
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  s   указатель на символы в памяти
			 * @param  n   количество символов
			 * @return     результат сравнения
			 */
			const int compare(size_t pos, size_t len, const wchar_t * s, size_t n) const noexcept {
				// Выводим результат
				return this->wstr().compare(pos, len, s, n);
			}
			/**
			 * compare Метод сравнения строк
			 * @param  pos    позиция строки для сравнения
			 * @param  len    длина строки для сравнения
			 * @param  str    строка для сравнения
			 * @param  subpos позиция подстроки для сравнения
			 * @param  sublen длина подстроки для сравнения
			 * @return        результат сравнения
			 */
			const int compare(size_t pos, size_t len, const string & str, size_t subpos, size_t sublen) const noexcept {
				// Выводим результат
				return this->str().compare(pos, len, str, subpos, sublen);
			}
			/**
			 * compare Метод сравнения строк
			 * @param  pos    позиция строки для сравнения
			 * @param  len    длина строки для сравнения
			 * @param  str    строка для сравнения
			 * @param  subpos позиция подстроки для сравнения
			 * @param  sublen длина подстроки для сравнения
			 * @return        результат сравнения
			 */
			const int compare(size_t pos, size_t len, const wstring & str, size_t subpos, size_t sublen) const noexcept {
				// Выводим результат
				return this->wstr().compare(pos, len, str, subpos, sublen);
			}
		public:
			/**
			 * append Метод добавления строки
			 * @param  word слово для добавления
			 * @return      получившаяся строка
			 */
			Word2 & append(const Word2 & word) noexcept {
				// Получаем временную строку
				const wstring & wstr = word.wstr();
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  s символ для добавления
			 * @return   получившаяся строка
			 */
			Word2 & append(const char * s) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(s);
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  s символ для добавления
			 * @return   получившаяся строка
			 */
			Word2 & append(const wchar_t * s) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(s);
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  n количество символов
			 * @param  c символ для добавления
			 * @return   получившаяся строка
			 */
			Word2 & append(size_t n, char c) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(n, c));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  n количество символов
			 * @param  c символ для добавления
			 * @return   получившаяся строка
			 */
			Word2 & append(size_t n, wchar_t c) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(n, c);
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param str строка для добавления
			 */
			Word2 & append(const string & str) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(str);
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param str строка для добавления
			 * @return    получившаяся строка
			 */
			Word2 & append(const wstring & str) noexcept {
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(str);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  s указатель на символы
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word2 & append(const char * s, size_t n) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(s, n));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  s указатель на символы
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word2 & append(const wchar_t * s, size_t n) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(s, n);
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param str    строка для добавления
			 * @param subpos начальная позиция
			 * @param sublen длина строки
			 */
			Word2 & append(const string & str, size_t subpos, size_t sublen) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(str, subpos, sublen));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param str    строка для добавления
			 * @param subpos начальная позиция
			 * @param sublen длина строки
			 * @return       получившаяся строка
			 */
			Word2 & append(const wstring & str, size_t subpos, size_t sublen) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(str, subpos, sublen);
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param first начальный итератор
			 * @param last  конечный итератор
			 * @return      получившаяся строка
			 */
			template <class InputIterator>
			Word2 & append(InputIterator first, InputIterator last) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(first, last);
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Выводим результат
				return (* this);
			}
		public:
			/**
			 * assign Метод перезаполнения строки
			 * @param  s символ для перезаписи
			 * @return   получившаяся строка
			 */
			Word2 & assign(const char * s) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(s);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  s символ для перезаписи
			 * @return   получившаяся строка
			 */
			Word2 & assign(const wchar_t * s) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(s);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 */
			Word2 & assign(const Word2 & word) noexcept {
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(word.wstr());
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  n количество символов для замены
			 * @param  c символ для замены
			 * @return   получившаяся строка
			 */
			Word2 & assign(size_t n, char c) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(n, c));
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  n количество символов для замены
			 * @param  c символ для замены
			 * @return   получившаяся строка
			 */
			Word2 & assign(size_t n, wchar_t c) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(n, c);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 * @param      получившаяся строка
			 */
			Word2 & assign(const string & str) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(str);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 * @param      получившаяся строка
			 */
			Word2 & assign(const wstring & str) noexcept {
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(str);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  s указатель на символы в памяти
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word2 & assign(const char * s, size_t n) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(s, n));
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  s указатель на символы в памяти
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word2 & assign(const wchar_t * s, size_t n) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(s, n);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  str    строка для замены
			 * @param  subpos позиция в строке
			 * @param  sublen количество символов для замены
			 * @return        получившаяся строка
			 */
			Word2 & assign(const string & str, size_t subpos, size_t sublen) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(str, subpos, sublen));
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  str    строка для замены
			 * @param  subpos позиция в строке
			 * @param  sublen количество символов для замены
			 * @return        получившаяся строка
			 */
			Word2 & assign(const wstring & str, size_t subpos, size_t sublen) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(str, subpos, sublen);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param first начальный итератор
			 * @param last  конечный итератор
			 * @return      получившаяся строка
			 */
			template <class InputIterator>
			Word2 & assign(InputIterator first, InputIterator last) noexcept {
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(first, last);
				// Выводим результат
				return (* this);
			}
		public:
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  s   строка для установки
			 * @return     получившаяся строка
			 */
			Word2 & insert(size_t pos, const char * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = this->wstr(s);
				// Вставляем строку в нужную позицию
				tmp.insert(pos, wstr.c_str());
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  s   строка для установки
			 * @return     получившаяся строка
			 */
			Word2 & insert(size_t pos, const wchar_t * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, s);
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  str строка для установки
			 * @return     получившаяся строка
			 */
			Word2 & insert(size_t pos, const Word2 & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, str.wstr());
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  str строка для установки
			 * @return     получившаяся строка
			 */
			Word2 & insert(size_t pos, const string & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, this->wstr(str));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  str строка для установки
			 * @return     получившаяся строка
			 */
			Word2 & insert(size_t pos, const wstring & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, str);
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  n   количество символов для установки
			 * @param  c   символ для установки
			 * @return     получившаяся строка
			 */
			Word2 & insert(size_t pos, size_t n, char c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, this->wstr(string(n, c)));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  n   количество символов для установки
			 * @param  c   символ для установки
			 * @return     получившаяся строка
			 */
			Word2 & insert(size_t pos, size_t n, wchar_t c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, wstring(n, c));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  s   указатель на символы в памяти
			 * @param  n   количество символов
			 * @return     получившаяся строка
			 */
			Word2 & insert(size_t pos, const char * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, this->wstr(string(s, n)));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  s   указатель на символы в памяти
			 * @param  n   количество символов
			 * @return     получившаяся строка
			 */
			Word2 & insert(size_t pos, const wchar_t * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, wstring(s, n));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos    позиция для установки
			 * @param  str    строка для установки
			 * @param  subpos позиция в строке
			 * @param  sublen количество символов для замены
			 * @return        получившаяся строка
			 */
			Word2 & insert(size_t pos, const Word2 & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, str.wstr().substr(subpos, sublen));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos    позиция для установки
			 * @param  str    строка для установки
			 * @param  subpos позиция в строке
			 * @param  sublen количество символов для замены
			 * @return        получившаяся строка
			 */
			Word2 & insert(size_t pos, const string & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, this->wstr(string(str, subpos, sublen)));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos    позиция для установки
			 * @param  str    строка для установки
			 * @param  subpos позиция в строке
			 * @param  sublen количество символов для замены
			 * @return        получившаяся строка
			 */
			Word2 & insert(size_t pos, const wstring & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, wstring(str, subpos, sublen));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  p итератор текущей позиции в строке
			 * @param  c символ для установки
			 * @return   итератор текущей позиции
			 */
			template <class iterator, class const_iterator>
			iterator insert(const_iterator p, char c) noexcept {
				// Результат работы функции
				iterator result;
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(1, c));
				// Добавляем к строке переданную строку
				result = reinterpret_cast <wstring *> (this)->insert(p, wstr.front());
				// Выводим результат
				return result;
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  p итератор текущей позиции в строке
			 * @param  c символ для установки
			 * @return   итератор текущей позиции
			 */
			template <class iterator, class const_iterator>
			iterator insert(const_iterator p, wchar_t c) noexcept {
				// Результат работы функции
				iterator result;
				// Получаем временную строку
				const wstring & wstr = wstring(1, c);
				// Добавляем к строке переданную строку
				result = reinterpret_cast <wstring *> (this)->insert(p, wstr.front());
				// Выводим результат
				return result;
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  p итератор текущей позиции в строке
			 * @param  n количество символов
			 * @param  c символ для установки
			 * @return   итератор текущей позиции
			 */
			template <class iterator, class const_iterator>
			iterator insert(const_iterator p, size_t n, char c) noexcept {
				// Результат работы функции
				iterator result;
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(n, c));
				// Добавляем к строке переданную строку
				result = reinterpret_cast <wstring *> (this)->insert(p, wstr);
				// Выводим результат
				return result;
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  p итератор текущей позиции в строке
			 * @param  n количество символов
			 * @param  c символ для установки
			 * @return   итератор текущей позиции
			 */
			template <class iterator, class const_iterator>
			iterator insert(const_iterator p, size_t n, wchar_t c) noexcept {
				// Результат работы функции
				iterator result;
				// Получаем временную строку
				const wstring & wstr = wstring(n, c);
				// Добавляем к строке переданную строку
				result = reinterpret_cast <wstring *> (this)->insert(p, wstr);
				// Выводим результат
				return result;
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  p     итератор текущей позиции в строке
			 * @param  first начальный итератор входящей строки
			 * @param  last  конечный итератор входящей строки
			 * @return       итератор текущей позиции
			 */
			template <class iterator, class InputIterator>
			iterator insert(iterator p, InputIterator first, InputIterator last) noexcept {
				// Результат работы функции
				iterator result;
				// Получаем временную строку
				const wstring & wstr = wstring(first, last);
				// Добавляем к строке переданную строку
				result = reinterpret_cast <wstring *> (this)->insert(p, wstr);
				// Выводим результат
				return result;
			}
		public:
			/**
			 * erase Метод удаления указанного количества символов
			 * @param  pos позиция в строке для удаления
			 * @param  len длина текста для удаления
			 * @return     получившаяся строка
			 */
			Word2 & erase(size_t pos = 0, size_t len = npos) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Удаляем указанные символы
				tmp.erase(pos, len);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * erase Метод удаления указанного количества символов
			 * @param  p итератор позиции для удаления
			 * @return   итератор текущей позиции
			 */
			template <class const_iterator>
			iterator erase(const_iterator p) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем индекс позиции
				const size_t index = distance(tmp.begin(), p);
				// Удаляем указанные символы
				tmp.erase(p);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (this->begin() + index);
			}
			/**
			 * erase Метод удаления указанного количества символов
			 * @param  first начальный итератор входящей строки
			 * @param  last  конечный итератор входящей строки
			 * @return       итератор текущей позиции
			 */
			template <class const_iterator>
			iterator erase(const_iterator first, const_iterator last) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем индекс позиции
				const size_t index = distance(tmp.begin(), first);
				// Удаляем указанные символы
				tmp.erase(first, last);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (this->begin() + index);
			}
		public:
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  s   указатель на символы в памяти
			 * @return     получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, const char * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(s));
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  s   указатель на символы в памяти
			 * @return     получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, const wchar_t * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = wstring(s);
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param pos  позиция с которой нужно произвести замену
			 * @param len  длина строки для замены
			 * @param word слово для замены
			 * @return     получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, const Word2 & word) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = word.wstr();
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  n   количество символов для замены
			 * @param  c   символ для замены
			 * @return     получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, size_t n, char c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(n, c));
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  n   количество символов для замены
			 * @param  c   символ для замены
			 * @return     получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, size_t n, wchar_t c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = wstring(n, c);
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param pos позиция с которой нужно произвести замену
			 * @param len длина строки для замены
			 * @param str строка для замены
			 * @return    получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, const string & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = this->wstr(str);
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param pos  позиция с которой нужно произвести замену
			 * @param len  длина строки для замены
			 * @param str  строка для замены
			 * @return     получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, const wstring & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Выполняем замену в слове
				tmp.replace(pos, len, str);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  s   указатель на символы в памяти
			 * @param  n   количество символов для замены
			 * @return     получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, const char * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(s, n));
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  s   указатель на символы в памяти
			 * @param  n   количество символов для замены
			 * @return     получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, const wchar_t * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = wstring(s, n);
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos    позиция в строке для замены
			 * @param  len    длина строки для замены
			 * @param  str    строка для замены
			 * @param  subpos позиция для замены
			 * @param  sublen количество заменяемых символов
			 * @return        получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, const string & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(str, subpos, sublen));
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos    позиция в строке для замены
			 * @param  len    длина строки для замены
			 * @param  str    строка для замены
			 * @param  subpos позиция для замены
			 * @param  sublen количество заменяемых символов
			 * @return        получившаяся строка
			 */
			Word2 & replace(size_t pos, size_t len, const wstring & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = wstring(str, subpos, sublen);
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  i1 начальный итератор
			 * @param  i2 конечный итератор
			 * @param  s  строка для замены
			 * @return    получившаяся строка
			 */
			template <class const_iterator>
			Word2 & replace(const_iterator i1, const_iterator i2, const char * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(s));
				// Выполняем замену в слове
				tmp.replace(i1, i2, wstr.c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  i1 начальный итератор
			 * @param  i2 конечный итератор
			 * @param  s  строка для замены
			 * @return    получившаяся строка
			 */
			template <class const_iterator>
			Word2 & replace(const_iterator i1, const_iterator i2, const wchar_t * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Выполняем замену в слове
				tmp.replace(i1, i2, s);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  i1 начальный итератор
			 * @param  i2 конечный итератор
			 * @param  n  количество символов для замены
			 * @param  c  символ для замены
			 * @return    получившаяся строка
			 */
			template <class const_iterator>
			Word2 & replace(const_iterator i1, const_iterator i2, size_t n, char c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(n, c));
				// Выполняем замену в слове
				tmp.replace(i1, i2, wstr.c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  i1 начальный итератор
			 * @param  i2 конечный итератор
			 * @param  n  количество символов для замены
			 * @param  c  символ для замены
			 * @return    получившаяся строка
			 */
			template <class const_iterator>
			Word2 & replace(const_iterator i1, const_iterator i2, size_t n, wchar_t c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Выполняем замену в слове
				tmp.replace(i1, i2, wstring(n, c).c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  i1  начальный итератор
			 * @param  i2  конечный итератор
			 * @param  str строка для замены
			 * @return     получившаяся строка
			 */
			template <class const_iterator>
			Word2 & replace(const_iterator i1, const_iterator i2, const Word2 & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Выполняем замену в слове
				tmp.replace(i1, i2, str.wstr().c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  i1  начальный итератор
			 * @param  i2  конечный итератор
			 * @param  str строка для замены
			 * @return     получившаяся строка
			 */
			template <class const_iterator>
			Word2 & replace(const_iterator i1, const_iterator i2, const string & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Выполняем замену в слове
				tmp.replace(i1, i2, this->wstr(str).c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  i1  начальный итератор
			 * @param  i2  конечный итератор
			 * @param  str строка для замены
			 * @return     получившаяся строка
			 */
			template <class const_iterator>
			Word2 & replace(const_iterator i1, const_iterator i2, const wstring & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Выполняем замену в слове
				tmp.replace(i1, i2, str.c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  i1 начальный итератор
			 * @param  i2 конечный итератор
			 * @param  s  указатель на символы в памяти
			 * @param  n  количество символов для замены
			 * @return    получившаяся строка
			 */
			template <class const_iterator>
			Word2 & replace(const_iterator i1, const_iterator i2, const char * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(s, n));
				// Выполняем замену в слове
				tmp.replace(i1, i2, wstr.c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  i1 начальный итератор
			 * @param  i2 конечный итератор
			 * @param  s  указатель на символы в памяти
			 * @param  n  количество символов для замены
			 * @return    получившаяся строка
			 */
			template <class const_iterator>
			Word2 & replace(const_iterator i1, const_iterator i2, const wchar_t * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wstr();
				// Выполняем замену в слове
				tmp.replace(i1, i2, wstring(s, n).c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
			/**
			 * replace Метод замены строки в тексте
			 * @param  i1    начальный итератор текущей строки
			 * @param  i2    конечный итератор текущей строки
			 * @param  first начальный итератор входящей строки
			 * @param  last  конечный итератор входящей строки
			 * @return       получившаяся строка
			 */
			template <class const_iterator, class InputIterator>
			Word2 & replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last) noexcept {
				// Получаем временную строку
				wstring tmp = this->wstr();
				// Выполняем замену в строке
				tmp.replace(i1, i2, first, last);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Выводим результат
				return (* this);
			}
		public:
			/**
			 * swap Метод обмена данными с другим текстом
			 * @param str строка для обмена данных
			 */
			void swap(Word2 & str) noexcept {
				// Получаем строку для обмена
				wstring tmp = str.wstr();
				// Выполняем обмен данными
				reinterpret_cast <wstring *> (this)->swap(tmp);
			}
			/**
			 * swap Метод обмена данными с другим текстом
			 * @param str строка для обмена данных
			 */
			void swap(string & str) noexcept {
				// Получаем строку для обмена
				wstring tmp = this->wstr(str);
				// Выполняем обмен данными
				reinterpret_cast <wstring *> (this)->swap(tmp);
			}
			/**
			 * swap Метод обмена данными с другим текстом
			 * @param str строка для обмена данных
			 */
			void swap(wstring & str) noexcept {
				// Выполняем обмен данными
				reinterpret_cast <wstring *> (this)->swap(str);
			}
		public:
			/**
			 * set Метод загрузки дампа
			 * @param dump дамп для загрузки данных
			 */
			void set(const vector <char> & dump) noexcept {
				// Если данные переданы
				if(!dump.empty()){
					// Получаем данные слова
					string word(dump.data(), dump.size());
					// Если слово не пустое
					if(!word.empty()) reinterpret_cast <wstring *> (this)->assign(this->wstr(word));
				}
			}
			/**
			 * set Метод загрузки дампа
			 * @param dump дамп для загрузки данных
			 * @param n    количество байт
			 */
			void set(const char * dump, size_t n) noexcept {
				// Если данные переданы
				if(dump != nullptr){
					// Получаем данные слова
					string word(dump, n);
					// Если слово не пустое
					if(!word.empty()) reinterpret_cast <wstring *> (this)->assign(this->wstr(word));
				}
			}
		public:
			/**
			 * dump Метод сбора дампа данных в бинарном виде
			 * @return дамп данных в байтах
			 */
			const vector <char> dump() noexcept {
				// Результат работы функции
				vector <char> result;
				// Если строка не пустая
				if(!this->empty()){
					// Получаем данные строки
					const string & str = this->str();
					// Добавляем в буфер данные строки
					result.insert(result.end(), str.begin(), str.end());
				}
				// Выводим результат
				return result;
			}
		public:
			/**
			 * back Метод получения последней буквы слова
			 * @return последняя бува слова
			 */
			wchar_t back() const noexcept {
				// Результат работы функции
				wchar_t result = 0;
				// Если слово не пустое
				if(!this->empty()) result = reinterpret_cast <const wstring *> (this)->back();
				// Выводим результат
				return result;
			}
			/**
			 * front Метод получения первой буквы слова
			 * @return первая буква слова
			 */
			wchar_t front() const noexcept {
				// Результат работы функции
				wchar_t result = 0;
				// Если слово не пустое
				if(!this->empty()) result = reinterpret_cast <const wstring *> (this)->front();
				// Выводим результат
				return result;
			}
		public:
			/**
			 * find Метод поиска подстроки в строке
			 * @param  c   символ для поиска
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t find(char c, size_t pos = 0) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(this->wstr(string(1, c)));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->find(wstr, pos);
			}
			/**
			 * find Метод поиска подстроки в строке
			 * @param  c   символ для поиска
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t find(wchar_t c, size_t pos = 0) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(wstring(1, c));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->find(wstr, pos);
			}
			/**
			 * find Метод поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t find(const char * s, size_t pos = 0) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(this->wstr(string(s)));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->find(wstr, pos);
			}
			/**
			 * find Метод поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t find(const wchar_t * s, size_t pos = 0) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(wstring(s));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->find(wstr, pos);
			}
			/**
			 * find Метод поиска подстроки в строке
			 * @param str строка для поиска
			 * @param pos позиция начиная с которой нужно производить поиск
			 * @return    найденная позиция в строке
			 */
			size_t find(const Word2 & str, size_t pos = 0) const noexcept {
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->find(str.wstr(), pos);
			}
			/**
			 * find Метод поиска подстроки в строке
			 * @param str строка для поиска
			 * @param pos позиция начиная с которой нужно производить поиск
			 * @return    найденная позиция в строке
			 */
			size_t find(const string & str, size_t pos = 0) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(this->wstr(str));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->find(wstr, pos);
			}
			/**
			 * find Метод поиска подстроки в строке
			 * @param str строка для поиска
			 * @param pos позиция начиная с которой нужно производить поиск
			 * @return    найденная позиция в строке
			 */
			size_t find(const wstring & str, size_t pos = 0) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(wstring(str));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->find(wstr, pos);
			}
			/**
			 * find Метод поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @param  n   количество символов для поиска
			 * @return     найденная позиция в строке
			 */
			size_t find(const char * s, size_t pos, size_t n) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(this->wstr(string(s)));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->find(wstr.c_str(), pos, n);
			}
			/**
			 * find Метод поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @param  n   количество символов для поиска
			 * @return     найденная позиция в строке
			 */
			size_t find(const wchar_t * s, size_t pos, size_t n) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(wstring(s));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->find(wstr.c_str(), pos, n);
			}
		public:
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  c   символ для поиска
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t rfind(char c, size_t pos = npos) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(this->wstr(string(1, c)));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->rfind(wstr, pos);
			}
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  c   символ для поиска
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t rfind(wchar_t c, size_t pos = npos) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(wstring(1, c));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->rfind(wstr, pos);
			}
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t rfind(const char * s, size_t pos = npos) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(this->wstr(string(s)));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->rfind(wstr, pos);
			}
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t rfind(const wchar_t * s, size_t pos = npos) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(wstring(s));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->rfind(wstr, pos);
			}
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param str строка для поиска
			 * @param pos позиция в строке
			 * @return    найденная позиция в строке
			 */
			size_t rfind(const Word2 & str, size_t pos = npos) const noexcept {
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->rfind(str.wstr(), pos);
			}
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param str строка для поиска
			 * @param pos позиция начиная с которой нужно производить поиск
			 * @return    найденная позиция в строке
			 */
			size_t rfind(const string & str, size_t pos = npos) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(this->wstr(str));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->rfind(wstr, pos);
			}
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param str строка для поиска
			 * @param pos позиция начиная с которой нужно производить поиск
			 * @return    найденная позиция в строке
			 */
			size_t rfind(const wstring & str, size_t pos = npos) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(wstring(str));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->rfind(wstr, pos);
			}
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @param  n   количество символов для поиска
			 * @return     найденная позиция в строке
			 */
			size_t rfind(const char * s, size_t pos, size_t n) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(this->wstr(string(s)));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->rfind(wstr.c_str(), pos, n);
			}
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @param  n   количество символов для поиска
			 * @return     найденная позиция в строке
			 */
			size_t rfind(const wchar_t * s, size_t pos, size_t n) const noexcept {
				// Получаем временную строку
				const wstring & wstr = this->lower(wstring(s));
				// Устанавливаем полученную строку
				return reinterpret_cast <const wstring *> (this)->rfind(wstr.c_str(), pos, n);
			}
		public:
			/**
			 * trim Метод удаления пробелов вначале и конце слова
			 * @return получившаяся строка
			 */
			Word2 trim() const noexcept {
				// Выводим результат
				return trim(this->wstr());
			}
			/**
			 * lower Метод приведения в нижний регистр
			 * @return получившаяся строка
			 */
			string lower() const noexcept {
				// Получаем строку из текущего слова
				wstring word = this->wstr();
				// Переходим по всем символам
				for(auto & c : word) c = towlower(c);
				// Выводим результат
				return this->str(word);
			}
			/**
			 * upper Метод приведения в верхний регистр
			 * @return получившаяся строка
			 */
			string upper() const noexcept {
				// Получаем строку из текущего слова
				wstring word = this->wstr();
				// Переходим по всем символам
				for(auto & c : word) c = towupper(c);
				// Выводим результат
				return this->str(word);
			}
			/**
			 * substr Метод обрезки сроки
			 * @param  pos позиция для начала обрезки
			 * @param  len количество символов для обрезки
			 * @return     получившаяся строка
			 */
			Word2 substr(size_t pos = 0, size_t len = npos) const noexcept {
				// Получаем строку из текущего слова
				wstring word = this->wstr();
				// Выводим результат
				return Word2(word.substr(pos, len));
			}
		public:
			/**
			 * data Данные строки
			 * @return указатель на бинарные данные
			 */
			const char * data() const noexcept {
				// Получаем текущую строку
				this->current = this->str();
				// Выводим результат
				return this->current.data();
			}
			/**
			 * c_str Метод получения указателя на строку
			 * @return указатель на строку
			 */
			const char * c_str() const noexcept {
				// Получаем текущую строку
				this->current = this->str();
				// Выводим результат
				return this->current.c_str();
			}
			/**
			 * copy Метод копирования данных строки в указанный буфер
			 * @param  s   буфер куда нужно скопировать данные
			 * @param  len размер данных для копирования в буфер
			 * @param  pos позиция в строке для копирования
			 * @return     количество скопированных байт
			 */
			size_t copy(char * s, size_t len, size_t pos = 0) const noexcept {
				// Получаем текущее значение слова
				const string & word = this->str();
				// Выполняем копирование данных
				return word.copy(s, len, pos);
			}
			/**
			 * copy Метод копирования данных строки в указанный буфер
			 * @param  s   буфер куда нужно скопировать данные
			 * @param  len размер данных для копирования в буфер
			 * @param  pos позиция в строке для копирования
			 * @return     количество скопированных байт
			 */
			size_t copy(wchar_t * s, size_t len, size_t pos = 0) const noexcept {
				// Получаем текущее значение слова
				const wstring & word = this->wstr();
				// Выполняем копирование данных
				return word.copy(s, len, pos);
			}
		public:
			/**
			 * str Метод получения строки
			 * @return строка в чистом виде
			 */
			const string str() const noexcept {
				// Результат работы функции
				string result = "";
				// Если строка не пустая
				if(!this->empty()){
					// Выводим результат
					result = this->str(* reinterpret_cast <const wstring *> (this));
				}
				// Выводим результат
				return result;
			}
			/**
			 * wstr Метод получения строки
			 * @return строка в чистом виде
			 */
			const wstring wstr() const noexcept {
				// Результат работы функции
				wstring result = L"";
				// Если строка не пустая
				if(!this->empty()){
					// Выводим результат
					result = (* reinterpret_cast <const wstring *> (this));
				}
				// Выводим результат
				return result;
			}
		public:
			/**
			 * Оператор объединения
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			Word2 operator + (char c) noexcept {
				// Получаем строку слова
				string word = this->str();
				// Добавляем к слову символ
				word.append(1, c);
				// Выводим результат
				return Word2(word);
			}
			/**
			 * Оператор объединения
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			Word2 operator + (wchar_t c) noexcept {
				// Получаем строку слова
				wstring word = this->wstr();
				// Добавляем к слову символ
				word.append(1, c);
				// Выводим результат
				return Word2(word);
			}
			/**
			 * Оператор объединения
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			Word2 operator + (const char * s) noexcept {
				// Получаем строку слова
				string word = this->str();
				// Добавляем к слову символ
				word.append(s);
				// Выводим результат
				return Word2(word);
			}
			/**
			 * Оператор объединения
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			Word2 operator + (const wchar_t * s) noexcept {
				// Получаем строку слова
				wstring word = this->wstr();
				// Добавляем к слову символ
				word.append(s);
				// Выводим результат
				return Word2(word);
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word2 operator + (const Word2 & word) noexcept {
				// Получаем строку слова
				Word2 result = (* this);
				// Добавляем к слову символ
				result.append(word);
				// Выводим результат
				return result;
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word2 operator + (const string & word) noexcept {
				// Получаем строку слова
				string str = this->str();
				// Добавляем к слову символ
				str.append(word);
				// Выводим результат
				return Word2(str);
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word2 operator + (const wstring & word) noexcept {
				// Получаем строку слова
				wstring str = this->wstr();
				// Добавляем к слову символ
				str.append(word);
				// Выводим результат
				return Word2(str);
			}
		public:
			/**
			 * Оператор присваивания
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			template <typename Value>
			Word2 & operator = (Value c) noexcept {
				// Присваиваем новое значение
				return this->assign(1, c);
			}
			/**
			 * Оператор присваивания
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			template <typename Value>
			Word2 & operator = (const Value * s) noexcept {
				// Присваиваем новое значение
				return this->assign(s);
			}
			/**
			 * Оператор присваивания
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word2 & operator = (const Word2 & word) noexcept {
				// Присваиваем новое значение
				return this->assign(word);
			}
			/**
			 * Оператор присваивания
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word2 & operator = (const string & word) noexcept {
				// Присваиваем новое значение
				return this->assign(word);
			}
			/**
			 * Оператор присваивания
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word2 & operator = (const wstring & word) noexcept {
				// Присваиваем новое значение
				return this->assign(word);
			}
		public:
			/**
			 * Оператор сложения и присваивания
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			template <typename Value>
			Word2 & operator += (Value c) noexcept {
				// Добавляем новое значение
				return this->append(1, c);
			}
			/**
			 * Оператор сложения и присваивания
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			template <typename Value>
			Word2 & operator += (const Value * s) noexcept {
				// Добавляем новое значение
				return this->append(s);
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word2 & operator += (const Word2 & word) noexcept {
				// Добавляем новое значение
				return this->append(word);
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word2 & operator += (const string & word) noexcept {
				// Добавляем новое значение
				return this->append(word);
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word2 & operator += (const wstring & word) noexcept {
				// Добавляем новое значение
				return this->append(word);
			}
		public:
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator < (const Word2 & word) noexcept {
				// Выводим результат
				return (this->size() < word.size());
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator < (const string & word) noexcept {
				// Выводим результат
				return (this->size() < this->wstr(word).size());
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator < (const wstring & word) noexcept {
				// Выводим результат
				return (this->size() < word.size());
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator > (const Word2 & word) noexcept {
				// Выводим результат
				return (this->size() > word.size());
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator > (const string & word) noexcept {
				// Выводим результат
				return (this->size() > this->wstr(word).size());
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator > (const wstring & word) noexcept {
				// Выводим результат
				return (this->size() > word.size());
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator <= (const Word2 & word) noexcept {
				// Выводим результат
				return ((this->compare(word) == 0) || (this->size() < word.size()));
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator <= (const string & word) noexcept {
				// Выводим результат
				return ((this->compare(word) == 0) || (this->size() < this->wstr(word).size()));
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator <= (const wstring & word) noexcept {
				// Выводим результат
				return ((this->compare(word) == 0) || (this->size() < word.size()));
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator >= (const Word2 & word) noexcept {
				// Выводим результат
				return ((this->compare(word) == 0) || (this->size() > word.size()));
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator >= (const string & word) noexcept {
				// Выводим результат
				return ((this->compare(word) == 0) || (this->size() > this->wstr(word).size()));
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator >= (const wstring & word) noexcept {
				// Выводим результат
				return ((this->compare(word) == 0) || (this->size() > word.size()));
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator == (const Word2 & word) noexcept {
				// Выводим результат
				return (this->compare(word) == 0);
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator == (const string & word) noexcept {
				// Выводим результат
				return (this->compare(word) == 0);
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator == (const wstring & word) noexcept {
				// Выводим результат
				return (this->compare(word) == 0);
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator != (const Word2 & word) noexcept {
				// Выводим результат
				return (this->compare(word) != 0);
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator != (const string & word) noexcept {
				// Выводим результат
				return (this->compare(word) != 0);
			}
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator != (const wstring & word) noexcept {
				// Выводим результат
				return (this->compare(word) != 0);
			}
		public:
			/**
			 * Word2 Конструктор
			 */
			Word2() noexcept {}
			/**
			 * Word2 Конструктор
			 * @param c символ для вставки
			 */
			template <typename Value>
			Word2(Value c) noexcept {
				// Изменяем строку
				this->assign(1, c);
			}
			/**
			 * Word2 Конструктор
			 * @param s указатель на символы в памяти
			 */
			template <typename Value>
			Word2(const Value * s) noexcept {
				// Изменяем строку
				this->assign(s);
			}
			/**
			 * Word2 Конструктор
			 * @param n количество символов для вставки
			 * @param c символ для вставки
			 */
			template <typename Value>
			Word2(size_t n, Value c) noexcept {
				// Изменяем строку
				this->assign(n, c);
			}
			/**
			 * Word2 Конструктор
			 * @param str строка для установки
			 */
			Word2(const string & str) noexcept {
				// Изменяем строку
				this->assign(str);
			}
			/**
			 * Word2 Конструктор
			 * @param str строка для установки
			 */
			Word2(const wstring & str) noexcept {
				// Изменяем строку
				this->assign(str);
			}
			/**
			 * Word2 Конструктор
			 * @param s указатель на символы в памяти
			 * @param n количество символов для вставки
			 */
			template <typename Value>
			Word2(const Value * s, size_t n) noexcept {
				// Изменяем строку
				this->assign(s, n);
			}
			/**
			 * Word2 Конструктор
			 * @param word слово для инициализации
			 * @param pos  позиция для вставки
			 * @param len  длина слова для вставки
			 */
			Word2(const string & word, size_t pos, size_t len = npos) noexcept {
				// Изменяем строку
				this->assign(word, pos, len);
			}
			/**
			 * Word2 Конструктор
			 * @param word слово для инициализации
			 * @param pos  позиция для вставки
			 * @param len  длина слова для вставки
			 */
			Word2(const wstring & word, size_t pos, size_t len = npos) noexcept {
				// Изменяем строку
				this->assign(word, pos, len);
			}
			/**
			 * Word2 Конструктор
			 * @param first начальный итератор
			 * @param last  конечный итератор
			 */
			template <class InputIterator>
			Word2(InputIterator first, InputIterator last) noexcept {
				// Изменяем строку
				this->assign(first, last);
			}
			/**
			 * ~Word2 Деструктор
			 */
			~Word2() noexcept {
				// Очищаем все данные
				this->clear();
			}
	} word2_t;
};

#endif // _ANYKS_WORD2_

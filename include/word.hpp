/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_WORD_
#define _ANYKS_WORD_

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
	 * OCDC Структура данных веса слова
	 */
	typedef struct OCDC {
		size_t oc, dc;
		/**
		 * OCDC Конструктор
		 * @param oc встречаемость слова во всех документах
		 * @param dc количество документов где встретилось слово
		 */
		OCDC(const size_t oc = 0, const size_t dc = 0) : oc(oc), dc(dc) {}
	} __attribute__((packed)) ocdc_t;
	/**
	 * Meta Структура метаблока словаря
	 */
	typedef struct Meta {
		size_t oc;
		size_t dc;
		float tf;
		float idf;
		float wltf;
		/**
		 * Оператор объединения
		 * @param meta объект структуры для объединения
		 */
		Meta operator + (const Meta & meta);
		/**
		 * Оператор присваивания
		 * @param meta объект структуры для присваивания
		 */
		Meta & operator = (const Meta & meta);
		/**
		 * Оператор объединения
		 * @param meta объект структуры для объединения
		 */
		Meta & operator += (const Meta & meta);
		/**
		 * Meta Конструктор
		 * @param oc   встречаемость слова во всех документах
		 * @param dc   количество документов где встретилось слово
		 * @param tf   частота встречаемости слова в документах
		 * @param idf  обратная частота документа
		 * @param wltf расчитанный вес слова
		 */
		Meta(size_t oc = 0, size_t dc = 0, float tf = -1e+20f, float idf = -1e+20f, float wltf = -1e+20f) : oc(oc), dc(dc), tf(tf), idf(idf), wltf(wltf) {}
	} __attribute__((packed)) meta_t;
	/**
	 * Metrics Класс расчета метрик слова
	 */
	typedef class Metrics {
		private:
			size_t ad;
			size_t cw;
			size_t dc;
			size_t oc;
		public:
			/**
			 * calc Метод расчета метрик для слова
			 * @return результат расчета метрик
			 */
			const meta_t calc();
			/**
			 * Metrics Конструктор
			 * @param ad количество используемых документов
			 * @param cw количество обработанных слов
			 * @param dc количество документов где встретилось слово
			 * @param oc количество встречь слова в текстах
			 */
			Metrics(size_t ad = 0, size_t cw = 0, size_t dc = 0, size_t oc = 0) : ad(ad), cw(cw), dc(dc), oc(oc) {}
	} __attribute__((packed)) metric_t;
	/**
	 * Word Класс слова в словаре
	 */
	typedef class Word : public std::wstring {
		private:
			// Список позиций букв в верхнем регистре
			size_t uppers = 0;
			// Метаданные слова
			size_t dc = 0, oc = 0;
			// Строка в обычных байтах
			mutable string current = "";
		public:
			// Строка не найдена
			static const size_t npos = -1;
		private:
			/**
			 * wordCase Метод запоминания регистра слова
			 */
			void wordCase();
			/**
			 * isUpCase Метод проверки на верхний регистр
			 * @param c буква для проверки
			 * @return  результат проверки
			 */
			bool isUpCase(wchar_t c);
			/**
			 * str Метод конвертирования строки utf-8 в строку
			 * @param  str строка utf-8 для конвертирования
			 * @return     обычная строка
			 */
			const string str(const wstring & str) const;
			/**
			 * wstr Метод конвертирования строки в строку utf-8
			 * @param  str строка для конвертирования
			 * @return     строка в utf-8
			 */
			const wstring wstr(const string & str) const;
		public:
			/**
			 * clear Метод очистки строки
			 */
			void clear();
			/**
			 * push_back Метод добавления символа в конец строки
			 * @param c символ для добавления
			 */
			void push_back(char c);
			/**
			 * push_back Метод добавления символа в конец строки
			 * @param c символ для добавления
			 */
			void push_back(wchar_t c);
			/**
			 * setUppers Метод установки списка регистров в бинарном виде
			 * @param uppers список регистров в бинарном виде
			 */
			void setUppers(const size_t uppers);
			/**
			 * compare Метод сравнения строк
			 * @param  str строка для сравнения
			 * @return     результат сравнения
			 */
			int compare(const string & str) const;
			/**
			 * compare Метод сравнения строк
			 * @param  str строка для сравнения
			 * @return     результат сравнения
			 */
			int compare(const wstring & str) const;
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  str строка для сравнения
			 * @return     результат сравнения
			 */
			int compare(size_t pos, size_t len, const string & str) const;
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  str строка для сравнения
			 * @return     результат сравнения
			 */
			int compare(size_t pos, size_t len, const wstring & str) const;
			/**
			 * compare Метод сравнения строк
			 * @param  pos    позиция строки для сравнения
			 * @param  len    длина строки для сравнения
			 * @param  str    строка для сравнения
			 * @param  subpos позиция подстроки для сравнения
			 * @param  sublen длина подстроки для сравнения
			 * @return        результат сравнения
			 */
			int compare(size_t pos, size_t len, const string & str, size_t subpos, size_t sublen) const;
			/**
			 * compare Метод сравнения строк
			 * @param  pos    позиция строки для сравнения
			 * @param  len    длина строки для сравнения
			 * @param  str    строка для сравнения
			 * @param  subpos позиция подстроки для сравнения
			 * @param  sublen длина подстроки для сравнения
			 * @return        результат сравнения
			 */
			int compare(size_t pos, size_t len, const wstring & str, size_t subpos, size_t sublen) const;
			/**
			 * compare Метод сравнения строк
			 * @param  s указатель на символы в памяти
			 * @return   результат сравнения
			 */
			int compare(const char * s) const;
			/**
			 * compare Метод сравнения строк
			 * @param  s указатель на символы в памяти
			 * @return   результат сравнения
			 */
			int compare(const wchar_t * s) const;
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  s   указатель на символы в памяти
			 * @return     результат сравнения
			 */
			int compare(size_t pos, size_t len, const char * s) const;
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  s   указатель на символы в памяти
			 * @return     результат сравнения
			 */
			int compare(size_t pos, size_t len, const wchar_t * s) const;
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  s   указатель на символы в памяти
			 * @param  n   количество символов
			 * @return     результат сравнения
			 */
			int compare(size_t pos, size_t len, const char * s, size_t n) const;
			/**
			 * compare Метод сравнения строк
			 * @param  pos позиция строки для сравнения
			 * @param  len длина строки для сравнения
			 * @param  s   указатель на символы в памяти
			 * @param  n   количество символов
			 * @return     результат сравнения
			 */
			int compare(size_t pos, size_t len, const wchar_t * s, size_t n) const;
			/**
			 * compare Метод сравнения двух слов
			 * @param word слово для сравнения
			 * @return     результат сравнения
			 */
			int compare(const Word & word) const;
			/**
			 * append Метод добавления строки
			 * @param str строка для добавления
			 */
			Word & append(const string & str);
			/**
			 * append Метод добавления строки
			 * @param str строка для добавления
			 * @return    получившаяся строка
			 */
			Word & append(const wstring & str);
			/**
			 * append Метод добавления строки
			 * @param str    строка для добавления
			 * @param subpos начальная позиция
			 * @param sublen длина строки
			 */
			Word & append(const string & str, size_t subpos, size_t sublen);
			/**
			 * append Метод добавления строки
			 * @param str    строка для добавления
			 * @param subpos начальная позиция
			 * @param sublen длина строки
			 * @return       получившаяся строка
			 */
			Word & append(const wstring & str, size_t subpos, size_t sublen);
			/**
			 * append Метод добавления строки
			 * @param  s символ для добавления
			 * @return   получившаяся строка
			 */
			Word & append(const char * s);
			/**
			 * append Метод добавления строки
			 * @param  s символ для добавления
			 * @return   получившаяся строка
			 */
			Word & append(const wchar_t * s);
			/**
			 * append Метод добавления строки
			 * @param  s указатель на символы
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word & append(const char * s, size_t n);
			/**
			 * append Метод добавления строки
			 * @param  s указатель на символы
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word & append(const wchar_t * s, size_t n);
			/**
			 * append Метод добавления строки
			 * @param  n количество символов
			 * @param  c символ для добавления
			 * @return   получившаяся строка
			 */
			Word & append(size_t n, char c);
			/**
			 * append Метод добавления строки
			 * @param  n количество символов
			 * @param  c символ для добавления
			 * @return   получившаяся строка
			 */
			Word & append(size_t n, wchar_t c);
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 * @param      получившаяся строка
			 */
			Word & assign(const string & str);
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 * @param      получившаяся строка
			 */
			Word & assign(const wstring & str);
			/**
			 * assign Метод перезаполнения строки
			 * @param  str    строка для замены
			 * @param  subpos позиция в строке
			 * @param  sublen количество символов для замены
			 * @return        получившаяся строка
			 */
			Word & assign(const string & str, size_t subpos, size_t sublen);
			/**
			 * assign Метод перезаполнения строки
			 * @param  str    строка для замены
			 * @param  subpos позиция в строке
			 * @param  sublen количество символов для замены
			 * @return        получившаяся строка
			 */
			Word & assign(const wstring & str, size_t subpos, size_t sublen);
			/**
			 * assign Метод перезаполнения строки
			 * @param  s символ для перезаписи
			 * @return   получившаяся строка
			 */
			Word & assign(const char * s);
			/**
			 * assign Метод перезаполнения строки
			 * @param  s символ для перезаписи
			 * @return   получившаяся строка
			 */
			Word & assign(const wchar_t * s);
			/**
			 * assign Метод перезаполнения строки
			 * @param  s указатель на символы в памяти
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word & assign(const char * s, size_t n);
			/**
			 * assign Метод перезаполнения строки
			 * @param  s указатель на символы в памяти
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word & assign(const wchar_t * s, size_t n);
			/**
			 * assign Метод перезаполнения строки
			 * @param  n количество символов для замены
			 * @param  c символ для замены
			 * @return   получившаяся строка
			 */
			Word & assign(size_t n, char c);
			/**
			 * assign Метод перезаполнения строки
			 * @param  n количество символов для замены
			 * @param  c символ для замены
			 * @return   получившаяся строка
			 */
			Word & assign(size_t n, wchar_t c);
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 * @param meta метаданные слова
			 */
			Word & assign(const string & word, const ocdc_t & meta);
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 * @param meta метаданные слова
			 */
			Word & assign(const wstring & word, const ocdc_t & meta);
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 */
			Word & assign(const Word & word);
			/**
			 * replace Метод замены строки в тексте
			 * @param pos позиция с которой нужно произвести замену
			 * @param len длина строки для замены
			 * @param str строка для замены
			 * @return    получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, const string & str);
			/**
			 * replace Метод замены строки в тексте
			 * @param pos  позиция с которой нужно произвести замену
			 * @param len  длина строки для замены
			 * @param str  строка для замены
			 * @return     получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, const wstring & str);
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos    позиция в строке для замены
			 * @param  len    длина строки для замены
			 * @param  str    строка для замены
			 * @param  subpos позиция для замены
			 * @param  sublen количество заменяемых символов
			 * @return        получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, const string & str, size_t subpos, size_t sublen);
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos    позиция в строке для замены
			 * @param  len    длина строки для замены
			 * @param  str    строка для замены
			 * @param  subpos позиция для замены
			 * @param  sublen количество заменяемых символов
			 * @return        получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, const wstring & str, size_t subpos, size_t sublen);
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  s   указатель на символы в памяти
			 * @return     получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, const char * s);
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  s   указатель на символы в памяти
			 * @return     получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, const wchar_t * s);
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  s   указатель на символы в памяти
			 * @param  n   количество символов для замены
			 * @return     получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, const char * s, size_t n);
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  s   указатель на символы в памяти
			 * @param  n   количество символов для замены
			 * @return     получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, const wchar_t * s, size_t n);
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  n   количество символов для замены
			 * @param  c   символ для замены
			 * @return     получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, size_t n, char c);
			/**
			 * replace Метод замены строки в тексте
			 * @param  pos позиция в строке для замены
			 * @param  len длина строки для замены
			 * @param  n   количество символов для замены
			 * @param  c   символ для замены
			 * @return     получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, size_t n, wchar_t c);
			/**
			 * replace Метод замены строки в тексте
			 * @param pos  позиция с которой нужно произвести замену
			 * @param len  длина строки для замены
			 * @param word слово для замены
			 * @return     получившаяся строка
			 */
			Word & replace(size_t pos, size_t len, const Word & word);
			/**
			 * set Метод загрузки дампа
			 * @param dump дамп для загрузки данных
			 */
			void set(const vector <char> & dump);
			/**
			 * set Метод загрузки дампа
			 * @param dump дамп для загрузки данных
			 * @param n    количество байт
			 */
			void set(const char * dump, size_t n);
			/**
			 * dump Метод сбора дампа данных в бинарном виде
			 * @return дамп данных в байтах
			 */
			const vector <char> dump();
			/**
			 * back Метод получения последней буквы слова
			 * @return последняя бува слова
			 */
			wchar_t back() const;
			/**
			 * front Метод получения первой буквы слова
			 * @return первая буква слова
			 */
			wchar_t front() const;
			/**
			 * getUppers Метод получения список регистров
			 * @return список регистров в бинарном виде
			 */
			size_t getUppers() const;
			/**
			 * find Метод поиска подстроки в строке
			 * @param str строка для поиска
			 * @param pos позиция начиная с которой нужно производить поиск
			 * @return    найденная позиция в строке
			 */
			size_t find(const string & str, size_t pos = 0) const;
			/**
			 * find Метод поиска подстроки в строке
			 * @param str строка для поиска
			 * @param pos позиция начиная с которой нужно производить поиск
			 * @return    найденная позиция в строке
			 */
			size_t find(const wstring & str, size_t pos = 0) const;
			/**
			 * find Метод поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t find(const char * s, size_t pos = 0) const;
			/**
			 * find Метод поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t find(const wchar_t * s, size_t pos = 0) const;
			/**
			 * find Метод поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @param  n   количество символов для поиска
			 * @return     найденная позиция в строке
			 */
			size_t find(const char * s, size_t pos, size_t n) const;
			/**
			 * find Метод поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @param  n   количество символов для поиска
			 * @return     найденная позиция в строке
			 */
			size_t find(const wchar_t * s, size_t pos, size_t n) const;
			/**
			 * find Метод поиска подстроки в строке
			 * @param  c   символ для поиска
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t find(char c, size_t pos = 0) const;
			/**
			 * find Метод поиска подстроки в строке
			 * @param  c   символ для поиска
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t find(wchar_t c, size_t pos = 0) const;
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param str строка для поиска
			 * @param pos позиция начиная с которой нужно производить поиск
			 * @return    найденная позиция в строке
			 */
			size_t rfind(const string & str, size_t pos = npos) const;
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param str строка для поиска
			 * @param pos позиция начиная с которой нужно производить поиск
			 * @return    найденная позиция в строке
			 */
			size_t rfind(const wstring & str, size_t pos = npos) const;
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t rfind(const char * s, size_t pos = npos) const;
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t rfind(const wchar_t * s, size_t pos = npos) const;
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @param  n   количество символов для поиска
			 * @return     найденная позиция в строке
			 */
			size_t rfind(const char * s, size_t pos, size_t n) const;
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  s   указатель на символы в памяти
			 * @param  pos позиция в строке
			 * @param  n   количество символов для поиска
			 * @return     найденная позиция в строке
			 */
			size_t rfind(const wchar_t * s, size_t pos, size_t n) const;
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  c   символ для поиска
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t rfind(char c, size_t pos = npos) const;
			/**
			 * rfind Метод обратного поиска подстроки в строке
			 * @param  c   символ для поиска
			 * @param  pos позиция в строке
			 * @return     найденная позиция в строке
			 */
			size_t rfind(wchar_t c, size_t pos = npos) const;
			/**
			 * trim Метод удаления пробелов вначале и конце слова
			 * @return получившаяся строка
			 */
			Word trim() const;
			/**
			 * lower Метод приведения в нижний регистр
			 * @return получившаяся строка
			 */
			Word lower() const;
			/**
			 * upper Метод приведения в верхний регистр
			 * @return получившаяся строка
			 */
			Word upper() const;
			/**
			 * substr Метод обрезки сроки
			 * @param  pos позиция для начала обрезки
			 * @param  len количество символов для обрезки
			 * @return     получившаяся строка
			 */
			Word substr(size_t pos = 0, size_t len = npos) const;
			/**
			 * data Данные строки
			 * @return указатель на бинарные данные
			 */
			const char * data() const;
			/**
			 * c_str Метод получения указателя на строку
			 * @return указатель на строку
			 */
			const char * c_str() const;
			/**
			 * str Метод получения строки
			 * @return строка в чистом виде
			 */
			const string str() const;
			/**
			 * wstr Метод получения строки
			 * @return строка в чистом виде
			 */
			const wstring wstr() const;
			/**
			 * real Метод извлечения слова с учётом регистра
			 * @return слово с учётом регистра
			 */
			const string real() const;
			/**
			 * wreal Метод извлечения слова с учётом регистра
			 * @return слово с учётом регистра
			 */
			const wstring wreal() const;
			/**
			 * getmeta Метод получения метаданных
			 * @return метаданные
			 */
			const ocdc_t getmeta() const;
			/**
			 * calc Метод подсчёта параметров слова
			 * @param ad количество используемых документов
			 * @param cw количество обработанных слов
			 * @return   подсчитанные параметры слова
			 */
			const meta_t calc(const size_t ad, const size_t cw) const;
			/**
			 * setmeta Метод установки метаданных
			 * @param meta метаданные слова
			 */
			void setmeta(const ocdc_t & meta);
			/**
			 * setmeta Метод установки метаданных
			 * @param dc количество документов где встретилось слово
			 * @param oc количество встречь слова в текстах
			 */
			void setmeta(const size_t dc, const size_t oc);
			/**
			 * Оператор объединения
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			Word operator + (const char * s);
			/**
			 * Оператор объединения
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			Word operator + (const wchar_t * s);
			/**
			 * Оператор объединения
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			Word operator + (char c);
			/**
			 * Оператор объединения
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			Word operator + (wchar_t c);
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word operator + (const string & word);
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word operator + (const wstring & word);
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word operator + (const Word & word);
			/**
			 * Оператор присваивания
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			Word & operator = (const char * s);
			/**
			 * Оператор присваивания
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			Word & operator = (const wchar_t * s);
			/**
			 * Оператор присваивания
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			Word & operator = (char c);
			/**
			 * Оператор присваивания
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			Word & operator = (wchar_t c);
			/**
			 * Оператор присваивания
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator = (const string & word);
			/**
			 * Оператор присваивания
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator = (const wstring & word);
			/**
			 * Оператор присваивания
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator = (const Word & word);
			/**
			 * Оператор сложения и присваивания
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			Word & operator += (const char * s);
			/**
			 * Оператор сложения и присваивания
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			Word & operator += (const wchar_t * s);
			/**
			 * Оператор сложения и присваивания
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			Word & operator += (char c);
			/**
			 * Оператор сложения и присваивания
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			Word & operator += (wchar_t c);
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator += (const Word & word);
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator += (const string & word);
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator += (const wstring & word);
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator == (const Word & word);
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator != (const Word & word);
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator < (const Word & word);
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator > (const Word & word);
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator <= (const Word & word);
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator >= (const Word & word);
			/**
			 * Word Конструктор
			 */
			Word();
			/**
			 * Word Конструктор
			 * @param word слово для инициализации
			 * @param pos  позиция для вставки
			 * @param len  длина слова для вставки
			 */
			Word(const string & word, size_t pos, size_t len = npos);
			/**
			 * Word Конструктор
			 * @param word слово для инициализации
			 * @param pos  позиция для вставки
			 * @param len  длина слова для вставки
			 */
			Word(const wstring & word, size_t pos, size_t len = npos);
			/**
			 * Word Конструктор
			 * @param s указатель на символы в памяти
			 */
			Word(const char * s);
			/**
			 * Word Конструктор
			 * @param s указатель на символы в памяти
			 */
			Word(const wchar_t * s);
			/**
			 * Word Конструктор
			 * @param s указатель на символы в памяти
			 * @param n количество символов для вставки
			 */
			Word(const char * s, size_t n);
			/**
			 * Word Конструктор
			 * @param s указатель на символы в памяти
			 * @param n количество символов для вставки
			 */
			Word(const wchar_t * s, size_t n);
			/**
			 * Word Конструктор
			 * @param n количество символов для вставки
			 * @param c символ для вставки
			 */
			Word(size_t n, char c);
			/**
			 * Word Конструктор
			 * @param n количество символов для вставки
			 * @param c символ для вставки
			 */
			Word(size_t n, wchar_t c);
			/**
			 * Word Конструктор
			 * @param str строка для установки
			 */
			Word(const string & str);
			/**
			 * Word Конструктор
			 * @param str строка для установки
			 */
			Word(const wstring & str);
			/**
			 * Word Конструктор
			 * @param str строка для установки
			 * @param meta метаданные слова
			 */
			Word(const string & str, const ocdc_t & meta);
			/**
			 * Word Конструктор
			 * @param str строка для установки
			 * @param meta метаданные слова
			 */
			Word(const wstring & str, const ocdc_t & meta);
			/**
			 * ~Word Деструктор
			 */
			~Word();
	} word_t;
	/**
	 * Оператор чтения из потока
	 * @param is   поток для чтения
	 * @param word слово куда нужно считать данные из потока
	 */
	istream & operator >> (istream & is, Word & word);
	/**
	 * Оператор вывода в поток из слова
	 * @param os   поток куда нужно вывести данные
	 * @param word слово из которого нужно вывести данные
	 */
	ostream & operator << (ostream & os, const Word & word);
};

#endif // _ANYKS_WORD_

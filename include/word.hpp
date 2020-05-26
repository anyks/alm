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
#include <locale>
#include <string>
#include <vector>
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
		double tf;
		double idf;
		double wltf;
		/**
		 * Оператор объединения
		 * @param meta объект структуры для объединения
		 */
		Meta operator + (const Meta & meta) noexcept {
			// Создаем новый объект
			Meta result = {
				this->oc + meta.oc,
				this->dc + meta.dc,
				this->tf + meta.tf,
				this->idf + meta.idf,
				this->wltf + meta.wltf
			};
			// Выводим результат
			return result;
		}
		/**
		 * Оператор присваивания
		 * @param meta объект структуры для присваивания
		 */
		Meta & operator = (const Meta & meta) noexcept {
			// Присваиваем новые значения
			this->oc = meta.oc;
			this->dc = meta.dc;
			this->tf = meta.tf;
			this->idf = meta.idf;
			this->wltf = meta.wltf;
			// Выводим результат
			return (* this);
		}
		/**
		 * Оператор объединения
		 * @param meta объект структуры для объединения
		 */
		Meta & operator += (const Meta & meta) noexcept {
			// Увеличиваем значение текущего объекта
			this->oc += meta.oc;
			this->dc += meta.dc;
			this->tf += meta.tf;
			this->idf += meta.idf;
			this->wltf += meta.wltf;
			// Выводим результат
			return (* this);
		}
		/**
		 * Meta Конструктор
		 * @param oc   встречаемость слова во всех документах
		 * @param dc   количество документов где встретилось слово
		 * @param tf   частота встречаемости слова в документах
		 * @param idf  обратная частота документа
		 * @param wltf расчитанный вес слова
		 */
		Meta(size_t oc = 0, size_t dc = 0, double tf = -1e+20, double idf = -1e+20, double wltf = -1e+20) noexcept : oc(oc), dc(dc), tf(tf), idf(idf), wltf(wltf) {}
	} __attribute__((packed)) meta_t;
	/**
	 * Metrics Класс расчета метрик слова
	 */
	typedef class Metrics {
		private:
			// Основные параметры
			size_t ad, cw, dc, oc;
		public:
			/**
			 * calc Метод расчета метрик для слова
			 * @return результат расчета метрик
			 */
			const meta_t calc() noexcept {
				// Количество всех документов
				const size_t ad = (this->ad > 0 ? this->ad : 1);
				// Встречаемость в документах
				const size_t dc = (this->dc > 0 ? this->dc : 1);
				// Количество используемых слов
				const size_t cw = (this->cw > 0 ? this->cw : 1);
				// Встречаемость в текстах
				const size_t oc = (this->oc > 0 ? this->oc : 1);
				// Выполняем расчет tf
				const double tf = (oc / double(cw));
				// Выполняем расчет idf
				const double idf = log(ad / double(dc));
				// Считаем вес слова
				const double wltf = (double(1) + log(tf * double(dc)));
				// Формируем результат расчета
				return {oc, dc, tf, idf, wltf};
			}
			/**
			 * Metrics Конструктор
			 * @param ad количество используемых документов
			 * @param cw количество обработанных слов
			 * @param dc количество документов где встретилось слово
			 * @param oc количество встречь слова в текстах
			 */
			Metrics(size_t ad = 0, size_t cw = 0, size_t dc = 0, size_t oc = 0) noexcept : ad(ad), cw(cw), dc(dc), oc(oc) {}
	} __attribute__((packed)) metric_t;
	/**
	 * Word Класс слова в словаре
	 */
	typedef class Word : public wstring {
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
			void wordCase() noexcept {
				// Если строка не пустая и пробел не найден
				if(!this->empty()){
					// Очищаем список позиций
					this->uppers = 0;
					// Получаем длину слова
					const size_t length = this->length();
					/**
					 * setFn Функция установки буквы
					 * @param index индекс буквы в слове
					 */
					auto setFn = [this](const size_t index = 0) noexcept {
						// Получаем значение текущей буквы
						wchar_t * letter = &this->at(index);
						// Получаем букву в нижнем регистре
						wchar_t c = towlower(* letter);
						// Если буквы не равны
						if((* letter) != c){
							// Переводим букву в нижний регистр
							(* letter) = c;
							// Добавляем позицию буквы в список
							this->uppers += (1 << index);
						}
					};
					// Если длина слова всего один символ
					if(length == 1) setFn();
					// Выполняем перебор слова
					else {
						// Переходим по всем буквам слова
						for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
							// Устанавливаем первую букву
							setFn(i);
							// Устанавливаем вторую букву
							if(i != j) setFn(j);
						}
					}
				}
			}
		private:
			/**
			 * isUpCase Метод проверки на верхний регистр
			 * @param c буква для проверки
			 * @return  результат проверки
			 */
			const bool isUpCase(wchar_t c) noexcept {
				// Если буквы не равны значит это верхний регистр
				return (towlower(c) != wint_t(c));
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
				// Обнуляем метаданные
				this->oc = 0;
				this->dc = 0;
				// Очищаем список позиций
				this->uppers = 0;
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
				// Запоминаем метаданные
				const ocdc_t meta = this->getmeta();
				// Изменяем строку
				this->append(1, c);
				// Добавляем метаданные
				this->setmeta(meta);
			}
		public:
			/**
			 * setUppers Метод установки списка регистров в бинарном виде
			 * @param uppers список регистров в бинарном виде
			 */
			void setUppers(const size_t uppers) noexcept {
				// Устанавливаем список регистров
				this->uppers = uppers;
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
			const int compare(const Word & word) const noexcept {
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
			Word & append(const Word & word) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Получаем временную строку
				const wstring & wstr = word.wreal();
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  s символ для добавления
			 * @return   получившаяся строка
			 */
			Word & append(const char * s) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Получаем временную строку
				const wstring & wstr = this->wstr(s);
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  s символ для добавления
			 * @return   получившаяся строка
			 */
			Word & append(const wchar_t * s) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Получаем временную строку
				const wstring & wstr = wstring(s);
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  n количество символов
			 * @param  c символ для добавления
			 * @return   получившаяся строка
			 */
			Word & append(size_t n, char c) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(n, c));
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  n количество символов
			 * @param  c символ для добавления
			 * @return   получившаяся строка
			 */
			Word & append(size_t n, wchar_t c) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Получаем временную строку
				const wstring & wstr = wstring(n, c);
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param str строка для добавления
			 */
			Word & append(const string & str) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(str);
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param str строка для добавления
			 * @return    получившаяся строка
			 */
			Word & append(const wstring & str) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем длину слова
				const size_t length = str.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(str.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(str.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(str.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(str);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  s указатель на символы
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word & append(const char * s, size_t n) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(s, n));
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param  s указатель на символы
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word & append(const wchar_t * s, size_t n) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Получаем временную строку
				const wstring & wstr = wstring(s, n);
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
				// Выводим результат
				return (* this);
			}
			/**
			 * append Метод добавления строки
			 * @param str    строка для добавления
			 * @param subpos начальная позиция
			 * @param sublen длина строки
			 */
			Word & append(const string & str, size_t subpos, size_t sublen) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(str, subpos, sublen));
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
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
			Word & append(const wstring & str, size_t subpos, size_t sublen) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = wstring(str, subpos, sublen);
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
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
			Word & append(InputIterator first, InputIterator last) noexcept {
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = wstring(first, last);
				// Получаем длину слова
				const size_t length = wstr.length();
				// Если длина слова всего 1 символ
				if((length == 1) && this->isUpCase(wstr.front())){
					// Если регистр букв изменился, запоминаем это
					uppers += (1 << this->length());
				// Если слово длиннее 1-го символа
				} else {
					// Переходим по всем буквам слова
					for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
						// Если регистр букв изменился, запоминаем это
						if(this->isUpCase(wstr.at(i))) uppers += (1 << (this->length() + i));
						// Устанавливаем регистр для второй буквы
						if((i != j) && this->isUpCase(wstr.at(j))) uppers += (1 << (this->length() + j));
					}
				}
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->append(wstr);
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
				// Выводим результат
				return (* this);
			}
		public:
			/**
			 * assign Метод перезаполнения строки
			 * @param  s символ для перезаписи
			 * @return   получившаяся строка
			 */
			Word & assign(const char * s) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(s);
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  s символ для перезаписи
			 * @return   получившаяся строка
			 */
			Word & assign(const wchar_t * s) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(s);
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 */
			Word & assign(const Word & word) noexcept {
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(word.wstr());
				// Добавляем метаданные
				this->setmeta(word.getmeta());
				// Устанавливаем регистры символов
				this->setUppers(word.getUppers());
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  n количество символов для замены
			 * @param  c символ для замены
			 * @return   получившаяся строка
			 */
			Word & assign(size_t n, char c) noexcept {
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(n, c));
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  n количество символов для замены
			 * @param  c символ для замены
			 * @return   получившаяся строка
			 */
			Word & assign(size_t n, wchar_t c) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(n, c);
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 * @param      получившаяся строка
			 */
			Word & assign(const string & str) noexcept {
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(str);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 * @param      получившаяся строка
			 */
			Word & assign(const wstring & str) noexcept {
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(str);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  s указатель на символы в памяти
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word & assign(const char * s, size_t n) noexcept {
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(s, n));
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param  s указатель на символы в памяти
			 * @param  n количество символов
			 * @return   получившаяся строка
			 */
			Word & assign(const wchar_t * s, size_t n) noexcept {
				// Получаем временную строку
				const wstring & wstr = wstring(s, n);
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 * @param meta метаданные слова
			 */
			Word & assign(const string & word, const ocdc_t & meta) noexcept {
				// Получаем временную строку
				const wstring & wstr = this->wstr(word);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * assign Метод перезаполнения строки
			 * @param word строка для замены
			 * @param meta метаданные слова
			 */
			Word & assign(const wstring & word, const ocdc_t & meta) noexcept {
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(word);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & assign(const string & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(str, subpos, sublen));
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & assign(const wstring & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = wstring(str, subpos, sublen);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(wstr);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & assign(InputIterator first, InputIterator last) noexcept {
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(first, last);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & insert(size_t pos, const char * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем временную строку
				const wstring & wstr = this->wstr(s);
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, wstr.c_str());
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  s   строка для установки
			 * @return     получившаяся строка
			 */
			Word & insert(size_t pos, const wchar_t * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, s);
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  str строка для установки
			 * @return     получившаяся строка
			 */
			Word & insert(size_t pos, const Word & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, str.wreal());
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  str строка для установки
			 * @return     получившаяся строка
			 */
			Word & insert(size_t pos, const string & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, this->wstr(str));
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
			/**
			 * insert Метод вставки строки текста
			 * @param  pos позиция для установки
			 * @param  str строка для установки
			 * @return     получившаяся строка
			 */
			Word & insert(size_t pos, const wstring & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, str);
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & insert(size_t pos, size_t n, char c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, this->wstr(string(n, c)));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & insert(size_t pos, size_t n, wchar_t c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, wstring(n, c));
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & insert(size_t pos, const char * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, this->wstr(string(s, n)));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & insert(size_t pos, const wchar_t * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, wstring(s, n));
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & insert(size_t pos, const Word & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, str.wreal().substr(subpos, sublen));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & insert(size_t pos, const string & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, this->wstr(string(str, subpos, sublen)));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & insert(size_t pos, const wstring & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Вставляем строку в нужную позицию
				tmp.insert(pos, wstring(str, subpos, sublen));
				// Добавляем к строке переданную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(1, c));
				// Добавляем к строке переданную строку
				result = reinterpret_cast <wstring *> (this)->insert(p, wstr.front());
				// Если регистр букв изменился, запоминаем это
				if(this->isUpCase(wstr.front())) uppers += (1 << distance(this->begin(), p));
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
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
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Получаем временную строку
				const wstring & wstr = wstring(1, c);
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Добавляем к строке переданную строку
				result = reinterpret_cast <wstring *> (this)->insert(p, wstr.front());
				// Если регистр букв изменился, запоминаем это
				if(this->isUpCase(wstr.front())) uppers += (1 << distance(this->begin(), p));
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
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
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(n, c));
				// Добавляем к строке переданную строку
				result = reinterpret_cast <wstring *> (this)->insert(p, wstr);
				// Если регистр букв изменился, запоминаем это
				if(this->isUpCase(wstr.front())) uppers += (1 << distance(this->begin(), p));
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
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
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Получаем временную строку
				const wstring & wstr = wstring(n, c);
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Добавляем к строке переданную строку
				result = reinterpret_cast <wstring *> (this)->insert(p, wstr);
				// Если регистр букв изменился, запоминаем это
				if(this->isUpCase(wstr.front())) uppers += (1 << distance(this->begin(), p));
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
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
				// Запоминаем текущий регистр букв
				size_t uppers = this->uppers;
				// Запоминаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = wstring(first, last);
				// Добавляем к строке переданную строку
				result = reinterpret_cast <wstring *> (this)->insert(p, wstr);
				// Если регистр букв изменился, запоминаем это
				if(this->isUpCase(wstr.front())) uppers += (1 << distance(this->begin(), p));
				// Добавляем метаданные
				this->setmeta(meta);
				// Восстанавливаем регистр букв
				this->uppers = uppers;
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
			Word & erase(size_t pos = 0, size_t len = npos) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Удаляем указанные символы
				tmp.erase(pos, len);
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем индекс позиции
				const size_t index = distance(tmp.begin(), p);
				// Удаляем указанные символы
				tmp.erase(p);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем индекс позиции
				const size_t index = distance(tmp.begin(), first);
				// Удаляем указанные символы
				tmp.erase(first, last);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, const char * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(s));
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, const wchar_t * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Выполняем замену в слове
				tmp.replace(pos, len, wstring(s));
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, const Word & word) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Выполняем замену в слове
				tmp.replace(pos, len, word.wreal());
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, size_t n, char c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(string(n, c));
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, size_t n, wchar_t c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Выполняем замену в слове
				tmp.replace(pos, len, wstring(n, c));
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, const string & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем временную строку
				const wstring & wstr = this->wstr(str);
				// Выполняем замену в слове
				tmp.replace(pos, len, wstr);
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, const wstring & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Выполняем замену в слове
				tmp.replace(pos, len, str);
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, const char * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем замену в слове
				tmp.replace(pos, len, this->wstr(string(s, n)));
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, const wchar_t * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Выполняем замену в слове
				tmp.replace(pos, len, wstring(s, n));
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, const string & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем замену в слове
				tmp.replace(pos, len, this->wstr(string(str, subpos, sublen)));
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(size_t pos, size_t len, const wstring & str, size_t subpos, size_t sublen) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем замену в слове
				tmp.replace(pos, len, wstring(str, subpos, sublen));
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(const_iterator i1, const_iterator i2, const char * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем замену в слове
				tmp.replace(i1, i2, this->wstr(string(s)).c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(const_iterator i1, const_iterator i2, const wchar_t * s) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Выполняем замену в слове
				tmp.replace(i1, i2, s);
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(const_iterator i1, const_iterator i2, size_t n, char c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем замену в слове
				tmp.replace(i1, i2, this->wstr(string(n, c)).c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(const_iterator i1, const_iterator i2, size_t n, wchar_t c) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем замену в слове
				tmp.replace(i1, i2, wstring(n, c).c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(const_iterator i1, const_iterator i2, const Word & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем замену в слове
				tmp.replace(i1, i2, str.wreal().c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(const_iterator i1, const_iterator i2, const string & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем замену в слове
				tmp.replace(i1, i2, this->wstr(str).c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(const_iterator i1, const_iterator i2, const wstring & str) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Выполняем замену в слове
				tmp.replace(i1, i2, str.c_str());
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(const_iterator i1, const_iterator i2, const char * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем замену в слове
				tmp.replace(i1, i2, this->wstr(string(s, n)).c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(const_iterator i1, const_iterator i2, const wchar_t * s, size_t n) noexcept {
				// Получаем текущее слово
				wstring tmp = this->wreal();
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем замену в слове
				tmp.replace(i1, i2, wstring(s, n).c_str());
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
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
			Word & replace(const_iterator i1, const_iterator i2, InputIterator first, InputIterator last) noexcept {
				// Получаем временную строку
				wstring tmp = this->wreal();
				// Выполняем замену в строке
				tmp.replace(i1, i2, first, last);
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Устанавливаем полученную строку
				reinterpret_cast <wstring *> (this)->assign(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
				// Выводим результат
				return (* this);
			}
		public:
			/**
			 * swap Метод обмена данными с другим текстом
			 * @param str строка для обмена данных
			 */
			void swap(Word & str) noexcept {
				// Получаем строку для обмена
				wstring tmp = str.wreal();
				// Выполняем обмен данными
				reinterpret_cast <wstring *> (this)->swap(tmp);
				// Добавляем метаданные
				this->setmeta(str.getmeta());
				// Устанавливаем регистры слова
				this->setUppers(str.getUppers());
			}
			/**
			 * swap Метод обмена данными с другим текстом
			 * @param str строка для обмена данных
			 */
			void swap(string & str) noexcept {
				// Получаем строку для обмена
				wstring tmp = this->wstr(str);
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем обмен данными
				reinterpret_cast <wstring *> (this)->swap(tmp);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
			}
			/**
			 * swap Метод обмена данными с другим текстом
			 * @param str строка для обмена данных
			 */
			void swap(wstring & str) noexcept {
				// Получаем текущие метаданные
				const ocdc_t & meta = this->getmeta();
				// Выполняем обмен данными
				reinterpret_cast <wstring *> (this)->swap(str);
				// Устанавливаем регистры слова
				this->wordCase();
				// Добавляем метаданные
				this->setmeta(meta);
			}
		public:
			/**
			 * set Метод загрузки дампа
			 * @param dump дамп для загрузки данных
			 */
			void set(const vector <char> & dump) noexcept {
				// Если данные переданы
				if(!dump.empty()){
					// Метаданные
					ocdc_t meta;
					// Смещеение в буфере
					size_t offset = 0;
					// Определяем размер метаданных
					const size_t size = sizeof(meta);
					// Получаем данные дампа
					const char * data = dump.data();
					// Запоминаем метаданные
					memcpy(&meta, data + offset, size);
					// Запоминаем смещение
					offset += size;
					// Если частота встречаемости не нулевая
					if((meta.oc > 0) && (meta.dc > 0)){
						// Список регистров букв в верхнем регистре
						size_t uppers = 0;
						// Получаем список регистров букв в верхнем регистре
						memcpy(&uppers, data + offset, sizeof(uppers));
						// Запоминаем смещение
						offset += sizeof(uppers);
						// Получаем данные слова
						string word(data + offset, dump.size() - offset);
						// Если слово не пустое
						if(!word.empty()){
							// Устанавливаем полученную строку
							reinterpret_cast <wstring *> (this)->assign(this->wstr(word));
							// Добавляем метаданные
							this->setmeta(meta);
							// Устанавливаем список букв в верхнем регистре
							this->uppers = uppers;
						}
					}
				}
			}
			/**
			 * set Метод загрузки дампа
			 * @param dump дамп для загрузки данных
			 * @param n    количество байт
			 */
			void set(const char * dump, size_t n) noexcept {
				// Определяем размер метаданных
				const size_t size = sizeof(ocdc_t);
				// Если данные переданы
				if((dump != nullptr) && (n > size)){
					// Метаданные
					ocdc_t meta;
					// Смещеение в буфере
					size_t offset = 0;
					// Запоминаем метаданные
					memcpy(&meta, dump + offset, size);
					// Запоминаем смещение
					offset += size;
					// Если частота встречаемости не нулевая
					if((meta.oc > 0) && (meta.dc > 0)){
						// Список регистров букв в верхнем регистре
						size_t uppers = 0;
						// Получаем список регистров букв в верхнем регистре
						memcpy(&uppers, dump + offset, sizeof(uppers));
						// Запоминаем смещение
						offset += sizeof(uppers);
						// Получаем данные слова
						string word(dump + offset, n - offset);
						// Если слово не пустое
						if(!word.empty()){
							// Устанавливаем полученную строку
							reinterpret_cast <wstring *> (this)->assign(this->wstr(word));
							// Добавляем метаданные
							this->setmeta(meta);
							// Устанавливаем список букв в верхнем регистре
							this->uppers = uppers;
						}
					}
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
					// Получаем метаданные слова
					const ocdc_t & meta = this->getmeta();
					// Получаем бинарные метаданные
					const char * mbin = reinterpret_cast <const char *> (&meta);
					// Получаем бинарные данные количества регистров
					const char * ubin = reinterpret_cast <const char *> (&this->uppers);
					// Добавляем в буфер метаданные
					result.insert(result.end(), mbin, mbin + sizeof(meta));
					// Добавляем в буфер количество регистров
					result.insert(result.end(), ubin, ubin + sizeof(this->uppers));
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
			/**
			 * getUppers Метод получения список регистров
			 * @return список регистров в бинарном виде
			 */
			size_t getUppers() const noexcept {
				// Выводим список регистров в бинарном виде
				return this->uppers;
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
			size_t find(const Word & str, size_t pos = 0) const noexcept {
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
			size_t rfind(const Word & str, size_t pos = npos) const noexcept {
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
			Word & trim() noexcept {
				// Очищаем полученную строку
				wstring * str = reinterpret_cast <wstring *> (this);
				// Выполняем удаление пробелов по краям
				str->erase(str->begin(), find_if_not(str->begin(), str->end(), [](wchar_t c){ return iswspace(c); }));
				str->erase(find_if_not(str->rbegin(), str->rend(), [](wchar_t c){ return iswspace(c); }).base(), str->end());
				// Выводим результат
				return (* this);
			}
			/**
			 * substr Метод обрезки сроки
			 * @param  pos позиция для начала обрезки
			 * @param  len количество символов для обрезки
			 * @return     получившаяся строка
			 */
			Word substr(size_t pos = 0, size_t len = npos) const noexcept {
				// Получаем строку из текущего слова
				wstring word = this->wreal();
				// Выводим результат
				return Word(word.substr(pos, len), this->getmeta());
			}
		public:
			/**
			 * data Данные строки
			 * @return указатель на бинарные данные
			 */
			const char * data() const noexcept {
				// Получаем текущую строку
				this->current = this->real();
				// Выводим результат
				return this->current.data();
			}
			/**
			 * c_str Метод получения указателя на строку
			 * @return указатель на строку
			 */
			const char * c_str() const noexcept {
				// Получаем текущую строку
				this->current = this->real();
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
				const string & word = this->real();
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
				const wstring & word = this->wreal();
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
			/**
			 * real Метод извлечения слова с учётом регистра
			 * @return слово с учётом регистра
			 */
			const string real() const noexcept {
				// Если список регистров пуст, выводим как есть
				if(this->uppers == 0) return this->str();
				// Если список не пустой
				else {
					// Буква в слове
					wchar_t * letter = nullptr;
					// Получаем строку из текущего слова
					wstring word = this->wstr();
					// Получаем длину слова
					const size_t length = word.length();
					// Если длина слова всего 1 символ
					if((length == 1) && (this->uppers == 1)){
						// Получаем букву в слове
						letter = &word.at(0);
						// Переводим букву в верхний регистр
						(* letter) = towupper(* letter);
					// Если длина слова длиннее
					} else {
						// Переходим по всем буквам слова
						for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
							// Проверяем существует ли позиция в списке регистров
							if((i <= 31) && ((1 << i) & this->uppers)){
								// Получаем букву в слове
								letter = &word.at(i);
								// Переводим букву в верхний регистр
								(* letter) = towupper(* letter);
							// Выходим если предел превышен
							} else if(i > 31) break;
							// Если и у второй буквы нужно сменить регистр
							if((i != j) && (j <= 31) && ((1 << j) & this->uppers)){
								// Получаем букву в слове
								letter = &word.at(j);
								// Переводим букву в верхний регистр
								(* letter) = towupper(* letter);
							}
						}
					}
					// Выводим результат
					return this->str(word);
				}
			}
			/**
			 * wreal Метод извлечения слова с учётом регистра
			 * @return слово с учётом регистра
			 */
			const wstring wreal() const noexcept {
				// Если список регистров пуст, выводим как есть
				if(this->uppers == 0) return this->wstr();
				// Если список не пустой
				else {
					// Буква в слове
					wchar_t * letter = nullptr;
					// Получаем строку из текущего слова
					wstring word = this->wstr();
					// Получаем длину слова
					const size_t length = word.length();
					// Если длина слова всего 1 символ
					if((length == 1) && (this->uppers == 1)){
						// Получаем букву в слове
						letter = &word.at(0);
						// Переводим букву в верхний регистр
						(* letter) = towupper(* letter);
					// Если длина слова длиннее
					} else {
						// Переходим по всем буквам слова
						for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
							// Проверяем существует ли позиция в списке регистров
							if((i <= 31) && ((1 << i) & this->uppers)){
								// Получаем букву в слове
								letter = &word.at(i);
								// Переводим букву в верхний регистр
								(* letter) = towupper(* letter);
							// Выходим если предел превышен
							} else if(i > 31) break;
							// Если и у второй буквы нужно сменить регистр
							if((i != j) && (j <= 31) && ((1 << j) & this->uppers)){
								// Получаем букву в слове
								letter = &word.at(j);
								// Переводим букву в верхний регистр
								(* letter) = towupper(* letter);
							}
						}
					}
					// Выводим результат
					return word;
				}
			}
		public:
			/**
			 * getmeta Метод получения метаданных
			 * @return метаданные
			 */
			const ocdc_t getmeta() const noexcept {
				// Выводим метаданные слова
				return ocdc_t(this->oc, this->dc);
			}
			/**
			 * calc Метод подсчёта параметров слова
			 * @param ad количество используемых документов
			 * @param cw количество обработанных слов
			 * @return   подсчитанные параметры слова
			 */
			const meta_t calc(const size_t ad, const size_t cw) const noexcept {
				// Создаём объект метрики
				metric_t metric(ad, cw, this->dc, this->oc);
				// Выводим параметры слова
				return metric.calc();
			}
		public:
			/**
			 * setmeta Метод установки метаданных
			 * @param meta метаданные слова
			 */
			void setmeta(const ocdc_t & meta) noexcept {
				// Запоминаем метаданные
				this->oc = meta.oc;
				this->dc = meta.dc;
			}
			/**
			 * setmeta Метод установки метаданных
			 * @param dc количество документов где встретилось слово
			 * @param oc количество встречь слова в текстах
			 */
			void setmeta(const size_t dc, const size_t oc) noexcept {
				// Запоминаем метаданные
				this->oc = oc;
				this->dc = dc;
			}
		public:
			/**
			 * Оператор объединения
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			Word operator + (char c) noexcept {
				// Получаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем строку слова
				string word = this->real();
				// Добавляем к слову символ
				word.append(1, c);
				// Выводим результат
				return Word(word, meta);
			}
			/**
			 * Оператор объединения
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			Word operator + (wchar_t c) noexcept {
				// Получаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем строку слова
				wstring word = this->wreal();
				// Добавляем к слову символ
				word.append(1, c);
				// Выводим результат
				return Word(word, meta);
			}
			/**
			 * Оператор объединения
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			Word operator + (const char * s) noexcept {
				// Получаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем строку слова
				string word = this->real();
				// Добавляем к слову символ
				word.append(s);
				// Выводим результат
				return Word(word, meta);
			}
			/**
			 * Оператор объединения
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			Word operator + (const wchar_t * s) noexcept {
				// Получаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем строку слова
				wstring word = this->wreal();
				// Добавляем к слову символ
				word.append(s);
				// Выводим результат
				return Word(word, meta);
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word operator + (const Word & word) noexcept {
				// Получаем строку слова
				Word result = (* this);
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
			Word operator + (const string & word) noexcept {
				// Получаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем строку слова
				string str = this->real();
				// Добавляем к слову символ
				str.append(word);
				// Выводим результат
				return Word(str, meta);
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word operator + (const wstring & word) noexcept {
				// Получаем метаданные
				const ocdc_t & meta = this->getmeta();
				// Получаем строку слова
				wstring str = this->wreal();
				// Добавляем к слову символ
				str.append(word);
				// Выводим результат
				return Word(str, meta);
			}
		public:
			/**
			 * Оператор присваивания
			 * @param c символ для присваивания
			 * @return  получившееся слово
			 */
			template <typename Value>
			Word & operator = (Value c) noexcept {
				// Присваиваем новое значение
				return this->assign(1, c);
			}
			/**
			 * Оператор присваивания
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			template <typename Value>
			Word & operator = (const Value * s) noexcept {
				// Присваиваем новое значение
				return this->assign(s);
			}
			/**
			 * Оператор присваивания
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator = (const Word & word) noexcept {
				// Присваиваем новое значение
				return this->assign(word);
			}
			/**
			 * Оператор присваивания
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator = (const string & word) noexcept {
				// Присваиваем новое значение
				return this->assign(word);
			}
			/**
			 * Оператор присваивания
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator = (const wstring & word) noexcept {
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
			Word & operator += (Value c) noexcept {
				// Добавляем новое значение
				return this->append(1, c);
			}
			/**
			 * Оператор сложения и присваивания
			 * @param s указатель на символы в памяти
			 * @return  получившееся слово
			 */
			template <typename Value>
			Word & operator += (const Value * s) noexcept {
				// Добавляем новое значение
				return this->append(s);
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator += (const Word & word) noexcept {
				// Добавляем новое значение
				return this->append(word);
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator += (const string & word) noexcept {
				// Добавляем новое значение
				return this->append(word);
			}
			/**
			 * Оператор объединения
			 * @param word объект слова
			 * @return     получившееся слово
			 */
			Word & operator += (const wstring & word) noexcept {
				// Добавляем новое значение
				return this->append(word);
			}
		public:
			/**
			 * Оператор сравнения слов
			 * @param word объект слова
			 * @return     результат сравнения
			 */
			bool operator < (const Word & word) noexcept {
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
			bool operator > (const Word & word) noexcept {
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
			bool operator <= (const Word & word) noexcept {
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
			bool operator >= (const Word & word) noexcept {
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
			bool operator == (const Word & word) noexcept {
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
			bool operator != (const Word & word) noexcept {
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
			 * Word Конструктор
			 */
			Word() noexcept {
				// Добавляем метаданные
				this->setmeta({1, 1});
			}
			/**
			 * Word Конструктор
			 * @param c символ для вставки
			 */
			template <typename Value>
			Word(Value c) noexcept {
				// Изменяем строку
				this->assign(1, c);
				// Добавляем метаданные
				this->setmeta({1, 1});
			}
			/**
			 * Word Конструктор
			 * @param s указатель на символы в памяти
			 */
			template <typename Value>
			Word(const Value * s) noexcept {
				// Изменяем строку
				this->assign(s);
				// Добавляем метаданные
				this->setmeta({1, 1});
			}
			/**
			 * Word Конструктор
			 * @param n количество символов для вставки
			 * @param c символ для вставки
			 */
			template <typename Value>
			Word(size_t n, Value c) noexcept {
				// Изменяем строку
				this->assign(n, c);
				// Добавляем метаданные
				this->setmeta({1, 1});
			}
			/**
			 * Word Конструктор
			 * @param str строка для установки
			 */
			Word(const string & str) noexcept {
				// Изменяем строку
				this->assign(str);
				// Добавляем метаданные
				this->setmeta({1, 1});
			}
			/**
			 * Word Конструктор
			 * @param str строка для установки
			 */
			Word(const wstring & str) noexcept {
				// Изменяем строку
				this->assign(str);
				// Добавляем метаданные
				this->setmeta({1, 1});
			}
			/**
			 * Word Конструктор
			 * @param s указатель на символы в памяти
			 * @param n количество символов для вставки
			 */
			template <typename Value>
			Word(const Value * s, size_t n) noexcept {
				// Изменяем строку
				this->assign(s, n);
				// Добавляем метаданные
				this->setmeta({1, 1});
			}
			/**
			 * Word Конструктор
			 * @param str строка для установки
			 * @param meta метаданные слова
			 */
			Word(const string & str, const ocdc_t & meta) noexcept {
				// Изменяем строку
				this->assign(str);
				// Добавляем метаданные
				this->setmeta(meta);
			}
			/**
			 * Word Конструктор
			 * @param str строка для установки
			 * @param meta метаданные слова
			 */
			Word(const wstring & str, const ocdc_t & meta) noexcept {
				// Изменяем строку
				this->assign(str);
				// Добавляем метаданные
				this->setmeta(meta);
			}
			/**
			 * Word Конструктор
			 * @param word слово для инициализации
			 * @param pos  позиция для вставки
			 * @param len  длина слова для вставки
			 */
			Word(const string & word, size_t pos, size_t len = npos) noexcept {
				// Изменяем строку
				this->assign(word, pos, len);
				// Добавляем метаданные
				this->setmeta({1, 1});
			}
			/**
			 * Word Конструктор
			 * @param word слово для инициализации
			 * @param pos  позиция для вставки
			 * @param len  длина слова для вставки
			 */
			Word(const wstring & word, size_t pos, size_t len = npos) noexcept {
				// Изменяем строку
				this->assign(word, pos, len);
				// Добавляем метаданные
				this->setmeta({1, 1});
			}
			/**
			 * Word Конструктор
			 * @param first начальный итератор
			 * @param last  конечный итератор
			 */
			template <class InputIterator>
			Word(InputIterator first, InputIterator last) noexcept {
				// Изменяем строку
				this->assign(first, last);
				// Добавляем метаданные
				this->setmeta({1, 1});
			}
			/**
			 * ~Word Деструктор
			 */
			~Word() noexcept {
				// Очищаем все данные
				this->clear();
			}
	} word_t;
};

#endif // _ANYKS_WORD_

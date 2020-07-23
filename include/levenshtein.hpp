/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_LEV__
#define __ANYKS_LEV__

/**
 * Стандартная библиотека
 */
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <iostream>
#include <stdlib.h>
#include <locale.h>
#include <sys/types.h>

// Реализуем функцию определения минимального значения
#define lvmin(begin, end) (((begin) < (end)) ? (begin) : (end))

// Устанавливаем область видимости
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Класс реализации дистанции Левенштейна
	 */
	typedef class LEV {
		public:
			/**
			 * Weighter структура для функций работы с весами
			 */
			typedef struct Weighter {
				double (* ins) (const wchar_t);                                             // Вставка
				double (* del) (const wchar_t);                                             // Удаление
				double (* rep) (const wchar_t, const wchar_t, const wchar_t, const wchar_t); // Замена
			} weighter_t;
		private:
			/**
			 * minimum Метод определения минимального значения
			 * @param  x первое число
			 * @param  y второе число
			 * @param  z третье число
			 * @return   самое минимальное значение из 3-х чисел
			 */
			const double minimum(const double x, const double y, const double z) const noexcept;
			/**
			 * minimum Метод определения минимального значения
			 * @param  x первое число
			 * @param  y второе число
			 * @param  z третье число
			 * @return   самое минимальное значение из 3-х чисел
			 */
			const u_short minimum(const u_short x, const u_short y, const u_short z) const noexcept;
		public:
			/**
			 * mulct Определение количества штрафов на основе Дамерау-Левенштейна
			 * @param  pattern шаблон с которым идет сравнение
			 * @param  text    исходный текст
			 * @return         дистанция
			 */
			const u_short mulct(const string & pattern, const string & text) const noexcept;
			/**
			 * mulct Определение количества штрафов на основе Дамерау-Левенштейна
			 * @param  pattern шаблон с которым идет сравнение
			 * @param  text    исходный текст
			 * @return         дистанция
			 */
			const u_short mulct(const wstring & pattern, const wstring & text) const noexcept;
		public:
			/**
			 * damerau Определение дистанции Дамерау-Левенштейна в фразах
			 * @param  pattern шаблон с которым идет сравнение
			 * @param  text    исходный текст
			 * @return         дистанция
			 */
			const u_short damerau(const string & pattern, const string & text) const noexcept;
			/**
			 * damerau Определение дистанции Дамерау-Левенштейна в фразах
			 * @param  pattern шаблон с которым идет сравнение
			 * @param  text    исходный текст
			 * @return         дистанция
			 */
			const u_short damerau(const wstring & pattern, const wstring & text) const noexcept;
		public:
			/**
			 * distance Определение дистанции в фразах
			 * @param  pattern шаблон с которым идет сравнение
			 * @param  text    исходный текст
			 * @return         дистанция
			 */
			const u_short distance(const string & pattern, const string & text) const noexcept;
			/**
			 * distance Определение дистанции в фразах
			 * @param  pattern шаблон с которым идет сравнение
			 * @param  text    исходный текст
			 * @return         дистанция
			 */
			const u_short distance(const wstring & pattern, const wstring & text) const noexcept;
		public:
			/**
			 * tanimoto Метод определения коэффициента Жаккара (частное — коэф. Танимото)
			 * @param  first  первое слово
			 * @param  second второе слово
			 * @param  stl    размер подстроки при сравнении двух слов (от 1 до минимального размера слова)
			 * @return        коэффициент Танимото
			 */
			const double tanimoto(const string & first, const string & second, const u_short stl = 2) const noexcept;
			/**
			 * tanimoto Метод определения коэффициента Жаккара (частное — коэф. Танимото)
			 * @param  first  первое слово
			 * @param  second второе слово
			 * @param  stl    размер подстроки при сравнении двух слов (от 1 до минимального размера слова)
			 * @return        коэффициент Танимото
			 */
			const double tanimoto(const wstring & first, const wstring & second, const u_short stl = 2) const noexcept;
		public:
			/**
			 * weighted Метод определения веса сравниваемого слова
			 * @param  first    первое слово
			 * @param  second   второе слово
			 * @param  weighter функции сравнения весов
			 * @return          вес сравниваемого слова
			 */
			const double weighted(const string & first, const string & second, const weighter_t * weighter = nullptr) const noexcept;
			/**
			 * weighted Метод определения веса сравниваемого слова
			 * @param  first    первое слово
			 * @param  second   второе слово
			 * @param  weighter функции сравнения весов
			 * @return          вес сравниваемого слова
			 */
			const double weighted(const wstring & first, const wstring & second, const weighter_t * weighter = nullptr) const noexcept;
		public:
			/**
			 * needlemanWunsch Метод натяжения слов
			 * @param first    первое слово
			 * @param second   второе слово
			 * @param match    вес соответствия
			 * @param mismatch вес несоответствия
			 * @param gap      размер разрыва
			 * @return         количество очков
			 */
			const int needlemanWunsch(const string & first, const string & second, const int match = 1, const int mismatch = -1, const int gap = -2) const noexcept;
			/**
			 * needlemanWunsch Метод натяжения слов
			 * @param first    первое слово
			 * @param second   второе слово
			 * @param match    вес соответствия
			 * @param mismatch вес несоответствия
			 * @param gap      размер разрыва
			 * @return         количество очков
			 */
			const int needlemanWunsch(const wstring & first, const wstring & second, const int match = 1, const int mismatch = -1, const int gap = -2) const noexcept;
	} lev_t;
};

#endif // __ANYKS_LEV__

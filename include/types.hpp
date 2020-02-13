/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_TYPES_
#define _ANYKS_TYPES_

/**
 * Стандартная библиотека
 */
#include <cmath>
#include <limits>
#include <sys/types.h>

using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	// Признаки n-грамм
	enum class sign_t : u_int {
		num = 2,     // признак арабского или римского числа <num>
		unk = 3,     // признак неизвестного слова <unk>
		url = 4,     // признак url адреса <url>
		abbr = 5,    // признак аббревиатуры <abbr>
		date = 6,    // признак даты <date>
		time = 7,    // признак времени <time>
		anum = 8,    // признак псевдо-числа <anum>
		math = 9,    // признак математической операции <math>
		specs = 10,   // признак спец-символа <specs>
		start = 1,   // признак начала предложения <s>
		aprox = 11,  // признак приблизительного числа <aprox>
		range = 12,  // признак диапазона чисел <range>
		score = 13,  // признак числового счёта <score>
		dimen = 14,  // признак габаритных размеров <dimen>
		fract = 15,  // признак числовой дроби <fract>
		punct = 16,  // признак знака пунктуации <punct>
		isolat = 17, // признак знака изоляции <isolat>
		finish = 18  // признак конеца предложения </s>
	};
	// Тип данных пары целых значений
	typedef pair <size_t, size_t> pair_t;
	// Получаем максимальное значение идентификатора
	constexpr size_t noID = numeric_limits <size_t>::max();
};

#endif // _ANYKS_TYPES_

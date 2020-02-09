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
	enum class sign_t : u_short {
		num = 2,    // признак арабского или римского числа <num>
		unk = 3,    // признак неизвестного слова <unk>
		abbr = 4,   // признак аббревиатуры <abr>
		date = 5,   // признак даты <date>
		time = 6,   // признак времени <time>
		anum = 7,   // признак псевдо-числа <anum>
		aprox = 8,  // признак приблизительного числа <aprox>
		start = 1,  // признак начала предложения <s>
		range = 9, // признак диапазона чисел <range>
		score = 10, // признак числового счёта <score>
		dimen = 11, // признак габаритных размеров <dimen>
		fract = 12, // признак числовой дроби <fract>
		punct = 13, // признак знака пунктуации <punct>
		finish = 14 // признак конеца предложения </s>
	};
	// Тип данных пары целых значений
	typedef pair <size_t, size_t> pair_t;
	// Получаем максимальное значение идентификатора
	constexpr size_t noID = numeric_limits <size_t>::max();
};

#endif // _ANYKS_TYPES_

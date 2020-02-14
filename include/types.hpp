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
	// Токены n-грамм
	enum class token_t : u_int {
		num = 2,     // токен арабского или римского числа <num>
		unk = 3,     // токен неизвестного слова <unk>
		url = 4,     // токен url адреса <url>
		abbr = 5,    // токен аббревиатуры <abbr>
		date = 6,    // токен даты <date>
		time = 7,    // токен времени <time>
		anum = 8,    // токен псевдо-числа <anum>
		math = 9,    // токен математической операции <math>
		specs = 10,  // токен спец-символа <specs>
		start = 1,   // токен начала предложения <s>
		aprox = 11,  // токен приблизительного числа <aprox>
		range = 12,  // токен диапазона чисел <range>
		score = 13,  // токен числового счёта <score>
		dimen = 14,  // токен габаритных размеров <dimen>
		fract = 15,  // токен числовой дроби <fract>
		punct = 16,  // токен знака пунктуации <punct>
		isolat = 17, // токен знака изоляции <isolat>
		finish = 18  // токен конеца предложения </s>
	};
	// Тип данных пары целых значений
	typedef pair <size_t, size_t> pair_t;
	// Получаем максимальное значение идентификатора
	constexpr size_t noID = numeric_limits <size_t>::max();
};

#endif // _ANYKS_TYPES_

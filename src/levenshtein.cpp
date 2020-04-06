/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <levenshtein.hpp>

/**
 * minimum Метод определения минимального значения
 * @param  x первое число
 * @param  y второе число
 * @param  z третье число
 * @return   самое минимальное значение из 3-х чисел
 */
const double anyks::LEV::minimum(const double x, const double y, const double z) const noexcept {
	// Определяем минимальное значение
	return lvmin(lvmin(x, y), z);
}
/**
 * minimum Метод определения минимального значения
 * @param  x первое число
 * @param  y второе число
 * @param  z третье число
 * @return   самое минимальное значение из 3-х чисел
 */
const u_short anyks::LEV::minimum(const u_short x, const u_short y, const u_short z) const noexcept {
	// Определяем минимальное значение
	return lvmin(lvmin(x, y), z);
}
/**
 * mulct Определение количества штрафов на основе Дамерау-Левенштейна
 * @param  pattern шаблон с которым идет сравнение
 * @param  text    исходный текст
 * @return         дистанция
 */
const u_short anyks::LEV::mulct(const string & pattern, const string & text) const noexcept {
	// Результат работы функции
	u_short result = 0;
	// Если шаблон для сравнения и исходный текст переданы
	if(!pattern.empty() && !text.empty()){
		// Объявляем конвертер
		wstring_convert <codecvt_utf8 <wchar_t>> conv;
		// Конвертируем строки
		const wstring & txt = conv.from_bytes(text);
		const wstring & ptr = conv.from_bytes(pattern);
		// Выводим результат
		result = this->mulct(ptr, txt);
	}
	// Выводим результат
	return result;
}
/**
 * mulct Определение количества штрафов на основе Дамерау-Левенштейна
 * @param  pattern шаблон с которым идет сравнение
 * @param  text    исходный текст
 * @return         дистанция
 */
const u_short anyks::LEV::mulct(const wstring & pattern, const wstring & text) const noexcept {
	// Результат работы функции
	u_short result = 0;
	// Если шаблон для сравнения и исходный текст переданы
	if(!pattern.empty() && !text.empty()){
		// Дефолтный взвешиватель весов
		weighter_t weighter;
		/**
		 * Функция для определения веса при вставке
		 * @param a символ вставки
		 * @return  вес символа
		 */
		weighter.ins = [](const wchar_t a) noexcept {return 2.0;};
		/**
		 * Функция для определения веса при удалении
		 * @param a символ удаления
		 * @return  вес символа
		 */
		weighter.del = [](const wchar_t a) noexcept {return 2.0;};
		/**
		 * Функция для определения веса при замене
		 * @param a заменяемый символ
		 * @param b символ замены
		 * @return  вес символа
		 */
		weighter.rep = [](const wchar_t a, const wchar_t b, const wchar_t c, const wchar_t d) noexcept {
			return ((a == d) && (b == c) ? 0.0 : 1.0);
		};
		// Выполняем сравнение
		result = u_short(this->weighted(pattern, text, &weighter));
	}
	// Выводим результат
	return result;
}
/**
 * damerau Определение дистанции Дамерау-Левенштейна в фразах
 * @param  pattern шаблон с которым идет сравнение
 * @param  text    исходный текст
 * @return         дистанция
 */
const u_short anyks::LEV::damerau(const string & pattern, const string & text) const noexcept {
	// Результат работы функции
	u_short result = 0;
	// Если шаблон для сравнения и исходный текст переданы
	if(!pattern.empty() && !text.empty()){
		// Объявляем конвертер
		wstring_convert <codecvt_utf8 <wchar_t>> conv;
		// Конвертируем строки
		const wstring & txt = conv.from_bytes(text);
		const wstring & ptr = conv.from_bytes(pattern);
		// Выводим результат
		result = this->damerau(ptr, txt);
	}
	// Выводим результат
	return result;
}
/**
 * damerau Определение дистанции Дамерау-Левенштейна в фразах
 * @param  pattern шаблон с которым идет сравнение
 * @param  text    исходный текст
 * @return         дистанция
 */
const u_short anyks::LEV::damerau(const wstring & pattern, const wstring & text) const noexcept {
	// Результат работы функции
	u_short result = 0;
	// Если шаблон для сравнения и исходный текст переданы
	if(!pattern.empty() && !text.empty()){
		// Дефолтный взвешиватель весов
		weighter_t weighter;
		/**
		 * Функция для определения веса при вставке
		 * @param a символ вставки
		 * @return  вес символа
		 */
		weighter.ins = [](const wchar_t a) noexcept {return 1.0;};
		/**
		 * Функция для определения веса при удалении
		 * @param a символ удаления
		 * @return  вес символа
		 */
		weighter.del = [](const wchar_t a) noexcept {return 1.0;};
		/**
		 * Функция для определения веса при замене
		 * @param a заменяемый символ
		 * @param b символ замены
		 * @return  вес символа
		 */
		weighter.rep = [](const wchar_t a, const wchar_t b, const wchar_t c, const wchar_t d) noexcept {
			return 1.0;
		};
		// Выполняем сравнение
		result = u_short(this->weighted(pattern, text, &weighter));
	}
	// Выводим результат
	return result;
}
/**
 * distance Определение дистанции в фразах
 * @param  pattern шаблон с которым идет сравнение
 * @param  text    исходный текст
 * @return         дистанция
 */
const u_short anyks::LEV::distance(const string & pattern, const string & text) const noexcept {
	// Результат работы функции
	u_short result = 0;
	// Если шаблон для сравнения и исходный текст переданы
	if(!pattern.empty() && !text.empty()){
		// Объявляем конвертер
		wstring_convert <codecvt_utf8 <wchar_t>> conv;
		// Конвертируем строки
		const wstring & txt = conv.from_bytes(text);
		const wstring & ptr = conv.from_bytes(pattern);
		// Выводим результат
		result = this->distance(ptr, txt);
	}
	// Выводим результат
	return result;
}
/**
 * distance Определение дистанции в фразах
 * @param  pattern шаблон с которым идет сравнение
 * @param  text    исходный текст
 * @return         дистанция
 */
const u_short anyks::LEV::distance(const wstring & pattern, const wstring & text) const noexcept {
	// Результат работы функции
	u_short result = 0;
	// Если шаблон для сравнения и исходный текст переданы
	if(!pattern.empty() && !text.empty()){
		// Конвертируем строки
		const wstring txt = text;
		const wstring ptr = pattern;
		// Определяем размер исходного текста
		const u_short txtlen = txt.length();
		// Определяем размер шаблона
		const u_short ptrlen = ptr.length();
		// Если исходный текст не передан, то разница в длину шаблона
		if(txtlen == 0) return ptrlen;
		// Если шаблон не передан, то разница в длину исходного текста
		else if(ptrlen == 0) return txtlen;
		// Выполняем создание матрицы
		vector <vector <u_short>> table(ptrlen + 1, vector <u_short> (txtlen + 1));
		// Заполняем столбцы в нулевой строке таблицы
		for(u_short i = 0; i <= txtlen; ++i) table[0][i] = i;
		// Заполняем строки в таблице
		for(u_short i = 0; i <= ptrlen; ++i) table[i][0] = i;
		// Переменные для измерения дистанции Левенштейна
		u_short aboveCell, leftCell, diagonalCell, cost;
		// Переходим по всем строкам начиная с 1-й строки
		for(u_short i = 1; i <= ptrlen; ++i){
			// Переходим по всем столбцам начиная с 1-го столбца
			for(u_short j = 1; j <= txtlen; ++j){
				// Если текущие два символа обеих строк одинаковы, то значение стоимости будет равна 0, иначе будет 1
				cost = ((ptr[i - 1] == txt[j - 1]) ? 0 : 1);
				// Находим указанную ячейку в текущей ячейке
				aboveCell = table[i - 1][j];
				// Находим левую ячейку в текущей ячейке
				leftCell = table[i][j - 1];
				// Находим диагональную ячейку в текущей ячейке
				diagonalCell = table[i - 1][j - 1];
				// Вычисляем текущее значение (редактируемое расстояние) и результат в текущей ячейки матрицы
				table[i][j] = this->minimum(u_short(aboveCell + 1), u_short(leftCell + 1), u_short(diagonalCell + cost));
			}
		}
		// Определяем результат (что и есть расстояние Левенштейна)
		result = table[ptrlen][txtlen];
	}
	// Выводим результат
	return result;
}
/**
 * tanimoto Метод определения коэффициента Жаккара (частное — коэф. Танимото)
 * @param  first  первое слово
 * @param  second второе слово
 * @param  stl    размер подстроки при сравнении двух слов (от 1 до минимального размера слова)
 * @return        коэффициент Танимото
 */
const double anyks::LEV::tanimoto(const string & first, const string & second, const u_short stl) const noexcept {
	// Результат работы функции
	double result = 0.0;
	// Если первое и второе слово переданы
	if(!first.empty() && !second.empty()){
		// Объявляем конвертер
		wstring_convert <codecvt_utf8 <wchar_t>> conv;
		// Конвертируем строки
		const wstring & text1 = conv.from_bytes(first);
		const wstring & text2 = conv.from_bytes(second);
		// Выводим расчет
		result = this->tanimoto(text1, text2, stl);
	}
	// Выводим результат
	return result;
}
/**
 * tanimoto Метод определения коэффициента Жаккара (частное — коэф. Танимото)
 * @param  first  первое слово
 * @param  second второе слово
 * @param  stl    размер подстроки при сравнении двух слов (от 1 до минимального размера слова)
 * @return        коэффициент Танимото
 */
const double anyks::LEV::tanimoto(const wstring & first, const wstring & second, const u_short stl) const noexcept {
	// Результат работы функции
	double result = 0.0;
	// Если первое и второе слово переданы
	if((first.length() > 1) && (second.length() > 1)){
		// Количество эквивалентных подстрок
		double esubCount = 0.0;
		// Подстроки для расчёта схожести
		wstring subFirst = L"", subSecond = L"";
		// Получаем размер подстроки
		const size_t subToken = ((stl > 2) && (stl < second.length()) ? stl : 2);
		// Получаем размер первой подстроки
		const size_t firstToken = ((first.length() - subToken) + 1);
		// Получаем размер второй подстроки
		const size_t secondToken = ((second.length() - subToken) + 1);
		// Выделяем динамически память
		vector <bool> tokens(secondToken);
		// Переходим по всем буквам первого слова
		for(size_t i = 0; i < firstToken; ++i){
			// Получаем подстроку первого слова
			subFirst = first.substr(i, subToken);
			// Переходим по всем буквам второго слова
			for(size_t j = 0; j < secondToken; ++j){
				// Если подстроки не совпадают
				if(!tokens[j]){
					// Получаем подстроку второго слова
					subSecond = second.substr(j, subToken);
					// Если подстроки совпадают
					if(subFirst.compare(subSecond) == 0){
						// Запоминаем что подстроки совпадают
						tokens[j] = true;
						// Увеличиваем количество эквивалентных подстрок
						esubCount += 1.0;
						// Выходим из цикла
						break;
					}
				}
			}
		}
		// Выполняем расчет коэффициента Танимото
		result = ((1.0 * esubCount) / ((double(firstToken) + double(secondToken)) - esubCount));
	}
	// Выводим результат
	return result;
}
/**
 * weighted Метод определения веса сравниваемого слова
 * @param  first    первое слово
 * @param  second   второе слово
 * @param  weighter функции сравнения весов
 * @return          вес сравниваемого слова
 */
const double anyks::LEV::weighted(const string & first, const string & second, const weighter_t * weighter) const noexcept {
	// Объявляем конвертер
	wstring_convert <codecvt_utf8 <wchar_t>> conv;
	// Конвертируем строки
	const wstring & ftxt = conv.from_bytes(first);
	const wstring & stxt = conv.from_bytes(second);
	// Выводим результат
	return this->weighted(ftxt, stxt, weighter);
}
/**
 * weighted Метод определения веса сравниваемого слова
 * @param  first    первое слово
 * @param  second   второе слово
 * @param  weighter функции сравнения весов
 * @return          вес сравниваемого слова
 */
const double anyks::LEV::weighted(const wstring & first, const wstring & second, const weighter_t * weighter) const noexcept {
	/**
	 * calc Функция расчета весов
	 * @param  first    первое слово
	 * @param  second   второе слово
	 * @param  weighter функции сравнения весов
	 * @return          вес сравниваемого слова
	 */
	auto calc = [this](const wstring & first, const wstring & second, const weighter_t * weighter = nullptr) noexcept {
		// Результат работы функции
		double result = 0.0;
		// Если данные переданы
		if(weighter != nullptr){
			// Буквы для сравнения
			wchar_t a, b, c, d;
			// Вспомогательные переменные
			double ic = 0.0, dc = 0.0, rc = 0.0, last = 0.0, old = 0.0;
			// Получаем размеры входящих слов
			size_t flen = first.length(), slen = second.length();
			// Если одно из слов не содержит символов
			if(first.empty() || second.empty()){
				// Подсчитываем вес для первого слова
				while(flen) result += weighter->del(first[--flen]);
				// Подсчитываем вес для второго слова
				while(slen) result += weighter->ins(second[--slen]);
				// Выводим результат
				return result;
			}
			// Выделяем память для массива
			vector <double> column((slen + 1) * sizeof(double));
			// Зануляем первый элемент
			column.at(0) = 0.0;
			// Заполняем символы в таблице для второго слова
			for(size_t j = 1; j <= slen; ++j) column.at(j) = (column[j - 1] + weighter->ins(second[j - 1]));
			// Выполняем обход первого слова
			for(size_t i = 1; i <= flen; ++i){
				// Запоминаем последний элемент
				last = column[0];
				// Прибавляем вес за удаленные символы
				column.at(0) += weighter->del(first[i - 1]);
				// Переходим по второму слову
				for(size_t j = 1; j <= slen; ++j){
					// Запоминаем старый символ второго слова
					old = column[j];
					// Если символы равны
					if(first[i - 1] == second[j - 1]) column.at(j) = last;
					// Иначе выполняем подсчет весов
					else {
						// Получаем буквы
						a = first[i - 1];
						b = second[j - 1];
						c = first[i];
						d = second[j];
						// Определяем вес для вставленного символа
						ic = (column[j - 1] + weighter->ins(b));
						// Определяем вес для удаленного символа
						dc = (column[j] + weighter->del(a));
						// Определяем вес для замененных символов
						rc = (last + weighter->rep(a, b, c, d));
						// Определяем самый минимальный вес
						column.at(j) = this->minimum(dc, rc, ic);
					}
					// Запоминаем последний символ
					last = old;
				}
			}
			// Запоминаем результат
			result = column[slen];
		}
		// Выводим результат
		return result;
	};
	// Дефолтный взвешиватель весов
	weighter_t defWeighter;
	/**
	 * Функция для определения веса при вставке
	 * @param a символ вставки
	 * @return  вес символа
	 */
	defWeighter.ins = [](const wchar_t a) noexcept {return double(0.6);};
	/**
	 * Функция для определения веса при удалении
	 * @param a символ удаления
	 * @return  вес символа
	 */
	defWeighter.del = [](const wchar_t a) noexcept {return double(0.55);};
	/**
	 * Функция для определения веса при замене
	 * @param a заменяемый символ
	 * @param b символ замены
	 * @return  вес символа
	 */
	defWeighter.rep = [](const wchar_t a, const wchar_t b, const wchar_t c, const wchar_t d) noexcept {
		return double((a == d) && (b == c) ? -0.2 : 0.5);
	};
	// Выполняем расчет
	return calc(first, second, (weighter != nullptr ? weighter : &defWeighter));
}
/**
 * needlemanWunsch Метод натяжения слов
 * @param first    первое слово
 * @param second   второе слово
 * @param match    вес соответствия
 * @param mismatch вес несоответствия
 * @param gap      размер разрыва
 * @return         количество очков
 */
const int anyks::LEV::needlemanWunsch(const string & first, const string & second, const int match, const int mismatch, const int gap) const noexcept {
	// Результат работы функции
	int result = -1000000;
	// Если слова переданы
	if(!first.empty() && !second.empty()){
		// Объявляем конвертер
		wstring_convert <codecvt_utf8 <wchar_t>> conv;
		// Конвертируем строки
		const wstring & txt1 = conv.from_bytes(first);
		const wstring & txt2 = conv.from_bytes(second);
		// Выводим результат
		result = this->needlemanWunsch(txt1, txt2, match, mismatch, gap);
	}
	// Выводим результат
	return result;
}
/**
 * needlemanWunsch Метод натяжения слов
 * @param first    первое слово
 * @param second   второе слово
 * @param match    вес соответствия
 * @param mismatch вес несоответствия
 * @param gap      размер разрыва
 * @return         количество очков
 */
const int anyks::LEV::needlemanWunsch(const wstring & first, const wstring & second, const int match, const int mismatch, const int gap) const noexcept {
	// Результат работы функции
	int result = -1000000;
	// Если слова переданы
	if(!first.empty() && !second.empty()){
		// Получаем размеры строк
		const size_t flen = first.length();
		const size_t slen = second.length();
		// Текущий список
		vector <int> current(slen + 1);
		// Петлевой список
		vector <int> lookback(slen + 1);
		// Заполняем текущий список
		for(size_t j = 0; j < (slen + 1); j++){
			// Заполняем список
			current[j] = (j * gap);
		}
		// Выполняем расчет матрицы
		for(size_t i = 1; i < (flen + 1); i++){
			// Выполняем заполнение петлевого списка
			for(size_t j = 0; j < (slen + 1); j++){
				// Заполняем петлевой список
				lookback[j] = current[j];
			}
			// Заменяем нулевой элемент текущего списка
			current[0] = (i * gap);
			// Выполняем расчет очков
			for(size_t j = 1; j < (slen + 1); j++){
				// Выполняем сравнение символов
				if(first[i - 1] == second[j - 1]) current[j] = (lookback[j - 1] + match);
				// Если символы не соответствуют
				else current[j] = max(lookback[j - 1] + mismatch, max(lookback[j] + gap, current[j - 1] + gap));
			}
		}
		// Запоминаем подсчитанные очки
		result = current[slen];
	}
	// Выводим результат
	return result;
}

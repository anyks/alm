/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <idw.hpp>

/**
 * initSequence Метод инициализации параметров последовательности
 */
void anyks::Idw::initSequence() noexcept {
	// Очищаем список модулей
	this->xs2.clear();
	// Получаем модуль
	this->mod2 = (pow(2, MAX_SEQ_LENGTH + 1) - 1);
	// Формируем диапазон значений
	for(size_t i = 0; i < MAX_SEQ_LENGTH; i++){
		// Формируем список модулей
		this->xs2.push_back(modexp(12, i, this->mod2).toUnsignedLong());
	}
}
/**
 * setOffset Метод установки смещения в алфавите
 * @param offset смещение в алфавите
 */
void anyks::Idw::setOffset(const u_short offset) noexcept {
	// Запоминаем смещение в словаре
	this->offset = offset;
}
/**
 * get Метод генерирования идентификатора слова
 * @param  word слово для генерации
 * @return      идентификатор слова
 */
const size_t anyks::Idw::get(const wstring & word) const noexcept {
	// Результат работы функции
	size_t result = NIDW;
	// Если слово передано
	if(!word.empty() && !this->letters.empty()){
		// Контрольная сумма
		size_t sum = 0, pos = 0;
		// Выполняем приведение слова к нижнему регистру
		const wstring & tmp = this->alphabet->toLower(word);
		// Переходим по всему слову
		for(size_t i = 0; i < tmp.length(); i++){
			// Если мы не дошли до конца
			if(i < this->xs1.size()){
				// Получаем позицию буквы в алфавите
				pos = this->letters.find(tmp[i]);
				// Генерируем вектор
				if(pos != wstring::npos) sum += (this->xs1[i] * (pos + this->offset));
			// Выходим из цикла
			} else break;
		}
		// Убираем колизии
		result = (sum > 0 ? (sum % this->mod1) : result);
	}
	// Выводим результат
	return result;
}
/**
 * get Метод генерирования идентификатора последовательности
 * @param  seq последовательность для генерации
 * @return     идентификатор последовательности
 */
const size_t anyks::Idw::get(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	size_t result = NIDW;
	// Если последовательность передана
	if(!seq.empty()){
		// Список собранных элементов
		list <size_t> elements;
		/**
		 * calcFn Прототип функции подсчёта поэлементно всех цифр числа
		 * @param  число для подсчёта
		 * @return результат подсчёта
		 */
		function <u_short (size_t)> calcFn;
		/**
		 * calcFn Функция подсчёта поэлементно всех цифр числа
		 * @param num число для подсчёта
		 * @return    результат подсчёта
		 */
		calcFn = [&calcFn](size_t num){
			// Результат работы функции
			u_short result = 0;
			// Обрабатываем каждую цифру числа
			while(num > 0){
				// Выполняем расчёт результата
				result += (num % 10);
				// Уменьшаем промежуточное число на одно значение
				num /= 10;
			}
			// Если результат посчитан не до конца, продолжаем
			if(floor(log10(result)) > 0) result = calcFn(result);
			// Выводим результат
			return result;
		};
		/**
		 * parseFn Функция парсинга числа
		 * @param num число для парсинга
		 * @param elm список собранных чисел
		 */
		auto parseFn = [&calcFn](size_t num, list <size_t> & elm){
			// Получаем множитель
			const u_short factor = pow(10, ceil(log10(num) / 5));
			// Обрабатываем каждую цифру числа
			while(num > 0){
				// Добавляем каждое число в список
				elm.push_back(calcFn(num % factor));
				// Уменьшаем число на одно значение
				num /= factor;
			}
		};
		// Переходим по всей последовательности
		for(auto & item : seq){
			// Выполняем парсинг числа
			parseFn(item, elements);
			// Добавляем разделитель
			elements.push_back(result);
		}
		// Если список элементов не пустой
		if(!elements.empty()){
			// Удаляем последний элемент
			elements.pop_back();
			// Контрольная сумма и позиция элемента
			size_t sum = 0, index = 0;
			// Переходим по всему списку элементов
			for(auto & element : elements){
				// Если мы не дошли до конца
				if(index < this->xs2.size()){
					// Выполняем расчёт суммы
					sum += (this->xs2[index] * (element == result ? 11 : element + 1));
					// Увеличиваем количество элементов
					index++;
				// Выходим из цикла
				} else break;
			}
			// Убираем колизии
			result = (sum > 0 ? (sum % this->mod2) : result);
		}
	}
	// Выводим результат
	return result;
}
/**
 * set Метод установки алфавита
 * @param alphabet объект алфавита
 * @param offset   смещение в алфавите
 */
void anyks::Idw::set(const alphabet_t * alphabet, const u_short offset) noexcept {
	// Если объект алфавита передан, запоминаем его
	if(alphabet != nullptr){
		// Очищаем список модулей
		this->xs1.clear();
		// Сбрасываем алфавит
		this->letters = L"¶";
		// Устанавливаем смещение в алфавите
		this->offset = offset;
		// Запоминаем алфавит
		this->alphabet = alphabet;
		// Получаем модуль
		this->mod1 = (pow(2, MAX_WORD_LENGTH + 1) - 1);
		// Получаем алфавит
		this->letters.append(this->alphabet->convert(this->alphabet->get()));
		// Добавляем стандартные символы в список
		this->letters.append(L"<>~-+=*/:%|^&#\\0123456789");
		// Получаем длину алфавита
		const size_t length = this->letters.length();
		// Формируем диапазон значений
		for(size_t i = 0; i < MAX_WORD_LENGTH; i++){
			// Формируем список модулей
			this->xs1.push_back(modexp(length, i, this->mod1).toUnsignedLong());
		}
	}
}
/**
 * Idw Конструктор
 */
anyks::Idw::Idw() noexcept {
	// Выполняем инициализацию параметров числовой последовательности
	this->initSequence();
}
/**
 * Idw Конструктор
 * @param offset смещение в алфавите
 */
anyks::Idw::Idw(const u_short offset) noexcept {
	// Выполняем инициализацию параметров числовой последовательности
	this->initSequence();
	// Устанавливаем смещение в алфавите
	this->setOffset(offset);
}
/**
 * Idw Конструктор
 * @param alphabet объект алфавита
 */
anyks::Idw::Idw(const alphabet_t * alphabet) noexcept {
	// Выполняем инициализацию параметров числовой последовательности
	this->initSequence();
	// Выполняем установку алфавита
	this->set(alphabet);
}
/**
 * Idw Конструктор
 * @param alphabet объект алфавита
 * @param offset   смещение в алфавите
 */
anyks::Idw::Idw(const alphabet_t * alphabet, const u_short offset) noexcept {
	// Выполняем инициализацию параметров числовой последовательности
	this->initSequence();
	// Выполняем установку алфавита
	this->set(alphabet, offset);
}

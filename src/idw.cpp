/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <idw.hpp>

/**
 * setOffset Метод установки смещения в алфавите
 * @param offset смещение в алфавите
 */
void anyks::Idw::setOffset(const u_short offset){
	// Запоминаем смещение в словаре
	this->offset = offset;
}
/**
 * get Метод генерирования идентификатора слова
 * @param  word слово для генерации
 * @return      идентификатор слова
 */
const size_t anyks::Idw::get(const wstring & word) const {
	// Результат работы функции
	size_t result = noID;
	// Если слово передано
	if(!word.empty() && !this->letters.empty()){
		// Контрольная сумма
		size_t sum = 0, pos = 0;
		// Выполняем приведение слова к нижнему регистру
		auto tmp = this->alphabet->toLower(word);
		// Переходим по всему слову
		for(size_t i = 0; i < tmp.length(); i++){
			// Получаем позицию буквы в алфавите
			pos = this->letters.find(tmp[i]);
			// Генерируем вектор
			if(pos != wstring::npos) sum += (this->xs[i] * (pos + this->offset));
		}
		// Убираем колизии
		result = (sum > 0 ? (sum % this->modulus) : result);
	}
	// Выводим результат
	return result;
}
/**
 * set Метод установки алфавита
 * @param alphabet объект алфавита
 * @param offset   смещение в алфавите
 */
void anyks::Idw::set(const alphabet_t * alphabet, const u_short offset){
	// Если объект алфавита передан, запоминаем его
	if(alphabet != nullptr){
		// Очищаем список модулей
		this->xs.clear();
		// Сбрасываем алфавит
		this->letters = L"¶";
		// Устанавливаем смещение в алфавите
		this->offset = offset;
		// Запоминаем алфавит
		this->alphabet = alphabet;
		// Получаем модуль
		this->modulus = (pow(2, MAX_WORD_LENGTH + 1) - 1);
		// Получаем алфавит
		this->letters.append(this->alphabet->convert(this->alphabet->get()));
		// Добавляем стандартные символы в список
		this->letters.append(L"<>~-+=*/:%|^&#\\0123456789");
		// Получаем длину алфавита
		const size_t length = this->letters.length();
		// Формируем диапазон значений
		for(size_t i = 0; i < MAX_WORD_LENGTH; i++) this->xs.push_back(modexp(length, i, this->modulus).toUnsignedLong());
	}
}
/**
 * Idw Конструктор
 * @param offset смещение в алфавите
 */
anyks::Idw::Idw(const u_short offset){
	// Устанавливаем смещение в алфавите
	this->setOffset(offset);
}
/**
 * Idw Конструктор
 * @param alphabet объект алфавита
 */
anyks::Idw::Idw(const alphabet_t * alphabet){
	// Выполняем установку алфавита
	this->set(alphabet);
}
/**
 * Idw Конструктор
 * @param alphabet объект алфавита
 * @param offset   смещение в алфавите
 */
anyks::Idw::Idw(const alphabet_t * alphabet, const u_short offset){
	// Выполняем установку алфавита
	this->set(alphabet, offset);
}

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
			if(i < this->xs.size()){
				// Получаем позицию буквы в алфавите
				pos = this->letters.find(tmp[i]);
				// Генерируем вектор
				if(pos != wstring::npos) sum += (this->xs[i] * (pos + this->offset));
			// Выходим из цикла
			} else break;
		}
		// Убираем колизии
		result = (sum > 0 ? (sum % this->mod) : result);
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
		// Создаём буфер данных
		vector <char> buffer;
		// Создаём разделитель слов в n-грамме
		const string & separator = this->alphabet->convert(L"¶");
		// Выполняем выделение памяти для буфера данных
		buffer.resize((seq.size() * sizeof(size_t)) + (seq.size() * sizeof(wchar_t)));
		// Переходим по всему списку последовательности
		for(auto & idw : seq){
			// Получаем блок бинарных данных идентификатора слова
			const char * data = (char *) &idw;
			// Добавляем в буфер бинарные данные идентификатора слова
			buffer.insert(buffer.end(), data, data + sizeof(idw));
			// Получаем бинарные данные разделителя слов в n-грамме
			data = separator.data();
			// Добавляем в буфер бинарные данные разделителя слов в n-грамме
			buffer.insert(buffer.end(), data, data + separator.size());
		}
		// Если буфер данных получен, формируем на его основе 64-битный хеш
		if(!buffer.empty()) result = CityHash64(buffer.data(), buffer.size());
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
		this->xs.clear();
		// Сбрасываем алфавит
		this->letters = L"¶";
		// Устанавливаем смещение в алфавите
		this->offset = offset;
		// Запоминаем алфавит
		this->alphabet = alphabet;
		// Получаем модуль
		this->mod = (pow(2, MAX_WORD_LENGTH + 1) - 1);
		// Получаем алфавит
		this->letters.append(this->alphabet->convert(this->alphabet->get()));
		// Добавляем стандартные символы в список
		this->letters.append(L"<>~-+=*/:%|^'&#\\0123456789");
		// Получаем длину алфавита
		const size_t length = this->letters.length();
		// Формируем диапазон значений
		for(size_t i = 0; i < MAX_WORD_LENGTH; i++){
			// Формируем список модулей
			this->xs.push_back(modexp(length, i, this->mod).toUnsignedLong());
		}
	}
}
/**
 * Idw Конструктор
 */
anyks::Idw::Idw() noexcept : mod(0), offset(0), letters(L"¶"), alphabet(nullptr) {}
/**
 * Idw Конструктор
 * @param offset смещение в алфавите
 */
anyks::Idw::Idw(const u_short offset) noexcept : mod(0), offset(0), letters(L"¶"), alphabet(nullptr) {
	// Устанавливаем смещение в алфавите
	this->setOffset(offset);
}
/**
 * Idw Конструктор
 * @param alphabet объект алфавита
 */
anyks::Idw::Idw(const alphabet_t * alphabet) noexcept : mod(0), offset(0), letters(L"¶"), alphabet(nullptr) {
	// Выполняем установку алфавита
	this->set(alphabet);
}
/**
 * Idw Конструктор
 * @param alphabet объект алфавита
 * @param offset   смещение в алфавите
 */
anyks::Idw::Idw(const alphabet_t * alphabet, const u_short offset) noexcept : mod(0), offset(0), letters(L"¶"), alphabet(nullptr) {
	// Выполняем установку алфавита
	this->set(alphabet, offset);
}

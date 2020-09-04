/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <tokenizer.hpp>

/**
 * isOption Метод проверки наличия опции
 * @param option опция для проверки
 * @return       результат проверки
 */
const bool anyks::Tokenizer::isOption(const options_t option) const noexcept {
	// Выполняем проверку наличия опции
	const bool result = this->options.test((u_short) option);
	// Если флаг дал положительный результат и это режим отладки
	if(result && (option == options_t::debug)) this->logfile = nullptr;
	// Выводим результат
	return result;
}
/**
 * restore Метод восстановления текста из контекста
 * @param first   текущее слово для проверки
 * @param second  следующее слово для проверки
 * @param text    текст для сборки контекста
 * @param context контекст типов токенов
 */
void anyks::Tokenizer::restore(const wstring & first, const wstring & second, wstring & text, stack <type_t> & context) const noexcept {
	// Если слово передано
	if(!first.empty()){
		/**
		 * typeFn Функция определения типа токена
		 * @param word слово для определения токена
		 * @return     тип токена в контексте
		 */
		auto typeFn = [this](const wstring & word) noexcept {
			// Результат работы функции
			type_t result = type_t::null;
			// Если слово передано
			if(!word.empty()){
				// Если длина слова больше 1-го символа, значит это слово
				if(word.length() > 1){
					// Определяем, число это или нет
					result = (this->alphabet->isNumber(word) || this->alphabet->isDecimal(word) ? type_t::num : type_t::word);
				// Если это всего один символ
				} else {
					// Получаем символ токена в нижнем регистре
					const wchar_t letter = this->alphabet->toLower(word.front());
					// Если - это математическая операция
					if(this->alphabet->isMath(letter)) result = type_t::math;
					// Если - это символ пробела
					else if(this->alphabet->isSpace(letter)) result = type_t::space;
					// Если - это знак пунктуации
					else if(this->alphabet->isPunct(letter)) result = type_t::punct;
					// Если - это символ греческого алфавита
					else if(this->alphabet->isGreek(letter)) result = type_t::greek;
					// Если - это символ направления (стрелки)
					else if(this->alphabet->isRoute(letter)) result = type_t::route;
					// Если - это спец-символ
					else if(this->alphabet->isSpecial(letter)) result = type_t::specl;
					// Если - это символ игральных карт
					else if(this->alphabet->isPlayCards(letter)) result = type_t::pcards;
					// Если - это символ мировой валюты
					else if(this->alphabet->isCurrency(letter)) result = type_t::currency;
					// Если - это арабское число
					else if(this->alphabet->isNumber(wstring(1, letter))) result = type_t::num;
					// Если - это изоляционный символ
					else if(this->alphabet->isIsolation(letter)) {
						// Определяем тип изоляционных знаков
						const u_short type = (
							(letter == L'(') || (letter == L'[') ||
							(letter == L'{') || (letter == L'«') ||
							(letter == L'„') || (letter == L'‹') ||
							(letter == L'⌈') || (letter == L'⌊') || 
							(letter == L'<') || (letter == L'〈') ? 1 :
							((letter == L')') || (letter == L']') ||
							(letter == L'}') || (letter == L'»') ||
							(letter == L'“') || (letter == L'›') ||
							(letter == L'⌉') || (letter == L'⌋') ||
							(letter == L'>') || (letter == L'〉') ? 2 : 0)
						);
						// Определяем тип изоляционного символа
						switch(type){
							// Если это просто изоляционный символ
							case 0: result = type_t::isolat; break;
							// Если это изоляционный символ открытия
							case 1: result = type_t::open;   break;
							// Если это изоляционный символ закрытия
							case 2: result = type_t::close;  break;
						}
					// Если - это разрешённая буква алфавита
					} else if(this->alphabet->isLetter(letter)) result = type_t::allow;
				}
			}
			// Выводим результат
			return result;
		};
		// Тип текущего токена контекста
		type_t next = type_t::null, token = typeFn(first);
		// Если это начало предложения
		if(text.empty()) text.append(first);
		// Иначе выполняем основную обработку
		else {
			// Определяем тип токена
			switch((u_short) token){
				// Если это неизвестный символ
				case (u_short) type_t::null:
				// Если это просто слово
				case (u_short) type_t::word:
				// Если это открытый изоляционный символ
				case (u_short) type_t::open:
				// Если это нормальная буква
				case (u_short) type_t::allow:
				// Если это символ греческого алфавита
				case (u_short) type_t::greek: {
					// Если предыдущий символ не является открытым изоляционнмы символом
					if((context.empty() ||
					(context.top() != type_t::open)) &&
					(text.back() != L'´') &&
					(text.back() != L'¸') &&
					(text.back() != L'\x301') &&
					(text.back() != L'\x311') &&
					!this->alphabet->isSpace(text.back()) &&
					(!this->alphabet->isNumber(first.substr(0, 1)) ||
					!this->alphabet->isMath(text.back()) ||
					(text.back() == L'='))) text.append(1, L' ');
					// Устанавливаем регистр у первой буквы в тексте
					if(this->isOption(options_t::uppers) && this->alphabet->isSpace(text.back())){
						// Сбрасываем флаг регистра
						bool uppers = false;
						// Получаем длину собранного текста
						const size_t length = text.length();
						// Если последний символ является точка
						if(text.at(length - 2) == L'.'){
							// Если размер текста больше 4-х символов
							if(length >= 4){
								// Если последние символы не многоточие, разрешаем увеличение регистра
								if(text.substr(length - 4, 3).compare(L"...") != 0) uppers = true;
							// Если это короткое слово, разрешаем увеличение регистра
							} else uppers = true;
						// Если последний символ является знаком вопроса или восклицания, разрешаем увеличение регистра
						} else if((text.at(length - 2) == L'!') || (text.at(length - 2) == L'?') || (text.at(length - 2) == L'¡') || (text.at(length - 2) == L'¿')) uppers = true;
						// Если флаг увеличения регистра установлен
						if(uppers) const_cast <wstring *> (&first)->front() = this->alphabet->toUpper(first.front());
					}
					// Добавляем слово
					text.append(first);
				} break;
				// Если это число
				case (u_short) type_t::num:
				// Если это математическая операция
				case (u_short) type_t::math: {
					// Если предыдущий символ не является математической операцией и пробелом
					if((context.empty() ||
					((first.front() == L'=') && (text.back() != L'=')) ||
					((first.front() != L'=') && (text.back() == L'=')) ||
					((context.top() != type_t::open) &&
					(context.top() != type_t::space))) &&
					(!this->alphabet->isSpace(text.back()))) text.append(1, L' ');
					// Добавляем слово
					text.append(first);
				} break;
				// Если это символ мировых валют
				case (u_short) type_t::currency: {
					// Если предыдущий символ не является числом и пробелом
					if((context.empty() ||
					((context.top() != type_t::open) &&
					(context.top() != type_t::space))) &&
					(!this->alphabet->isSpace(text.back()))) text.append(1, L' ');
					// Добавляем слово
					text.append(first);
				} break;
				// Если это спец-символ
				case (u_short) type_t::specl: {
					// Если предыдущий символ не является спец-символом и пробелом
					if((context.empty() ||
					((context.top() != type_t::specl) &&
					(context.top() != type_t::open) &&
					(context.top() != type_t::space))) &&
					(!this->alphabet->isSpace(text.back()))) text.append(1, L' ');
					// Добавляем слово
					text.append(first);
				} break;
				// Если это символ направления (стрелки)
				case (u_short) type_t::route: {
					// Если предыдущий символ не является символом направления и пробелом
					if((context.empty() ||
					((context.top() != type_t::route) &&
					(context.top() != type_t::open) &&
					(context.top() != type_t::space))) &&
					(!this->alphabet->isSpace(text.back()))) text.append(1, L' ');
					// Добавляем слово
					text.append(first);
				} break;
				// Если это символ игральных карт
				case (u_short) type_t::pcards: {
					// Если предыдущий символ не является символом игральных карт и пробелом
					if((context.empty() ||
					((context.top() != type_t::pcards) &&
					(context.top() != type_t::open) &&
					(context.top() != type_t::space))) &&
					(!this->alphabet->isSpace(text.back()))) text.append(1, L' ');
					// Добавляем слово
					text.append(first);
				} break;
				// Если это закрытый изоляционный символ
				case (u_short) type_t::close:
				// Если это знак-пунктуации
				case (u_short) type_t::punct: {
					// Получаем тип токена
					next = (!second.empty() ? typeFn(second) : type_t::end);
					// Добавляем слово
					text.append(first);
					// Если следующий символ не является знаком пунктуации
					if((text.back() != L'´') &&
					(text.back() != L'¸') &&
					(text.back() != L'\x301') &&
					(text.back() != L'\x311') &&
					(next != type_t::punct) &&
					(next != type_t::end) &&
					(next != type_t::space) &&
					(next != type_t::close)) text.append(1, L' ');
				} break;
				// Если это изоляционный символ
				case (u_short) type_t::isolat: {
					// Получаем тип токена
					next = (!second.empty() ? typeFn(second) : type_t::end);
					// Если предыдущий символ не является изоляционным символом
					if((context.empty() || (context.top() != type_t::space)) &&
					(!this->alphabet->isSpace(text.back()))) text.append(1, L' ');
					// Добавляем слово
					text.append(first);
					// Если следующий символ является любым из слов кроме изоляционного символа и конца текста
					if((next != type_t::end) &&
					(next != type_t::space) &&
					(next != type_t::punct)) text.append(1, L' ');
				} break;
			}
		}
		// Добавляем тип токена в контекст токенов
		context.push(token);
	}
}
/**
 * addAbbr Метод добавления аббревиатуры
 * @param idw идентификатор слова для добавления
 */
void anyks::Tokenizer::addAbbr(const size_t idw) noexcept {
	// Устанавливаем идентификатор слова
	if((idw > 0) && (idw != idw_t::NIDW)) this->abbrs.emplace(idw);
}
/**
 * addAbbr Метод добавления аббревиатуры
 * @param word слово для добавления
 */
void anyks::Tokenizer::addAbbr(const string & word) noexcept {
	// Если слово передано, добавляем его в список аббревиатур
	if(word.size() > 2) this->addAbbr(this->idw(this->alphabet->convert(word)));
}
/**
 * addAbbr Метод добавления аббревиатуры
 * @param word слово для добавления
 */
void anyks::Tokenizer::addAbbr(const wstring & word) noexcept {
	// Если слово передано, добавляем его в список аббревиатур
	if(word.size() > 2) this->addAbbr(this->idw(word));
}
/**
 * setAbbrs Метод установки списка аббревиатур
 * @param abbrs список аббревиатур
 */
void anyks::Tokenizer::setAbbrs(const set <size_t> & abbrs) noexcept {
	// Если список аббревиатур передан
	if(!abbrs.empty()) this->abbrs = move(abbrs);
}
/**
 * setLogfile Метод установки файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::Tokenizer::setLogfile(const char * logfile) noexcept {
	// Устанавливаем адрес log файла
	this->logfile = logfile;
}
/**
 * setOption Метод установки опций модуля
 * @param option опция для установки
 */
void anyks::Tokenizer::setOption(const options_t option) noexcept {
	// Устанавливаем опции
	this->options.set((u_short) option);
}
/**
 * unsetOption Метод отключения опции модуля
 * @param option опция для отключения
 */
void anyks::Tokenizer::unsetOption(const options_t option) noexcept {
	// Устанавливаем опции
	this->options.reset((u_short) option);
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита
 */
void anyks::Tokenizer::setAlphabet(const alphabet_t * alphabet) noexcept {
	// Устанавливаем алфавит
	this->alphabet = alphabet;
	// Выполняем обновление параметров
	this->update();
}
/**
 * addSuffix Метод установки суффикса цифровой аббревиатуры
 * @param idw идентификатор суффикса цифровой аббревиатуры
 */
void anyks::Tokenizer::addSuffix(const size_t idw) const noexcept {
	// Если суффикс передан
	if(idw > 0) this->suffix.emplace(idw);
}
/**
 * setSuffixes Метод установки списка суффиксов цифровых аббревиатур
 * @param suffix список суффиксов цифровых аббревиатур
 */
void anyks::Tokenizer::setSuffixes(const set <size_t> & suffix) const noexcept {
	// Если список аббревиатур передан
	if(!suffix.empty()) this->suffix = suffix;
}
/**
 * addSuffix Метод извлечения суффикса из цифровой аббревиатуры
 * @param word слово для извлечения суффикса аббревиатуры
 * @param idw  идентификатор обрабатываемого слова
 */
void anyks::Tokenizer::addSuffix(const wstring & word, const size_t idw) const noexcept {
	// Если слово передано и оно является аббревиатурой
	if(!word.empty() && (word.back() != L'-')){
		// Если проверка пройедна
		if(((idw == idw_t::NIDW) || (idw == size_t(token_t::abbr))) &&
		this->alphabet->isNumber(wstring(1, word.front())) &&
		!this->alphabet->isNumber(wstring(1, word.back()))){
			// Выполняем поиск дефиса
			const size_t pos = word.rfind(L'-');
			// Если дефис найден
			if((pos != wstring::npos) && this->alphabet->isNumber(word.substr(0, pos))){
				// Получаем новое слово
				const wstring & suffix = word.substr(pos + 1);
				// Получаем установленный алфавит
				const wstring & alphabet = this->alphabet->wget();
				// Выполняем проверку на соответствие символов
				for(auto & letter : suffix){
					// Если буква не соответствует алфавиту, выходим
					if(alphabet.find(letter) == wstring::npos) return;
				}
				// Получаем идентификатор слова
				const size_t idw = this->idw(suffix);
				// Если идентификатор получен
				if(idw > 0) this->suffix.emplace(idw);
			}
		}
	}
}
/**
 * getAbbrs Метод извлечения списка аббревиатур
 * @return список аббревиатур
 */
const set <size_t> & anyks::Tokenizer::getAbbrs() const noexcept {
	// Выводим список аббревиатур
	return this->abbrs;
}
/**
 * getSuffixes Метод извлечения списка суффиксов цифровых аббревиатур
 * @return список цифровых аббревиатур
 */
const set <size_t> & anyks::Tokenizer::getSuffixes() const noexcept {
	// Выводим список суффиксов цифровых аббревиатур
	return this->suffix;
}
/**
 * fti Метод удаления дробной части числа
 * @param  num   число для обработки
 * @param  count количество символов после запятой
 * @return       число без дробной части
 */
const size_t anyks::Tokenizer::fti(const double num, const u_short count) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если число больше 0
	if(num > 0.0){
		// Если количество знаков после запятой не нулевой
		if(count > 0) result = (num * pow(10, floor(count)));
		// Если размер не указан
		else {
			// Определяем количество знаков после запятой
			u_short count = 0;
			// Расчёт текущего числа
			size_t number = 0;
			// Получаем текущее число
			const double tmp = fmod(num, 1);
			// Выполняем перебор чисел после запятой до нулей
			for(size_t i = 10;; i *= 10){
				// Выполняем смещение по числам после запятой
				number = ((size_t) (tmp * i) % 10);
				// Если число является нулём, выходим
				if((number <= 0) || (count >= 9)) break;
				// Считаем количество чисел после запятой
				else count++;
			}
			// Выполняем основной расчёт
			result = (num * (count > 0 ? pow(10, floor(count)) : pow(10, floor(log10(num)))));
		}
	}
	// Выводим результат
	return result;
}
/**
 * abbrEmpty Метод проверки на существование буквенных аббревиатур
 * @return результат проверки
 */
const bool anyks::Tokenizer::abbrEmpty() const noexcept {
	// Выводим результат проверки
	return this->abbrs.empty();
}
/**
 * suffixEmpty Метод проверки на существование суффиксов цифровых аббревиатур
 * @return результат проверки
 */
const bool anyks::Tokenizer::suffixEmpty() const noexcept {
	// Выводим результат проверки
	return this->suffix.empty();
}
/**
 * isToken Метод проверки идентификатора на токен
 * @param idw идентификатор слова для проверки
 * @return    результат проверки
 */
const bool anyks::Tokenizer::isToken(const size_t idw) const noexcept {
	// Выводим результат првоерки
	return (idw < size_t(token_t::endtoken));
}
/**
 * isIdWord Метод проверки на соответствие идентификатора слову
 * @param idw идентификатор слова для проверки
 * @return    результат проверки идентификатора
 */
const bool anyks::Tokenizer::isIdWord(const size_t idw) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если токен передан и он валиден
	if((idw > 0) && (idw != idw_t::NIDW)){
		// Выполняем проверку, является ли токен - нормальным словом
		result = !this->isToken(idw);
		// Если это системный токен
		if(!result){
			// Получаем идентификатор системного токена
			const token_t idt = (token_t) idw;
			// Проверяем, является ли токен разрешённым
			result = (
				(idt == token_t::num) ||
				(idt == token_t::url) ||
				(idt == token_t::unk) ||
				(idt == token_t::abbr) ||
				(idt == token_t::date) ||
				(idt == token_t::time) ||
				(idt == token_t::anum) ||
				(idt == token_t::rnum) ||
				(idt == token_t::greek) ||
				(idt == token_t::specl) ||
				(idt == token_t::aprox) ||
				(idt == token_t::range) ||
				(idt == token_t::score) ||
				(idt == token_t::dimen) ||
				(idt == token_t::fract) ||
				(idt == token_t::currency)
			);
		}
	}
	// Выводим результат проверки
	return result;
}
/**
 * isAbbr Метод проверки слова на соответствие аббревиатуры
 * @param idw идентификатор слова для проверки
 * @return    результат проверки
 */
const bool anyks::Tokenizer::isAbbr(const size_t idw) const noexcept {
		// Результат работы функции
	bool result = false;
	// Если слово передано
	if((idw > 0) && (idw != idw_t::NIDW) && !this->abbrs.empty()){
		// Выполняем проверку сущестования аббревиатуры
		result = (this->abbrs.count(idw) > 0);
	}
	// Выводим результат
	return result;
}
/**
 * isAbbr Метод проверки слова на соответствие аббревиатуры
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::Tokenizer::isAbbr(const wstring & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty()){
		// Если - это сборка данных, детектируем аббревиатуры только через модуль алфавита
		if(this->isOption(options_t::collect)) result = this->alphabet->isAbbr(word);
		// Иначе выполняем детектирование аббревиатуры на основе собранных данных
		else {
			// Если проверка пройедна
			if(this->alphabet->isNumber(wstring(1, word.front())) && !this->alphabet->isNumber(wstring(1, word.back()))){
				// Если список суффиксов цифровых аббревиатур передан
				if(!this->suffix.empty()){
					// Выполняем поиск дефиса
					const size_t pos = word.rfind(L'-');
					// Если дефис найден
					if((pos != wstring::npos) && this->alphabet->isNumber(word.substr(0, pos))){
						// Получаем идентификатор слова
						const size_t idw = this->idw(word.substr(pos + 1));
						// Если идентификатор получен
						if(idw > 0) result = (this->suffix.count(idw) > 0);
					}
				// Иначе проверяем на аббревиатуру с помощью алфавита
				} else result = this->alphabet->isAbbr(word);
			// Если список буквенных аббревиатур передан
			} else if(!this->abbrs.empty()){
				// Идентификатор слова
				size_t idw = idw_t::NIDW;
				// Если последний символ является точкой
				if(word.back() == L'.'){
					// Получаем слово для провеки
					const wstring tmp(word.begin(), word.end() - 1);
					// Получаем идентификатор слова
					idw = this->idw(tmp);
				// Если точка в слове не найдена
				} else idw = this->idw(word);
				// Выполняем проверку сущестования аббревиатуры
				result = (this->abbrs.count(idw) > 0);
				// Если аббревиатура не найдена, проверяем слово по словарю
				if(!result) result = this->alphabet->isAbbr(word);
			// Иначе проверяем на аббревиатуру с помощью алфавита
			} else result = this->alphabet->isAbbr(word);
		}
	}
	// Выводим результат
	return result;
}
/**
 * isSuffix Метод проверки слова на суффикс цифровой аббревиатуры
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::Tokenizer::isSuffix(const wstring & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty() && !this->suffix.empty()){
		// Если проверка пройедна
		if(this->alphabet->isNumber(wstring(1, word.front())) && !this->alphabet->isNumber(wstring(1, word.back()))){
			// Выполняем поиск дефиса
			const size_t pos = word.rfind(L'-');
			// Если дефис найден
			if((pos != wstring::npos) && this->alphabet->isNumber(word.substr(0, pos))){
				// Получаем идентификатор слова
				const size_t idw = this->idw(word.substr(pos + 1));
				// Если идентификатор получен
				if(idw > 0) result = (this->suffix.count(idw) > 0);
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * idt Метод извлечения идентификатора токена
 * @param  word слово для получения идентификатора
 * @return      идентификатор токена
 */
const anyks::token_t anyks::Tokenizer::idt(const wstring & word) const noexcept {
	// Результат работы функции
	token_t result = token_t::null;
	// Если слово передано
	if(!word.empty()){
		// Длина переданного слова
		const size_t size = word.size();
		// Переводим слово в нижний регистр
		const wstring & wrd = this->alphabet->toLower(word);
		// Если это не одна буква
		if(size > 1){
			// Значение текущей буквы
			wchar_t letter = 0;
			// Получаем первый символ слова
			const wchar_t first = wrd.front();
			// Получаем последний символ слова
			const wchar_t second = wrd.back();
			// Проверяем является ли первый символ числом
			const bool frontNum = this->alphabet->isNumber(wstring(1, first));
			// Определяем является ли последний символ числом
			const bool backNum = this->alphabet->isNumber(wstring(1, second));
			// Если первый символ не является числом а второй является (+42, +22.84, -12, -15.64, -18,22, ~25, ~845.53, ~12,46, ±43, ±44.22)
			if(!frontNum && backNum){
				// Проверяем является ли первый символ (-/+ или ~)
				if((first == L'-') || (first == L'−') || (first == L'~') || (first == L'∼') || (first == L'+') || (first == L'±')){
					// Получаем оставшуюся часть слова
					const wstring & tmp = wrd.substr(1);
					// Проверяем оставшуюся часть слова является числом
					if(this->alphabet->isNumber(tmp) || this->alphabet->isDecimal(tmp)){
						// Определяем тип токена
						switch(first){
							// Это положительно-отрицательное число
							case L'±':
							// Это положительное число
							case L'+':
							// Это отрицательное число
							case L'-':
							// Это отрицательное число
							case L'−': result = token_t::num; break;
							// Это приблизительное число
							case L'~':
							// Это приблизительное число
							case L'∼': result = token_t::aprox; break;
						}
					// Сообщаем что это псевдо-число
					} else result = token_t::anum;
				// Если слово не идентифицируемо, проверяем является ли оно url-адресом
				} else if(this->alphabet->isUrl(wrd)) result = token_t::url;
				// Если это не отрицательное и не приблизительное число (Дом-2)
				else {
					// Ищем дефис в конце слова
					size_t pos = wrd.rfind(L'-');
					// Если дефис не найден и не найдено завершение слова в виде числа
					if((pos == wstring::npos) || !this->alphabet->isNumber(wrd.substr(pos + 1))){
						// Сообщаем что это псевдо-число
						result = token_t::anum;
					}
				}
			// Если первый символ является числом а последний нет (2-й, 13-летний, 12$, 30°)
			} else if(frontNum && !backNum) {
				// Проверяем является ли первый символ числом со значением
				if((second == L'°') || (second == L'%') || (second == L'¹') || (second == L'²') ||
				(second == L'³') || (second == L'½') || (second == L'⅓') || (second == L'¼') || (second == L'¾')){
					// Получаем оставшуюся часть слова
					const wstring & tmp = wrd.substr(0, wrd.length() - 1);
					// Проверяем оставшуюся часть слова является числом
					if(this->alphabet->isNumber(tmp) || this->alphabet->isDecimal(tmp))
						// Запоминаем, что это число
						result = token_t::num;
					// Сообщаем что это псевдо-число
					else result = token_t::anum;
				// Если последний символ, является символом валюты
				} else if(this->alphabet->isCurrency(second)) {
					// Получаем оставшуюся часть слова
					const wstring & tmp = wrd.substr(0, wrd.length() - 1);
					// Проверяем оставшуюся часть слова является числом
					if(this->alphabet->isNumber(tmp) || this->alphabet->isDecimal(tmp))
						// Запоминаем, что это мировая валюта
						result = token_t::currency;
					// Сообщаем что это псевдо-число
					else result = token_t::anum;
				// Если это аббревиатура, запоминаем тип токена
				} else if(this->isAbbr(wrd))
					// Запоминаем что это аббревиатура
					result = token_t::abbr;
				// Если слово не идентифицируемо, проверяем является ли оно url-адресом
				else if(this->alphabet->isUrl(wrd)) result = token_t::url;
				// Иначе - это просто, псевдо-число
				else result = token_t::anum;
			// Если оба символа являются числом (5353, 5353.243, 3:4, 18:00, 18:00:01, 18.02.2012, 18/02/2012, 2/3, 3х10, 3~4)
			} else if(frontNum && backNum) {
				// Если это число
				if(this->alphabet->isNumber(wrd)) result = token_t::num;
				// Если это псевдо-число
				else {
					// Разделитель слова найден
					bool delim = false;
					// Запоминаем что это псевдо-число
					result = token_t::anum;
					// Переходим по всем символам слова
					for(size_t i = 0; i < size; i++){
						// Получаем значение текущей буквы
						letter = wrd.at(i);
						// Если плавающая точка найдена
						if((letter == L'.') || (letter == L',') || (letter == L':') || (letter == L'/') || (letter == L'x') ||
						(letter == L'х') || (letter == L'×') || (letter == L'~') || (letter == L'∼') || (letter == L'-') || (letter == L'−')){
							// Проверяем правые и левую части
							delim = (this->alphabet->isNumber(wrd.substr(0, i)) && this->alphabet->isNumber(wrd.substr(i + 1)));
							// Если число собрано
							if(delim){
								// Определяем тип разделителя
								switch(letter){
									case L',':
									case L'.': result = token_t::num;   break;
									case L'~':
									case L'∼': result = token_t::aprox; break;
									case L'-':
									case L'−': result = token_t::range; break;
									case L'/': result = token_t::fract; break;
									case L'x':
									case L'х':
									case L'×': result = token_t::dimen; break;
									case L':': result = token_t::score; break;
								}
							// Если число не собрано а являетс временем или датой
							} else if((letter == L':') || (letter == L'.') || (letter == L'/')) {
								// Список элементов слова
								vector <wstring> words;
								// Выполняем разбивку на составляющие
								this->alphabet->split(wrd, {letter}, words);
								// Если список разбит правильно
								if(words.size() == 3){
									// Переходим по всему списку слова
									for(auto & word : words){
										// Если слово не является числом
										if(!this->alphabet->isNumber(word)) return token_t::null;
									}
									// Определяем тип разделителя
									switch(letter){
										case L'/':
										case L'.': result = token_t::date; break;
										case L':': result = token_t::time; break;
									}
								// Если это точка, количество элементов в списке 4, предполагаем что это ip адрес
								} else if((letter == L'.') && (words.size() == 4)) result = token_t::null;
							}
							// Выходим из цикла
							break;
						}
					}
					// Если слово не идентифицируемо, проверяем является ли оно url-адресом
					if((result == token_t::null) && this->alphabet->isUrl(wrd)) result = token_t::url;
				}
			// Если это вообще не число, проверяем может это римское число
			} else if(!frontNum && !backNum) {
				// Проверяем является ли первый символ (-/+ или ~)
				if((wrd.length() > 2) && ((first == L'-') || (first == L'−') || (first == L'~') || (first == L'∼') || (first == L'+') || (first == L'±'))){
					// Проверяем является ли первый символ числом со значением
					if((second == L'°') || (second == L'%') || (second == L'¹') || (second == L'²') ||
					(second == L'³') || (second == L'½') || (second == L'⅓') || (second == L'¼') || (second == L'¾')){
						// Получаем оставшуюся часть слова
						const wstring & tmp = wrd.substr(1, wrd.length() - 2);
						// Проверяем оставшуюся часть слова является числом
						if(this->alphabet->isNumber(tmp) || this->alphabet->isDecimal(tmp))
							// Запоминаем, что это число
							result = token_t::num;
						// Сообщаем что это псевдо-число
						else result = token_t::anum;
					// Если последний символ, является символом валюты
					} else if(this->alphabet->isCurrency(second)) {
						// Получаем оставшуюся часть слова
						const wstring & tmp = wrd.substr(1, wrd.length() - 2);
						// Проверяем оставшуюся часть слова является числом
						if(this->alphabet->isNumber(tmp) || this->alphabet->isDecimal(tmp))
							// Запоминаем, что это мировая валюта
							result = token_t::currency;
						// Сообщаем что это псевдо-число
						else result = token_t::anum;
					// Сообщаем что это псевдо-число
					} else result = token_t::anum;
				// Если - это обычное слово, проверяем его
				} else {
					// Если - это аббревиатура
					if(this->isAbbr(wrd)) result = token_t::abbr;
					// Проверяем, является ли слово url-адресом
					else if(this->alphabet->isUrl(wrd)) result = token_t::url;
					// Определяем является ли слово, псевдо-числом
					else if(this->alphabet->isANumber(wrd)) result = token_t::anum;
					// Запоминаем что это число
					else if(this->alphabet->roman2Arabic(wrd) > 0) result = token_t::num;
				}
			}
		// Если это число то выводим токен числа
		} else if(this->alphabet->isNumber(wrd)) result = token_t::num;
		// Если это математический символ
		else if(this->alphabet->isMath(wrd.front())) result = token_t::math;
		// Если это символ знака пунктуации
		else if(this->alphabet->isPunct(wrd.front())) result = token_t::punct;
		// Если это символ греческого алфавита
		else if(this->alphabet->isGreek(wrd.front())) result = token_t::greek;
		// Если это символ направления (стрелка)
		else if(this->alphabet->isRoute(wrd.front())) result = token_t::route;
		// Если это спец-символ
		else if(this->alphabet->isSpecial(wrd.front())) result = token_t::specl;
		// Если это символ изоляции
		else if(this->alphabet->isIsolation(wrd.front())) result = token_t::isolat;
		// Если это символ игральных карт
		else if(this->alphabet->isPlayCards(wrd.front())) result = token_t::pcards;
		// Если это символ мировой валюты
		else if(this->alphabet->isCurrency(wrd.front())) result = token_t::currency;
		// Если слово не идентифицируемо и не разрешено, устанавливаем неизвестное слово
		if((result == token_t::null) && (!this->alphabet->isAllowed(wrd) && !this->alphabet->isLatian(wrd))) result = token_t::unk;
	}
	// Выводим результат
	return result;
}
/**
 * idw Метод извлечения идентификатора слова
 * @param  word слово для получения идентификатора
 * @return      идентификатор слова
 */
const size_t anyks::Tokenizer::idw(const wstring & word) const noexcept {
	// Формируем идентификатор слова
	return this->wrdId.get(word);
}
/**
 * ids Метод извлечения идентификатора последовательности
 * @param  seq последовательность для получения идентификатора
 * @return     идентификатор последовательности
 */
const size_t anyks::Tokenizer::ids(const vector <size_t> & seq) const noexcept {
	// Формируем идентификатор последовательности
	return this->wrdId.get(seq);
}
/**
 * readline Метод извлечения строки из текста
 * @param  is  файловый поток для чтения данных
 * @param  str строка для извлечения текста
 * @return     файловый поток с текущий позиции
 */
istream & anyks::Tokenizer::readline(istream & is, string & str) const noexcept {
	// Очищаем строку
	str.clear();
	// Создаем сторожа файлового потока
	istream::sentry se(is, true);
	// Создаем буфер чтения данных
	streambuf * sb = is.rdbuf();
	// Выполняем перебор всех входящих данных
	while(true){
		// Считываем символ из строки
		char c = sb->sbumpc();
		// Определяем тип символа
		switch(c){
			// Если это символ переноса строки
 			case '\n': return is;
 			// Если это символ возврата каретки
			case '\r': {
				// Проверяем следующий символ, если это перенос каретки, выходим
				if(sb->sgetc() == '\n') sb->sbumpc();
				// Выводим позицию файлового потока
				return is;
			}
			// Если это символ завершения файла
			case streambuf::traits_type::eof(): {
				// Если это конец, файла устанавливаем символ конца строки
				if(str.empty()) is.setstate(ios::eofbit);
				// Выводим позицию файлового потока
				return is;
			}
			// Если это любой другой символ, собираем строку
			default: str += c;
		}
	}
}
/**
 * restore Метод восстановления текста из контекста
 * @param context токенизированный контекст
 * @return        результирующий текст
 */
const string anyks::Tokenizer::restore(const vector <string> & context) const noexcept {
	// Результат работы функции
	string result = "";
	// Если контекст передан
	if(!context.empty() && (this->alphabet != nullptr)){
		// Собранный текст
		wstring text = L"";
		// Стек типов собранного контекста
		stack <type_t> types;
		// Получаем количество слов в контексте
		const size_t count = context.size();
		// Переходим по всем токенам
		for(size_t i = 0; i < count; ++i){
			// Выполняем формирование контекста
			this->restore(this->alphabet->convert(context[i]), ((i + 1) < count ? this->alphabet->convert(context[i + 1]) : L""), text, types);
		}
		// Устанавливаем регистр у первой буквы в тексте
		if(this->isOption(options_t::uppers)) text.front() = this->alphabet->toUpper(text.front());
		// Формируем результат
		result = this->alphabet->convert(text);
	}
	// Выводим результат
	return result;
}
/**
 * restore Метод восстановления текста из контекста
 * @param context токенизированный контекст
 * @return        результирующий текст
 */
const wstring anyks::Tokenizer::restore(const vector <wstring> & context) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если контекст передан
	if(!context.empty() && (this->alphabet != nullptr)){
		// Стек типов собранного контекста
		stack <type_t> types;
		// Получаем количество слов в контексте
		const size_t count = context.size();
		// Переходим по всем токенам
		for(size_t i = 0; i < count; ++i){
			// Выполняем формирование контекста
			this->restore(context[i], ((i + 1) < count ? context[i + 1] : L""), result, types);
		}
		// Устанавливаем регистр у первой буквы в тексте
		if(this->isOption(options_t::uppers)) result.front() = this->alphabet->toUpper(result.front());
	}
	// Выводим результат
	return result;
}
/**
 * clear Метод очистки собранных данных
 */
void anyks::Tokenizer::clear() noexcept {
	// Очищаем список аббревиатур
	this->abbrs.clear();
	// Очищаем список суффиксов цифровых аббревиатур
	this->suffix.clear();
}
/**
 * update Метод обновления параметров
 */
void anyks::Tokenizer::update() noexcept {
	// Устанавливаем алфавит и смещение в 23 позиций (количество системных токенов arpa)
	this->wrdId.set(this->alphabet, u_short(token_t::endtoken));
}
/**
 * setExternal Метод установки внешней функции токенизатора
 * @param fn внешняя функция токенизатора
 */
void anyks::Tokenizer::setExternal(tokenz_t fn) noexcept {
	// Устанавливаем функцию внешнего токенизатора
	this->extFn = fn;
}
/**
 * jsonToText Метод преобразования текста в формате json в текст
 * @param text     текст для преобразования в формате json
 * @param callback функция обратного вызова, на каждой итерации
 */
void anyks::Tokenizer::jsonToText(const string & text, function <void (const string &)> callback) const noexcept {
	// Если текст передан, и текст не больше 100Mb в одну строку
	if(!text.empty() && (this->alphabet != nullptr)){
		try {
			// Получаем данные в виде json
			json obj = json::parse(text);
			// Если это массив
			if(obj.is_array()){
				// Слово в контексте
				string word = "";
				// Список полученных слов
				vector <string> context;
				// Переходим по всем элементам объекта
				for(auto & item : obj.items()){
					// Если значение является массивом
					if(item.value().is_array()){
						// Переходим по всем словам контекста
						for(auto & elem : item.value().items()){
							// Если это строка
							if(elem.value().is_string()) elem.value().get_to(word);
							// Если слово получено, добавляем в контекст
							if(!word.empty()) context.push_back(word);
						}
						// Если список токенов получен
						if(!context.empty()) callback(this->restore(context));
					}
					// Очищаем полученный контекст
					context.clear();
				}
			}
		// Если возникает ошибка, ничего не делаем
		} catch(const exception & e) {}
	}
}
/**
 * textToJson Метод преобразования текста в json
 * @param text     текст для преобразования
 * @param callback функция обратного вызова, на каждой итерации
 */
void anyks::Tokenizer::textToJson(const string & text, function <void (const string &)> callback) const noexcept {
	// Если текст передан, и текст не больше 100Mb в одну строку
	if(!text.empty() && (text.size() <= MAX_STRING_BYTES) && (this->alphabet != nullptr)){
		// Формируем результат
		vector <string> tokens;
		// Результирующий объект
		vector <vector <string>> result;
		// Выполняем разбивку на токенизацию
		this->run(text, [&result, &tokens, this](const wstring & word, const vector <string> & context, const bool reset, const bool end) noexcept {
			// Если это сброс контекста
			if(reset || end){
				// Формируем новый список токенов
				if((!reset && end) && !word.empty()){
					// Дополняем список токенов
					tokens.push_back(this->alphabet->convert(word));
				}
				// Если список токенов получен
				if(!tokens.empty()){
					// Формируем строку результата
					result.push_back(tokens);
					// Очищаем список токенов
					tokens.clear();
				}
				// Если слово существует
				if(!word.empty()){
					// Формируем новый список токенов
					if(!end) tokens.push_back(this->alphabet->convert(word));
					// Если это сброс контекста и завершение текста
					else if(reset) result.push_back({this->alphabet->convert(word)});
				}
			// Добавляем новое слово в список токенов
			} else if(!word.empty()) tokens.push_back(this->alphabet->convert(word));
			// Выводим результат
			return true;
		});
		// Если результат получен
		if(!result.empty()){
			// Получаем результат в виде json
			json answer(result);
			// Выводим результат
			callback(answer.dump());
		}
	}
}
/**
 * writeSuffix Метод записи данных в файл суффиксов цифровых аббревиатур
 * @param filename адрес файла для записи
 * @param status   функция вывода статуса
 */
void anyks::Tokenizer::writeSuffix(const string & filename, function <void (const u_short)> status) noexcept {
	// Если адрес файла передан
	if(!filename.empty()){
		// Получаем список суффиксов цифровых аббревиатур
		const auto & abbrs = this->getSuffixes();
		// Если список суффиксов цифровых аббревиатур получен
		if(!abbrs.empty()){
			// Открываем файл на запись
			ofstream file(filename, ios::binary);
			// Если файл открыт, выполняем запись в файл результата
			if(file.is_open()){
				// Количество обработанных данных
				size_t index = 0;
				// Текущий и предыдущий статус
				u_short actual = 0, past = 100;
				// Переходим по всему списку аббревиатур
				for(auto & abbr : abbrs){
					// Создаём текст для записи
					const string & text = this->alphabet->format("%zu\r\n", abbr);
					// Выполняем запись данных в файл
					file.write(text.data(), text.size());
					// Если отладка включена
					if(status != nullptr){
						// Общий полученный размер данных
						index++;
						// Подсчитываем статус выполнения
						actual = u_short(index / double(abbrs.size()) * 100.0);
						// Если процентное соотношение изменилось
						if(past != actual){
							// Запоминаем текущее процентное соотношение
							past = actual;
							// Выводим статистику
							status(actual);
						}
					}
				}
				// Выводим статистику
				if(status != nullptr) status(100);
				// Закрываем файл
				file.close();
			}
		}
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "abbr file is not set");
}
/**
 * readSuffix Метод чтения данных из файла суффиксов цифровых аббревиатур
 * @param filename адрес файла для чтения
 * @param status   функция вывода статуса
 */
void anyks::Tokenizer::readSuffix(const string & filename, function <void (const string &, const u_short)> status) noexcept {
	// Если адрес файла передан
	if(!filename.empty()){
		// Количество обработанных данных
		size_t index = 0;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		/**
		 * parseFn Функция парсинга аббревиатур
		 * @param text     строка текста для парсинга
		 * @param filename адрес файла для чтения
		 * @param size     размер файла для парсинга
		 */
		auto parseFn = [&](const string & text, const string & filename, const uintmax_t size) noexcept {
			// Если текст передан
			if(!text.empty()){
				// Идентификатор суффикса цифровой аббревиатуры
				size_t idw = idw_t::NIDW;
				// Если текст является числом
				if(this->alphabet->isNumber(text)) idw = stoull(text);
				// Если текст числом не является, получаем его идентификатор
				else idw = this->idw(this->alphabet->convert(text));
				// Добавляем идентификатор аббревиатуры в токенизатор
				if((idw > 0) && (idw != idw_t::NIDW)) this->addSuffix(idw);
				// Если функция вывода статуса передана
				if(status != nullptr){
					// Увеличиваем количество записанных n-грамм
					index += text.size();
					// Выполняем расчёт текущего статуса
					actual = u_short(index / double(size) * 100.0);
					// Если статус обновился
					if(actual != past){
						// Запоминаем текущий статус
						past = actual;
						// Выводим статус извлечения
						status(filename, actual);
					}
				}
			}
		};
		// Если это файл
		if(fsys_t::isfile(filename)){
			// Выполняем считывание всех строк текста
			fsys_t::rfile(filename, [&filename, &parseFn](const string & text, const uintmax_t fileSize) noexcept {
				// Выполняем обработку полученного текста
				if(!text.empty()) parseFn(text, filename, fileSize);
			});
		// Если это каталог
		} else if(fsys_t::isdir(filename)) {
			// Переходим по всему списку словарей в каталоге
			fsys_t::rdir(filename, "abbr", [&](const string & filename, const uintmax_t dirSize) noexcept {
				// Выполняем считывание всех строк текста
				fsys_t::rfile2(filename, [&filename, &dirSize, &parseFn](const string & text, const uintmax_t fileSize) noexcept {
					// Выполняем обработку полученного текста
					if(!text.empty()) parseFn(text, filename, dirSize);
				});
			});
		// Выводим сообщение об ошибке
		} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "abbr file or path is broken");
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "abbr file is not set");
}
/**
 * run Метод разбивки текста на токены
 * @param text     входной текст для обработки
 * @param callback функция обратного вызова, на каждой итерации
 */
void anyks::Tokenizer::run(const string & text, function <const bool (const wstring &, const vector <string> &, const bool, const bool)> callback) const noexcept {
	// Если текст передан, и текст не больше 100Mb в одну строку
	if(!text.empty() && (text.size() <= MAX_STRING_BYTES) && (this->alphabet != nullptr)){
		// Выполняем токенизацию
		this->run(this->alphabet->convert(text), callback);
	}
}
/**
 * run Метод разбивки текста на токены
 * @param text     входной текст для обработки
 * @param callback функция обратного вызова, на каждой итерации
 */
void anyks::Tokenizer::run(const wstring & text, function <const bool (const wstring &, const vector <string> &, const bool, const bool)> callback) const noexcept {
	// Если текст передан, и текст не больше 100Mb в одну строку
	if(!text.empty() && (text.size() <= MAX_STRING_BYTES) && (this->alphabet != nullptr)){
		// Если внешняя функция существует, выполняем её
		if(this->extFn != nullptr) this->extFn(text, callback);
		// Иначе выполняем обработк собственными методами
		else {
			// Типы флагов
			enum class type_t : u_short {
				url,     // Интернет адрес
				num,     // Символ числа в тексте
				null,    // Не определено
				math,    // Математическая операция
				greek,   // Символ греческого алфавита
				route,   // Символ направления (стрелок)
				specl,   // Спец-символ в тексте
				space,   // Символ пробела в тексте
				allow,   // Разрешённый символ
				punct,   // Знак препинания
				pcards,  // Символ игральных карт
				isolat,  // Изоляционный символ в строке
				currency // Символ мировой валюты
			};
			// Полученное слово
			wstring word = L"";
			// Собранный контекст
			vector <string> context;
			// Стек открытых скобок
			stack <wchar_t> brackets;
			// Тип следующего символа
			type_t type = type_t::null;
			// Текущая буква и следующий символ
			wchar_t letter = 0, lletter = 0, backLetter = 0, next = 0;
			// Основные флаги токенизации
			bool open = false, begin = false, end = false, nend = false;
			bool backPunct = false, stopToken = false, notClear = false, stress = false;
			// Получаем буквы алфавита
			const wstring & letters = this->alphabet->convert(this->alphabet->get());
			/**
			 * callbackFn Функция вывода результата
			 * @param word полученное слово
			 * @param end  конец обработки текста
			 * @return     нужно ли завершить работу
			 */
			auto callbackFn = [&begin, &context, &callback, this](const wstring & word, const bool end) noexcept {
				// Выполняем сборку суффиксов цифровых аббревиатур
				if(this->isOption(options_t::collect)) this->addSuffix(word);
				// Отдаём результат
				const bool result = callback(word, context, begin && context.empty(), end);
				// Запоминаем что работа началась
				begin = (!begin ? !begin : begin);
				// Выводим результат
				return result;
			};
			/**
			 * erangeFn Функция проверяющая вхождения позиции в диапазон координат
			 * @param pos         значение позиции
			 * @param coordinates список координат в тексте
			 * @return            результат проверки
			 */
			auto erangeFn = [](const size_t pos, const map <size_t, size_t> & coordinates) noexcept {
				// Результат проверки
				bool result = false;
				// Если в списке нет неустановленных координат
				if(!coordinates.empty() && (coordinates.count(wstring::npos) < 1)){
					// Выполняем обход всех координат
					for(auto & item : coordinates){
						// Если позиция попала в запрещенную зону то сообщаем это
						result = !((pos <= item.first) || (pos >= item.second));
						// Если позиция вошла в указанный диапазон то выходим
						if(result) break;
					}
				}
				// Выводим результат
				return result;
			};
			// Выполняем поиск координат в тексте
			const auto coordinates = this->alphabet->urls(text);
			// Переходим по всему тексту
			for(size_t i = 0; i < text.length(); i++){
				// Получаем значение текущей буквы
				letter = text.at(i);
				// Переводим букву в нижний регистр
				lletter = this->alphabet->toLower(letter);
				// Определяем является ли это концом предложения
				end = (i == (text.length() - 1));
				// Определяем является ли слово адресом интернета
				if(erangeFn(i, coordinates)){
					// Формируем слово в виде url адреса
					word.append(1, letter);
					// Если это конец текста
					if(end && !callbackFn(word, end)) return;
				// Выполняем обычную обработку
				} else {
					// Получаем значение следующего символа
					next = (!end ? this->alphabet->toLower(text.at(i + 1)) : 0);
					// Если следующий символ является концом строки
					if((i + 1) == (text.length() - 1)) nend = true;
					// Если следующий символ является знаком препинания
					if(this->alphabet->isPunct(next)) type = type_t::punct;
					// Если следующий символ является математической операцией
					else if(this->alphabet->isMath(next)) type = type_t::math;
					// Если следующий символ является пробелом
					else if(this->alphabet->isSpace(next)) type = type_t::space;
					// Если это символ греческого алфавита
					else if(this->alphabet->isGreek(next)) type = type_t::greek;
					// Если это символ направления (стрелки)
					else if(this->alphabet->isRoute(next)) type = type_t::route;
					// Если следующий символ является спец-символом
					else if(this->alphabet->isSpecial(next)) type = type_t::specl;
					// Если это символ игральных карт
					else if(this->alphabet->isPlayCards(next)) type = type_t::pcards;
					// Если это символ мировой валюты
					else if(this->alphabet->isCurrency(next)) type = type_t::currency;
					// Если следующий символ является символом изоляции
					else if(this->alphabet->isIsolation(next)) type = type_t::isolat;
					// Если следующий символ является числом
					else if(this->alphabet->isNumber(wstring(1, next))) type = type_t::num;
					// Если следующий символ является разрешённым
					else if(this->alphabet->check(next)) type = type_t::allow;
					// Иначе зануляем следующий тип
					else type = type_t::null;
					// Если это пробел и слово не пустое
					if(this->alphabet->isSpace(lletter) && !word.empty()){
						// Выводим полученное слово
						if(!callbackFn(word, end)) return;
						// Добавляем слово в контекст
						context.push_back(this->alphabet->convert(word));
						// Очищаем слово
						word.clear();
					// Если это знак пунктуации
					} else if(this->alphabet->isPunct(lletter) ||
					this->alphabet->isSpecial(lletter) ||
					this->alphabet->isRoute(lletter) ||
					this->alphabet->isMath(lletter) ||
					this->alphabet->isPlayCards(lletter)) {
						// Проверяем следующие символы на стоп-токены
						stopToken = (
							(type == type_t::math) ||
							(type == type_t::space) ||
							(type == type_t::specl) ||
							(type == type_t::punct) ||
							(type == type_t::route) ||
							(type == type_t::isolat) ||
							(type == type_t::pcards)
						);
						// Проверяем является ли предыдущий символ также знаком пунктуации
						backPunct = ((backLetter > 0) && this->alphabet->isPunct(backLetter));
						// Запоминаем, что это символ ударения или подобный
						stress = (this->isOption(options_t::stress) && ((lletter == L'´') || (lletter == L'¸') || (lletter == L'\x301') || (lletter == L'\x311')));
						// Выводим результат как он есть
						if(end){
							// Если слово не пустое
							if(!word.empty()){
								// Если это спец-символ
								if(this->alphabet->isSpecial(lletter) || this->alphabet->isRoute(lletter) || this->alphabet->isPlayCards(lletter)){
									// Добавляем букву в слово
									word.append(1, letter);
									// Выводим полученное слово
									if(!callbackFn(word, end)) return;
								// Если это не спец-символ а математическая операция или знак препинания
								} else {
									// Выводим полученное слово
									if(!callbackFn(word, !end)) return;
									// Добавляем слово в контекст
									context.push_back(this->alphabet->convert(word));
									// Выводим знак препинания
									if(!callbackFn({letter}, end)) return;
								}
							// Выводим знак препинания
							} else if(!callbackFn({letter}, end)) return;
						// Если слово не существует
						} else if(word.empty() && !this->alphabet->isPunct(lletter) &&
						!(((lletter == L'-') || (lletter == L'+') || (lletter == L'*') || (lletter == L'×') ||
						(lletter == L'÷') || (lletter == L'∗') || (lletter == L'±') || (lletter == L'·') ||
						(lletter == L'⋅') || (lletter == L'≤') || (lletter == L'≥') || (lletter == L'−') ||
						(lletter == L'∼')) && (type == type_t::num))) {
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Добавляем знак препинания в контекст
							context.push_back(this->alphabet->convert(wstring(1, letter)));
						// Если слово не пустое и это точка, запятая или двоеточие
						} else if(stress || (lletter == L'.') || (lletter == L'…') || (lletter == L',') ||
						(lletter == L':') || (lletter == L';') || (lletter == L'/') || (lletter == L'™')) {
							// Если это многоточие
							if(lletter == L'…') backPunct = true;
							// Если это символ ударения, устанавливаем, что это не стоп-токен
							if(stress) stopToken = false;
							// Получаем символ для проверки
							wchar_t sumbol = (
								((lletter == L'.') || (lletter == L'…')) ?
								(!nend && (type == type_t::space) ? text.at(i + 2) :
								((type == type_t::allow) ? text.at(i + 1) : 0)) : 0
							);
							// Проверяем является ли слово аббревиатурой
							bool abbr = (
								(!word.empty() && (word.find(L'.') != wstring::npos)) ||
								((sumbol > 0) && ((word.length() < 3) ||
								((word.length() > 2) && (this->abbrs.count(this->idw(word)) > 0))) &&
								(!this->alphabet->isUpper(sumbol) || (word.length() == 1) ||
								(letters.find(tolower(sumbol)) == wstring::npos)))
							);
							// Если следующий символ является пробелом или нормальным словом
							if((lletter != L'/') && ((!stress && (type == type_t::allow) &&
							((lletter != L'.') || !abbr)) || ((stopToken && !abbr) || backPunct))){
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Добавляем слово в контекст
								if(!word.empty()) context.push_back(this->alphabet->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Добавляем знак препинания в контекст
								context.push_back(this->alphabet->convert(wstring(1, letter)));
								// Иначе очищаем контекст
								if(!notClear && (!backPunct || !!this->alphabet->isUpper(sumbol)) &&
								((lletter == L'.') || (lletter == L'…')) && (type != type_t::punct)) context.clear();
								// Очищаем слово
								word.clear();
							// Если это не пробел
							} else if(!this->alphabet->isSpace(lletter)) {
								// Если следующий символ является концом текста или стоп-токеном
								if(nend && stopToken){
									// Выводим полученное слово
									if(!callbackFn(word, end)) return;
									// Добавляем слово в контекст
									if(!word.empty()) context.push_back(this->alphabet->convert(word));
									// Выводим знак препинания
									if(!callbackFn({letter}, end)) return;
									// Добавляем знак препинания в контекст
									context.push_back(this->alphabet->convert(wstring(1, letter)));
									// Очищаем слово
									word.clear();
								// Иначе добавляем символ в словарь
								} else word.append(1, letter);
							}
						// Если это другие знаки препинания
						} else if((lletter == L'!') || (lletter == L'?') || (lletter == L'¡') || (lletter == L'¿')) {
							// Если следующий символ является пробелом или нормальным словом
							if((type == type_t::allow) || (type == type_t::space) ||
							(type == type_t::isolat) || stopToken || backPunct) {
								// Если это бинарное сравнение, символ в слово
								if((lletter == L'!') && (next == L'=')) word.append(1, letter);
								// Иначе обрываем контекст
								else {
									// Выводим полученное слово
									if(!callbackFn(word, end)) return;
									// Добавляем слово в контекст
									if(!word.empty()) context.push_back(this->alphabet->convert(word));
									// Выводим знак препинания
									if(!callbackFn({letter}, end)) return;
									// Добавляем знак препинания в контекст
									context.push_back(this->alphabet->convert(wstring(1, letter)));
									// Иначе очищаем контекст
									if(!notClear && !nend && (type != type_t::punct)
									&& (type != type_t::isolat)) context.clear();
									// Очищаем слово
									word.clear();
								}
							// Если это не пробел
							} else if(!this->alphabet->isSpace(lletter)) {
								// Если следующий символ является концом текста или стоп-токеном
								if(nend && stopToken){
									// Выводим полученное слово
									if(!callbackFn(word, end)) return;
									// Добавляем слово в контекст
									if(!word.empty()) context.push_back(this->alphabet->convert(word));
									// Выводим знак препинания
									if(!callbackFn({letter}, end)) return;
									// Добавляем знак препинания в контекст
									context.push_back(this->alphabet->convert(wstring(1, letter)));
									// Очищаем слово
									word.clear();
								// Иначе добавляем символ в словарь
								} else word.append(1, letter);
							}
						// Если это спец-символ
						} else if(this->alphabet->isSpecial(lletter) || this->alphabet->isMath(lletter) ||
						this->alphabet->isRoute(lletter) || this->alphabet->isPlayCards(lletter)) {
							// Добавляем символ в слово
							if(!this->alphabet->isSpace(lletter)) word.append(1, letter);
							// Если следующий символ является пробелом
							if(type == type_t::space){
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Добавляем слово в контекст
								if(!word.empty()) context.push_back(this->alphabet->convert(word));
								// Очищаем слово
								word.clear();
							}
						}
					// Если это изоляционный символ
					} else if(this->alphabet->isIsolation(lletter) && (!this->alphabet->isAllowApostrophe() || (lletter != L'\''))) {
						// Выводим результат как он есть
						if(end){
							// Если слово не пустое
							if(!word.empty()){
								// Выводим полученное слово
								if(!callbackFn(word, !end)) return;
								// Добавляем слово в контекст
								if(open) context.push_back(this->alphabet->convert(word));
								// Очищаем контекст
								else if(!notClear) context.clear();
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
							// Выводим знак препинания
							} else if(!callbackFn({letter}, end)) return;
						// Если это не конец
						} else {
							// Определяем тип изоляционных знаков
							u_short typeBracket = (
								(lletter == L'(') || (lletter == L'[') ||
								(lletter == L'{') || (lletter == L'«') ||
								(lletter == L'„') || (lletter == L'‹') ||
								(lletter == L'⌈') || (lletter == L'⌊') || 
								(lletter == L'<') || (lletter == L'〈') ? 1 :
								((lletter == L')') || (lletter == L']') ||
								(lletter == L'}') || (lletter == L'»') ||
								(lletter == L'“') || (lletter == L'›') ||
								(lletter == L'⌉') || (lletter == L'⌋') ||
								(lletter == L'>') || (lletter == L'〉') ? 2 : 0)
							);
							// Если это изоляционный знак открытия
							if(typeBracket == 1){
								// Запрещаем очистку словаря
								notClear = true;
								// Добавляем в стек наш знак
								brackets.push(letter);
								// Если слово не пустое
								if(!word.empty()){
									// Добавляем слово в контекст
									context.push_back(this->alphabet->convert(word));
									// Выводим полученное слово
									if(!callbackFn(word, end)) return;
									// Очищаем слово
									word.clear();
								}
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Добавляем знак препинания в контекст
								context.push_back(this->alphabet->convert(wstring(1, letter)));
							// Если это изоляционный знак закрытия и он совпадает с открытым
							} else if(typeBracket == 2) {
								// Разрешено продолжить работу
								bool mode = false;
								// Если стек не пустой
								if(!brackets.empty()){
									// Определяем тип открытой скобки
									switch(brackets.top()){
										// Если это круглая скобка
										case L'(': mode = (letter == L')'); break;
										// Если это квадратная скобка
										case L'[': mode = (letter == L']'); break;
										// Если это фигурная скобка
										case L'{': mode = (letter == L'}'); break;
										// Если это угловые кавычки
										case L'«': mode = (letter == L'»'); break;
										// Если это литературные кавычки
										case L'„': mode = (letter == L'“'); break;
										// Если это угловые кавычки
										case L'‹': mode = (letter == L'›'); break;
										// Если это верхние скобки
										case L'⌈': mode = (letter == L'⌉'); break;
										// Если это нижние скобки
										case L'⌊': mode = (letter == L'⌋'); break;
										// Если это обычные угловые скобки
										case L'<': mode = (letter == L'>'); break;
										// Если это большие угловые скобки
										case L'〈': mode = (letter == L'〉'); break;
									}
								}
								// Если слово не пусто
								if(!word.empty() && !callbackFn(word, end)) return;
								// Добавляем слово в контекст
								if(!word.empty()) context.push_back(this->alphabet->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Очищаем слово
								word.clear();
								// Добавляем знак препинания в контекст
								context.push_back(this->alphabet->convert(wstring(1, letter)));
								// Если разрешено продолжить
								if(mode) brackets.pop();
								// Если больше символов изоляции не открыто, разрешаем очистку контекста
								notClear = !brackets.empty();
							// Если это открытие изоляционного знака
							} else if((open = !open)) {
								// Если слово не пустое
								if(!word.empty()){
									// Если апостроф который находится в латинском слове
									if((letter == L'\'') && this->alphabet->isLatian({next}) &&
									this->alphabet->isLatian(this->alphabet->toLower(word))){
										// Отмечаем что символ изоляции не открыт
										open = false;
										// Добавляем символ к слову
										word.append(1, letter);
										// Продолжаем дальше
										continue;
									// Если это нормальное состояние
									} else {
										// Добавляем слово в контекст
										context.push_back(this->alphabet->convert(word));
										// Выводим полученное слово
										if(!callbackFn(word, end)) return;
										// Очищаем слово
										word.clear();
									}
								}
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Добавляем знак препинания в контекст
								if(!this->alphabet->isSpace(letter)){
									// Добавляем знак препинания в контекст
									context.push_back(this->alphabet->convert(wstring(1, letter)));
								}
							// Если это закрытие изоляционного знака
							} else {
								// Если слово не пустое
								if(!word.empty() && !callbackFn(word, end)) return;
								// Добавляем слово в контекст
								if(!word.empty()) context.push_back(this->alphabet->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Очищаем слово
								word.clear();
								// Добавляем знак препинания в контекст
								context.push_back(this->alphabet->convert(wstring(1, letter)));
							}
						}
					// Если это другие символы
					} else {
						// Если это первая буква в первом слове
						if(context.empty() && word.empty()){
							// Приводим букву к верхнему регистру
							if(this->isOption(options_t::uppers)) letter = this->alphabet->toUpper(letter);
						}
						// Формируем слово
						if(!this->alphabet->isSpace(lletter)) word.append(1, letter);
						// Выводим полученное слово, если это конец текста
						if(end) if(!callbackFn(word, end)) return;
					}
				}
				// Запоминаем текущую букву
				backLetter = lletter;
			}
		}
	}
}
/**
 * Tokenizer Конструктор
 * @param alphabet объект алфавита
 */
anyks::Tokenizer::Tokenizer(const alphabet_t * alphabet) noexcept : extFn(nullptr), alphabet(alphabet), logfile(nullptr) {
	// Устанавливаем лафавит
	if(alphabet != nullptr) this->setAlphabet(alphabet);
}
/**
 * ~Tokenizer Деструктор
 */
anyks::Tokenizer::~Tokenizer() noexcept {
	// Выполняем очистку собранных данных
	this->clear();
}

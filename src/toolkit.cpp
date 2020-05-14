/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <toolkit.hpp>

/**
 * word Метод получения данных слова
 * @param idw идентификатор слова
 * @return    запрашиваемое слово
 */
const anyks::word_t * anyks::Toolkit::word(const size_t idw) const noexcept {
	// Результат работы функции
	const word_t * result = nullptr;
	// Если нужно проверить пользовательские токены
	if(!this->utokens.empty() && (this->utokens.count(idw) > 0)){
		// Получаем слово обозначения пользовательского токена
		result = &this->utokens.at(idw).name;
	// Если это не пользовательский токен
	} else {
		// Ищем слово по идентификатору
		auto it = this->vocab.find(idw);
		// Если слово найдено
		if(it != this->vocab.end()) result = &it->second;
	}
	// Выводим результат
	return result;
}
/**
 * isOption Метод проверки наличия опции
 * @param option опция для проверки
 * @return       результат проверки
 */
const bool anyks::Toolkit::isOption(const options_t option) const noexcept {
	// Выполняем проверку наличия опции
	return this->options.test((u_short) option);
}
/**
 * getIdws Метод генерирования идентификатора слова
 * @param  word  слово для генерации
 * @param  check нужно выполнить дополнительную проверку слова
 * @return       идентификатор слова
 */
const size_t anyks::Toolkit::getIdw(const wstring & word, const bool check) const noexcept {
	// Результат работы функции
	size_t result = idw_t::NIDW;
	// Если слово передано
	if(!word.empty()){
		// Проверяем является ли слово, началом предложения
		if(word.compare(L"<s>") == 0) result = (size_t) token_t::start;
		// Проверяем является ли слово числом
		else if(word.compare(L"<num>") == 0) result = (size_t) token_t::num;
		// Проверяем является ли слово неизвестным
		else if(word.compare(L"<unk>") == 0) result = (size_t) token_t::unk;
		// Проверяем является ли слово url адресом
		else if(word.compare(L"<url>") == 0) result = (size_t) token_t::url;
		// Проверяем является ли слово аббревиатурой
		else if(word.compare(L"<abbr>") == 0) result = (size_t) token_t::abbr;
		// Проверяем является ли слово датой
		else if(word.compare(L"<date>") == 0) result = (size_t) token_t::date;
		// Проверяем является ли слово временем
		else if(word.compare(L"<time>") == 0) result = (size_t) token_t::time;
		// Проверяем является ли слово псевдо-числом
		else if(word.compare(L"<anum>") == 0) result = (size_t) token_t::anum;
		// Проверяем является ли слово концом предложения
		else if(word.compare(L"</s>") == 0) result = (size_t) token_t::finish;
		// Проверяем является ли слово математической операцией
		else if(word.compare(L"<math>") == 0) result = (size_t) token_t::math;
		// Проверяем является ли слово спец-символом
		else if(word.compare(L"<specl>") == 0) result = (size_t) token_t::specl;
		// Проверяем является ли слово диапазоном чисел
		else if(word.compare(L"<range>") == 0) result = (size_t) token_t::range;
		// Проверяем является ли слово знаком пунктуации
		else if(word.compare(L"<punct>") == 0) result = (size_t) token_t::punct;
		// Проверяем является ли слово приблизительным числом
		else if(word.compare(L"<aprox>") == 0) result = (size_t) token_t::aprox;
		// Проверяем является ли слово числовым счётом
		else if(word.compare(L"<score>") == 0) result = (size_t) token_t::score;
		// Проверяем является ли слово габаритными размерами
		else if(word.compare(L"<dimen>") == 0) result = (size_t) token_t::dimen;
		// Проверяем является ли слово числовой дробью
		else if(word.compare(L"<fract>") == 0) result = (size_t) token_t::fract;
		// Проверяем является ли слово знаком изоляции
		else if(word.compare(L"<isolat>") == 0) result = (size_t) token_t::isolat;
		// Если это другое слово
		else {
			// Формируем идентификатор слова
			result = this->tokenizer->idw(word);
			// Если нужно выполнить проверку слов
			if(check){
				// Проверяем является ли слово хорошим
				if(this->goodwords.empty() || (this->goodwords.count(result) < 1)){
					// Получаем временное слово
					wstring tmp = word;
					// Подсчитываем количество дефисов
					u_short hyphenCounts = 0;
					// Если нужно проверить пользовательские токены
					if(!this->utokens.empty()){
						// Переходим по всему списку пользовательских токенов
						for(auto & token : this->utokens){
							// Если сработал пользовательский токен
							if(token.second.test(
								token.second.name.real(),
								this->alphabet->convert(tmp)
							)) return token.first;
						}
					}
					// Проверяем является ли слово знаком пунктуации
					if(tmp.length() == 1){
						// Получаем букву для проверки
						const wchar_t letter = tmp.front();
						// Если это знак пунктуации
						if(this->alphabet->isPunct(letter)){
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::punct) < 1) result = (size_t) token_t::punct;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::punct) > 0) result = (size_t) token_t::unk;
						// Если буква является арабским числом
						} else if(this->alphabet->isNumber({letter})) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::num) < 1) result = (size_t) token_t::num;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::num) > 0) result = (size_t) token_t::unk;
						// Если это символ математической операции
						} else if(this->alphabet->isMath(letter)) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::math) < 1) result = (size_t) token_t::math;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::math) > 0) result = (size_t) token_t::unk;
						// Если это спец-символ
						} else if(this->alphabet->isSpecial(letter)) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::specl) < 1) result = (size_t) token_t::specl;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::specl) > 0) result = (size_t) token_t::unk;
						// Если это знак изоляции
						} else if(this->alphabet->isIsolation(letter)) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::isolat) < 1) result = (size_t) token_t::isolat;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::isolat) > 0) result = (size_t) token_t::unk;
						}
					// Проверяем есть ли изоляционный знак и количество дефисов в слове больше 2-х
					} else if(
						this->alphabet->isIsolation(tmp.back()) ||
						this->alphabet->isIsolation(tmp.front()) ||
						((hyphenCounts = this->alphabet->countLetter(tmp, L'-')) > 2)
					) result = (size_t) token_t::unk;
					// Если идентификатор определить не удалось
					else {
						// Идентификатор токена слова
						token_t idt = token_t::null;
						// Получаем идентификатор токена слова
						const token_t token = this->tokenizer->idt(tmp);
						// Пытаемся определить идентификатор слова
						switch((u_short) token){
							// Если это токен неизвестного слова
							case (u_short) token_t::unk: idt = token; break;
							// Если это токен числа, запоминаем его
							case (u_short) token_t::num: {
								// Если идентифицирование токена не отключено
								if(this->tokenDisable.count(token) < 1) idt = token;
								// Устанавливаем токен неизвестного слова
								if(this->tokenUnknown.count(token) > 0) idt = token_t::unk;
							} break;
							// Если токен определён как слово
							case (u_short) token_t::null: if(this->isOption(options_t::onlyGood)) idt = token_t::unk; break;
							// Если это другие токены
							default: {
								// Если идентифицирование токена не отключено
								if(this->tokenDisable.count(token) < 1) idt = token;
								// Устанавливаем токен неизвестного слова
								if(this->tokenUnknown.count(token) > 0) idt = token_t::unk;
							}
						}
						// Если слово определено как число но это не число, значит это римское число
						if((idt == token_t::num) && !this->alphabet->isNumber({tmp.back()})){
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::rnum) < 1) result = (size_t) token_t::num;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::rnum) > 0) result = (size_t) token_t::unk;
						// Иначе запоминаем идентификатор так-как он передан
						} else if(idt != token_t::null) result = (size_t) idt;
						// Если разрешено детектировать слова из смешанных словарей
						else if(this->isOption(options_t::mixdicts)){
							// Пытаемся детектировать слово со смешанными буквами
							if(this->alphabet->rest(tmp)) result = (size_t) token_t::unk;
							// Если слова отличаются, получаем новый идентификатор
							else if(tmp.compare(word) != 0) result = this->tokenizer->idw(tmp);
						}
					}
				}
			}
		}
	}
	// Если результат не определён, возвращаем неизвестное слово
	result = (result > 0 ? result : (size_t) token_t::unk);
	// Выводим результат
	return result;
}
/**
 * clearShielding Функция удаления экранирования
 * @param word  слово в котором следует удалить экранирование
 * @param front первый символ экранирования
 * @param back  последний символ экранирования
 */
void anyks::Toolkit::clearShielding(const string & word, const string & front, const string & back) const noexcept {
	// Если слово передано
	if(!word.empty() && (!front.empty() || !back.empty())){
		// Избавляемся от модификатора const
		string & str = const_cast <string &> (word);
		// Если первый символ экранирования передан
		if(!front.empty() && (str.substr(0, front.length()).compare(front) == 0)){
			// Удаляем первый символ экранирования
			str.replace(0, front.length(), "");
		}
		// Если второй символ экранирования передан
		if(!back.empty() && (str.substr(str.length() - back.length(), back.length()).compare(back) == 0)){
			// Удаляем первый символ экранирования
			str.replace(str.length() - back.length(), back.length(), "");
		}
	}
}
/**
 * getSize Метод получения размера n-грамы
 * @return длина n-граммы в языковой моделе
 */
const u_short anyks::Toolkit::getSize() const noexcept {
	// Выводим размер n-грамм в словаре
	return this->size;
}
/**
 * getOptions Метод извлечения установленных опций
 * @return установленные опции
 */
const u_int anyks::Toolkit::getOptions() const noexcept {
	// Выводим установленные опции
	return this->options.to_ulong();
}
/**
 * getParams Метод извлечения параметров алгоритма сжатия
 * @return параметры алгоритма сжатия
 */
const anyks::Toolkit::params_t & anyks::Toolkit::getParams() const noexcept {
	// Выводим параметры алгоритма сжатия
	return this->params;
}
/**
 * getUnknown Метод извлечения неизвестного слова
 * @return установленное неизвестное слово
 */
const string & anyks::Toolkit::getUnknown() const noexcept {
	// Результат работы функции
	static const string result = "";
	// Если неизвестное слово установлено
	if(this->unknown > 0){
		// Выполняем поиск в словаре
		auto it = this->vocab.find(this->unknown);
		// Если слово найдено
		if(it != this->vocab.end()) const_cast <string *> (&result)->assign(it->second.real());
	}
	// Выводим результат
	return result;
}
/**
 * getWordScript Метод извлечения скрипта обработки слов
 * @return адрес скрипта python обработки слов
 */
const string & anyks::Toolkit::getWordScript() const noexcept {
	// Результат работы функции
	static const string result = "";
	// Если скрипты установлены
	if(!this->scripts.empty()){
		// Ищем скрипт обработки слов
		auto it = this->scripts.find(1);
		// Если скрипт обработки слов установлен
		if(it != this->scripts.end()) const_cast <string *> (&result)->assign(it->second.first);
	}
	// Выводим результат
	return result;
}
/**
 * getUserTokenScript Метод извлечения скрипта обработки пользовательских токенов
 * @return адрес скрипта python обработки пользовательских токенов
 */
const string & anyks::Toolkit::getUserTokenScript() const noexcept {
	// Результат работы функции
	static const string result = "";
	// Если скрипты установлены
	if(!this->scripts.empty()){
		// Ищем скрипт обработки слов
		auto it = this->scripts.find(2);
		// Если скрипт обработки слов установлен
		if(it != this->scripts.end()) const_cast <string *> (&result)->assign(it->second.first);
	}
	// Выводим результат
	return result;
}
/**
 * getBadwords Метод извлечения чёрного списка
 * @return чёрный список слов
 */
const set <size_t> & anyks::Toolkit::getBadwords() const noexcept {
	// Выводим результат
	return this->badwords;
}
/**
 * getGoodwords Метод извлечения белого списка
 * @return белый список слов
 */
const set <size_t> & anyks::Toolkit::getGoodwords() const noexcept {
	// Выводим результат
	return this->goodwords;
}
/**
 * getTokensUnknown Метод извлечения списка токенов приводимых к <unk>
 * @return список токенов
 */
const set <anyks::token_t> & anyks::Toolkit::getTokensUnknown() const noexcept {
	// Выводим список токенов
	return this->tokenUnknown;
}
/**
 * getTokensDisable Метод извлечения списка запрещённых токенов
 * @return список токенов
 */
const set <anyks::token_t> & anyks::Toolkit::getTokensDisable() const noexcept {
	// Выводим список токенов
	return this->tokenDisable;
}
/**
 * getStatistic Метод извлечения общей статистики
 * @return общее количество документов и слов в корпусах при обучении
 */
const pair <size_t, size_t> anyks::Toolkit::getStatistic() const noexcept {
	// Выводим результат
	return make_pair(this->info.ad, this->info.cw);
}
/**
 * getUserTokens Метод извлечения списка пользовательских токенов
 * @return список пользовательских токенов
 */
const vector <string> & anyks::Toolkit::getUserTokens() const noexcept {
	// Результат работы функции
	static const vector <string> result;
	// Если пользовательские токены есть
	if(!this->utokens.empty()){
		// Переходим по всем пользовательским токенам
		for(auto & token : this->utokens){
			// Добавляем в список токен слова
			const_cast <vector <string> *> (&result)->push_back(this->getUserTokenWord(token.first));
		}
	}
	// Выводим результат
	return result;
}
/**
 * getUserTokenWord Метод получения пользовательского токена по его идентификатору
 * @param idw идентификатор пользовательского токена
 * @return    пользовательский токен соответствующий идентификатору
 */
const string anyks::Toolkit::getUserTokenWord(const size_t idw) const noexcept {
	// Результат работы функции
	string result = "";
	// Если идентификатор передан
	if((idw > 0) && (idw < idw_t::NIDW)){
		// Ищем идентификатор токена
		auto it = this->utokens.find(idw);
		// Если такой идентификатор существует
		if(it != this->utokens.end()){
			// Выводим слово соответствующее идентификатору
			result = it->second.name.real();
			// Выполняем удаление экранирования
			this->clearShielding(result, "<", ">");
		}
	}
	// Выводим результат
	return result;
}
/**
 * getUserTokenId Метод получения идентификатора пользовательского токена
 * @param name слово для которого нужно получить идентификатор
 * @return     идентификатор пользовательского токена соответствующий слову
 */
const size_t anyks::Toolkit::getUserTokenId(const string & name) const noexcept {
	// Результат работы функции
	size_t result = idw_t::NIDW;
	// Если слово передано
	if(!name.empty()){
		// Выполняем удаление экранирования
		this->clearShielding(name, "<", ">"); {
			// Название токена
			string word = "<";
			// Добавляем название
			word.append(name);
			// Добавляем экранирование
			word.append(">");
			// Получаем идентификатор слова
			const size_t idw = this->tokenizer->idw(this->alphabet->convert(word));
			// Если такой идентификатор существует, выводим его
			result = (this->utokens.count(idw) > 0 ? idw : result);
		}
	}
	// Выводим результат
	return result;
}
/**
 * clear Метод очистки
 */
void anyks::Toolkit::clear() noexcept {
	// Очищаем неизвестное слово
	this->unknown = 0;
	// Очищаем словарь arpa
	this->arpa->clear();
	// Очищаем словарь
	this->vocab.clear();
	// Очищаем список пользовательских токенов
	this->utokens.clear();
	// Очищаем список плохих слов
	this->badwords.clear();
	// Очищаем список хороших слов
	this->goodwords.clear();
	// Очищаем список токенов приводимых к <unk>
	this->tokenUnknown.clear();
	// Очищаем список запрещённых токенов
	this->tokenDisable.clear();
	// Очищаем объект статистики
	this->info = info_t();
	// Если объект питона установлен внешний
	if(this->notCleanPython){
		// Зануляем объект питона
		this->python = nullptr;
		// Разрешаем очистку объекта
		this->notCleanPython = false;
	}
}
/**
 * clearBadwords Метод очистки списка плохих слов
 */
void anyks::Toolkit::clearBadwords() noexcept {
	// Очищаем список плохих слов
	this->badwords.clear();
}
/**
 * clearGoodwords Метод очистки списка хороших слов
 */
void anyks::Toolkit::clearGoodwords() noexcept {
	// Очищаем список хороших слов
	this->goodwords.clear();
}
/**
 * clearUserTokens Метод очистки пользовательских токенов
 */
void anyks::Toolkit::clearUserTokens() noexcept {
	// Очищаем список пользовательских токенов
	this->utokens.clear();
}
/**
 * setAllTokenUnknown Метод установки всех токенов идентифицируемых как <unk>
 */
void anyks::Toolkit::setAllTokenUnknown() noexcept {
	// Устанавливаем все токены для идентифицировоания как <unk>
	this->tokenUnknown = {
		token_t::num,
		token_t::url,
		token_t::abbr,
		token_t::date,
		token_t::time,
		token_t::anum,
		token_t::math,
		token_t::rnum,
		token_t::specl,
		token_t::aprox,
		token_t::range,
		token_t::score,
		token_t::dimen,
		token_t::fract,
		token_t::punct,
		token_t::isolat
	};
}
/**
 * setAllTokenDisable Метод установки всех токенов как не идентифицируемых
 */
void anyks::Toolkit::setAllTokenDisable() noexcept {
	// Устанавливаем все токены для отключения
	this->tokenDisable = {
		token_t::num,
		token_t::url,
		token_t::abbr,
		token_t::date,
		token_t::time,
		token_t::anum,
		token_t::math,
		token_t::rnum,
		token_t::specl,
		token_t::aprox,
		token_t::range,
		token_t::score,
		token_t::dimen,
		token_t::fract,
		token_t::punct,
		token_t::isolat
	};
}
/**
 * addBadword Метод добавления идентификатора похого слова в список
 * @param idw идентификатор слова
 */
void anyks::Toolkit::addBadword(const size_t idw) noexcept {
	// Если идентификатор передан
	if((idw > 0) && (idw < idw_t::NIDW)) this->badwords.emplace(idw);
}
/**
 * addGoodword Метод добавления идентификатора хорошего слова в список
 * @param idw идентификатор слова
 */
void anyks::Toolkit::addGoodword(const size_t idw) noexcept {
	// Если идентификатор передан
	if((idw > 0) && (idw < idw_t::NIDW)) this->goodwords.emplace(idw);
}
/**
 * addBadword Метод добавления похого слова в список
 * @param word слово для добавления
 */
void anyks::Toolkit::addBadword(const string & word) noexcept {
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t idw = this->tokenizer->idw(this->alphabet->convert(word));
		// Добавляем слово в список плохих слов
		this->addBadword(idw);
	}
}
/**
 * addGoodword Метод добавления хорошего слова в список
 * @param word слово для добавления
 */
void anyks::Toolkit::addGoodword(const string & word) noexcept {
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t idw = this->tokenizer->idw(this->alphabet->convert(word));
		// Добавляем слово в список хороших слов
		this->addGoodword(idw);
	}
}
/**
 * setSize Метод установки размера n-граммы
 * @param size размер n-граммы
 */
void anyks::Toolkit::setSize(const u_short size) noexcept {
	// Устанавливаем размерность n-граммы
	this->size = (size > 0 ? size : DEFNGRAM);
}
/**
 * setUnknown Метод установки неизвестного слова
 * @param word слово для добавления
 */
void anyks::Toolkit::setUnknown(const string & word) noexcept {
	// Если слово передано
	if(!word.empty()){
		// Формируем идентификатор слова
		this->unknown = this->tokenizer->idw(this->alphabet->convert(word));
		// Если идентификатор получен
		if(this->unknown > 0){
			// Если слова нет в словаре, добавляем его
			if(this->vocab.count(this->unknown) < 1)
				// Добавляем слово в список
				this->vocab.emplace(this->unknown, word);
			// Если слово уже существует
			else {
				// Получаем слово из словаря
				word_t * word = &this->vocab.at(this->unknown);
				// Получаем метаданные
				auto meta = word->getmeta();
				// Увеличиваем встречаемость слова
				meta.oc++;
				// Устанавливаем полученные метаданные
				word->setmeta(meta);
			}
		}
	}
}
/**
 * setOptions Метод установки опций
 * @param options опции для установки
 */
void anyks::Toolkit::setOptions(const u_int options) noexcept {
	// Устанавливаем опции модуля
	this->options = options;
}
/**
 * setPythonObj Метод установки внешнего объекта питона
 * @param python внешний объект питона
 */
void anyks::Toolkit::setPythonObj(python_t * python) noexcept {
	// Если объект передан
	if(python != nullptr){
		// Устанавливаем объект питона
		this->python = python;
		// Запрещаем очистку объекта
		this->notCleanPython = true;
	}
}
/**
 * setLogfile Метод установки файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::Toolkit::setLogfile(const char * logfile) noexcept {
	// Устанавливаем адрес log файла
	this->logfile = logfile;
}
/**
 * setUserToken Метод добавления токена пользователя
 * @param name слово - обозначение токена
 */
void anyks::Toolkit::setUserToken(const string & name) noexcept {
	// Если слово передано
	if(!name.empty()){
		// Создаём новый пользовательский токен
		utoken_t userToken;
		// Выполняем удаление экранирования
		this->clearShielding(name, "<", ">"); {
			// Название токена
			string word = "<";
			// Добавляем название
			word.append(name);
			// Добавляем экранирование
			word.append(">");
			// Получаем идентификатор слова
			userToken.idw = this->tokenizer->idw(this->alphabet->convert(word));
			// Если такого идентификатора нет в списке
			if((userToken.idw > 0) && (this->utokens.count(userToken.idw) < 1)){
				// Добавляем в пользовательский токен наши параметры
				userToken.name = move(word);
				// Добавляем в список токенов, наш токен
				this->utokens.emplace(userToken.idw, move(userToken));
			}
		}
	}
}
/**
 * setOption Метод подключения опции модуля
 * @param option опция для подключения
 */
void anyks::Toolkit::setOption(const options_t option) noexcept {
	// Устанавливаем опции
	this->options.set((u_short) option);
}
/**
 * unsetOption Метод отключения опции модуля
 * @param option опция для отключения
 */
void anyks::Toolkit::unsetOption(const options_t option) noexcept {
	// Устанавливаем опции
	this->options.reset((u_short) option);
}
/**
 * setWordScript Метод установки скрипта обработки слов
 * @param script скрипт python обработки слов
 */
void anyks::Toolkit::setWordScript(const string & script) noexcept {
	// Выполняем добавление скрипта
	this->scripts.emplace(1, make_pair(script, 0));
}
/**
 * setTokenUnknown Метод установки списка токенов которых нужно идентифицировать как <unk>
 * @param options список токенов которых нужно идентифицировать как <unk>
 */
void anyks::Toolkit::setTokenUnknown(const string & options) noexcept {
	// Если опции переданы
	if(!options.empty()){
		// Список токенов для работы
		vector <wstring> tokens;
		// Выполняем разбиение строки на составляющие
		this->alphabet->split(options, "|", tokens);
		// Если токены получены
		if(!tokens.empty()){
			// Переходим по всему списку токенов
			for(auto & token : tokens){
				// Определяем тип токена
				if(token.compare(L"num") == 0)         this->tokenUnknown.emplace(token_t::num);
				else if(token.compare(L"url") == 0)    this->tokenUnknown.emplace(token_t::url);
				else if(token.compare(L"abbr") == 0)   this->tokenUnknown.emplace(token_t::abbr);
				else if(token.compare(L"date") == 0)   this->tokenUnknown.emplace(token_t::date);
				else if(token.compare(L"time") == 0)   this->tokenUnknown.emplace(token_t::time);
				else if(token.compare(L"anum") == 0)   this->tokenUnknown.emplace(token_t::anum);
				else if(token.compare(L"math") == 0)   this->tokenUnknown.emplace(token_t::math);
				else if(token.compare(L"rnum") == 0)   this->tokenUnknown.emplace(token_t::rnum);
				else if(token.compare(L"specl") == 0)  this->tokenUnknown.emplace(token_t::specl);
				else if(token.compare(L"aprox") == 0)  this->tokenUnknown.emplace(token_t::aprox);
				else if(token.compare(L"range") == 0)  this->tokenUnknown.emplace(token_t::range);
				else if(token.compare(L"score") == 0)  this->tokenUnknown.emplace(token_t::score);
				else if(token.compare(L"dimen") == 0)  this->tokenUnknown.emplace(token_t::dimen);
				else if(token.compare(L"fract") == 0)  this->tokenUnknown.emplace(token_t::fract);
				else if(token.compare(L"punct") == 0)  this->tokenUnknown.emplace(token_t::punct);
				else if(token.compare(L"isolat") == 0) this->tokenUnknown.emplace(token_t::isolat);
			}
		}
	}
}
/**
 * setTokenDisable Метод установки списка не идентифицируемых токенов
 * @param options список не идентифицируемых токенов
 */
void anyks::Toolkit::setTokenDisable(const string & options) noexcept {
	// Если опции переданы
	if(!options.empty()){
		// Список токенов для работы
		vector <wstring> tokens;
		// Выполняем разбиение строки на составляющие
		this->alphabet->split(options, "|", tokens);
		// Если токены получены
		if(!tokens.empty()){
			// Переходим по всему списку токенов
			for(auto & token : tokens){
				// Определяем тип токена
				if(token.compare(L"num") == 0)         this->tokenDisable.emplace(token_t::num);
				else if(token.compare(L"url") == 0)    this->tokenDisable.emplace(token_t::url);
				else if(token.compare(L"abbr") == 0)   this->tokenDisable.emplace(token_t::abbr);
				else if(token.compare(L"date") == 0)   this->tokenDisable.emplace(token_t::date);
				else if(token.compare(L"time") == 0)   this->tokenDisable.emplace(token_t::time);
				else if(token.compare(L"anum") == 0)   this->tokenDisable.emplace(token_t::anum);
				else if(token.compare(L"math") == 0)   this->tokenDisable.emplace(token_t::math);
				else if(token.compare(L"rnum") == 0)   this->tokenDisable.emplace(token_t::rnum);
				else if(token.compare(L"specl") == 0)  this->tokenDisable.emplace(token_t::specl);
				else if(token.compare(L"aprox") == 0)  this->tokenDisable.emplace(token_t::aprox);
				else if(token.compare(L"range") == 0)  this->tokenDisable.emplace(token_t::range);
				else if(token.compare(L"score") == 0)  this->tokenDisable.emplace(token_t::score);
				else if(token.compare(L"dimen") == 0)  this->tokenDisable.emplace(token_t::dimen);
				else if(token.compare(L"fract") == 0)  this->tokenDisable.emplace(token_t::fract);
				else if(token.compare(L"punct") == 0)  this->tokenDisable.emplace(token_t::punct);
				else if(token.compare(L"isolat") == 0) this->tokenDisable.emplace(token_t::isolat);
			}
		}
	}
}
/**
 * setUserTokenScript Метод установки скрипта обработки пользовательских токенов
 * @param script скрипт python обработки пользовательских токенов
 */
void anyks::Toolkit::setUserTokenScript(const string & script) noexcept {
	// Выполняем добавление скрипта
	this->scripts.emplace(2, make_pair(script, 0));
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита
 */
void anyks::Toolkit::setAlphabet(const alphabet_t * alphabet) noexcept {
	// Если алфавит передан
	if(alphabet != nullptr) this->alphabet = alphabet;
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer объект токенизатора
 */
void anyks::Toolkit::setTokenizer(const tokenizer_t * tokenizer) noexcept {
	// Если токенизатор передан
	if(tokenizer != nullptr) this->tokenizer = tokenizer;
}
/**
 * setTokensUnknown Метод установки списка токенов приводимых к <unk>
 * @param tokens список токенов для установки
 */
void anyks::Toolkit::setTokensUnknown(const set <token_t> & tokens) noexcept {
	// Если список получен, устанавливаем его
	if(!tokens.empty()) this->tokenUnknown = move(tokens);
}
/**
 * setTokensDisable Метод установки списка запрещённых токенов
 * @param tokens список токенов для установки
 */
void anyks::Toolkit::setTokensDisable(const set <token_t> & tokens) noexcept {
	// Если список получен, устанавливаем его
	if(!tokens.empty()) this->tokenDisable = move(tokens);
}
/**
 * getStatistic Метод установки общей статистики
 * @param ad общее количество документов при обучении
 * @param cw общее количество слов при обучении
 */
void anyks::Toolkit::getStatistic(const size_t ad, const size_t cw) noexcept {
	// Устанавливаем общее количество документов
	if(ad > 0) this->info.ad = ad;
	// Устанавливаем общее количество слов
	if(cw > 0) this->info.cw = cw;
}
/**
 * setBadwords Метод установки списка идентификаторов плохих слов в список
 * @param badwords список идентификаторов плохих слов
 */
void anyks::Toolkit::setBadwords(const set <size_t> & badwords) noexcept {
	// Если список не пустой, устанавливаем список
	if(!badwords.empty()) this->badwords = move(badwords);
}
/**
 * setBadwords Метод установки списка плохих слов в список
 * @param badwords список плохих слов
 */
void anyks::Toolkit::setBadwords(const vector <string> & badwords) noexcept {
	// Если список не пустой
	if(!badwords.empty()){
		// Очищаем список плохих слов
		this->badwords.clear();
		// Переходим по всему списку слов
		for(auto & word : badwords) this->addBadword(word);
	}
}
/**
 * setGoodwords Метод установки списка идентификаторов хороших слов в список
 * @param goodwords список идентификаторов хороших слов
 */
void anyks::Toolkit::setGoodwords(const set <size_t> & goodwords) noexcept {
	// Если список не пустой, устанавливаем список
	if(!goodwords.empty()) this->goodwords = move(goodwords);
}
/**
 * setGoodwords Метод установки списка хороших слов в список
 * @param goodwords список хороших слов
 */
void anyks::Toolkit::setGoodwords(const vector <string> & goodwords) noexcept {
	// Если список не пустой
	if(!goodwords.empty()){
		// Очищаем список хороших слов
		this->goodwords.clear();
		// Переходим по всему списку слов
		for(auto & word : goodwords) this->addGoodword(word);
	}
}
/**
 * addText Метод добавления текста для расчёта
 * @param text текст который нужно добавить
 * @param idd  идентификатор документа
 */
void anyks::Toolkit::addText(const string & text, const size_t idd) noexcept {
	// Если текст передан
	if(!text.empty()){
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		// Идентификатор начала предложения
		const size_t sid = (size_t) token_t::start;
		// Идентификатор конца предложения
		const size_t fid = (size_t) token_t::finish;
		// Список последовательностей для обучения
		vector <pair_t> seq = {{sid, 0}};
		/**
		 * unkFn Функция установки неизвестного слова в последовательность
		 * @return нужно ли остановить сбор последовательности
		 */
		auto unkFn = [&seq, uid, idd, this]() noexcept {
			// Результат работы функции
			bool result = true;
			// Если неизвестное слово не установлено
			if((this->unknown == 0) && this->isOption(options_t::allowUnk)) seq.emplace_back(uid, 0);
			// Если неизвестное слово установлено
			else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
			// Если неизвестный токен нельзя хранить в arpa
			else {
				// Отправляем последовательность для добавления в arpa
				if(seq.size() > 1) this->arpa->add(seq, idd);
				// Очищаем список последовательностей
				seq.clear();
				// Запоминаем что нужно остановить вывод
				result = false;
			}
			// Выводим результат
			return result;
		};
		/**
		 * resFn Функция вывода результата
		 */
		auto resFn = [&seq, fid, sid, idd, this]() noexcept {
			// Идентификатор предыдущего слова
			size_t idw = idw_t::NIDW;
			// Удаляем следующие друг за другом неизвестные слова
			for(auto it = seq.begin(); it != seq.end();){
				/**
				 * Если текущее слово это неизвестный символ
				 * и предыдущее слово тоже было неизвестным символом
				 */
				if((it->first == uid) && (idw == uid)){
					// Запоминаем текущее слово
					idw = it->first;
					// Удаляем текущее слово
					it = seq.erase(it);
				// Если предыдущее слово небыло неизвестным
				} else {
					// Запоминаем текущее слово
					idw = it->first;
					// Продолжаем обход дальше
					++it;
				}
			}
			// Если последним словом идет неизвестное слово, удаляем его
			if(seq.back().first == uid) seq.pop_back();
			// Добавляем в список конец предложения
			seq.emplace_back(fid, 0);
			/**
			 * Если слова всего два, значит это начало и конец предложения
			 * Нам же нужны только нормальные n-граммы
			 */
			if(seq.size() > 2) this->arpa->add(seq, idd);
			// Очищаем список последовательностей
			seq.clear();
			// Добавляем в список начало предложения
			seq.emplace_back(sid, 0);
		};
		/**
		 * modeFn Функция обработки разбитого текста
		 * @param word  слово для обработки
		 * @param ctx   контекст к которому принадлежит слово
		 * @param reset флаг сброса контекста
		 * @param stop  флаг завершения обработки
		 */
		auto modeFn = [&](const wstring & word, const vector <string> & ctx, const bool reset, const bool stop) noexcept {
			// Если это сброс контекста, отправляем результат
			if(reset) resFn();
			// Если слово передано
			if(!word.empty()){
				// Получаем данные слова
				word_t tmp = word;
				// Если модуль питона активирован
				if(this->python != nullptr){
					// Ищем скрипт обработки слов
					auto it = this->scripts.find(1);
					// Если скрипт обработки слов установлен
					if(it != this->scripts.end()){
						// Блокируем поток
						this->locker.lock();
						// Выполняем внешний python скрипт
						tmp = this->python->run(it->second.second, {tmp.real()}, ctx);
						// Разблокируем поток
						this->locker.unlock();
					}
				// Если модуль предобработки слов, существует
				} else if(this->wordPress != nullptr) tmp = this->wordPress(tmp.real(), ctx);
				// Если слово не разрешено
				if((tmp.length() >= MAX_WORD_LENGTH) && !unkFn()) return true;
				// Если слово разрешено
				else if(!tmp.empty()) {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Если это плохое слово, заменяем его на неизвестное
					if(((idw == 0) || (idw == idw_t::NIDW) || (this->badwords.count(idw) > 0)) && !unkFn()) return true;
					// Иначе продолжаем дальше
					else {
						// Флаг проверки слова на событийное
						const bool isWord = this->arpa->event(idw);
						// Если это неизвестное слово
						if((idw == uid) && !unkFn()) return true;
						// Иначе добавляем слово
						else if(!isWord || (this->goodwords.count(idw) > 0) || this->alphabet->isAllowed(tmp)) {
							// Регистры слова
							size_t uppers = 0;
							// Если это событийное слово
							if(isWord){
								// Получаем регистры слова
								uppers = (seq.size() > 1 ? tmp.getUppers() : 0);
								// Если название токена получено, устанавливаем его регистры
								if(this->utokens.count(idw) > 0) uppers = 0;
								// Устанавливаем регистр слова
								tmp.setUppers(uppers);
								// Добавляем слово в словарь
								this->addWord(tmp.wreal(), idw, idd);
							}
							// Добавляем слово в последовательность
							seq.emplace_back(idw, uppers);
						// Отправляем слово как неизвестное
						} else if(!unkFn()) return true;
					}
				}
			}
			// Если это конец, отправляем результат
			if(stop) resFn();
			// Выводим результат
			return true;
		};
		// Выполняем разбивку текста на токены
		this->tokenizer->run(text, modeFn);
	}
}
/**
 * addWord Метод добавления слова в словарь
 * @param word слово для добавления
 * @param idw  идентификатор слова, если нужноы
 * @param idd  идентификатор документа
 */
void anyks::Toolkit::addWord(const word_t & word, const size_t idw, const size_t idd) noexcept {
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t id = (idw > 0 ? idw : this->getIdw(word));
		// Если идентификатор нормальный
		if((id > 0) && (id < idw_t::NIDW) && this->arpa->event(id) &&
		(this->utokens.count(id) < 1) && (this->badwords.count(id) < 1)){
			// Ищем слово в словаре
			auto it = this->vocab.find(id);
			// Если слово найдено
			if(it != this->vocab.end()){
				// Получаем метаданные
				auto meta = it->second.getmeta();
				// Получаем метаданные слова
				const auto & wmta = word.getmeta();
				// Увеличиваем встречаемость слова
				meta.oc += wmta.oc;
				// Если документ сменился
				meta.dc += wmta.dc;
				// Устанавливаем полученные метаданные
				it->second.setmeta(meta);
			// Иначе устанавливаем слово
			} else {
				// Добавляем слово в словарь
				this->vocab.emplace(id, word);
				// Запоминаем количество уникальных слов
				this->info.unq = this->vocab.size();
			}
		}
		// Увеличиваем количество слов
		this->info.cw++;
		// Если документ сменился
		if(this->info.idd != idd){
			// Увеличиваем количество документов
			this->info.ad++;
			// Запоминаем идентификатор документа
			this->info.idd = idd;
		}
	}
}
/**
 * addWord Метод добавления слова в словарь
 * @param word слово для добавления
 * @param idw  идентификатор слова, если нужноы
 * @param idd  идентификатор документа
 */
void anyks::Toolkit::addWord(const wstring & word, const size_t idw, const size_t idd) noexcept {
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t id = (idw > 0 ? idw : this->getIdw(word));
		// Если идентификатор нормальный
		if((id > 0) && (id < idw_t::NIDW) && this->arpa->event(id) &&
		(this->utokens.count(id) < 1) && (this->badwords.count(id) < 1)){
			// Ищем слово в словаре
			auto it = this->vocab.find(id);
			// Если слово найдено
			if(it != this->vocab.end()){
				// Получаем метаданные
				auto meta = it->second.getmeta();
				// Увеличиваем встречаемость слова
				meta.oc++;
				// Если документ сменился
				if(this->info.idd != idd) meta.dc++;
				// Устанавливаем полученные метаданные
				it->second.setmeta(meta);
			// Иначе устанавливаем слово
			} else {
				// Добавляем слово в словарь
				this->vocab.emplace(id, word);
				// Запоминаем количество уникальных слов
				this->info.unq = this->vocab.size();
			}
		}
		// Увеличиваем количество слов
		this->info.cw++;
		// Если документ сменился
		if(this->info.idd != idd){
			// Увеличиваем количество документов
			this->info.ad++;
			// Запоминаем идентификатор документа
			this->info.idd = idd;
		}
	}
}
/**
 * setWordPreprocessingMethod Метод установки функции препроцессинга слова
 * @param fn внешняя функция препроцессинга слова
 */
void anyks::Toolkit::setWordPreprocessingMethod(wpres_t fn) noexcept {
	// Устанавливаем функцию
	this->wordPress = fn;
}
/**
 * setUserTokenMethod Метод установки функции обработки пользовательского токена
 * @param name слово - обозначение токена
 * @param fn   внешняя функция обрабатывающая пользовательский токен
 */
void anyks::Toolkit::setUserTokenMethod(const string & name, function <bool (const string &, const string &)> fn) noexcept {
	// Если название токена передано
	if(!name.empty()){
		// Получаем идентификатор токена
		const size_t idw = this->getUserTokenId(name);
		// Ищем нам токен
		auto it = this->utokens.find(idw);
		// Если такой токен найден, устанавливаем функци
		if(it != this->utokens.end()) it->second.test = fn;
	}
}
/**
 * init Метод инициализации языковой модели
 * @param algorithm алгоритм расчёта языковой модели
 * @param modified  количество уже изменённых младших заказов
 * @param prepares  необходимость изменения счёта, после вычисления
 * @param mod       дополнительный параметр дельты
 */
void anyks::Toolkit::init(const algorithm_t algorithm, const bool modified, const bool prepares, const double mod){
	// Если память под arpa еще не выделена
	if(this->arpa == nullptr){
		// Экранируем возможность ошибки памяти
		try {
			/**
			 * getWordFn Функция получения слова по его идентификатору
			 * @param idw идентификатор слова
			 * @return    искомое слово
			 */
			auto getWordFn = [this](const size_t idw) noexcept {
				// Выводим результат
				return this->word(idw);
			};
			// Устанавливаем дополнительный параметр дельты
			this->params.mod = mod;
			// Устанавливаем необходимость изменения счёта, после вычисления
			this->params.prepares = prepares;
			// Устанавливаем количество уже изменённых младших заказов
			this->params.modified = modified;
			// Устанавливаем алгоритм сглаживания
			this->params.algorithm = (u_short) algorithm;
			// Определяем тип алгоритма расчёта языковой модели
			switch((size_t) algorithm){
				// Активируем алгоритм WittenBell
				case (size_t) algorithm_t::wittenBell: this->arpa = new WittenBell(alphabet, getWordFn); break;
				// Активируем алгоритм GoodTuring
				case (size_t) algorithm_t::goodTuring: this->arpa = new GoodTuring(alphabet, getWordFn); break;
				// Активируем алгоритм NaturalDiscount
				case (size_t) algorithm_t::naturalDiscount: this->arpa = new NaturalDiscount(alphabet, getWordFn); break;
				// Активируем алгоритм KneserNey
				case (size_t) algorithm_t::kneserNey: this->arpa = new KneserNey(alphabet, getWordFn, modified, prepares); break;
				// Активируем алгоритм AddSmooth
				case (size_t) algorithm_t::addSmooth: this->arpa = new AddSmooth(alphabet, getWordFn, (mod > 0.0 ? mod : 1.0)); break;
				// Активируем алгоритм ModKneserNey
				case (size_t) algorithm_t::modKneserNey: this->arpa = new ModKneserNey(alphabet, getWordFn, modified, prepares); break;
				// Активируем алгоритм ConstDiscount
				case (size_t) algorithm_t::constDiscount: this->arpa = new ConstDiscount(alphabet, getWordFn, (mod > 0.0 ? mod : 0.3)); break;
			}
			// Устанавливаем размер n-граммы
			this->arpa->setSize(this->size);
			// Устанавливаем файл логирования
			this->arpa->setLogfile(this->logfile);
			// Если скрипт получен
			if(((this->scripts.count(2) > 0) && !this->utokens.empty()) || (this->scripts.count(1) > 0)){
				// Создаём объект для работы с python
				if(this->python == nullptr) this->python = new python_t(this->tokenizer);
				// Блокируем поток
				this->locker.lock();
				// Если нужно активировать режим отладки
				if(this->isOption(options_t::debug)) this->python->setDebug();
				// Разблокируем поток
				this->locker.unlock();
				// Ищем скрипт обработки слов
				auto it = this->scripts.find(1);
				// Если скрипт обработки слов установлен
				if(it != this->scripts.end()){
					// Блокируем поток
					this->locker.lock();
					// Запоминаем идентификатор скрипта
					it->second.second = this->python->add(it->second.first, 2);
					// Разблокируем поток
					this->locker.unlock();
				}
				// Ищем скрипт обработки пользовательских токенов
				it = this->scripts.find(2);
				// Если скрипт обработки пользовательских токенов установлен
				if((it != this->scripts.end()) && !this->utokens.empty()){
					// Блокируем поток
					this->locker.lock();
					// Выполняем добавление скрипта
					const size_t sid = this->python->add(it->second.first, 2);
					// Разблокируем поток
					this->locker.unlock();
					// Переходим по всему списку пользовательских токенов
					for(auto & token : this->utokens){
						// Добавляем в пользовательский токен функцию проверки
						token.second.test = [sid, this](const string & token, const string & word){
							// Результат работы функции
							bool result = false;
							// Если слово и токен переданы
							if(!token.empty() && !word.empty()){
								// Блокируем поток
								this->locker.lock();
								// Выполняем скрипт
								const wstring & res = this->python->run(sid, {token, word});
								// Проверяем результат
								result = (this->alphabet->toLower(res).compare(L"ok") == 0);
								// Разблокируем поток
								this->locker.unlock();
							}
							// Выводим результат
							return result;
						};
					}
				}
			}
			// Если нужно активировать интерполяцию, активируем её
			if(this->isOption(options_t::interpolate)
			&& (algorithm != algorithm_t::constDiscount)
			&& (algorithm != algorithm_t::addSmooth)) this->arpa->setOption(arpa_t::options_t::interpolate);
			// Активируем флаг отладки если необходимо
			if(this->isOption(options_t::debug)) this->arpa->setOption(arpa_t::options_t::debug);
			// Активируем сброс веса токена неизвестного слова
			if(this->isOption(options_t::resetUnk)) this->arpa->setOption(arpa_t::options_t::resetUnk);
			// Активируем расчёт всех возможных грамм
			if(this->isOption(options_t::allGrams)) this->arpa->setOption(arpa_t::options_t::allGrams);
			// Активируем перевод слов в arpa к нижнему регистру
			if(this->isOption(options_t::lowerCase)) this->arpa->setOption(arpa_t::options_t::lowerCase);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выводим сообщение об ошибке, если режим отладки включён
			if(this->isOption(options_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "bad alloc for init language model");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * sweep Метод удаления низкочастотных n-грамм arpa
 * @param status статус расёта
 */
void anyks::Toolkit::sweep(function <void (const u_short)> status) noexcept {
	// Выполняем удаление низкочастотных n-грамм
	this->arpa->sweep(status);
}
/**
 * train Метод запуска обучения языковой модели
 * @param status функция вывода статуса обучения
 */
void anyks::Toolkit::train(function <void (const u_short)> status) noexcept {
	// Выполняем обучение arpa
	this->arpa->train(status);
}
/**
 * repair Метод ремонта уже расчитанной ранее arpa
 * @param status статус расёта
 */
void anyks::Toolkit::repair(function <void (const u_short)> status) noexcept {
	// Выполняем исправление arpa
	this->arpa->repair(status);
}
/**
 * pruneVocab Метод прунинга словаря
 * @param wltf   пороговый вес слова для прунинга
 * @param status статус прунинга словаря
 */
void anyks::Toolkit::pruneVocab(const double wltf, function <void (const u_short)> status) noexcept {
	// Если словарь не пустой
	if(!this->vocab.empty()){
		// Количество извлечённых слов
		size_t index = 0;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Переходим по всему списку слов
		for(auto it = this->vocab.begin(); it != this->vocab.end();){
			// Получаем метаданные слова
			auto meta = it->second.calc(this->info.ad, this->info.cw);
			// Если вес слова не ниже порогового значения
			if((wltf != 0.0) && (meta.wltf <= wltf)){
				// Выполняем удаление слова в arpa
				this->arpa->removeWord(it->first);
				// Удаляем слово в алфавите
				it = this->vocab.erase(it);
			// Увеличиваем значение итератора
			} else it++;
			// Если функция вывода статуса передана
			if(status != nullptr){
				// Увеличиваем количество записанных слов
				index++;
				// Выполняем расчёт текущего статуса
				actual = u_short(index / double(this->vocab.size()) * 100.0);
				// Если статус обновился
				if(actual != past){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус извлечения
					status(actual);
				}
			}
		}
		// Обновляем количество уникальных слов
		this->info.unq = this->vocab.size();
	}
}
/**
 * modify Метод модификации arpa
 * @param filename адрес файла для чтения
 * @param flag     флаг модификации arpa
 * @param status   функция вывода статуса модификации
 */
void anyks::Toolkit::modify(const string & filename, modify_t flag, function <void (const u_short)> status) noexcept {
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Последовательность для добавления
		vector <pair_t> seq;
		// Список полученных слов последовательности
		vector <wstring> words;
		// Список добавленных слов в словарь
		map <size_t, size_t> added;
		// Количество обработанных данных
		size_t index = 0, pos = 0;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		/**
		 * cleanAddedFn Функция очистки добавленных слов
		 */
		auto cleanAddedFn = [&added, this]() noexcept {
			// Если список добавленных слов существует
			if(!added.empty()){
				// Переходим по списку добавленных слов
				for(auto & item : added){
					// Ищем слово в словаре
					auto it = this->vocab.find(item.first);
					// Если слово найдено
					if(it != this->vocab.end()){
						// Получаем метаданные слова
						auto meta = it->second.getmeta();
						// Если встречаемости равны, удаляем слово
						if(meta.oc == item.second) this->vocab.erase(item.first);
						// Меняем вероятность
						else {
							// Уменьшаем встречаемость
							meta.oc -= item.second;
							// Устанавливаем обратно метаданные
							it->second.setmeta(meta);
						}
						// Уменьшаем общее количество добавленных слов
						this->info.cw -= item.second;
					}
				}
			}
		};
		/**
		 * organizationSeqFn Функция организации последовательности
		 * @param words список слов из которых нужно сделать последовательность
		 */
		auto organizationSeqFn = [&added, &seq, this](const vector <wstring> & words) noexcept {
			// Если список слов получен
			if(!words.empty()){
				// Идентификатор слова
				size_t idw = 0;
				// Полученное слово
				word_t word = L"";
				// Переходим по всему списку слов
				for(auto & item : words){
					// Получаем слово
					word = move(item);
					// Получаем идентификатор слова
					idw = this->getIdw(word);
					// Проверяем отсутствует ли слово в списке запрещённых слов
					if((idw > 0) && (idw < idw_t::NIDW) && (this->badwords.count(idw) < 1)){
						// Если это неизвестное слово
						if(idw == uid){
							// Если неизвестное слово не установлено
							if(this->unknown == 0) seq.emplace_back(idw, 0);
							// Если неизвестное слово установлено
							else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
						// Добавляем слово в список
						} else {
							// Получаем регистры слова
							size_t uppers = word.getUppers();
							// Если название токена получено, устанавливаем его регистры
							if(this->utokens.count(idw) > 0) uppers = 0;
							// Добавляем слово в список последовательности
							seq.emplace_back(idw, uppers);
							// Если это правильное слово
							if(this->arpa->event(idw) && (this->vocab.count(idw) < 1)){
								// Если слово еще не добавлено
								if(added.count(idw) < 1)
									// Запоминаем что слово добавили
									added.emplace(idw, 1);
								// Иначе увеличиваем количество слов
								else added.at(idw)++;
								// Добавляем слово в словарь
								this->addWord(word.wreal(), idw);
							}
						}
					// Если слово найдено в всписке запрещённых
					} else {
						// Очищаем последовательность
						seq.clear();
						// Выходим из цикла
						break;
					}
				}
			}
		};
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
			// Если слово получено
			if(!text.empty()){
				// Определяем тип модификации
				switch((u_short) flag){
					// Если это изменение n-грамм
					case (u_short) modify_t::change:
					// Если это добавление в файл n-грамм
					case (u_short) modify_t::emplace: {
						// Считываем данные слова
						if((pos = text.find("\t")) != string::npos){
							// Очищаем последовательность
							seq.clear();
							// Считываем частоту n-граммы
							const string & weight = this->alphabet->trim(text.substr(0, pos));
							// Считываем данные n-граммы
							const string & ngram = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
							// Если частота и n-грамма получены
							if(!weight.empty() && !ngram.empty()){
								// Выполняем сплит n-грамм
								this->alphabet->split(ngram, " ", words);
								// Если список слов получен
								if(!words.empty()){
									// Формируем последовательность
									organizationSeqFn(words);
									// Добавляем последовательность в словарь
									if(!seq.empty()){
										// Если это добавления последовательности в arpa
										if(flag == modify_t::emplace){
											// Если добавление не выполнено
											if(!this->arpa->emplace(seq, stod(weight))) cleanAddedFn();
										// Если это изменение частот последовательности
										} else if(flag == modify_t::change) {
											// Выполняем инкрементацию частот
											this->arpa->inc(seq, stod(weight));
											// Выполняем неверно-добавленных слов
											cleanAddedFn();
										}
										// Очищаем список добавленных слов
										added.clear();
									}
								}
							}
						}
					} break;
					// Если это удаление из файла n-грамм
					case (u_short) modify_t::remove: {
						// Выполняем сплит n-грамм
						this->alphabet->split(text, " ", words);
						// Если список слов получен
						if(!words.empty()){
							// Идентификатор слова
							size_t idw = 0;
							// Последовательность для добавления
							vector <size_t> seq(words.size());
							// Переходим по всему списку слов
							for(size_t i = 0; i < words.size(); i++){
								// Определяем идентификатор слова
								idw = this->getIdw(words[i]);
								// Если идентификатор получен
								if((idw > 0) && (idw < idw_t::NIDW)) seq[i] = idw;
								// Если слово не получено, выходим
								else {
									// Очищаем список слов
									words.clear();
									// Выходим из цикла
									break;
								}
							}
							// Удаляем последовательность из словаря
							if(!words.empty()) this->arpa->del(seq);
						}
					} break;
					// Если это замена n-грамм
					case (u_short) modify_t::replace: {
						// Считываем данные слова
						if((pos = text.find("\t")) != string::npos){
							// Очищаем последовательность
							seq.clear();
							// Считываем n-грамму которую нужно заменить
							const string & fromNgram = this->alphabet->trim(text.substr(0, pos));
							// Считываем n-грамму на которую нужно заменить
							const string & toNgram = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
							// Если частота и n-грамма получены
							if(!fromNgram.empty() && !toNgram.empty()){
								// Выполняем сплит n-грамм
								this->alphabet->split(fromNgram, " ", words);
								// Если список слов получен
								if(!words.empty()){
									// Идентификатор слова
									size_t idw = 0;
									// Последовательность для добавления
									vector <size_t> seq2(words.size());
									// Переходим по всему списку слов
									for(size_t i = 0; i < words.size(); i++){
										// Определяем идентификатор слова
										idw = this->getIdw(words[i]);
										// Если идентификатор получен
										if((idw > 0) && (idw < idw_t::NIDW)){
											// Устанавливаем идентификатор слова
											seq2[i] = idw;
											// Если слова нет в словаре и это не токен, добавляем в словарь
											if(this->arpa->event(idw) && (this->vocab.count(idw) < 1)){
												// Если слово еще не добавлено
												if(added.count(idw) < 1)
													// Запоминаем что слово добавили
													added.emplace(idw, 1);
												// Иначе увеличиваем количество слов
												else added.at(idw)++;
												// Добавляем слово в словарь
												this->addWord(words.at(i), idw);
											}
										// Если слово не получено, выходим
										} else {
											// Очищаем список слов
											words.clear();
											// Выходим из цикла
											break;
										}
									}
									// Выполняем сплит n-грамм
									if(!words.empty()) this->alphabet->split(toNgram, " ", words);
									// Если список слов получен
									if(!words.empty()){
										// Формируем последовательность
										organizationSeqFn(words);
										// Добавляем последовательность в словарь
										if(!seq.empty() && !seq2.empty()){
											// Если замена не удалась
											if(!this->arpa->replace(seq2, seq)) cleanAddedFn();
											// Очищаем список добавленных слов
											added.clear();
										}
									}
								}
							}
						}
					} break;
				}
			}
			// Если функция вывода статуса передана
			if(status != nullptr){
				// Увеличиваем количество записанных n-грамм
				index += text.size();
				// Выполняем расчёт текущего статуса
				actual = u_short(index / double(fileSize) * 100.0);
				// Если статус обновился
				if(actual != past){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус извлечения
					status(actual);
				}
			}
		});
		// Выполняем исправление arpa
		this->arpa->repair(status);
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "modify file is not exist");
}
/**
 * prune Метод прунинга языковой модели
 * @param threshold порог частоты прунинга
 * @param mingram   значение минимальной n-граммы за которую нельзя прунить
 * @param status    функция вывода статуса обучения
 */
void anyks::Toolkit::prune(const double threshold, const u_short mingram, function <void (const u_short)> status) const noexcept {
	// Выполняем прунинг arpa
	this->arpa->prune(threshold, mingram, status);
}
/**
 * mix Метод интерполяции нескольких arpa
 * @param filenames список файлов arpa для объединения
 * @param lambdas   список весов первой модели при интерполяции
 * @param backward  необходимо выполнить интерполяцию в обратном направлении
 * @param status    функция вывода статуса чтения
 */
void anyks::Toolkit::mix(const vector <string> & filenames, const vector <double> & lambdas, const bool backward, function <void (const string &, const u_short)> status) noexcept {
	// Если адреса файлов и лямбды переданы
	if(!filenames.empty() && !lambdas.empty() && (filenames.size() == (lambdas.size() + 1))){
		// Тип извлекаемых данных
		u_short type = 0;
		// Общий размер данных и количество обработанных данных
		size_t maxSize = 0, index = 0;
		// Список языковых моделей
		map <u_short, pair <string, arpa_t *>> arpas;
		/**
		 * getWordFn Функция получения слова по его идентификатору
		 * @param idw идентификатор слова
		 * @return    искомое слово
		 */
		auto getWordFn = [this](const size_t idw) noexcept {
			// Выводим результат
			return this->word(idw);
		};
		/**
		 * loadFn Функция загрузки текстовых данных языковой модели
		 * @param arpa объект arpa для добавления
		 * @param text текст для парсинга данных arpa
		 */
		auto loadFn = [&type, this](arpa_t * arpa, const string & text) noexcept {
			// Если слово получено
			if(!text.empty() && (arpa != nullptr)){
				// Последовательность для добавления
				vector <pair_t> seq;
				// Список полученных слов последовательности
				vector <wstring> words;
				// Позиции в текстовых данных
				size_t pos = 0, loc = 0;
				// Идентификатор неизвестного слова
				const size_t uid = (size_t) token_t::unk;
				// Определяем тип считываемых данных
				switch(type){
					// Если получено обозначение заголовка
					case 0: if(text.find("\\data\\") != string::npos) type++; break;
					// Если мы дошли до считывания данных слов
					case 1: if(text.rfind("-grams:") != string::npos) type++; break;
					// Если это тип считывания данных n-грамм
					case 2: {
						// Если мы дошли до конца, выходим
						if(text.find("\\end\\") != string::npos) break;
						// Иначе считываем данные n-граммы
						else if((pos = text.find("\t")) != string::npos){
							// Обнуляем локальную позицию
							loc = 0;
							// Данные n-граммы и обратной частоты документа
							string ngram = "", backoff = "0.0";
							// Считываем частоту n-граммы
							const string & weight = this->alphabet->trim(text.substr(loc, pos));
							// Запоминаем текущую позицию
							loc = pos;
							// Ищем значение n-граммы
							if((pos = text.find("\t", pos + 1)) != string::npos){
								// Извлекаем данные n-граммы
								ngram = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
								// Излвлекаем обратную частоту документа
								backoff = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
							// Извлекаем данные n-граммы
							} else ngram = this->alphabet->trim(text.substr(loc + 1, text.length() - (loc + 1)));
							// Если данные получены
							if(!ngram.empty() && !weight.empty()){
								// Выполняем сплит n-грамм
								this->alphabet->split(ngram, " ", words);
								// Если список слов получен
								if(!words.empty()){
									// Идентификатор слова
									size_t idw = 0;
									// Полученное слово
									word_t word = L"";
									// Переходим по всему списку слов
									for(auto & item : words){
										// Получаем слово
										word = move(item);
										// Получаем идентификатор слова
										idw = this->getIdw(word, !this->isOption(options_t::confidence));
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(this->badwords.count(idw) < 1){
											// Если это неизвестное слово
											if(uid == idw){
												// Если неизвестное слово не установлено
												if(this->unknown == 0) seq.emplace_back(idw, 0);
												// Если неизвестное слово установлено
												else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
											// Добавляем слово в список последовательности
											} else seq.emplace_back(idw, ((words.size() > 1) && (this->utokens.count(idw) > 0) ? 0 : word.getUppers()));
										// Если слово найдено в всписке запрещённых
										} else {
											// Очищаем последовательность
											seq.clear();
											// Выходим из цикла
											break;
										}
									}
									// Если последовательность получена
									if(!seq.empty() && !weight.empty()){
										// Получаем размер n-граммы
										const size_t size = seq.size();
										// Устанавливаем новый размер n-грамм для arpa
										arpa->setSize(size);
										// Если количество собранных n-грамм выше установленных, меняем
										if(size > size_t(this->size)) this->size = size;
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(arpa->event(idw) && (this->badwords.count(idw) < 1)){
											// Добавляем слово в словарь
											this->addWord(word.wreal(), idw);
										}
										// Добавляем последовательность в словарь
										arpa->set(seq, stod(weight), stod(backoff));
									}
								}
							}
						}
					} break;
				}
			}
		};
		// Экранируем возможность ошибки памяти
		try {
			// Переходим по всем файлам arpa
			for(auto & filename : filenames){
				// Если файл существует
				if(fsys_t::isfile(filename)){
					// Получаем размер файла
					maxSize += fsys_t::fsize(filename);
					// Если список языковых моделей пуст, добавляем текущую языковую модель
					if(arpas.empty()){
						// Если языковая модель не создана
						if(this->arpa == nullptr){
							// Создаем объект языковой модели
							this->arpa = new arpa_t(this->alphabet, getWordFn);
							// Устанавливаем размер n-граммы
							this->arpa->setSize(this->size);
							// Устанавливаем файл логирования
							this->arpa->setLogfile(this->logfile);
							// Активируем флаг отладки если необходимо
							if(this->isOption(options_t::debug)) this->arpa->setOption(arpa_t::options_t::debug);
							// Активируем сброс веса токена неизвестного слова
							if(this->isOption(options_t::resetUnk)) this->arpa->setOption(arpa_t::options_t::resetUnk);
							// Активируем перевод слов в arpa к нижнему регистру
							if(this->isOption(options_t::lowerCase)) this->arpa->setOption(arpa_t::options_t::lowerCase);
						}
						// Добавляем основную языковую модель в список
						arpas.emplace(index, make_pair(filename, this->arpa));
					// Иначе создаём языковую модель динамически
					} else {
						// Создаем объект языковой модели
						arpa_t * arpa = new arpa_t(this->alphabet, getWordFn);
						// Добавляем в список новую языковую модель
						auto ret = arpas.emplace(index, make_pair(filename, arpa));
						// Устанавливаем размер n-граммы
						ret.first->second.second->setSize(this->size);
						// Устанавливаем файл логирования
						ret.first->second.second->setLogfile(this->logfile);
						// Активируем флаг отладки если необходимо
						if(this->isOption(options_t::debug)) ret.first->second.second->setOption(arpa_t::options_t::debug);
						// Активируем сброс веса токена неизвестного слова
						if(this->isOption(options_t::resetUnk)) ret.first->second.second->setOption(arpa_t::options_t::resetUnk);
						// Активируем перевод слов в arpa к нижнему регистру
						if(this->isOption(options_t::lowerCase)) ret.first->second.second->setOption(arpa_t::options_t::lowerCase);
					}
					// Увеличиваем индекс обработанных данных
					index++;
				}
			}
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выводим сообщение об ошибке, если режим отладки включён
			if(this->isOption(options_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "bad alloc for mix language models");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
		// Если список языковых моделей получен
		if(!arpas.empty() && (maxSize > 0)){
			// Обнуляем значение индекса
			index = 0;
			// Получаем значение лямбды
			double lambda = (1.0 - lambdas.at(0));
			// Перебираем все остальные значения ламбды
			for(size_t i = 1; i < lambdas.size(); i++) lambda -= lambdas.at(i);
			// Получаем суммарное значение лямбд
			double sumLambdas = lambda;
			// Переходим по всем файлам arpa
			for(auto & item : arpas){
				// Обнуляем тип считываемых данных
				type = 0;
				// Выполняем считывание всех строк текста
				fsys_t::rfile(item.second.first, [&](const string & text, const uintmax_t fileSize) noexcept {
					// Если текст получен, загружаем в языковую модель
					if(!text.empty()) loadFn(item.second.second, text);
				});
				// Если это не основная языковая модель
				if(index > 0){
					// Получаем сумму лямбд
					sumLambdas += lambdas.at(index - 1);
					// Если нужно производить интерполяцию в прямом направлении
					if(!backward){
						// Выполняем объединение языковых моделей
						this->arpa->mixForward(item.second.second, (1.0 - ((lambda == 0.0) ? 0.0 : lambda / sumLambdas)), [&item, &status](const u_short actual){
							// Выводим результат объединения
							if(actual < 100) status(item.second.first, actual);
						});
					// Если нужно произвести интерполяцию в обратном направлении
					} else {
						// Выполняем объединение языковых моделей
						this->arpa->mixBackward(item.second.second, (1.0 - ((lambda == 0.0) ? 0.0 : lambda / sumLambdas)), [&item, &status](const u_short actual){
							// Выводим результат объединения
							if(actual < 100) status(item.second.first, actual);
						});
					}
				}
				// Увеличиваем индекс обработанных данных
				index++;
			}
			// Обновляем количество уникальных слов
			this->info.unq = this->vocab.size();
			// Очищаем выделенную ранее память
			for(size_t i = 1; i < arpas.size(); i++) delete arpas.at(i).second;
		}
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "arpa file is not exist");
}
/**
 * mix Метод интерполяции нескольких arpa алгоритмами (Баеса и Логарифмическо-линейным)
 * @param filenames список файлов arpa для объединения
 * @param lambdas   список весов первой модели при интерполяции
 * @param length    байесовская интерполяция с учетом контекста длины length
 * @param scale     логарифмическая шкала вероятности для алгоритма Баеса
 * @param status    функция вывода статуса чтения
 */
void anyks::Toolkit::mix(const vector <string> & filenames, const vector <double> & lambdas, const size_t length, const double scale, function <void (const string &, const u_short)> status) noexcept {
	// Если адреса файлов и лямбды переданы
	if(!filenames.empty() && !lambdas.empty() && (filenames.size() == lambdas.size())){
		// Тип извлекаемых данных
		u_short type = 0;
		// Общий размер данных и количество обработанных данных
		size_t maxSize = 0, index = 0;
		// Список языковых моделей
		map <u_short, pair <string, arpa_t *>> arpas;
		/**
		 * getWordFn Функция получения слова по его идентификатору
		 * @param idw идентификатор слова
		 * @return    искомое слово
		 */
		auto getWordFn = [this](const size_t idw) noexcept {
			// Выводим результат
			return this->word(idw);
		};
		/**
		 * loadFn Функция загрузки текстовых данных языковой модели
		 * @param arpa объект arpa для добавления
		 * @param text текст для парсинга данных arpa
		 */
		auto loadFn = [&type, this](arpa_t * arpa, const string & text) noexcept {
			// Если слово получено
			if(!text.empty() && (arpa != nullptr)){
				// Последовательность для добавления
				vector <pair_t> seq;
				// Список полученных слов последовательности
				vector <wstring> words;
				// Позиции в текстовых данных
				size_t pos = 0, loc = 0;
				// Идентификатор неизвестного слова
				const size_t uid = (size_t) token_t::unk;
				// Определяем тип считываемых данных
				switch(type){
					// Если получено обозначение заголовка
					case 0: if(text.find("\\data\\") != string::npos) type++; break;
					// Если мы дошли до считывания данных слов
					case 1: if(text.rfind("-grams:") != string::npos) type++; break;
					// Если это тип считывания данных n-грамм
					case 2: {
						// Если мы дошли до конца, выходим
						if(text.find("\\end\\") != string::npos) break;
						// Иначе считываем данные n-граммы
						else if((pos = text.find("\t")) != string::npos){
							// Обнуляем локальную позицию
							loc = 0;
							// Данные n-граммы и обратной частоты документа
							string ngram = "", backoff = "0.0";
							// Считываем частоту n-граммы
							const string & weight = this->alphabet->trim(text.substr(loc, pos));
							// Запоминаем текущую позицию
							loc = pos;
							// Ищем значение n-граммы
							if((pos = text.find("\t", pos + 1)) != string::npos){
								// Извлекаем данные n-граммы
								ngram = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
								// Излвлекаем обратную частоту документа
								backoff = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
							// Извлекаем данные n-граммы
							} else ngram = this->alphabet->trim(text.substr(loc + 1, text.length() - (loc + 1)));
							// Если данные получены
							if(!ngram.empty() && !weight.empty()){
								// Выполняем сплит n-грамм
								this->alphabet->split(ngram, " ", words);
								// Если список слов получен
								if(!words.empty()){
									// Идентификатор слова
									size_t idw = 0;
									// Полученное слово
									word_t word = L"";
									// Переходим по всему списку слов
									for(auto & item : words){
										// Получаем слово
										word = move(item);
										// Получаем идентификатор слова
										idw = this->getIdw(word, !this->isOption(options_t::confidence));
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(this->badwords.count(idw) < 1){
											// Если это неизвестное слово
											if(uid == idw){
												// Если неизвестное слово не установлено
												if(this->unknown == 0) seq.emplace_back(idw, 0);
												// Если неизвестное слово установлено
												else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
											// Добавляем слово в список последовательности
											} else seq.emplace_back(idw, ((words.size() > 1) && (this->utokens.count(idw) > 0) ? 0 : word.getUppers()));
										// Если слово найдено в всписке запрещённых
										} else {
											// Очищаем последовательность
											seq.clear();
											// Выходим из цикла
											break;
										}
									}
									// Если последовательность получена
									if(!seq.empty() && !weight.empty()){
										// Получаем размер n-граммы
										const size_t size = seq.size();
										// Устанавливаем новый размер n-грамм для arpa
										arpa->setSize(size);
										// Если количество собранных n-грамм выше установленных, меняем
										if(size > size_t(this->size)) this->size = size;
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(arpa->event(idw) && (this->badwords.count(idw) < 1)){
											// Добавляем слово в словарь
											this->addWord(word.wreal(), idw);
										}
										// Добавляем последовательность в словарь
										arpa->set(seq, stod(weight), stod(backoff));
									}
								}
							}
						}
					} break;
				}
			}
		};
		// Экранируем возможность ошибки памяти
		try {
			// Переходим по всем файлам arpa
			for(auto & filename : filenames){
				// Если файл существует
				if(fsys_t::isfile(filename)){
					// Получаем размер файла
					maxSize += fsys_t::fsize(filename);
					// Если список языковых моделей пуст, добавляем текущую языковую модель
					if(arpas.empty()){
						// Если языковая модель не создана
						if(this->arpa == nullptr){
							// Создаем объект языковой модели
							this->arpa = new arpa_t(this->alphabet, getWordFn);
							// Устанавливаем размер n-граммы
							this->arpa->setSize(this->size);
							// Устанавливаем файл логирования
							this->arpa->setLogfile(this->logfile);
							// Активируем флаг отладки если необходимо
							if(this->isOption(options_t::debug)) this->arpa->setOption(arpa_t::options_t::debug);
							// Активируем сброс веса токена неизвестного слова
							if(this->isOption(options_t::resetUnk)) this->arpa->setOption(arpa_t::options_t::resetUnk);
							// Активируем перевод слов в arpa к нижнему регистру
							if(this->isOption(options_t::lowerCase)) this->arpa->setOption(arpa_t::options_t::lowerCase);
						}
						// Добавляем основную языковую модель в список
						arpas.emplace(index, make_pair(filename, this->arpa));
					// Иначе создаём языковую модель динамически
					} else {
						// Создаем объект языковой модели
						arpa_t * arpa = new arpa_t(this->alphabet, getWordFn);
						// Добавляем в список новую языковую модель
						auto ret = arpas.emplace(index, make_pair(filename, arpa));
						// Устанавливаем размер n-граммы
						ret.first->second.second->setSize(this->size);
						// Устанавливаем файл логирования
						ret.first->second.second->setLogfile(this->logfile);
						// Активируем флаг отладки если необходимо
						if(this->isOption(options_t::debug)) ret.first->second.second->setOption(arpa_t::options_t::debug);
						// Активируем сброс веса токена неизвестного слова
						if(this->isOption(options_t::resetUnk)) ret.first->second.second->setOption(arpa_t::options_t::resetUnk);
						// Активируем перевод слов в arpa к нижнему регистру
						if(this->isOption(options_t::lowerCase)) ret.first->second.second->setOption(arpa_t::options_t::lowerCase);
					}
					// Увеличиваем индекс обработанных данных
					index++;
				}
			}
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выводим сообщение об ошибке, если режим отладки включён
			if(this->isOption(options_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "bad alloc for mix language models");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
		// Если список языковых моделей получен
		if(!arpas.empty() && (maxSize > 0)){
			// Обнуляем значение индекса
			index = 0;
			// Список языковых моделей для интерполяции
			vector <const arpa_t *> lms;
			// Текущий и предыдущий статус
			u_short actual = 0, past = 100;
			// Переходим по всем файлам arpa
			for(auto & item : arpas){
				// Обнуляем тип считываемых данных
				type = 0;
				// Выполняем считывание всех строк текста
				fsys_t::rfile(item.second.first, [&](const string & text, const uintmax_t fileSize) noexcept {
					// Если текст получен, загружаем в языковую модель
					if(!text.empty()) loadFn(item.second.second, text);
				});
				// Добавляем языковую модель в список для интерполяции
				if(index > 0) lms.push_back(item.second.second);
				// Увеличиваем индекс обработанных данных
				index++;
				// Если функция вывода статуса передана
				if(status != nullptr){
					// Выполняем расчёт текущего статуса
					actual = u_short(index / double(arpas.size()) * 100.0);
					// Если статус обновился
					if(actual != past){
						// Запоминаем текущий статус
						past = actual;
						// Выводим статус извлечения
						if(actual < 100) status(item.second.first, actual);
					}
				}
			}
			// Если нужно выполнить интерполяцию методом Баеса
			if(length > 0){
				// Выполняем интерполяцию алгоритмом Баеса
				this->arpa->mixBayes(lms, lambdas, length, scale, [&status](const u_short value){
					// Выводим результат объединения
					status("", value);
				});
			// Если нужно выполнить интерполяцию методом логарифмическо-линейным
			} else {
				// Выполняем интерполяцию Логарифмическо-линейным алгоритмом
				this->arpa->mixLoglinear(lms, lambdas, [&status](const u_short value){
					// Выводим результат объединения
					status("", value);
				});
			}
			// Обновляем количество уникальных слов
			this->info.unq = this->vocab.size();
			// Очищаем выделенную ранее память
			for(size_t i = 1; i < arpas.size(); i++) delete arpas.at(i).second;
		}
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "arpa file is not exist");
}
/**
 * loadVocab Метод загрузки бинарных данных в словарь
 * @param buffer буфер с бинарными данными
 * @param arpa   нужно добавить только данные arpa
 */
void anyks::Toolkit::loadVocab(const vector <char> & buffer) noexcept {
	// Если буфер слова передан
	if(!buffer.empty()){
		// Идентификатор слова и смещение в буфере
		size_t idw = 0, offset = 0;
		// Извлекаем бинарные данные буфера
		const char * data = buffer.data();
		// Извлекаем идентификатор слова
		memcpy(&idw, data + offset, sizeof(idw));
		// Увеличиваем смещение
		offset += sizeof(idw);
		// Если идентификатор слова получен
		if((idw > 0) && (idw < idw_t::NIDW)){
			// Слово для добавления в словарь
			word_t word = L"";
			// Добавляем бинарные данные слова
			word.set(data + offset, buffer.size() - offset);
			// Добавляем слово в словарь
			if(!word.empty()) this->vocab.emplace(idw, move(word));
			// Запоминаем количество уникальных слов
			this->info.unq = this->vocab.size();
		}
	}
}
/**
 * loadInfoVocab Метод загрузки бинарных информационных данных словаря
 * @param buffer буфер бинарных информационных данных словаря
 */
void anyks::Toolkit::loadInfoVocab(const vector <char> & buffer) noexcept {
	// Если буфер слова передан
	if(!buffer.empty()){
		// Загружаем информационные данные словаря
		memcpy(&this->info, buffer.data(), sizeof(this->info));
		// Сбрасываем идентификатор последнего документа
		this->info.idd = idw_t::NIDW;
	}
}
/**
 * load Метод загрузки бинарных данных n-грамм в словарь
 * @param buffer буфер с бинарными данными
 * @param arpa   нужно добавить только данные arpa
 */
void anyks::Toolkit::loadArpa(const vector <char> & buffer, const bool arpa) noexcept {
	// Выполняем загрузку бинарных данных
	this->arpa->setBin(buffer, arpa);
}
/**
 * appendArpa Метод добавления бинарных данных в словарь
 * @param buffer буфер с бинарными данными
 * @param idd    идентификатор документа в котором получена n-грамма
 */
void anyks::Toolkit::appendArpa(const vector <char> & buffer, const size_t idd) const noexcept {
	// Выполняем добавление бинарных данных
	this->arpa->addBin(buffer, idd);
}
/**
 * saveInfoVocab Метод сохранения бинарных информационных данных словаря
 * @param buffer буфер бинарных информационных данных словаря
 */
void anyks::Toolkit::saveInfoVocab(vector <char> & buffer) const noexcept {
	// Очищаем переданный буфер данных
	buffer.clear();
	// Получаем бинарные информационные данные словаря
	const char * bin = reinterpret_cast <const char *> (&this->info);
	// Добавляем в буфер бинарные информационные данные словаря
	buffer.insert(buffer.end(), bin, bin + sizeof(this->info));
}
/**
 * saveVocab Метод извлечения данных словаря в бинарном виде
 * @param callback функция обратного вызова
 */
void anyks::Toolkit::saveVocab(function <void (const vector <char> &, const u_short)> callback) const noexcept {
	// Если словарь не пустой
	if(!this->vocab.empty()){
		// Индекс текущего слова
		size_t index = 0;
		// Буфер данных слова
		vector <char> buffer;
		// Переходим по всему списку слов
		for(auto & item : this->vocab){
			// Извлекаем бинарные данные слова
			auto data = item.second.dump();
			// Получаем бинарные данные идентификатора слова
			const char * bin = reinterpret_cast <const char *> (&item.first);
			// Добавляем в буфер идентификатор слова
			buffer.insert(buffer.end(), bin, bin + sizeof(item.first));
			// Добавляем в буфер бинарные данные самого слова
			buffer.insert(buffer.end(), data.begin(), data.end());
			// Выводим результат
			callback(buffer, (index / double(this->vocab.size()) * 100.0));
			// Очищаем собранный буфер данных
			buffer.clear();
			// Увеличиваем индекс слова
			index++;
		}
	}
}
/**
 * writeArpa Метод записи данных в файл arpa
 * @param filename адрес файла для записи
 * @param status   функция вывода статуса записи
 */
void anyks::Toolkit::writeArpa(const string & filename, function <void (const u_short)> status) const noexcept {
	// Если адрес файла передан
	if(!filename.empty()){
		// Открываем файл на запись
		ofstream file(filename, ios::binary);
		// Если файл открыт
		if(file.is_open()){
			// Данные данных для записи
			string data = "";
			// Текущий и предыдущий статус
			u_short actual = 0, past = 100;
			// Количество n-грамм и количество всех n-грамм
			size_t count = 0, counts = 0, index = 0;
			// Получаем штамп файла
			const string & stamp = this->arpa->stamp();
			// Выполняем запись штампа в файл
			file.write(stamp.data(), stamp.size());
			// Формируем начало документа
			data = "\\data\\\n";
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Переходим по всей длине n-граммы
			for(u_short i = 1; i <= this->size; i++){
				// Получаем количество n-грамм
				count = this->arpa->count(i);
				// Формируем количество n-грамм
				data = this->alphabet->format("ngram %u=%u\n", i, count);
				// Выполняем запись данных в файл
				file.write(data.data(), data.size());
				// Увеличиваем количество всех n-грамм
				counts += count;
			}
			// Если количество элементов больше 0
			if(counts > 0){
				// Переходим по всей длине n-граммы
				for(u_short i = 1; i <= this->size; i++){
					// Получаем количество n-грамм
					data = this->alphabet->format("\n\\%u-grams:\n", i);
					// Выполняем запись данных в файл
					file.write(data.data(), data.size());
					// Выполняем извлечение данных n-граммы
					this->arpa->arpa(i, [&](const string & ngram){
						// Если строка получена
						if(!ngram.empty()){
							// Формируем тело n-граммы
							data = this->alphabet->format("%s\n", ngram.c_str());
							// Выполняем запись данных в файл
							file.write(data.data(), data.size());
						}
						// Если функция вывода статуса передана
						if(status != nullptr){
							// Увеличиваем количество записанных n-грамм
							index++;
							// Выполняем расчёт текущего статуса
							actual = u_short(index / double(counts) * 100.0);
							// Если статус обновился
							if(actual != past){
								// Запоминаем текущий статус
								past = actual;
								// Выводим статус извлечения
								status(actual);
							}
						}
					});
				}
			}
			// Формируем конец документа
			data = "\n\\end\\\n";
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Закрываем файл
			file.close();
		// Выводим сообщение об ошибке
		} else this->alphabet->log("arpa file: %s is broken", alphabet_t::log_t::error, this->logfile, filename.c_str());
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "arpa file is empty");
}
/**
 * writeVocab Метод записи данных словаря в файл
 * @param filename адрес файла для записи
 * @param status   функция вывода статуса записи
 */
void anyks::Toolkit::writeVocab(const string & filename, function <void (const u_short)> status) const noexcept {
	// Если адрес файла передан
	if(!filename.empty()){
		// Открываем файл на запись
		ofstream file(filename, ios::binary);
		// Если файл открыт
		if(file.is_open()){
			// Данные данных для записи
			string data = "";
			// Количество извлечённых слов
			size_t index = 0;
			// Текущий и предыдущий статус
			u_short actual = 0, past = 100;
			// Получаем штамп файла
			const string & stamp = this->arpa->stamp();
			// Выполняем запись штампа в файл
			file.write(stamp.data(), stamp.size());
			// Формируем значение заголовка
			data = "\\data\\\n";
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Получаем характеристики словаря
			data = this->alphabet->format("ad=%lld\ncw=%lld\nunq=%lld\n\n", this->info.ad, this->info.cw, this->info.unq);
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Формируем значение тела
			data = "\\words:\n";
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Переходим по всему списку слов
			for(auto & item : this->vocab){
				// Получаем метаданные слова
				const auto & meta = item.second.calc(this->info.ad, this->info.cw);
				// Получаем слово для экспорта
				const string & word = (this->isOption(options_t::lowerCase) ? item.second.str() : item.second.real());
				// Получаем данные слова
				data = this->alphabet->format("%lld\t%s\t%lld | %lld | %f | %f | %f\n", item.first, word.c_str(), meta.oc, meta.dc, meta.tf, meta.idf, meta.wltf);
				// Выполняем запись данных в файл
				file.write(data.data(), data.size());
				// Если функция вывода статуса передана
				if(status != nullptr){
					// Увеличиваем количество записанных слов
					index++;
					// Выполняем расчёт текущего статуса
					actual = u_short(index / double(this->info.unq) * 100.0);
					// Если статус обновился
					if(actual != past){
						// Запоминаем текущий статус
						past = actual;
						// Выводим статус извлечения
						status(actual);
					}
				}
			}
			// Формируем значение тела
			data = "\n\\end\\\n";
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Закрываем файл
			file.close();
		// Выводим сообщение об ошибке
		} else this->alphabet->log("vocab file: %s is broken", alphabet_t::log_t::error, this->logfile, filename.c_str());
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "vocab file is empty");
}
/**
 * writeWords Метод записи данных слов в файл
 * @param filename адрес файла для записи
 * @param status   функция вывода статуса записи
 */
void anyks::Toolkit::writeWords(const string & filename, function <void (const u_short)> status) const noexcept {
	// Если адрес файла передан
	if(!filename.empty() && !this->vocab.empty()){
		// Открываем файл на запись
		ofstream file(filename, ios::binary);
		// Если файл открыт
		if(file.is_open()){
			// Количество извлечённых слов
			size_t index = 0;
			// Текущий и предыдущий статус
			u_short actual = 0, past = 100;
			// Список отсортированных слов
			multimap <double, string> words;
			// Переходим по всему списку слов
			for(auto & item : this->vocab){
				// Получаем метаданные слова
				const auto & meta = item.second.calc(this->info.ad, this->info.cw);
				// Добавляем в список слов полученное слово
				words.emplace(meta.wltf, (this->isOption(options_t::lowerCase) ? item.second.str() : item.second.real()));
				// Если функция вывода статуса передана
				if(status != nullptr){
					// Увеличиваем количество записанных слов
					index++;
					// Выполняем расчёт текущего статуса
					actual = u_short(index / double(this->info.unq * 2) * 100.0);
					// Если статус обновился
					if(actual != past){
						// Запоминаем текущий статус
						past = actual;
						// Выводим статус извлечения
						status(actual);
					}
				}
			}
			// Получаем штамп файла
			const string & stamp = this->arpa->stamp();
			// Выполняем запись штампа в файл
			file.write(stamp.data(), stamp.size());
			// Переходим по всему списку слов
			for(auto it = words.crbegin(); it != words.crend(); ++it){
				// Получаем данные слова
				const string & word = this->alphabet->format("%s\n", it->second.c_str());
				// Выполняем запись данных в файл
				file.write(word.data(), word.size());
				// Если функция вывода статуса передана
				if(status != nullptr){
					// Увеличиваем количество записанных слов
					index++;
					// Выполняем расчёт текущего статуса
					actual = u_short(index / double(this->info.unq * 2) * 100.0);
					// Если статус обновился
					if(actual != past){
						// Запоминаем текущий статус
						past = actual;
						// Выводим статус извлечения
						status(actual);
					}
				}
			}
			// Закрываем файл
			file.close();
		// Выводим сообщение об ошибке
		} else this->alphabet->log("words file: %s is broken", alphabet_t::log_t::error, this->logfile, filename.c_str());
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "words file is empty");
}
/**
 * writeNgrams Метод записи данных в файлы ngrams
 * @param filename адрес файла для записи
 * @param status   функция вывода статуса записи
 */
void anyks::Toolkit::writeNgrams(const string & filename, function <void (const u_short)> status) const noexcept {
	// Если адрес файла передан
	if(!filename.empty()){
		// Открываем файл на запись
		ofstream file(filename, ios::binary);
		// Если файл открыт
		if(file.is_open()){
			// Данные данных для записи
			string data = "";
			// Текущий и предыдущий статус
			u_short actual = 0, past = 100;
			// Количество n-грамм и количество всех n-грамм
			size_t count = 0, counts = 0, index = 0;
			// Получаем штамп файла
			const string & stamp = this->arpa->stamp();
			// Выполняем запись штампа в файл
			file.write(stamp.data(), stamp.size());
			// Формируем значение заголовка
			data = "\\data\\\n";
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Получаем характеристики словаря
			data = this->alphabet->format("ad=%lld\ncw=%lld\nunq=%lld\n\n", this->info.ad, this->info.cw, this->info.unq);
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Переходим по всей длине n-граммы
			for(u_short i = 1; i <= this->size; i++){
				// Получаем количество n-грамм
				count = this->arpa->count(i);
				// Формируем количество n-грамм
				data = this->alphabet->format("ngram %u=%u\n", i, count);
				// Выполняем запись данных в файл
				file.write(data.data(), data.size());
				// Увеличиваем количество всех n-грамм
				counts += count;
			}
			// Если количество элементов больше 0
			if(counts > 0){
				// Переходим по всей длине n-граммы
				for(u_short i = 1; i <= this->size; i++){
					// Формируем значение тела
					data = this->alphabet->format("\n\\%u-grams:\n", i);
					// Выполняем запись данных в файл
					file.write(data.data(), data.size());
					// Выполняем извлечение данных n-граммы
					this->arpa->grams(i, [&](const string & ngram){
						// Если строка получена
						if(!ngram.empty()){
							// Формируем тело n-граммы
							data = this->alphabet->format("%s\n", ngram.c_str());
							// Выполняем запись данных в файл
							file.write(data.data(), data.size());
						}
						// Если функция вывода статуса передана
						if(status != nullptr){
							// Увеличиваем количество записанных n-грамм
							index++;
							// Выполняем расчёт текущего статуса
							actual = u_short(index / double(counts) * 100.0);
							// Если статус обновился
							if(actual != past){
								// Запоминаем текущий статус
								past = actual;
								// Выводим статус извлечения
								status(actual);
							}
						}
					});
				}
				// Формируем значение тела
				data = "\n\\end\\\n";
				// Выполняем запись данных в файл
				file.write(data.data(), data.size());
			// Сообщаем что arpa пустая
			} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "arpa is empty");
			// Закрываем файл
			file.close();
		// Выводим сообщение об ошибке
		} else this->alphabet->log("ngrams file: %s is broken", alphabet_t::log_t::error, this->logfile, filename.c_str());
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ngrams file is empty");
}
/**
 * saveArpa Метод извлечения данных n-грамм в бинарном виде
 * @param callback функция обратного вызова
 * @param arpa     нужно добавить только данные arpa
 */
void anyks::Toolkit::saveArpa(function <void (const vector <char> &, const u_short)> callback, const bool arpa) const noexcept {
	// Выполняем извлечение данных arpa в бинарном виде
	this->arpa->getBin(arpa, callback);
}
/**
 * writeMap Метод записи карты последовательности в файл
 * @param filename адрес map файла карты последовательности
 * @param status   функция вывода статуса записи
 * @param delim    разделитель последовательностей
 */
void anyks::Toolkit::writeMap(const string & filename, function <void (const u_short)> status, const string & delim) const noexcept {
	// Если адрес файла передан
	if(!filename.empty()){
		// Открываем файл на запись
		ofstream file(filename, ios::binary);
		// Если файл открыт
		if(file.is_open()){
			// Текущий и предыдущий статус
			u_short past = 100;
			// Выполняем извлечение данных карты последовательностей
			this->arpa->map([&](const string & str, const u_short actual){
				// Если строка получена
				if(!str.empty()){
					// Получаем количество n-грамм
					const string & data = this->alphabet->format("%s\n", str.c_str());
					// Выполняем запись данных в файл
					file.write(data.data(), data.size());
				}
				// Если функция вывода статуса передана
				if((status != nullptr) && (actual != past)){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус извлечения
					status(actual);
				}
			}, delim);
			// Закрываем файл
			file.close();
		// Выводим сообщение об ошибке
		} else this->alphabet->log("map sequence file: %s is broken", alphabet_t::log_t::error, this->logfile, filename.c_str());
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "map sequence file is empty");
}
/**
 * words Метод извлечения слов из словаря
 * @param callback функция обратного вызова (word, idw, count words)
 */
void anyks::Toolkit::words(function <const bool (const word_t &, const size_t, const size_t)> callback) const noexcept {
	// Если словарь не пустой
	if(!this->vocab.empty()){
		// Количество слов в словаре
		const size_t count = this->vocab.size();
		// Переходим по всему словарю
		for(auto & item : this->vocab){
			// Если слово не пустое
			if(!item.second.empty() && !callback(item.second, item.first, count)) break;
		}
	// Сообщаем что словарь пустой
	} else callback(L"", 0, 0);
}
/**
 * words Метод извлечения слов из словаря
 * @param callback функция обратного вызова (word, idw, oc, dc, count words)
 */
void anyks::Toolkit::words(function <const bool (const wstring &, const size_t, const size_t, const size_t, const size_t)> callback) const noexcept {
	// Если словарь не пустой
	if(!this->vocab.empty()){
		// Метаданные слова
		anyks::ocdc_t meta;
		// Количество слов в словаре
		const size_t count = this->vocab.size();
		// Переходим по всему словарю
		for(auto & item : this->vocab){
			// Если слово не пустое
			if(!item.second.empty()){
				// Получаем метаданные слова
				meta = item.second.getmeta();
				// Если продолжать не нужно, выходим
				if(!callback(item.second.wreal(), item.first, meta.oc, meta.dc, count)) break;
			}
		}
	// Сообщаем что словарь пустой
	} else callback(L"", 0, 0, 0, 0);
}
/**
 * readArpa Метод чтения данных из файла arpa
 * @param filename адрес файла для чтения
 * @param status   функция вывода статуса чтения
 */
void anyks::Toolkit::readArpa(const string & filename, function <void (const u_short)> status) noexcept {
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Тип извлечения данных
		u_short type = 0;
		// Последовательность для добавления
		vector <pair_t> seq;
		// Список полученных слов последовательности
		vector <wstring> words;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Количество обработанных данных
		size_t index = 0, pos = 0, loc = 0;
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
			// Если слово получено
			if(!text.empty()){
				// Определяем тип считываемых данных
				switch(type){
					// Если получено обозначение заголовка
					case 0: if(text.find("\\data\\") != string::npos) type++; break;
					// Если мы дошли до считывания данных слов
					case 1: if(text.rfind("-grams:") != string::npos) type++; break;
					// Если это тип считывания данных n-грамм
					case 2: {
						// Если мы дошли до конца, выходим
						if(text.find("\\end\\") != string::npos) break;
						// Иначе считываем данные n-граммы
						else if((pos = text.find("\t")) != string::npos){
							// Обнуляем локальную позицию
							loc = 0;
							// Данные n-граммы и обратной частоты документа
							string ngram = "", backoff = "0.0";
							// Считываем частоту n-граммы
							const string & weight = this->alphabet->trim(text.substr(loc, pos));
							// Запоминаем текущую позицию
							loc = pos;
							// Ищем значение n-граммы
							if((pos = text.find("\t", pos + 1)) != string::npos){
								// Извлекаем данные n-граммы
								ngram = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
								// Излвлекаем обратную частоту документа
								backoff = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
							// Извлекаем данные n-граммы
							} else ngram = this->alphabet->trim(text.substr(loc + 1, text.length() - (loc + 1)));
							// Если данные получены
							if(!ngram.empty() && !weight.empty()){
								// Очищаем последовательность
								seq.clear();
								// Выполняем сплит n-грамм
								this->alphabet->split(ngram, " ", words);
								// Если список слов получен
								if(!words.empty()){
									// Идентификатор слова
									size_t idw = 0;
									// Полученное слово
									word_t word = L"";
									// Переходим по всему списку слов
									for(auto & item : words){
										// Получаем слово
										word = move(item);
										// Получаем идентификатор слова
										idw = this->getIdw(word, !this->isOption(options_t::confidence));
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(this->badwords.count(idw) < 1){
											// Если это неизвестное слово
											if(uid == idw){
												// Если неизвестное слово не установлено
												if(this->unknown == 0) seq.emplace_back(idw, 0);
												// Если неизвестное слово установлено
												else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
											// Добавляем слово в список последовательности
											} else seq.emplace_back(idw, ((words.size() > 1) && (this->utokens.count(idw) > 0) ? 0 : word.getUppers()));
										// Если слово найдено в всписке запрещённых
										} else {
											// Очищаем последовательность
											seq.clear();
											// Выходим из цикла
											break;
										}
									}
									// Если количество собранных n-грамм выше установленных, меняем
									if(seq.size() > size_t(this->size)){
										// Устанавливаем новый размер n-грамм
										this->size = seq.size();
										// Устанавливаем новый размер n-грамм для arpa
										this->arpa->setSize(this->size);
									}
									// Добавляем последовательность в словарь
									if(!seq.empty()){
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(this->arpa->event(idw) && (this->badwords.count(idw) < 1)){
											// Добавляем слово в словарь
											this->addWord(word.wreal(), idw);
										}
										// Добавляем последовательность в языковую модель
										this->arpa->set(seq, stod(weight), stod(backoff));
									}
								}
							}
						}
					} break;
				}
			}
			// Если функция вывода статуса передана
			if(status != nullptr){
				// Увеличиваем количество записанных n-грамм
				index += text.size();
				// Выполняем расчёт текущего статуса
				actual = u_short(index / double(fileSize) * 100.0);
				// Если статус обновился
				if(actual != past){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус извлечения
					status(actual);
				}
			}
		});
		// Обновляем количество уникальных слов
		this->info.unq = this->vocab.size();
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "arpa file is not exist");
}
/**
 * readVocab Метод чтения словаря из файла
 * @param filename файл для чтения словаря
 * @param status   функция вывода статуса чтения
 */
void anyks::Toolkit::readVocab(const string & filename, function <void (const u_short)> status) noexcept {
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Тип извлечения данных
		u_short type = 0;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Количество обработанных данных
		size_t index = 0, pos = 0, loc = 0;
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
			// Если слово получено
			if(!text.empty()){
				// Определяем тип считываемых данных
				switch(type){
					// Если получено обозначение заголовка
					case 0: if(text.find("\\data\\") != string::npos) type++; break;
					// Если это тип считывания статистики
					case 1: {
						// Если мы дошли до считывания данных слов
						if(text.find("\\words:") != string::npos) type++;
						// Пытаемся прочитать статистику
						else if((pos = text.find("=")) != string::npos){
							// Извлекаем ключ
							const string & key = this->alphabet->trim(text.substr(0, pos));
							// Получаем значение записи
							const string & val = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
							// Если это общее количество документов
							if(key.compare("ad") == 0) this->info.ad += stoull(val);
							// Если же это количество слов во всех документах
							else if(key.compare("cw") == 0) this->info.cw += stoull(val);
							// Если же это количество уникальных слов во всех документах
							else if(key.compare("unq") == 0) this->info.unq = stoull(val);
						}
					} break;
					// Если это тип считывания данных слова
					case 2: {
						// Если мы дошли до конца, выходим
						if(text.find("\\end\\") != string::npos) break;
						// Иначе считываем данные слова
						else if((pos = text.find("\t")) != string::npos){
							// Обнуляем локальную позицию
							loc = 0;
							// Считываем идентификатор слова
							const string & idWord = this->alphabet->trim(text.substr(loc, pos));
							// Запоминаем текущую позицию
							loc = pos;
							// Ищем значение слова
							if((pos = text.find("\t", pos + 1)) != string::npos){
								// Извлекаем данные слова
								word_t word = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
								// Запоминаем текущую позицию
								loc = pos;
								// Ищем встречаемость слова
								if((pos = text.find("|", pos + 1)) != string::npos){
									// Извлекаем встречаемость слова
									const string & oc = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
									// Запоминаем текущую позицию
									loc = pos;
									// Ищем встречаемость в документах
									if((pos = text.find("|", pos + 1)) != string::npos){
										// Получаем идентификатор слова
										const size_t idw = stoull(idWord);
										// Количество документов где встретилось слово
										const string & dc = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
										// Выполняем поиск слова в словаре
										auto it = this->vocab.find(idw);
										// Если слово найдено
										if(it != this->vocab.end()){
											// Получаем метаданные слова
											auto meta = it->second.getmeta();
											// Добавляем в слово новые метаданные
											it->second.setmeta(meta.dc + stoull(dc), meta.oc + stoull(oc));
										// Если слово в словаре не найдено
										} else {
											// Добавляем в слово его метаданные
											word.setmeta(stoull(dc), stoull(oc));
											// Добавляем слово в словарь
											this->vocab.emplace(idw, word);
										}
									}
								}
							}
						}
					} break;
				}
			}
			// Если функция вывода статуса передана
			if(status != nullptr){
				// Увеличиваем количество записанных n-грамм
				index += text.size();
				// Выполняем расчёт текущего статуса
				actual = u_short(index / double(fileSize) * 100.0);
				// Если статус обновился
				if(actual != past){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус извлечения
					status(actual);
				}
			}
		});
		// Обновляем количество уникальных слов
		this->info.unq = this->vocab.size();
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "vocab file is not exist");
}
/**
 * readNgram Метод чтения данных из файла ngrams
 * @param filename адрес файла для чтения
 * @param status   функция вывода статуса чтения
 */
void anyks::Toolkit::readNgram(const string & filename, function <void (const u_short)> status) noexcept {
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Последовательность для добавления
		vector <pair_t> seq;
		// Список полученных слов последовательности
		vector <wstring> words;
		// Количество обработанных данных
		size_t index = 0, pos = 0, loc = 0;
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100, type = 0;
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
			// Если слово получено
			if(!text.empty()){
				// Определяем тип считываемых данных
				switch(type){
					// Если получено обозначение заголовка
					case 0: if(text.find("\\data\\") != string::npos) type++; break;
					// Если это тип считывания статистики
					case 1: {
						// Если мы дошли до считывания данных слов
						if(text.rfind("-grams:") != string::npos) type++;
						// Пытаемся прочитать статистику
						else if((pos = text.find("=")) != string::npos){
							// Извлекаем ключ
							const string & key = this->alphabet->trim(text.substr(0, pos));
							// Получаем значение записи
							const string & val = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
							// Если это общее количество документов
							if(key.compare("ad") == 0) this->info.ad = stoull(val);
							// Если же это количество слов во всех документах
							else if(key.compare("cw") == 0) this->info.cw = stoull(val);
							// Если же это количество уникальных слов во всех документах
							else if(key.compare("unq") == 0) this->info.unq = stoull(val);
						}
					} break;
					// Если это тип считывания данных n-грамм
					case 2: {
						// Если мы дошли до конца, выходим
						if(text.find("\\end\\") != string::npos) break;
						// Иначе считываем данные слова
						else if((pos = text.find("\t")) != string::npos){
							// Обнуляем локальную позицию
							loc = 0;
							// Считываем n-грамму
							const string & ngram = this->alphabet->trim(text.substr(loc, pos));
							// Запоминаем текущую позицию
							loc = pos;
							// Ищем встречаемость слова
							if((pos = text.find("|", pos + 1)) != string::npos){
								// Очищаем последовательность
								seq.clear();
								// Извлекаем встречаемость слова
								const string & oc = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
								// Количество документов где встретилось слово
								const string & dc = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
								// Выполняем сплит n-грамм
								this->alphabet->split(ngram, " ", words);
								// Если список слов получен
								if(!words.empty() && !oc.empty() && !dc.empty()){
									// Полученное слово
									word_t word = L"";
									// Если это юниграмма
									if(words.size() == 1){
										// Получаем слово
										word = move(words.front());
										// Получаем идентификатор слова
										size_t idw = this->getIdw(word);
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(this->badwords.count(idw) < 1){
											// Если это неизвестное слово
											if(idw == uid){
												// Если неизвестное слово не установлено
												if(this->unknown == 0) seq.emplace_back(idw, 0);
												// Если неизвестное слово установлено
												else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
											// Добавляем слово в список
											} else {
												// Получаем регистры слова
												size_t uppers = word.getUppers();
												// Если название токена получено, устанавливаем его регистры
												if(this->utokens.count(idw) > 0) uppers = 0;
												// Добавляем слово в список последовательности
												seq.emplace_back(idw, uppers);
												// Если это правильное слово
												if((idw > 0) && this->arpa->event(idw)){
													// Добавляем в слово его метаданные
													word.setmeta(stoull(dc), stoull(oc));
													// Добавляем слово в словарь
													this->vocab.emplace(idw, word);
												}
											}
											// Добавляем последовательность в словарь
											this->arpa->set(seq, (size_t) stoull(oc), (size_t) stoull(dc));
										}
									// Переходим по всему списку слов и создаём последовательность
									} else {
										// Переходим по всему списку слов
										for(auto & item : words){
											// Получаем слово
											word = move(item);
											// Получаем идентификатор слова
											size_t idw = this->getIdw(word);
											// Проверяем отсутствует ли слово в списке запрещённых слов
											if(this->badwords.count(idw) < 1){
												// Если это неизвестное слово
												if(idw == uid){
													// Если неизвестное слово не установлено
													if(this->unknown == 0) seq.emplace_back(idw, 0);
													// Если неизвестное слово установлено
													else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
												// Добавляем слово в список последовательности
												} else seq.emplace_back(idw, this->utokens.count(idw) > 0 ? 0 : word.getUppers());
											// Если слово найдено в всписке запрещённых
											} else {
												// Очищаем последовательность
												seq.clear();
												// Выходим из цикла
												break;
											}
										}
										// Добавляем последовательность в словарь
										if(!seq.empty()) this->arpa->set(seq, (size_t) stoull(oc), (size_t) stoull(dc));
									}
								}
							}
						}
					} break;
				}
			}
			// Если функция вывода статуса передана
			if(status != nullptr){
				// Увеличиваем количество записанных n-грамм
				index += text.size();
				// Выполняем расчёт текущего статуса
				actual = u_short(index / double(fileSize) * 100.0);
				// Если статус обновился
				if(actual != past){
					// Запоминаем текущий статус
					past = actual;
					// Выводим статус извлечения
					status(actual);
				}
			}
		});
		// Обновляем количество уникальных слов
		this->info.unq = this->vocab.size();
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ngrams file is not exist");
}
/**
 * readMap Метод чтения карты последовательности из файла
 * @param filename адрес map файла карты последовательности
 * @param status   функция вывода статуса чтения
 * @param delim    разделитель последовательностей
 */
void anyks::Toolkit::readMap(const string & filename, function <void (const u_short)> status, const string & delim) noexcept {
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Количество обработанных данных
		size_t index = 0;
		// Данные последовательности
		arpa_t::seq_t sequence;
		// Список последовательностей для обучения
		vector <arpa_t::seq_t> seq;
		// Данные n-граммы и список параметров слова
		vector <wstring> ngram, params;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Определяем разрешены ли неизвестные слова
		const bool allowUnk = this->isOption(options_t::allowUnk);
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
			// Если слово получено
			if(!text.empty()){
				// Извлекаем данные n-граммы
				this->alphabet->split(text, delim, ngram);
				// Если данные получены
				if(!ngram.empty()){
					// Позиция слова и его идентификатор
					size_t idw = 0, pos = 0;
					// Переходим по всем словам n-граммы
					for(auto & item : ngram){
						// Ищем разделитель слова
						if((pos = item.find(L":{")) != wstring::npos){
							// Получаем идентификатор слова
							idw = stoull(item.substr(0, pos));
							// Проверяем является ли слово спец-словом
							bool noEvent = !this->arpa->event(idw);
							// Проверяем существует ли слово в словаре
							bool isWord = (this->vocab.count(idw) > 0);
							// Проверяем является ли слово пользовательским токеном
							bool isUtoken = (this->utokens.count(idw) > 0);
							// Проверяем отсутствует ли слово в списке запрещённых слов
							bool isAllow = (this->badwords.count(idw) < 1);
							/**
							 * Если это спец-слово, или слово существует в словаре оно также разрешено,
							 * или в arpa разрешён учёт неизвестных слов
							 */
							if(noEvent || isUtoken || ((isWord && isAllow) || allowUnk)){
								/**
								 * Если это не спец-слово и слово не существует в словаре или запрещено,
								 * устанавливаем идентификатор неизвестного слова
								 */
								if(!noEvent && !isUtoken && (!isWord || !isAllow)) idw = (size_t) token_t::unk;
								// Извлекаем параметров слова
								this->alphabet->split(item.substr(pos + 2, item.length() - ((pos + 2) + 1)), L",", params);
								// Если параметры получены
								if(!params.empty()){
									// Формируем блок слова
									sequence.idw  = idw;
									sequence.oc   = stoull(params[0]);
									sequence.dc   = stoull(params[1]);
									sequence.ups  = stoull(params[2]);
									// Формируем последовательность
									seq.push_back(sequence);
								}
							// Выходим из последовательности, она нам не нужна
							} else {
								// Очищаем последовательность
								seq.clear();
								// Выходим из цикла
								break;
							}
						}
					}
					// Если последовательность получена
					if(!seq.empty()) this->arpa->set(seq);
					// Очищаем последовательность
					seq.clear();
				}
				// Если функция вывода статуса передана
				if(status != nullptr){
					// Увеличиваем количество общих обработанных байт
					index += text.size();
					// Выполняем расчёт текущего статуса
					actual = u_short(index / double(fileSize) * 100.0);
					// Если статус обновился
					if(actual != past){
						// Запоминаем текущий статус
						past = actual;
						// Выводим статус извлечения
						status(actual);
					}
				}
			}
		});
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "map sequence file is not exist");
}
/**
 * readNgrams Метод чтения данных из каталога файлов ngrams
 * @param path   адрес где лежат ngrams файлы
 * @param ext    расширение файлов в каталоге
 * @param status функция вывода статуса чтения
 */
void anyks::Toolkit::readNgrams(const string & path, const string & ext, function <void (const u_short)> status) noexcept {
	// Получаем путь до скрипта
	const char * fullpath = realpath((!path.empty() ? path.c_str() : "./"), nullptr);
	// Если полный путь получен
	if(fullpath != nullptr){
		// Тип извлечения данных
		u_short type = 0;
		// Список последовательностей для обучения
		vector <pair_t> seq;
		// Список слов n-граммы
		vector <wstring> words;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Количество обработанных данных
		size_t index = 0, pos = 0, loc = 0;
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		// Получаем расширение файлов в каталоге
		const string & extension = (!ext.empty() ? ext : "ngrams");
		// Переходим по всему списку файлов в каталоге
		fsys_t::rdir(fullpath, extension, [&](const string & filename, const uintmax_t dirSize) noexcept {
			// Выполняем считывание всех строк текста
			fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
				// Если слово получено
				if(!text.empty()){
					// Определяем тип считываемых данных
					switch(type){
						// Если получено обозначение заголовка
						case 0: if(text.find("\\data\\") != string::npos) type++; break;
						// Если это тип считывания статистики
						case 1: {
							// Если мы дошли до считывания данных слов
							if(text.rfind("-grams:") != string::npos) type++;
							// Пытаемся прочитать статистику
							else if((pos = text.find("=")) != string::npos){
								// Извлекаем ключ
								const string & key = this->alphabet->trim(text.substr(0, pos));
								// Получаем значение записи
								const string & val = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
								// Если это общее количество документов
								if(key.compare("ad") == 0) this->info.ad += stoull(val);
								// Если же это количество слов во всех документах
								else if(key.compare("cw") == 0) this->info.cw += stoull(val);
								// Если же это количество уникальных слов во всех документах
								else if(key.compare("unq") == 0) this->info.unq = stoull(val);
							}
						} break;
						// Если это тип считывания данных n-грамм
						case 2: {
							// Если мы дошли до конца, выходим
							if(text.find("\\end\\") != string::npos) break;
							// Иначе считываем данные слова
							else if((pos = text.find("\t")) != string::npos){
								// Обнуляем локальную позицию
								loc = 0;
								// Считываем n-грамму
								const string & ngram = this->alphabet->trim(text.substr(loc, pos));
								// Запоминаем текущую позицию
								loc = pos;
								// Ищем встречаемость слова
								if((pos = text.find("|", pos + 1)) != string::npos){
									// Очищаем последовательность
									seq.clear();
									// Извлекаем встречаемость слова
									const string & oc = this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1)));
									// Количество документов где встретилось слово
									const string & dc = this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1)));
									// Выполняем сплит n-грамм
									this->alphabet->split(ngram, " ", words);
									// Если список слов получен
									if(!words.empty()){
										// Полученное слово
										word_t word = L"";
										// Если это юниграмма
										if(words.size() == 1){
											// Получаем слово
											word = move(words.front());
											// Получаем идентификатор слова
											size_t idw = this->getIdw(word);
											// Проверяем отсутствует ли слово в списке запрещённых слов
											if(this->badwords.count(idw) < 1){
												// Если это неизвестное слово
												if(idw == uid){
													// Если неизвестное слово не установлено
													if(this->unknown == 0) seq.emplace_back(idw, 0);
													// Если неизвестное слово установлено
													else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
												// Добавляем слово в список
												} else {
													// Получаем регистры слова
													size_t uppers = word.getUppers();
													// Если название токена получено, устанавливаем его регистры
													if(this->utokens.count(idw) > 0) uppers = 0;
													// Добавляем слово в список последовательности
													seq.emplace_back(idw, uppers);
													// Если это правильное слово
													if((idw > 0) && this->arpa->event(idw)){
														// Добавляем в слово его метаданные
														word.setmeta(stoull(dc), stoull(oc));
														// Добавляем слово в словарь
														this->vocab.emplace(idw, word);
													}
												}
												// Добавляем последовательность в словарь
												this->arpa->set(seq, (size_t) stoull(oc), (size_t) stoull(dc));
											}
										// Переходим по всему списку слов и создаём последовательность
										} else {
											// Переходим по всему списку слов
											for(auto & item : words){
												// Получаем слово
												word = move(item);
												// Получаем идентификатор слова
												size_t idw = this->getIdw(word);
												// Проверяем отсутствует ли слово в списке запрещённых слов
												if(this->badwords.count(idw) < 1){
													// Если это неизвестное слово
													if(idw == uid){
														// Если неизвестное слово не установлено
														if(this->unknown == 0) seq.emplace_back(idw, 0);
														// Если неизвестное слово установлено
														else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
													// Добавляем слово в список
													} else {
														// Получаем регистры слова
														size_t uppers = word.getUppers();
														// Если название токена получено, устанавливаем его регистры
														if(this->utokens.count(idw) > 0) uppers = 0;
														// Добавляем слово в список последовательности
														seq.emplace_back(idw, uppers);
													}
												// Если слово найдено в всписке запрещённых
												} else {
													// Очищаем последовательность
													seq.clear();
													// Выходим из цикла
													break;
												}
											}
											// Добавляем последовательность в словарь
											if(!seq.empty()) this->arpa->set(seq, (size_t) stoull(oc), (size_t) stoull(dc));
										}
									}
								}
							}
						} break;
					}
					// Если функция вывода статуса передана
					if(status != nullptr){
						// Увеличиваем количество общих обработанных байт
						index += text.size();
						// Выполняем расчёт текущего статуса
						actual = u_short(index / double(dirSize) * 100.0);
						// Если статус обновился
						if(actual != past){
							// Запоминаем текущий статус
							past = actual;
							// Выводим статус извлечения
							status(actual);
						}
					}
				}
			});
		});
		// Обновляем количество уникальных слов
		this->info.unq = this->vocab.size();
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ngrams files path is empty");
}
/**
 * readMaps Метод добавления - объединения карт последовательностей
 * @param path   адрес где лежат map файлы
 * @param ext    расширение файлов в каталоге
 * @param status функция вывода статуса чтения
 * @param delim  разделитель последовательностей
 */
void anyks::Toolkit::readMaps(const string & path, const string & ext, function <void (const u_short)> status, const string & delim) noexcept {
	// Получаем путь до скрипта
	const char * fullpath = realpath((!path.empty() ? path.c_str() : "./"), nullptr);
	// Если полный путь получен
	if(fullpath != nullptr){
		// Данные последовательности
		arpa_t::seq_t sequence;
		// Идентификатор документа и количество обработанных байт
		size_t idd = 0, index = 0;
		// Список последовательностей для обучения
		vector <arpa_t::seq_t> seq;
		// Данные n-граммы и список параметров слова
		vector <wstring> ngram, params;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Получаем расширение файлов в каталоге
		const string & extension = (!ext.empty() ? ext : "map");
		// Определяем разрешены ли неизвестные слова
		const bool allowUnk = this->isOption(options_t::allowUnk);
		// Переходим по всему списку файлов в каталоге
		fsys_t::rdir(fullpath, extension, [&](const string & filename, const uintmax_t dirSize) noexcept {
			// Выполняем считывание всех строк текста
			fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
				// Если слово получено
				if(!text.empty()){
					// Извлекаем данные n-граммы
					this->alphabet->split(text, delim, ngram);
					// Если данные получены
					if(!ngram.empty()){
						// Позиция слова и его идентификатор
						size_t idw = 0, pos = 0;
						// Переходим по всем словам n-граммы
						for(auto & item : ngram){
							// Ищем разделитель слова
							if((pos = item.find(L":{")) != wstring::npos){
								// Получаем идентификатор слова
								idw = stoull(item.substr(0, pos));
								// Проверяем является ли слово спец-словом
								bool noEvent = !this->arpa->event(idw);
								// Проверяем существует ли слово в словаре
								bool isWord = (this->vocab.count(idw) > 0);
								// Проверяем является ли слово пользовательским токеном
								bool isUtoken = (this->utokens.count(idw) > 0);
								// Проверяем отсутствует ли слово в списке запрещённых слов
								bool isAllow = (this->badwords.count(idw) < 1);
								/**
								 * Если это спец-слово, или слово существует в словаре оно также разрешено,
								 * или в arpa разрешён учёт неизвестных слов
								 */
								if(noEvent || isUtoken || ((isWord && isAllow) || allowUnk)){
									/**
									 * Если это не спец-слово и слово не существует в словаре или запрещено,
									 * устанавливаем идентификатор неизвестного слова
									 */
									if(!noEvent && !isUtoken && (!isWord || !isAllow)) idw = (size_t) token_t::unk;
									// Извлекаем параметров слова
									this->alphabet->split(item.substr(pos + 2, item.length() - ((pos + 2) + 1)), L",", params);
									// Если параметры получены
									if(!params.empty()){
										// Формируем блок слова
										sequence.idw  = idw;
										sequence.oc   = stoull(params[0]);
										sequence.dc   = stoull(params[1]);
										sequence.ups  = stoull(params[2]);
										// Формируем последовательность
										seq.push_back(sequence);
									}
								// Выходим из последовательности, она нам не нужна
								} else {
									// Очищаем последовательность
									seq.clear();
									// Выходим из цикла
									break;
								}
							}
						}
						// Если последовательность получена
						if(!seq.empty()){
							// Добавляем последовательность в базу
							this->arpa->add(seq, idd);
							// Очищаем последовательность
							seq.clear();
						}
					}
					// Если функция вывода статуса передана
					if(status != nullptr){
						// Увеличиваем количество общих обработанных байт
						index += text.size();
						// Выполняем расчёт текущего статуса
						actual = u_short(index / double(dirSize) * 100.0);
						// Если статус обновился
						if(actual != past){
							// Запоминаем текущий статус
							past = actual;
							// Выводим статус извлечения
							status(actual);
						}
					}
				}
			});
			// Выполняем смену идентификатора документа
			idd++;
		});
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "map files path is empty");
}
/**
 * Toolkit Конструктор
 * @param tokenizer объект токенизатора
 */
anyks::Toolkit::Toolkit(const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем токенизатор
	this->setTokenizer(tokenizer);
}
/**
 * Toolkit Конструктор
 * @param size размерность n-грамм
 */
anyks::Toolkit::Toolkit(const u_short size) noexcept {
	// Устанавливаем размерность n-грамм
	this->setSize(size);
}
/**
 * Toolkit Конструктор
 * @param alphabet  объект алфавита
 * @param tokenizer объект токенизатора
 */
anyks::Toolkit::Toolkit(const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем алфавит
	this->setAlphabet(alphabet);
	// Устанавливаем токенизатор
	this->setTokenizer(tokenizer);
}
/**
 * Toolkit Конструктор
 * @param alphabet объект алфавита
 * @param size     размерность n-грамм
 */
anyks::Toolkit::Toolkit(const alphabet_t * alphabet, const u_short size) noexcept {
	// Устанавливаем размерность n-грамм
	this->setSize(size);
	// Устанавливаем алфавит
	this->setAlphabet(alphabet);
}
/**
 * Toolkit Конструктор
 * @param tokenizer объект токенизатора
 * @param size      размерность n-грамм
 */
anyks::Toolkit::Toolkit(const tokenizer_t * tokenizer, const u_short size) noexcept {
	// Устанавливаем размерность n-грамм
	this->setSize(size);
	// Устанавливаем токенизатор
	this->setTokenizer(tokenizer);
}
/**
 * Toolkit Конструктор
 * @param alphabet  объект алфавита
 * @param tokenizer объект токенизатора
 * @param size      размерность n-грамм
 */
anyks::Toolkit::Toolkit(const alphabet_t * alphabet, const tokenizer_t * tokenizer, const u_short size) noexcept {
	// Устанавливаем размерность n-грамм
	this->setSize(size);
	// Устанавливаем алфавит
	this->setAlphabet(alphabet);
	// Устанавливаем токенизатор
	this->setTokenizer(tokenizer);
}
/**
 * ~Toolkit Деструктор
 */
anyks::Toolkit::~Toolkit() noexcept {
	// Очищаем языковую модель
	this->clear();
	// Очищаем выделенную память под arpa
	if(this->arpa != nullptr) delete this->arpa;
	// Очищаем выделенную память под python
	if(!notCleanPython && (this->python != nullptr)) delete this->python;
}

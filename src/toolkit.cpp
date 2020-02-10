/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include "toolkit.hpp"

/**
 * getIdws Метод генерирования идентификатора слова
 * @param  word слово для генерации
 * @return      идентификатор слова
 */
const size_t anyks::Toolkit::getIdw(const wstring & word) const {
	// Результат работы функции
	size_t result = noID;
	// Если слово передано
	if(!word.empty()){
		// Проверяем является ли слово, началом предложения
		if(word.compare(L"<s>") == 0) result = (size_t) sign_t::start;
		// Проверяем является ли слово числом
		else if(word.compare(L"<num>") == 0) result = (size_t) sign_t::num;
		// Проверяем является ли слово неизвестным
		else if(word.compare(L"<unk>") == 0) result = (size_t) sign_t::unk;
		// Проверяем является ли слово аббревиатурой
		else if(word.compare(L"<abbr>") == 0) result = (size_t) sign_t::abbr;
		// Проверяем является ли слово датой
		else if(word.compare(L"<date>") == 0) result = (size_t) sign_t::date;
		// Проверяем является ли слово временем
		else if(word.compare(L"<time>") == 0) result = (size_t) sign_t::time;
		// Проверяем является ли слово псевдо-числом
		else if(word.compare(L"<anum>") == 0) result = (size_t) sign_t::anum;
		// Проверяем является ли слово концом предложения
		else if(word.compare(L"</s>") == 0) result = (size_t) sign_t::finish;
		// Проверяем является ли слово диапазоном чисел
		else if(word.compare(L"<range>") == 0) result = (size_t) sign_t::range;
		// Проверяем является ли слово знаком пунктуации
		else if(word.compare(L"<punct>") == 0) result = (size_t) sign_t::punct;
		// Проверяем является ли слово приблизительным числом
		else if(word.compare(L"<aprox>") == 0) result = (size_t) sign_t::aprox;
		// Проверяем является ли слово числовым счётом
		else if(word.compare(L"<score>") == 0) result = (size_t) sign_t::score;
		// Проверяем является ли слово габаритными размерами
		else if(word.compare(L"<dimen>") == 0) result = (size_t) sign_t::dimen;
		// Проверяем является ли слово числовой дробью
		else if(word.compare(L"<fract>") == 0) result = (size_t) sign_t::fract;
		// Проверяем является ли слово знаком изоляции
		else if(word.compare(L"<isolat>") == 0) result = (size_t) sign_t::isolat;
		// Если это другое слово
		else {
			// Формируем идентификатор слова
			result = this->idw.get(word);
			// Проверяем является ли слово хорошим
			if(this->goodwords.count(result) < 1){
				// Подсчитываем количество дефисов
				u_short hyphenCounts = 0;
				// Если нужно проверить пользовательские признаки
				if(!this->isOption(options_t::notUserSign) && !this->usigns.empty()){
					// Переходим по всему списку пользовательских признаков
					for(auto & sign : this->usigns){
						// Если сработал пользовательский признак
						if(sign.second.test(sign.second.name.real(), this->alphabet->convert(word))) return sign.first;
					}
				}
				// Проверяем является ли слово знаком пунктуации
				if(word.length() == 1){
					// Если это знак пунктуации
					if(!this->isOption(options_t::notPunct) && this->alphabet->isPunct(word.front())){
						// Запоминаем что это знак пунктуации
						result = (size_t) sign_t::punct;
					// Если это знак изоляции
					} else if(!this->isOption(options_t::notIsolat) && this->alphabet->isIsolation(word.front())){
						// Запоминаем что это знак изоляции
						result = (size_t) sign_t::isolat;
					}
				// Проверяем есть ли изоляционный знак и количество дефисов в слове больше 2-х
				} else if(
					this->alphabet->isIsolation(word.back()) ||
					this->alphabet->isIsolation(word.front()) ||
					((hyphenCounts = this->alphabet->countLetter(word, L'-')) > 2)
				) result = (size_t) sign_t::unk;
				// Если идентификатор определить не удалось
				else {
					// Идентификатор слова
					size_t idw = 0;
					// Пытаемся определить идентификатор слова
					switch(this->alphabet->numSign(word)){
						// Если признак не определён
						case 0: if(this->isOption(options_t::onlyGood)) idw = (size_t) sign_t::unk; break;
						// Если это признак числа, запоминаем его
						case (u_short) sign_t::num: if(!this->isOption(options_t::notNumber)) idw = (size_t) sign_t::num; break;
						// Если это признак аббривиатуры
						case (u_short) sign_t::abbr: if(!this->isOption(options_t::notAbbr)) idw = (size_t) sign_t::abbr; break;
						// Если это признак даты
						case (u_short) sign_t::date: if(!this->isOption(options_t::notDate)) idw = (size_t) sign_t::date; break;
						// Если это признак времени
						case (u_short) sign_t::time: if(!this->isOption(options_t::notTime)) idw = (size_t) sign_t::time; break;
						// Если это признак приблизительного числа
						case (u_short) sign_t::aprox: if(!this->isOption(options_t::notAprox)) idw = (size_t) sign_t::aprox; break;
						// Если это признак псевдо-числа
						case (u_short) sign_t::anum: if(!this->isOption(options_t::notANumber)) idw = (size_t) sign_t::anum; break;
						// Если это признак диапазона чисел
						case (u_short) sign_t::range: if(!this->isOption(options_t::notRange)) idw = (size_t) sign_t::range; break;
						// Если это признак числового счёта
						case (u_short) sign_t::score: if(!this->isOption(options_t::notScore)) idw = (size_t) sign_t::score; break;
						// Если это признак габаритных размеров
						case (u_short) sign_t::dimen: if(!this->isOption(options_t::notDimen)) idw = (size_t) sign_t::dimen; break;
						// Если это признак числовой дроби
						case (u_short) sign_t::fract: if(!this->isOption(options_t::notFract)) idw = (size_t) sign_t::fract; break;
					}
					// Если слово определено как число но это не число, значит это римское число
					if((idw == (size_t) sign_t::num) && !this->alphabet->isNumber({word.back()})){
						// Если не запрещено выводить римские числа, выводим его
						if(!this->isOption(options_t::notRoman)) result = idw;
					// Иначе запоминаем идентификатор так-как он передан
					} else if(idw > 0) result = idw;
					// Если слово не разрешено, устанавливаем неизвестное слово
					else if(!this->alphabet->isAllowed(word)) result = (size_t) sign_t::unk;
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * isOption Метод проверки наличия опции
 * @param option опция для проверки
 * @return       результат проверки
 */
const bool anyks::Toolkit::isOption(const options_t option) const {
	// Выполняем проверку наличия опции
	return this->options.test((u_short) option);
}
/**
 * clearShielding Функция удаления экранирования
 * @param word  слово в котором следует удалить экранирование
 * @param front первый символ экранирования
 * @param back  последний символ экранирования
 */
void anyks::Toolkit::clearShielding(const string & word, const string & front, const string & back) const {
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
const u_short anyks::Toolkit::getSize() const {
	// Выводим размер n-грамм в словаре
	return this->size;
}
/**
 * getOptions Метод извлечения установленных опций
 * @return установленные опции
 */
const u_short anyks::Toolkit::getOptions() const {
	// Выводим установленные опции
	return this->options.to_ulong();
}
/**
 * getUnknown Метод извлечения неизвестного слова
 * @return установленное неизвестное слово
 */
const string anyks::Toolkit::getUnknown() const {
	// Результат работы функции
	string result = "";
	// Если неизвестное слово установлено
	if(this->unknown > 0){
		// Выполняем поиск в словаре
		auto it = this->vocab.find(this->unknown);
		// Если слово найдено
		if(it != this->vocab.end()) result = it->second.real();
	}
	// Выводим результат
	return result;
}
/**
 * getWordScript Метод извлечения скрипта обработки слов
 * @return адрес скрипта python обработки слов
 */
const string anyks::Toolkit::getWordScript() const {
	// Результат работы функции
	string result = "";
	// Если скрипты установлены
	if(!this->scripts.empty()){
		// Ищем скрипт обработки слов
		auto it = this->scripts.find(1);
		// Если скрипт обработки слов установлен
		if(it != this->scripts.end()) result = it->second.first;
	}
	// Выводим результат
	return result;
}
/**
 * getUsignScript Метод извлечения скрипта обработки пользовательских признаков
 * @return адрес скрипта python обработки пользовательских признаков
 */
const string anyks::Toolkit::getUsignScript() const {
	// Результат работы функции
	string result = "";
	// Если скрипты установлены
	if(!this->scripts.empty()){
		// Ищем скрипт обработки слов
		auto it = this->scripts.find(2);
		// Если скрипт обработки слов установлен
		if(it != this->scripts.end()) result = it->second.first;
	}
	// Выводим результат
	return result;
}
/**
 * getParams Метод извлечения параметров алгоритма сжатия
 * @return параметры алгоритма сжатия
 */
const anyks::Toolkit::params_t & anyks::Toolkit::getParams() const {
	// Выводим параметры алгоритма сжатия
	return this->params;
}
/**
 * getUsigns Метод извлечения списка пользовательских признаков
 * @return список пользовательских признаков
 */
const vector <string> anyks::Toolkit::getUsigns() const {
	// Результат работы функции
	vector <string> result;
	// Если пользовательские признаки есть
	if(!this->usigns.empty()){
		// Переходим по всем пользовательским признакам
		for(auto & sign : this->usigns){
			// Добавляем в список признак слова
			result.push_back(this->getUsignWord(sign.first));
		}
	}
	// Выводим результат
	return result;
}
/**
 * getBadwords Метод извлечения чёрного списка
 * @return чёрный список слов
 */
const set <size_t> & anyks::Toolkit::getBadwords() const {
	// Выводим результат
	return this->badwords;
}
/**
 * getGoodwords Метод извлечения белого списка
 * @return белый список слов
 */
const set <size_t> & anyks::Toolkit::getGoodwords() const {
	// Выводим результат
	return this->goodwords;
}
/**
 * getUsignWord Метод получения пользовательского признака по его идентификатору
 * @param idw идентификатор пользовательского признака
 * @return    пользовательский признак соответствующий идентификатору
 */
const string anyks::Toolkit::getUsignWord(const size_t idw) const {
	// Результат работы функции
	string result = "";
	// Если идентификатор передан
	if((idw > 0) && (idw < noID)){
		// Ищем идентификатор признака
		auto it = this->usigns.find(idw);
		// Если такой идентификатор существует
		if(it != this->usigns.end()){
			// Выводим слово соответствующее идентификатору
			result = move(it->second.name.real());
			// Выполняем удаление экранирования
			this->clearShielding(result, "<", ">");
		}
	}
	// Выводим результат
	return result;
}
/**
 * getUsignId Метод получения идентификатора пользовательского признака
 * @param name слово для которого нужно получить идентификатор
 * @return     идентификатор пользовательского признака соответствующий слову
 */
const size_t anyks::Toolkit::getUsignId(const string & name) const {
	// Результат работы функции
	size_t result = noID;
	// Если слово передано
	if(!name.empty()){
		// Выполняем удаление экранирования
		this->clearShielding(name, "<", ">"); {
			// Название признака
			string word = "<";
			// Добавляем название
			word.append(name);
			// Добавляем экранирование
			word.append(">");
			// Получаем идентификатор слова
			const size_t idw = this->idw.get(this->alphabet->convert(word));
			// Если такой идентификатор существует, выводим его
			result = (this->usigns.count(idw) > 0 ? idw : result);
		}
	}
	// Выводим результат
	return result;
}
/**
 * clear Метод очистки
 */
void anyks::Toolkit::clear(){
	// Очищаем неизвестное слово
	this->unknown = 0;
	// Очищаем словарь arpa
	this->arpa->clear();
	// Очищаем словарь
	this->vocab.clear();
	// Очищаем список пользовательских признаков
	this->usigns.clear();
	// Очищаем список плохих слов
	this->badwords.clear();
	// Очищаем список хороших слов
	this->goodwords.clear();
	// Очищаем объект статистики
	this->info = info_t();
}
/**
 * Метод очистки пользовательских признаков
 */
void anyks::Toolkit::clearUsigns(){
	// Очищаем список пользовательских признаков
	this->usigns.clear();
}
/**
 * clearBadwords Метод очистки списка плохих слов
 */
void anyks::Toolkit::clearBadwords(){
	// Очищаем список плохих слов
	this->badwords.clear();
}
/**
 * clearGoodwords Метод очистки списка хороших слов
 */
void anyks::Toolkit::clearGoodwords(){
	// Очищаем список хороших слов
	this->goodwords.clear();
}
/**
 * addBadword Метод добавления идентификатора похого слова в список
 * @param idw идентификатор слова
 */
void anyks::Toolkit::addBadword(const size_t idw){
	// Если идентификатор передан
	if((idw > 0) && (idw < noID)) this->badwords.emplace(idw);
}
/**
 * addBadword Метод добавления похого слова в список
 * @param word слово для добавления
 */
void anyks::Toolkit::addBadword(const string & word){
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t idw = this->idw.get(this->alphabet->convert(word));
		// Добавляем слово в список плохих слов
		this->addBadword(idw);
	}
}
/**
 * addGoodword Метод добавления идентификатора хорошего слова в список
 * @param idw идентификатор слова
 */
void anyks::Toolkit::addGoodword(const size_t idw){
	// Если идентификатор передан
	if((idw > 0) && (idw < noID)) this->goodwords.emplace(idw);
}
/**
 * addGoodword Метод добавления хорошего слова в список
 * @param word слово для добавления
 */
void anyks::Toolkit::addGoodword(const string & word){
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t idw = this->idw.get(this->alphabet->convert(word));
		// Добавляем слово в список хороших слов
		this->addGoodword(idw);
	}
}
/**
 * setOptions Метод установки опций
 * @param options опции для установки
 */
void anyks::Toolkit::setOptions(const u_short options){
	// Устанавливаем опции модуля
	this->options = options;
}
/**
 * setLogfile Метод установка файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::Toolkit::setLogfile(const char * logfile){
	// Устанавливаем адрес log файла
	this->logfile = logfile;
}
/**
 * setUsign Метод добавления признака пользователя
 * @param name слово - обозначение признака
 */
void anyks::Toolkit::setUsign(const string & name){
	// Если слово передано
	if(!name.empty()){
		// Создаём новый пользовательский признак
		usign_t userSign;
		// Выполняем удаление экранирования
		this->clearShielding(name, "<", ">"); {
			// Название признака
			string word = "<";
			// Добавляем название
			word.append(name);
			// Добавляем экранирование
			word.append(">");
			// Получаем идентификатор слова
			userSign.idw = this->idw.get(this->alphabet->convert(word));
			// Если такого идентификатора нет в списке
			if((userSign.idw > 0) && (this->usigns.count(userSign.idw) < 1)){
				// Добавляем в пользовательский признак наши параметры
				userSign.name = move(word);
				// Добавляем в список признакав, наш признак
				this->usigns.emplace(userSign.idw, move(userSign));
			}
		}
	}
}
/**
 * setUnknown Метод установки неизвестного слова
 * @param word слово для добавления
 */
void anyks::Toolkit::setUnknown(const string & word){
	// Если слово передано
	if(!word.empty()){
		// Формируем идентификатор слова
		this->unknown = this->idw.get(this->alphabet->convert(word));
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
 * setOption Метод подключения опции модуля
 * @param option опция для подключения
 */
void anyks::Toolkit::setOption(const options_t option){
	// Устанавливаем опции
	this->options.set((u_short) option);
}
/**
 * unsetOption Метод отключения опции модуля
 * @param option опция для отключения
 */
void anyks::Toolkit::unsetOption(const options_t option){
	// Устанавливаем опции
	this->options.reset((u_short) option);
}
/**
 * setWordScript Метод установки скрипта обработки слов
 * @param script скрипт python обработки слов
 */
void anyks::Toolkit::setWordScript(const string & script){
	// Выполняем добавление скрипта
	this->scripts.emplace(1, make_pair(script, 0));
}
/**
 * setUsignScript Метод установки скрипта обработки пользовательских признаков
 * @param script скрипт python обработки пользовательских признаков
 */
void anyks::Toolkit::setUsignScript(const string & script){
	// Выполняем добавление скрипта
	this->scripts.emplace(2, make_pair(script, 0));
}
/**
 * setSize Метод установки размера n-граммы
 * @param size размер n-граммы
 */
void anyks::Toolkit::setSize(const u_short size){
	// Устанавливаем размерность n-граммы
	this->size = (size > 0 ? size : DEFNGRAM);
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита
 */
void anyks::Toolkit::setAlphabet(const alphabet_t * alphabet){
	// Если алфавит передан
	if(alphabet != nullptr){
		// Устанавливаем переданный алфавит
		this->alphabet = alphabet;
		// Устанавливаем алфавит и смещение в 15 позиций (количество системных признаков arpa)
		this->idw.set(this->alphabet, 15);
	}
}
/**
 * addBadwords Метод добавления списка идентификаторов плохих слов в список
 * @param badwords список идентификаторов плохих слов
 */
void anyks::Toolkit::addBadwords(const vector <size_t> & badwords){
	// Если список не пустой
	if(!badwords.empty()){
		// Переходим по всему списку слов
		for(auto & idw : badwords) this->addBadword(idw);
	}
}
/**
 * addBadwords Метод добавления списка плохих слов в список
 * @param badwords список плохих слов
 */
void anyks::Toolkit::addBadwords(const vector <string> & badwords){
	// Если список не пустой
	if(!badwords.empty()){
		// Переходим по всему списку слов
		for(auto & word : badwords) this->addBadword(word);
	}
}
/**
 * addGoodwords Метод добавления списка идентификаторов хороших слов в список
 * @param goodwords список идентификаторов хороших слов
 */
void anyks::Toolkit::addGoodwords(const vector <size_t> & goodwords){
	// Если список не пустой
	if(!goodwords.empty()){
		// Переходим по всему списку слов
		for(auto & idw : goodwords) this->addGoodword(idw);
	}
}
/**
 * addGoodwords Метод добавления списка хороших слов в список
 * @param goodwords список хороших слов
 */
void anyks::Toolkit::addGoodwords(const vector <string> & goodwords){
	// Если список не пустой
	if(!goodwords.empty()){
		// Переходим по всему списку слов
		for(auto & word : goodwords) this->addGoodword(word);
	}
}
/**
 * addWord Метод добавления слова в словарь
 * @param word слово для добавления
 * @param idd  идентификатор документа
 */
void anyks::Toolkit::addWord(const wstring & word, const size_t idd){
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t idw = this->getIdw(word);
		// Если идентификатор нормальный
		if((idw > 0) && (idw < noID) && this->arpa->event(idw) && (this->usigns.count(idw) < 1)){
			// Ищем слово в словаре
			auto it = this->vocab.find(idw);
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
				this->vocab.emplace(idw, word);
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
 * addText Метод добавления текста для расчёта
 * @param text текст который нужно добавить
 * @param idd  идентификатор документа
 */
void anyks::Toolkit::addText(const string & text, const size_t idd){
	// Если текст передан
	if(!text.empty()){
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) sign_t::unk;
		// Идентификатор начала предложения
		const size_t sid = (size_t) sign_t::start;
		// Идентификатор конца предложения
		const size_t fid = (size_t) sign_t::finish;
		// Последовательность собранных слов
		vector <string> words = {"<s>"};
		// Список последовательностей для обучения
		vector <pair_t> seq = {{sid, 0}};
		/**
		 * unkFn Функция установки неизвестного слова в последовательность
		 */
		auto unkFn = [&uid, &seq, this]{
			// Если неизвестное слово не установлено
			if((this->unknown == 0) && this->isOption(options_t::allowUnk)) seq.emplace_back(uid, 0);
			// Если неизвестное слово установлено
			else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
		};
		/**
		 * modeFn Функция обработки разбитого текста
		 * @param early  предыдущее слово
		 * @param word   текущее слово
		 * @param punct  знак пунктуации если он есть
		 * @param pos    позиция текущего слова
		 * @param stop   флаг завершения обработки
		 */
		auto modeFn = [&](
			const wstring & early,
			const wstring & word,
			const wchar_t punct = 0,
			const size_t pos = 0,
			const bool stop = false
		){
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
						// Создаем слово для списка контекста
						const string & seqWord = this->alphabet->convert(word);
						// Выполняем внешний python скрипт
						tmp = move(this->python->run(it->second.second, {seqWord}, words));
						// Если результат не получен, возвращаем слово
						if(tmp.empty()) tmp = move(word);
						// Добавляем слово в список
						words.push_back(seqWord);
					}
				}
				// Если слово не разрешено
				if(tmp.length() >= MAX_WORD_LENGTH) unkFn();
				// Если слово разрешено
				else {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Если это плохое слово, заменяем его на неизвестное
					if((idw == 0) || (idw == noID) || (this->badwords.count(idw) > 0)) unkFn();
					// Иначе продолжаем дальше
					else {
						// Если это неизвестное слово
						if(idw == uid) unkFn();
						// Иначе добавляем слово
						else {
							// Получаем регистры слова
							size_t uppers = (seq.size() > 1 ? tmp.getUppers() : 0);
							// Если название признака получено, устанавливаем его регистры
							if(this->usigns.count(idw) > 0) uppers = 0;
							// Добавляем слово в последовательность
							seq.emplace_back(idw, uppers);
						}
						// Добавляем слово в словарь если разрешено
						if(this->alphabet->isAllowed(tmp)) this->addWord(tmp, idd);
					}
				}
			}
			// Если найден знак пунктуации
			if(stop || ((seq.size() > 2) && this->alphabet->isPunct(punct))){
				// Идентификатор предыдущего слова
				size_t idw = noID;
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
				// Очищаем список слов
				words.clear();
				// Добавляем в список начало предложения
				seq.emplace_back(sid, 0);
			}
			// Выводим результат
			return true;
		};
		// Выполняем разбивку текста на токены
		this->alphabet->tokens(text, modeFn);
	}
}
/**
 * setUsignMethod Метод добавления функции обработки пользовательского признака
 * @param name слово - обозначение признака
 * @param fn   внешняя функция обрабатывающая пользовательский признак
 */
void anyks::Toolkit::setUsignMethod(const string & name, function <bool (const string &, const string &)> fn){
	// Если название признака передано
	if(!name.empty()){
		// Получаем идентификатор признака
		const size_t idw = this->getUsignId(name);
		// Ищем нам признак
		auto it = this->usigns.find(idw);
		// Если такой признак найден, устанавливаем функци
		if(it != this->usigns.end()) it->second.test = fn;
	}
}
/**
 * sweep Метод удаления низкочастотных n-грамм arpa
 * @param status статус расёта
 */
void anyks::Toolkit::sweep(function <void (const u_short)> status){
	// Выполняем удаление низкочастотных n-грамм
	this->arpa->sweep(status);
}
/**
 * train Метод запуска обучения языковой модели
 * @param status функция вывода статуса обучения
 */
void anyks::Toolkit::train(function <void (const u_short)> status){
	// Выполняем обучение arpa
	this->arpa->train(status);
}
/**
 * repair Метод ремонта уже расчитанной ранее arpa
 * @param status статус расёта
 */
void anyks::Toolkit::repair(function <void (const u_short)> status){
	// Выполняем исправление arpa
	this->arpa->repair(status);
}
/**
 * loadInfoVocab Метод загрузки бинарных информационных данных словаря
 * @param buffer буфер бинарных информационных данных словаря
 */
void anyks::Toolkit::loadInfoVocab(const vector <char> & buffer){
	// Если буфер слова передан
	if(!buffer.empty()){
		// Загружаем информационные данные словаря
		memcpy(&this->info, buffer.data(), sizeof(this->info));
		// Сбрасываем идентификатор последнего документа
		this->info.idd = noID;
	}
}
/**
 * saveInfoVocab Метод сохранения бинарных информационных данных словаря
 * @param buffer буфер бинарных информационных данных словаря
 */
void anyks::Toolkit::saveInfoVocab(vector <char> & buffer) const {
	// Очищаем переданный буфер данных
	buffer.clear();
	// Получаем бинарные информационные данные словаря
	const char * bin = reinterpret_cast <const char *> (&this->info);
	// Добавляем в буфер бинарные информационные данные словаря
	buffer.insert(buffer.end(), bin, bin + sizeof(this->info));
}
/**
 * loadVocab Метод загрузки бинарных данных в словарь
 * @param buffer буфер с бинарными данными
 * @param arpa   нужно добавить только данные arpa
 */
void anyks::Toolkit::loadVocab(const vector <char> & buffer) const {
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
		if((idw > 0) && (idw < noID)){
			// Слово для добавления в словарь
			word_t word = L"";
			// Добавляем бинарные данные слова
			word.set(data + offset, buffer.size() - offset);
			// Добавляем слово в словарь
			if(!word.empty()) this->vocab.emplace(idw, move(word));
		}
	}
}
/**
 * load Метод загрузки бинарных данных n-грамм в словарь
 * @param buffer буфер с бинарными данными
 * @param arpa   нужно добавить только данные arpa
 */
void anyks::Toolkit::loadArpa(const vector <char> & buffer, const bool arpa) const {
	// Выполняем загрузку бинарных данных
	this->arpa->load(buffer, arpa);
}
/**
 * saveVocab Метод извлечения данных словаря в бинарном виде
 * @param callback функция обратного вызова
 */
void anyks::Toolkit::saveVocab(function <void (const vector <char> &, const u_short)> callback) const {
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
			callback(buffer, (index / float(this->vocab.size()) * 100.0f));
			// Очищаем собранный буфер данных
			buffer.clear();
			// Увеличиваем индекс слова
			index++;
		}
	}
}
/**
 * readArpas Метод чтения данных из каталога файлов arpa
 * @param path   адрес где лежат arpa файлы
 * @param status функция вывода статуса чтения
 */
void anyks::Toolkit::readArpas(const string & path, function <void (const u_short)> status){
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
		size_t idd = 0, index = 0, pos = 0, loc = 0;
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) sign_t::unk;
		// Переходим по всему списку файлов в каталоге
		fsys_t::rdir(fullpath, "arpa", [&](const string & filename, const uintmax_t dirSize){
			// Выполняем считывание всех строк текста
			fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize){
				// Если слово получено
				if(!text.empty()){
					// Определяем тип считываемых данных
					switch(type){
						// Если получено обозначение заголовка
						case 0: if(text.compare("\\data\\") == 0) type++; break;
						// Если мы дошли до считывания данных слов
						case 1: if(text.rfind("-grams:") != string::npos) type++; break;
						// Если это тип считывания данных n-грамм
						case 2: {
							// Если мы дошли до конца, выходим
							if(text.compare("\\end\\") == 0) break;
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
									ngram = move(this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1))));
									// Излвлекаем обратную частоту документа
									backoff = move(this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1))));
								// Извлекаем данные n-граммы
								} else ngram = move(this->alphabet->trim(text.substr(loc + 1, text.length() - (loc + 1))));
								// Если данные получены
								if(!ngram.empty() && !weight.empty()){
									// Очищаем последовательность
									seq.clear();
									// Выполняем сплит n-грамм
									this->alphabet->split(ngram, " ", words);
									// Если список слов получен
									if(!words.empty()){
										// Полученное слово
										word_t word = L"";
										// Если это биграмма
										if(words.size() < 3){
											// Получаем слово
											word = words.front();
											// Получаем идентификатор слова
											const size_t idw = this->getIdw(word);
											// Проверяем отсутствует ли слово в списке запрещённых слов
											if(this->badwords.count(idw) < 1){
												// Если это юниграмма и её еще нет в словаре
												if((idw != (size_t) sign_t::start) &&
												((words.size() > 1) || (this->vocab.count(idw) < 1))) this->addWord(word, idd);
											}
										}
										// Переходим по всему списку слов
										for(auto & item : words){
											// Получаем слово
											word = move(item);
											// Получаем идентификатор слова
											size_t idw = this->getIdw(word);
											// Проверяем отсутствует ли слово в списке запрещённых слов
											if(this->badwords.count(idw) < 1){
												// Если это неизвестное слово
												if(uid == idw){
													// Если неизвестное слово не установлено
													if(this->unknown == 0) seq.emplace_back(idw, 0);
													// Если неизвестное слово установлено
													else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
												// Добавляем слово в список последовательности
												} else seq.emplace_back(idw, ((words.size() > 1) && (this->usigns.count(idw) > 0) ? 0 : word.getUppers()));
											// Если слово найдено в всписке запрещённых
											} else {
												// Очищаем последовательность
												seq.clear();
												// Выходим из цикла
												break;
											}
										}
										// Добавляем последовательность в словарь
										if(!seq.empty()) this->arpa->add(seq, stof(weight), stof(backoff));
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
					actual = u_short(index / float(dirSize) * 100.0f);
					// Если статус обновился
					if(actual != past){
						// Запоминаем текущий статус
						past = actual;
						// Выводим статус извлечения
						status(actual);
					}
				}
			});
			// Выполняем смену идентификатора документа
			idd++;
		});
		// Выполняем исправление arpa
		this->arpa->repair(status);
		// Обновляем количество уникальных слов
		this->info.unq = this->vocab.size();
	// Выводим сообщение об ошибке
	} else this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "arpa files path is empty");
}
/**
 * readNgrams Метод чтения данных из каталога файлов ngrams
 * @param path   адрес где лежат ngrams файлы
 * @param status функция вывода статуса чтения
 */
void anyks::Toolkit::readNgrams(const string & path, function <void (const u_short)> status){
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
		const size_t uid = (size_t) sign_t::unk;
		// Определяем разрешены ли неизвестные слова
		const bool allowUnk = this->isOption(options_t::allowUnk);
		// Переходим по всему списку файлов в каталоге
		fsys_t::rdir(fullpath, "ngrams", [&](const string & filename, const uintmax_t dirSize){
			// Выполняем считывание всех строк текста
			fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize){
				// Если слово получено
				if(!text.empty()){
					// Определяем тип считываемых данных
					switch(type){
						// Если получено обозначение заголовка
						case 0: if(text.compare("\\data\\") == 0) type++; break;
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
							if(text.compare("\\end\\") == 0) break;
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
													// Если название признака получено, устанавливаем его регистры
													if(this->usigns.count(idw) > 0) uppers = 0;
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
														// Если название признака получено, устанавливаем его регистры
														if(this->usigns.count(idw) > 0) uppers = 0;
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
						actual = u_short(index / float(dirSize) * 100.0f);
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
 * readArpa Метод чтения данных из файла arpa
 * @param filename адрес файла для чтения
 * @param status   функция вывода статуса чтения
 */
void anyks::Toolkit::readArpa(const string & filename, function <void (const u_short)> status){
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
		const size_t uid = (size_t) sign_t::unk;
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize){
			// Если слово получено
			if(!text.empty()){
				// Определяем тип считываемых данных
				switch(type){
					// Если получено обозначение заголовка
					case 0: if(text.compare("\\data\\") == 0) type++; break;
					// Если мы дошли до считывания данных слов
					case 1: if(text.rfind("-grams:") != string::npos) type++; break;
					// Если это тип считывания данных n-грамм
					case 2: {
						// Если мы дошли до конца, выходим
						if(text.compare("\\end\\") == 0) break;
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
								ngram = move(this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1))));
								// Излвлекаем обратную частоту документа
								backoff = move(this->alphabet->trim(text.substr(pos + 1, text.length() - (pos + 1))));
							// Извлекаем данные n-граммы
							} else ngram = move(this->alphabet->trim(text.substr(loc + 1, text.length() - (loc + 1))));
							// Если данные получены
							if(!ngram.empty() && !weight.empty()){
								// Очищаем последовательность
								seq.clear();
								// Выполняем сплит n-грамм
								this->alphabet->split(ngram, " ", words);
								// Если список слов получен
								if(!words.empty()){
									// Полученное слово
									word_t word = L"";
									// Если это биграмма
									if(words.size() < 3){
										// Получаем слово
										word = words.front();
										// Получаем идентификатор слова
										const size_t idw = this->getIdw(word);
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(this->badwords.count(idw) < 1){
											// Если это юниграмма и её еще нет в словаре
											if((idw != (size_t) sign_t::start) &&
											((words.size() > 1) || (this->vocab.count(idw) < 1))) this->addWord(word);
										}
									}
									// Переходим по всему списку слов
									for(auto & item : words){
										// Получаем слово
										word = move(item);
										// Получаем идентификатор слова
										size_t idw = this->getIdw(word);
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(this->badwords.count(idw) < 1){
											// Если это неизвестное слово
											if(uid == idw){
												// Если неизвестное слово не установлено
												if(this->unknown == 0) seq.emplace_back(idw, 0);
												// Если неизвестное слово установлено
												else if(this->unknown > 0) seq.emplace_back(this->unknown, this->vocab.at(this->unknown).getUppers());
											// Добавляем слово в список последовательности
											} else seq.emplace_back(idw, ((words.size() > 1) && (this->usigns.count(idw) > 0) ? 0 : word.getUppers()));
										// Если слово найдено в всписке запрещённых
										} else {
											// Очищаем последовательность
											seq.clear();
											// Выходим из цикла
											break;
										}
									}
									// Добавляем последовательность в словарь
									if(!seq.empty()) this->arpa->set(seq, stof(weight), stof(backoff));
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
				actual = u_short(index / float(fileSize) * 100.0f);
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
void anyks::Toolkit::readVocab(const string & filename, function <void (const u_short)> status){
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Тип извлечения данных
		u_short type = 0;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Количество обработанных данных
		size_t index = 0, pos = 0, loc = 0;
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize){
			// Если слово получено
			if(!text.empty()){
				// Определяем тип считываемых данных
				switch(type){
					// Если получено обозначение заголовка
					case 0: if(text.compare("\\data\\") == 0) type++; break;
					// Если это тип считывания статистики
					case 1: {
						// Если мы дошли до считывания данных слов
						if(text.compare("\\words:") == 0) type++;
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
						if(text.compare("\\end\\") == 0) break;
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
								word_t word = move(this->alphabet->trim(text.substr(loc + 1, pos - (loc + 1))));
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
				actual = u_short(index / float(fileSize) * 100.0f);
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
void anyks::Toolkit::readNgram(const string & filename, function <void (const u_short)> status){
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Последовательность для добавления
		vector <pair_t> seq;
		// Список полученных слов последовательности
		vector <wstring> words;
		// Количество обработанных данных
		size_t index = 0, pos = 0, loc = 0;
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) sign_t::unk;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100, type = 0;
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize){
			// Если слово получено
			if(!text.empty()){
				// Определяем тип считываемых данных
				switch(type){
					// Если получено обозначение заголовка
					case 0: if(text.compare("\\data\\") == 0) type++; break;
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
						if(text.compare("\\end\\") == 0) break;
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
												// Если название признака получено, устанавливаем его регистры
												if(this->usigns.count(idw) > 0) uppers = 0;
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
												} else seq.emplace_back(idw, this->usigns.count(idw) > 0 ? 0 : word.getUppers());
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
				actual = u_short(index / float(fileSize) * 100.0f);
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
 * writeArpa Метод записи данных в файл arpa
 * @param filename адрес файла для записи
 * @param status   функция вывода статуса записи
 */
void anyks::Toolkit::writeArpa(const string & filename, function <void (const u_short)> status) const {
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
				data = move(this->alphabet->format("ngram %u=%u\n", i, count));
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
					data = move(this->alphabet->format("\n\\%u-grams:\n", i));
					// Выполняем запись данных в файл
					file.write(data.data(), data.size());
					// Выполняем извлечение данных n-граммы
					this->arpa->data(i, [&](const string & ngram){
						// Если строка получена
						if(!ngram.empty()){
							// Формируем тело n-граммы
							data = move(this->alphabet->format("%s\n", ngram.c_str()));
							// Выполняем запись данных в файл
							file.write(data.data(), data.size());
						}
						// Если функция вывода статуса передана
						if(status != nullptr){
							// Увеличиваем количество записанных n-грамм
							index++;
							// Выполняем расчёт текущего статуса
							actual = u_short(float(index) / float(counts) * 100.0f);
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
void anyks::Toolkit::writeVocab(const string & filename, function <void (const u_short)> status) const {
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
			data = move(this->alphabet->format("ad=%lld\ncw=%lld\nunq=%lld\n\n", this->info.ad, this->info.cw, this->info.unq));
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Формируем значение тела
			data = "\\words:\n";
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Переходим по всему списку слов
			for(auto & item : this->vocab){
				// Получаем метаданные слова
				auto meta = item.second.calc(this->info.ad, this->info.cw);
				// Получаем данные слова
				data = move(this->alphabet->format("%lld\t%s\t%lld | %lld | %f | %f | %f\n", item.first, item.second.real().c_str(), meta.oc, meta.dc, meta.tf, meta.idf, meta.wltf));
				// Выполняем запись данных в файл
				file.write(data.data(), data.size());
				// Если функция вывода статуса передана
				if(status != nullptr){
					// Увеличиваем количество записанных слов
					index++;
					// Выполняем расчёт текущего статуса
					actual = u_short(float(index) / float(this->info.unq) * 100.0f);
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
 * writeNgrams Метод записи данных в файлы ngrams
 * @param filename адрес файла для записи
 * @param status   функция вывода статуса записи
 */
void anyks::Toolkit::writeNgrams(const string & filename, function <void (const u_short)> status) const {
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
			data = move(this->alphabet->format("ad=%lld\ncw=%lld\nunq=%lld\n\n", this->info.ad, this->info.cw, this->info.unq));
			// Выполняем запись данных в файл
			file.write(data.data(), data.size());
			// Переходим по всей длине n-граммы
			for(u_short i = 1; i <= this->size; i++){
				// Получаем количество n-грамм
				count = this->arpa->count(i);
				// Формируем количество n-грамм
				data = move(this->alphabet->format("ngram %u=%u\n", i, count));
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
					data = move(this->alphabet->format("\n\\%u-grams:\n", i));
					// Выполняем запись данных в файл
					file.write(data.data(), data.size());
					// Выполняем извлечение данных n-граммы
					this->arpa->grams(i, [&](const string & ngram){
						// Если строка получена
						if(!ngram.empty()){
							// Формируем тело n-граммы
							data = move(this->alphabet->format("%s\n", ngram.c_str()));
							// Выполняем запись данных в файл
							file.write(data.data(), data.size());
						}
						// Если функция вывода статуса передана
						if(status != nullptr){
							// Увеличиваем количество записанных n-грамм
							index++;
							// Выполняем расчёт текущего статуса
							actual = u_short(float(index) / float(counts) * 100.0f);
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
 * modifyArpa Метод модификации arpa
 * @param filename адрес файла для чтения
 * @param flag     флаг модификации arpa
 * @param status   функция вывода статуса модификации
 */
void anyks::Toolkit::modifyArpa(const string & filename, modify_t flag, function <void (const u_short)> status){
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Последовательность для добавления
		vector <pair_t> seq;
		// Список полученных слов последовательности
		vector <wstring> words;
		// Список добавленных слов в словарь
		map <size_t, size_t> added;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Количество обработанных данных
		size_t index = 0, pos = 0, loc = 0;
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) sign_t::unk;
		/**
		 * cleanAddedFn Функция очистки добавленных слов
		 */
		auto cleanAddedFn = [&added, this]{
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
		auto organizationSeqFn = [&added, &seq, this](const vector <wstring> & words){
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
					if((idw > 0) && (idw < noID) && (this->badwords.count(idw) < 1)){
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
							// Если название признака получено, устанавливаем его регистры
							if(this->usigns.count(idw) > 0) uppers = 0;
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
								this->addWord(word);
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
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize){
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
											if(!this->arpa->emplace(seq, stof(weight))) cleanAddedFn();
										// Если это изменение частот последовательности
										} else if(flag == modify_t::change) {
											// Выполняем инкрементацию частот
											this->arpa->inc(seq, stof(weight));
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
								if((idw > 0) && (idw < noID)) seq[i] = idw;
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
										if((idw > 0) && (idw < noID)){
											// Устанавливаем идентификатор слова
											seq2[i] = idw;
											// Если слова нет в словаре и это не признак, добавляем в словарь
											if(this->arpa->event(idw) && (this->vocab.count(idw) < 1)){
												// Если слово еще не добавлено
												if(added.count(idw) < 1)
													// Запоминаем что слово добавили
													added.emplace(idw, 1);
												// Иначе увеличиваем количество слов
												else added.at(idw)++;
												// Добавляем слово в словарь
												this->addWord(words[i]);
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
				actual = u_short(index / float(fileSize) * 100.0f);
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
 * saveArpa Метод извлечения данных n-грамм в бинарном виде
 * @param callback функция обратного вызова
 * @param arpa     нужно добавить только данные arpa
 */
void anyks::Toolkit::saveArpa(function <void (const vector <char> &, const u_short)> callback, const bool arpa) const {
	// Выполняем извлечение данных arpa в бинарном виде
	this->arpa->save(arpa, callback);
}
/**
 * prune Метод прунинга языковой модели
 * @param threshold порог частоты прунинга
 * @param mingram   значение минимальной n-граммы за которую нельзя прунить
 * @param status    функция вывода статуса обучения
 */
void anyks::Toolkit::prune(const double threshold, const u_short mingram, function <void (const u_short)> status) const {
	// Выполняем прунинг arpa
	this->arpa->prune(threshold, mingram, status);
}
/**
 * readMaps Метод добавления - объединения карт последовательностей
 * @param path   адрес где лежат map файлы
 * @param status функция вывода статуса чтения
 * @param delim  разделитель последовательностей
 */
void anyks::Toolkit::readMaps(const string & path, function <void (const u_short)> status, const string & delim){
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
		// Определяем разрешены ли неизвестные слова
		const bool allowUnk = this->isOption(options_t::allowUnk);
		// Переходим по всему списку файлов в каталоге
		fsys_t::rdir(fullpath, "map", [&](const string & filename, const uintmax_t dirSize){
			// Выполняем считывание всех строк текста
			fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize){
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
								// Проверяем является ли слово пользовательским признаком
								bool isUsign = (this->usigns.count(idw) > 0);
								// Проверяем отсутствует ли слово в списке запрещённых слов
								bool isAllow = (this->badwords.count(idw) < 1);
								/**
								 * Если это спец-слово, или слово существует в словаре оно также разрешено,
								 * или в arpa разрешён учёт неизвестных слов
								 */
								if(noEvent || isUsign || ((isWord && isAllow) || allowUnk)){
									/**
									 * Если это не спец-слово и слово не существует в словаре или запрещено,
									 * устанавливаем идентификатор неизвестного слова
									 */
									if(!noEvent && !isUsign && (!isWord || !isAllow)) idw = (size_t) sign_t::unk;
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
						actual = u_short(index / float(dirSize) * 100.0f);
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
 * readMap Метод чтения карты последовательности из файла
 * @param filename адрес map файла карты последовательности
 * @param status   функция вывода статуса чтения
 * @param delim    разделитель последовательностей
 */
void anyks::Toolkit::readMap(const string & filename, function <void (const u_short)> status, const string & delim){
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
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize){
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
							// Проверяем является ли слово пользовательским признаком
							bool isUsign = (this->usigns.count(idw) > 0);
							// Проверяем отсутствует ли слово в списке запрещённых слов
							bool isAllow = (this->badwords.count(idw) < 1);
							/**
							 * Если это спец-слово, или слово существует в словаре оно также разрешено,
							 * или в arpa разрешён учёт неизвестных слов
							 */
							if(noEvent || isUsign || ((isWord && isAllow) || allowUnk)){
								/**
								 * Если это не спец-слово и слово не существует в словаре или запрещено,
								 * устанавливаем идентификатор неизвестного слова
								 */
								if(!noEvent && !isUsign && (!isWord || !isAllow)) idw = (size_t) sign_t::unk;
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
					actual = u_short(index / float(fileSize) * 100.0f);
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
 * writeMap Метод записи карты последовательности в файл
 * @param filename адрес map файла карты последовательности
 * @param status   функция вывода статуса записи
 * @param delim    разделитель последовательностей
 */
void anyks::Toolkit::writeMap(const string & filename, function <void (const u_short)> status, const string & delim) const {
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
 * init Метод инициализации языковой модели
 * @param algorithm алгоритм расчёта языковой модели
 * @param modified  количество уже изменённых младших заказов
 * @param prepares  необходимость изменения счёта, после вычисления
 * @param mod       дополнительный параметр дельты
 */
void anyks::Toolkit::init(const algorithm_t algorithm, const bool modified, const bool prepares, const double mod){
	// Если память под arpa еще не выделена
	if((this->arpa == nullptr) && (this->python == nullptr)){
		// Экранируем возможность ошибки памяти
		try {
			/**
			 * Функция получения слова по его идентификатору
			 * @param idw идентификатор слова
			 * @return    искомое слово
			 */
			auto getWordFn = [this](const size_t idw){
				// Результат работы функции
				const word_t * result = nullptr;
				// Если нужно проверить пользовательские признаки
				if(!this->isOption(options_t::notUserSign) && (this->usigns.count(idw) > 0)){
					// Получаем слово обозначения пользовательского признака
					result = &this->usigns.at(idw).name;
				// Если это не пользовательский признак
				} else {
					// Ищем слово по идентификатору
					auto it = this->vocab.find(idw);
					// Если слово найдено
					if(it != this->vocab.end()) result = &it->second;
				}
				// Выводим результат
				return result;
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
			if(((this->scripts.count(2) > 0) && !this->usigns.empty()) || (this->scripts.count(1) > 0)){
				// Создаём объект для работы с python
				this->python = new python_t(&this->idw);
				// Если нужно активировать режим отладки
				if(this->isOption(options_t::debug)) this->python->setDebug();
				// Ищем скрипт обработки слов
				auto it = this->scripts.find(1);
				// Если скрипт обработки слов установлен
				if(it != this->scripts.end()){
					// Запоминаем идентификатор скрипта
					it->second.second = this->python->add(it->second.first, 2);
				}
				// Ищем скрипт обработки пользовательских признаков
				it = this->scripts.find(2);
				// Если скрипт обработки пользовательских признаков установлен
				if((it != this->scripts.end()) && !this->usigns.empty()){
					// Выполняем добавление скрипта
					const size_t sid = this->python->add(it->second.first, 2);
					// Переходим по всему списку пользовательских признаков
					for(auto & sign : this->usigns){
						// Добавляем в пользовательский признак функцию проверки
						sign.second.test = [sid, this](const string & sign, const string & word){
							// Результат работы функции
							bool result = false;
							// Если слово и признак переданы
							if(!sign.empty() && !word.empty()){
								// Выполняем скрипт
								const wstring & res = this->python->run(sid, {sign, word});
								// Проверяем результат
								result = (this->alphabet->toLower(res).compare(L"ok") == 0);
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
			// Активируем расчёт всех возможных грамм
			if(this->isOption(options_t::allGrams)) this->arpa->setOption(arpa_t::options_t::allGrams);
			// Активируем перевод слов в arpa к нижнему регистру
			if(this->isOption(options_t::lowerCase)) this->arpa->setOption(arpa_t::options_t::lowerCase);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выводим сообщение об ошибке, если режим отладки включён
			if(this->isOption(options_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "bad alloc for init model language");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * Toolkit Конструктор
 * @param size размерность n-грамм
 */
anyks::Toolkit::Toolkit(const u_short size){
	// Устанавливаем размерность n-грамм
	this->setSize(size);
}
/**
 * Toolkit Конструктор
 * @param alphabet объект алфавита
 * @param size     размерность n-грамм
 */
anyks::Toolkit::Toolkit(const alphabet_t * alphabet, const u_short size){
	// Устанавливаем размерность n-грамм
	this->setSize(size);
	// Устанавливаем алфавит
	this->setAlphabet(alphabet);
}
/**
 * ~Toolkit Деструктор
 */
anyks::Toolkit::~Toolkit(){
	// Очищаем языковую модель
	this->clear();
	// Очищаем выделенную память под arpa
	if(this->arpa != nullptr) delete this->arpa;
	// Очищаем выделенную память под python
	if(this->python != nullptr) delete this->python;
}

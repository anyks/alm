/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include "alm.hpp"

/**
 * perplexity Метод расчёта перплексии текста
 * @param  text текст для расчёта
 * @return      результат расчёта
 */
const anyks::ppl_t anyks::LM::perplexity(const wstring & text) const noexcept {
	// Выполняем расчёт перплексии
	auto ppl = this->alm->perplexity(text);
	// Выполняем расчёт перплексии
	return (* reinterpret_cast <const ppl_t *> (&ppl));
}
/**
 * perplexity Метод расчёта перплексии
 * @param  seq список последовательностей
 * @return     результат расчёта
 */
const anyks::ppl_t anyks::LM::perplexity(const vector <size_t> & seq) const noexcept {
	// Выполняем расчёт перплексии
	auto ppl = this->alm->perplexity(seq);
	// Выполняем расчёт перплексии
	return (* reinterpret_cast <const ppl_t *> (&ppl));
}
/**
 * pplByFiles Метод чтения расчёта перплексии по файлу или группе файлов
 * @param path адрес каталога или файла для расчёта перплексии
 * @param ext  расширение файлов в каталоге (если адрес передан каталога)
 * @return     результат расчёта
 */
const anyks::ppl_t anyks::LM::pplByFiles(const string & path, const string & ext) const noexcept {
	// Выполняем расчёт перплексии по файлу или группе файлов
	auto ppl = this->alm->pplByFiles(path, [this](const u_short status){
		// Выводим статус подсчёта
		if(this->callback != nullptr) this->callback->status(status);
	}, ext);
	// Выполняем расчёт перплексии
	return (* reinterpret_cast <const ppl_t *> (&ppl));
}
/**
 * pplConcatenate Метод объединения перплексий
 * @param ppl1 первая перплексия
 * @param ppl2 вторая перплексия
 * @return     объединённая перплексия
 */
const anyks::ppl_t anyks::LM::pplConcatenate(const ppl_t & ppl1, const ppl_t & ppl2) const noexcept {
	// Выполняем расчёт перплексии
	auto ppl = this->alm->pplConcatenate(* reinterpret_cast <const alm_t::ppl_t *> (&ppl1), * reinterpret_cast <const alm_t::ppl_t *> (&ppl2));
	// Выполняем расчёт перплексии
	return (* reinterpret_cast <const ppl_t *> (&ppl));
}
/**
 * check Метод проверки существования последовательности
 * @param text     текст для проверки существования
 * @param accurate режим точной проверки
 * @return         результат проверки
 */
const pair <bool, size_t> anyks::LM::check(const wstring & text, const bool accurate) const noexcept {
	// Выполняем проверку существования последовательности
	return this->alm->check(text, accurate);
}
/**
 * check Метод проверки существования последовательности
 * @param seq      список слов последовательности
 * @param accurate режим точной проверки
 * @return         результат проверки
 */
const pair <bool, size_t> anyks::LM::check(const vector <size_t> & seq, const bool accurate) const noexcept {
	// Выполняем проверку существования последовательности
	return this->alm->check(seq, accurate);
}
/**
 * fixUppers Метод исправления регистров в тексте
 * @param text текст для исправления регистров
 * @return     текст с исправленными регистрами слов
 */
const wstring anyks::LM::fixUppers(const wstring & text) const noexcept {
	// Выполняем исправление регистров в тексте
	return this->alm->fixUppers(text);
}
/**
 * context Метод сборки текстового контекста из последовательности
 * @param seq  последовательность слов для сборки контекста
 * @param nwrd флаг разрешающий вывод системных токенов
 * @return     собранный текстовый контекст
 */
const wstring anyks::LM::context(const vector <size_t> & seq, const bool nwrd) const noexcept {
	// Выполняем сборку текстового контекста из последовательности
	return this->alm->context(seq, nwrd);
}
/**
 * clear Метод очистки
 */
void anyks::LM::clear(){
	// Выполняем очистику языковой модели
	this->alm->clear();
	// Выполняем очистику алфавита
	this->alphabet->clear();
	// Выполняем очистку токенизатора
	this->tokenizer->clear();
}
/**
 * initPython Метод инициализации скриптов питона
 */
void anyks::LM::initPython(){
	// Выполняем инициализацию скриптов питона
	this->alm->initPython();
}
/**
 * clearBadwords Метод очистки списка плохих слов
 */
void anyks::LM::clearBadwords() noexcept {
	// Выполняем очистку списка плохих слов
	this->alm->clearBadwords();
}
/**
 * clearGoodwords Метод очистки списка хороших слов
 */
void anyks::LM::clearGoodwords() noexcept {
	// Выполняем очистку списка хороших слов
	this->alm->clearGoodwords();
}
/**
 * clearUserTokens Метод очистки пользовательских токенов
 */
void anyks::LM::clearUserTokens() noexcept {
	// Выполняем очистку пользовательских токенов
	this->alm->clearUserTokens();
}
/**
 * setAllTokenUnknown Метод установки всех токенов идентифицируемых как <unk>
 */
void anyks::LM::setAllTokenUnknown() noexcept {
	// Выполняем установку всех токенов идентифицируемых как <unk>
	this->alm->setAllTokenUnknown();
}
/**
 * setAllTokenDisable Метод установки всех токенов как не идентифицируемых
 */
void anyks::LM::setAllTokenDisable() noexcept {
	// Выполняем установку всех токенов как не идентифицируемых
	this->alm->setAllTokenDisable();
}
/**
 * addBadword Метод добавления идентификатора похого слова в список
 * @param idw идентификатор слова
 */
void anyks::LM::addBadword(const size_t idw) noexcept {
	// Выполняем добавление идентификатора похого слова в список
	this->alm->addBadword(idw);
}
/**
 * addGoodword Метод добавления идентификатора хорошего слова в список
 * @param idw идентификатор слова
 */
void anyks::LM::addGoodword(const size_t idw) noexcept {
	// Выполняем добавление идентификатора хорошего слова в список
	this->alm->addGoodword(idw);
}
/**
 * addBadword Метод добавления похого слова в список
 * @param word слово для добавления
 */
void anyks::LM::addBadword(const string & word) noexcept {
	// Выполняем добавление похого слова в список
	this->alm->addBadword(word);
}
/**
 * addGoodword Метод добавления хорошего слова в список
 * @param word слово для добавления
 */
void anyks::LM::addGoodword(const string & word) noexcept {
	// Выполняем добавление хорошего слова в список
	this->alm->addGoodword(word);
}
/**
 * sentences Метод генерации предложений
 */
void anyks::LM::sentences() const noexcept {
	// Выполняем генерацию предложений
	this->alm->sentences([this](const wstring & text){
		// Выводим статус подсчёта
		if(this->callback != nullptr)
			// Выполняем сборку предложений
			return this->callback->sentences(text);
		// Иначе прекращаем обработку
		else return false;
	});
}
/**
 * setUnknown Метод установки неизвестного слова
 * @param word слово для добавления
 */
void anyks::LM::setUnknown(const string & word) noexcept {
	// Выполняем установку неизвестного слова
	this->alm->setUnknown(word);
}
/**
 * find Метод поиска n-грамм в тексте
 * @param text текст в котором необходимо найти n-граммы
 */
void anyks::LM::find(const wstring & text) const noexcept {
	// Выполняем поиск n-грамм в тексте
	this->alm->find(text, [this](const wstring & text){
		// Выводим результат
		if(this->callback != nullptr) this->callback->text(this->alphabet->convert(text));
	});
}
/**
 * setOption Метод установки опций модуля
 * @param option опция для установки
 */
void anyks::LM::setOption(const options_t option) noexcept {
	// Выполняем установку опций модуля
	this->alm->setOption((alm_t::options_t) option);
}
/**
 * unsetOption Метод отключения опции модуля
 * @param option опция для отключения
 */
void anyks::LM::unsetOption(const options_t option) noexcept {
	// Выполняем отключение опции модуля
	this->alm->unsetOption((alm_t::options_t) option);
}
/**
 * setLogfile Метод установки файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::LM::setLogfile(const string & logfile) noexcept {
	// Если адрес логов передан
	if(!logfile.empty()){
		// Запоминаем адрес файла логов
		this->logfile = logfile;
		// Выполняем установку файла для вывода логов
		this->alm->setLogfile(this->logfile.c_str());
	}
}
/**
 * setOOvFile Метом установки файла для сохранения OOV слов
 * @param oovfile адрес файла для сохранения oov слов
 */
void anyks::LM::setOOvFile(const string & oovfile) noexcept {
	// Выполняем установку файла для сохранения OOV слов
	if(!oovfile.empty()) this->alm->setOOvFile(oovfile.c_str());
}
/**
 * setUserToken Метод добавления токена пользователя
 * @param name слово - обозначение токена
 */
void anyks::LM::setUserToken(const string & name) noexcept {
	// Выполняем добавление токена пользователя
	this->alm->setUserToken(name);
}
/**
 * setThreads Метод установки количества потоков
 * @param threads количество потоков для работы
 */
void anyks::LM::setThreads(const size_t threads) noexcept {
	// Выполняем установку количества потоков
	this->alm->setThreads(threads);
}
/**
 * setWordScript Метод установки скрипта обработки слов
 * @param script скрипт python обработки слов
 */
void anyks::LM::setWordScript(const string & script) noexcept {
	// Выполняем установку скрипта обработки слов
	this->alm->setWordScript(script);
}
/**
 * setTokenUnknown Метод установки списка токенов которых нужно идентифицировать как <unk>
 * @param options список токенов которых нужно идентифицировать как <unk>
 */
void anyks::LM::setTokenUnknown(const string & options) noexcept {
	// Выполняем установку списка токенов которых нужно идентифицировать как <unk>
	this->alm->setTokenUnknown(options);
}
/**
 * setTokenDisable Метод установки списка не идентифицируемых токенов
 * @param options список не идентифицируемых токенов
 */
void anyks::LM::setTokenDisable(const string & options) noexcept {
	// Выполняем установку списка не идентифицируемых токенов
	this->alm->setTokenDisable(options);
}
/**
 * setUserTokenMethod Метод добавления функции обработки пользовательского токена
 * @param name слово - обозначение токена
 */
void anyks::LM::setUserTokenMethod(const string & name) noexcept {
	// Выполняем добавление функции обработки пользовательского токена
	this->alm->setUserTokenMethod(name, [this](const string & token, const string & word){
		// Если функция обратного вызова существует
		if(this->callback != nullptr)
			// Выполняем обработку пользовательского токена
			return this->callback->utoken(token, word);
		// Иначе просто пропускаем
		else return false;
	});
}
/**
 * setUserTokenScript Метод установки скрипта обработки пользовательских токенов
 * @param script скрипт python обработки пользовательских токенов
 */
void anyks::LM::setUserTokenScript(const string & script) noexcept {
	// Выполняем установку скрипта обработки пользовательских токенов
	this->alm->setUserTokenScript(script);
}
/**
 * setBadwords Метод установки списка идентификаторов плохих слов в список
 * @param badwords список идентификаторов плохих слов
 */
void anyks::LM::setBadwords(const set <size_t> & badwords) noexcept {
	// Выполняем установку списка идентификаторов плохих слов в список
	this->alm->setBadwords(badwords);
}
/**
 * setBadwords Метод установки списка плохих слов в список
 * @param badwords список плохих слов
 */
void anyks::LM::setBadwords(const vector <string> & badwords) noexcept {
	// Выполняем установку списка плохих слов в список
	this->alm->setBadwords(badwords);
}
/**
 * setTokensUnknown Метод установки списка токенов приводимых к <unk>
 * @param tokens список токенов для установки
 */
void anyks::LM::setTokensUnknown(const set <size_t> & tokens) noexcept {
	// Если токены переданы
	if(!tokens.empty()){
		// Сприсок токенов
		set <token_t> tmp;
		// Переходим по всему списку токенов
		for(auto & token : tokens) tmp.emplace(token_t(token));
		// Выполняем установку списка токенов приводимых к <unk>
		this->alm->setTokensUnknown(tmp);
	}
}
/**
 * setTokensDisable Метод установки списка запрещённых токенов
 * @param tokens список токенов для установки
 */
void anyks::LM::setTokensDisable(const set <size_t> & tokens) noexcept {
	// Если токены переданы
	if(!tokens.empty()){
		// Сприсок токенов
		set <token_t> tmp;
		// Переходим по всему списку токенов
		for(auto & token : tokens) tmp.emplace(token_t(token));
		// Выполняем установку списка запрещённых токенов
		this->alm->setTokensDisable(tmp);
	}
}
/**
 * setGoodwords Метод установки списка идентификаторов хороших слов в список
 * @param goodwords список идентификаторов хороших слов
 */
void anyks::LM::setGoodwords(const set <size_t> & goodwords) noexcept {
	// Выполняем установку списка идентификаторов хороших слов в список
	this->alm->setGoodwords(goodwords);
}
/**
 * setGoodwords Метод установки списка хороших слов в список
 * @param goodwords список хороших слов
 */
void anyks::LM::setGoodwords(const vector <string> & goodwords) noexcept {
	// Выполняем установку списка хороших слов в список
	this->alm->setGoodwords(goodwords);
}
/**
 * read Метод чтения данных из файла arpa
 * @param filename адрес файла для чтения
 * @param meta метаданные бинарного контейнера в формате json
 */
void anyks::LM::read(const string & filename, const string & meta) noexcept {
	// Если адрес файла передан
	if(!filename.empty()){
		// Если это бинарный контейнер
		if(filename.rfind(".alm") != string::npos){
			// Создаём объект бинарного контейнера
			ablm_t ablm(filename, this->alm, this->alphabet, this->tokenizer, (!this->logfile.empty() ? this->logfile.c_str() : nullptr));
			// Если метеданные получены
			if(!meta.empty()) ablm.setMeta(json::parse(meta));
			// Выполняем инициализацию словаря
			ablm.init();
			// Выполняем чтение бинарных данных
			ablm.readAlm([this](const u_short status){
				// Выводим статистику если требуется
				if(this->callback != nullptr) this->callback->status(status);
			});
		// Иначе если это arpa, загружаем обычным способом
		} else {
			// Выполняем чтение arpa файла
			this->alm->read(filename, [this](const u_short status){
				// Выводим статистику если требуется
				if(this->callback != nullptr) this->callback->status(status);
			});
		}
	}
}
/**
 * getUppers Метод извлечения регистров для каждого слова
 * @param seq  последовательность слов для сборки контекста
 * @param upps список извлечённых последовательностей
 */
void anyks::LM::getUppers(const vector <size_t> & seq, vector <size_t> & upps) const noexcept {
	// Выполняем извлечение регистров для каждого слова
	this->alm->getUppers(seq, upps);
}
/**
 * sentencesToFile Метод сборки указанного количества предложений и записи в файл
 * @param counts   количество предложений для сборки
 * @param filename адрес файла для записи результата
 */
void anyks::LM::sentencesToFile(const size_t counts, const string & filename) const noexcept {
	// Выполняем сборку указанного количества предложений
	this->alm->sentencesToFile(counts, filename, [this](const u_short status){
		// Выводим статус подсчёта
		if(this->callback != nullptr) this->callback->status(status);
	});
}
/**
 * findByFiles Метод поиска n-грамм в текстовом файле
 * @param path     адрес каталога или файла для обработки
 * @param filename адрес файла для записи результата
 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
 */
void anyks::LM::findByFiles(const string & path, const string & filename, const string & ext) const noexcept {
	// Выполняем поиск n-грамм в текстовом файле
	this->alm->findByFiles(path, filename, [this](const string & filename, const u_short status){
		// Выводим статус подсчёта
		if(this->callback != nullptr) this->callback->status(status);
	}, ext);
}
/**
 * fixUppersByFiles Метод исправления регистров текста в текстовом файле
 * @param path     адрес каталога или файла для обработки
 * @param filename адрес файла для записи результата
 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
 */
void anyks::LM::fixUppersByFiles(const string & path, const string & filename, const string & ext) const noexcept {
	// Выполняем исправление регистров текста
	this->alm->fixUppersByFiles(path, filename, [this](const string & filename, const u_short status){
		// Выводим статус подсчёта
		if(this->callback != nullptr) this->callback->status(status);
	}, ext);
}
/**
 * countsByFiles Метод подсчёта количества n-грамм в текстовом файле
 * @param path     адрес каталога или файла для обработки
 * @param filename адрес файла для записи результата
 * @param ngrams   размер n-граммы для подсчёта
 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
 */
void anyks::LM::countsByFiles(const string & path, const string & filename, const size_t ngrams, const string & ext) const noexcept {
	// Выполняем подсчёт количества n-грамм
	this->alm->countsByFiles(path, filename, ngrams, [this](const string & filename, const u_short status){
		// Выводим статус подсчёта
		if(this->callback != nullptr) this->callback->status(status);
	}, ext);
}
/**
 * checkByFiles Метод проверки существования последовательности в текстовом файле
 * @param path     адрес каталога или файла для обработки
 * @param filename адрес файла для записи результата
 * @param accurate режим точной проверки
 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
 */
void anyks::LM::checkByFiles(const string & path, const string & filename, const bool accurate, const string & ext) const noexcept {
	// Выполняем проверку существования последовательности
	this->alm->checkByFiles(path, filename, accurate, [this](const string & filename, const u_short status){
		// Выводим статус подсчёта
		if(this->callback != nullptr) this->callback->status(status);
	}, ext);
}
/**
 * getAlphabet Метод получения алфавита языка
 * @return алфавит языка
 */
const string anyks::LM::getAlphabet() const noexcept {
	// Выводим данные алфавита
	return this->alphabet->get();
}
/**
 * rest Метод исправления и детектирования слов со смешенными алфавитами
 * @param  word слово для проверки и исправления
 * @return      результат исправления
 */
const wstring anyks::LM::rest(const wstring & word) const noexcept {
	// Создаем слово для исправления
	wstring tmp = word;
	// Выполняем исправление слов
	if(this->alphabet->rest(tmp)) return tmp;
	// Выводим результат
	else return word;
}
/**
 * arabic2Roman Метод перевода арабских чисел в римские
 * @param  number арабское число от 1 до 4999
 * @return        римское число
 */
const wstring anyks::LM::arabic2Roman(const size_t number) const noexcept {
	// Выполняем конвертацию чисел
	return this->alphabet->arabic2Roman(number);
}
/**
 * arabic2Roman Метод перевода арабских чисел в римские
 * @param  word арабское число от 1 до 4999
 * @return      римское число
 */
const wstring anyks::LM::arabic2Roman(const wstring & word) const noexcept {
	// Выполняем конвертацию чисел
	return this->alphabet->arabic2Roman(word);
}
/**
 * delPunctInWord Метод очистки текста от всех знаков препинаний
 * @param word слово для очистки
 * @return     текст без запрещенных символов
 */
const wstring anyks::LM::delPunctInWord(const wstring & word) const noexcept {
	// Выполняем очистку текста
	return this->alphabet->delPunctInWord(word);
}
/**
 * delBrokenInWord Метод очистки текста от всех символов кроме разрешенных
 * @param word слово для очистки
 * @return     текст без запрещенных символов
 */
const wstring anyks::LM::delBrokenInWord(const wstring & word) const noexcept {
	// Выполняем очистку текста
	return this->alphabet->delBrokenInWord(word);
}
/**
 * delHyphenInWord Метод удаления дефиса из слова
 * @param word слово в котором нужно удалить дефис
 * @return     слово без дефиса
 */
const wstring anyks::LM::delHyphenInWord(const wstring & word) const noexcept {
	// Выполняем удаление дефисов
	return this->alphabet->delHyphenInWord(word);
}
/**
 * countAlphabet Метод получения количества букв в словаре
 * @return количество букв в словаре
 */
const size_t anyks::LM::countAlphabet() const noexcept {
	// Выводим количество букв в словаре
	return this->alphabet->count();
}
/**
 * roman2Arabic Метод перевода римских цифр в арабские
 * @param  word римское число
 * @return      арабское число
 */
const size_t anyks::LM::roman2Arabic(const wstring & word) const noexcept {
	// Выводим конвертацию чисел
	return this->alphabet->roman2Arabic(word);
}
/**
 * countLetter Метод подсчета количества указанной буквы в слове
 * @param word   слово в котором нужно подсчитать букву
 * @param letter букву которую нужно подсчитать
 * @return       результат подсчёта
 */
const size_t anyks::LM::countLetter(const wstring & word, const wstring & letter) const noexcept {
	// Выводим результат подсчёта
	return this->alphabet->countLetter(word, letter.front());
}
/**
 * isUrl Метод проверки соответствия слова url адресу
 * @param word слово для проверки
 * @return     результат проверки
 */
const bool anyks::LM::isUrl(const wstring & word) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isUrl(word);
}
/**
 * isLatian Метод проверки является ли строка латиницей
 * @param str строка для проверки
 * @return    результат проверки
 */
const bool anyks::LM::isLatian(const wstring & str) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isLatian(str);
}
/**
 * isMath Метод определения математических операий
 * @param letter буква для проверки
 * @return       результат проверки
 */
const bool anyks::LM::isMath(const wstring & letter) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isMath(letter.front());
}
/**
 * isNumber Метод проверки является ли слово числом
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::LM::isNumber(const wstring & word) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isNumber(word);
}
/**
 * isUpper Метод проверки символ на верхний регистр
 * @param letter буква для проверки
 * @return       результат проверки
 */
const bool anyks::LM::isUpper(const wstring & letter) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isUpper(letter.front());
}
/**
 * isPunct Метод проверки является ли буква, знаком препинания
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::LM::isPunct(const wstring & letter) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isPunct(letter.front());
}
/**
 * isSpace Метод проверки является ли буква, пробелом
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::LM::isSpace(const wstring & letter) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isSpace(letter.front());
}
/**
 * isANumber Метод проверки является ли косвенно слово числом
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::LM::isANumber(const wstring & word) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isANumber(word);
}
/**
 * isAllowed Метод проверки соответствия слова словарю
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::LM::isAllowed(const wstring & word) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isAllowed(word);
}
/**
 * isDecimal Метод проверки является ли слово дробным числом
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::LM::isDecimal(const wstring & word) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isDecimal(word);
}
/**
 * isSpecial Метод определения спец-символа
 * @param letter буква для проверки
 * @return       результат проверки
 */
const bool anyks::LM::isSpecial(const wstring & letter) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isSpecial(letter.front());
}
/**
 * isIsolation Метод определения знака изоляции (кавычки, скобки)
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::LM::isIsolation(const wstring & letter) const noexcept {
	// Выводим результат проверки
	return this->alphabet->isIsolation(letter.front());
}
/**
 * checkHome2 Метод проверки слова на Дом-2
 * @param  word слово для проверки
 * @return      результат работы метода
 */
const bool anyks::LM::checkHome2(const wstring & word) const noexcept {
	// Выводим результат проверки
	return this->alphabet->checkHome2(word);
}
/**
 * checkLatian Метод проверки наличия латинских символов в строке
 * @param str строка для проверки
 * @return    результат проверки
 */
const bool anyks::LM::checkLatian(const wstring & str) const noexcept {
	// Выводим результат проверки
	return this->alphabet->checkLatian(str);
}
/**
 * checkHyphen Метод проверки наличия дефиса в строке
 * @param str строка для проверки
 * @return    результат проверки
 */
const bool anyks::LM::checkHyphen(const wstring & str) const noexcept {
	// Выводим результат проверки
	return this->alphabet->checkHyphen(str);
}
/**
 * checkSimilars Метод проверки на симиляции букв с другими языками
 * @param  str строка для проверки
 * @return     результат проверки
 */
const bool anyks::LM::checkSimilars(const wstring & str) const noexcept {
	// Выводим результат проверки
	return this->alphabet->checkSimilars(str);
}
/**
 * checkLetter Метод проверки соответствии буквы
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::LM::checkLetter(const wstring & letter) const noexcept {
	// Выводим результат проверки
	return this->alphabet->check(letter.front());
}
/**
 * getSubstitutes Метод извлечения букв для исправления слов из смешанных алфавитов
 * @param return список букв разных алфавитов соответствующих друг-другу
 */
const map <string, string> & anyks::LM::getSubstitutes() const noexcept {
	// Выводим список букв
	return this->alphabet->getSubstitutes();
}
/**
 * urls Метод извлечения координат url адресов в строке
 * @param text текст для извлечения url адресов
 * @return     список координат с url адресами
 */
const map <size_t, size_t> anyks::LM::urls(const wstring & text) const noexcept {
	// Выводим координаты url адреса
	return this->alphabet->urls(text);
}
/**
 * checkHypLat Метод поиска дефиса и латинского символа
 * @param str строка для проверки
 * @return    результат проверки
 */
const pair <bool, bool> anyks::LM::checkHypLat(const wstring & str) const noexcept {
	// Выводим результат првоерки
	return this->alphabet->checkHypLat(str);
}
/**
 * setzone Метод установки пользовательской зоны
 * @param zone пользовательская зона
 */
void anyks::LM::setzone(const wstring & zone) noexcept {
	// Устанавливаем доменную зону
	this->alphabet->setzone(zone);
}
/**
 * setCallback Метод установки каллбека
 * @param callback каллбек для установки
 */
void anyks::LM::setCallback(callback_t * callback) noexcept {
	// Устанавливаем каллбек функции
	this->callback = callback;
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet алфавит символов для текущего языка
 */
void anyks::LM::setAlphabet(const string & alphabet) noexcept {
	// Устанавливаем алфавит
	this->alphabet->set(alphabet);
}
/**
 * setlocale Метод установки локали
 * @param locale локализация приложения
 */
void anyks::LM::setlocale(const string & locale) noexcept {
	// Устанавливаем локаль алфавита
	this->alphabet->setlocale(locale);
}
/**
 * setSubstitutes Метод установки букв для исправления слов из смешанных алфавитов
 * @param letters список букв разных алфавитов соответствующих друг-другу
 */
void anyks::LM::setSubstitutes(const map <string, string> & letters) noexcept {
	// Выполняем установку букв
	this->alphabet->setSubstitutes(letters);
}
/**
 * idw Метод извлечения идентификатора слова
 * @param  word слово для получения идентификатора
 * @return      идентификатор слова
 */
const size_t anyks::LM::idw(const wstring & word) const noexcept {
	// Выводим идентификатор слова
	return this->tokenizer->idw(word);
}
/**
 * idt Метод извлечения идентификатора токена
 * @param  word слово для получения идентификатора
 * @return      идентификатор токена
 */
const size_t anyks::LM::idt(const wstring & word) const noexcept {
	// Выводим идентификатор токена
	return size_t(this->tokenizer->idt(word));
}
/**
 * isAllowApostrophe Метод проверки разрешения апострофа
 * @return результат проверки
 */
const bool anyks::LM::isAllowApostrophe() const noexcept {
	// Выводрим результат проверки
	return this->tokenizer->isAllowApostrophe();
}
/**
 * isAbbr Метод проверки слова на соответствие аббревиатуры
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::LM::isAbbr(const wstring & word) const noexcept {
	// Выполняем проверку аббревиатуры
	return this->tokenizer->isAbbr(word);
}
/**
 * restore Метод восстановления текста из контекста
 * @param context токенизированный контекст
 * @return        результирующий текст
 */
const wstring anyks::LM::restore(const vector <wstring> & context) const noexcept {
	// Восстанавливаем текст
	return this->tokenizer->restore(context);
}
/**
 * fti Метод удаления дробной части числа
 * @param  num   число для обработки
 * @param  count количество символов после запятой
 * @return       число без дробной части
 */
const size_t anyks::LM::fti(const double num, const size_t count) const noexcept {
	// Выполняем удаление дробной части
	return this->tokenizer->fti(num, count);
}
/**
 * switchAllowApostrophe Метод разрешения или запрещения апострофа как части слова
 */
void anyks::LM::switchAllowApostrophe() noexcept {
	// Выполняем переключение разрешения апострофа
	this->tokenizer->switchAllowApostrophe();
}
/**
 * setAbbr Метод добавления аббревиатуры
 * @param word слово для добавления
 */
void anyks::LM::setAbbr(const wstring & word) noexcept {
	// Устанавливаем аббревиатуру
	this->tokenizer->setAbbr(word);
}
/**
 * jsonToText Метод преобразования текста в формате json в текст
 * @param text текст для преобразования в формате json
 */
void anyks::LM::jsonToText(const string & text) const noexcept {
	// Запускаем обработку конвертации
	this->tokenizer->jsonToText(text, [this](const string & text){
		// Выводим результат
		if(this->callback != nullptr) this->callback->text(text);
	});
}
/**
 * textToJson Метод преобразования текста в json
 * @param text текст для преобразования
 */
void anyks::LM::textToJson(const string & text) const noexcept {
	// Запускаем обработку конвертации
	this->tokenizer->textToJson(text, [this](const string & text){
		// Выводим результат
		if(this->callback != nullptr) this->callback->text(text);
	});
}
/**
 * tokenization Метод разбивки текста на токены
 * @param text входной текст для обработки
 */
void anyks::LM::tokenization(const wstring & text) const noexcept {
	// Запускаем обработку токенизации
	this->tokenizer->run(text, [this](const wstring & word, const vector <string> & context, const bool reset, const bool stop){
		// Выводим результат токенизатора
		return this->callback->tokens(word, context, reset, stop);
	});
}
/**
 * getSize Метод получения размера n-грамы
 * @return длина n-граммы в языковой моделе
 */
const size_t anyks::LM::getSize() const {
	// Выводим результат
	return this->alm->getSize();
}
/**
 * getVersion Метод получения версии языковой модели
 * @return версия языковой модели
 */
const string anyks::LM::getVersion() const noexcept {
	// Выводим результат
	return ANYKS_VERSION;
}
/**
 * getUnknown Метод извлечения неизвестного слова
 * @return установленное неизвестное слово
 */
const string & anyks::LM::getUnknown() const noexcept {
	// Выводим результат
	return this->alm->getUnknown();
}
/**
 * getWordScript Метод извлечения скрипта обработки слов
 * @return адрес скрипта python обработки слов
 */
const string & anyks::LM::getWordScript() const noexcept {
	// Выводим результат
	return this->alm->getWordScript();
}
/**
 * getUserTokenScript Метод извлечения скрипта обработки пользовательских токенов
 * @return адрес скрипта python обработки пользовательских токенов
 */
const string & anyks::LM::getUserTokenScript() const noexcept {
	// Выводим результат
	return this->alm->getUserTokenScript();
}
/**
 * getBadwords Метод извлечения чёрного списка
 * @return чёрный список слов
 */
const set <size_t> & anyks::LM::getBadwords() const noexcept {
	// Выводим результат
	return this->alm->getBadwords();
}
/**
 * getGoodwords Метод извлечения белого списка
 * @return белый список слов
 */
const set <size_t> & anyks::LM::getGoodwords() const noexcept {
	// Выводим результат
	return this->alm->getGoodwords();
}
/**
 * getTokensUnknown Метод извлечения списка токенов приводимых к <unk>
 * @return список токенов
 */
const set <size_t> anyks::LM::getTokensUnknown() const noexcept {
	// Результат работы функции
	set <size_t> result;
	// Извлекаем список токенов
	const auto & tokens = this->alm->getTokensUnknown();
	// Если список токенов получен
	if(!tokens.empty()){
		// Переходим по всему списку токенов
		for(auto & token : tokens) result.emplace(size_t(token));
	}
	// Выводим результат
	return result;
}
/**
 * getTokensDisable Метод извлечения списка запрещённых токенов
 * @return список токенов
 */
const set <size_t> anyks::LM::getTokensDisable() const noexcept {
	// Результат работы функции
	set <size_t> result;
	// Извлекаем список токенов
	const auto & tokens = this->alm->getTokensDisable();
	// Если список токенов получен
	if(!tokens.empty()){
		// Переходим по всему списку токенов
		for(auto & token : tokens) result.emplace(size_t(token));
	}
	// Выводим результат
	return result;
}
/**
 * getUserTokens Метод извлечения списка пользовательских токенов
 * @return список пользовательских токенов
 */
const vector <string> & anyks::LM::getUserTokens() const noexcept {
	// Выводим результат
	return this->alm->getUserTokens();
}
/**
 * grams Метод проверки количества найденных в тексте n-грамм
 * @param text текст для расчёта
 * @return     количество найденных n-грамм
 */
const size_t anyks::LM::grams(const string & text) const noexcept {
	// Выводим результат
	return this->alm->grams(text);
}
/**
 * grams Метод проверки количества найденных n-грамм
 * @param seq список последовательностей
 * @return    количество найденных n-грамм
 */
const size_t anyks::LM::grams(const vector <size_t> & seq) const noexcept {
	// Выводим результат
	return this->alm->grams(seq);
}
/**
 * bigrams Метод проверки количества найденных в тексте биграмм
 * @param text текст для расчёта
 * @return     количество найденных биграмм
 */
const size_t anyks::LM::bigrams(const string & text) const noexcept {
	// Выводим результат
	return this->alm->bigrams(text);
}
/**
 * bigrams Метод проверки количества найденных биграмм
 * @param seq список последовательностей
 * @return    количество найденных биграмм
 */
const size_t anyks::LM::bigrams(const vector <size_t> & seq) const noexcept {
	// Выводим результат
	return this->alm->bigrams(seq);
}
/**
 * trigrams Метод проверки количества найденных в тексте триграмм
 * @param text текст для расчёта
 * @return     количество найденных триграмм
 */
const size_t anyks::LM::trigrams(const string & text) const noexcept {
	// Выводим результат
	return this->alm->trigrams(text);
}
/**
 * trigrams Метод проверки количества найденных последовательных триграмм
 * @param seq список последовательностей
 * @return    количество найденных триграмм
 */
const size_t anyks::LM::trigrams(const vector <size_t> & seq) const noexcept {
	// Выводим результат
	return this->alm->trigrams(seq);
}
/**
 * getUserTokenWord Метод получения пользовательского токена по его идентификатору
 * @param idw идентификатор пользовательского токена
 * @return    пользовательский токен соответствующий идентификатору
 */
const string anyks::LM::getUserTokenWord(const size_t idw) const noexcept {
	// Выводим результат
	return this->alm->getUserTokenWord(idw);
}
/**
 * getUserTokenId Метод получения идентификатора пользовательского токена
 * @param name слово для которого нужно получить идентификатор
 * @return     идентификатор пользовательского токена соответствующий слову
 */
const size_t anyks::LM::getUserTokenId(const string & name) const noexcept {
	// Выводим результат
	return this->alm->getUserTokenId(name);
}
/**
 * LM Конструктор
 */
anyks::LM::LM() noexcept {
	// Экранируем возможность ошибки памяти
	try {
		// Выделяем память для объекта алфавита
		this->alphabet = new alphabet_t();
		// Выделяем память для токенизатора
		this->tokenizer = new tokenizer_t(this->alphabet);
		// Выделяем память для языковой модели
		this->alm = new alm_t(this->alphabet, this->tokenizer);
	// Если происходит ошибка то игнорируем её
	} catch(const bad_alloc &) {
		// Выводим сообщение об ошибке
		cerr << "bad alloc for init language model" << endl;
		// Выходим из приложения
		exit(EXIT_FAILURE);
	}
}
/**
 * LM Конструктор
 * @param alphabet алфавит символов для текущего языка
 */
anyks::LM::LM(const string & alphabet) noexcept {
	// Экранируем возможность ошибки памяти
	try {
		// Выделяем память для объекта алфавита
		this->alphabet = new alphabet_t(alphabet, "en_US.UTF-8");
		// Выделяем память для токенизатора
		this->tokenizer = new tokenizer_t(this->alphabet);
		// Выделяем память для языковой модели
		this->alm = new alm_t(this->alphabet, this->tokenizer);
	// Если происходит ошибка то игнорируем её
	} catch(const bad_alloc &) {
		// Выводим сообщение об ошибке
		cerr << "bad alloc for init language model" << endl;
		// Выходим из приложения
		exit(EXIT_FAILURE);
	}
}
/**
 * LM Конструктор
 * @param alphabet алфавит символов для текущего языка
 * @param locale   локализация приложения
 */
anyks::LM::LM(const string & alphabet, const string & locale) noexcept {
	// Экранируем возможность ошибки памяти
	try {
		// Выделяем память для объекта алфавита
		this->alphabet = new alphabet_t(alphabet, locale);
		// Выделяем память для токенизатора
		this->tokenizer = new tokenizer_t(this->alphabet);
		// Выделяем память для языковой модели
		this->alm = new alm_t(this->alphabet, this->tokenizer);
	// Если происходит ошибка то игнорируем её
	} catch(const bad_alloc &) {
		// Выводим сообщение об ошибке
		cerr << "bad alloc for init language model" << endl;
		// Выходим из приложения
		exit(EXIT_FAILURE);
	}
}
/**
 * ~LM Деструктор
 */
anyks::LM::~LM() noexcept {
	// Если языковая модель создана, удаляем
	if(this->alm != nullptr) delete this->alm;
	// Если токенизатор создан, удаляем
	if(this->tokenizer != nullptr) delete this->tokenizer;
	// Если алфавит создан, удаляем
	if(this->alphabet != nullptr) delete this->alphabet;
}

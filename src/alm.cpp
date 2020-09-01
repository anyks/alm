/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <alm.hpp>

/**
 * isOption Метод проверки наличия опции
 * @param option опция для проверки
 * @return       результат проверки
 */
const bool anyks::Alm::isOption(const options_t option) const noexcept {
	// Выполняем проверку наличия опции
	const bool result = this->options.test((u_short) option);
	// Если флаг дал положительный результат и это режим отладки
	if(result && (option == options_t::debug)) this->logfile = nullptr;
	// Выводим результат
	return result;
}
/**
 * exist Метод проверки существования последовательности
 * @param seq список слов последовательности
 * @return    результат проверки
 */
const bool anyks::Alm::exist(const vector <size_t> & seq) const noexcept {
	// Блокируем варнинг
	(void) seq;
	// Выводим результат
	return false;
}
/**
 * backoff Метод извлечения обратной частоты последовательности
 * @param seq последовательность для извлечения обратной частоты
 * @return    обратная частота последовательности
 */
const double anyks::Alm::backoff(const vector <size_t> & seq) const noexcept {
	// Блокируем варнинг
	(void) seq;
	// Выводим результат
	return 0.0;
}
/**
 * weight Метод извлечения веса последовательности
 * @param seq последовательность для извлечения веса
 * @return    вес последовательноси и n-грамма для которой она получена
 */
const pair <u_short, double> anyks::Alm::weight(const vector <size_t> & seq) const noexcept {
	// Блокируем варнинг
	(void) seq;
	// Выводим результат
	return {0, 0.0};
}
/**
 * frequency Метод извлечения частоты n-граммы
 * @param seq список слов последовательности
 * @return    частота и обратная частота n-граммы
 */
const pair <double, double> anyks::Alm::frequency(const vector <size_t> & seq) const noexcept {
	// Блокируем варнинг
	(void) seq;
	// Выводим результат
	return {this->zero, this->zero};
}
/**
 * set Метод установки последовательности в словарь
 * @param seq последовательность слов для установки
 */
void anyks::Alm::set(const vector <seq_t> & seq) const noexcept {
	// Блокируем варнинг
	(void) seq;
}
/**
 * set Метод установки последовательности в словарь
 * @param seq     список идентификаторов слов которые нужно добавить
 * @param uppers  список верхних регистров последнего слова последовательности
 * @param weight  вес n-граммы из файла arpa
 * @param backoff обратная частота документа из файла arpa
 */
void anyks::Alm::set(const vector <size_t> & seq, const size_t uppers, const double weight, const double backoff) const noexcept {
	// Блокируем варнинг
	(void) seq;
	(void) uppers;
	(void) weight;
	(void) backoff;
}
/**
 * clearShielding Функция удаления экранирования
 * @param word  слово в котором следует удалить экранирование
 * @param front первый символ экранирования
 * @param back  последний символ экранирования
 */
void anyks::Alm::clearShielding(const string & word, const string & front, const string & back) const noexcept {
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
 * pplCalculate Метод расчёта значения перплексии по весу
 * @param logprob вес последовательности n-грамм полученный из расчётов
 * @param wirds   список слов используемых при расчёте
 * @param oovs    список неизвестных слов используемых при расчётах
 * @return        значение перплексии полученное при расчётах
 */
const pair <double, double> anyks::Alm::pplCalculate(const double logprob, const size_t words, const size_t oovs) const noexcept {
	// Результат работы функции
	pair <double, double> result = {this->zero, this->zero};
	// Если вес переданный верный
	if((logprob != 0.0) && (logprob != this->zero) && (words > 0)){
		// Выполняем расчёт перплексии
		result.second = exp(- (logprob / double(words - oovs)) * this->mln10);
		result.first  = exp(- (logprob / double(words - oovs + 1)) * this->mln10);
	}
	// Выводим результат
	return result;
}
/**
 * perplexity Метод расчёта перплексии текста
 * @param  text текст для расчёта
 * @return      результат расчёта
 */
const anyks::Alm::ppl_t anyks::Alm::perplexity(const string & text) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если текст передан
	if(!text.empty()) result = this->perplexity(this->alphabet->convert(text));
	// Выводим результат
	return result;
}
/**
 * perplexity Метод расчёта перплексии текста
 * @param  text текст для расчёта
 * @return      результат расчёта
 */
const anyks::Alm::ppl_t anyks::Alm::perplexity(const wstring & text) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если текст передан
	if(!text.empty()){
		// Список собранных OOV слов
		unordered_map <wstring, size_t> oovs;
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		// Идентификатор начала предложения
		const size_t sid = (size_t) token_t::start;
		// Идентификатор конца предложения
		const size_t fid = (size_t) token_t::finish;
		// Список последовательностей для обучения
		vector <size_t> seq = {sid};
		/**
		 * unkFn Функция установки неизвестного слова в последовательность
		 * @param word слово для добавления в список неизвестных слов
		 * @return     нужно ли остановить сбор последовательности
		 */
		auto unkFn = [&seq, uid, &oovs, this](const wstring & word) noexcept {
			// Если неизвестное слово не установлено
			if(this->unknown == 0) seq.push_back(uid);
			// Если неизвестное слово установлено
			else seq.push_back(this->unknown);
			// Если слово передано
			if(!word.empty()){
				// Выполняем поиск слова в списке
				auto it = oovs.find(word);
				// Если слово существует
				if(it != oovs.end()) it->second++;
				// Если слово не найдено в списке
				else oovs.emplace(word, 1);
			}
		};
		/**
		 * resFn Функция вывода результата
		 */
		auto resFn = [&result, &seq, fid, sid, this]() noexcept {
			// Добавляем в список конец предложения
			seq.push_back(fid);
			/**
			 * Если слова всего два, значит это начало и конец предложения
			 * Нам же нужны только нормальные n-граммы
			 */
			if(seq.size() > 2) result = (result.words == 0 ? this->perplexity(seq) : this->pplConcatenate(result, this->perplexity(seq)));
			// Очищаем список последовательностей
			seq.clear();
			// Добавляем в список начало предложения
			seq.push_back(sid);
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
						const auto & res = this->python->run(it->second.second, {tmp.real()}, ctx);
						// Если результат получен
						if(!res.empty()) tmp = res;
						// Разблокируем поток
						this->locker.unlock();
					}
				// Если модуль предобработки слов, существует
				} else if(this->wordPress != nullptr) tmp = this->wordPress(tmp.real(), ctx);
				// Если слово не разрешено
				if(tmp.length() >= MAX_WORD_LENGTH) unkFn(word);
				// Если слово разрешено
				else if(!tmp.empty()) {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Выполняем проверку на плохое слово
					const bool isBad = (this->badwords.count(idw) > 0);
					// Если это плохое слово, заменяем его на неизвестное
					if(isBad || (idw == 0) || (idw == idw_t::NIDW)) unkFn(word);
					// Иначе продолжаем дальше
					else {
						// Проверяем является ли строка словом
						const bool isWord = !this->tokenizer->isToken(idw);
						// Если это неизвестное слово
						if(isBad || (idw == uid) || (isWord && (this->getWord(idw) == nullptr))) unkFn(word);
						// Иначе добавляем слово
						else if(!isBad && (!isWord || (this->goodwords.count(idw) > 0) || this->alphabet->isAllowed(tmp)))
							// Собираем последовательность
							seq.push_back(idw);
						// Отправляем слово как неизвестное
						else unkFn(word);
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
		// Выводим отладочную информацию
		if((this->isOption(options_t::debug)) && (this->threads == 1)){
			// Выводим сообщение отладки - количество слов
			this->alphabet->log(
				"%u sentences, %u words, %u OOVs",
				alphabet_t::log_t::info,
				this->logfile,
				result.sentences,
				result.words,
				result.oovs
			);
			// Выводим сообщение отладки - результатов расчёта
			this->alphabet->log(
				"%u zeroprobs, logprob= %4.8f ppl= %4.8f ppl1= %4.8f\r\n",
				alphabet_t::log_t::info,
				this->logfile,
				result.zeroprobs,
				result.logprob,
				result.ppl,
				result.ppl1
			);
		}
		// Если список неизвестных слов получен и есть куда его выводить
		if(!oovs.empty() && (this->oovfile != nullptr)){
			// Блокируем поток
			this->locker.lock();
			// Переходим по всему списку неизвестных слов
			for(auto & item : oovs){
				// Добавляем слово в файл
				this->alphabet->log(
					"%ls\t%u",
					alphabet_t::log_t::null,
					this->oovfile,
					item.first.c_str(),
					item.second
				);
			}
			// Разблокируем поток
			this->locker.unlock();
		}
	}
	// Выводим результат
	return result;
}
/**
 * perplexity Метод расчёта перплексии текста
 * @param  words список слов для расчёта
 * @return       результат расчёта
 */
const anyks::Alm::ppl_t anyks::Alm::perplexity(const vector <wstring> & words) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если список слов переданы
	if(!words.empty()){
		// Список последовательности
		vector <size_t> seq(words.size());
		// Переходим по всем словам последовательности
		for(size_t i = 0; i < words.size(); i++){
			// Получаем идентификатор слова в последовательности
			seq.at(i) = this->getIdw(words.at(i));
		}
		// Выполняем расчёт перплексии
		if(!seq.empty()){
			// Добавляем конец предложения
			if(seq.back() != size_t(token_t::finish)) seq.push_back(size_t(token_t::finish));
			// Добавляем начало предложения
			if(seq.front() != size_t(token_t::start)) seq.insert(seq.begin(), size_t(token_t::start));
			// Выполняем расчёт перплексии
			result = this->perplexity(seq);
		}
	}
	// Выводим результат
	return result;
}
/**
 * perplexity Метод расчёта перплексии
 * @param  seq список последовательностей
 * @return     результат расчёта
 */
const anyks::Alm::ppl_t anyks::Alm::perplexity(const vector <size_t> & seq) const noexcept {
	// Блокируем варнинг
	(void) seq;
	// Выводим результат
	return ppl_t();
}
/**
 * pplConcatenate Метод объединения перплексий
 * @param ppl1 первая перплексия
 * @param ppl2 вторая перплексия
 * @return     объединённая перплексия
 */
const anyks::Alm::ppl_t anyks::Alm::pplConcatenate(const ppl_t & ppl1, const ppl_t & ppl2) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если перплексии переданы
	if((ppl1.words > 0) && (ppl2.words > 0)){
		// Выполняем сложение двух перплексий
		result.oovs      = (ppl1.oovs + ppl2.oovs);
		result.words     = (ppl1.words + ppl2.words);
		result.logprob   = (ppl1.logprob + ppl2.logprob);
		result.sentences = (ppl1.sentences + ppl2.sentences);
		result.zeroprobs = (ppl1.zeroprobs + ppl2.zeroprobs);
		// Выполняем расчёт перплексии
		const auto ppl = this->pplCalculate(result.logprob, result.words, result.oovs);
		// Усталавниваем полученные значения перплексии
		result.ppl  = ppl.first;
		result.ppl1 = ppl.second;
	}
	// Выводим результат
	return result;
}
/**
 * pplByFiles Метод чтения расчёта перплексии по файлу или группе файлов
 * @param path   адрес каталога или файла для расчёта перплексии
 * @param status функция вывода статуса
 * @param ext    расширение файлов в каталоге (если адрес передан каталога)
 * @return       результат расчёта
 */
const anyks::Alm::ppl_t anyks::Alm::pplByFiles(const string & path, function <void (const u_short)> status, const string & ext) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если адрес файла передан
	if(!path.empty()){
		// Выполняем инициализацию тредпула
		this->tpool.init(this->threads);
		// Параметры индикаторы процесса
		size_t csize = 0, actual = 0, rate = 0;
		/**
		 * statusFn Функция вывода статуса работы процесса
		 * @param size размер обрабатываемых данных
		 * @param max  максимальный размер данных
		 */
		auto statusFn = [&csize, &actual, &rate, &status](const size_t size, const double max){
			// Общий полученный размер данных
			csize += size;
			// Подсчитываем статус выполнения
			actual = u_short(csize / max * 100.0);
			// Если процентное соотношение изменилось
			if((status != nullptr) && (rate != actual)){
				// Запоминаем текущее процентное соотношение
				rate = actual;
				// Выводим статистику
				status(actual);
			}
		};
		/**
		 * runFn Функция расчёта перплексии
		 * @param text текст для обработки
		 * @param size общий размер обрабатываемых данных
		 */
		auto runFn = [&result, &statusFn, this](const string & text, const size_t size){
			// Если это не первый этап обработки
			if(result.words != 0){
				// Выполняем расчёт перплексии
				ppl_t res = this->perplexity(text);
				// Выполняем блокировку потока
				this->locker.lock();
				// Выполняем сложение перплексий
				result = this->pplConcatenate(result, res);
				// Выводим статус прогресса работы
				statusFn(text.size(), size);
				// Выполняем разблокировку потока
				this->locker.unlock();
			// Если это первый этап обработки
			} else {
				// Выполняем расчёт перплексии
				result = this->perplexity(text);
				// Выводим статус прогресса работы
				statusFn(text.size(), size);
			}
		};
		// Если это файл
		if(fsys_t::isfile(path)){
			// Выполняем загрузку данных текстового файла
			fsys_t::rfile(path, [&](const string & text, const uintmax_t fileSize) noexcept {
				// Если текст передан
				if(!text.empty()){
					// Если это первый расчёт, считаем его в основном потоке
					if(result.words == 0) runFn(text, fileSize);
					// Добавляем в тредпул новое задание на обработку
					else this->tpool.push(runFn, text, fileSize);
				}
			});
		// Если это каталог
		} else if(fsys_t::isdir(path)) {
			// Выполняем загрузку каталога с текстовыми файлами
			fsys_t::rfdir(path, ext, [&](const string & text, const string & filename, const uintmax_t fileSize, const uintmax_t dirSize) noexcept {
				// Если текст передан
				if(!text.empty()){
					// Если это первый расчёт, считаем его в основном потоке
					if(result.words == 0) runFn(text, dirSize);
					// Добавляем в тредпул новое задание на обработку
					else this->tpool.push(runFn, text, dirSize);
				}
			});
		}
		// Ожидаем завершения обработки
		this->tpool.wait();
		// Выводим отладочную информацию
		if(this->isOption(options_t::debug)){
			// Выводим разделитель
			this->alphabet->log("%s", alphabet_t::log_t::null, this->logfile, "\r\n");
			// Выводим сообщение отладки - количество слов
			this->alphabet->log(
				"%u sentences, %u words, %u OOVs",
				alphabet_t::log_t::info,
				this->logfile,
				result.sentences,
				result.words,
				result.oovs
			);
			// Выводим сообщение отладки - результатов расчёта
			this->alphabet->log(
				"%u zeroprobs, logprob= %4.8f ppl= %4.8f ppl1= %4.8f\r\n",
				alphabet_t::log_t::info,
				this->logfile,
				result.zeroprobs,
				result.logprob,
				result.ppl,
				result.ppl1
			);
		}
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности, с указанным шагом
 * @param text текст для проверки существования
 * @param step размер шага проверки последовательности
 * @return     результат проверки
 */
const bool anyks::Alm::check(const string & text, const u_short step) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!text.empty()) result = this->check(this->alphabet->convert(text), step);
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности, с указанным шагом
 * @param text текст для проверки существования
 * @param step размер шага проверки последовательности
 * @return     результат проверки
 */
const bool anyks::Alm::check(const wstring & text, const u_short step) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!text.empty()){
		// Список последовательностей для обучения
		vector <size_t> seq = {};
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		/**
		 * unkFn Функция установки неизвестного слова в последовательность
		 * @return нужно ли остановить сбор последовательности
		 */
		auto unkFn = [&seq, uid, this]() noexcept {
			// Если неизвестное слово не установлено
			if(this->unknown == 0) seq.push_back(uid);
			// Если неизвестное слово установлено
			else seq.push_back(this->unknown);
		};
		/**
		 * resFn Функция вывода результата
		 */
		auto resFn = [&result, &seq, step, this]() noexcept {
			/**
			 * Если слова всего два, значит это начало и конец предложения
			 * Нам же нужны только нормальные n-граммы
			 */
			if(seq.size() > 2) result = this->check(seq, step);
			// Очищаем список последовательностей
			seq.clear();
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
						const auto & res = this->python->run(it->second.second, {tmp.real()}, ctx);
						// Если результат получен
						if(!res.empty()) tmp = res;
						// Разблокируем поток
						this->locker.unlock();
					}
				// Если модуль предобработки слов, существует
				} else if(this->wordPress != nullptr) tmp = this->wordPress(tmp.real(), ctx);
				// Если слово не разрешено
				if(tmp.length() >= MAX_WORD_LENGTH) unkFn();
				// Если слово разрешено
				else if(!tmp.empty()) {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Выполняем проверку на плохое слово
					const bool isBad = (this->badwords.count(idw) > 0);
					// Если это плохое слово, заменяем его на неизвестное
					if(isBad || (idw == 0) || (idw == idw_t::NIDW)) unkFn();
					// Иначе продолжаем дальше
					else {
						// Проверяем является ли строка словом
						const bool isWord = !this->tokenizer->isToken(idw);
						// Если это неизвестное слово
						if(isBad || (idw == uid) || (isWord && (this->getWord(idw) == nullptr))) unkFn();
						// Иначе добавляем слово
						else if(!isBad && (!isWord || (this->goodwords.count(idw) > 0) || this->alphabet->isAllowed(tmp)))
							// Собираем последовательность
							seq.push_back(idw);
						// Отправляем слово как неизвестное
						else unkFn();
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
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности, с указанным шагом
 * @param seq  список слов последовательности
 * @param step размер шага проверки последовательности
 * @return     результат проверки
 */
const bool anyks::Alm::check(const vector <string> & seq, const u_short step) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если последовательность получена
	if(!seq.empty()){
		// Список последовательности для проверки
		vector <size_t> tmp(seq.size());
		// Переходим по всей последовательности
		for(size_t i = 0; i < seq.size(); i++){
			// Устанавливаем полученное слово
			tmp.at(i) = this->getIdw(this->alphabet->convert(seq.at(i)));
		}
		// Получаем результат
		result = this->check(tmp, step);
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности, с указанным шагом
 * @param seq  список слов последовательности
 * @param step размер шага проверки последовательности
 * @return     результат проверки
 */
const bool anyks::Alm::check(const vector <wstring> & seq, const u_short step) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если последовательность получена
	if(!seq.empty()){
		// Список последовательности для проверки
		vector <size_t> tmp(seq.size());
		// Переходим по всей последовательности
		for(size_t i = 0; i < seq.size(); i++){
			// Устанавливаем полученное слово
			tmp.at(i) = this->getIdw(seq.at(i));
		}
		// Получаем результат
		result = this->check(tmp, step);
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности, с указанным шагом
 * @param seq  список слов последовательности
 * @param step размер шага проверки последовательности
 * @return     результат проверки
 */
const bool anyks::Alm::check(const vector <size_t> & seq, const u_short step) const noexcept {
	// Блокируем варнинг
	(void) seq;
	(void) step;
	// Выводим результат
	return false;
}
/**
 * check Метод проверки существования последовательности
 * @param text     текст для проверки существования
 * @param accurate режим точной проверки
 * @return         результат проверки
 */
const pair <bool, size_t> anyks::Alm::check(const string & text, const bool accurate) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result = {false, 0};
	// Если слово передано
	if(!text.empty()) result = this->check(this->alphabet->convert(text), accurate);
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности
 * @param text     текст для проверки существования
 * @param accurate режим точной проверки
 * @return         результат проверки
 */
const pair <bool, size_t> anyks::Alm::check(const wstring & text, const bool accurate) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result = {false, 0};
	// Если слово передано
	if(!text.empty()){
		// Список последовательностей для обучения
		vector <size_t> seq = {};
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		/**
		 * unkFn Функция установки неизвестного слова в последовательность
		 * @return нужно ли остановить сбор последовательности
		 */
		auto unkFn = [&seq, uid, this]() noexcept {
			// Если неизвестное слово не установлено
			if(this->unknown == 0) seq.push_back(uid);
			// Если неизвестное слово установлено
			else seq.push_back(this->unknown);
		};
		/**
		 * resFn Функция вывода результата
		 */
		auto resFn = [&result, &seq, accurate, this]() noexcept {
			/**
			 * Если слова всего два, значит это начало и конец предложения
			 * Нам же нужны только нормальные n-граммы
			 */
			if(seq.size() > 2) result = this->check(seq, accurate);
			// Очищаем список последовательностей
			seq.clear();
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
						const auto & res = this->python->run(it->second.second, {tmp.real()}, ctx);
						// Если результат получен
						if(!res.empty()) tmp = res;
						// Разблокируем поток
						this->locker.unlock();
					}
				// Если модуль предобработки слов, существует
				} else if(this->wordPress != nullptr) tmp = this->wordPress(tmp.real(), ctx);
				// Если слово не разрешено
				if(tmp.length() >= MAX_WORD_LENGTH) unkFn();
				// Если слово разрешено
				else if(!tmp.empty()) {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Выполняем проверку на плохое слово
					const bool isBad = (this->badwords.count(idw) > 0);
					// Если это плохое слово, заменяем его на неизвестное
					if(isBad || (idw == 0) || (idw == idw_t::NIDW)) unkFn();
					// Иначе продолжаем дальше
					else {
						// Проверяем является ли строка словом
						const bool isWord = !this->tokenizer->isToken(idw);
						// Если это неизвестное слово
						if(isBad || (idw == uid) || (isWord && (this->getWord(idw) == nullptr))) unkFn();
						// Иначе добавляем слово
						else if(!isBad && (!isWord || (this->goodwords.count(idw) > 0) || this->alphabet->isAllowed(tmp)))
							// Собираем последовательность
							seq.push_back(idw);
						// Отправляем слово как неизвестное
						else unkFn();
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
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности
 * @param seq      список слов последовательности
 * @param accurate режим точной проверки
 * @return         результат проверки
 */
const pair <bool, size_t> anyks::Alm::check(const vector <string> & seq, const bool accurate) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result;
	// Если последовательность получена
	if(!seq.empty()){
		// Список последовательности для проверки
		vector <size_t> tmp(seq.size());
		// Переходим по всей последовательности
		for(size_t i = 0; i < seq.size(); i++){
			// Устанавливаем полученное слово
			tmp.at(i) = this->getIdw(this->alphabet->convert(seq.at(i)));
		}
		// Получаем результат
		result = this->check(tmp, accurate);
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности
 * @param seq      список слов последовательности
 * @param accurate режим точной проверки
 * @return         результат проверки
 */
const pair <bool, size_t> anyks::Alm::check(const vector <wstring> & seq, const bool accurate) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result;
	// Если последовательность получена
	if(!seq.empty()){
		// Список последовательности для проверки
		vector <size_t> tmp(seq.size());
		// Переходим по всей последовательности
		for(size_t i = 0; i < seq.size(); i++){
			// Устанавливаем полученное слово
			tmp.at(i) = this->getIdw(seq.at(i));
		}
		// Получаем результат
		result = this->check(tmp, accurate);
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки существования последовательности
 * @param seq      список слов последовательности
 * @param accurate режим точной проверки
 * @return         результат проверки
 */
const pair <bool, size_t> anyks::Alm::check(const vector <size_t> & seq, const bool accurate) const noexcept {
	// Блокируем варнинг
	(void) seq;
	(void) accurate;
	// Выводим результат
	return {false, 0};
}
/**
 * exist Метод проверки существования последовательности
 * @param text текст для проверки существования
 * @param step размер шага проверки последовательности
 * @return     результат проверки
 */
const pair <bool, size_t> anyks::Alm::exist(const string & text, const u_short step) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result = {false, 0};
	// Если слово передано
	if(!text.empty()) result = this->exist(this->alphabet->convert(text), step);
	// Выводим результат
	return result;
}
/**
 * exist Метод проверки существования последовательности
 * @param text текст для проверки существования
 * @param step размер шага проверки последовательности
 * @return     результат проверки
 */
const pair <bool, size_t> anyks::Alm::exist(const wstring & text, const u_short step) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result = {false, 0};
	// Если слово передано
	if(!text.empty()){
		// Список последовательностей для обучения
		vector <size_t> seq = {};
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		/**
		 * unkFn Функция установки неизвестного слова в последовательность
		 * @return нужно ли остановить сбор последовательности
		 */
		auto unkFn = [&seq, uid, this]() noexcept {
			// Если неизвестное слово не установлено
			if(this->unknown == 0) seq.push_back(uid);
			// Если неизвестное слово установлено
			else seq.push_back(this->unknown);
		};
		/**
		 * resFn Функция вывода результата
		 */
		auto resFn = [&result, &seq, step, this]() noexcept {
			/**
			 * Если слова всего два, значит это начало и конец предложения
			 * Нам же нужны только нормальные n-граммы
			 */
			if(seq.size() > 2) result = this->exist(seq, step);
			// Очищаем список последовательностей
			seq.clear();
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
						const auto & res = this->python->run(it->second.second, {tmp.real()}, ctx);
						// Если результат получен
						if(!res.empty()) tmp = res;
						// Разблокируем поток
						this->locker.unlock();
					}
				// Если модуль предобработки слов, существует
				} else if(this->wordPress != nullptr) tmp = this->wordPress(tmp.real(), ctx);
				// Если слово не разрешено
				if(tmp.length() >= MAX_WORD_LENGTH) unkFn();
				// Если слово разрешено
				else if(!tmp.empty()) {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Выполняем проверку на плохое слово
					const bool isBad = (this->badwords.count(idw) > 0);
					// Если это плохое слово, заменяем его на неизвестное
					if(isBad || (idw == 0) || (idw == idw_t::NIDW)) unkFn();
					// Иначе продолжаем дальше
					else {
						// Проверяем является ли строка словом
						const bool isWord = !this->tokenizer->isToken(idw);
						// Если это неизвестное слово
						if(isBad || (idw == uid) || (isWord && (this->getWord(idw) == nullptr))) unkFn();
						// Иначе добавляем слово
						else if(!isBad && (!isWord || (this->goodwords.count(idw) > 0) || this->alphabet->isAllowed(tmp)))
							// Собираем последовательность
							seq.push_back(idw);
						// Отправляем слово как неизвестное
						else unkFn();
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
	// Выводим результат
	return result;
}
/**
 * exist Метод проверки существования последовательности
 * @param seq  список слов последовательности
 * @param step размер шага проверки последовательности
 * @return     результат проверки
 */
const pair <bool, size_t> anyks::Alm::exist(const vector <string> & seq, const u_short step) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result = {false, 0};
	// Если последовательность получена
	if(!seq.empty()){
		// Список последовательности для проверки
		vector <size_t> tmp(seq.size());
		// Переходим по всей последовательности
		for(size_t i = 0; i < seq.size(); i++){
			// Устанавливаем полученное слово
			tmp.at(i) = this->getIdw(this->alphabet->convert(seq.at(i)));
		}
		// Получаем результат
		result = this->exist(tmp, step);
	}
	// Выводим результат
	return result;
}
/**
 * exist Метод проверки существования последовательности
 * @param seq  список слов последовательности
 * @param step размер шага проверки последовательности
 * @return     результат проверки
 */
const pair <bool, size_t> anyks::Alm::exist(const vector <wstring> & seq, const u_short step) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result = {false, 0};
	// Если последовательность получена
	if(!seq.empty()){
		// Список последовательности для проверки
		vector <size_t> tmp(seq.size());
		// Переходим по всей последовательности
		for(size_t i = 0; i < seq.size(); i++){
			// Устанавливаем полученное слово
			tmp.at(i) = this->getIdw(seq.at(i));
		}
		// Получаем результат
		result = this->exist(tmp, step);
	}
	// Выводим результат
	return result;
}
/**
 * exist Метод проверки существования последовательности
 * @param seq  список слов последовательности
 * @param step размер шага проверки последовательности
 * @return     результат проверки
 */
const pair <bool, size_t> anyks::Alm::exist(const vector <size_t> & seq, const u_short step) const noexcept {
	// Блокируем варнинг
	(void) seq;
	(void) step;
	// Выводим результат
	return {false, 0};
}
/**
 * fixUppers Метод исправления регистров в тексте
 * @param text текст для исправления регистров
 * @return     текст с исправленными регистрами слов
 */
const string anyks::Alm::fixUppers(const string & text) const noexcept {
	// Результат работы функции
	string result = "";
	// Если текст передан
	if(!text.empty()) result = this->alphabet->convert(this->fixUppers(this->alphabet->convert(text)));
	// Выводим результат
	return result;
}
/**
 * fixUppers Метод исправления регистров в тексте
 * @param text текст для исправления регистров
 * @return     текст с исправленными регистрами слов
 */
const wstring anyks::Alm::fixUppers(const wstring & text) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если текст передан
	if(!text.empty()){
		// Флаг первой итерации
		bool start = false;
		// Список последовательностей для обучения
		vector <size_t> seq = {};
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		/**
		 * resFn Функция вывода результата
		 * @param word слово которое нужно вставить как есть
		 * @param idw  идентификатор слова если надо
		 */
		auto resFn = [&result, &start, &seq, this](const word_t & word = L"", const size_t idw = 0) noexcept {
			// Выполняем исправление регистров текста
			if(!seq.empty()){
				// Получаем текст последовательности
				wstring text = this->context(seq);
				// Если дополнительное слово не передано, меняем регистр у первой буквы
				if(start && !word.empty() && (word.front() != L'!') &&
				(word.front() != L'?') && (word.front() != L'.')) text.at(0) = towlower(text.front());
				// Добавляем исправленный текст
				result.append(text);
				// Если слово передано
				if(!word.empty()){
					// Добавляем разделитель
					if(idw != size_t(token_t::punct)) result.append(L" ");
					// Добавляем переданное слово
					result.append(word.wreal());
				}
				// Добавляем разделитель
				result.append(L" ");
			// Если слово передано
			} else if(!word.empty()){
				// Добавляем разделитель
				if((idw != size_t(token_t::punct)) && !this->alphabet->isSpace(result.back()))     result.append(L" ");
				else if((idw == size_t(token_t::punct)) && this->alphabet->isSpace(result.back())) result.pop_back();
				// Добавляем переданное слово
				result.append(word.wreal());
				// Добавляем разделитель
				result.append(L" ");
			}
			// Очищаем список последовательностей
			seq.clear();
			// Запоминаем что первая итерация выполнена
			start = true;
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
						const auto & res = this->python->run(it->second.second, {tmp.real()}, ctx);
						// Если результат получен
						if(!res.empty()) tmp = res;
						// Разблокируем поток
						this->locker.unlock();
					}
				// Если модуль предобработки слов, существует
				} else if(this->wordPress != nullptr) tmp = this->wordPress(tmp.real(), ctx);
				// Если слово не разрешено
				if(tmp.length() >= MAX_WORD_LENGTH) resFn(tmp);
				// Если слово разрешено
				else if(!tmp.empty()) {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Выполняем проверку на плохое слово
					const bool isBad = (this->badwords.count(idw) > 0);
					// Если это плохое слово, заменяем его на неизвестное
					if(isBad || (idw == 0) || (idw == idw_t::NIDW)) resFn(tmp);
					// Иначе продолжаем дальше
					else {
						// Проверяем является ли строка словом
						const bool isWord = !this->tokenizer->isToken(idw);
						// Если это неизвестное слово
						if(isBad || (idw == uid) || (isWord && (this->getWord(idw) == nullptr))) resFn(tmp, idw);
						// Иначе добавляем слово
						else if(!isBad && (!isWord || (this->goodwords.count(idw) > 0) || this->alphabet->isAllowed(tmp)))
							// Собираем последовательность
							seq.push_back(idw);
						// Отправляем слово как неизвестное
						else resFn(tmp, idw);
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
		// Если результат получен, удаляем последний пробел
		if(!result.empty()) result.pop_back();
	}
	// Выводим результат
	return result;
}
/**
 * setBin Метод установки бинарных данных в словарь
 * @param buffer буфер с бинарными данными
 */
void anyks::Alm::setBin(const vector <char> & buffer) const noexcept {
	// Блокируем варнинг
	(void) buffer;
}
/**
 * getBin Метод извлечения данных arpa в бинарном виде
 * @param callback функция обратного вызова
 */
void anyks::Alm::getBin(function <void (const vector <char> &, const size_t, const u_short)> callback) const noexcept {
	// Выводим результат
	callback({}, 0, 100);
}
/**
 * clear Метод очистки всех данных
 */
void anyks::Alm::clear(){
	// Очищаем неизвестное слово
	this->unknown = 0;
	// Очищаем словарь
	this->vocab.clear();
	// Очищаем список скриптов
	this->scripts.clear();
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
void anyks::Alm::clearBadwords() noexcept {
	// Очищаем список плохих слов
	this->badwords.clear();
}
/**
 * clearGoodwords Метод очистки списка хороших слов
 */
void anyks::Alm::clearGoodwords() noexcept {
	// Очищаем список хороших слов
	this->goodwords.clear();
}
/**
 * clearUserTokens Метод очистки пользовательских токенов
 */
void anyks::Alm::clearUserTokens() noexcept {
	// Очищаем список пользовательских токенов
	this->utokens.clear();
}
/**
 * addBadword Метод добавления идентификатора похого слова в список
 * @param idw идентификатор слова
 */
void anyks::Alm::addBadword(const size_t idw) noexcept {
	// Если идентификатор передан
	if((idw > 0) && (idw < idw_t::NIDW)) this->badwords.emplace(idw);
}
/**
 * addGoodword Метод добавления идентификатора хорошего слова в список
 * @param idw идентификатор слова
 */
void anyks::Alm::addGoodword(const size_t idw) noexcept {
	// Если идентификатор передан
	if((idw > 0) && (idw < idw_t::NIDW)) this->goodwords.emplace(idw);
}
/**
 * addBadword Метод добавления похого слова в список
 * @param word слово для добавления
 */
void anyks::Alm::addBadword(const string & word) noexcept {
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
void anyks::Alm::addGoodword(const string & word) noexcept {
	// Если слово передано
	if(!word.empty()){
		// Получаем идентификатор слова
		const size_t idw = this->tokenizer->idw(this->alphabet->convert(word));
		// Добавляем слово в список хороших слов
		this->addGoodword(idw);
	}
}
/**
 * setAllTokenUnknown Метод установки всех токенов идентифицируемых как <unk>
 */
void anyks::Alm::setAllTokenUnknown() noexcept {
	// Переходим по всем токенам
	for(u_short i = 0; i < u_short(token_t::endtoken); i++){
		// Если это не спец. токен, добавляем в список
		if((i != u_short(token_t::null)) && (i != u_short(token_t::start)) && (i != u_short(token_t::finish)) && (i != u_short(token_t::unk))){
			// Устанавливаем все токены для идентифицировоания как <unk>
			this->tokenUnknown.emplace((token_t) i);
		}
	}
}
/**
 * setAllTokenDisable Метод установки всех токенов как не идентифицируемых
 */
void anyks::Alm::setAllTokenDisable() noexcept {
	// Переходим по всем токенам
	for(u_short i = 0; i < u_short(token_t::endtoken); i++){
		// Если это не спец. токен, добавляем в список
		if((i != u_short(token_t::null)) && (i != u_short(token_t::start)) && (i != u_short(token_t::finish)) && (i != u_short(token_t::unk))){
			// Устанавливаем все токены для отключения
			this->tokenDisable.emplace((token_t) i);
		}
	}
}
/**
 * initPython Метод инициализации скриптов питона
 */
void anyks::Alm::initPython(){
	// Если скрипт получен
	if(((this->scripts.count(2) > 0) && !this->utokens.empty()) || (this->scripts.count(1) > 0)){
		// Экранируем возможность ошибки памяти
		try {
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
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выводим сообщение об ошибке, если режим отладки включён
			if(this->isOption(options_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "bad alloc for init python scripts");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * setSize Метод установки размера n-граммы
 * @param size размер n-граммы
 */
void anyks::Alm::setSize(const u_short size) noexcept {
	// Устанавливаем размерность n-граммы
	this->size = (size > 0 ? size : DEFNGRAM);
}
/**
 * setUnknown Метод установки неизвестного слова
 * @param word слово для добавления
 */
void anyks::Alm::setUnknown(const string & word) noexcept {
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
void anyks::Alm::setOptions(const u_int options) noexcept {
	// Устанавливаем опции модуля
	this->options = options;
}
/**
 * setPythonObj Метод установки внешнего объекта питона
 * @param python внешний объект питона
 */
void anyks::Alm::setPythonObj(python_t * python) noexcept {
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
void anyks::Alm::setLogfile(const char * logfile) noexcept {
	// Устанавливаем адрес log файла
	this->logfile = logfile;
}
/**
 * setOOvFile Метод установки файла для сохранения OOV слов
 * @param oovfile адрес файла для сохранения oov слов
 */
void anyks::Alm::setOOvFile(const char * oovfile) noexcept {
	// Устанавливаем адрес oov файла
	this->oovfile = oovfile;
}
/**
 * setUserToken Метод добавления токена пользователя
 * @param name слово - обозначение токена
 */
void anyks::Alm::setUserToken(const string & name) noexcept {
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
 * setOption Метод установки опций модуля
 * @param option опция для установки
 */
void anyks::Alm::setOption(const options_t option) noexcept {
	// Устанавливаем опции
	this->options.set((u_short) option);
}
/**
 * setWordFn Метод установки функций получения и добавления слов
 * @param fn1 функция получения слова
 * @param fn2 функция добавления слова
 */
void anyks::Alm::setWordFn(words_t fn1, addw_t fn2) noexcept {
	// Если функции переданы
	if((fn1 != nullptr) && (fn2 != nullptr)){
		// Устанавливаем функцию получения слова
		this->getWord = fn1;
		// Устанавливаем функцию добавления слова
		this->addWord = fn2;
	// Если функции не переданы
	} else if((fn1 == nullptr) && (fn2 == nullptr)) {
		// Устанавливаем функцию получения слова
		this->getWord = [this](const size_t idw) noexcept {
			// Результат работы функции
			const word_t * result = nullptr;
			// Если же функция не указана используем внутренний словарь
			if(!this->vocab.empty()) {
				// Получаем слово по его идентификатору
				auto it = this->vocab.find(idw);
				// Если слово получено
				if(it != this->vocab.end()) result = &it->second;
			}
			// Выводим результат
			return result;
		};
		// Устанавливаем функцию добавления слова
		this->addWord = [this](const size_t idw, const word_t & word) noexcept {
			// Проверяем отсутствует ли слово в списке запрещённых слов
			this->vocab.emplace(idw, word);
		};
	}
}
/**
 * unsetOption Метод отключения опции модуля
 * @param option опция для отключения
 */
void anyks::Alm::unsetOption(const options_t option) noexcept {
	// Устанавливаем опции
	this->options.reset((u_short) option);
}
/**
 * setThreads Метод установки количества потоков
 * @param threads количество потоков для работы
 */
void anyks::Alm::setThreads(const size_t threads) noexcept {
	// Устанавливаем новое количество потоков
	this->threads = (threads > 0 ? threads : thread::hardware_concurrency());
}
/**
 * setWordScript Метод установки скрипта обработки слов
 * @param script скрипт python обработки слов
 */
void anyks::Alm::setWordScript(const string & script) noexcept {
	// Выполняем добавление скрипта
	this->scripts.emplace(1, make_pair(script, 0));
}
/**
 * setWordPreprocessingMethod Метод установки функции препроцессинга слова
 * @param fn внешняя функция препроцессинга слова
 */
void anyks::Alm::setWordPreprocessingMethod(wpres_t fn) noexcept {
	// Устанавливаем функцию
	this->wordPress = fn;
}
/**
 * setTokenUnknown Метод установки списка токенов которых нужно идентифицировать как <unk>
 * @param options список токенов которых нужно идентифицировать как <unk>
 */
void anyks::Alm::setTokenUnknown(const string & options) noexcept {
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
				if(token.compare(L"num") == 0)           this->tokenUnknown.emplace(token_t::num);
				else if(token.compare(L"url") == 0)      this->tokenUnknown.emplace(token_t::url);
				else if(token.compare(L"abbr") == 0)     this->tokenUnknown.emplace(token_t::abbr);
				else if(token.compare(L"date") == 0)     this->tokenUnknown.emplace(token_t::date);
				else if(token.compare(L"time") == 0)     this->tokenUnknown.emplace(token_t::time);
				else if(token.compare(L"anum") == 0)     this->tokenUnknown.emplace(token_t::anum);
				else if(token.compare(L"math") == 0)     this->tokenUnknown.emplace(token_t::math);
				else if(token.compare(L"rnum") == 0)     this->tokenUnknown.emplace(token_t::rnum);
				else if(token.compare(L"specl") == 0)    this->tokenUnknown.emplace(token_t::specl);
				else if(token.compare(L"aprox") == 0)    this->tokenUnknown.emplace(token_t::aprox);
				else if(token.compare(L"range") == 0)    this->tokenUnknown.emplace(token_t::range);
				else if(token.compare(L"score") == 0)    this->tokenUnknown.emplace(token_t::score);
				else if(token.compare(L"dimen") == 0)    this->tokenUnknown.emplace(token_t::dimen);
				else if(token.compare(L"fract") == 0)    this->tokenUnknown.emplace(token_t::fract);
				else if(token.compare(L"punct") == 0)    this->tokenUnknown.emplace(token_t::punct);
				else if(token.compare(L"greek") == 0)    this->tokenUnknown.emplace(token_t::greek);
				else if(token.compare(L"route") == 0)    this->tokenUnknown.emplace(token_t::route);
				else if(token.compare(L"isolat") == 0)   this->tokenUnknown.emplace(token_t::isolat);
				else if(token.compare(L"pcards") == 0)   this->tokenUnknown.emplace(token_t::pcards);
				else if(token.compare(L"currency") == 0) this->tokenUnknown.emplace(token_t::currency);
			}
		}
	}
}
/**
 * setTokenDisable Метод установки списка не идентифицируемых токенов
 * @param options список не идентифицируемых токенов
 */
void anyks::Alm::setTokenDisable(const string & options) noexcept {
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
				if(token.compare(L"num") == 0)           this->tokenDisable.emplace(token_t::num);
				else if(token.compare(L"url") == 0)      this->tokenDisable.emplace(token_t::url);
				else if(token.compare(L"abbr") == 0)     this->tokenDisable.emplace(token_t::abbr);
				else if(token.compare(L"date") == 0)     this->tokenDisable.emplace(token_t::date);
				else if(token.compare(L"time") == 0)     this->tokenDisable.emplace(token_t::time);
				else if(token.compare(L"anum") == 0)     this->tokenDisable.emplace(token_t::anum);
				else if(token.compare(L"math") == 0)     this->tokenDisable.emplace(token_t::math);
				else if(token.compare(L"rnum") == 0)     this->tokenDisable.emplace(token_t::rnum);
				else if(token.compare(L"specl") == 0)    this->tokenDisable.emplace(token_t::specl);
				else if(token.compare(L"aprox") == 0)    this->tokenDisable.emplace(token_t::aprox);
				else if(token.compare(L"range") == 0)    this->tokenDisable.emplace(token_t::range);
				else if(token.compare(L"score") == 0)    this->tokenDisable.emplace(token_t::score);
				else if(token.compare(L"dimen") == 0)    this->tokenDisable.emplace(token_t::dimen);
				else if(token.compare(L"fract") == 0)    this->tokenDisable.emplace(token_t::fract);
				else if(token.compare(L"punct") == 0)    this->tokenDisable.emplace(token_t::punct);
				else if(token.compare(L"greek") == 0)    this->tokenDisable.emplace(token_t::greek);
				else if(token.compare(L"route") == 0)    this->tokenDisable.emplace(token_t::route);
				else if(token.compare(L"isolat") == 0)   this->tokenDisable.emplace(token_t::isolat);
				else if(token.compare(L"pcards") == 0)   this->tokenDisable.emplace(token_t::pcards);
				else if(token.compare(L"currency") == 0) this->tokenDisable.emplace(token_t::currency);
			}
		}
	}
}
/**
 * setUserTokenScript Метод установки скрипта обработки пользовательских токенов
 * @param script скрипт python обработки пользовательских токенов
 */
void anyks::Alm::setUserTokenScript(const string & script) noexcept {
	// Выполняем добавление скрипта
	this->scripts.emplace(2, make_pair(script, 0));
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита
 */
void anyks::Alm::setAlphabet(const alphabet_t * alphabet) noexcept {
	// Запоминаем объект алфавита
	if(alphabet != nullptr) this->alphabet = alphabet;
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer объект токенизатора
 */
void anyks::Alm::setTokenizer(const tokenizer_t * tokenizer) noexcept {
	// Если токенизатор передан
	if(tokenizer != nullptr) this->tokenizer = tokenizer;
}
/**
 * setTokensUnknown Метод установки списка токенов приводимых к <unk>
 * @param tokens список токенов для установки
 */
void anyks::Alm::setTokensUnknown(const std::set <token_t> & tokens) noexcept {
	// Если список получен, устанавливаем его
	if(!tokens.empty()) this->tokenUnknown = tokens;
}
/**
 * setTokensDisable Метод установки списка запрещённых токенов
 * @param tokens список токенов для установки
 */
void anyks::Alm::setTokensDisable(const std::set <token_t> & tokens) noexcept {
	// Если список получен, устанавливаем его
	if(!tokens.empty()) this->tokenDisable = tokens;
}
/**
 * setBadwords Метод установки списка идентификаторов плохих слов в список
 * @param badwords список идентификаторов плохих слов
 */
void anyks::Alm::setBadwords(const std::set <size_t> & badwords) noexcept {
	// Если список не пустой, устанавливаем список
	if(!badwords.empty()) this->badwords = badwords;
}
/**
 * setBadwords Метод установки списка плохих слов в список
 * @param badwords список плохих слов
 */
void anyks::Alm::setBadwords(const vector <string> & badwords) noexcept {
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
void anyks::Alm::setGoodwords(const std::set <size_t> & goodwords) noexcept {
	// Если список не пустой, устанавливаем список
	if(!goodwords.empty()) this->goodwords = goodwords;
}
/**
 * setGoodwords Метод установки списка хороших слов в список
 * @param goodwords список хороших слов
 */
void anyks::Alm::setGoodwords(const vector <string> & goodwords) noexcept {
	// Если список не пустой
	if(!goodwords.empty()){
		// Очищаем список хороших слов
		this->goodwords.clear();
		// Переходим по всему списку слов
		for(auto & word : goodwords) this->addGoodword(word);
	}
}
/**
 * setUserTokenMethod Метод установки функции обработки пользовательского токена
 * @param name слово - обозначение токена
 * @param fn   внешняя функция обрабатывающая пользовательский токен
 */
void anyks::Alm::setUserTokenMethod(const string & name, function <const bool (const string &, const string &)> fn) noexcept {
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
 * setVocab Метод загрузки бинарных данных в словарь
 * @param buffer буфер с бинарными данными
 * @param arpa   нужно добавить только данные arpa
 */
void anyks::Alm::setVocab(const vector <char> & buffer) noexcept {
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
			if(!word.empty()) this->addWord(idw, move(word));
		}
	}
}
/**
 * getVocab Метод извлечения данных словаря в бинарном виде
 * @param callback функция обратного вызова
 */
void anyks::Alm::getVocab(function <void (const vector <char> &, const u_short)> callback) const noexcept {
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
 * sentences Метод генерации предложений
 * @param callback функция обратного вызова
 */
void anyks::Alm::sentences(function <const bool (const wstring &)> callback) const noexcept {
	// Выводим результат
	callback(L"");
}
/**
 * getUppers Метод извлечения регистров для каждого слова
 * @param seq  последовательность слов для сборки контекста
 * @param upps список извлечённых последовательностей
 */
void anyks::Alm::getUppers(const vector <size_t> & seq, vector <size_t> & upps) const noexcept {
	// Блокируем варнинг
	(void) seq;
	(void) upps;
}
/**
 * context Метод сборки текстового контекста из последовательности
 * @param seq  последовательность слов для сборки контекста
 * @param nwrd флаг разрешающий вывод системных токенов
 * @return     собранный текстовый контекст
 */
const wstring anyks::Alm::context(const vector <size_t> & seq, const bool nwrd) const noexcept {
	// Блокируем варнинг
	(void) seq;
	(void) nwrd;
 	// Выводим результат
	return L"";
}
/**
 * find Метод поиска n-грамм в тексте
 * @param text     текст в котором необходимо найти n-граммы
 * @param callback функция обратного вызова
 */
void anyks::Alm::find(const string & text, function <void (const string &)> callback) const noexcept {
	// Если текст передан, выполняем обработку
	if(!text.empty()){
		// Выполняем поиск n-грамм в тексте
		this->find(this->alphabet->convert(text), [&callback, this](const wstring & text) noexcept {
			// Выводим результат
			if(!text.empty()) callback(this->alphabet->convert(text));
		});
	}
}
/**
 * find Метод поиска n-грамм в тексте
 * @param text     текст в котором необходимо найти n-граммы
 * @param callback функция обратного вызова
 */
void anyks::Alm::find(const wstring & text, function <void (const wstring &)> callback) const noexcept {
	// Блокируем варнинг
	(void) text;
	// Выводим результат
	callback(L"");
}
/**
 * read Метод чтения данных из файла arpa
 * @param filename адрес файла для чтения
 * @param status   функция вывода статуса
 */
void anyks::Alm::read(const string & filename, function <void (const u_short)> status) noexcept {
	// Если адрес файла передан
	if(!filename.empty() && fsys_t::isfile(filename)){
		// Тип извлечения данных
		u_short type = 0;
		// Последовательность для добавления
		vector <size_t> seq;
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
									// Полученное слово
									word_t word = L"";
									// Идентификатор и регистры слова
									size_t idw = 0, uppers = 0;
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
												if(this->unknown == 0){
													// Обнуляем регистры слова
													uppers = 0;
													// Формируем последовательность
													seq.push_back(idw);
												// Если неизвестное слово установлено
												} else if(this->unknown > 0) {
													// Формируем последовательность
													seq.push_back(this->unknown);
													// Получаем регистры слова
													uppers = this->vocab.at(this->unknown).getUppers();
												}
											// Добавляем слово в список последовательности
											} else {
												// Формируем последовательность
												seq.push_back(idw);
												// Получаем регистры слова
												uppers = ((words.size() > 1) && (this->utokens.count(idw) > 0) ? 0 : word.getUppers());
											}
										// Если слово найдено в всписке запрещённых
										} else {
											// Очищаем последовательность
											seq.clear();
											// Выходим из цикла
											break;
										}
									}
									// Если количество собранных n-грамм выше установленных, меняем
									if(seq.size() > size_t(this->size)) this->size = seq.size();
									// Добавляем последовательность в словарь
									if(!seq.empty()){
										// Проверяем отсутствует ли слово в списке запрещённых слов
										if(!this->tokenizer->isToken(idw) && (this->badwords.count(idw) < 1)) this->addWord(idw, word);
										// Добавляем последовательность в языковую модель
										this->set(seq, uppers, stod(weight), stod(backoff));
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
	// Выводим сообщение об ошибке
	} else if(this->isOption(options_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "arpa file is not exist");
}
/**
 * sentencesToFile Метод сборки указанного количества предложений и записи в файл
 * @param counts   количество предложений для сборки
 * @param filename адрес файла для записи результата
 * @param status   функция вывода статуса
 */
void anyks::Alm::sentencesToFile(const u_short counts, const string & filename, function <void (const u_short)> status) const noexcept {
	// Если данные переданы верные
	if((counts > 0) && (counts < 65000) && (this->isOption(options_t::debug) || !filename.empty())){
		// Индекс собранных данных, статус и процентное соотношение
		u_short index = 0, actual = 0, rate = 100;
		// Выполняем сборку предложений
		this->sentences([&](const wstring & text){
			// Увеличиваем индекс собранных данных
			index++;
			// Если текст получен
			if(!text.empty()){
				// Выполняем запись в файл
				if(!filename.empty()) this->alphabet->log("%ls", alphabet_t::log_t::null, filename.c_str(), text.c_str());
				// Выводим результат в консоль, если включён режим отладки или файл для записи не передан
				if(this->isOption(options_t::debug)) this->alphabet->log("%ls", alphabet_t::log_t::info, nullptr, text.c_str());
			}
			// Если отладка включена
			if(status != nullptr){
				// Подсчитываем статус выполнения
				actual = u_short(index / double(counts) * 100.0);
				// Если процентное соотношение изменилось
				if(rate != actual){
					// Запоминаем текущее процентное соотношение
					rate = actual;
					// Выводим результат
					status(actual);
				}
			}
			// Продолжаем работу или прекращаем её
			return (index < counts);
		});
	}
}
/**
 * findByFiles Метод поиска n-грамм в текстовом файле
 * @param path     адрес каталога или файла для обработки
 * @param filename адрес файла для записи результата
 * @param status   функция вывода статуса
 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
 */
void anyks::Alm::findByFiles(const string & path, const string & filename, function <void (const string &, const u_short)> status, const string & ext) const noexcept {
	// Если данные переданы
	if(!path.empty() && !filename.empty()){
		// Общий размер полученных данных
		size_t csize = 0;
		// Статус и процентное соотношение
		u_short actual = 0, rate = 100;
		/**
		 * runFn Функция запусука проверки n-грамм
		 * @param text текст для обработки
		 * @param readfile обрабатываемый в данный момент файл
		 * @param size     размер обрабатываемого файла
		 */
		auto runFn = [&](const string & text, const string & readfile, const size_t size){
			// Если текст получен
			if(!text.empty()){
				// Собранный список n-грамм текста
				string result = "";
				// Флаг переноса строки
				bool isOBreak = false;
				// Выполняем поиск n-грамм
				this->find(text, [&result, &isOBreak](const string & text){
					// Выводим список найденных n-рамм
					if(!text.empty()){
						// Проверяем, является ли текст переносом строки
						const bool isBreak = (text.compare("\r\n") == 0);
						// Если текущий флаг это перенос строки и предыдущий тоже
						if(isBreak && isOBreak) return;
						// Собираем все n-граммы
						result.append(text);
						// Добавляем перенос строки
						if(!isBreak) result.append("\r\n");
						// Запоминаем текущий флаг
						isOBreak = isBreak;
					}
				});
				// Если результат получен
				if(!result.empty()){
					// Выполняем блокировку потока
					this->locker.lock();
					// Выполняем запись в файл
					this->alphabet->log("%s", alphabet_t::log_t::null, filename.c_str(), result.c_str());
					// Выводим результат
					if(this->isOption(options_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::info, nullptr, result.c_str());
					// Выполняем разблокировку потока
					this->locker.unlock();
				}
			}
			// Если отладка включена
			if(status != nullptr){
				// Выполняем блокировку потока
				this->locker.lock();
				// Общий полученный размер данных
				csize += text.size();
				// Подсчитываем статус выполнения
				actual = u_short(csize / double(size) * 100.0);
				// Если процентное соотношение изменилось
				if(rate != actual){
					// Запоминаем текущее процентное соотношение
					rate = actual;
					// Выводим результат
					status(readfile, actual);
				}
				// Выполняем разблокировку потока
				this->locker.unlock();
			}
		};
		// Выполняем инициализацию тредпула
		this->tpool.init(this->threads);
		// Если это файл
		if(fsys_t::isfile(path)){
			// Выполняем считывание всех строк текста
			fsys_t::rfile(path, [&path, &runFn, this](const string & text, const uintmax_t fileSize) noexcept {
				// Выполняем обработку
				this->tpool.push(runFn, text, path, fileSize);
			});
		// Если это каталог
		} else if(fsys_t::isdir(path)) {
			// Выполняем загрузку каталога с текстовыми файлами
			fsys_t::rfdir(path, ext, [&](const string & text, const string & filename, const uintmax_t fileSize, const uintmax_t dirSize) noexcept {
				// Выполняем обработку
				this->tpool.push(runFn, text, filename, dirSize);
			});
		}
		// Ожидаем завершения обработки
		this->tpool.wait();
	}
}
/**
 * fixUppersByFiles Метод исправления регистров текста в текстовом файле
 * @param path     адрес каталога или файла для обработки
 * @param filename адрес файла для записи результата
 * @param status   функция вывода статуса
 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
 */
void anyks::Alm::fixUppersByFiles(const string & path, const string & filename, function <void (const string &, const u_short)> status, const string & ext) const noexcept {
	// Если данные переданы
	if(!path.empty() && !filename.empty()){
		// Общий размер полученных данных
		size_t csize = 0;
		// Статус и процентное соотношение
		u_short actual = 0, rate = 100;
		/**
		 * runFn Функция запусука проверки n-грамм
		 * @param text текст для обработки
		 * @param readfile обрабатываемый в данный момент файл
		 * @param size     размер обрабатываемого файла
		 */
		auto runFn = [&](const string & text, const string & readfile, const size_t size){
			// Если текст получен
			if(!text.empty()){
				// Выполняем првоерку текста
				const string & str = this->fixUppers(text);
				// Если строка получена
				if(!str.empty()){
					// Выполняем блокировку потока
					this->locker.lock();
					// Выполняем запись в файл
					this->alphabet->log("%s\r\n", alphabet_t::log_t::null, filename.c_str(), str.c_str());
					// Выводим результат
					if(this->isOption(options_t::debug)) this->alphabet->log("%s\r\n", alphabet_t::log_t::info, nullptr, str.c_str());
					// Выполняем разблокировку потока
					this->locker.unlock();
				}
			}
			// Если отладка включена
			if(status != nullptr){
				// Выполняем блокировку потока
				this->locker.lock();
				// Общий полученный размер данных
				csize += text.size();
				// Подсчитываем статус выполнения
				actual = u_short(csize / double(size) * 100.0);
				// Если процентное соотношение изменилось
				if(rate != actual){
					// Запоминаем текущее процентное соотношение
					rate = actual;
					// Выводим результат
					status(readfile, actual);
				}
				// Выполняем разблокировку потока
				this->locker.unlock();
			}
		};
		// Выполняем инициализацию тредпула
		this->tpool.init(this->threads);
		// Если это файл
		if(fsys_t::isfile(path)){
			// Выполняем считывание всех строк текста
			fsys_t::rfile(path, [&path, &runFn, this](const string & text, const uintmax_t fileSize) noexcept {
				// Выполняем обработку
				this->tpool.push(runFn, text, path, fileSize);
			});
		// Если это каталог
		} else if(fsys_t::isdir(path)) {
			// Выполняем загрузку каталога с текстовыми файлами
			fsys_t::rfdir(path, ext, [&](const string & text, const string & filename, const uintmax_t fileSize, const uintmax_t dirSize) noexcept {
				// Выполняем обработку
				this->tpool.push(runFn, text, filename, dirSize);
			});
		}
		// Ожидаем завершения обработки
		this->tpool.wait();
	}
}
/**
 * countsByFiles Метод подсчёта количества n-грамм в текстовом файле
 * @param path     адрес каталога или файла для обработки
 * @param filename адрес файла для записи результата
 * @param ngrams   размер n-граммы для подсчёта
 * @param status   функция вывода статуса
 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
 */
void anyks::Alm::countsByFiles(const string & path, const string & filename, const u_short ngrams, function <void (const string &, const u_short)> status, const string & ext) const noexcept {
	// Если данные переданы
	if(!path.empty() && !filename.empty()){
		// Общий размер полученных данных
		size_t csize = 0, count = 0;
		// Статус и процентное соотношение
		u_short actual = 0, rate = 100;
		/**
		 * runFn Функция запусука проверки n-грамм
		 * @param text текст для обработки
		 * @param readfile обрабатываемый в данный момент файл
		 * @param size     размер обрабатываемого файла
		 */
		auto runFn = [&](const string & text, const string & readfile, const size_t size){
			// Если текст получен
			if(!text.empty()){
				// Количество грамм в тексте
				size_t grams = 0;
				// Определяем тип размеров n-грамм
				switch(ngrams){
					// Если размер n-грамм не определен
					case 1: {
						// Выполняем расчёт количества n-грамм
						grams = this->grams(text);
						// Выполняем блокировку потока
						this->locker.lock();
						// Выводим сообщение в файл
						this->alphabet->log("%zu | %s\r\n", alphabet_t::log_t::null, filename.c_str(), grams, text.c_str());
						// Выводим сообщение в консоль
						if(this->isOption(options_t::debug)) this->alphabet->log("%zu | %s\r\n", alphabet_t::log_t::info, nullptr, grams, text.c_str());
						// Выполняем разблокировку потока
						this->locker.unlock();
					} break;
					// Если размер n-грамм биграммы
					case 2: {
						// Выполняем расчёт количества биграмм
						grams = this->bigrams(text);
						// Выполняем блокировку потока
						this->locker.lock();
						// Выводим сообщение в файл
						this->alphabet->log("%zu | %s\r\n", alphabet_t::log_t::null, filename.c_str(), grams, text.c_str());
						// Выводим сообщение в консоль
						if(this->isOption(options_t::debug)) this->alphabet->log("%zu | %s\r\n", alphabet_t::log_t::info, nullptr, grams, text.c_str());
						// Выполняем разблокировку потока
						this->locker.unlock();
					} break;
					// Если размер n-грамм триграммы
					case 3: {
						// Выполняем расчёт количества триграмм
						grams = this->trigrams(text);
						// Выполняем блокировку потока
						this->locker.lock();
						// Выводим сообщение в файл
						this->alphabet->log("%zu | %s\r\n", alphabet_t::log_t::null, filename.c_str(), grams, text.c_str());
						// Выводим сообщение в консоль
						if(this->isOption(options_t::debug)) this->alphabet->log("%zu | %s\r\n", alphabet_t::log_t::info, nullptr, grams, text.c_str());
						// Выполняем разблокировку потока
						this->locker.unlock();
					} break;
				}
				// Выполняем блокировку потока
				this->locker.lock();
				// Увкличиваем количество собранных n-грамм
				count += grams;
				// Выполняем разблокировку потока
				this->locker.unlock();
			}
			// Если отладка включена
			if(status != nullptr){
				// Выполняем блокировку потока
				this->locker.lock();
				// Общий полученный размер данных
				csize += text.size();
				// Подсчитываем статус выполнения
				actual = u_short(csize / double(size) * 100.0);
				// Если процентное соотношение изменилось
				if(rate != actual){
					// Запоминаем текущее процентное соотношение
					rate = actual;
					// Выводим результат
					status(readfile, actual);
				}
				// Выполняем разблокировку потока
				this->locker.unlock();
			}
		};
		// Выполняем инициализацию тредпула
		this->tpool.init(this->threads);
		// Если это файл
		if(fsys_t::isfile(path)){
			// Выполняем считывание всех строк текста
			fsys_t::rfile(path, [&path, &runFn, this](const string & text, const uintmax_t fileSize) noexcept {
				// Выполняем обработку
				this->tpool.push(runFn, text, path, fileSize);
			});
		// Если это каталог
		} else if(fsys_t::isdir(path)) {
			// Выполняем загрузку каталога с текстовыми файлами
			fsys_t::rfdir(path, ext, [&](const string & text, const string & filename, const uintmax_t fileSize, const uintmax_t dirSize) noexcept {
				// Выполняем обработку
				this->tpool.push(runFn, text, filename, dirSize);
			});
		}
		// Ожидаем завершения обработки
		this->tpool.wait();
		// Выводим сообщение об общем количестве обработанных n-грамм в консоль
		this->alphabet->log("Counts %hugrams: %zu\r\n", alphabet_t::log_t::null, nullptr, (ngrams == 1 ? this->size : ngrams), count);
		// Выводим сообщение об общем количестве обработанных n-грамм в файл
		this->alphabet->log("Counts %hugrams: %zu\r\n", alphabet_t::log_t::null, filename.c_str(), (ngrams == 1 ? this->size : ngrams), count);
	}
}
/**
 * checkByFiles Метод проверки существования последовательности в текстовом файле
 * @param path     адрес каталога или файла для обработки
 * @param filename адрес файла для записи результата
 * @param step     шаг размера N-граммы для перебора текста
 * @param status   функция вывода статуса
 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
 */
void anyks::Alm::checkByFiles(const string & path, const string & filename, const u_short step, function <void (const string &, const u_short)> status, const string & ext) const noexcept {
	// Если данные переданы
	if(!path.empty() && !filename.empty() && (this->size >= step)){
		// Статус и процентное соотношение
		u_short actual = 0, rate = 100;
		// Общий размер полученных данных
		size_t csize = 0, count = 0, exists = 0;
		/**
		 * runFn Функция запусука проверки n-грамм
		 * @param text текст для обработки
		 * @param readfile обрабатываемый в данный момент файл
		 * @param size     размер обрабатываемого файла
		 */
		auto runFn = [&](const string & text, const string & readfile, const size_t size){
			// Если текст получен
			if(!text.empty()){
				// Выполняем првоерку текста
				const auto & check = this->exist(text, u_short(step < 2 ? 2 : step));
				// Выполняем блокировку потока
				this->locker.lock();
				// Считаем количество обработанных предложений
				count++;
				// Если слово найдено считаем количество предложений
				if(check.first) exists++;
				// Выполняем запись в файл
				this->alphabet->log("%zu | %s | %s\r\n", alphabet_t::log_t::null, filename.c_str(), count, (check.first ? "YES" : "NO"), text.c_str());
				// Выводим результат
				if(this->isOption(options_t::debug)) this->alphabet->log("%zu | %s | %s\r\n", alphabet_t::log_t::info, nullptr, count, (check.first ? "YES" : "NO"), text.c_str());
				// Выполняем разблокировку потока
				this->locker.unlock();
			}
			// Если отладка включена
			if(status != nullptr){
				// Выполняем блокировку потока
				this->locker.lock();
				// Общий полученный размер данных
				csize += text.size();
				// Подсчитываем статус выполнения
				actual = u_short(csize / double(size) * 100.0);
				// Если процентное соотношение изменилось
				if(rate != actual){
					// Запоминаем текущее процентное соотношение
					rate = actual;
					// Выводим результат
					status(readfile, actual);
				}
				// Выполняем разблокировку потока
				this->locker.unlock();
			}
		};
		// Выполняем инициализацию тредпула
		this->tpool.init(this->threads);
		// Если это файл
		if(fsys_t::isfile(path)){
			// Выполняем считывание всех строк текста
			fsys_t::rfile(path, [&path, &runFn, this](const string & text, const uintmax_t fileSize) noexcept {
				// Выполняем обработку
				this->tpool.push(runFn, text, path, fileSize);
			});
		// Если это каталог
		} else if(fsys_t::isdir(path)) {
			// Выполняем загрузку каталога с текстовыми файлами
			fsys_t::rfdir(path, ext, [&](const string & text, const string & filename, const uintmax_t fileSize, const uintmax_t dirSize) noexcept {
				// Выполняем обработку
				this->tpool.push(runFn, text, filename, dirSize);
			});
		}
		// Ожидаем завершения обработки
		this->tpool.wait();
		// Выводим сообщение об общем количестве обработанных предложений
		this->alphabet->log("All texts: %zu\r\nExists texts: %zu\r\nNot exists texts: %zu\r\n", alphabet_t::log_t::null, nullptr, count, exists, count - exists);
		// Выполняем запись в файл
		this->alphabet->log("All texts: %zu\r\nExists texts: %zu\r\nNot exists texts: %zu\r\n", alphabet_t::log_t::null, filename.c_str(), count, exists, count - exists);
	}
}
/**
 * checkByFiles Метод проверки существования последовательности в текстовом файле
 * @param path     адрес каталога или файла для обработки
 * @param filename адрес файла для записи результата
 * @param accurate режим точной проверки
 * @param status   функция вывода статуса
 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
 */
void anyks::Alm::checkByFiles(const string & path, const string & filename, const bool accurate, function <void (const string &, const u_short)> status, const string & ext) const noexcept {
	// Если данные переданы
	if(!path.empty() && !filename.empty()){
		// Статус и процентное соотношение
		u_short actual = 0, rate = 100;
		// Общий размер полученных данных
		size_t csize = 0, count = 0, exists = 0;
		/**
		 * runFn Функция запусука проверки n-грамм
		 * @param text текст для обработки
		 * @param readfile обрабатываемый в данный момент файл
		 * @param size     размер обрабатываемого файла
		 */
		auto runFn = [&](const string & text, const string & readfile, const size_t size){
			// Если текст получен
			if(!text.empty()){
				// Выполняем првоерку текста
				const auto & res = this->check(text, accurate);
				// Выполняем блокировку потока
				this->locker.lock();
				// Считаем количество обработанных предложений
				count++;
				// Если слово найдено считаем количество предложений
				if(res.first) exists++;
				// Выполняем запись в файл
				this->alphabet->log("%zu | %s | %s\r\n", alphabet_t::log_t::null, filename.c_str(), count, (res.first ? "YES" : "NO"), text.c_str());
				// Выводим результат
				if(this->isOption(options_t::debug)) this->alphabet->log("%zu | %s | %s\r\n", alphabet_t::log_t::info, nullptr, count, (res.first ? "YES" : "NO"), text.c_str());
				// Выполняем разблокировку потока
				this->locker.unlock();
			}
			// Если отладка включена
			if(status != nullptr){
				// Выполняем блокировку потока
				this->locker.lock();
				// Общий полученный размер данных
				csize += text.size();
				// Подсчитываем статус выполнения
				actual = u_short(csize / double(size) * 100.0);
				// Если процентное соотношение изменилось
				if(rate != actual){
					// Запоминаем текущее процентное соотношение
					rate = actual;
					// Выводим результат
					status(readfile, actual);
				}
				// Выполняем разблокировку потока
				this->locker.unlock();
			}
		};
		// Выполняем инициализацию тредпула
		this->tpool.init(this->threads);
		// Если это файл
		if(fsys_t::isfile(path)){
			// Выполняем считывание всех строк текста
			fsys_t::rfile(path, [&path, &runFn, this](const string & text, const uintmax_t fileSize) noexcept {
				// Выполняем обработку
				this->tpool.push(runFn, text, path, fileSize);
			});
		// Если это каталог
		} else if(fsys_t::isdir(path)) {
			// Выполняем загрузку каталога с текстовыми файлами
			fsys_t::rfdir(path, ext, [&](const string & text, const string & filename, const uintmax_t fileSize, const uintmax_t dirSize) noexcept {
				// Выполняем обработку
				this->tpool.push(runFn, text, filename, dirSize);
			});
		}
		// Ожидаем завершения обработки
		this->tpool.wait();
		// Выводим сообщение об общем количестве обработанных предложений
		this->alphabet->log("All texts: %zu\r\nExists texts: %zu\r\nNot exists texts: %zu\r\n", alphabet_t::log_t::null, nullptr, count, exists, count - exists);
		// Выполняем запись в файл
		this->alphabet->log("All texts: %zu\r\nExists texts: %zu\r\nNot exists texts: %zu\r\n", alphabet_t::log_t::null, filename.c_str(), count, exists, count - exists);
	}
}
/**
 * getSize Метод получения размера n-грамы
 * @return длина n-граммы в языковой моделе
 */
const u_short anyks::Alm::getSize() const noexcept {
	// Выводим размер n-граммы
	return this->size;
}
/**
 * getOptions Метод извлечения установленных опций
 * @return установленные опции
 */
const u_short anyks::Alm::getOptions() const noexcept {
	// Выводим установленные опции
	return this->options.to_ulong();
}
/**
 * getUserTokenId Метод получения идентификатора пользовательского токена
 * @param name слово для которого нужно получить идентификатор
 * @return     идентификатор пользовательского токена соответствующий слову
 */
const size_t anyks::Alm::getUserTokenId(const string & name) const noexcept {
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
 * getIdw Метод генерирования идентификатора слова
 * @param  word  слово для генерации
 * @param  check нужно выполнить дополнительную проверку слова
 * @return       идентификатор слова
 */
const size_t anyks::Alm::getIdw(const wstring & word, const bool check) const noexcept {
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
		// Проверяем является ли слово греческим символом
		else if(word.compare(L"<greek>") == 0) result = (size_t) token_t::greek;
		// Проверяем является ли слово знаком направления (стрелкой)
		else if(word.compare(L"<route>") == 0) result = (size_t) token_t::route;
		// Проверяем является ли слово знаком изоляции
		else if(word.compare(L"<isolat>") == 0) result = (size_t) token_t::isolat;
		// Проверяем является ли слово знаком игральной карты
		else if(word.compare(L"<pcards>") == 0) result = (size_t) token_t::pcards;
		// Проверяем является ли слово знаком мировой валюты
		else if(word.compare(L"<currency>") == 0) result = (size_t) token_t::currency;
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
						const wchar_t letter = this->alphabet->toLower(tmp.front());
						// Если это знак пунктуации
						if(this->alphabet->isPunct(letter)){
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::punct) < 1) result = (size_t) token_t::punct;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::punct) > 0) result = (size_t) token_t::unk;
						// Если буква является арабским числом
						} else if(this->alphabet->isNumber(wstring(1, letter))) {
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
						// Если это символ греческого алфавита
						} else if(this->alphabet->isGreek(letter)) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::greek) < 1) result = (size_t) token_t::greek;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::greek) > 0) result = (size_t) token_t::unk;
						// Если это символ направления (стрелка)
						} else if(this->alphabet->isRoute(letter)) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::route) < 1) result = (size_t) token_t::route;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::route) > 0) result = (size_t) token_t::unk;
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
						// Если это символ игральных карт
						} else if(this->alphabet->isPlayCards(letter)) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::pcards) < 1) result = (size_t) token_t::pcards;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::pcards) > 0) result = (size_t) token_t::unk;
						// Если это знак мировой валюты
						} else if(this->alphabet->isCurrency(letter)) {
							// Если идентифицирование токена не отключено
							if(this->tokenDisable.count(token_t::currency) < 1) result = (size_t) token_t::currency;
							// Устанавливаем токен неизвестного слова
							if(this->tokenUnknown.count(token_t::currency) > 0) result = (size_t) token_t::unk;
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
						if((idt == token_t::num) && !this->alphabet->isNumber(wstring(1, tmp.back()))){
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
 * getUnknown Метод извлечения неизвестного слова
 * @return установленное неизвестное слово
 */
const string & anyks::Alm::getUnknown() const noexcept {
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
const string & anyks::Alm::getWordScript() const noexcept {
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
const string & anyks::Alm::getUserTokenScript() const noexcept {
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
 * getUserTokenWord Метод получения пользовательского токена по его идентификатору
 * @param idw идентификатор пользовательского токена
 * @return    пользовательский токен соответствующий идентификатору
 */
const string anyks::Alm::getUserTokenWord(const size_t idw) const noexcept {
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
 * word Метод извлечения слова по его идентификатору
 * @param idw идентификатор слова
 * @param ups регистры слова
 * @return    слово соответствующее идентификатору
 */
const anyks::word_t anyks::Alm::word(const size_t idw, const size_t ups) const noexcept {
	// Результат работы функции
	word_t result = L"";
	// Определяем тип записи
	switch(idw){
		// Если это токен арабского или римского числа
		case u_short(token_t::num): result = L"<num>"; break;
		// Если это токен неизвестного слова
		case u_short(token_t::unk): result = L"<unk>"; break;
		// Если это токен url адреса
		case u_short(token_t::url): result = L"<url>"; break;
		// Если это токен начала предложения
		case u_short(token_t::start): result = L"<s>"; break;
		// Если это токен аббревиатуры
		case u_short(token_t::abbr): result = L"<abbr>"; break;
		// Если это токен даты
		case u_short(token_t::date): result = L"<date>"; break;
		// Если это токен времени
		case u_short(token_t::time): result = L"<time>"; break;
		// Если это токен псевдо-числа
		case u_short(token_t::anum): result = L"<anum>"; break;
		// Если это символ математической операции
		case u_short(token_t::math): result = L"<math>"; break;
		// Если это токен конца предложения
		case u_short(token_t::finish): result = L"</s>"; break;
		// Если это спец-символ
		case u_short(token_t::specl): result = L"<specl>"; break;
		// Если это токен диапазона чисел
		case u_short(token_t::range): result = L"<range>"; break;
		// Если это токен знака пунктуации
		case u_short(token_t::punct): result = L"<punct>"; break;
		// Если это токен знака греческого символа
		case u_short(token_t::greek): result = L"<greek>"; break;
		// Если это токен знака направления (стрелки)
		case u_short(token_t::route): result = L"<route>"; break;
		// Если это токен приблизительного числа
		case u_short(token_t::aprox): result = L"<aprox>"; break;
		// Если это токен числового счёта
		case u_short(token_t::score): result = L"<score>"; break;
		// Если это токен габаритных размеров
		case u_short(token_t::dimen): result = L"<dimen>"; break;
		// Если это токен числовой дроби
		case u_short(token_t::fract): result = L"<fract>"; break;
		// Если это токен изоляции
		case u_short(token_t::isolat): result = L"<isolat>"; break;
		// Если это токен знака игральных карт
		case u_short(token_t::pcards): result = L"<pcards>"; break;
		// Если это токен знака мировой валюты
		case u_short(token_t::currency): result = L"<currency>"; break;
		// Если это нормальное слово
		default: {
			// Получаем слово по его идентификатору
			const word_t * word = this->getWord(idw);
			// Если слово получено
			if(word != nullptr){
				// Получаем данные слова
				word_t tmp = (* word);
				// Устанавливаем регистры слова
				tmp.setUppers(ups);
				// Выводим результат
				result = tmp;
			// Устанавливаем как неизвестное слово
			} else result = L"<unk>";
		}
	}
	// Выводим результат
	return result;
}
/**
 * getBadwords Метод извлечения чёрного списка
 * @return чёрный список слов
 */
const std::set <size_t> & anyks::Alm::getBadwords() const noexcept {
	// Выводим результат
	return this->badwords;
}
/**
 * getGoodwords Метод извлечения белого списка
 * @return белый список слов
 */
const std::set <size_t> & anyks::Alm::getGoodwords() const noexcept {
	// Выводим результат
	return this->goodwords;
}
/**
 * getTokensUnknown Метод извлечения списка токенов приводимых к <unk>
 * @return список токенов
 */
const std::set <anyks::token_t> & anyks::Alm::getTokensUnknown() const noexcept {
	// Выводим список токенов
	return this->tokenUnknown;
}
/**
 * getTokensDisable Метод извлечения списка запрещённых токенов
 * @return список токенов
 */
const std::set <anyks::token_t> & anyks::Alm::getTokensDisable() const noexcept {
	// Выводим список токенов
	return this->tokenDisable;
}
/**
 * getUserTokens Метод извлечения списка пользовательских токенов
 * @return список пользовательских токенов
 */
const vector <string> & anyks::Alm::getUserTokens() const noexcept {
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
 * grams Метод проверки количества найденных в тексте n-грамм
 * @param text текст для расчёта
 * @return     количество найденных n-грамм
 */
const size_t anyks::Alm::grams(const string & text) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если текст передан
	if(!text.empty()){
		// Список полученных слов
		vector <wstring> words;
		// Выполняем сплит текста
		this->alphabet->split(text, " ", words);
		// Если список слов получен
		if(!words.empty()){
			// Идентификатор слова
			size_t idw = 0;
			// Последовательность n-грамм
			vector <size_t> seq;
			// Переходим по всему списку слов и формируем последовательность
			for(auto & word : words){
				// Получаем идентификатор слова
				idw = this->getIdw(word);
				// Если идентификатор получен
				if(idw > 0) seq.push_back(idw);
			}
			// Если последовательность получена, выполняем расчёт
			if(!seq.empty()) result = this->grams(seq);
		}
	}
	// Выводим результат
	return result;
}
/**
 * grams Метод проверки количества найденных n-грамм
 * @param seq список последовательностей
 * @return    количество найденных n-грамм
 */
const size_t anyks::Alm::grams(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если последовательность передана
	if(!seq.empty()){
		// Временная последовательность
		vector <size_t> tmp;
		// Количество переданных последовательностей
		const size_t count = seq.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = (count > size_t(this->size) ? this->size : count);
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(seq.begin() + offset1, seq.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty() && this->exist(tmp)) result++;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * grams Метод проверки количества найденных n-грамм
 * @param words список слов для расчёта
 * @return      количество найденных n-грамм
 */
const size_t anyks::Alm::grams(const vector <wstring> & words) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если список слов переданы
	if(!words.empty()){
		// Список последовательности
		vector <size_t> seq(words.size());
		// Переходим по всем словам последовательности
		for(size_t i = 0; i < words.size(); i++){
			// Получаем идентификатор слова в последовательности
			seq.at(i) = this->getIdw(words.at(i));
		}
		// Выполняем расчёт перплексии
		if(!seq.empty()) result = this->grams(seq);
	}
	// Выводим результат
	return result;
}
/**
 * bigrams Метод проверки количества найденных в тексте биграмм
 * @param text текст для расчёта
 * @return     количество найденных биграмм
 */
const size_t anyks::Alm::bigrams(const string & text) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если текст передан
	if(!text.empty()){
		// Список полученных слов
		vector <wstring> words;
		// Выполняем сплит текста
		this->alphabet->split(text, " ", words);
		// Если список слов получен
		if(!words.empty()){
			// Идентификатор слова
			size_t idw = 0;
			// Последовательность n-грамм
			vector <size_t> seq;
			// Переходим по всему списку слов и формируем последовательность
			for(auto & word : words){
				// Получаем идентификатор слова
				idw = this->getIdw(word);
				// Если идентификатор получен
				if(idw > 0) seq.push_back(idw);
			}
			// Если последовательность получена, выполняем расчёт
			if(!seq.empty()) result = this->bigrams(seq);
		}
	}
	// Выводим результат
	return result;
}
/**
 * bigrams Метод проверки количества найденных биграмм
 * @param seq список последовательностей
 * @return    количество найденных биграмм
 */
const size_t anyks::Alm::bigrams(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если последовательность передана
	if(!seq.empty() && (seq.size() > 1)){
		// Временная последовательность
		vector <size_t> tmp;
		// Количество переданных последовательностей
		const size_t count = seq.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = 2;
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(seq.begin() + offset1, seq.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty() && this->exist(tmp)) result++;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * bigrams Метод проверки количества найденных биграмм
 * @param words список слов для расчёта
 * @return      количество найденных биграмм
 */
const size_t anyks::Alm::bigrams(const vector <wstring> & words) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если список слов переданы
	if(!words.empty()){
		// Список последовательности
		vector <size_t> seq(words.size());
		// Переходим по всем словам последовательности
		for(size_t i = 0; i < words.size(); i++){
			// Получаем идентификатор слова в последовательности
			seq.at(i) = this->getIdw(words.at(i));
		}
		// Выполняем расчёт перплексии
		if(!seq.empty()) result = this->bigrams(seq);
	}
	// Выводим результат
	return result;
}
/**
 * trigrams Метод проверки количества найденных в тексте триграмм
 * @param text текст для расчёта
 * @return     количество найденных триграмм
 */
const size_t anyks::Alm::trigrams(const string & text) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если текст передан
	if(!text.empty()){
		// Список полученных слов
		vector <wstring> words;
		// Выполняем сплит текста
		this->alphabet->split(text, " ", words);
		// Если список слов получен
		if(!words.empty()){
			// Идентификатор слова
			size_t idw = 0;
			// Последовательность n-грамм
			vector <size_t> seq;
			// Переходим по всему списку слов и формируем последовательность
			for(auto & word : words){
				// Получаем идентификатор слова
				idw = this->getIdw(word);
				// Если идентификатор получен
				if(idw > 0) seq.push_back(idw);
			}
			// Если последовательность получена, выполняем расчёт
			if(!seq.empty()) result = this->trigrams(seq);
		}
	}
	// Выводим результат
	return result;
}
/**
 * trigrams Метод проверки количества найденных последовательных триграмм
 * @param seq список последовательностей
 * @return    количество найденных триграмм
 */
const size_t anyks::Alm::trigrams(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если последовательность передана
	if(!seq.empty() && (seq.size() > 2)){
		// Временная последовательность
		vector <size_t> tmp;
		// Количество переданных последовательностей
		const size_t count = seq.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = 3;
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(seq.begin() + offset1, seq.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty() && this->exist(tmp)) result++;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * trigrams Метод проверки количества найденных последовательных триграмм
 * @param words список слов для расчёта
 * @return      количество найденных триграмм
 */
const size_t anyks::Alm::trigrams(const vector <wstring> & words) const noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если список слов переданы
	if(!words.empty()){
		// Список последовательности
		vector <size_t> seq(words.size());
		// Переходим по всем словам последовательности
		for(size_t i = 0; i < words.size(); i++){
			// Получаем идентификатор слова в последовательности
			seq.at(i) = this->getIdw(words.at(i));
		}
		// Выполняем расчёт перплексии
		if(!seq.empty()) result = this->trigrams(seq);
	}
	// Выводим результат
	return result;
}
/**
 * Alm Конструктор
 */
anyks::Alm::Alm() noexcept {
	// Устанавливаем функции работы со словами
	this->setWordFn(nullptr, nullptr);
}
/**
 * Alm Конструктор
 * @param alphabet объект алфавита
 */
anyks::Alm::Alm(const alphabet_t * alphabet) noexcept {
	// Устанавливаем алфавит
	this->setAlphabet(alphabet);
	// Устанавливаем функции работы со словами
	this->setWordFn(nullptr, nullptr);
}
/**
 * Alm Конструктор
 * @param tokenizer объект токенизатора
 */
anyks::Alm::Alm(const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем токенизатор
	this->setTokenizer(tokenizer);
	// Устанавливаем функции работы со словами
	this->setWordFn(nullptr, nullptr);
}
/**
 * Alm Конструктор
 * @param alphabet  объект алфавита
 * @param tokenizer объект токенизатора
 */
anyks::Alm::Alm(const alphabet_t * alphabet, const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем алфавит
	this->setAlphabet(alphabet);
	// Устанавливаем токенизатор
	this->setTokenizer(tokenizer);
	// Устанавливаем функции работы со словами
	this->setWordFn(nullptr, nullptr);
}
/**
 * ~Alm Деструктор
 */
anyks::Alm::~Alm() noexcept {
	// Очищаем языковую модель
	this->clear();
	// Очищаем выделенную память под python
	if(!notCleanPython && (this->python != nullptr)) delete this->python;
}

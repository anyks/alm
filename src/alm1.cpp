/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <alm.hpp>

/**
 * exist Метод проверки существования последовательности
 * @param seq список слов последовательности
 * @return    результат проверки
 */
const bool anyks::Alm1::exist(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0) && !this->arpa.empty()){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		arpa_t * obj = &this->arpa;
		// Переходим по всему объекту
		for(auto & idw : seq){
			// Выполняем поиск нашего слова
			auto it = obj->find(idw);
			// Если слово найдено
			if(it != obj->end()){
				// Получаем блок структуры
				obj = &it->second;
				// Если мы дошли до конца
				result = (i == (seq.size() - 1));
			// Выходим из цикла
			} else break;
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
	}
	// Выводим результат
	return result;
}
/**
 * backoff Метод извлечения обратной частоты последовательности
 * @param seq последовательность для извлечения обратной частоты
 * @return    обратная частота последовательности
 */
const double anyks::Alm1::backoff(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	double result = 0.0;
	// Если контекст передан
	if(!seq.empty() && !this->arpa.empty()){
		// Получаем частоту последовательности
		const auto & data = this->frequency(seq);
		// Запоминаем обратную частоту последовательности
		if(data.second != this->zero) result = data.second;
		// Если последовательность длинее юниграммы
		if(seq.size() > 1){
			// Временная последовательность
			vector <size_t> tmp(seq.begin() + 1, seq.end());
			// Продолжаем увеличение обратной частоты
			result += this->backoff(tmp);
		}
	}
	// Выводим результат
	return result;
}
/**
 * weight Метод извлечения веса последовательности
 * @param seq последовательность для извлечения веса
 * @return    вес последовательноси и n-грамма для которой она получена
 */
const pair <u_short, double> anyks::Alm1::weight(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	pair <u_short, double> result = {0, 0.0};
	// Если контекст передан
	if(!seq.empty() && !this->arpa.empty()){
		// Флаг полностью собранной последовательности
		bool isFull = true;
		// Копируем основную карту
		const arpa_t * obj = &this->arpa;
		// Временная последовательность
		vector <size_t> tmp(seq.begin() + 1, seq.end());
		// Переходим по всей последовательности
		for(auto & idw : tmp){
			// Ищем нашу n-грамму
			auto it = obj->find(idw);
			// Если n-грамма найдена
			if(it != obj->end()) obj = &it->second;
			// Запоминаем что последовательность не собрана
			else {
				// Запоминаем что последовательность не существует
				isFull = false;
				// Выходим из цикла
				break;
			}
		}
		// Если последовательность существует, получаем её вес
		if(isFull && (obj->weight != this->zero))
			// Формируем полученный вес n-граммы
			result = make_pair((u_short) tmp.size(), obj->weight);
		// Иначе продолжаем дальше
		else result = this->weight(tmp);
	}
	// Выводим результат
	return result;
}
/**
 * frequency Метод извлечения частоты n-граммы
 * @param seq список слов последовательности
 * @return    частота и обратная частота n-граммы
 */
const pair <double, double> anyks::Alm1::frequency(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	pair <double, double> result = {this->zero, this->zero};
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0) && !this->arpa.empty()){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		arpa_t * obj = &this->arpa;
		// Переходим по всему объекту
		for(auto & idw : seq){
			// Выполняем поиск нашего слова
			auto it = obj->find(idw);
			// Если слово найдено
			if(it != obj->end()){
				// Получаем блок структуры
				obj = &it->second;
				// Если мы дошли до конца
				if(i == (seq.size() - 1)) result = make_pair(obj->weight, obj->backoff);
			// Выходим из цикла
			} else break;
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
	}
	// Выводим результат
	return result;
}
/**
 * set Метод установки последовательности в словарь
 * @param seq последовательность слов для установки
 */
void anyks::Alm1::set(const vector <alm_t::seq_t> & seq) const noexcept {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		arpa_t * obj = &this->arpa;
		// Переходим по всему объекту
		for(auto & item : seq){
			// Добавляем слово в словарь
			auto ret = obj->emplace(item.idw, arpa_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Запоминаем количество документов
			obj->weight = item.weight;
			// Запоминаем встречаемость так-как она есть
			obj->backoff = item.backoff;
			// Если регистры слова переданы, считаем их
			if(item.ups > 0) obj->uppers = item.ups;
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
	}
}
/**
 * set Метод установки последовательности в словарь
 * @param seq     список идентификаторов слов которые нужно добавить
 * @param uppers  список верхних регистров последнего слова последовательности
 * @param weight  вес n-граммы из файла arpa
 * @param backoff обратная частота документа из файла arpa
 */
void anyks::Alm1::set(const vector <size_t> & seq, const size_t uppers, const double weight, const double backoff) const noexcept {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		arpa_t * obj = &this->arpa;
		// Переходим по всему объекту
		for(auto & idw : seq){
			// Добавляем слово в словарь
			auto ret = obj->emplace(idw, arpa_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Если мы дошли до конца
			if(i == (seq.size() - 1)){
				// Запоминаем регистры слова
				obj->uppers = uppers;
				// Запоминаем обратную частоту документа
				obj->backoff = (backoff == 0.0 ? this->zero : backoff);
				// Запоминаем частоту n-граммы
				obj->weight = ((weight == 0.0) || (fabs(round(weight)) >= 99.0) ? this->zero : weight);
			}
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
	}
}
/**
 * clear Метод очистки всех данных
 */
void anyks::Alm1::clear(){
	// Выполняем очистку объекта arpa
	this->arpa.clear();
	// Выполняем удаление всех основных параметров
	reinterpret_cast <alm_t *> (this)->clear();
}
/**
 * perplexity Метод расчёта перплексии
 * @param  seq список последовательностей
 * @return     результат расчёта
 */
const anyks::Alm::ppl_t anyks::Alm1::perplexity(const vector <size_t> & seq) const noexcept {
	// Результат работы функции
	ppl_t result;
	// Если текст передан
	if(!this->arpa.empty() && (seq.size() > 2) && (this->size > 0) &&
	(seq.front() == size_t(token_t::start)) && (seq.back() == size_t(token_t::finish))){
		// Позиция n-граммы в контексте
		size_t index = 0;
		// Количество переданных последовательностей
		const size_t count = seq.size();
		// Текст данных отладки собранных при расчёте
		map <size_t, pair <string, string>> debugMessages;
		// Временная последовательность
		vector <size_t> tmp;
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = (count > this->size ? this->size : count);
		// Проверяем разрешено ли неизвестное слово
		const bool isAllowUnk = (this->frequency({size_t(token_t::unk)}).first != this->zero);
		/**
		 * debugFn Функция вывода отладочной информации
		 * @param first  первое слово
		 * @param second второе слово
		 * @param bigram является ли n-грамма длиннее биграммы
		 * @param gram   граммность n-граммы для которой был произведён расчёт
		 * @param weight полученный вес n-граммы при расчёте
		 * @param delim  проверочный делитель n-граммы
		 * @param pos    позиция n-граммы в контексте
		 */
		auto debugFn = [&debugMessages, this](const string & first, const string & second, const bool bigram, const u_short gram, const double weight, const double delim, const size_t pos){
			// Выводим отладочную информацию
			if(this->isOption(options_t::debug) || (this->logfile != nullptr)){
				// Результат работы функции
				pair <string, string> result;
				// Граммность n-граммы
				string numGram = "OOV";
				// Значение полученного веса
				double prob = 0.0, lprob = this->zero;
				// Если вес не нулевой
				if(weight != 0.0){
					// Запоминаем вес n-граммы
					lprob = weight;
					// Избавляемся от логорифма
					prob = pow(10, weight);
					// Устанавливаем граммность
					numGram = (to_string(gram) + "gram");
				}
				// Формируем информационное сообщение
				result.first = this->alphabet->format(
					"p( %s | %s %s) \t= [%s] %4.8f [ %4.8f ] / %4.8f",
					second.c_str(),
					first.c_str(),
					(bigram ? "..." : ""),
					numGram.c_str(),
					prob, lprob, delim
				);
				// Выполняем округление делителя
				const double value = (ceil((delim * 10000.0) + 0.5) / 10000.0);
				// Если делитель не сходится к единице, выводим сообщение
				if(fabs(value - 1.0) > 0.0009) result.second = this->alphabet->format("word probs for this context sum to %4.8f != 1", delim);
				// Блокируем поток
				this->locker.lock();
				// Добавляем в список отладки
				debugMessages.emplace(pos, move(result));
				// Разблокируем поток
				this->locker.unlock();
			}
		};
		/**
		 * calcFn Функция расчёта перплексии
		 * @param seq последовательность слов для обработки
		 * @return    вес n-граммы
		 */
		auto calcFn = [isAllowUnk, this](const vector <size_t> & seq) noexcept {
			// Результат работы функции
			pair <u_short, double> result = {0, 0.0};
			// Если данные не пустые
			if(!seq.empty()){
				// Получаем нашу последовательность
				vector <size_t> tmp = seq;
				// Если первый элемент является неизвестным словом, удаляем его
				if(!isAllowUnk){
					// Удаляем все первые неизвестные слова
					while(!tmp.empty() && (tmp.front() == size_t(token_t::unk))){
						// Удаляем первый элемент в списке
						tmp.assign(tmp.begin() + 1, tmp.end());
					}
				}
				// Если есть еще смысл искать
				if(!tmp.empty()){
					// Копируем основную карту
					const arpa_t * obj = &this->arpa;
					// Переходим по всей последовательностив
					for(auto & idw : tmp){
						// Ищем нашу n-грамму
						auto it = obj->find(idw);
						// Если n-грамма найдена
						if(it != obj->end()){
							// Запоминаем следующую n-грамму
							obj = &it->second;
							// Запоминаем вес n-граммы
							if(obj->weight != this->zero) result.second = obj->weight;
						// Если n-грамма не найдена
						} else {
							// Если последнее слово последовательности найдено
							if(this->arpa.count(tmp.back()) > 0){
								// Получаем вес последовательности
								const auto & wrs = this->weight(tmp);
								// Получаем грамность
								result.first = wrs.first;
								// Запоминаем полученный вес
								result.second = wrs.second;
								// Если вес получен для юниграммы, выполняем поиск частоты отката
								if(result.first == 1){
									// Получаем список последовательности для извлечения обратной частоты
									tmp.assign(tmp.begin(), tmp.end() - 1);
									// Выполняем расчёт веса n-граммы
									result.second += this->backoff(tmp);
								}
							// Если слово не найдено, устанавливаем -inf
							} else result.second = 0.0;
							// Выходим из цикла
							break;
						}
						// Увеличиваем граммность
						result.first++;
					}
				}
			}
			// Выводим результат
			return result;
		};
		/**
		 * putDebugFn Функция расчёта отладочной информации
		 * @param seq    последовательность слов для обработки
		 * @param gram   граммность n-граммы для которой был произведён расчёт
		 * @param weight полученный вес n-граммы при расчёте
		 * @param pos    позиция n-граммы в контексте
		 */
		auto putDebugFn = [&debugFn, this](const vector <size_t> & seq, const u_short gram, const double weight, const size_t pos) noexcept {
			// Если последовательность передана
			if(!seq.empty() && (this->isOption(options_t::debug) || (this->logfile != nullptr))){
				// Получившийся разделитель
				double delim = 0.0;
				// Получаем нашу последовательность
				vector <size_t> tmp = seq;
				// Получаем количество слов в последовательности
				const size_t count = tmp.size();
				// Выполняем првоерку больше ли переданная последовательность биграммы
				const bool isBigram = (count > 2);
				// Получаем второе слово
				const string & second = this->word(tmp.back()).real();
				// Получаем первое слово
				const string & first = this->word(tmp.at(count - 2)).real();
				// Укорачиваем последовательность до 2-х слов
				if(count > 2) tmp.assign(tmp.begin() + (count - 2), tmp.end());
				// Удаляем последний элемент в списке
				tmp.pop_back();
				// Выполняем расчёт обратной частоты последовательности
				const double backoff = this->backoff(tmp);
				// Переходим по всем словам словаря
				for(auto & value : this->arpa){
					// Если веса у n-граммы нету
					if((value.second.weight != this->zero)){
						// Формируем нашу последовательность
						tmp.push_back(value.first);
						// Получаем частоту последовательности
						auto calc = this->frequency(tmp);
						// Если частота последовательности получена
						if(calc.first != this->zero) delim += exp(calc.first * this->mln10);
						// Если последовательность не существует, считаем частоту иначе
						else delim += exp((this->weight(tmp).second + backoff) * this->mln10);
						// Удаляем последний элемент в списке
						tmp.pop_back();
					}
				}
				// Выводим отладочную информацию
				debugFn(first, second, isBigram, gram, weight, delim, pos);
			}
		};
		// Сбрасываем значение результата
		result.logprob = 0.0;
		/**
		 * runFn Функция запуска расчёта перплексии
		 * @param seq последовательность слов для обработки
		 * @param pos позиция n-граммы в контексте
		 */
		auto runFn = [&result, &calcFn, &putDebugFn](const vector <size_t> & seq, const size_t pos){
			// Выполняем проверку существования граммы
			auto calc = calcFn(seq);
			// Если вес получен
			if(calc.second != 0.0)
				// Увеличиваем общее значение веса
				result.logprob += calc.second;
			// Увеличиваем количество нулевых весов
			else result.zeroprobs++;
			// Выводим отладочную информацию
			putDebugFn(seq, calc.first, calc.second, pos);
		};
		// Обрабатываем первую часть n-грамм
		for(u_short i = 2; i < offset2; i++){
			// Получаем первую часть последовательности
			tmp.assign(seq.begin(), seq.begin() + i);
			// Добавляем в тредпул новое задание на обработку
			runFn(tmp, index);
			// Увеличиваем смещение позиции
			index++;
		}
		// Если есть ещё n-граммы
		if(count >= this->size){
			// Выполняем извлечение данных
			while(offset2 < (count + 1)){
				// Получаем первую часть последовательности
				tmp.assign(seq.begin() + offset1, seq.begin() + offset2);
				// Добавляем в тредпул новое задание на обработку
				runFn(tmp, index);
				// Увеличиваем смещение позиции
				index++;
				// Увеличиваем смещение
				offset1++;
				offset2++;
			}
		}
		// Если неизвестное слово не разрешено
		if(!isAllowUnk){
			// Считаем количество неизвестных слов
			for(auto & idw : seq){
				// Считаем количество неизвестных слов
				if(idw == size_t(token_t::unk)) result.oovs++;
			}
		}
		// Устанавливаем предложение
		result.sentences = 1;
		// Устанавливаем количество слов
		result.words = (seq.size() - 2);
		// Если количество нулевых весов и количество неизвестных слов получено
		if((result.oovs > 0) && (result.zeroprobs > 0)) result.zeroprobs -= result.oovs;
		// Выполняем расчёт перплексии
		const auto ppl = this->pplCalculate(result.logprob, result.words, result.oovs);
		// Усталавниваем полученные значения перплексии
		result.ppl  = ppl.first;
		result.ppl1 = ppl.second;
		// Выводим отладочную информацию
		if(this->isOption(options_t::debug) || (this->logfile != nullptr)){
			// Блокируем поток
			this->locker.lock();
			// Если список отладки сформирован
			if(!debugMessages.empty()){
				// Получаем обрабатываемый текст
				const wstring & text = this->context(seq, true);
				// Выводим сообщение отладки - количество слов
				this->alphabet->log("%s\n", alphabet_t::log_t::info, this->logfile, this->alphabet->convert(text).c_str());
				// Переходим по всему списку отладки
				for(auto & mess : debugMessages){
					// Выводим основное сообщение отладки
					this->alphabet->log("%s", alphabet_t::log_t::info, this->logfile, mess.second.first.c_str());
					// Если второе сообщение существует, выводим и его
					if(!mess.second.second.empty()) this->alphabet->log("%s", alphabet_t::log_t::warning, this->logfile, mess.second.second.c_str());
				}
			}
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
			// Разблокируем поток
			this->locker.unlock();
		}
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
const pair <bool, size_t> anyks::Alm1::check(const vector <size_t> & seq, const bool accurate) const noexcept {
	// Результат работы функции
	pair <bool, size_t> result = {false, 0};
	// Если последовательность передана
	if(!seq.empty() && !this->arpa.empty()){
		// Временная последовательность
		vector <size_t> tmp, sequence;
		// Если последовательность не экранированна
		if((seq.back() == size_t(token_t::finish)) &&
		(seq.front() == size_t(token_t::start))) sequence.assign(seq.begin() + 1, seq.end() - 1);
		else if(seq.back() == size_t(token_t::finish)) sequence.assign(seq.begin() + 1, seq.end());
		else if(seq.front() == size_t(token_t::start)) sequence.assign(seq.begin(), seq.end() - 1);
		else sequence.assign(seq.begin(), seq.end());
		/**
		 * checkFn Прототип функции проверки существования последовательности
		 * @param  список слов последовательности
		 * @return результат проверки
		 */
		function <const pair <bool, size_t> (const vector <size_t> &)> checkFn;
		/**
		 * checkFn Функция проверки существования последовательности
		 * @param seq список слов последовательности
		 * @return    результат проверки
		 */
		checkFn = [&checkFn, accurate, this](const vector <size_t> & seq) noexcept {
			// Регистры слова в последовательности
			pair <bool, size_t> result = {false, 0};
			// Если список последовательностей передан
			if(!seq.empty() && (this->size > 0)){
				// Итератор для подсчета длины n-граммы
				u_short i = 0;
				// Копируем основную карту
				arpa_t * obj = &this->arpa;
				// Переходим по всему объекту
				for(auto & idw : seq){
					// Выполняем поиск нашего слова
					auto it = obj->find(idw);
					// Если слово найдено
					if(it != obj->end()){
						// Получаем блок структуры
						obj = &it->second;
						// Если мы дошли до конца
						result.first = (i == (seq.size() - 1));
						// Если последовательность существует
						if(result.first) result.second = obj->uppers;
					// Выходим из цикла
					} else break;
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
				// Если последовательность не существует
				if(!accurate && !result.first && (seq.size() > 2)){
					// Получаем новую последовательность
					vector <size_t> tmp(seq.begin() + 1, seq.end());
					// Пробуем уменьшить n-грамму
					result = checkFn(tmp);
				}
			}
			// Выводим результат
			return result;
		};
		// Количество переданных последовательностей
		const size_t count = sequence.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = (count > this->size ? this->size : count);
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(sequence.begin() + offset1, sequence.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty()){
				// Получаем регистр слова
				result = checkFn(tmp);
				// Если последовательность не найдена, выходим
				if(!result.first) break;
			// Выходим из цикла
			} else break;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * setBin Метод установки бинарных данных в словарь
 * @param buffer буфер с бинарными данными
 */
void anyks::Alm1::setBin(const vector <char> & buffer) const noexcept {
	// Если буфер передан
	if(!buffer.empty()){
		// Количество слов в последовательности
		u_short count = 0;
		// Смещение в буфере
		size_t offset = 0;
		// Полученные данные последовательности
		vector <seq_t> seq;
		// Получаем данные буфера
		const char * data = buffer.data();
		// Извлекаем количество слов в последовательности
		memcpy(&count, data + offset, sizeof(count));
		// Увеличиваем смещение
		offset += sizeof(count);
		// Если последовательность получена
		if(count > 0){
			// Полученная последовательность
			seq_t sequence;
			// Выделяем память для последовательности
			seq.resize(count);
			// Переходим по всем словам последовательности
			for(u_short i = 0; i < count; i++){
				// Извлекаем данные слова
				memcpy(&sequence, data + offset, sizeof(sequence));
				// Добавляем последовательность в список
				seq[i] = sequence;
				// Увеличиваем смещение
				offset += sizeof(sequence);
			}
			// Если нужно установить исходные данные
			this->set(seq);
		}
	}
}
/**
 * getBin Метод извлечения данных arpa в бинарном виде
 * @param callback функция обратного вызова
 */
void anyks::Alm1::getBin(function <void (const vector <char> &, const u_short)> callback) const noexcept {
	// Если данные загружены
	if(!this->arpa.empty()){
		// Данные последовательности
		seq_t sequence;
		// Индекс обработки статуса
		size_t index = 0;
		// Собранная последовательность
		vector <seq_t> seq;
		// Буфер данных n-граммы
		vector <char> buffer;
		/**
		 * resultFn Метод формирования результата
		 */
		auto resultFn = [&buffer, &seq, &index, &callback, this]() noexcept {
			// Получаем количество n-грамм в списке
			u_short count = seq.size();
			// Получаем бинарные данные количества слов
			const char * bin = reinterpret_cast <const char *> (&count);
			// Добавляем в буфер количество слов
			buffer.insert(buffer.end(), bin, bin + sizeof(count));
			// Переходим по всему списку последовательности
			for(auto & item : seq){
				// Получаем бинарные данные последовательности
				bin = reinterpret_cast <const char *> (&item);
				// Добавляем в буфер бинарные данные последовательности
				buffer.insert(buffer.end(), bin, bin + sizeof(item));
			}
			// Выводим собранную последовательность
			callback(buffer, u_short(index / double(this->arpa.size()) * 100.0));
			// Очищаем полученный буфер n-граммы
			buffer.clear();
		};
		/**
		 * runFn Прототип функции запуска формирования map карты последовательностей
		 * @param словарь для извлечения слова с его параметрами
		 */
		function <void (const arpa_t *)> runFn;
		/**
		 * runFn Функция запуска формирования map карты последовательностей
		 * @param ngrams словарь для извлечения слова с его параметрами
		 */
		runFn = [&](const arpa_t * ngrams) noexcept {
			// Идентификатор полученного и добавленного слова
			size_t idw = idw_t::NIDW, aidw = 0;
			// Получаем объект данных
			const arpa_t * obj = (ngrams != nullptr ? ngrams : &this->arpa);
			// Переходим по всему объекту с данными
			for(auto & item : * obj){
				// Увеличиваем индекс если это юниграмма
				if(ngrams == nullptr) index++;
				// Запоминаем идентификатор добавленного слова
				idw = sequence.idw;
				// Извлекаем основные данные
				sequence.idw     = item.first;
				sequence.ups     = item.second.uppers;
				sequence.weight  = item.second.weight;
				sequence.backoff = item.second.backoff;
				// Формируем последовательность
				seq.push_back(sequence);
				// Если еще есть продолжение граммы
				if(!item.second.empty()) runFn(&item.second);
				// Иначе выводим то что есть
				else if(idw != aidw) {
					// Запоминаем идентификатор добавленного слова
					aidw = idw;
					// Выводим результат
					resultFn();
				}
				// Удаляем последний элемент в списке
				seq.pop_back();
			}
		};
		// Запускаем извлечение данных
		runFn(nullptr);
	// Выводим пустой результат
	} else callback({}, 0);
}
/**
 * sentences Метод генерации предложений
 * @param callback функция обратного вызова
 */
void anyks::Alm1::sentences(function <const bool (const wstring &)> callback) const noexcept {
	// Если языковая модель загружена
	if(!this->arpa.empty()){
		/**
		 * resultFn Функция формирования предложения
		 * @param data собранный список n-грамм
		 * @return     флаг продолжения или завершения работы
		 */
		auto resultFn = [&callback, this](const list <vector <size_t>> & data){
			// Если список последовательностей передан
			if(!data.empty()){
				// Флаг сборки первой n-граммы
				bool isStart = false;
				// Строка результата
				vector <size_t> result = {size_t(token_t::start)};
				// Переходим по всему списку n-грамм
				for(auto & seq : data){
					// Если это первая итерация
					if(!isStart && (isStart = !isStart))
						// Добавляем в список первую n-грамму
						result.insert(result.end(), seq.begin(), seq.end());
					// Если это не первая n-грамма
					else result.push_back(seq.back());
					// Если последняя грамма является концом предложения
					if(seq.back() == size_t(token_t::finish)){
						// Выводим результат
						if(!callback(this->context(result, true))) return false;
						// Удаляем последний элемент в списке
						result.pop_back();
					}
				}
			}
			// Продолжаем обработку
			return true;
		};
		/**
		 * estimateFn Прототип функции оценки собранных последовательностей
		 * @param  список собранной последовательности
		 * @return список собранных последовательностей
		 */
		function <const list <vector <size_t>> (const vector <size_t> &)> estimateFn;
		/**
		 * estimateFn Функция оценки собранных последовательностей
		 * @param seq список собранной последовательности
		 * @return    список собранных последовательностей
		 */
		estimateFn = [&estimateFn, this](const vector <size_t> & seq) noexcept {
			// Результат работы функции
			list <vector <size_t>> result;
			// Если найден - конец предложения
			if(seq.back() == size_t(token_t::finish)) result.push_back(seq);
			// Если последовательность передана
			else if(seq.size() > 1){
				// Получаем новый список последовательности
				vector <size_t> tmp(seq.begin() + 1, seq.end());
				// Если последовательность не пустая
				if(!tmp.empty()){
					// Итератор для подсчета длины n-граммы
					u_short i = 0;
					// Копируем основную карту
					const arpa_t * obj = &this->arpa;
					// Переходим по всему объекту
					for(auto & idw : seq){
						// Выполняем поиск нашего слова
						auto it = obj->find(idw);
						// Если слово найдено
						if(it != obj->end()){
							// Получаем блок структуры
							obj = &it->second;
							// Если мы дошли до конца
							if(i == (seq.size() - 1)){
								// Если это не конец и следующий блок пустой, выходим
								if(!it->second.empty()){
									// Переходим по всему списку следующих n-грамм
									for(auto & item : it->second){
										// Добавляем полученную грамму в список последовательности
										tmp.push_back(item.first);
										// Если это конец предложения
										if(item.first == size_t(token_t::finish)){
											// Если результат пустой, добавляем в него первоначальную последовательность
											if(result.empty()) result.push_back(seq);
											// Добавляем в список полученную последовательность
											result.push_back(tmp);
										// Если это не конец предложения
										} else {
											// Выполняем новый запрос
											const auto & res = estimateFn(tmp);
											// Если результат получен
											if(!res.empty()){
												// Если результат пустой, добавляем в него первоначальную последовательность
												if(result.empty()) result.push_back(seq);
												// Добавляем в список полученную последовательность
												result.insert(result.end(), res.begin(), res.end());
											}
										}
										// Удаляем последнее добавление
										tmp.pop_back();
									}
								}
								// Выходим из цикла
								break;
							// Если это не конец и следующий блок пустой, выходим
							} else if(it->second.empty()) break;
						// Выходим из цикла
						} else break;
						// Если количество n-грамм достигло предела, выходим
						if((++i) > (this->size - 1)) break;
					}
				}
			}
			// Выводим результат
			return result;
		};
		/**
		 * runFn Прототип функции запуска перебора предложения
		 * @param  список собранной последовательности
		 * @param  контекст в котором нужно собирать данные
		 * @return флаг продолжения или завершения работы
		 */
		function <const bool (vector <size_t>, const arpa_t *)> runFn;
		/**
		 * runFn Функция запуска перебора предложения
		 * @param seq     список собранной последовательности
		 * @param context контекст в котором нужно собирать данные
		 * @return        флаг продолжения или завершения работы
		 */
		runFn = [&runFn, &estimateFn, &resultFn](vector <size_t> seq, const arpa_t * context) noexcept {
			// Если данные переданы
			if(!seq.empty() && (context != nullptr)){
				// Переходим по всему списку
				for(auto & item : * context){
					// Добавляем в список n-грамму
					seq.push_back(item.first);
					// Продолжаем сборку данных
					if(!item.second.empty()) runFn(seq, &item.second);
					// Выполняем расчёт собранных данных
					else if(!resultFn(estimateFn(seq))) return false;
					// Удаляем данные в последовательности
					seq.pop_back();
				}
			}
			// Продолжаем работу
			return true;
		};
		// Ищем начало предложения
		auto it = this->arpa.find(size_t(token_t::start));
		// Если начало предложения получено
		if(it != this->arpa.end()){
			// Собранная последовательность
			vector <size_t> seq;
			// Переходим по всем данным
			for(auto & item : it->second){
				// Формируем начало последовательности
				seq.push_back(item.first);
				// Выполняем обработку данных
				if(!runFn(seq, &item.second)) return;
				// Удаляем данные в последовательности
				seq.pop_back();
			}
		}
	}
}
/**
 * getUppers Метод извлечения регистров для каждого слова
 * @param seq  последовательность слов для сборки контекста
 * @param upps список извлечённых последовательностей
 */
void anyks::Alm1::getUppers(const vector <size_t> & seq, vector <size_t> & upps) const noexcept {
	// Если последовательность передана
	if(!seq.empty() && !this->arpa.empty()){
		// Очищаем список регистров
		upps.clear();
		// Временная последовательность
		vector <size_t> tmp, sequence = seq;
		// Если последовательность не экранированна
		const bool isFront = (seq.back() == size_t(token_t::finish));
		const bool isBack =  (seq.front() == size_t(token_t::start));
		// Если флаги не установлены
		if(!isFront) sequence.push_back((size_t) token_t::finish);
		if(!isBack)  sequence.insert(sequence.begin(), (size_t) token_t::start);
		/**
		 * uppersFn Прототип функции извлечения регистров последовательности
		 * @param  список слов последовательности
		 * @return регистры последнего слова последовательности
		 */
		function <const size_t (const vector <size_t> &)> uppersFn;
		/**
		 * uppersFn Функция извлечения регистров последовательности
		 * @param seq список слов последовательности
		 * @return    регистры последнего слова последовательности
		 */
		uppersFn = [&uppersFn, this](const vector <size_t> & seq) noexcept {
			// Регистры слова в последовательности
			size_t result = 0;
			// Если список последовательностей передан
			if(!seq.empty() && (this->size > 0)){
				// Итератор для подсчета длины n-граммы
				u_short i = 0;
				// Копируем основную карту
				arpa_t * obj = &this->arpa;
				// Переходим по всему объекту
				for(auto & idw : seq){
					// Выполняем поиск нашего слова
					auto it = obj->find(idw);
					// Если слово найдено
					if(it != obj->end()){
						// Получаем блок структуры
						obj = &it->second;
						// Если мы дошли до конца
						if(i == (seq.size() - 1)) result = obj->uppers;
					// Выходим из цикла
					} else break;
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
				// Если последовательность не существует
				if((i < seq.size()) && (seq.size() > 2)){
					// Получаем новую последовательность
					vector <size_t> tmp(seq.begin() + 1, seq.end());
					// Пробуем уменьшить n-грамму
					result = uppersFn(tmp);
				}
			}
			// Выводим результат
			return result;
		};
		// Регистр слова
		size_t uppers = 0;
		// Флаг сборки первой итерации
		bool flag = false;
		// Количество переданных последовательностей
		const size_t count = sequence.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = (count > this->size ? this->size : count);
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(sequence.begin() + offset1, sequence.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty()){
				// Получаем регистр слова
				uppers = uppersFn(tmp);
				// Если сборка первой n-граммы не выполнена
				if(!flag && (flag = !flag)){
					// Переходим по всей последовательности
					for(size_t i = (!isFront ? 1 : 0); i < tmp.size(); i++){
						// Если это не последнее слово в списке, добавляем нули
						if(i != (tmp.size() - 1))
							upps.push_back(0);
						// Если же это последнее слово, добавляем регистр
						else upps.push_back(uppers);
					}
				// Если же сборка первой n-граммы уже выполнена
				} else upps.push_back(uppers);
			// Выходим из цикла
			} else break;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
		// Удаляем лишний элемент регистра
		if(!isBack) upps.pop_back();
	}
}
/**
 * find Метод поиска n-грамм в тексте
 * @param text     текст в котором необходимо найти n-граммы
 * @param callback функция обратного вызова
 */
void anyks::Alm1::find(const wstring & text, function <void (const wstring &)> callback) const noexcept {
	// Если слово передано
	if(!text.empty() && !this->arpa.empty()){
		// Идентификатор неизвестного слова
		const size_t uid = (size_t) token_t::unk;
		// Идентификатор начала предложения
		const size_t sid = (size_t) token_t::start;
		// Идентификатор конца предложения
		const size_t fid = (size_t) token_t::finish;
		// Список последовательностей для обучения
		vector <size_t> seq = {sid};
		// Собранная n-грамма для проверки
		vector <wstring> words = {L"<s>"};
		// Кэш списка собранных n-грамм
		unordered_set <wstring> cache = {};
		/**
		 * callbackFn Функция вывода результата
		 * @param words список слов для вывода результата
		 * @param count количество слов для вывода результата
		 */
		auto callbackFn = [&cache, &callback](const vector <wstring> & words, const size_t count){
			// Если список слов передан
			if(!words.empty() && (count > 1)){
				// Получившаяся строка текста
				wstring text = L"";
				// Переходим по всему списку слов
				for(size_t i = 0; i < count; i++){
					// Добавляем в текст слово
					text.append(words.at(i));
					// Добавляем пробел
					text.append(L" ");
				}
				// Удаляем последний пробел
				text.pop_back();
				// Если текст получен
				if(!text.empty() && (cache.count(text) < 1)){
					// Выводим результат
					callback(text);
					// Добавляем собранный результат в кэш
					cache.emplace(text);
				}
			}
		};
		/**
		 * unkFn Функция установки неизвестного слова в последовательность
		 * @return нужно ли остановить сбор последовательности
		 */
		auto unkFn = [&seq, &words, uid, this]() noexcept {
			// Получаем установленное неизвестное слово
			const word_t & word = (this->unknown > 0 ? this->word(this->unknown) : L"");
			// Если неизвестное слово не установлено
			if((this->unknown == 0) || word.empty()){
				// Добавляем неизвестное слово
				seq.push_back(uid);
				// Добавляем в список неизвестное слово
				words.push_back(L"<unk>");
			// Если неизвестное слово установлено
			} else if(!word.empty()) {
				// Добавляем установленное неизвестное слово
				seq.push_back(this->unknown);
				// Добавляем полученное ранее слово
				words.push_back(word.wreal());
			}
		};
		/**
		 * checkFn Прототип функции проверки существования последовательности
		 * @param список слов последовательности
		 * @param список реальных слов в последовательности
		 */
		function <void (const vector <size_t> &, const vector <wstring> &)> checkFn;
		/**
		 * checkFn Функция проверки существования последовательности
		 * @param seq   список слов последовательности
		 * @param words список реальных слов в последовательности
		 */
		checkFn = [&checkFn, &callbackFn, this](const vector <size_t> & seq, const vector <wstring> & words) noexcept {
			// Если список последовательностей передан
			if(!seq.empty() && !words.empty() && (this->size > 0)){
				// Итератор для подсчета длины n-граммы
				u_short i = 0;
				// Результат поиска слова
				bool exist = false;
				// Копируем основную карту
				arpa_t * obj = &this->arpa;
				// Переходим по всему объекту
				for(auto & idw : seq){
					// Выполняем поиск нашего слова
					auto it = obj->find(idw);
					// Если слово найдено
					if(it != obj->end()){
						// Получаем блок структуры
						obj = &it->second;
						// Если мы дошли до конца
						exist = (i == (seq.size() - 1));
					// Выходим из цикла
					} else break;
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
				// Выводим результат
				callbackFn(words, i);
				// Если последовательность не существует
				if(!exist && (seq.size() > 2)){
					// Получаем новую последовательность
					vector <size_t> tmp1(seq.begin() + 1, seq.end());
					// Получаем новую последовательность слов
					vector <wstring> tmp2(words.begin() + 1, words.end());
					// Пробуем уменьшить n-грамму
					checkFn(tmp1, tmp2);
				}
			}
		};
		/**
		 * resFn Функция вывода результата
		 */
		auto resFn = [&]() noexcept {
			// Добавляем в список конец предложения
			seq.push_back(fid);
			// Добавляем конец предложения
			words.push_back(L"</s>");
			/**
			 * Если слова всего два, значит это начало и конец предложения.
			 * Нам же нужны только нормальные n-граммы.
			 */
			if((seq.size() > 2) && (seq.size() == words.size())){
				// Временная последовательность
				vector <size_t> tmp1;
				// Временный список слов
				vector <wstring> tmp2;
				// Количество переданных последовательностей
				const size_t count = seq.size();
				// Определяем смещение в последовательности
				size_t offset1 = 0, offset2 = (count > this->size ? this->size : count);
				// Выполняем извлечение данных
				while(offset2 < (count + 1)){
					// Получаем первую часть последовательности
					tmp1.assign(seq.begin() + offset1, seq.begin() + offset2);
					// Получаем первую часть списка слов
					tmp2.assign(words.begin() + offset1, words.begin() + offset2);
					// Если последовательность получена
					if(!tmp1.empty()) checkFn(tmp1, tmp2);
					// Увеличиваем смещение
					offset1++;
					offset2++;
				}
				// Выводим разделитель предложений
				callback(L"\r\n");
			}
			// Очищаем список последовательностей
			seq.clear();
			// Очищаем список собранных слов
			words.clear();
			// Добавляем в список начало предложения
			seq.push_back(sid);
			// Добавляем начало предложения
			words.push_back(L"<s>");
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
				if(tmp.length() >= MAX_WORD_LENGTH) unkFn();
				// Если слово разрешено
				else if(!tmp.empty()) {
					// Получаем идентификатор слова
					const size_t idw = this->getIdw(tmp);
					// Если это плохое слово, заменяем его на неизвестное
					if((idw == 0) || (idw == idw_t::NIDW) || (this->badwords.count(idw) > 0)) unkFn();
					// Иначе продолжаем дальше
					else {
						// Проверяем является ли строка словом
						const bool isWord = this->event(idw);
						// Если это неизвестное слово
						if((idw == uid) || (isWord && (this->getWord(idw) == nullptr))) unkFn();
						// Иначе добавляем слово
						else if(!isWord || (this->goodwords.count(idw) > 0) || this->alphabet->isAllowed(tmp)){
							// Добавляем идентификатор в список последовательности
							seq.push_back(idw);
							// Добавляем слово в список слов
							words.push_back(word);
						// Отправляем слово как неизвестное
						} else unkFn();
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
 * context Метод сборки текстового контекста из последовательности
 * @param seq  последовательность слов для сборки контекста
 * @param nwrd флаг разрешающий вывод системных токенов
 * @return     собранный текстовый контекст
 */
const wstring anyks::Alm1::context(const vector <size_t> & seq, const bool nwrd) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если последовательность передана
	if(!seq.empty() && !this->arpa.empty()){
		// Временная последовательность
		vector <size_t> tmp, sequence = seq;
		// Если последовательность не экранированна
		if(seq.back() != size_t(token_t::finish)) sequence.push_back((size_t) token_t::finish);
		if(seq.front() != size_t(token_t::start)) sequence.insert(sequence.begin(), (size_t) token_t::start);
		/**
		 * uppersFn Прототип функции извлечения регистров последовательности
		 * @param  список слов последовательности
		 * @return регистры последнего слова последовательности
		 */
		function <const size_t (const vector <size_t> &)> uppersFn;
		/**
		 * uppersFn Функция извлечения регистров последовательности
		 * @param seq список слов последовательности
		 * @return    регистры последнего слова последовательности
		 */
		uppersFn = [&uppersFn, this](const vector <size_t> & seq) noexcept {
			// Регистры слова в последовательности
			size_t result = 0;
			// Если список последовательностей передан
			if(!seq.empty() && (this->size > 0)){
				// Итератор для подсчета длины n-граммы
				u_short i = 0;
				// Копируем основную карту
				arpa_t * obj = &this->arpa;
				// Переходим по всему объекту
				for(auto & idw : seq){
					// Выполняем поиск нашего слова
					auto it = obj->find(idw);
					// Если слово найдено
					if(it != obj->end()){
						// Получаем блок структуры
						obj = &it->second;
						// Если мы дошли до конца
						if(i == (seq.size() - 1)) result = obj->uppers;
					// Выходим из цикла
					} else break;
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
				// Если последовательность не существует
				if((i < seq.size()) && (seq.size() > 2)){
					// Получаем новую последовательность
					vector <size_t> tmp(seq.begin() + 1, seq.end());
					// Пробуем уменьшить n-грамму
					result = uppersFn(tmp);
				}
			}
			// Выводим результат
			return result;
		};
		// Полученное слово последовательности
		word_t word = L"";
		// Регистр слова
		size_t uppers = 0;
		// Флаг сборки первой итерации
		bool flag = false;
		// Количество переданных последовательностей
		const size_t count = sequence.size();
		// Определяем смещение в последовательности
		size_t offset1 = 0, offset2 = (count > this->size ? this->size : count);
		// Выполняем извлечение данных
		while(offset2 < (count + 1)){
			// Получаем первую часть последовательности
			tmp.assign(sequence.begin() + offset1, sequence.begin() + offset2);
			// Если последовательность получена
			if(!tmp.empty()){
				// Получаем регистр слова
				uppers = uppersFn(tmp);
				// Если сборка первой n-граммы не выполнена
				if(!flag && (flag = !flag)){
					// Переходим по всей последовательности
					for(size_t i = 0; i < tmp.size(); i++){
						// Получаем слово
						word = this->word(tmp.at(i));
						// Если разрешено выводить системные токены или это нормальные слова
						if(nwrd || ((word.front() != L'<') && (word.back() != L'>'))){
							// Если это первое слово
							if(i == 1) word.setUppers(1);
							// Если это последнее слово
							else if(i == (tmp.size() - 1)) word.setUppers(uppers);
							// Формируем строку
							result.append(word.wreal());
							// Добавляем разделитель
							result.append(L" ");
						}
					}
				// Если же сборка первой n-граммы уже выполнена
				} else {
					// Получаем слово
					word = this->word(tmp.back());
					// Если разрешено выводить системные токены или это нормальные слова
					if(nwrd || ((word.front() != L'<') && (word.back() != L'>'))){
						// Устанавливаем регистры слова
						word.setUppers(uppers);
						// Формируем строку
						result.append(word.wreal());
						// Добавляем разделитель
						result.append(L" ");
					}
				}
			// Выходим из цикла
			} else break;
			// Увеличиваем смещение
			offset1++;
			offset2++;
		}
		// Если строка получена
		if(!result.empty()) result.pop_back();
	}
	// Выводим результат
	return result;
}
/**
 * ~Alm1 Деструктор
 */
anyks::Alm1::~Alm1() noexcept {
	// Очищаем языковую модель
	this->clear();
}

/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <arpa.hpp>

/**
 * use Метод получения текущего размеры n-граммы
 * @return текущий размер n-граммы
 */
const u_short anyks::Arpa::use() const {
	// Получаем позицию в списке минимальной встречаемости n-граммы
	return ((this->gram > MAXSIZE) || this->isOption(options_t::allGrams) ? 0 : this->gram);
}
/**
 * unigrams Метод получения количества частотных юниграмм
 * @return количество юниграмм
 */
const size_t anyks::Arpa::unigrams() const {
	// Выводим количество юниграмм
	return (this->vocab.size() - 1);
}
/**
 * midw Метод извлечения идентификатора первого слова в n-грамме
 * @param context контекст которому принадлежит слово
 * @return        идентификатор первого слова в n-грамме
 */
const size_t anyks::Arpa::midw(const vocab_t * context) const {
	// Результат работы функции
	size_t result = 0;
	// Если контекст передан
	if(context != nullptr){
		// Извлекаем предшествующую n-грамму
		while(context->father != nullptr){
			// Запоминаем результат
			result = context->idw;
			// Выполняем смещение
			context = context->father;
		}
	}
	// Выводим результат
	return result;
}
/**
 * uppers Метод извлечения регистра слова
 * @param context контекст n-граммы
 * @return        регистр слова
 */
const anyks::pair_t anyks::Arpa::uppers(const vocab_t * context) const {
	// Результат работы функции
	pair_t result = make_pair(0, 0);
	// Если контекст передан
	if(context != nullptr) result = this->uppers(context->uppers, context->oc);
	// Выводим результат
	return result;
}
/**
 * uppers Метод извлечения регистра слова
 * @param  ups список регистров для выбора
 * @param  oc  встречаемость граммы
 * @return     регистр слова
 */
const anyks::pair_t anyks::Arpa::uppers(const std::map <size_t, size_t> & ups, const size_t oc) const {
	// Результат работы функции
	pair_t result = make_pair(0, 0);
	// Если результат не пустой
	if(!ups.empty()){
		// Ищем результат с наибольшим весом
		for(auto & item : ups){
			// Если вес больше предыдущего
			if(item.second > result.second){
				// Запоминаем встречаемость регистра
				result.second = item.second;
				// Запоминаем регистр слова
				result.first = item.first;
			// Если встречаемость регистров совпадает
			} else if((item.second == result.second) && (item.first < result.first)) {
				// Запоминаем результат
				result.first = item.first;
			}
		}
		// Если слово в верхнем регистре встретилось меньше 50% раз, снимаем регистр
		if((oc > 0) && ((result.second / float(oc) * 100.0f) < 50.0f)) result = make_pair(0, 0);
	}
	// Выводим результат
	return result;
}
/**
 * weights Метод определения суммы весов всех дочерних грамм
 * @param ngram контекст для расчёта
 * @return      сумма всех весов дочерних грамм
 */
const float anyks::Arpa::weights(const vocab_t * ngram) const {
	// Результат работы функции
	float result = 0.0f;
	// Если список n-грамм получен
	if((ngram != nullptr) && !ngram->empty()){
		// Переходим по всему списку слов
		for(auto & value : * ngram) result += value.second.weight;
	}
	// Выводим результат
	return result;
}
/**
 * backoff Метод подсчёта обратной частоты n-граммы
 * @param idw     идентификатор текущего слова
 * @param context контекст которому принадлежит слово
 * @param gram    размер n-граммы для отката
 * @return        результат расчёта обратной частоты
 */
const float anyks::Arpa::backoff(const size_t idw, const vocab_t * context, const u_short gram) const {
	// Результат работы функции
	float result = 0.0f;
	// Если это нулевая грамма
	if((gram == 0) || (context == nullptr)){
		// Выполняем поиск юниграммы в словаре
		auto it = this->vocab.find(idw);
		// Если слово найдено
		if(it != this->vocab.end()) result = it->second.weight;
		// Если юниграмма не найдена, ищем перебором
		else {
			// Переходим по всем юниграммам
			for(auto & item : this->vocab){
				// Если юниграмма найдена
				if(item.second.idw == idw){
					// Запоминаем результат
					result = item.second.weight;
					// Выходим из цикла
					break;
				}
			}
		}
	// Иначе продолжаем расчёт
	} else {
		// Индекс перехода
		u_short index = 0;
		// Список последовательности
		vector <size_t> idws = {idw};
		// Извлекаем предшествующую n-грамму
		while((context->father != nullptr) && (index < gram)){
			// Добавляем идентификатор
			idws.insert(idws.begin(), context->idw);
			// Выполняем смещение
			context = context->father;
			// Увеличиваем индекс
			index++;
		}
		// Если список слов собран
		if(!idws.empty()){
			// Получаем текущий контекст
			const vocab_t * context = &this->vocab;
			// Переходим по всему списку идентификаторов
			for(auto & item : idws){
				// Ищем нашу n-грамму
				auto it = context->find(item);
				// Если n-грамма найдена
				if(it != context->end()){
					// Получаем данные n-граммы
					context = &it->second;
					// Получаем вес n-граммы
					result = context->weight;
				}
			}
		// Если данные не получены
		} else result = (this->vocab.at(idw).weight + this->vocab.at(context->idw).backoff);
	}
	// Выводим результат
	return result;
}
/**
 * isOption Метод проверки наличия опции
 * @param option опция для проверки
 * @return       результат проверки
 */
const bool anyks::Arpa::isOption(const options_t option) const {
	// Выполняем проверку наличия опции
	return this->options.test((u_short) option);
}
/**
 * isContext Метод проверки сущестования контекста
 * @param  seq последовательность для проверки
 * @return     результат проверки
 */
const bool anyks::Arpa::isContext(const vector <size_t> & seq) const {
	// Результат работы функции
	bool result = false;
	// Если последовательность передана
	if(!seq.empty()){
		// Запоминаем контекст
		vocab_t * obj = &this->vocab;
		// Переходим по всей последовательности
		for(size_t i = 0; i < seq.size(); i++){
			// Ищем слово в последовательности
			auto it = obj->find(seq[i]);
			// Если слово в последовательности найдено
			result = (it != obj->end());
			// Получаем блок структуры
			if(result) obj = &it->second;
			// Выходим из цикла
			else break;
		}
	}
	// Выводим результат
	return result;
}
/**
 * backoffs Метод для расчёта частот отката
 * @param gram  размер n-граммы для работы
 * @param ngram контекст для расчёта
 * @return      результат расчёта
 */
const bool anyks::Arpa::backoffs(const u_short gram, vocab_t * ngram) const {
	// Результат работы функции
	bool result = true;
	// Если данные граммы переданы
	if(ngram != nullptr){
		// Устанавливаем нуминатор и денуминатор
		double numerator = 0.0, denominator = 0.0;
		// Проверяем включён ли режим отладки
		const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
		/**
		 * Пытаемся расчитать коэффициенты для веса и частоты отката,
		 * чтобы общий вес n-грамм и частоты отката,
		 * можно было распределить в рамках 1.0.
		 */
		if(this->compute(ngram, gram, numerator, denominator)){
			/**
			 * Если юнграммовые зонды оставляют ненулевую вероятность
			 * тогда мы должны присвоить эту массу нулевому порядку (равномерно)
			 * для нулевых слов. Однако формат ARPA BO не поддерживает "нулевой контекст" BOW.
			 * Мы моделируем предполагаемое распределение, равномерно распределяя
			 * оставшуюся массу по всем элементам словаря, которые имеют нулевую вероятность.
			 * ПРИМЕЧАНИЕ. Раньше мы делали это только в том случае, если оставалась пробная масса,
			 * но некоторые программы ngram требуют, чтобы все слова отображались в виде юниграмм,
			 * что достигается путём предоставления им нулевой вероятности.
			 */
			if(gram == 0){
				/**
				 * Если нуминатор расчитать не вышло,
				 * избегаем ложных ненулевых вероятностей юниграмм
				 */
				if(numerator < EPSILON) numerator = 0.0;
				// Выполняем перерасчёт распространения частоты
				this->distribute(numerator);
			// Если это не первая грамма и расчёт не удачный
			} else if((numerator < EPSILON) && (denominator < EPSILON)) ngram->backoff = log(1);
			// Устанавливаем частоту отката
			else ngram->backoff = (log10(numerator) - log10(denominator));
		// Если расчитать не вышло
		} else {
			// Сообщаем что расчёт не удачный
			result = false;
			// Устанавливаем частоту отката
			ngram->backoff = this->zero;
		}
		// Выводим сообщение отладки
		if(debug && (ngram->backoff != this->zero)){
			// Выводим статистику в сообщении
			this->alphabet->log("CONTEXT %s numerator %4.6f denominator %4.6f BOW %4.6f", alphabet_t::log_t::info, this->logfile, this->context(ngram).c_str(), numerator, denominator, ngram->backoff);
		}
	}
	// Выводим результат
	return result;
}
/**
 * checkIdw Метод поиска наличия слова в корпусе
 * @param idw  идентификатор слова для поиска (кроме юниграммы)
 * @param gram размер n-граммы для работы
 * @return     результат проверки
 */
const bool anyks::Arpa::checkIdw(const size_t idw, const u_short gram) const {
	// Результат работы функции
	bool result = false;
	// Если идентификатор и граммность переданы верно
	if((idw > 0) && (gram > 1) && (gram <= this->size) && (this->size > 0)){
		// Список n-грамм для работы
		list <vocab_t *> ngrams;
		// Выполняем извлечение n-грамм
		this->get(gram, &ngrams);
		// Если список n-грамм получен
		if(!ngrams.empty()){
			// Переходим по всему списку полученных n-грамм
			for(auto & item : ngrams){
				// Если в n-грамме есть дочерные граммы
				if(!item->empty() && (item->idw != size_t(token_t::start))){
					// Ищем слово в последовательности
					auto it = item->find(idw);
					// Если последовательность найдена и она пустая, зануляем её
					result = ((it != item->end()) && (it->second.weight != 0.0f));
					// Выходим из цикла
					if(result) break;
				}
			}
		}
		// Если слово не найдено, ищем дальше
		if(!result && ((gram + 1) <= this->size)) result = this->checkIdw(idw, gram + 1);
	}
	// Выводим результат
	return result;
}
/**
 * compute Метод расчёта коэффициентов для распределения весов в рамках 1.0
 * @param ngram       контекст для расчёта
 * @param gram        размер n-граммы для работы
 * @param numerator   разность частот n-грамм
 * @param denominator разность частот отката n-грамм
 */
const bool anyks::Arpa::compute(vocab_t * ngram, const u_short gram, double & numerator, double & denominator) const {
	// Результат работы функции
	bool result = false;
	// Проверяем включён ли режим отладки
	const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
	/**
	 * Частота отката BOW(c) для контекста рассчитывается как,
	 *
	 * BOW(C) = (1 - Sum P(X | C)) /  (1 - Sum p_BO(X | C))
	 *
	 * где Sum - сумма всех слов X с явными вероятностями
	 * в контексте C, P(X | C) это - вероятность, и p_BO(X | C) это -
	 * вероятность для этого слова в соответствии с алгоритмом отката.
	 */
	numerator   = 1.0;
	denominator = 1.0;
	// Переходим по всему слов в контексте
	for(auto & item : * ngram){
		// Если частота n-граммы существует
		if(item.second.weight != 0.0f){
			// Считаем нуминатор
			numerator -= pow(10, item.second.weight);
			// Считаем денуминатор
			if(gram > 0) denominator -= pow(10, this->backoff(item.second.idw, ngram, gram - 1));
		}
	}
	/**
	 * Избегаем некоторых предсказуемых аномалий из-за ошибок округления
	 */
	if(fabs(numerator) < EPSILON)   numerator = 0.0;
	if(fabs(denominator) < EPSILON) denominator = 0.0;
	// Если нуминатор и денуминатор посчитанны неверно
	if((denominator < EPSILON) && (numerator > EPSILON)){
		/**
		 * Распределение частоты отката не имеет никакой вероятности.
		 * Чтобы избежать потери вероятности, масштабируем N-граммные вероятности до 1.
		 */
		if(debug) this->alphabet->log("BOW denominator for context \"%s\" is zero; scaling probabilities to sum to 1", alphabet_t::log_t::warning, this->logfile, this->context(ngram).c_str());
		// Получаем масштаб
		const float scale = - log10(1 - numerator);
		// Переходим по всему слов в контексте
		for(auto & item : * ngram) item.second.weight += scale;
		// Сбрасываем нуминатор и денуминатор
		numerator   = 0.0;
		denominator = 0.0;
		// Запоминаем что всё хорошо
		result = true;
	// Если нуминатор меньше 0
	} else if(numerator < 0.0) {
		// Выводим сообщение об ошибке
		if(debug) this->alphabet->log("BOW numerator for context \"%s\" is %4.6f < 0", alphabet_t::log_t::warning, this->logfile, this->context(ngram).c_str(), numerator);
	// Если денуминатор меньше либо равен 0
	} else if(denominator <= 0.0) {
		// Если нуминатор посчитанн не верно
		if(numerator > EPSILON){
			// Выводим сообщение об ошибке
			if(debug) this->alphabet->log("BOW denominator for context \"%s\" is %4.6f <= 0, numerator is %4.6f", alphabet_t::log_t::warning, this->logfile, this->context(ngram).c_str(), denominator, numerator);
		// Если всё хорошо, обнуляем коэффициенты
		} else {
			// Сбрасываем нуминатор и денуминатор
			numerator   = 0.0;
			denominator = 0.0;
			// Запоминаем что всё хорошо
			result = true;
		}
	// Запоминаем что всё хорошо
	} else result = true;
	// Выводим результат
	return result;
}
/**
 * contextFn Метод получения контекста
 * @param context контекст n-граммы
 * @return        текст контекста n-граммы
 */
const string anyks::Arpa::context(const vocab_t * context) const {
	// Результат работы функции
	string result = "";
	// Если контекстпередан
	if(context != nullptr){
		// Список последовательности
		list <string> words;
		// Извлекаем предшествующую n-грамму
		while(context->father != nullptr){
			// Добавляем идентификатор
			words.push_back(this->word(context->idw, this->uppers(context).first));
			// Выполняем смещение
			context = context->father;
		}
		// Выполняем реверс списка
		words.reverse();
		// Формируем результирующую строку
		for(auto & word : words){
			// Добавляем слово в список
			result.append(word);
			// Добавляем разделитель
			result.append(" ");
		}
		// Удаляем лишние пробелы
		result = this->alphabet->trim(result);
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
const string anyks::Arpa::word(const size_t idw, const size_t ups) const {
	// Результат работы функции
	string result = "";
	// Определяем тип записи
	switch(idw){
		// Если это токен арабского или римского числа
		case u_short(token_t::num): result = "<num>"; break;
		// Если это токен неизвестного слова
		case u_short(token_t::unk): result = "<unk>"; break;
		// Если это токен url адреса
		case u_short(token_t::url): result = "<url>"; break;
		// Если это токен начала предложения
		case u_short(token_t::start): result = "<s>"; break;
		// Если это токен аббревиатуры
		case u_short(token_t::abbr): result = "<abbr>"; break;
		// Если это токен даты
		case u_short(token_t::date): result = "<date>"; break;
		// Если это токен времени
		case u_short(token_t::time): result = "<time>"; break;
		// Если это токен псевдо-числа
		case u_short(token_t::anum): result = "<anum>"; break;
		// Если это символ математической операции
		case u_short(token_t::math): result = "<math>"; break;
		// Если это токен конца предложения
		case u_short(token_t::finish): result = "</s>"; break;
		// Если это спец-символ
		case u_short(token_t::specl): result = "<specl>"; break;
		// Если это токен диапазона чисел
		case u_short(token_t::range): result = "<range>"; break;
		// Если это токен знака пунктуации
		case u_short(token_t::punct): result = "<punct>"; break;
		// Если это токен приблизительного числа
		case u_short(token_t::aprox): result = "<aprox>"; break;
		// Если это токен числового счёта
		case u_short(token_t::score): result = "<score>"; break;
		// Если это токен габаритных размеров
		case u_short(token_t::dimen): result = "<dimen>"; break;
		// Если это токен числовой дроби
		case u_short(token_t::fract): result = "<fract>"; break;
		// Если это токен изоляции
		case u_short(token_t::isolat): result = "<isolat>"; break;
		// Если это нормальное слово
		default: {
			// Получаем слово по его идентификатору
			const word_t * word = this->getWord(idw);
			// Если слово получено
			if(word != nullptr){
				// Если слово нужно вывести с учётом регистра
				if(!this->isOption(options_t::lowerCase)){
					// Получаем данные слова
					word_t tmp = (* word);
					// Устанавливаем регистры слова
					tmp.setUppers(ups);
					// Выводим результат
					result = move(tmp.real());
				// Иначе выводим слово без учёта регистра
				} else result = move(word->str());
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * distribute Метод перерасчёта частоты
 * @param mass разность частот n-грамм
 */
void anyks::Arpa::distribute(const double mass) const {
	// Если контекст существует
	if(!this->vocab.empty()){
		// Количество слов
		double numWords = 0.0;
		// Количество слов с нулевой частотой
		double numZeroProbs = 0.0;
		// Проверяем включён ли режим отладки
		const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
		// Переходим по всему слов в контексте
		for(auto & item : this->vocab){
			// Если это не безсобытийная грамма (Начальная)
			if((item.second.idw != size_t(token_t::start)) &&
			(!this->isOption(options_t::resetUnk) || (item.second.idw != size_t(token_t::unk)))){
				// Считаем количество слов
				numWords++;
				// Если частота нулевая, преобразуем частоту в псевдоноль
				if(item.second.weight == 0.0f) item.second.weight = this->zero;
				// Увеличиваем количество нулевых весов
				if(item.second.weight == this->zero) numZeroProbs++;
			}
		}
		/**
		 * Если слова с нулевыми весами найдены,
		 * расчитываем фейковые веса для всех юниграмм у которых нет веса
		 */
		if(numZeroProbs > 0){
			// Выводим сообщение ошибки
			if(debug) this->alphabet->log("distributing %4.6f left-over probability mass over %4.6f zeroton words", alphabet_t::log_t::warning, this->logfile, mass, numZeroProbs);
			// Получаем множитель
			const double add = (mass / numZeroProbs);
			// Переходим по всему слов в контексте
			for(auto & item : this->vocab){
				// Если это не безсобытийная грамма (Начальная)
				if((item.second.idw != size_t(token_t::start)) &&
				(!this->isOption(options_t::resetUnk) ||
				(item.second.idw != size_t(token_t::unk))) &&
				(item.second.weight == this->zero)){
					// Устанавливаем новый размер
					item.second.weight = log10(add);
				}
			}
		/**
		 * Если слова с нулевыми весами не обнаружены,
		 * распределяем все веса юниграмм так,
		 * чтобы их сумма была примерно равна 1.0
		 */
		} else if(mass != 0.0) {
			// Выводим сообщение ошибки
			if(debug) this->alphabet->log("distributing %4.6f left-over probability mass over all %4.6f words", alphabet_t::log_t::warning, this->logfile, mass, numWords);
			// Получаем множитель
			const double add = (mass / numWords);
			// Переходим по всему слов в контексте
			for(auto & item : this->vocab){
				// Если это не безсобытийная грамма (Начальная)
				if((item.second.idw != size_t(token_t::start)) &&
				(!this->isOption(options_t::resetUnk) ||
				(item.second.idw != size_t(token_t::unk)))){
					// Устанавливаем новый размер
					item.second.weight = log10(pow(10, item.second.weight) + add);
				}
			}
		}
	}
}
/**
 * fixupProbs Метод фиксации неверных частот
 * @param gram размер n-граммы для работы
 *
 * Вставляем вероятности для всех контекстных грамм
 * Формат ARPA заставляет нас иметь вероятность для каждой контекстной граммы.
 * Таким образом, мы создаём его, если необходимо,
 * и устанавливаем такую же вероятность,
 * как и алгоритм отката (чтобы не изменять распределение).
 */
void anyks::Arpa::fixupProbs(const u_short gram) const {
	// Если n-грамма передана
	if(gram <= this->size){
		// Количество фейковых частот
		size_t numFakes = 0;
		// Список n-грамм для работы
		list <vocab_t *> ngrams;
		// Проверяем включён ли режим отладки
		const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
		// Если это нулевая n-грамма
		switch(gram){
			// Если это юниграмма
			case 1: {
				/**
				 * Для контекста abcd нам нужно создать вероятностные записи
				 * P(d | abc), P(c | ab), P(b | a), P(a) если необходимо.
				 * Если что-то из этого будет найдено в таком порядке,
				 * мы можем остановиться, так-как предыдущий проход уже создал оставшиеся.
				 */
				for(auto & value : this->vocab){
					// Если частота n-граммы нормальная, пропускаем
					if(value.second.weight != this->zero) continue;
					/**
					 * Просто сохраняем что-то ненулевое,
					 * чтобы отличить его от внутренних узлов,
					 * которые добавляются неявно.
					 */
					else if((value.second.idw != size_t(token_t::start)) &&
					(!this->isOption(options_t::resetUnk) ||
					(value.second.idw != size_t(token_t::unk)))){
						// Устанавливаем ненулевую частоту юниграммы
						value.second.weight = 1;
					}
				}
				// Переходим по всему списку юниграмм
				for(auto & value : this->vocab){
					// Если вес юниграммы равно 1
					if(value.second.weight == 1){
						// Считаем количество фейковых частот
						numFakes++;
						/**
						 * Примечание: мы не можем объединить два приведённых ниже оператора,
						 * так-как мы создаём запись с нулевой вероятностью,
						 * что помешает backoff() вычислить обратную частоту n-граммы!
						 */
						value.second.weight = this->backoff(value.second.idw, value.second.father, gram - 1);
						// Выводим сообщение об контексте, фейковых n-грамм
						if(debug) this->alphabet->log("faking probability for context %s", alphabet_t::log_t::info, this->logfile, this->context(&value.second).c_str());
					}
				}
			} break;
			// Если это биграмма или больше
			default: {
				// Выполняем извлечение n-грамм
				this->get(gram, &ngrams);
				// Если список n-грамм получен
				if(!ngrams.empty()){
					/**
					 * Для контекста abcd нам нужно создать вероятностные записи
					 * P(d | abc), P(c | ab), P(b | a), P(a) если необходимо.
					 * Если что-то из этого будет найдено в таком порядке,
					 * мы можем остановиться, так-как предыдущий проход уже создал оставшиеся.
					 */
					for(auto & item : ngrams){
						// Если в n-грамме есть дочерные граммы
						if(!item->empty()){
							// Переходим по всему списку юниграмм
							for(auto & value : * item){
								// Если частота n-граммы нормальная, пропускаем
								if(value.second.weight != this->zero) break;
								/**
								 * Просто сохраняем что-то ненулевое,
								 * чтобы отличить его от внутренних узлов,
								 * которые добавляются неявно.
								 */
								else value.second.weight = 1;
							}
							// Переходим по всему списку юниграмм
							for(auto & value : * item){
								// Если вес юниграммы равно 1
								if(value.second.weight == 1){
									// Считаем количество фейковых частот
									numFakes++;
									/**
									 * Примечание: мы не можем объединить два приведённых ниже оператора,
									 * так-как мы создаём запись с нулевой вероятностью,
									 * что помешает backoff() вычислить обратную частоту n-граммы!
									 */
									value.second.weight = this->backoff(value.second.idw, value.second.father, gram - 1);
									// Выводим сообщение об контексте, фейковых n-грамм
									if(debug) this->alphabet->log("faking probability for context %s", alphabet_t::log_t::info, this->logfile, this->context(&value.second).c_str());
								}
							}
						}
					}
				}
			}
		}
		// Если фейковые n-граммы найдены, выводим статистику по ним
		if(debug && (numFakes > 0)){
			// Выводим статистику в сообщении
			this->alphabet->log("inserted %u redundant %u-gram probs", alphabet_t::log_t::info, this->logfile, numFakes, gram);
		}
		// Если есть следующий шаг
		this->fixupProbs(gram + 1);
	}
}
/**
 * Метод получения регистров слов для юниграммы
 * @param idw    идентификатор слова для которого требуется получить регистры
 * @param uppers список регистров слова
 */
void anyks::Arpa::uniUppers(const size_t idw, std::set <size_t> & uppers) const {
	// Очищаем список регистров слов
	if(!uppers.empty()) uppers.clear();
	// Если идентификатор передан
	if((idw > 0) && (idw < idw_t::NIDW)){
		// Список n-грамм для работы
		list <vocab_t *> ngrams;
		// Переходим по всем граммам корпуса
		for(u_short i = 2; i <= this->size; i++){
			// Выполняем запрос грамм
			this->get(i, &ngrams);
			// Если n-граммы получены
			if(!ngrams.empty()){
				// Переходим по всему списку полученных n-грамм
				for(auto & item : ngrams){
					// Переходим по всему списку слов
					for(auto & value : * item){
						// Если идентификатор слова соответствует
						if(this->event(idw) && (value.second.idw == idw) && !value.second.uppers.empty()){
							// Получаем регистры слова
							size_t ups = this->uppers(value.second.uppers, value.second.oc).first;
							// Формируем список регистов данного слова
							if(ups > 0) uppers.emplace(ups);
							// Выходим из цикла
							break;
						}
					}
				}
			}
		}
	}
}
/**
 * get Метод извлечения списка n-грамм указанного размера
 * @param gram   размер n-граммы список грамм которой нужно извлечь
 * @param ngrams указатель на список запрашиваемых n-грамм
 */
void anyks::Arpa::get(const u_short gram, list <vocab_t *> * ngrams) const {
	/**
	 * runFn Прототип функции перехода по граммам
	 * @param размер текущей n-граммы
	 */
	function <void (const u_short)> runFn;
	/**
	 * runFn Функция перехода по граммам
	 * @param gram размер текущей n-граммы
	 */
	runFn = [&](const u_short gram){
		// Параметры следующего шага
		list <vocab_t *> tmp;
		// Если это нулевая n-грамма
		switch(gram){
			// Если это юниграмма
			case 0: {
				// Добавляем в временный список юниграммы
				tmp.push_back(&this->vocab);
				// Добавляем в список юниграмм
				this->ngrams.emplace(gram + 1, tmp);
				// Продолжаем дальше
				runFn(gram + 1);
			} break;
			// Если это биграмма или больше
			default: {
				// Если шаг не пустой
				if(this->ngrams.count(gram) > 0){
					// Переходим по всему словарю
					for(auto & item : this->ngrams.at(gram)){
						// Если в n-грамме есть дочерные граммы
						if(!item->empty()){
							// Переходим по всему списку грамм
							for(auto & value : * item){
								// Если еще есть граммы
								if(!value.second.empty()) tmp.push_back(&value.second);
							}
						}
					}
					// Добавляем в список юниграмм
					if(!tmp.empty()){
						// Если биграмма существует
						this->ngrams.emplace(gram + 1, tmp);
						// Продолжаем дальше
						runFn(gram + 1);
					}
				}
			}
		}
	};
	// Выполняем сбор данных
	if(this->ngrams.count(gram) < 1){
		// Очищаем список грамм
		this->ngrams.clear();
		// Выполняем сбор данных
		runFn(0);
	}
	// Если список n-грамм для извлечения передан
	if(ngrams != nullptr){
		// Очищаем список ранее собранных n-грамм
		ngrams->clear();
		// Если список грамм получен
		auto it = this->ngrams.find(gram);
		// Если список n-грамм получен
		if(it != this->ngrams.end()) (* ngrams) = it->second;
	}
}
/**
 * nodiscount Метод проверки на необходимость расчёта скидки
 * @return результат проверки
 */
const bool anyks::Arpa::nodiscount() const {
	// Выводим результат
	return false;
}
/**
 * prepare Метод разбора встречаемостей
 * @param gram максимальный размер n-граммы
 */
void anyks::Arpa::prepare(const u_short gram) const {
	// Выходим
	return;
}
/**
 * estimate Метод оценки
 * @param  gram максимальный размер n-граммы
 * @return      результат оценки
 */
const bool anyks::Arpa::estimate(const u_short gram) const {
	// Выполняем извлечение списков n-грамм в память
	this->get(gram);
	// Выводим результат
	return true;
}
/**
 * discount Метод расчёта скидки
 * @param  count    встречаемость n-граммы
 * @param  total    общее количество встречаемости всех n-грамм
 * @param  observed количество существующих n-грамм
 * @return          результат расчёта
 */
const double anyks::Arpa::discount(const size_t count, const size_t total, const size_t observed) const {
	// Выводим результат
	return 1.0;
}
/**
 * lowerWeight Метод расчёта интерполяции
 * @param  total     общее количество встречаемости всех n-грамм
 * @param  observed  количество существующих n-грамм
 * @param  min2Vocab встречаемость больше 2-х раз
 * @param  min3Vocab встречаемость больше 3-х раз
 * @return           результат расчёта интерполяции
 */
const double anyks::Arpa::lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const {
	// Выводим результат
	return 0.0;
}
/**
 * stamp Метод вывода штампа файла
 * @return строка с данными штампа
 */
const string anyks::Arpa::stamp() const {
	// Создаем буфер для хранения даты
	char date[80];
	// Заполняем его нулями
	memset(date, 0, sizeof(date));
	// Получаем дату словаря
	const time_t dateBuild = time(nullptr);
	// Получаем структуру локального времени
	struct tm * timeinfo = localtime(&dateBuild);
	// Создаем формат полученного времени
	const string & dateformat = "%m/%d/%Y %H:%M:%S";
	// Копируем в буфер полученную дату и время
	const int length = strftime(date, sizeof(date), dateformat.c_str(), timeinfo);
	// Блок для записи
	return this->alphabet->format(
		"#\n#  version:   %s\n"
		"#  author:    %s\n"
		"#  telegram:  %s\n"
		"#  phone:     %s\n"
		"#  email:     %s\n"
		"#  site:      %s\n"
		"#  built:     %s\n#\n\n",
		ANYKS_VERSION,
		ANYKS_AUTHOR,
		ANYKS_CONTACT,
		ANYKS_PHONE,
		ANYKS_EMAIL,
		ANYKS_SITE,
		date
	);
}
/**
 * event Метод проверки на спец-слово
 * @param idw идентификатор слова для проверки
 * @return    результат проверки
 */
const bool anyks::Arpa::event(const size_t idw) const {
	// Если это не спец-слова, формируем слово
	return (
		(idw != size_t(token_t::num)) &&
		(idw != size_t(token_t::unk)) &&
		(idw != size_t(token_t::url)) &&
		(idw != size_t(token_t::abbr)) &&
		(idw != size_t(token_t::date)) &&
		(idw != size_t(token_t::time)) &&
		(idw != size_t(token_t::anum)) &&
		(idw != size_t(token_t::math)) &&
		(idw != size_t(token_t::specl)) &&
		(idw != size_t(token_t::aprox)) &&
		(idw != size_t(token_t::start)) &&
		(idw != size_t(token_t::range)) &&
		(idw != size_t(token_t::score)) &&
		(idw != size_t(token_t::punct)) &&
		(idw != size_t(token_t::dimen)) &&
		(idw != size_t(token_t::fract)) &&
		(idw != size_t(token_t::isolat)) &&
		(idw != size_t(token_t::finish))
	);
}
/**
 * emplace Метод добавления последовательности в словарь
 * @param seq    список идентификаторов слов которые нужно добавить
 * @param weight вес n-граммы из файла arpa
 */
const bool anyks::Arpa::emplace(const vector <pair_t> & seq, const float weight) const {
	// Результат работы функции
	bool result = false;
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		/**
		 * checkFn Функция проверки существования последовательности
		 * @param seq последовательность для проверки
		 * @return    результат проверки
		 */
		auto checkFn = [this](const vector <pair_t> & seq){
			// Результат работф функции
			bool result = false;
			// Если последовательность передана
			if(!seq.empty()){
				// Копируем основную карту
				vocab_t * obj = &this->vocab;
				// Переходим по всему объекту
				for(auto & item : seq){
					// Запоминаем текущий объект
					const vocab_t * father = obj;
					// Проверяем существует ли данное слово в n-грамме
					auto it = obj->find(item.first);
					// Если слово существует
					if((result = (it != obj->end()))) obj = &it->second;
					// Иначе выходим
					else break;
				}
			}
			// Выводим результат
			return result;
		};
		/**
		 * addFn Функция добавления слова в n-грамму
		 * @param idw идентификатор слова
		 * @param ups регистры слова
		 * @param obj карта последовательности
		 */
		auto addFn = [weight, this](const size_t idw, const size_t ups, vocab_t * obj){
			// Запоминаем текущий объект
			const vocab_t * father = obj;
			// Добавляем юниграмму в словарь
			auto ret = obj->emplace(idw, vocab_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Компенсируем последовательность
			obj->oc = 1;
			// Устанавливаем количество документов
			obj->dc = 1;
			// Запоминаем идентификатор слова
			obj->idw = idw;
			// Запоминаем частоту n-граммы
			obj->weight = weight;
			// Запоминаем родительский объект
			obj->father = father;
			// Запоминаем обратную частоту документа
			obj->backoff = this->zero;
			// Добавляем в список регистр слова
			if(ups > 0) obj->uppers.emplace(ups, 1);
		};
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		vocab_t * obj = &this->vocab;
		// Если это юниграмма
		if((result = (seq.size() == 1) && (obj->count(seq.front().first) < 1))){
			// Добавляем юниграмму в словарь
			addFn(seq.front().first, seq.front().second, obj);
		// Если это не юниграмма
		} else if(seq.size() > 1) {
			// Формируем последовательность меньше на одно слово
			vector <pair_t> tmp(seq.begin() + 1, seq.end());
			// Проверяем включён ли режим отладки
			const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
			// Проверяем существует ли дочерняя n-грамма
			if(checkFn(tmp)){
				// Переходим по всему объекту
				for(auto & item : seq){
					// Запоминаем текущий объект
					const vocab_t * father = obj;
					// Проверяем существует ли данное слово в n-грамме
					auto it = obj->find(item.first);
					// Если это не конечная грамма
					if(i != (seq.size() - 1)){
						// Если слово существует
						if(it != obj->end()) obj = &it->second;
						// Иначе выходим
						else break;
					// Если это конечная грамма и слово не найдено
					} else if((result = (it == obj->end()))) addFn(item.first, item.second, obj);
					// Если количество n-грамм достигло предела, выходим
					if((++i) > (this->size - 1)) break;
				}
			// Если отладка включена
			} else if(debug){
				// Собранный текстовый контекст
				string context = "";
				// Переходим по всем переданным n-граммам
				for(auto & item : seq){
					// Добавляем слово в контекст
					context.append(this->word(item.first, item.second));
					// Добавляем разделитель
					context.append(" ");
				}
				// Удаляем лишние пробелы
				context = this->alphabet->trim(context);
				// Если контекст получен
				if(!context.empty()){
					// Выводим сообщение
					this->alphabet->log("the context [%s] is wrong, emplacement is not possible", alphabet_t::log_t::warning, this->logfile, context.c_str());
				}
			}
		}
		// Очищаем собранный список n-грамм
		if(!this->ngrams.empty()) this->ngrams.clear();
	}
	// Выводим результат
	return result;
}
/**
 * replace Метод замны одной последовательности на другую
 * @param seq1 последовательность которую нужно заменить
 * @param seq2 последовательность на которую нужно зменить
 */
const bool anyks::Arpa::replace(const vector <size_t> & seq1, const vector <pair_t> & seq2) const {
	// Результат работы функции
	bool result = false;
	// Проверяем включён ли режим отладки
	const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
	// Если последовательности переданы
	if(!seq1.empty() && !seq2.empty() && (seq1.size() == seq2.size())){
		// Проверяем на валидность контекста
		if(this->isContext(seq1)){
			// Список n-грамм для работы
			list <vocab_t *> ngrams;
			// Не существующий идентификатор
			const size_t noid = idw_t::NIDW;
			// Список полученных последовательностей
			std::map <size_t, vector <size_t>> variants;
			/**
			 * variantsFn Функция подборки вариантов для проверки
			 * @param seq список последовательности
			 */
			auto variantsFn = [&variants, noid, this](const vector <size_t> & seq){
				// Если последовательность передана
				if(!seq.empty()){
					// Длина последовательности
					const u_short length = seq.size();
					// Получаем максимальный размер последовательности
					const u_short size = (this->size <= length ? this->size - 1 : length);
					// Временная последовательность
					vector <size_t> tmp(seq.begin(), seq.begin() + size);
					// Добавляем в список первый вариант
					variants.emplace(noid, tmp);
					// Если размер n-граммы требует дальнейшей сборки
					if(length > 2){
						// Собираем остальные варианты
						for(u_short i = 0; i < size - 1; i++){
							// Формируем последовательность
							tmp.assign(seq.begin() + (i + 1), seq.begin() + (size + 1));
							// Добавляем в список первый вариант
							variants.emplace(seq[i], tmp);
						}
					}
				}
			};
			/**
			 * checkFn Функция проверки существования последовательности в словаре
			 * @param idw идентификатор родительской граммы
			 * @param seq последовательность для удаления
			 */
			auto checkFn = [&ngrams, noid, this](const size_t idw, const vector <size_t> & seq){
				// Результат работы функции
				bool result = false;
				// Если список n-грамм получен
				if(!ngrams.empty()){
					// Количество найденных грамм
					u_short count = 0;
					// Переходим по всему списку полученных n-грамм
					for(auto & item : ngrams){
						// Если в n-грамме есть дочерные граммы
						if(!item->empty() && (item->idw != size_t(token_t::start))){
							// Ищем первый элемент в списке
							auto it = item->find(seq[0]);
							// Ищем идентификатор
							if((it != item->end()) && !it->second.empty()){
								// Копируем основную карту
								vocab_t * obj = &it->second;
								// Если родительский идентификатор соответствует
								if((idw == obj->idw) || (idw == noid)){
									// Увеличиваем количество найденных грамм
									count++;
									// Переходим по всей последовательности
									for(size_t i = 1; i < seq.size(); i++){
										// Ищем слово в последовательности
										auto it = obj->find(seq[i]);
										// Если слово в последовательности найдено
										if(it != obj->end()){
											// Увеличиваем количество найденных грамм
											count++;
											// Запоминаем что последовательность найдена
											result = true;
											// Получаем блок структуры
											if(!it->second.empty()) obj = &it->second;
										// Если последовательность не найдена
										} else {
											// Запоминаем что последовательность не найдена
											result = false;
											// Выходим из цикла
											break;
										}
									}
								}
							}
						}
						// Если результат найден, выходим
						if(result) break;
					}
					// Если результат найден но количество элементов не совпадает
					if(result && (count != u_short(seq.size()))) result = false;
				}
				// Выводим результат
				return result;
			};
			/**
			 * replaceFn Прототип функции замены данных последовательности
			 * @param последовательность которую нужно заменить
			 * @param последовательность на которую нужно зменить
			 * @param необходимо продолжить работу в следующих граммах
			 * @param объект n-граммы для работы
			 */
			function <void (const vector <size_t> &, const vector <pair_t> &, const bool, vocab_t *)> replaceFn;
			/**
			 * replaceFn Функция замены данных последовательности
			 * @param seq1    последовательность которую нужно заменить
			 * @param seq2    последовательность на которую нужно зменить
			 * @param context объект n-граммы для работы
			 * @param mode    необходимо продолжить работу в следующих граммах
			 */
			replaceFn = [&replaceFn, this](const vector <size_t> & seq1, const vector <pair_t> & seq2, const bool mode, vocab_t * context){
				// Запоминаем контекст
				vocab_t * obj = context;
				// Переходим по всей последовательности
				for(size_t i = 0; i < seq1.size(); i++){
					// Ищем слово в последовательности
					auto it = obj->find(seq1[i]);
					// Если слово в последовательности найдено
					if(it != obj->end()){
						// Если нужно произвести замену
						it->second.idw = seq2[i].first;
						// Если регистры слова переданы, считаем их
						if(seq2[i].second > 0){
							// Получаем данные текущих регистров
							auto uppers = this->uppers(it->second.uppers);
							// Очищаем текущий список регистров
							it->second.uppers.clear();
							// Устанавливаем новые значения регистров
							it->second.uppers.emplace(seq2[i].second, (it->second.oc > 1 ? ceil(it->second.oc / float(2)) : (uppers.second > 0 ? uppers.second : 1)));
						}
						// Получаем блок структуры
						obj = &it->second;
					// Выходим из цикла
					} else break;
				}
				// Если последовательность не юниграмма
				if(mode && (seq1.size() > 1)){
					// Смещаем первую последовательность
					vector <size_t> tmp1(seq1.begin() + 1, seq1.end());
					// Смещаем вторую последовательность
					vector <pair_t> tmp2(seq2.begin() + 1, seq2.end());
					// Продолжаем дальше
					replaceFn(tmp1, tmp2, mode, context);
				}
			};
			/**
			 * replaceStart Функция заменны n-грамм в начале предложения
			 */
			auto replaceStart = [&replaceFn, this](const vector <size_t> & seq1, const vector <pair_t> & seq2){
				// Если последовательности переданы
				if(!seq1.empty() && !seq2.empty()){
					// Извлекаем n-грамму начала слова
					auto it = this->vocab.find((size_t) token_t::start);
					// Если n-грамма найдена
					if(it != this->vocab.end()){
						// Выполняем замену основных n-грамм
						replaceFn(seq1, seq2, false, &it->second);
					}
				}
			};
			/**
			 * setChildren Прототип функции вставки дочерних n-грамм
			 * @param последовательность которую нужно заменить
			 * @param последовательность на которую нужно зменить
			 */
			function <void (const vector <size_t> &, const vector <pair_t> &)> setChildren;
			/**
			 * setChildren Функция вставки дочерних n-грамм
			 * @param seq1 последовательность которую нужно заменить
			 * @param seq2 последовательность на которую нужно зменить
			 */
			setChildren = [&setChildren, this](const vector <size_t> & seq1, const vector <pair_t> & seq2){
				// Если последовательности переданы
				if(!seq1.empty() && !seq2.empty() && (seq1.size() > 1)){
					// Смещаем первую последовательность
					vector <size_t> tmp1(seq1.begin() + 1, seq1.end());
					// Смещаем вторую последовательность
					vector <pair_t> tmp2(seq2.begin() + 1, seq2.end());
					// Запоминаем контекст
					vocab_t * obj = &this->vocab;
					// Переходим по всей последовательности
					for(size_t i = 0; i < tmp1.size(); i++){
						// Ищем слово в последовательности
						auto it = obj->find(tmp1[i]);
						// Если слово в последовательности найдено
						if(it != obj->end()){
							// Выполняем вставку n-граммы
							if(it->second.idw != tmp2[i].first) this->set(tmp2, it->second.weight, it->second.backoff);
							// Получаем блок структуры
							obj = &it->second;
						// Выходим из цикла
						} else break;
					}
					// Если размер граммы больше 1
					if(tmp1.size() > 1) setChildren(tmp1, tmp2);
				}
			};
			/**
			 * repFakeFn Функция фейковой замены самой длинной n-граммы
			 * @param seq1 последовательность которую нужно заменить
			 * @param seq2 последовательность на которую нужно зменить
			 */
			auto repFakeFn = [&setChildren, this](const vector <size_t> & seq1, const vector <pair_t> & seq2){
				// Если последовательности переданы
				if(!seq1.empty() && !seq2.empty()){
					// Временная последовательность
					vector <pair_t> tmp;
					// Запоминаем контекст
					vocab_t * obj = &this->vocab;
					// Переходим по всей последовательности
					for(size_t i = 0; i < seq1.size(); i++){
						// Ищем слово в последовательности
						auto it = obj->find(seq1[i]);
						// Если слово в последовательности найдено
						if(it != obj->end()){
							// Выполняем вставку n-граммы
							if(it->second.idw != seq2[i].first){
								// Формируем временную последовательность
								tmp.assign(seq2.begin(), seq2.begin() + (i + 1));
								// Вставляем новую последовательность
								this->set(tmp, it->second.weight, it->second.backoff);
								// Если это последняя n-грамма то ломаем её
								if(i == (seq1.size() - 1)) it->second.weight = 0.0f;
							}
							// Получаем блок структуры
							obj = &it->second;
						// Выходим из цикла
						} else break;
					}
					// Если размер граммы больше 1
					if(seq1.size() > 1){
						/*
						 * Заполняем юниграммы
						 */
						// Переходим по всей последовательности
						for(size_t i = 0; i < seq1.size(); i++){
							// Если такой юниграммы не существует
							if(this->vocab.count(seq2[i].first) < 1){
								// Ищем слово в последовательности
								auto it = this->vocab.find(seq1[i]);
								// Если слово в последовательности найдено
								if(it != obj->end()){
									// Вставляем новую последовательность
									this->set({seq2[i]}, it->second.weight, it->second.backoff);
								// Выходим из цикла
								} else break;
							}
						}
						// Вставляем дочерние n-граммы
						setChildren(seq1, seq2);
					}
				}
			};
			// Выполняем сборку вариантов для проверки
			if(seq1.size() > 1) variantsFn(seq1);
			// Если варианты собраны
			if(!variants.empty()){
				// Выполняем извлечение n-грамм
				this->get(2, &ngrams);
				// Если список n-грамм получен
				if(!ngrams.empty()){
					// Переходим по всему списку вариантов
					for(auto & item : variants){
						// Выполняем проверку
						result = !checkFn(item.first, item.second);
						// Если вариант найден, выходим
						if(!result) break;
					}
					// Если n-грамму можно безопасно заменить
					if(result){
						// Выполняем замену основных n-грамм
						replaceFn(seq1, seq2, true, &this->vocab);
						// Выполняем замену n-грамм начала предложений
						replaceStart(seq1, seq2);
						// Выполняем вставку дочерних n-грамм
						setChildren(seq1, seq2);
					// Если замена не разрешена но это максимальная n-грамма
					} else if((result = (u_short(seq1.size()) == this->size))) repFakeFn(seq1, seq2);
					// Выводим сообщение что n-грамму заменить нельзя
					else if(debug) {
						// Описание переданного контекста
						string context = "";
						// Запоминаем контекст
						vocab_t * obj = &this->vocab;
						// Переходим по всей последовательности
						for(size_t i = 0; i < seq1.size(); i++){
							// Ищем слово в последовательности
							auto it = obj->find(seq1[i]);
							// Если слово в последовательности найдено
							if(it != obj->end()){
								// Если это последнее слово
								if(i == (seq1.size() - 1)) context = this->context(&it->second);
								// Получаем блок структуры
								obj = &it->second;
							// Выходим из цикла
							} else {
								// Стираем весь собранный контекст
								context.clear();
								// Выходим из цикла
								break;
							}
						}
						// Если описание контекста получено
						if(!context.empty()) this->alphabet->log("different context depends on the context [%s]", alphabet_t::log_t::warning, this->logfile, context.c_str());
					}
				}
			// Если это юниграмма
			} else if(seq1.size() == 1) {
				// Извлекаем наше слово
				auto it = this->vocab.find(seq1[0]);
				// Если n-грамма найдена и она единственная
				if(it != this->vocab.end()){
					// Флаг проверки на валидность слова
					result = it->second.empty();
					// Если слово пустое или там только конец предложения
					if(!result && (it->second.size() == 1)){
						// Если это конец предложения
						result = (it->second.count((size_t) token_t::finish) > 0);
					}
					// Если всё хорошо, выполняем замену
					if(result){
						// Выполняем замену основных n-грамм
						replaceFn(seq1, seq2, false, &this->vocab);
						// Выполняем замену n-грамм начала предложений
						replaceStart(seq1, seq2);
					// Выводим сообщение что юниграмму заменить нельзя
					} else if(debug) this->alphabet->log("different unigram depends on the context [%s]", alphabet_t::log_t::warning, this->logfile, this->context(&it->second).c_str());
				}
			}
			// Очищаем собранный список n-грамм
			if(!this->ngrams.empty()) this->ngrams.clear();
		// Если контекст не валидный
		} else if(debug) {
			// Описание переданного контекста
			string context = "";
			// Переходим по всей последовательности
			for(size_t i = 0; i < seq1.size(); i++){
				// Получаем значение слова
				context.append(this->word(seq1[i], seq2[i].second));
				// Добавляем пробел
				if(i < (seq1.size() - 1)) context.append(" ");
			}
			// Если описание контекста получено
			if(!context.empty()) this->alphabet->log("context [%s] does not exist", alphabet_t::log_t::warning, this->logfile, this->alphabet->trim(context).c_str());
		}
	// Сообщаем что контекст поломан
	} else if(debug) this->alphabet->log("%s", alphabet_t::log_t::warning, this->logfile, "context is broken");
	// Выводим результат
	return result;
}
/**
 * count Количество грамм для указанного размера n-граммы
 * @param  gram размер n-граммы для которой нужно получить количество грамм
 * @param  real выводить реальное количество грамм
 * @return      количество грамм для указанного размера n-граммы
 */
const size_t anyks::Arpa::count(const u_short gram, const bool real) const {
	// Результат работы функции
	size_t result = 0;
	// Если это юниграмма
	if(!real && (gram == 1)){
		// Список регистров слова
		std::set <size_t> uppers;
		// Переходим по всему списку юниграмм
		for(auto & value : this->vocab){
			// Если n-грамма имеет вес
			if(value.second.weight != 0.0f){
				// Считаем только одно слово
				result++;
				// Если это нормальное слово
				if(this->event(value.second.idw) && !this->isOption(options_t::lowerCase)){
					// Получаем список регистров слова
					this->uniUppers(value.second.idw, uppers);
					// Если список регистров получен
					if(!uppers.empty()) result += uppers.size();
				}
			}
		}
	// Если это n-грамма
	} else {
		// Список n-грамм для работы
		list <vocab_t *> ngrams;
		// Выполняем извлечение n-грамм
		this->get(gram, &ngrams);
		// Если список n-грамм получен
		if(!ngrams.empty()){
			// Переходим по всему списку полученных n-грамм
			for(auto & item : ngrams){
				// Переходим по всему списку слов
				for(auto & value : * item){
					// Если n-грамма имеет вес
					if(real || (value.second.weight != 0.0f)) result++;
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * clear Метод очистки словаря
 */
void anyks::Arpa::clear(){
	// Сбрасываем обработанные граммы
	this->gram = 1;
	// Очищаем словарь n-грамм
	this->vocab.clear();
	// Очищаем список собранных грамм
	this->ngrams.clear();
	// Сбрасываем параметры расчёта
	this->param = param_t();
}
/**
 * setWordMethod Метод установки функции получения слова
 * @param word функция получения слова
 */
void anyks::Arpa::setWordMethod(words_t word){
	// Устанавливаем функцию получения слова
	this->getWord = word;
}
/**
 * setSize Метод установки максимального размера n-граммы
 * @param size максимальный размер n-граммы
 */
void anyks::Arpa::setSize(const u_short size){
	// Устанавливаем максимальный размер n-граммы
	if((size >= 1) && (size <= MAXSIZE)) this->size = size;
}
/**
 * setLogfile Метод установка файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::Arpa::setLogfile(const char * logfile){
	// Устанавливаем адрес log файла
	this->logfile = logfile;
}
/**
 * setOption Метод установки опций модуля
 * @param option опция для установки
 */
void anyks::Arpa::setOption(const options_t option){
	// Устанавливаем опции
	this->options.set((u_short) option);
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита
 */
void anyks::Arpa::setAlphabet(const alphabet_t * alphabet){
	// Устанавливаем алфавит
	this->alphabet = alphabet;
}
/**
 * del Метод удаления последовательности
 * @param seq последовательность слов для удаления
 */
void anyks::Arpa::del(const vector <size_t> & seq) const {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Индекс смещения
		u_short index = 0;
		// Копируем основную карту
		vocab_t * obj = &this->vocab;
		// Проверяем включён ли режим отладки
		const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
		// Выполняем удаление n-граммы
		while(index < seq.size()){
			// Ищем слово в последовательности
			auto it = obj->find(seq[index]);
			// Если слово найдено
			if(it != obj->end()){
				// Зануляем грамму
				if(index > 0) it->second.weight = 0.0f;
				// Если последовательность еще есть
				if(!it->second.empty() && (index < (seq.size() - 1))){
					// Увеличиваем индекс
					index++;
					// Углубляемся дальше
					obj = &it->second;
				// Выходим из цикла
				} else break;
			// Если такое слово не существует
			} else {
				// Собранный текстовый контекст
				string context = "";
				// Переходим по всем переданным n-граммам
				for(auto & idw : seq){
					// Добавляем слово в контекст
					context.append(this->word(idw));
					// Добавляем разделитель
					context.append(" ");
				}
				// Удаляем лишние пробелы
				context = this->alphabet->trim(context);
				// Если контекст получен
				if(!context.empty()){
					// Выводим сообщение
					this->alphabet->log("the context [%s] is wrong, delete is not possible", alphabet_t::log_t::warning, this->logfile, context.c_str());
				}
				// Выходим из цикла
				break;
			}
		}
		// Очищаем собранный список n-грамм
		if(!this->ngrams.empty()) this->ngrams.clear();
	}
}
/**
 * inc Метод увеличения последовательности в словаре
 * @param seq   список идентификаторов слов которые нужно увеличить
 * @param value значение на которое нужно увеличить вес n-граммы
 */
void anyks::Arpa::inc(const vector <pair_t> & seq, const float value) const {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Индекс смещения
		u_short index = 0;
		// Копируем основную карту
		vocab_t * obj = &this->vocab;
		// Проверяем включён ли режим отладки
		const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
		// Выполняем увеличение частоты n-граммы
		while(index < seq.size()){
			// Ищем слово в последовательности
			auto it = obj->find(seq[index].first);
			// Если слово найдено
			if(it != obj->end()){
				// Увеличиваем вес n-граммы
				if(index == (seq.size() - 1)){
					// Увеличиваем частоту
					it->second.weight += value;
					// Добавляем в список регистр слова
					if(seq[index].second > 0) obj->uppers.emplace(seq[index].second, 1);
				}
				// Если последовательность еще есть
				if(!it->second.empty() && (index < (seq.size() - 1))){
					// Увеличиваем индекс
					index++;
					// Углубляемся дальше
					obj = &it->second;
				// Выходим из цикла
				} else break;
			// Если такое слово не существует
			} else {
				// Собранный текстовый контекст
				string context = "";
				// Переходим по всем переданным n-граммам
				for(auto & item : seq){
					// Добавляем слово в контекст
					context.append(this->word(item.first, item.second));
					// Добавляем разделитель
					context.append(" ");
				}
				// Удаляем лишние пробелы
				context = this->alphabet->trim(context);
				// Если контекст получен
				if(!context.empty()){
					// Выводим сообщение
					this->alphabet->log("the context [%s] is wrong, increment is not possible", alphabet_t::log_t::warning, this->logfile, context.c_str());
				}
				// Выходим из цикла
				break;
			}
		}
	}
}
/**
 * load Метод загрузки бинарных данных в словарь
 * @param buffer буфер с бинарными данными
 * @param arpa   нужно добавить только данные arpa
 */
void anyks::Arpa::load(const vector <char> & buffer, const bool arpa) const {
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
			if(!arpa) this->set(seq);
			// Иначе нам нужно загрузить arpa
			else {
				// Последовательность для установки
				vector <pair_t> tmp;
				// Переходим по всему списку последовательности
				for(auto & item : seq){
					// Формируем последовательность
					tmp.emplace_back((size_t) item.idw, (size_t) item.ups);
					// Добавляем данные arpa в словарь
					this->set(tmp, item.weight, item.backoff);
				}
			}
		}
		// Очищаем собранный список n-грамм
		if(!this->ngrams.empty()) this->ngrams.clear();
	}
}
/**
 * set Метод установки последовательности в словарь
 * @param seq последовательность слов для установки
 */
void anyks::Arpa::set(const vector <seq_t> & seq) const {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		vocab_t * obj = &this->vocab;
		// Переходим по всему объекту
		for(auto & item : seq){
			// Запоминаем текущий объект
			const vocab_t * father = obj;
			// Добавляем слово в словарь
			auto ret = obj->emplace(item.idw, vocab_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Запоминаем количество документов
			obj->dc = item.dc;
			// Запоминаем встречаемость так-как она есть
			obj->oc = item.oc;
			// Запоминаем идентификатор слова
			obj->idw = item.idw;
			// Запоминаем родительский объект
			obj->father = father;
			// Если регистры слова переданы, считаем их
			if(item.ups > 0){
				// Если такого регистра еще нет в списке
				if(obj->uppers.count(item.ups) == 0)
					// Добавляем в список регистр слова
					obj->uppers.emplace(item.ups, 1);
				// Иначе увеличиваем существующий регистр слова
				else obj->uppers.at(item.ups)++;
			}
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
		// Очищаем собранный список n-грамм
		if(!this->ngrams.empty()) this->ngrams.clear();
	}
}
/**
 * set Метод установки последовательности в словарь
 * @param seq список идентификаторов слов которые нужно добавить
 * @param oc  встречаемость последовательности
 * @param dc  количество документов где встретилась последовательность
 */
void anyks::Arpa::set(const vector <pair_t> & seq, const size_t oc, const size_t dc) const {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		vocab_t * obj = &this->vocab;
		// Переходим по всему объекту
		for(auto & item : seq){
			// Запоминаем текущий объект
			const vocab_t * father = obj;
			// Добавляем слово в словарь
			auto ret = obj->emplace(item.first, vocab_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Запоминаем родительский объект
			obj->father = father;
			// Запоминаем идентификатор слова
			obj->idw = item.first;
			// Если мы дошли до конца
			if(i == (seq.size() - 1)){
				// Если это не псевдо-юниграммы, выполняем инкремент
				if((obj->oc != oc) || (obj->dc != dc) || (seq.size() > 1)){
					// Запоминаем количество документов
					obj->dc += dc;
					// Запоминаем встречаемость так-как она есть
					obj->oc += oc;
				}
			}
			// Если регистры слова переданы, считаем их
			if(item.second > 0){
				// Если такого регистра еще нет в списке
				if(obj->uppers.count(item.second) == 0)
					// Добавляем в список регистр слова
					obj->uppers.emplace(item.second, 1);
				// Иначе увеличиваем существующий регистр слова
				else obj->uppers.at(item.second)++;
			}
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
		// Очищаем собранный список n-грамм
		if(!this->ngrams.empty()) this->ngrams.clear();
	}
}
/**
 * set Метод установки последовательности в словарь
 * @param seq     список идентификаторов слов которые нужно добавить
 * @param weight  вес n-граммы из файла arpa
 * @param backoff обратная частота документа из файла arpa
 */
void anyks::Arpa::set(const vector <pair_t> & seq, const float weight, const float backoff) const {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		vocab_t * obj = &this->vocab;
		// Переходим по всему объекту
		for(auto & item : seq){
			// Запоминаем текущий объект
			const vocab_t * father = obj;
			// Добавляем слово в словарь
			auto ret = obj->emplace(item.first, vocab_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Запоминаем родительский объект
			obj->father = father;
			// Запоминаем идентификатор слова
			obj->idw = item.first;
			// Если нужно инкрементировать последовательность
			obj->oc++;
			// Если мы дошли до конца
			if(i == (seq.size() - 1)){
				// Устанавливаем количество документов
				obj->dc = 1;
				// Запоминаем обратную частоту документа
				obj->backoff = (backoff == 0.0f ? this->zero : backoff);
				// Запоминаем частоту n-граммы
				obj->weight = ((weight == 0.0f) || (fabs(round(weight)) >= 99.0f) ? this->zero : weight);
			}
			// Если регистры слова переданы, считаем их
			if(item.second > 0){
				// Если такого регистра еще нет в списке
				if(obj->uppers.count(item.second) == 0)
					// Добавляем в список регистр слова
					obj->uppers.emplace(item.second, 1);
				// Иначе увеличиваем существующий регистр слова
				else obj->uppers.at(item.second)++;
			}
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
		// Очищаем собранный список n-грамм
		if(!this->ngrams.empty()) this->ngrams.clear();
	}
}
/**
 * add Метод добавления последовательности в словарь
 * @param seq список идентификаторов слов которые нужно добавить
 * @param idd идентификатор документа в котором получена n-грамма
 */
void anyks::Arpa::add(const vector <pair_t> & seq, const size_t idd) const {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		vocab_t * obj = &this->vocab;
		// Получаем временный список последовательности
		vector <pair_t> tmp(seq.cbegin(), seq.cend());
		// Переходим по всему объекту
		for(auto & seq : tmp){
			// Запоминаем текущий объект
			const vocab_t * father = obj;
			// Добавляем слово в словарь
			auto ret = obj->emplace(seq.first, vocab_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Запоминаем идентификатор слова
			obj->idw = seq.first;
			// Запоминаем родительский объект
			obj->father = father;
			// Если нужно инкрементировать последовательность
			obj->oc++;
			// Если идентификаторы документов не совпадают
			if(obj->idd != idd){
				// Увеличиваем количество документов
				obj->dc++;
				// Запоминаем идентификатор документа
				obj->idd = idd;
			}
			// Если регистры слова переданы, считаем их
			if(seq.second > 0){
				// Если такого регистра еще нет в списке
				if(obj->uppers.count(seq.second) == 0)
					// Добавляем в список регистр слова
					obj->uppers.emplace(seq.second, 1);
				// Иначе увеличиваем существующий регистр слова
				else obj->uppers.at(seq.second)++;
			}
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
		// Удаляем первый идентификатор
		tmp.erase(tmp.begin());
		// Если список еще не пустой, продолжаем дальше
		if(!tmp.empty()) this->add(tmp, idd);
		// Очищаем собранный список n-грамм
		if(!this->ngrams.empty()) this->ngrams.clear();
	}
}
/**
 * add Метод добавления последовательности в словарь
 * @param seq     список идентификаторов слов которые нужно добавить
 * @param weight  вес n-граммы из файла arpa
 * @param backoff обратная частота документа из файла arpa
 */
void anyks::Arpa::add(const vector <pair_t> & seq, const float weight, const float backoff) const {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		vocab_t * obj = &this->vocab;
		// Переходим по всему объекту
		for(auto & item : seq){
			// Запоминаем текущий объект
			const vocab_t * father = obj;
			// Добавляем слово в словарь
			auto ret = obj->emplace(item.first, vocab_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Запоминаем родительский объект
			obj->father = father;
			// Запоминаем идентификатор слова
			obj->idw = item.first;
			// Если нужно инкрементировать последовательность
			obj->oc++;
			// Если мы дошли до конца
			if(i == (seq.size() - 1)){
				// Устанавливаем количество документов
				obj->dc = 1;
				// Вычисляем временное значение частоты отката
				float backoffTmp = (backoff == 0.0f ? this->zero : backoff);
				// Вычисляем временное значение частоты
				float weightTmp = ((weight == 0.0f) || (fabs(round(weight)) >= 99.0f) ? this->zero : weight);
				// Нужно ли перерасчитывать вес n-граммы
				bool noWeight = ((obj->idw != size_t(token_t::start)) && ((seq.size() > 1) || !this->isOption(options_t::resetUnk) || (obj->idw != size_t(token_t::unk))));
				// Если частота не установлена
				if(obj->weight == 0.0f) obj->weight = weightTmp;
				// Увеличиваем основную частоту
				else if(noWeight) obj->weight = log10(pow(10, obj->weight) + pow(10, weightTmp));
				// Если частота отката не установлена
				if(obj->backoff == 0.0f) obj->backoff = backoffTmp;
				// Увеличиваем обратную частоту документа
				else if(noWeight) obj->backoff = log10(pow(10, obj->backoff) + pow(10, backoffTmp));
			}
			// Если регистры слова переданы, считаем их
			if(item.second > 0){
				// Если такого регистра еще нет в списке
				if(obj->uppers.count(item.second) == 0)
					// Добавляем в список регистр слова
					obj->uppers.emplace(item.second, 1);
				// Иначе увеличиваем существующий регистр слова
				else obj->uppers.at(item.second)++;
			}
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
		// Очищаем собранный список n-грамм
		if(!this->ngrams.empty()) this->ngrams.clear();
	}
}
/**
 * add Метод добавления последовательности в словарь
 * @param seq  последовательность слов для установки
 * @param idd  идентификатор документа в котором получена n-грамма
 * @param rest необходимо сделать переоценку встречаемости (необходимо если объединяются две карты с разными размерами n-грамм)
 */
void anyks::Arpa::add(const vector <seq_t> & seq, const size_t idd, const bool rest) const {
	// Если список последовательностей передан
	if(!seq.empty() && (this->size > 0)){
		// Итератор для подсчета длины n-граммы
		u_short i = 0;
		// Копируем основную карту
		vocab_t * obj = &this->vocab;
		// Получаем временный список последовательности
		vector <seq_t> tmp(seq.cbegin(), seq.cend());
		// Переходим по всему объекту
		for(auto & seq : tmp){
			// Запоминаем текущий объект
			const vocab_t * father = obj;
			// Добавляем слово в словарь
			auto ret = obj->emplace((size_t) seq.idw, vocab_t());
			// Получаем блок структуры
			obj = &ret.first->second;
			// Запоминаем идентификатор слова
			obj->idw = seq.idw;
			// Запоминаем родительский объект
			obj->father = father;
			// Если нужно выполнить переоценку
			if(rest) obj->oc++;
			// Если идентификаторы документов не совпадают
			else if(obj->idd != idd){
				// Запоминаем идентификатор документа
				obj->idd = idd;
				// Добавляем последовательность
				obj->oc += seq.oc;
				// Увеличиваем количество документов
				obj->dc += seq.dc;
			}
			// Добавляем в список регистр слова
			if(!rest && (seq.ups > 0)){
				// Если такого регистра еще нет в списке
				if(obj->uppers.count(seq.ups) == 0)
					// Добавляем в список регистр слова
					obj->uppers.emplace((size_t) seq.ups, 1);
				// Иначе увеличиваем существующий регистр слова
				else obj->uppers.at(seq.ups)++;
			}
			// Если количество n-грамм достигло предела, выходим
			if((++i) > (this->size - 1)) break;
		}
		// Удаляем первый идентификатор
		tmp.erase(tmp.begin());
		// Если список еще не пустой, продолжаем дальше
		if(tmp.size() >= size_t(this->size)) this->add(tmp, idd, true);
		// Очищаем собранный список n-грамм
		if(!this->ngrams.empty()) this->ngrams.clear();
	}
}
/**
 * prune Метод прунинга языковой модели
 * @param threshold порог частоты прунинга
 * @param mingram   значение минимальной n-граммы за которую нельзя прунить
 * @param status    функция вывода статуса обучения
 */
void anyks::Arpa::prune(const double threshold, const u_short mingram, function <void (const u_short)> status) const {
	// Проверяем включён ли режим отладки
	const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
	// Если словарь не пустой
	if(!this->vocab.empty() && (this->size > 1)){
		/**
		 * sumFn Функция подсчёта суммы весов n-граммы
		 * @param gram    значение текущей n-граммы
		 * @param context контекст которому принадлежит слово
		 * @return        подсчитанная сумма весов
		 */
		auto sumFn = [this](const u_short gram, const vocab_t * context){
			// Результат работы функции
			float result = 0.0f;
			// Если контекст передан
			if(context != nullptr){
				// Извлекаем предшествующую n-грамму
				while(context->father != nullptr){
					// Запоминаем результат
					if((context->weight != this->zero) &&
					(context->idw != size_t(token_t::start)) &&
					((gram > 1) || !this->isOption(options_t::resetUnk) ||
					(context->idw != size_t(token_t::unk)))) result += context->weight;
					/**
					 * Если мы вычисляем предельную вероятность контекста униграммы <s>,
					 * мы должны искать </s> вместо неё, поскольку у начала предложения вес = 0.
					 */
					if(context->idw == size_t(token_t::start)) result += context->father->at((size_t) token_t::finish).weight;
					// Выполняем смещение
					context = context->father;
				}
			}
			// Выводим результат
			return result;
		};
		// Список n-грамм для работы
		list <vocab_t *> ngrams;
		// Количество n-грамм которые нужно обработать
		size_t counts = 0, index = 0;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Устанавливаем нуминатор и денуминатор
		double numerator = 0.0, denominator = 0.0;
		// Частота и обратная частота документа
		float weight = 0.0f, backoff = 0.0f, total = 0.0f;
		// Если размер минимальной n-граммы не установлен, устанавливаем его
		const u_short gram = (mingram == 0 ? this->size : mingram);
		// Переходим по всем n-граммам задом наперёд
		for(u_short i = this->size; (i > 0) && (i >= gram); i--){
			// Получаем количество n-грамм
			counts += this->count(i, true);
		}
		// Переходим по всем n-граммам задом наперёд
		for(u_short i = this->size; (i > 0) && (i >= gram); i--){
			// Выполняем извлечение n-грамм
			this->get(i, &ngrams);
			// Если список n-грамм получен
			if(!ngrams.empty()){
				// Количество обработанных n-грамм
				size_t prunedNgrams = 0;
				// Переходим по всему списку полученных n-грамм
				for(auto & item : ngrams){
					// Если в n-грамме есть дочерные граммы
					if(!item->empty()){
						// Обнуляем нумератор
						numerator = 0.0;
						// Обнуляем денумератор
						denominator = 0.0;
						// Получаем обратную частоту документа
						backoff = item->backoff;
						// Если обратная частота документа существует
						if(backoff != 0.0f){
							/**
							 * Вычисляем числитель и знаменатель обратной частоты отката,
							 * чтобы мы могли быстро вычислить корректировку BOW из-за отсутствия одного пробника.
							 */
							if(!this->compute(item, i - 1, numerator, denominator)){
								// Увеличиваем количество обработанных n-грамм
								index++;
								// Пропускаем ход
								continue;
							}
							// Получаем сумму весов
							total = sumFn(i, item);
							// Прунник всех вариантов
							bool allPruned = true;
							// Переходим по всему слов в контексте
							for(auto & value : * item){
								// Если частота n-граммы существует
								if(value.second.weight != 0.0f){
									// Генерируем обратную частоту n-граммы
									float bow = this->backoff(value.second.idw, item, i - 2);
									// Расчитываем новую обратную частоту документа
									float nbow = (log10(numerator + pow(10, value.second.weight)) - log10(denominator + pow(10, bow)));
									/**
									 * Вычислить изменение энтропии за счет удаления ngram
									 * deltaH = - P(H) x
									 *  {P(W | H) [log P(w|h') + log BOW'(h) - log P(w|h)] +
									 *  (1 - \sum_{v,h ngrams} P(v|h)) [log BOW'(h) - log BOW(h)]}
									 *
									 * (1-\sum_{v,h ngrams}) - масса вероятности,
									 * оставшаяся от n-граммы текущего порядка,
									 * и такая же, как числитель в BOW(h).
									 */
									float delta = (bow + nbow - value.second.weight);
									// Считаем дельту энтропии
									float deltaEntropy = (- pow(10, total) * (pow(10, value.second.weight) * delta + numerator * (nbow - backoff)));
									/**
									 * Вычисляем относительное изменение модели (трейн-сета) перплексия
									 *  (PPL' - PPL)/PPL = PPL'/PPL - 1
									 *                   = exp(H')/exp(H) - 1
									 *                   = exp(H' - H) - 1
									 */
									double perpChange = (pow(10, deltaEntropy) - 1.0);
									// Проверяем выполнен ли прунинг
									bool pruned = ((threshold > 0) && (perpChange < threshold));
									/**
									 * Убеждаемся, что мы не удаляем n-граммы,
									 * чьи частоты отката нам необходимы ...
									 */
									if(pruned && ((value.second.backoff != this->zero) && (this->weights(&value.second) != 0.0f))) pruned = false;
									// Если отладка включена
									if(debug){
										// Выводим статистику в сообщении
										this->alphabet->log(
											"CONTEXT %s WORD %s CONTEXTPROB %4.6f OLDPROB %4.6f NEWPROB %4.6f DELTA-H %4.6f DELTA-LOGP %4.6f PPL-CHANGE %4.6f PRUNED %u",
											alphabet_t::log_t::info,
											this->logfile,
											this->context(item).c_str(),
											this->word(value.second.idw, this->uppers(&value.second).first).c_str(),
											total,
											value.second.weight,
											(bow + nbow),
											deltaEntropy,
											delta,
											perpChange,
											pruned
										);
									}
									// Если пруннинг выполнен
									if(pruned){
										// Запоминаем количество обработанных n-грамм
										prunedNgrams++;
										// Удаляем вес n-граммы
										value.second.weight = 0.0f;
									// Отмечаем что пруннинг выполнен не всех n-грамм
									} else allPruned = false;
								}
								// Если функция вывода статуса передана
								if(status != nullptr){
									// Увеличиваем количество обработанных n-грамм
									index++;
									// Выполняем расчёт текущего статуса
									actual = u_short(index / float(counts) * 100.0f);
									// Если статус обновился
									if(actual != past){
										// Запоминаем текущий статус
										past = actual;
										// Выводим статус обработки
										status(actual);
									}
								}
							}
							/**
							 * Если мы удалили все n-граммы для этого контекста,
							 * мы можем удалить сам контекст,
							 * но только если текущий контекст не является префиксом более длинного.
							 */
							if(allPruned && (this->weights(item) == 0.0f)) item->backoff = this->zero;
						}
					}
				}
				// Если отладка включена
				if(debug && (prunedNgrams > 0)){
					// Выводим статистику в сообщении
					this->alphabet->log("pruned %u %u-grams", alphabet_t::log_t::info, this->logfile, prunedNgrams, i);
				}
			}
		}
		/**
		 * Здесь важно, чтобы мы вычисляли веса отката в возрастающем порядке,
		 * поскольку старшие по порядку относятся к младшим в алгоритме отсрочки.
		 * Обратите внимание, что будут сгенерированны узлы отката только для тех контекстов,
		 * в которых есть слова с явными вероятностями.
		 * Но это именно так, как и должно быть.
		 */
		for(u_short i = 1; i <= this->size; i++){
			// Выполняем извлечение n-грамм
			this->get(i, &ngrams);
			// Если список n-грамм получен
			if(!ngrams.empty()){
				// Переходим по всему списку полученных n-грамм
				for(auto & item : ngrams){
					// Если в n-грамме есть дочерные граммы
					if(!item->empty()) this->backoffs(i - 1, item);
				}
			}
		}
	// Сообщаем что словарь оказался пустым
	} else if(debug) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "vocab is empty");
}
/**
 * sweep Метод удаления низкочастотных n-грамм arpa
 * @param status статус расёта
 */
void anyks::Arpa::sweep(function <void (const u_short)> status) const {
	// Проверяем включён ли режим отладки
	const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
	// Если словарь не пустой
	if(!this->vocab.empty()){
		// Список n-грамм для работы
		list <vocab_t *> ngrams;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Количество n-грамм и индекс обработанной n-граммы
		size_t count = 0, index = 0, erased = 0;
		// Переходим по всем n-граммам и считаем количество дочерних n-грамм
		for(u_short i = 2; i <= this->size; i++){
			// Получаем количество n-грамм
			count += this->count(i, true);
		}
		// Если количество элементов больше 0
		if(count > 0){
			/**
			 * removeFn Прототип функции зануления всех дочерних n-грамм
			 * @param позиция текущего контекста
			 */
			function <void (vocab_t *)> removeFn;
			/**
			 * removeFn Функция зануления всех дочерних n-грамм
			 * @param context позиция текущего контекста
			 */
			removeFn = [&removeFn](vocab_t * context){
				// Если список не пустой
				if(!context->empty()){
					// Переходим по всем n-граммам
					for(auto & item : * context){
						// Зануляем ненужную нам n-грамму
						item.second.weight = 0.0f;
						// Если есть дочерние n-граммы зануляем и их
						if(!item.second.empty()) removeFn(&item.second);
					}
				}
			};
			// Переходим по всем n-граммам и удаляем те у которых слишком низкая частота
			for(u_short i = (this->size - 1); i > 1; i--){
				// Выполняем извлечение n-грамм
				this->get(i, &ngrams);
				// Если список n-грамм получен
				if(!ngrams.empty()){
					// Переходим по всему списку полученных n-грамм
					for(auto & item : ngrams){
						// Если в n-грамме есть дочерные граммы
						if(!item->empty()){
							// Переходим по всему списку грамм
							for(auto & value : * item){
								// Если частота n-граммы меньше её частоты отката, удаляем n-грамму
								if((value.second.father->weight == 0.0f) || (value.second.weight < value.second.backoff) || (value.second.backoff == this->zero)){
									// Считаем количество удалённых n-грамм
									erased++;
									// Выводим результат
									if(debug){
										// Сообщение выводимое в лог
										string message = "CONTEXT %s HAS BEEN REMOVED";
										// Если удаление идет по частоте
										if(value.second.weight < value.second.backoff){
											// Выводим данные частот
											message.append(" LPROB %4.6f BOW %4.6f");
										}
										// Выводим статистику в сообщении
										this->alphabet->log(message.c_str(), alphabet_t::log_t::info, this->logfile, this->context(&value.second).c_str(), value.second.weight, value.second.backoff);
									}
									// Зануляем ненужную нам n-грамму
									value.second.weight = 0.0f;
									// Продолжаем зануление
									removeFn(&value.second);
								}
								// Если функция вывода статуса передана
								if(status != nullptr){
									// Увеличиваем значение индекса
									index++;
									// Выполняем расчёт текущего статуса
									actual = u_short(index / float(count) * 100.0f);
									// Если статус обновился
									if(actual != past){
										// Запоминаем текущий статус
										past = actual;
										// Выводим статус обучения
										status(actual);
									}
								}
							}
						}
					}
				}
			}
		}
		// Выводим сообщение количество удалённых грамм
		if(debug) this->alphabet->log("%u n-grams were erased", alphabet_t::log_t::info, this->logfile, erased);
	// Сообщаем что словарь оказался пустым
	} else if(debug) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "vocab is empty");
}
/**
 * trian Метод расёта частот n-грамм
 * @param status статус расёта
 */
void anyks::Arpa::train(function <void (const u_short)> status) const {
	// Проверяем включён ли режим отладки
	const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
	// Если словарь не пустой
	if(!this->vocab.empty()){
		// Сбрасываем текущую позицию n-граммы
		this->gram = 1;
		// Список n-грамм для работы
		list <vocab_t *> ngrams;
		// Количество n-грамм и индекс обработанной n-граммы
		size_t count = 0, index = 0;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		// Получаем значение флага интерполяции
		bool interpolate = this->isOption(options_t::interpolate);
		/**
		 * estimate1Fn Функция первоначального подсчёта грамм
		 * @param oc встречаемость n-граммы
		 */
		auto estimate1Fn = [this](const size_t oc){
			// Считаем существующее слова
			this->param.observed++;
			// Считаем общее количество встречаемости
			this->param.total += oc;
			// Увеличиваем количество минимальных встречающихся слов
			if(oc >= 2) this->param.min2Vocab++;
			if(oc >= 3) this->param.min3Vocab++;
		};
		/**
		 * estimate2Fn Функция основного подсчёта грамм
		 * @param idw   идентификатор слова
		 * @param ngram параметры граммы
		 */
		auto estimate2Fn = [debug, &interpolate, this](const size_t idw, vocab_t * ngram){
			// Результат расчёта
			float lprob = 0.0f;
			// Вероятность n-граммы
			double prob = 0.0f;
			// Размер скидки
			double discount = 0.0f;
			// Если это безсобытийная грамма (Начальная)
			if((idw == size_t(token_t::start)) || ((this->gram == 1) &&
			this->isOption(options_t::resetUnk) && (idw == size_t(token_t::unk)))){
				// Если это неизвестный символ, то считаем количество безсобытийных грамм
				if(idw == size_t(token_t::unk)) this->param.nonevent++;
				// Устанавливаем значение скидки по умолчанию
				discount = 1.0f;
				// Устанавливаем результат по умолчанию
				lprob = this->zero;
				// Запоминаем результат
				ngram->weight = lprob;
			// Если это обычное слово
			} else {
				/**
				 * Для интерполированных оценок мы вычисляем взвешенную
				 * линейную комбинацию оценки высокого порядка
				 * (вычислено выше) и оценку более низкого порядка.
				 * Вес старшего заказа определяется коэффициентом дисконтирования,
				 * вес младшего разряда получен из скидки
				 * метод (может быть 0, если метод не поддерживает интерполяцию).
				 */
				float lowerProb    = 0.0f;
				double lowerWeight = 0.0f;
				/**
				 * Переданный массив скидок может содержать 0 элементов,
				 * что указывает на отсутствие дисконтирования в этом заказе.
				 */
				if(this->nodiscount()) discount = 1.0f;
				// Выполняем дисконтирование
				else discount = this->discount(ngram->oc, this->param.total, this->param.observed);
				// Выполняем расчёт
				prob = ((discount * double(ngram->oc)) / double(this->param.total));
				// Если нужно использовать интерполирование
				if(interpolate){
					// Выполняем расчет веса интерполирования
					lowerWeight = this->lowerWeight(this->param.total, this->param.observed, this->param.min2Vocab, this->param.min3Vocab);
					// Если это не юниграмма
					if(this->gram > 1) lowerProb = this->backoff(idw, ngram->father, this->gram - 2);
					// Если же это юниграмма
					else lowerProb = (-log10((double) this->unigrams()));
					// Выполняем окончательный расчёт
					prob += (lowerWeight * pow(10, lowerProb));
				}
				// Получаем финальный результат
				lprob = log10(prob);
				// Если результат получен
				if(discount != 0.0){
					// Увеличиваем общую вероятность n-грамм
					this->param.prob += prob;
					// Если режим отладки включён
					if(debug){
						// Выводимое сообщение статистики
						string message = "CONTEXT %s WORD %s NUMER %u DENOM %u DISCOUNT %4.6f";
						// Если включено интерполирование
						if(interpolate){
							// Дополняем сообщение
							message.append(" LOW %4.6f LOLPROB %4.6f");
							// Выводим статистику в сообщении
							this->alphabet->log(message.c_str(), alphabet_t::log_t::info, this->logfile, this->context(ngram->father).c_str(), this->word(idw).c_str(), ngram->oc, this->param.total, discount, lowerWeight, lowerProb);
						// Если интерполирование отключено
						} else {
							// Дополняем сообщение
							message.append(" LPROB %4.6f");
							// Выводим статистику в сообщении
							this->alphabet->log(message.c_str(), alphabet_t::log_t::info, this->logfile, this->context(ngram->father).c_str(), this->word(idw).c_str(), ngram->oc, this->param.total, discount, lprob);
						}
					}
				}
				// Если дисконтирование не удалось расчитать
				if(discount == 0.0)
					// Увеличиваем количество n-грамм которые не удалось расчитать
					this->param.discounted++;
				// Запоминаем результат
				else ngram->weight = lprob;
			}
		};
		/**
		 * runFn Прототип функции перехода по граммам
		 */
		function <void ()> runFn;
		/**
		 * runFn Функция перехода по граммам
		 */
		runFn = [&]{
			// Если дисконтирование включено, изменяем встречаемости
			if(!this->nodiscount()) this->prepare(this->gram);
			// Если это нулевая n-грамма
			switch(this->gram){
				// Если это юниграмма
				case 1: {
					// Переходим по всему списку юниграмм
					for(auto & value : this->vocab){
						// Если это безсобытийная грамма (Начальная)
						if((value.second.idw == size_t(token_t::start)) ||
						(this->isOption(options_t::resetUnk) &&
						(value.second.idw == size_t(token_t::unk)))) continue;
						// Выполняем первоначальный подсчёт грамм
						estimate1Fn(value.second.oc);
					}
					// Переходим по всему списку юниграмм
					for(auto & value : this->vocab){
						// Выполняем расчёт
						estimate2Fn(value.second.idw, &value.second);
						// Если функция вывода статуса передана
						if(status != nullptr){
							// Увеличиваем значение индекса
							index++;
							// Выполняем расчёт текущего статуса
							actual = u_short(index / float(count) * 100.0f);
							// Если статус обновился
							if(actual != past){
								// Запоминаем текущий статус
								past = actual;
								// Выводим статус обучения
								status(actual);
							}
						}
					}
					/**
					 * При наличии всех проб, BOWs получаются просто путем обычной нормализации.
					 * Мы делаем это прямо перед вычислением проб более высокого порядка,
					 * поскольку оценка N-грамм более высокого порядка может относиться
					 * к оценкам более низкого порядка (например, для интерполированных оценок).
					 */
					this->backoffs(0, &this->vocab);
				} break;
				// Если это биграмма или больше
				default: {
					// Выполняем извлечение n-грамм
					this->get(this->gram, &ngrams);
					// Если список n-грамм получен
					if(!ngrams.empty()){
						// Переходим по всему списку полученных n-грамм
						for(auto & item : ngrams){
							// Если в n-грамме есть дочерные граммы
							if(!item->empty()){
								// Обнуляем встречаемость всех n-грамм
								this->param.total = 0;
								// Обнуляем количество реальных слов
								this->param.observed = 0;
								// Обнуляем встречаемость больше 2-х раз
								this->param.min2Vocab = 0;
								// Обнуляем встречаемость больше 3-х раз
								this->param.min3Vocab = 0;
								// Активируем снова интерполяцию
								interpolate = this->isOption(options_t::interpolate);
								// Переходим по всему списку грамм
								for(auto & value : * item){
									// Если это безсобытийная грамма (Начальная)
									if(value.second.idw == size_t(token_t::start)) continue;
									// Выполняем первоначальный подсчёт грамм
									estimate1Fn(value.second.oc);
								}
								// Если общее количество n-грамм собрать не вышло
								if(this->param.total == 0) continue;
								/**
								 * Вычисляем дисконтированные вероятности
								 * из подсчётов и сохраняем их в частоте отката.
								 */
								retry:
								// Обнуляем общую вероятность n-грамм
								this->param.prob = 0.0f;
								// Переходим по всему списку грамм
								for(auto & value : * item){
									// Если количество n-грамм не определено
									if((this->gram > 1) && (value.second.oc == 0)) continue;
									// Выполняем расчёт
									estimate2Fn(value.second.idw, &value.second);
									// Если функция вывода статуса передана
									if(status != nullptr){
										// Увеличиваем значение индекса
										index++;
										// Выполняем расчёт текущего статуса
										actual = u_short(index / float(count) * 100.0f);
										// Если статус обновился
										if(actual != past){
											// Запоминаем текущий статус
											past = actual;
											// Выводим статус обучения
											status(actual);
										}
									}
								}
								/**
								 * Этот взлом зачислен Дугу Полу (Рони Розенфельдом в его инструментах CMU).
								 * Может случиться так, что после суммирования всех явных проб не останется
								 * массы вероятности, как правило, потому, что коэффициенты
								 * дисконтирования были вне диапазона и были установлены на 1.0.
								 * Если мы не увидели все словарные слова в этом контексте,
								 * чтобы получить некоторую ненулевую массу отката,
								 * мы пытаемся увеличить знаменатель в оценке на 1.
								 * Еще один хак: если в методе дисконтирования используется интерполяция,
								 * мы сначала пытаемся отключить его, поскольку интерполяция удаляет массу вероятности.
								 */
								if(!this->nodiscount() && (this->param.total > 0) && (this->param.observed < this->unigrams()) && (this->param.prob > (1.0 - EPSILON))){
									// Выводим предупреждение
									if(debug){
										// Формируем вывод сообщения
										string message = "%4.6f backoff probability mass left for \"%s\" -- ";
										// Если интеполяция включена - сообщаем, что отключаем интерполяцию
										if(interpolate)
											// Если интеполяция включена - сообщаем, что отключаем интерполяцию
											message.append("disabling interpolation");
										// Выводим сообщение для неинтерполированного деноминатора
										else message.append("incrementing denominator");
										// Выводим сообщение об ошибке
										this->alphabet->log(message.c_str(), alphabet_t::log_t::warning, this->logfile, (1.0f - this->param.prob), this->context(item).c_str());
									}
									// Отключаем интерполяцию
									if(interpolate) interpolate = false;
									// Увеличиваем количество n-грамм
									else this->param.total += 1;
									// Обновляем обработанный индекс
									if(status != nullptr) index -= this->ngrams[this->gram].size();
									// Пробуем обработать еще раз
									goto retry;
								}
								/**
								 * При наличии всех проб, BOWs получаются просто путем обычной нормализации.
								 * Мы делаем это прямо перед вычислением проб более высокого порядка,
								 * поскольку оценка N-грамм более высокого порядка может относиться
								 * к оценкам более низкого порядка (например, для интерполированных оценок).
								 */
								this->backoffs(this->gram - 1, item);
							}
						}
					}
				}
			}
			// Выводим результат работы
			if(debug && (this->param.discounted > 0)){
				// Выводим статистику в сообщении
				this->alphabet->log("discarded %u %u-gram probs discounted to zero", alphabet_t::log_t::info, this->logfile, this->param.discounted, this->gram);
			}
			// Выводим результат работы
			if(debug && (this->param.nonevent > 0)){
				// Выводим статистику в сообщении
				this->alphabet->log("discarded %u %u-gram probs predicting pseudo-events", alphabet_t::log_t::info, this->logfile, this->param.nonevent, this->gram);
			}
			// Увеличиваем размер граммы
			this->gram++;
			// Если есть следующий шаг
			if(this->gram <= this->size) runFn();
		};
		// Выполняем оценку для каждой n-граммы
		for(u_short i = 1; i <= this->size; i++){
			// Если оценка не выполнена, выводим ошибку
			if(!this->estimate(i)){
				// Выводим сообщение об ошибке
				if(debug) this->alphabet->log("in discount estimator for order %u", alphabet_t::log_t::warning, this->logfile, i);
				// Выходим из приложения
				exit(EXIT_FAILURE);
			// Считаем количество всех n-грамм
			} else count += this->count(i, true);
		}
		// Запускаем переход по граммам
		runFn();
		// Выполняем фиксацию частот
		this->fixupProbs(1);
	// Сообщаем что словарь оказался пустым
	} else if(debug) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "vocab is empty");
}
/**
 * repair Метод ремонта уже расчитанной ранее arpa
 * @param status статус расёта
 */
void anyks::Arpa::repair(function <void (const u_short)> status) const {
	// Проверяем включён ли режим отладки
	const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
	// Если словарь не пустой
	if(!this->vocab.empty()){
		// Сбрасываем текущую позицию n-граммы
		this->gram = 1;
		// Список n-грамм для работы
		list <vocab_t *> ngrams;
		// Количество n-грамм и индекс обработанной n-граммы
		size_t count = 0, index = 0;
		// Текущий и предыдущий статус
		u_short actual = 0, past = 100;
		/**
		 * runFn Прототип функции перехода по граммам
		 */
		function <void ()> runFn;
		/**
		 * runFn Функция перехода по граммам
		 */
		runFn = [&]{
			// Если это нулевая n-грамма
			switch(this->gram){
				// Если это юниграмма
				case 1: {
					/**
					 * При наличии всех проб, BOWs получаются просто путем обычной нормализации.
					 * Мы делаем это прямо перед вычислением проб более высокого порядка,
					 * поскольку оценка N-грамм более высокого порядка может относиться
					 * к оценкам более низкого порядка (например, для интерполированных оценок).
					 */
					this->backoffs(0, &this->vocab);
					// Переходим по всему списку юниграмм
					for(auto & value : this->vocab){
						// Если функция вывода статуса передана
						if(status != nullptr){
							// Увеличиваем значение индекса
							index++;
							// Выполняем расчёт текущего статуса
							actual = u_short(index / float(count) * 100.0f);
							// Если статус обновился
							if(actual != past){
								// Запоминаем текущий статус
								past = actual;
								// Выводим статус обучения
								status(actual);
							}
						}
					}
				} break;
				// Если это биграмма или больше
				default: {
					// Выполняем извлечение n-грамм
					this->get(this->gram, &ngrams);
					// Если список n-грамм получен
					if(!ngrams.empty()){
						// Переходим по всему списку полученных n-грамм
						for(auto & item : ngrams){
							// Если в n-грамме есть дочерные граммы
							if(!item->empty()){
								/**
								 * При наличии всех проб, BOWs получаются просто путем обычной нормализации.
								 * Мы делаем это прямо перед вычислением проб более высокого порядка,
								 * поскольку оценка N-грамм более высокого порядка может относиться
								 * к оценкам более низкого порядка (например, для интерполированных оценок).
								 */
								this->backoffs(this->gram - 1, item);
								// Переходим по всему списку грамм
								for(auto & value : * item){
									// Если функция вывода статуса передана
									if(status != nullptr){
										// Увеличиваем значение индекса
										index++;
										// Выполняем расчёт текущего статуса
										actual = u_short(index / float(count) * 100.0f);
										// Если статус обновился
										if(actual != past){
											// Запоминаем текущий статус
											past = actual;
											// Выводим статус обучения
											status(actual);
										}
									}
								}
							}
						}
					}
				}
			}
			// Увеличиваем размер граммы
			this->gram++;
			// Если есть следующий шаг
			if(this->gram <= this->size) runFn();
		};
		// Выполняем оценку для каждой n-граммы
		for(u_short i = 1; i <= this->size; i++){
			// Считаем количество всех n-грамм
			count += this->count(i, true);
		}
		// Запускаем переход по граммам
		runFn();
		// Выполняем фиксацию частот
		this->fixupProbs(1);
	// Сообщаем что словарь оказался пустым
	} else if(debug) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "vocab is empty");
}
/**
 * data Метод извлечения данных arpa
 * @param gram     размер n-граммы для извлечения
 * @param callback функция обратного вызова
 */
void anyks::Arpa::data(const u_short gram, function <void (const string &)> callback) const {
	// Если n-граммы существуют
	if(!this->vocab.empty()){
		// Строка результата и вес n-граммы
		string result = "", weight = "", backoff = "";
		// Если это юниграмма
		if(gram == 1){
			// Список регистров слова
			std::set <size_t> uppers;
			// Переходим по всему списку юниграмм
			for(auto & value : this->vocab){
				// Очищаем предыдущие граммы
				result.clear();
				// Если n-грамма имеет вес
				if(value.second.weight != 0.0f){
					// Если это -Infinity или псевдо-ноль
					if((value.second.weight == this->zero) || (value.second.weight == this->pseudoZero))
						// Устанавливаем вес равный псевдо-нулю
						weight = move(to_string(this->pseudoZero));
					// Если это нормальный вес
					else weight = move(to_string(value.second.weight));
					// Если слово имеет частоту отката
					if((gram < this->size) && ((value.second.backoff != this->zero) && (this->weights(&value.second) != 0.0f))){
						// Получаем обратную частоту модели
						backoff = (value.second.backoff != 0.0f ? move(to_string(value.second.backoff)) : "0");
					// Иначе очищаем обратную частоту документа
					} else backoff.clear();
					// Если слова нужно выводить не в нижнем регистре
					if(!this->isOption(options_t::lowerCase)){
						// Получаем список регистров слова
						this->uniUppers(value.second.idw, uppers);
					}
					// Если список получен
					if(!uppers.empty()){
						// Переходим по всему списку регистров
						for(auto & ups : uppers){
							// Формируем слово с учётом регистра
							result = move(this->alphabet->format("%s\t%s", weight.c_str(), this->word(value.second.idw, ups).c_str()));
							// Если обратная частота документа присутствует
							if(!backoff.empty()){
								// Добавляем разделитель
								result.append("\t");
								// Добавляем частоту отката
								result.append(backoff);
							}
							// Выводим результат
							callback(result);
						}
					}
					// Формируем слово без учёта регистра
					result = move(this->alphabet->format("%s\t%s", weight.c_str(), this->word(value.second.idw).c_str()));
					// Если обратная частота документа присутствует
					if(!backoff.empty()){
						// Добавляем разделитель
						result.append("\t");
						// Добавляем частоту отката
						result.append(backoff);
					}
					// Выводим результат
					callback(result);
				}
			}
		// Если это n-грамма
		} else {
			// Список n-грамм для работы
			list <vocab_t *> ngrams;
			// Выполняем извлечение n-грамм
			this->get(gram, &ngrams);
			// Если список n-грамм получен
			if(!ngrams.empty()){
				// Значение регистров слова
				size_t uppers = 0;
				// Переходим по всему списку полученных n-грамм
				for(auto & item : ngrams){
					// Переходим по всему списку слов
					for(auto & value : * item){
						// Если n-грамма имеет вес
						if(value.second.weight != 0.0f){
							// Если это -Infinity или псевдо-ноль
							if((value.second.weight == this->zero) || (value.second.weight == this->pseudoZero))
								// Устанавливаем вес равный псевдо-нулю
								weight = move(to_string(this->pseudoZero));
							// Если это нормальный вес
							else weight = move(to_string(value.second.weight));
							// Формируем результат с учётом регистра
							result = move(this->alphabet->format("%s\t%s", weight.c_str(), this->context(&value.second).c_str()));
							// Если слово имеет частоту отката
							if((gram < this->size) && ((value.second.backoff != this->zero) && (this->weights(&value.second) != 0.0f))){
								// Добавляем разделитель
								result.append("\t");
								// Получаем обратную частоту модели
								backoff = (value.second.backoff != 0.0f ? move(to_string(value.second.backoff)) : "0");
								// Добавляем частоту отката
								result.append(move(backoff));
							}
							// Если слово существует, выводим его
							if(!result.empty()) callback(result);
						}
					}
				}
			}
		}
	// Выводим пустой результат
	} else callback("");
}
/**
 * grams Метод извлечения данных n-грамм в текстовом виде
 * @param gram     размер n-граммы для извлечения
 * @param callback функция обратного вызова
 */
void anyks::Arpa::grams(const u_short gram, function <void (const string &)> callback) const {
	// Если n-граммы существуют
	if(!this->vocab.empty()){
		// Если это юниграмма
		if(gram == 1){
			// Список регистров слова
			std::set <size_t> uppers;
			// Переходим по всему списку юниграмм
			for(auto & value : this->vocab){
				// Если n-грамма имеет вес
				if(value.second.weight != 0.0f){
					// Если слова нужно выводить не в нижнем регистре
					if(!this->isOption(options_t::lowerCase)){
						// Получаем список регистров слова
						this->uniUppers(value.second.idw, uppers);
					}
					// Если список получен
					if(!uppers.empty()){
						// Переходим по всему списку регистров
						for(auto & ups : uppers){
							// Выводим слово с учётом регистра
							callback(this->alphabet->format("%s\t%lld | %lld", this->word(value.second.idw, ups).c_str(), value.second.oc, value.second.dc));
						}
					}
					// Выводим слово без учёта регистра
					callback(this->alphabet->format("%s\t%lld | %lld", this->word(value.second.idw).c_str(), value.second.oc, value.second.dc));
				}
			}
		// Если это n-грамма
		} else {
			// Список n-грамм для работы
			list <vocab_t *> ngrams;
			// Выполняем извлечение n-грамм
			this->get(gram, &ngrams);
			// Если список n-грамм получен
			if(!ngrams.empty()){
				// Значение регистров слова
				size_t uppers = 0;
				// Переходим по всему списку полученных n-грамм
				for(auto & item : ngrams){
					// Переходим по всему списку слов
					for(auto & value : * item){
						// Если это верная n-грамма
						if(value.second.weight != 0.0f){
							// Формируем результат с учётом регистра
							callback(this->alphabet->format("%s\t%lld | %lld", this->context(&value.second).c_str(), value.second.oc, value.second.dc));
						}
					}
				}
			}
		}
	// Выводим пустой результат
	} else callback("");
}
/**
 * save Метод извлечения данных arpa в бинарном виде
 * @param arpa     флаг извлечения только arpa
 * @param callback функция обратного вызова
 */
void anyks::Arpa::save(const bool arpa, function <void (const vector <char> &, const u_short)> callback) const {
	// Если данные загружены
	if(!this->vocab.empty()){
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
		auto resultFn = [&buffer, &seq, &index, &callback, this]{
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
			callback(buffer, u_short(index / float(this->vocab.size()) * 100.0f));
			// Очищаем полученный буфер n-граммы
			buffer.clear();
		};
		/**
		 * runFn Прототип функции запуска формирования map карты последовательностей
		 * @param словарь для извлечения слова с его параметрами
		 */
		function <void (const vocab_t *)> runFn;
		/**
		 * runFn Функция запуска формирования map карты последовательностей
		 * @param vocab словарь для извлечения слова с его параметрами
		 */
		runFn = [&](const vocab_t * vocab){
			// Регистры слова
			pair_t uppers;
			// Идентификатор полученного и добавленного слова
			size_t idw = idw_t::NIDW, aidw = 0;
			// Получаем объект данных
			const vocab_t * obj = (vocab != nullptr ? vocab : &this->vocab);
			// Если есть смысл обрабатывать варианты
			if(this->weights(obj) != 0.0f){
				// Переходим по всему объекту с данными
				for(auto & item : * obj){
					// Увеличиваем индекс если это юниграмма
					if(vocab == nullptr) index++;
					// Если это верная n-грамма
					if(item.second.weight != 0.0f){
						// Запоминаем идентификатор добавленного слова
						idw = sequence.idw;
						// Получаем лучший регистр слова
						uppers = move(this->uppers(item.second.uppers, item.second.oc));
						// Формируем блок слова
						sequence.ups = uppers.first;
						// Извлекаем основные данные
						sequence.idw     = item.second.idw;
						sequence.weight  = item.second.weight;
						sequence.backoff = item.second.backoff;
						// Если нужно извлечь все данные
						if(!arpa){
							sequence.oc  = item.second.oc;
							sequence.dc  = item.second.dc;
						}
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
				}
			// Выводим собранную последовательность
			} else resultFn();
		};
		// Запускаем извлечение данных
		runFn(nullptr);
	// Выводим пустой результат
	} else callback({}, 0);
}
/**
 * map Метод извлечения карты последовательностей в виде CSV
 * @param callback функция обратного вызова
 * @param delim    разделитель последовательностей
 */
void anyks::Arpa::map(function <void (const string &, const u_short)> callback, const string & delim) const {
	// Если данные загружены
	if(!this->vocab.empty()){
		// Регистры слова
		pair_t uppers;
		// Индекс обработки статуса
		size_t index = 0;
		/**
		 * runFn Прототип функции запуска формирования map карты последовательностей
		 * @param словарь для извлечения слова с его параметрами
		 * @param сформированная строка последовательности
		 */
		function <void (const vocab_t *, const string &)> runFn;
		/**
		 * runFn Функция запуска формирования map карты последовательностей
		 * @param vocab словарь для извлечения слова с его параметрами
		 * @param str   сформированная строка последовательности
		 */
		runFn = [&](const vocab_t * vocab, const string & str){
			// Строка для извлечения данных
			string data = "";
			// Получаем объект данных
			const vocab_t * obj = (vocab != nullptr ? vocab : &this->vocab);
			// Если есть смысл обрабатывать варианты
			if(this->weights(obj) != 0.0f){
				// Переходим по всему объекту с данными
				for(auto & item : * obj){
					// Получаем данные строки
					data = str;
					// Увеличиваем индекс если это юниграмма
					if(vocab == nullptr) index++;
					// Если это верная n-грамма
					if(item.second.weight != 0.0f){
						// Добавляем разделитель
						if(!data.empty()) data.append(delim);
						// Получаем лучший регистр слова
						uppers = move(this->uppers(item.second.uppers, item.second.oc));
						// Добавляем полученные данные
						data.append(to_string(item.second.idw));
						// Добавляем экранирование характеристик
						data.append(":{");
						// Добавляем встречаемость n-граммы
						data.append(to_string(item.second.oc));
						// Добавляем разделитель
						data.append(",");
						// Добавляем количество документов где, встретилась n-грамма
						data.append(to_string(item.second.dc));
						// Добавляем разделитель
						data.append(",");
						// Добавляем регистр слова
						data.append(to_string(uppers.first));
						// Закрываем экранирование характеристик
						data.append("}");
						// Если еще есть продолжение граммы
						if(!item.second.empty()) runFn(&item.second, data);
						// Иначе выводим то что есть
						else callback(data, u_short(index / float(this->vocab.size()) * 100.0f));
					}
				}
			// Выводим собранную последовательность
			} else callback(str, u_short(index / float(this->vocab.size()) * 100.0f));
		};
		// Запускаем извлечение данных
		runFn(nullptr, "");
	// Выводим пустой результат
	} else callback("", 0);
};
/**
 * Arpa Конструктор
 * @param word функция получения слова
 */
anyks::Arpa::Arpa(words_t word){
	// Устанавливаем функцию получения слова
	this->setWordMethod(word);
}
/**
 * Arpa Конструктор
 * @param alphabet объект алфавита
 */
anyks::Arpa::Arpa(const alphabet_t * alphabet){
	// Устанавливаем алфавит
	this->setAlphabet(alphabet);
}
/**
 * Arpa Конструктор
 * @param alphabet объект алфавита
 * @param word     функция получения слова
 */
anyks::Arpa::Arpa(const alphabet_t * alphabet, words_t word){
	// Устанавливаем функцию получения слова
	this->setWordMethod(word);
	// Устанавливаем алфавит
	this->setAlphabet(alphabet);
}
/**
 * ~Arpa Деструктор
 */
anyks::Arpa::~Arpa(){
	// Очищаем выделенную ранее память
	this->clear();
}
/**
 * init Метод инициализации первоначальных данных
 */
void anyks::GoodTuring::init() const {
	// Выделяем память под объект
	this->minCountTuring.reserve(this->size);
	this->maxCountTuring.reserve(this->size);
	this->discountCoeffs.reserve(this->size);
	// Формируем данные по умолчанию
	for(u_short i = 0; i < this->size; i++){
		// Запоминаем размер максимальной и минимальной встречаемости
		this->minCountTuring[i + 1] = this->minCount[(((i + 1) > MAXSIZE) || this->isOption(options_t::allGrams) ? 0 : i + 1)];
		this->maxCountTuring[i + 1] = this->maxCount[(((i + 1) > MAXSIZE) || this->isOption(options_t::allGrams) ? 0 : i + 1)];
		// Добавляем первоначальное значение
		this->discountCoeffs[i + 1].resize(this->maxCountTuring[i + 1], 1.0);
	}
}
/**
 * nodiscount Метод проверки на необходимость расчёта скидки
 * @return результат проверки
 */
const bool anyks::GoodTuring::nodiscount() const {
	// Выполняем проверки, нужно ли отключить дисконтирование
	return ((this->minCountTuring[this->gram] <= 1) && (this->maxCountTuring[this->gram] <= 0));
}
/**
 * estimate Метод оценки
 * @param  gram максимальный размер n-граммы
 * @return      результат оценки
 */
const bool anyks::GoodTuring::estimate(const u_short gram) const {
	// Получаем максимальное значение встречаемости граммы
	const size_t maxCount = this->maxCountTuring[gram];
	// Встречаемость каждой граммы
	vector <size_t> countOfCounts(maxCount + 2);
	// Заполняем список встречаемости нулями
	for(size_t i = 0; i <= (maxCount + 1); i++) countOfCounts[i] = 0;
	// Список n-грамм для работы
	list <vocab_t *> ngrams;
	// Выполняем извлечение n-грамм
	this->get(gram, &ngrams);
	// Если список n-грамм получен
	if(!ngrams.empty()){
		// Значение начальное n-граммы
		size_t idw = 0;
		// Проверяем включён ли режим отладки
		const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
		// Переходим по всему списку полученных n-грамм
		for(auto & item : ngrams){
			// Если дочерние граммы существует
			if(!item->empty()){
				// Переходим по всем n-граммам
				for(auto & value : * item){
					// Получаем начальную n-грамму
					idw = this->midw(&value.second);
					// Если это безсобытийная грамма (Начальная)
					if((idw == size_t(token_t::start)) || ((gram == 1) &&
					this->isOption(options_t::resetUnk) && (idw == size_t(token_t::unk)))) continue;
					// Если встречаемость меньше максимального значения
					else if(value.second.oc <= (maxCount + 1)) countOfCounts[value.second.oc]++;
				}
			}
		}
		// Если отладка включена
		if(debug){
			// Выводим статистику в сообщении
			this->alphabet->log("Good-Turing discounting %u-grams", alphabet_t::log_t::info, this->logfile, gram);
			// Выводим все собранные данные n-грамм
			for(size_t i = 0; i <= (maxCount + 1); i++){
				// Выводим статистику в сообщении
				this->alphabet->log("GT-count [%u] = %u", alphabet_t::log_t::info, this->logfile, i, countOfCounts[i]);
			}
		}
		// Если первый элемент не существует
		if(countOfCounts[1] == 0){
			// Обнуляем максимальную встречаемость n-граммы
			this->maxCountTuring[gram] = 0;
			// Выводим сообщение об ошибке
			if(debug) this->alphabet->log("%s", alphabet_t::log_t::warning, this->logfile, "no singleton counts");
		}
		// Ищем n-граммы с нулевой встречаемостью, если находим уменьшаем максимальную встречаемость
		while((this->maxCountTuring[gram] > 0) && (countOfCounts[this->maxCountTuring[gram] + 1] == 0)){
			// Выводим сообщение об ошибке
			if(debug) this->alphabet->log("count of count %u is zero -- lowering maxcount", alphabet_t::log_t::warning, this->logfile, (this->maxCountTuring[gram] + 1));
			// Уменьшаем максимальную встречаемость n-грамм
			this->maxCountTuring[gram]--;
		}
		// Если максимальная встречаемость не обнаружена
		if(this->maxCountTuring[gram] <= 0){
			// Выводим сообщение об ошибке
			if(debug) this->alphabet->log("%s", alphabet_t::log_t::warning, this->logfile, "GT discounting disabled");
		// Иначе продолжаем
		} else {
			// Коэффициент расчёта
			double coeff = 0.0, coeff0 = 0.0;
			// Получаем максимальное значение встречаемости граммы
			const size_t maxCount = this->maxCountTuring[gram];
			// Выполняем расчёт общего термина
			const double commonTerm = ((maxCount + 1) * double(countOfCounts[maxCount + 1]) / double(countOfCounts[1]));
			// Перебираем все размеры встреченных грамм
			for(size_t i = 1; i <= maxCount; i++){
				// Если грамма ни разу не встретилась
				if(countOfCounts[i] == 0){
					// Устанавливаем начальное значение коэффициента расчёта
					coeff = 1.0;
					// Выводим сообщение об ошибке
					if(debug) this->alphabet->log("count of count %u is zero", alphabet_t::log_t::warning, this->logfile, i);
				// Если грамма встретилась
				} else {
					// Выполняем расчёт нулевого коэффициента
					coeff0 = (double((i + 1) * double(countOfCounts[i + 1]) / double(i * double(countOfCounts[i]))));
					// Выполняем расчёт основного коэффициента
					coeff = ((coeff0 - commonTerm) / (1.0 - commonTerm));
					// Если расчёт не верный
					if(!isfinite(coeff) || (coeff <= EPSILON) || (coeff0 > 1.0)){
						// Выводим сообщение об ошибке
						if(debug) this->alphabet->log("discount coeff %u is out of range: %4.6f", alphabet_t::log_t::warning, this->logfile, i, coeff);
						// Устанавливаем начальное значение коэффициента расчёта
						coeff = 1.0;
					}
				}
				// Устанавливаем полученный коэффициент расчёта
				this->discountCoeffs[gram][i] = coeff;
			}
		}
	}
	// Выводим результат
	return true;
}
/**
 * discount Метод расчёта скидки
 * @param  count    встречаемость n-граммы
 * @param  total    общее количество встречаемости всех n-грамм
 * @param  observed количество существующих n-грамм
 * @return          результат расчёта
 */
const double anyks::GoodTuring::discount(const size_t count, const size_t total, const size_t observed) const {
	// Выполняем дисконтирование
	if(count <= 0) return 1.0;
	else if(count < this->minCount[this->gram]) return 0.0;
	else if(count > this->maxCount[this->gram]) return 1.0;
	else return this->discountCoeffs[this->gram][count];
}
/**
 * GoodTuring Конструктор
 * @param word функция получения слова
 */
anyks::GoodTuring::GoodTuring(words_t word) : Arpa(word) {
	// Выполняем инициализацию данных
	this->init();
}
/**
 * GoodTuring Конструктор
 * @param alphabet объект алфавита
 */
anyks::GoodTuring::GoodTuring(const alphabet_t * alphabet) : Arpa(alphabet) {
	// Выполняем инициализацию данных
	this->init();
}
/**
 * GoodTuring Конструктор
 * @param alphabet объект алфавита
 * @param word     функция получения слова
 */
anyks::GoodTuring::GoodTuring(const alphabet_t * alphabet, words_t word) : Arpa(alphabet, word) {
	// Выполняем инициализацию данных
	this->init();
}
/**
 * nodiscount Метод проверки на необходимость расчёта скидки
 * @return результат проверки
 */
const bool anyks::ConstDiscount::nodiscount() const {
	// Выполняем проверки, нужно ли отключить дисконтирование
	return ((this->minCount[this->use()] <= 1) && (this->cdiscount[this->gram] == 0.0));
}
/**
 * init Метод инициализации первоначальных данных
 * @param discount значение дискаунта для расчёта
 */
void anyks::ConstDiscount::init(const double discount) const {
	// Выделяем память под объект
	this->cdiscount.reserve(this->size);
	// Формируем данные по умолчанию
	for(u_short i = 0; i < this->size; i++) this->cdiscount[i + 1] = ((discount < 0.0 ? 0.0 : (discount > 1.0 ? 1.0 : discount)));
}
/**
 * discount Метод расчёта скидки
 * @param  count    встречаемость n-граммы
 * @param  total    общее количество встречаемости всех n-грамм
 * @param  observed количество существующих n-грамм
 * @return          результат расчёта
 */
const double anyks::ConstDiscount::discount(const size_t count, const size_t total, const size_t observed) const {
	// Выполняем дисконтирование
	return ((count <= 0) ? 1.0 : (count < size_t(this->minCount[this->use()])) ? 0.0 : ((double(count) - this->cdiscount[this->gram]) / double(count)));
}
/**
 * lowerWeight Метод расчёта интерполяции
 * @param  total     общее количество встречаемости всех n-грамм
 * @param  observed  количество существующих n-грамм
 * @param  min2Vocab встречаемость больше 2-х раз
 * @param  min3Vocab встречаемость больше 3-х раз
 * @return           результат расчёта интерполяции
 */
const double anyks::ConstDiscount::lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const {
	// Выполняем расчёт интерполяции
	return (this->cdiscount[this->gram] * double(observed) / double(total));
}
/**
 * ConstDiscount Конструктор
 * @param discount значение дисконтирования
 */
anyks::ConstDiscount::ConstDiscount(const double discount){
	// Выполняем инициализацию данных
	this->init(discount);
}
/**
 * ConstDiscount Конструктор
 * @param word     функция получения слова
 * @param discount значение дисконтирования
 */
anyks::ConstDiscount::ConstDiscount(words_t word, const double discount) : Arpa(word) {
	// Выполняем инициализацию данных
	this->init(discount);
}
/**
 * ConstDiscount Конструктор
 * @param alphabet объект алфавита
 * @param discount значение дисконтирования
 */
anyks::ConstDiscount::ConstDiscount(const alphabet_t * alphabet, const double discount) : Arpa(alphabet) {
	// Выполняем инициализацию данных
	this->init(discount);
}
/**
 * ConstDiscount Конструктор
 * @param alphabet объект алфавита
 * @param word     функция получения слова
 * @param discount значение дисконтирования
 */
anyks::ConstDiscount::ConstDiscount(const alphabet_t * alphabet, words_t word, const double discount) : Arpa(alphabet, word) {
	// Выполняем инициализацию данных
	this->init(discount);
}
/**
 * discount Метод расчёта скидки
 * @param  count    встречаемость n-граммы
 * @param  total    общее количество встречаемости всех n-грамм
 * @param  observed количество существующих n-грамм
 * @return          результат расчёта
 */
const double anyks::NaturalDiscount::discount(const size_t count, const size_t total, const size_t observed) const {
	// Преобразуем в дробную часть
	double counts      = total;
	double countsVocab = observed;
	// Если встречаемость слишком низкая
	if(count < size_t(this->minCount[this->use()])) return 0.0;
	// Если количество настоящих грамм, совпадает с количеством слов в словаре
	else if(observed == this->unigrams()) return 1.0;
	// Иначе выполняем расчёт скидки
	else return (counts * (counts + 1.0) + countsVocab * (1.0 - countsVocab)) / (counts * (counts + 1.0) + 2.0 * countsVocab);
}
/**
 * nodiscount Метод проверки на необходимость расчёта скидки
 * @return результат проверки
 */
const bool anyks::AddSmooth::nodiscount() const {
	// Выполняем проверки, нужно ли отключить дисконтирование
	return ((this->minCount[this->use()] <= 1) && (this->delta[this->gram] == 0.0));
}
/**
 * init Метод инициализации первоначальных данных
 * @param delta значение дельты для расчёта
 */
void anyks::AddSmooth::init(const double delta) const {
	// Выделяем память под объект
	this->delta.reserve(this->size);
	// Формируем данные по умолчанию
	for(u_short i = 0; i < this->size; i++) this->delta[i + 1] = (delta < 0.0 ? 0.0 : delta);
}
/**
 * discount Метод расчёта скидки
 * @param  count    встречаемость n-граммы
 * @param  total    общее количество встречаемости всех n-грамм
 * @param  observed количество существующих n-грамм
 * @return          результат расчёта
 */
const double anyks::AddSmooth::discount(const size_t count, const size_t total, const size_t observed) const {
	// Получаем размер словаря
	const double size = this->unigrams();
	// Получаем текущее значение дельты
	const double delta = this->delta[this->gram];
	// Выполняем дисконтирование
	return (count <= 0) ? 1.0 : (count < size_t(this->minCount[this->use()])) ? 0.0 : (1.0 + delta / double(count)) / (1.0 + size * delta / double(total));
}
/**
 * AddSmooth Конструктор
 * @param delta значение дельты для расчёта
 */
anyks::AddSmooth::AddSmooth(const double delta){
	// Выполняем инициализацию данных
	this->init(delta);
}
/**
 * AddSmooth Конструктор
 * @param word  функция получения слова
 * @param delta значение дельты для расчёта
 */
anyks::AddSmooth::AddSmooth(words_t word, const double delta) : Arpa(word) {
	// Выполняем инициализацию данных
	this->init(delta);
}
/**
 * AddSmooth Конструктор
 * @param alphabet объект алфавита
 * @param delta    значение дельты для расчёта
 */
anyks::AddSmooth::AddSmooth(const alphabet_t * alphabet, const double delta) : Arpa(alphabet) {
	// Выполняем инициализацию данных
	this->init(delta);
}
/**
 * AddSmooth Конструктор
 * @param alphabet объект алфавита
 * @param word     функция получения слова
 * @param delta    значение дельты для расчёта
 */
anyks::AddSmooth::AddSmooth(const alphabet_t * alphabet, words_t word, const double delta) : Arpa(alphabet, word) {
	// Выполняем инициализацию данных
	this->init(delta);
}
/**
 * discount Метод расчёта скидки
 * @param  count    встречаемость n-граммы
 * @param  total    общее количество встречаемости всех n-грамм
 * @param  observed количество существующих n-грамм
 * @return          результат расчёта
 */
const double anyks::WittenBell::discount(const size_t count, const size_t total, const size_t observed) const {
	// Выполняем дисконтирование
	return ((count <= 0) ? 1.0 : (count < size_t(this->minCount[this->use()])) ? 0.0 : (double(total) / double(total + observed)));
}
/**
 * lowerWeight Метод расчёта интерполяции
 * @param  total     общее количество встречаемости всех n-грамм
 * @param  observed  количество существующих n-грамм
 * @param  min2Vocab встречаемость больше 2-х раз
 * @param  min3Vocab встречаемость больше 3-х раз
 * @return           результат расчёта интерполяции
 */
const double anyks::WittenBell::lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const {
	// Выполняем расчёт интерполяции
	return (double(observed) / double(total + observed));
}
/**
 * prepare Метод разбора встречаемостей
 * @param gram максимальный размер n-граммы
 */
void anyks::KneserNey::prepare(const u_short gram) const {
	// Если все n-граммы обработаны или не нужно модифицировать встречаемости, выходим
	if(this->modified[gram] || (gram >= this->size)) return;
	// Проверяем включён ли режим отладки
	const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
	// Если отладка включена, выводим сообщение в консоль
	if(debug) this->alphabet->log("modifying %u-gram counts for Kneser-Ney smoothing", alphabet_t::log_t::info, this->logfile, gram);
	// Список n-грамм для работы
	list <vocab_t *> first, second;
	// Выполняем извлечение n-грамм первой группы
	this->get(gram, &first);
	// Выполняем извлечение n-грамм второй группы
	this->get(gram + 1, &second);
	// Если список n-грамм получен
	if(!first.empty() && !second.empty()){
		// Ключ искомого элемента
		pair_t key;
		// Список встречаемости предыдущих грамм
		multimap <pair_t, vocab_t *> range;
		// Переходим по всему списку полученных n-грамм и обнуляем их
		for(auto & item : first){
			// Переходим по всем n-граммам
			for(auto & value : * item){
				// Если это не безсобытийная грамма (Начальная)
				if(this->midw(&value.second) != size_t(token_t::start)){
					// Обнуляем текущую встречаемость
					value.second.oc = 0;
					// Формируем ключ записи
					key = move(make_pair(gram == 1 ? 0 : value.second.father->idw, value.second.idw));
					// Добавляем слово в список
					range.emplace(key, &value.second);
				}
			}
		}
		// Переходим по всему списку второй группы и считаем граммы
		for(auto & item : second){
			// Переходим по всем n-граммам
			for(auto & value : * item){
				// Формируем ключ записи
				key = move(make_pair(gram == 1 ? 0 : value.second.father->idw, value.second.idw));
				// Получаем список слов для которых нужно пересчитать встречаемость
				auto ret = range.equal_range(key);
				// Переходим по всему списку найденных вариантов
				for(auto it = ret.first; it != ret.second; ++it){
					// Увеличиваем встречаемость n-граммы
					it->second->oc++;
				}
			}
		}
	}
	// Запоминаем что модификация выполнена
	this->modified[gram] = true;
}
/**
 * estimate Метод оценки
 * @param  gram максимальный размер n-граммы
 * @return      результат оценки
 */
const bool anyks::KneserNey::estimate(const u_short gram) const {
	// Если данные не расчитаны, расчитываем их
	if(!this->prepares[gram]) this->prepare(gram);
	// Список n-грамм для работы
	list <vocab_t *> ngrams;
	// Выполняем извлечение n-грамм
	this->get(gram, &ngrams);
	// Если список n-грамм получен
	if(!ngrams.empty()){
		// Первый табулированный счетчик и идентификатор слова
		size_t n1 = 0, n2 = 0, idw = 0;
		// Проверяем включён ли режим отладки
		const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
		// Переходим по всему списку полученных n-грамм
		for(auto & item : ngrams){
			// Переходим по всем n-граммам
			for(auto & value : * item){
				// Получаем начальную n-грамму
				idw = this->midw(&value.second);
				// Если это безсобытийная грамма (Начальная)
				if((idw == size_t(token_t::start)) || ((gram == 1) &&
				this->isOption(options_t::resetUnk) && (idw == size_t(token_t::unk)))) continue;
				// Иначе продолжаем
				else if(value.second.oc == 1) n1++;
				else if(value.second.oc == 2) n2++;
			}
		}
		// Если отладка разрешена
		if(debug) this->alphabet->log("Kneser-Ney smoothing %u-grams\r\nn1 = %u\r\nn2 = %u", alphabet_t::log_t::info, this->logfile, gram, n1, n2);
		// Если расчёты не удачны
		if((n1 == 0) || (n2 == 0)){
			// Выводим сообщение об ошибке
			if(debug) this->alphabet->log("%s", alphabet_t::log_t::warning, this->logfile, "one of required KneserNey count-of-counts is zero");
			// Выходим
			return false;
		}
		// Выполняем расчёт дискаунтера
		this->discounting[gram] = (double(n1) / double(n1 + 2 * n2));
		// Выводим отладочную информацию
		if(debug) this->alphabet->log("D = %4.6f", alphabet_t::log_t::info, this->logfile, this->discounting[gram]);
		/*
		// Если расчёт дискаунтеров ничего не дал
		if(this->discounting[gram] < 0.0){
			// Выводим сообщение об ошибке
			if(debug) this->alphabet->log("%s", alphabet_t::log_t::warning, this->logfile, "KneserNey discount is negative");
			// Выходим
			return false;
		}
		*/
		// Если данные не расчитаны, расчитываем их
		if(this->prepares[gram]) this->prepare(gram);
	}
	// Сообщаем что всё удачно
	return true;
}
/**
 * init Метод инициализации первоначальных данных
 * @param modified количество уже изменённых младших заказов
 * @param prepares необходимость изменения счёта, после вычисления
 */
void anyks::KneserNey::init(const bool modified, const bool prepares) const {
	// Выделяем память под объекты
	this->modified.reserve(this->size);
	this->prepares.reserve(this->size);
	this->discounting.reserve(this->size);
	// Формируем данные по умолчанию
	for(u_short i = 0; i < this->size; i++){
		this->discounting[i + 1] = 0.0;
		this->modified[i + 1] = modified;
		this->prepares[i + 1] = prepares;
	}
}
/**
 * discount Метод расчёта скидки
 * @param  count    встречаемость n-граммы
 * @param  total    общее количество встречаемости всех n-грамм
 * @param  observed количество существующих n-грамм
 * @return          результат расчёта
 */
const double anyks::KneserNey::discount(const size_t count, const size_t total, const size_t observed) const {
	// Если встречаемость не существует
	if(count <= 0) return 1.0;
	// Если встречаемость меньше минимального значения
	else if(count < size_t(this->minCount[this->use()])) return 0.0;
	// Выполняем дисконтирование
	else return ((double(count) - this->discounting[this->gram]) / double(count));
}
/**
 * lowerWeight Метод расчёта интерполяции
 * @param  total     общее количество встречаемости всех n-грамм
 * @param  observed  количество существующих n-грамм
 * @param  min2Vocab встречаемость больше 2-х раз
 * @param  min3Vocab встречаемость больше 3-х раз
 * @return           результат расчёта интерполяции
 */
const double anyks::KneserNey::lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const {
	// Выполняем расчёт интерполяции
	return (double(this->discounting[this->gram] * observed) / double(total));
}
/**
 * KneserNey Конструктор
 * @param modified количество уже изменённых младших заказов
 * @param prepares необходимость изменения счёта, после вычисления
 */
anyks::KneserNey::KneserNey(const bool modified, const bool prepares){
	// Выполняем инициализацию
	this->init(modified, prepares);
}
/**
 * KneserNey Конструктор
 * @param word     функция получения слова
 * @param modified количество уже изменённых младших заказов
 * @param prepares необходимость изменения счёта, после вычисления
 */
anyks::KneserNey::KneserNey(words_t word, const bool modified, const bool prepares) : Arpa(word) {
	// Выполняем инициализацию
	this->init(modified, prepares);
}
/**
 * KneserNey Конструктор
 * @param alphabet объект алфавита
 * @param modified количество уже изменённых младших заказов
 * @param prepares необходимость изменения счёта, после вычисления
 */
anyks::KneserNey::KneserNey(const alphabet_t * alphabet, const bool modified, const bool prepares) : Arpa(alphabet) {
	// Выполняем инициализацию
	this->init(modified, prepares);
}
/**
 * KneserNey Конструктор
 * @param alphabet объект алфавита
 * @param word     функция получения слова
 * @param modified количество уже изменённых младших заказов
 * @param prepares необходимость изменения счёта, после вычисления
 */
anyks::KneserNey::KneserNey(const alphabet_t * alphabet, words_t word, const bool modified, const bool prepares) : Arpa(alphabet, word) {
	// Выполняем инициализацию
	this->init(modified, prepares);
}
/**
 * init Метод инициализации первоначальных данных
 */
void anyks::ModKneserNey::init() const {
	// Выделяем память под объекты
	this->discountplus.reserve(this->size);
	this->modDiscounting.reserve(this->size);
	// Формируем данные по умолчанию
	for(u_short i = 0; i < this->size; i++){
		this->discountplus[i + 1]   = 0.0;
		this->modDiscounting[i + 1] = 0.0;
	}
}
/**
 * estimate Метод оценки
 * @param  gram максимальный размер n-граммы
 * @return      результат оценки
 */
const bool anyks::ModKneserNey::estimate(const u_short gram) const {
	// Если данные не расчитаны, расчитываем их
	if(!this->prepares[gram]) this->prepare(gram);
	// Список n-грамм для работы
	list <vocab_t *> ngrams;
	// Выполняем извлечение n-грамм
	this->get(gram, &ngrams);
	// Если список n-грамм получен
	if(!ngrams.empty()){
		// Первый табулированный счетчик
		size_t n1 = 0, n2 = 0, n3 = 0, n4 = 0, idw = 0;
		// Проверяем включён ли режим отладки
		const bool debug = (this->isOption(options_t::debug) || (this->logfile != nullptr));
		// Переходим по всему списку полученных n-грамм
		for(auto & item : ngrams){
			// Переходим по всем n-граммам
			for(auto & value : * item){
				// Получаем начальную n-грамму
				idw = this->midw(&value.second);
				// Если это безсобытийная грамма (Начальная)
				if((idw == size_t(token_t::start)) || ((gram == 1) &&
				this->isOption(options_t::resetUnk) && (idw == size_t(token_t::unk)))) continue;
				// Иначе продолжаем
				else if(value.second.oc == 1) n1++;
				else if(value.second.oc == 2) n2++;
				else if(value.second.oc == 3) n3++;
				else if(value.second.oc == 4) n4++;
			}
		}
		// Если отладка разрешена
		if(debug) this->alphabet->log("Kneser-Ney smoothing %u-grams\r\nn1 = %u\r\nn2 = %u\r\nn3 = %u\r\nn4 = %u", alphabet_t::log_t::info, this->logfile, gram, n1, n2, n3, n4);
		// Если расчёты не удачны
		if((n1 == 0) || (n2 == 0) || (n3 == 0) || (n4 == 0)){
			// Выводим сообщение об ошибке
			if(debug) this->alphabet->log("%s", alphabet_t::log_t::warning, this->logfile, "one of required KneserNey count-of-counts is zero");
			// Выходим
			return false;
		}
		// Расчёт дисконта констант (Ries 1997, Chen & Goodman 1998)
		double Y = (double) n1 /(n1 + 2 * n2);
		// Выполняем расчёт дискаунтеров
		this->discounting[gram]    = 1 - 2 * Y * n2 / n1;
		this->modDiscounting[gram] = 2 - 3 * Y * n3 / n2;
		this->discountplus[gram]   = 3 - 4 * Y * n4 / n3;
		// Выводим отладочную информацию
		if(debug) this->alphabet->log("D1 = %4.6f\r\nD2 = %4.6f\r\nD3+ = %4.6f", alphabet_t::log_t::info, this->logfile, this->discounting[gram], this->modDiscounting[gram], this->discountplus[gram]);
		// Если расчёт дискаунтеров ничего не дал
		if((this->discounting[gram] < 0.0) || (this->modDiscounting[gram] < 0.0) || (this->discountplus[gram] < 0.0)){
			// Выводим сообщение об ошибке
			if(debug) this->alphabet->log("%s", alphabet_t::log_t::warning, this->logfile, "one of modified KneserNey discounts is negative");
			// Выходим
			return false;
		}
		// Если данные не расчитаны, расчитываем их
		if(this->prepares[gram]) this->prepare(gram);
	}
	// Сообщаем что всё удачно
	return true;
}
/**
 * discount Метод расчёта скидки
 * @param  count    встречаемость n-граммы
 * @param  total    общее количество встречаемости всех n-грамм
 * @param  observed количество существующих n-грамм
 * @return          результат расчёта
 */
const double anyks::ModKneserNey::discount(const size_t count, const size_t total, const size_t observed) const {
	// Выполняем дисконтирование
	if(count <= 0) return 1.0;
	else if(count < size_t(this->minCount[this->use()])) return 0.0;
	else if(count == 1) return ((count - this->discounting[this->gram]) / count);
	else if(count == 2) return ((count - this->modDiscounting[this->gram]) / count);
	else return ((count - this->discountplus[this->gram]) / count);
}
/**
 * lowerWeight Метод расчёта интерполяции
 * @param  total     общее количество встречаемости всех n-грамм
 * @param  observed  количество существующих n-грамм
 * @param  min2Vocab встречаемость больше 2-х раз
 * @param  min3Vocab встречаемость больше 3-х раз
 * @return           результат расчёта интерполяции
 */
const double anyks::ModKneserNey::lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const {
	// Выполняем расчёт интерполяции
	return (
		(this->discounting[this->gram] *
		(observed - min2Vocab) +
		this->modDiscounting[this->gram] *
		(min2Vocab - min3Vocab) +
		this->discountplus[this->gram] *
		min3Vocab) / total
	);
}
/**
 * ModKneserNey Конструктор
 * @param modified количество уже изменённых младших заказов
 * @param prepares необходимость изменения счёта, после вычисления
 */
anyks::ModKneserNey::ModKneserNey(const bool modified, const bool prepares) : KneserNey(modified, prepares) {
	// Выполняем инициализацию
	this->init();
}
/**
 * ModKneserNey Конструктор
 * @param word     функция получения слова
 * @param modified количество уже изменённых младших заказов
 * @param prepares необходимость изменения счёта, после вычисления
 */
anyks::ModKneserNey::ModKneserNey(words_t word, const bool modified, const bool prepares) : KneserNey(word, modified, prepares) {
	// Выполняем инициализацию
	this->init();
}
/**
 * ModKneserNey Конструктор
 * @param alphabet объект алфавита
 * @param modified количество уже изменённых младших заказов
 * @param prepares необходимость изменения счёта, после вычисления
 */
anyks::ModKneserNey::ModKneserNey(const alphabet_t * alphabet, const bool modified, const bool prepares) : KneserNey(alphabet, modified, prepares) {
	// Выполняем инициализацию
	this->init();
}
/**
 * ModKneserNey Конструктор
 * @param alphabet объект алфавита
 * @param word     функция получения слова
 * @param modified количество уже изменённых младших заказов
 * @param prepares необходимость изменения счёта, после вычисления
 */
anyks::ModKneserNey::ModKneserNey(const alphabet_t * alphabet, words_t word, const bool modified, const bool prepares) : KneserNey(alphabet, word, modified, prepares) {
	// Выполняем инициализацию
	this->init();
}

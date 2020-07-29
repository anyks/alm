/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <collector.hpp>

/**
 * start Метод инициализации работы тредпула
 */
void anyks::Collector::start(){
	// Если тредпул ещё не создан
	if(this->tpool == nullptr){
		// Экранируем возможность ошибки памяти
		try {
			// Создаём тредпул
			this->tpool = new tpool_t;
			// Выполняем инициализацию тредпула
			this->tpool->init(this->threads);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выводим сообщение об ошибке, если режим отладки включён
			if(this->debug > 0) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "bad alloc for init threadpool");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * finish Метод завершения работы тредпула
 */
void anyks::Collector::finish(){
	// Если тредпул создан
	if(this->tpool != nullptr){
		// Ожидаем завершения обработки
		this->tpool->wait();
		// Удаляем выделенную память
		delete this->tpool;
		// Зануляем объект
		this->tpool = nullptr;
	}
}
/**
 * initPython Метод инициализации модуля питона
 */
void anyks::Collector::initPython(){
	// Если объект питона еще не создан
	if(this->python == nullptr){
		// Экранируем возможность ошибки памяти
		try {
			// Создаём объект для работы с python
			this->python = new python_t(this->tokenizer);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выводим сообщение об ошибке, если режим отладки включён
			if(this->debug > 0) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "bad alloc for init python");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * createDir Метод создания каталога для сохранения результата
 * @return адрес созданного каталога
 */
const string & anyks::Collector::createDir() const noexcept {
	// Название каталога для собранных файлов
	static const string dirName = "";
	// Если разрешено сохранять промежуточные результаты
	if(this->intermed){
		// Создаем буфер для хранения даты
		char date[80];
		// Добавляем в название каталога штамп времени
		// Получаем текущее время
		time_t timeStamp = time(nullptr);
		// Заполняем его нулями
		memset(date, 0, sizeof(date));
		// Получаем структуру локального времени
		struct tm * timeinfo = localtime(&timeStamp);
		// Создаем формат полученного времени
		const string & dateformat = "%m-%d-%Y_%H-%M-%S";
		// Копируем в буфер полученную дату и время
		const int length = strftime(date, sizeof(date), dateformat.c_str(), timeinfo);
		// Если дата создана
		if(length > 0){
			// Получаем данные строки
			string * dir = const_cast <string *> (&dirName);
			// Добавляем префикс каталога
			dir->append("./alm_");
			// Добавляем дату в адрес каталога
			dir->append(date, length);
			// Если каталога не существует, создаём его
			if(!fsys_t::isdir(dirName)) fsys_t::mkdir(dirName);
		}
	}
	// Выводим результат
	return dirName;
}
/**
 * getSize Функция получения размера в байтах
 * @param  str размер сегмента (b, kb, Mb, Gb)
 * @return     размер сегмента в байтах
 */
const long anyks::Collector::getSize(const string & str) const noexcept {
	/*
	* Help - http://www.securitylab.ru/analytics/243414.php
	*
	* 0.04 - Пропускная способность сети 40 милисекунд
	* 100 - Скорость в мегабитах (Мб) на пользователя
	* 8 - Количество бит в байте
	* 1024000 - количество байт в мегабайте
	* (2 * 0.04) * ((100 * 1024000) / 8)  = 1000 байт
	*
	*/
	// Размер буфера в байтах
	long size = -1;
	// Результат работы регулярного выражения
	smatch match;
	// Устанавливаем правило регулярного выражения
	regex e("([\\d\\.\\,]+)(b|kb|Mb|Gb)", regex::ECMAScript);
	// Выполняем поиск скорости
	regex_search(str, match, e);
	// Если данные найдены
	if(!match.empty()){
		// Запоминаем параметры
		const string & param = match[2].str();
		// Размерность скорости
		double dimension = 1;
		// Получаем значение скорости
		const double value = stod(match[1].str());
		// Проверяем являются ли переданные данные байтами (8, 16, 32, 64, 128, 256, 512, 1024 ...)
		const bool isbite = !fmod(value / 8, 2);
		// Если это байты
		if(param.compare("b") == 0) dimension = 1;
		// Если это размерность в киллобитах
		else if(param.compare("kb") == 0) dimension = (isbite ? 1000 : 1024);
		// Если это размерность в мегабитах
		else if(param.compare("Mb") == 0) dimension = (isbite ? 1000000 : 1048576);
		// Если это размерность в гигабитах
		else if(param.compare("Gb") == 0) dimension = (isbite ? 1000000000 : 1073741824);
		// Размер буфера по умолчанию
		size = (long) value;
		// Если скорость установлена тогда расчитываем размер буфера
		if(value > -1) size = (2 * 0.04) * ((value * dimension) / 8);
	}
	// Выводим результат
	return size;
}
/**
 * train Обучения полученного текста
 * @param dest      адрес каталога для сохранения
 * @param filenames список файлов для обучения
 */
void anyks::Collector::train(const string & dest, const vector <string> & filenames) noexcept {
	// Если текст передан
	if(!dest.empty() && !filenames.empty() && (this->tpool != nullptr)){
		// Добавляем в тредпул новое задание на обработку
		this->tpool->push([this](const string dest, const vector <string> filenames){
			// Общее количество данных собранное этим потоком
			size_t index = 0;
			// Получаем копию объекта тулкита
			toolkit_t toolkit(this->alphabet, this->tokenizer, this->order);
			// Устанавливаем log файл
			toolkit.setLogfile(this->logfile);
			// Устанавливаем внешний объект питона
			toolkit.setPythonObj(this->python);
			// Устанавливаем неизвестное слово
			toolkit.setUnknown(this->toolkit->getUnknown());
			// Устанавливаем опции тулкита
			toolkit.setOptions(this->toolkit->getOptions());
			// Устанавливаем список токенов приводимых к <unk>
			toolkit.setTokensUnknown(this->toolkit->getTokensUnknown());
			// Устанавливаем список запрещённых токенов
			toolkit.setTokensDisable(this->toolkit->getTokensDisable());
			// Устанавливаем скрипт препроцессинга слов
			toolkit.setWordScript(this->toolkit->getWordScript());
			// Устанавливаем скрипт идентифицирования пользовательский токенов
			toolkit.setUserTokenScript(this->toolkit->getUserTokenScript());
			// Получаем пользовательские токены
			const auto & tokens = this->toolkit->getUserTokens();
			// Получаем список плохих слов
			const auto & badwords = this->toolkit->getBadwords();
			// Получаем список хороших слов
			const auto & goodwords = this->toolkit->getGoodwords();
			// Устанавливаем пользовательские токены
			if(!tokens.empty()) for(auto & token : tokens) toolkit.setUserToken(token);
			// Переходим по всему списку плохих слов и добавляем их
			if(!badwords.empty()) for(auto & idw : badwords) toolkit.addBadword(idw);
			// Переходим по всему списку хороших слов и добавляем их
			if(!goodwords.empty()) for(auto & idw : goodwords) toolkit.addGoodword(idw);
			// Получаем параметры туллкита
			const auto params = this->toolkit->getParams();
			// Выполняем инициализацию тулкита
			toolkit.init((toolkit_t::algorithm_t) params.algorithm, params.modified, params.prepares, params.mod);
			// Переходим по всему списку файлов
			for(auto & filename : filenames){
				// Увеличиваем значение индекса
				index++;
				// Выполняем считывание всех строк текста
				fsys_t::rfile2(filename, [&](const string & str, const uintmax_t fileSize) noexcept {
					// Добавляем полученную строку текста
					if(!str.empty()) toolkit.addText(str, index);
				});
				// Если отладка включена, выводим индикатор загрузки
				if(this->debug > 0){
					// Общий полученный размер данных
					this->allSize.store(this->allSize + 1, memory_order_relaxed);
					// Подсчитываем статус выполнения
					this->status = u_short(this->allSize / double(this->dataSize) * 100.0);
					// Если процентное соотношение изменилось
					if(this->rate != this->status){
						// Запоминаем текущее процентное соотношение
						this->rate.store(this->status, memory_order_relaxed);
						// Блокируем поток
						this->locker.lock();
						// Устанавливаем название файла
						this->pss.description(filename);
						// Отображаем ход процесса
						switch(this->debug){
							case 1: this->pss.update(this->status); break;
							case 2: this->pss.status(this->status); break;
						}
						// Разблокируем поток
						this->locker.unlock();
					}
				}
			}
			// Блокируем поток
			this->locker.lock();
			// Получаем данные статистики словаря
			const auto & stat1 = toolkit.getStatistic();
			// Получаем данные статистики основного словаря
			const auto & stat2 = this->toolkit->getStatistic();
			// Увеличиваем статистику основного словаря
			this->toolkit->getStatistic(stat1.first + stat2.first, stat1.second + stat2.second);
			// Считываем все слова словаря
			toolkit.words([this](const word_t & word, const size_t idw, const size_t size){
				// Добавляем слово в словарь
				this->toolkit->addWord(word, idw);
				// Разрешаем перебор остальных слов
				return true;
			});
			// Извлекаем n-граммы
			toolkit.saveArpa([this](const vector <char> & buffer, const u_short status){
				// Загружаем данные n-граммы
				this->toolkit->appendArpa(buffer);
			});
			// Разблокируем поток
			this->locker.unlock();
			// Если нужно вывести промежуточные результаты
			if(this->intermed){
				// Экспортируем полученные данные карты
				toolkit.writeMap(dest + ".map");
				// Экспортируем полученные данные словаря
				toolkit.writeVocab(dest + ".vocab");
			}
		}, dest, filenames);
		// Получаем объект списка файлов
		vector <string> * obj = const_cast <vector <string> *> (&filenames);
		// Очищаем список файлов
		obj->clear();
		// Очищаем выделенную память
		vector <string> ().swap(* obj);
	}
}
/**
 * train Обучения полученного текста
 * @param dest     адрес каталога для сохранения
 * @param filename файл для чтения
 * @param idd      идентификатор документа
 */
void anyks::Collector::train(const string & dest, const string & filename, const size_t idd) noexcept {
	// Если текст передан
	if(!dest.empty() && !filename.empty() && (this->tpool != nullptr)){
		// Добавляем в тредпул новое задание на обработку
		this->tpool->push([this](const string dest, const string filename, const size_t idd){
			// Получаем копию объекта тулкита
			toolkit_t toolkit(this->alphabet, this->tokenizer, this->order);
			// Устанавливаем log файл
			toolkit.setLogfile(this->logfile);
			// Устанавливаем внешний объект питона
			toolkit.setPythonObj(this->python);
			// Устанавливаем неизвестное слово
			toolkit.setUnknown(this->toolkit->getUnknown());
			// Устанавливаем опции тулкита
			toolkit.setOptions(this->toolkit->getOptions());
			// Устанавливаем список токенов приводимых к <unk>
			toolkit.setTokensUnknown(this->toolkit->getTokensUnknown());
			// Устанавливаем список запрещённых токенов
			toolkit.setTokensDisable(this->toolkit->getTokensDisable());
			// Устанавливаем скрипт препроцессинга слов
			toolkit.setWordScript(this->toolkit->getWordScript());
			// Устанавливаем скрипт идентифицирования пользовательский токенов
			toolkit.setUserTokenScript(this->toolkit->getUserTokenScript());
			// Получаем пользовательские токены
			const auto & tokens = this->toolkit->getUserTokens();
			// Получаем список плохих слов
			const auto & badwords = this->toolkit->getBadwords();
			// Получаем список хороших слов
			const auto & goodwords = this->toolkit->getGoodwords();
			// Устанавливаем пользовательские токены
			if(!tokens.empty()) for(auto & token : tokens) toolkit.setUserToken(token);
			// Переходим по всему списку плохих слов и добавляем их
			if(!badwords.empty()) for(auto & idw : badwords) toolkit.addBadword(idw);
			// Переходим по всему списку хороших слов и добавляем их
			if(!goodwords.empty()) for(auto & idw : goodwords) toolkit.addGoodword(idw);
			// Получаем параметры туллкита
			const auto params = this->toolkit->getParams();
			// Выполняем инициализацию тулкита
			toolkit.init((toolkit_t::algorithm_t) params.algorithm, params.modified, params.prepares, params.mod);
			// Выполняем считывание всех строк текста
			fsys_t::rfile2(filename, [&](const string & str, const uintmax_t fileSize) noexcept {
				// Добавляем полученную строку текста
				if(!str.empty()) toolkit.addText(str, idd);
			});
			// Блокируем поток
			this->locker.lock();
			// Получаем данные статистики словаря
			const auto & stat1 = toolkit.getStatistic();
			// Получаем данные статистики основного словаря
			const auto & stat2 = this->toolkit->getStatistic();
			// Увеличиваем статистику основного словаря
			this->toolkit->getStatistic(stat1.first + stat2.first, stat1.second + stat2.second);
			// Считываем все слова словаря
			toolkit.words([this](const word_t & word, const size_t idw, const size_t size){
				// Добавляем слово в словарь
				this->toolkit->addWord(word, idw);
				// Разрешаем перебор остальных слов
				return true;
			});
			// Извлекаем n-граммы
			toolkit.saveArpa([idd, this](const vector <char> & buffer, const u_short status){
				// Загружаем данные n-граммы
				this->toolkit->appendArpa(buffer, idd);
			});
			// Разблокируем поток
			this->locker.unlock();
			// Если нужно вывести промежуточные результаты
			if(this->intermed){
				// Экспортируем полученные данные карты
				toolkit.writeMap(dest + to_string(idd) + ".map");
				// Экспортируем полученные данные словаря
				toolkit.writeVocab(dest + to_string(idd) + ".vocab");
			}
			// Если отладка включена, выводим индикатор загрузки
			if(this->debug > 0){
				// Общий полученный размер данных
				this->allSize.store(this->allSize + 1, memory_order_relaxed);
				// Подсчитываем статус выполнения
				this->status = u_short(this->allSize / double(this->dataSize) * 100.0);
				// Если процентное соотношение изменилось
				if(this->rate != this->status){
					// Запоминаем текущее процентное соотношение
					this->rate.store(this->status, memory_order_relaxed);
					// Блокируем поток
					this->locker.lock();
					// Устанавливаем название файла
					this->pss.description(filename);
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss.update(this->status); break;
						case 2: this->pss.status(this->status); break;
					}
					// Разблокируем поток
					this->locker.unlock();
				}
			}
		}, dest, filename, idd);
	}
}
/**
 * train Обучения полученного текста
 * @param dest адрес каталога для сохранения
 * @param text список строк текста для обучения
 * @param idd  идентификатор документа
 */
void anyks::Collector::train(const string & dest, const vector <string> & text, const size_t idd) noexcept {
	// Если текст передан
	if(!dest.empty() && !text.empty() && (this->tpool != nullptr)){
		// Добавляем в тредпул новое задание на обработку
		this->tpool->push([this](const string dest, const vector <string> text, const size_t idd){
			// Общее количество данных собранное этим потоком
			size_t size = 0;
			// Получаем максимальное значение общего размера
			const double maxSize = (this->dataSize + ceil(1 * this->dataSize / 100.0));
			// Получаем копию объекта тулкита
			toolkit_t toolkit(this->alphabet, this->tokenizer, this->order);
			// Устанавливаем log файл
			toolkit.setLogfile(this->logfile);
			// Устанавливаем внешний объект питона
			toolkit.setPythonObj(this->python);
			// Устанавливаем неизвестное слово
			toolkit.setUnknown(this->toolkit->getUnknown());
			// Устанавливаем опции тулкита
			toolkit.setOptions(this->toolkit->getOptions());
			// Устанавливаем список токенов приводимых к <unk>
			toolkit.setTokensUnknown(this->toolkit->getTokensUnknown());
			// Устанавливаем список запрещённых токенов
			toolkit.setTokensDisable(this->toolkit->getTokensDisable());
			// Устанавливаем скрипт препроцессинга слов
			toolkit.setWordScript(this->toolkit->getWordScript());
			// Устанавливаем скрипт идентифицирования пользовательский токенов
			toolkit.setUserTokenScript(this->toolkit->getUserTokenScript());
			// Получаем пользовательские токены
			const auto & tokens = this->toolkit->getUserTokens();
			// Получаем список плохих слов
			const auto & badwords = this->toolkit->getBadwords();
			// Получаем список хороших слов
			const auto & goodwords = this->toolkit->getGoodwords();
			// Устанавливаем пользовательские токены
			if(!tokens.empty()) for(auto & token : tokens) toolkit.setUserToken(token);
			// Переходим по всему списку плохих слов и добавляем их
			if(!badwords.empty()) for(auto & idw : badwords) toolkit.addBadword(idw);
			// Переходим по всему списку хороших слов и добавляем их
			if(!goodwords.empty()) for(auto & idw : goodwords) toolkit.addGoodword(idw);
			// Получаем параметры туллкита
			const auto params = this->toolkit->getParams();
			// Выполняем инициализацию тулкита
			toolkit.init((toolkit_t::algorithm_t) params.algorithm, params.modified, params.prepares, params.mod);
			// Сегодня в мире - две остановки
			// Переходим по всему списку строк текста
			for(auto & str : text){
				// Добавляем полученную строку текста
				toolkit.addText(str, idd);
				// Считаем общий размер данных обработанное данным процессом
				size += str.size();
				// Если отладка включена, выводим индикатор загрузки
				if(this->debug > 0){
					// Общий полученный размер данных
					this->allSize.store(this->allSize + str.size(), memory_order_relaxed);
					// Подсчитываем статус выполнения
					this->status = u_short(this->allSize / maxSize * 100.0);
					// Если процентное соотношение изменилось
					if(this->rate != this->status){
						// Запоминаем текущее процентное соотношение
						this->rate.store(this->status, memory_order_relaxed);
						// Блокируем поток
						this->locker.lock();
						// Отображаем ход процесса
						switch(this->debug){
							case 1: this->pss.update(this->status); break;
							case 2: this->pss.status(this->status); break;
						}
						// Разблокируем поток
						this->locker.unlock();
					}
				}
			}
			// Блокируем поток
			this->locker.lock();
			// Получаем минимальный размер оставшихся данных
			const double minSize = (1 * size / 100.0);
			// Получаем данные статистики словаря
			const auto & stat1 = toolkit.getStatistic();
			// Получаем данные статистики основного словаря
			const auto & stat2 = this->toolkit->getStatistic();
			// Увеличиваем статистику основного словаря
			this->toolkit->getStatistic(stat1.first + stat2.first, stat1.second + stat2.second);
			// Считываем все слова словаря
			toolkit.words([minSize, maxSize, this](const word_t & word, const size_t idw, const size_t size){
				// Добавляем слово в словарь
				this->toolkit->addWord(word, idw);
				// Если отладка включена, выводим индикатор загрузки
				if(this->debug > 0){
					// Общий полученный размер данных
					this->allSize.store(this->allSize + (size / (minSize / 2)), memory_order_relaxed);
					// Подсчитываем статус выполнения
					this->status = u_short(this->allSize / maxSize * 100.0);
					// Если процентное соотношение изменилось
					if(this->rate != this->status){
						// Запоминаем текущее процентное соотношение
						this->rate.store(this->status, memory_order_relaxed);
						// Отображаем ход процесса
						switch(this->debug){
							case 1: this->pss.update(this->status); break;
							case 2: this->pss.status(this->status); break;
						}
					}
				}
				// Разрешаем перебор остальных слов
				return true;
			});
			// Извлекаем n-граммы
			toolkit.saveArpa([idd, minSize, maxSize, this](const vector <char> & buffer, const u_short status){
				// Загружаем данные n-граммы
				this->toolkit->appendArpa(buffer, idd);
				// Если отладка включена, выводим индикатор загрузки
				if(this->debug > 0){
					// Общий полученный размер данных
					this->allSize.store(this->allSize + (100 / (minSize / 2)), memory_order_relaxed);
					// Подсчитываем статус выполнения
					this->status = u_short(this->allSize / maxSize * 100.0);
					// Если процентное соотношение изменилось
					if(this->rate != this->status){
						// Запоминаем текущее процентное соотношение
						this->rate.store(this->status, memory_order_relaxed);
						// Отображаем ход процесса
						switch(this->debug){
							case 1: this->pss.update(this->status); break;
							case 2: this->pss.status(this->status); break;
						}
					}
				}
			});
			// Разблокируем поток
			this->locker.unlock();
			// Если нужно вывести промежуточные результаты
			if(this->intermed){
				// Экспортируем полученные данные карты
				toolkit.writeMap(dest + to_string(idd) + ".map");
				// Экспортируем полученные данные словаря
				toolkit.writeVocab(dest + to_string(idd) + ".vocab");
			}
		}, dest, text, idd);
		// Получаем объект текста
		vector <string> * obj = const_cast <vector <string> *> (&text);
		// Очищаем список текстов
		obj->clear();
		// Очищаем выделенную память
		vector <string> ().swap(* obj);
	}
}
/**
 * setOrder Метод установки размер n-граммы
 * @param order размер n-граммы
 */
void anyks::Collector::setOrder(const u_short order) noexcept {
	// Устанавливаем общий размер n-граммы
	this->order = order;
}
/**
 * setDebug Метод установки флага отладки
 * @param debug флаг отладки
 */
void anyks::Collector::setDebug(const u_short debug) noexcept {
	// Устанавливаем флаг отладки
	this->debug = debug;
}
/**
 * setIntermed Метод установки флага вывода промежуточных результатов
 * @param flag значение флага для устаноки
 */
void anyks::Collector::setIntermed(const bool flag) noexcept {
	// Устанавливаем флаг вывода промежуточных результатов
	this->intermed = flag;
}
/**
 * setToolkit Метод установки объекта тулкита
 * @param toolkit объект тулкита
 */
void anyks::Collector::setToolkit(toolkit_t * toolkit) noexcept {
	// Устанавливаем объект тулкита
	this->toolkit = toolkit;
}
/**
 * setLogfile Метод установки файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::Collector::setLogfile(const char * logfile) noexcept {
	// Устанавливаем адрес log файла
	this->logfile = logfile;
}
/**
 * setThreads Метод установки количества потоков
 * @param threads количество потоков для работы
 */
void anyks::Collector::setThreads(const size_t threads) noexcept {
	// Устанавливаем новое количество потоков
	this->threads = (threads > 0 ? threads : thread::hardware_concurrency());
}
/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита
 */
void anyks::Collector::setAlphabet(const alphabet_t * alphabet) noexcept {
	// Устанавливаем объект алфавита
	this->alphabet = alphabet;
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer объект токенизатора
 */
void anyks::Collector::setTokenizer(const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем объект токенизатора
	this->tokenizer = tokenizer;
}
/**
 * setSegment Метод установки параметров сегментации
 * @param segments разрешить выполнять сегментацию файлов
 * @param size     размер каждого сегмента (auto - получить размер автоматически)
 */
void anyks::Collector::setSegment(const bool segments, const string & size) noexcept {
	// Если размер сегмента передан
	if(!size.empty() && (size.compare("auto") != 0)){
		// Получаем размер в байтах
		const long segment = this->getSize(size);
		// Устанавливаем размер сегмента
		if(segment > -1) this->segmentSize = segment;
	}
	// Активируем флаг сегментации
	this->segments = segments;
}
/**
 * readFile Метод запуска чтение текстового корпуса из одного файла
 * @param filename адрес файла для чтения
 */
void anyks::Collector::readFile(const string & filename) noexcept {
	// Если адрес файла передан
	if((this->toolkit != nullptr) && !filename.empty() && fsys_t::isfile(filename)){
		// Получаем размер файла
		this->dataSize = fsys_t::fsize(filename);
		// Если размер файла получен
		if(this->dataSize > 0){
			// Количество файлов
			size_t count = 0;
			// Размер собранных данных
			uintmax_t size = 0;
			// Список строк для обработки
			vector <string> text = {};
			// Создаём каталог
			const string & dir = this->createDir();
			// Если отладка включена, выводим индикатор загрузки
			if(this->debug > 0){
				// Сбрасываем общий размер собранных данных
				this->allSize = 0;
				// Очищаем предыдущий прогресс-бар
				this->pss.clear();
				// Устанавливаем название файла
				this->pss.description(filename);
				// Устанавливаем заголовки прогресс-бара
				this->pss.title("Read text corpora", "Read text corpora is done");
				// Выводим индикатор прогресс-бара
				switch(this->debug){
					case 1: this->pss.update(); break;
					case 2: this->pss.status(); break;
				}
			}
			// Создаём тредпул
			this->start();
			// Если нужно произвести сегментацию файла
			if(this->segments && (this->dataSize > this->segmentSize)){
				// Если размер сегмента нулевой
				if(this->segmentSize == 0) this->segmentSize = ceil(this->dataSize / double(this->threads));
				// Выполняем считывание всех строк текста
				fsys_t::rfile(filename, [&count, &size, &text, &dir, this](const string & str, const uintmax_t fileSize) noexcept {
					// Если текст получен
					if(!str.empty()){
						// Формируем блок собранных данных
						size += str.size();
						// Если собранных данных достаточно, добавляем данные в поток
						if(size >= this->segmentSize){
							// Очищаем размер собранных данных
							size = 0;
							// Выполняем обучение полученного текста
							this->train(dir + "/alm", text, count);
							// Увеличиваем количество файлов
							count++;
						}
						// Добавляем в список текстов полученный текст
						text.push_back(str);
					}
				});
				// Выполняем обучение полученного текста
				this->train(dir + "/alm", text, count);
			// Если сегментация файла не нужна
			} else {
				// Выполняем считывание всех строк текста
				fsys_t::rfile(filename, [&text](const string & str, const uintmax_t fileSize) noexcept {
					// Если текст получен
					if(!str.empty()) text.push_back(str);
				});
				// Выполняем обучение полученного текста
				this->train(dir + "/alm", text, 0);
			}
			// Завершаем работу тредпула
			this->finish();
			// Отображаем ход процесса
			switch(this->debug){
				case 1: this->pss.update(100); break;
				case 2: this->pss.status(100); break;
			}
		}
	}
}
/**
 * readDir Метод запуска чтение текстового корпуса из каталога
 * @param path адрес каталога для чтения
 * @param ext  расширение файла для поиска в каталоге
 */
void anyks::Collector::readDir(const string & path, const string & ext) noexcept {
	// Если адрес каталога передан
	if((this->toolkit != nullptr) && !path.empty() && !ext.empty() && fsys_t::isdir(path)){
		// Получаем размер файла
		this->dataSize = fsys_t::fcount(path, ext);
		// Если размер файла получен
		if(this->dataSize > 0){
			// Количество файлов
			size_t count = 0;
			// Список строк файлов для обработки
			vector <string> filenames = {};
			// Создаём каталог
			const string & dir = this->createDir();
			// Если отладка включена, выводим индикатор загрузки
			if(this->debug > 0){
				// Сбрасываем общий размер собранных данных
				this->allSize = 0;
				// Очищаем предыдущий прогресс-бар
				this->pss.clear();
				// Устанавливаем заголовки прогресс-бара
				this->pss.title("Read text corpora", "Read text corpora is done");
				// Выводим индикатор прогресс-бара
				switch(this->debug){
					case 1: this->pss.update(); break;
					case 2: this->pss.status(); break;
				}
			}
			// Создаём тредпул
			this->start();
			// Если нужно произвести сегментацию файла
			if(this->segments && (this->dataSize > this->segmentSize)){
				// Если размер сегмента нулевой
				if(this->segmentSize == 0) this->segmentSize = ceil(this->dataSize / double(this->threads));
				// Переходим по всему списку файлов в каталоге
				fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
					// Увеличиваем идентификатор документа
					count++;
					// Если собранных данных достаточно, добавляем данные в поток
					if(count >= this->segmentSize){
						// Очищаем размер собранных данных
						count = 0;
						// Выполняем обучение полученного текста
						this->train(dir + "/alm", filenames);
					}
					// Добавляем в список файлов полученный файл
					filenames.push_back(filename);
				});
				// Выполняем обучение полученного текста
				this->train(dir + "/alm", filenames);
			// Если сегментация файла не нужна
			} else {
				// Переходим по всему списку файлов в каталоге
				fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
					// Выполняем обучение полученного текста
					this->train(dir + "/alm", filename, count);
					// Увеличиваем идентификатор документа
					count++;
				});
			}
			// Завершаем работу тредпула
			this->finish();
			// Отображаем ход процесса
			switch(this->debug){
				case 1: this->pss.update(100); break;
				case 2: this->pss.status(100); break;
			}
		}
	}
}
/**
 * Конструктор
 */
anyks::Collector::Collector(){
	// Выполняем инициализацию питона
	this->initPython();
}
/**
 * Конструктор
 * @param toolkit  объект тулкита
 * @param alphabet объект алфавита
 * @param alphabet объект токенизатора
 * @param logifle  адрес файла для вывода отладочной информации
 */
anyks::Collector::Collector(toolkit_t * toolkit, const alphabet_t * alphabet, const tokenizer_t * tokenizer, const char * logfile){
	// Выполняем инициализацию питона
	this->initPython();
	// Устанавливаем адрес файла для логирования
	this->setLogfile(logfile);
	// Устанавливаем объект тулкита
	this->setToolkit(toolkit);
	// Устанавливаем объект алфавита
	this->setAlphabet(alphabet);
	// Устанавливаем объект токенизатора
	this->setTokenizer(tokenizer);
}
/**
 * Деструктор
 */
anyks::Collector::~Collector() noexcept {
	// Удаляем объект питона
	if(this->python != nullptr) delete this->python;
}

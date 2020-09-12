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
 * dumpRaw Метод дампа промежуточных данных
 */
void anyks::Collector::dumpRaw(){
	// Если нужно вывести промежуточные результаты
	if(this->intermed && (this->toolkit != nullptr)){
		// Создаём каталог
		const string & dir = this->createDir();
		// Еали каталог получен
		if(!dir.empty()){
			// Получаем адрес файла карты последовательностей
			const string & fileMap = this->alphabet->format("%s/raw.map", dir.c_str());
			// Получаем адрес файла словаря собранных слов
			const string & fileVocab = this->alphabet->format("%s/raw.vocab", dir.c_str());
			// Если отладка включена, выводим индикатор загрузки
			if((this->debug > 0) && (this->progressFn == nullptr)){
				// Очищаем предыдущий прогресс-бар
				this->pss.clear();
				// Устанавливаем название файла
				this->pss.description(fileMap);
				// Устанавливаем заголовки прогресс-бара
				this->pss.title("Write raw map", "Write raw map is done");
				// Выводим индикатор прогресс-бара
				switch(this->debug){
					case 1: this->pss.update(); break;
					case 2: this->pss.status(); break;
				}
			}
			// Выполняем сохранение каты последовательности
			this->toolkit->writeMap(fileMap, [this](const u_short status) noexcept {
				// Если внешний прогресс-бар отключен
				if(this->progressFn == nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss.update(status); break;
						case 2: this->pss.status(status); break;
					}
				// Выводим сообщение во внешний прогресс-бар
				} else this->progressFn(L"Write raw map", status);
			});
			// Если внешний прогресс-бар отключен
			if(this->progressFn == nullptr){
				// Отображаем ход процесса
				switch(this->debug){
					case 1: this->pss.update(100); break;
					case 2: this->pss.status(100); break;
				}
			// Выводим сообщение во внешний прогресс-бар
			} else this->progressFn(L"Write raw map", 100);
			// Если отладка включена, выводим индикатор загрузки
			if((this->debug > 0) && (this->progressFn == nullptr)){
				// Очищаем предыдущий прогресс-бар
				this->pss.clear();
				// Устанавливаем название файла
				this->pss.description(fileVocab);
				// Устанавливаем заголовки прогресс-бара
				this->pss.title("Write raw vocab", "Write raw vocab is done");
				// Выводим индикатор прогресс-бара
				switch(this->debug){
					case 1: this->pss.update(); break;
					case 2: this->pss.status(); break;
				}
			}
			// Выполняем извлечение словаря в файл
			this->toolkit->writeVocab(fileVocab, [this](const u_short status) noexcept {
				// Если внешний прогресс-бар отключен
				if(this->progressFn == nullptr){
					// Отображаем ход процесса
					switch(this->debug){
						case 1: this->pss.update(status); break;
						case 2: this->pss.status(status); break;
					}
				// Выводим сообщение во внешний прогресс-бар
				} else this->progressFn(L"Write raw vocab", status);
			});
			// Если внешний прогресс-бар отключен
			if(this->progressFn == nullptr){
				// Отображаем ход процесса
				switch(this->debug){
					case 1: this->pss.update(100); break;
					case 2: this->pss.status(100); break;
				}
			// Выводим сообщение во внешний прогресс-бар
			} else this->progressFn(L"Write raw vocab", 100);
		}
	}
}
/**
 * initPython Метод инициализации модуля питона
 */
void anyks::Collector::initPython(){
	// Если объект питона еще не создан
	if((this->python == nullptr) && !this->nopython){
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
 * allowPython Метод разрешения использования объекта Python
 */
void anyks::Collector::allowPython(){
	// Разрешаем использовать объект Python
	this->nopython = false;
}
/**
 * disallowPython Метод запрещения использования объекта Python
 */
void anyks::Collector::disallowPython(){
	// Запрещаем использовать объект Python
	this->nopython = true;
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
		// Если пункт назначения указан
		if(!this->destination.empty()){
			// Получаем данные строки
			string * dir = const_cast <string *> (&dirName);
			// Устанавливаем адрес назначения
			dir->append(this->destination);
		// Если пункт назначения не указан
		} else {
			// Создаем буфер для хранения даты
			char date[80];
			// Добавляем в название каталога штамп времени
			// Получаем текущее время
			time_t timeStamp = time(nullptr);
			// Заполняем его нулями
			memset(date, 0, sizeof(date));
			// Получаем структуру локального времени
			struct tm * timeinfo = localtime(&timeStamp);
			// Копируем в буфер полученную дату и время
			const int length = strftime(date, sizeof(date), "%m-%d-%Y_%H-%M-%S", timeinfo);
			// Если дата создана
			if(length > 0){
				// Получаем данные строки
				string * dir = const_cast <string *> (&dirName);
				// Добавляем префикс каталога
				dir->append("./alm_");
				// Добавляем дату в адрес каталога
				dir->append(date, length);
			}
		}
		// Если каталога не существует, создаём его
		if(!dirName.empty() && !fsys_t::isdir(dirName)) fsys_t::mkdir(dirName);
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
 * @param filename файл для чтения
 * @param idd      идентификатор документа
 */
void anyks::Collector::train(const string & filename, const size_t idd) noexcept {
	// Если текст передан
	if(!filename.empty() && (this->tpool != nullptr)){
		// Добавляем в тредпул новое задание на обработку
		this->tpool->push([this](const string filename, const size_t idd){
			// Получаем копию объекта тулкита
			toolkit_t toolkit(this->alphabet, this->tokenizer, this->order);
			// Устанавливаем log файл
			toolkit.setLogfile(this->logfile);
			// Устанавливаем неизвестное слово
			toolkit.setUnknown(this->toolkit->getUnknown());
			// Устанавливаем опции тулкита
			toolkit.setOptions(this->toolkit->getOptions());
			// Устанавливаем скрипт препроцессинга слов
			toolkit.setWordScript(this->toolkit->getWordScript());
			// Устанавливаем внешний объект питона
			if(!this->nopython) toolkit.setPythonObj(this->python);
			// Устанавливаем список токенов приводимых к <unk>
			toolkit.setTokensUnknown(this->toolkit->getTokensUnknown());
			// Устанавливаем список запрещённых токенов
			toolkit.setTokensDisable(this->toolkit->getTokensDisable());
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
					// Если внешний прогресс-бар отключен
					if(this->progressFn == nullptr){
						// Устанавливаем название файла
						this->pss.description(filename);
						// Отображаем ход процесса
						switch(this->debug){
							case 1: this->pss.update(this->status); break;
							case 2: this->pss.status(this->status); break;
						}
					// Выводим сообщение во внешний прогресс-бар
					} else this->progressFn(L"Read text corpora", this->status);
					// Разблокируем поток
					this->locker.unlock();
				}
			}
		}, filename, idd);
	}
}
/**
 * train Обучения полученного текста
 * @param texts список строк текста для обучения
 * @param idd   идентификатор документа
 */
void anyks::Collector::train(const vector <string> & texts, const size_t idd) noexcept {
	// Если тексты переданы
	if(!texts.empty() && (this->tpool != nullptr)){
		// Добавляем в тредпул новое задание на обработку
		this->tpool->push([this](const vector <string> texts, const size_t idd){
			// Получаем копию объекта тулкита
			toolkit_t toolkit(this->alphabet, this->tokenizer, this->order);
			// Устанавливаем log файл
			toolkit.setLogfile(this->logfile);
			// Устанавливаем неизвестное слово
			toolkit.setUnknown(this->toolkit->getUnknown());
			// Устанавливаем опции тулкита
			toolkit.setOptions(this->toolkit->getOptions());
			// Устанавливаем скрипт препроцессинга слов
			toolkit.setWordScript(this->toolkit->getWordScript());
			// Устанавливаем внешний объект питона
			if(!this->nopython) toolkit.setPythonObj(this->python);
			// Устанавливаем список токенов приводимых к <unk>
			toolkit.setTokensUnknown(this->toolkit->getTokensUnknown());
			// Устанавливаем список запрещённых токенов
			toolkit.setTokensDisable(this->toolkit->getTokensDisable());
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
			for(auto & text : texts){
				// Если текст не пустой
				if(!text.empty()){
					// Добавляем полученную строку текста
					toolkit.addText(text, idd);
					// Если отладка включена, выводим индикатор загрузки
					if(this->debug > 0){
						// Общий полученный размер данных
						this->allSize.store(this->allSize + text.size(), memory_order_relaxed);
						// Подсчитываем статус выполнения
						this->status = u_short(this->allSize / double(this->dataSize) * 100.0);
						// Если процентное соотношение изменилось
						if(this->rate != this->status){
							// Запоминаем текущее процентное соотношение
							this->rate.store(this->status, memory_order_relaxed);
							// Блокируем поток
							this->locker.lock();
							// Если внешний прогресс-бар отключен
							if(this->progressFn == nullptr){
								// Отображаем ход процесса
								switch(this->debug){
									case 1: this->pss.update(this->status); break;
									case 2: this->pss.status(this->status); break;
								}
							// Выводим сообщение во внешний прогресс-бар
							} else this->progressFn(L"Read text corpora", this->status);
							// Разблокируем поток
							this->locker.unlock();
						}
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
			toolkit.saveArpa([idd, this](const vector <char> & buffer, const u_short status){
				// Загружаем данные n-граммы
				this->toolkit->appendArpa(buffer, idd);
			});
			// Разблокируем поток
			this->locker.unlock();
		}, texts, idd);
		// Получаем объект текста
		vector <string> * obj = const_cast <vector <string> *> (&texts);
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
 * setDest Метод установки место назначения для сохранения сырых данных
 * @param destination местоназначения для сырых данных
 */
void anyks::Collector::setDest(const string & destination) noexcept {
	// Выполняем установку мета назначения для сохранения сырых данных
	if(!destination.empty()) this->destination = destination;
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
 * setProgressFn Метод установки внешнего прогресс-бара
 * @param fn функция внешнего прогресс-бара
 */
void anyks::Collector::setProgressFn(function <void (const wstring &, const u_short)> fn) noexcept {
	// Устанавливаем функцию внешнего прогресс-бара
	this->progressFn = fn;
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
			// Список текстов для обработки
			vector <string> texts;
			// Если отладка включена, выводим индикатор загрузки
			if((this->debug > 0) && (this->progressFn == nullptr)){
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
				fsys_t::rfile(filename, [&count, &size, &texts, this](const string & text, const uintmax_t fileSize) noexcept {
					// Если текст получен
					if(!text.empty()){
						// Формируем блок собранных данных
						size += text.size();
						// Если собранных данных достаточно, добавляем данные в поток
						if(size >= this->segmentSize){
							// Очищаем размер собранных данных
							size = 0;
							// Выполняем обучение полученного текста
							this->train(texts, count);
							// Увеличиваем количество файлов
							count++;
						}
						// Добавляем в список текстов полученный текст
						texts.push_back(text);
					}
				});
				// Выполняем обучение полученного текста
				this->train(texts, count);
			// Если сегментация файла не нужна
			} else {
				// Выполняем считывание всех строк текста
				fsys_t::rfile(filename, [&texts](const string & text, const uintmax_t fileSize) noexcept {
					// Если текст получен
					if(!text.empty()) texts.push_back(text);
				});
				// Выполняем обучение полученного текста
				this->train(texts, 0);
			}
			// Завершаем работу тредпула
			this->finish();
			// Если внешний прогресс-бар отключен
			if(this->progressFn == nullptr){
				// Отображаем ход процесса
				switch(this->debug){
					case 1: this->pss.update(100); break;
					case 2: this->pss.status(100); break;
				}
			// Выводим сообщение во внешний прогресс-бар
			} else this->progressFn(L"Read text corpora", 100);
			// Выполняем дамп промежуточных данных
			this->dumpRaw();
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
		// Количество файлов
		size_t count = 0;
		// Если отладка включена, выводим индикатор загрузки
		if((this->debug > 0) && (this->progressFn == nullptr)){
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
		// Получаем размер файлов в каталоге
		if(this->segments) this->dataSize = fsys_t::dsize(path, ext);
		// Если нужно произвести сегментацию файла
		if(this->segments && (this->dataSize > this->segmentSize)){
			// Размер собранных данных
			uintmax_t size = 0;
			// Список текстов для обработки
			vector <string> texts;
			// Если размер сегмента нулевой
			if(this->segmentSize == 0) this->segmentSize = ceil(this->dataSize / double(this->threads));
			// Переходим по всему списку файлов в каталоге
			fsys_t::rdir(path, ext, [&count, &size, &texts, this](const string & filename, const uintmax_t dirSize) noexcept {
				// Устанавливаем название файла
				if(this->progressFn == nullptr) this->pss.description(filename);
				// Выполняем считывание всех строк текста
				fsys_t::rfile2(filename, [&count, &size, &texts, this](const string & text, const uintmax_t fileSize) noexcept {
					// Если текст получен
					if(!text.empty()){
						// Формируем блок собранных данных
						size += text.size();
						// Если собранных данных достаточно, добавляем данные в поток
						if(size >= this->segmentSize){
							// Очищаем размер собранных данных
							size = 0;
							// Выполняем обучение полученного текста
							this->train(texts, count);
							// Увеличиваем количество файлов
							count++;
						}
						// Добавляем в список текстов полученный текст
						texts.push_back(text);
					}
				});
			});
			// Выполняем обучение полученного текста
			this->train(texts, count);
		// Если сегментация файла не нужна
		} else {
			// Получаем размер файла
			this->dataSize = fsys_t::fcount(path, ext);
			// Если количество файлов в каталоге, больше 0
			if(this->dataSize > 0){
				// Переходим по всему списку файлов в каталоге
				fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
					// Выполняем обучение полученного текста
					this->train(filename, count);
					// Увеличиваем идентификатор документа
					count++;
				});
			}
		}
		// Завершаем работу тредпула
		this->finish();
		// Если внешний прогресс-бар отключен
		if(this->progressFn == nullptr){
			// Отображаем ход процесса
			switch(this->debug){
				case 1: this->pss.update(100); break;
				case 2: this->pss.status(100); break;
			}
		// Выводим сообщение во внешний прогресс-бар
		} else this->progressFn(L"Read text corpora", 100);
		// Выполняем дамп промежуточных данных
		this->dumpRaw();
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
	if((this->python != nullptr) && !this->nopython) delete this->python;
}

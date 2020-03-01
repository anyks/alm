/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <collector.hpp>

/**
 * setToolkit Метод установки объекта тулкита
 * @param toolkit объект тулкита
 */
void anyks::Collector::setToolkit(toolkit_t * toolkit){
	// Устанавливаем объект тулкита
	this->toolkit = toolkit;
}
/**
 * setThreads Метод установки количества потоков
 * @param threads количество потоков для работы
 */
void anyks::Collector::setThreads(const size_t threads){
	// Устанавливаем новое количество потоков
	this->threads = (threads > 0 ? threads : thread::hardware_concurrency());
}
/**
 * readFile Метод запуска чтение текстового корпуса из одного файла
 * @param filename адрес файла для чтения
 * @param status   функция вывода статуса обучения
 */
void anyks::Collector::readFile(const string & filename, function <void (const u_short)> status){
	// Если адрес файла передан
	if((this->toolkit != nullptr) && !filename.empty() && fsys_t::isfile(filename)){
		// Список строк для обработки
		list <string> texts;
		// Общий размер собранных данных
		atomic <size_t> size(0);
		// Статус и процентное соотношение
		atomic <u_short> actual(0), rate(101);
		// Общий размер файла
		size_t allSize = 0, currentSize = 0;
		/**
		 * tokensFn Функция разбивки текста на токены
		 * @param texts список текстов для обработки
		 */
		auto tokensFn = [&](const list <string> texts){
			// Переходим по всему списку текстов
			for(auto & text : texts){
				// Выполняем разбивку текста на токены
				this->toolkit->addText(text, 0);
				// Если отладка включена
				if(status != nullptr){
					// Общий полученный размер данных
					size.store(size + text.size(), memory_order_relaxed);
					// Подсчитываем статус выполнения
					actual.store(u_short(size / float(allSize) * 100.0f), memory_order_relaxed);
					// Если процентное соотношение изменилось
					if(rate != actual){
						// Запоминаем текущее процентное соотношение
						rate.store(actual, memory_order_relaxed);
						// Отображаем ход процесса
						status(actual);
					}
				}
			}
		};
		/**
		 * setTextFn Функция запуска текста для обработки
		 */
		auto setTextFn = [&]{
			// Сбрасываем счётчик строк
			currentSize = 0;
			// Если список текстов не пустой
			if(!texts.empty()){
				// Добавляем в список новый поток
				this->tpool.push(tokensFn, texts);
				// Очищаем список текстов
				texts.clear();
				// Очищаем выделенную память
				list <string> ().swap(texts);
			}
		};
		// Выполняем инициализацию тредпула
		this->tpool.init(this->threads);
		// Выполняем считывание всех строк текста
		fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize){
			// Запоминаем размер файла
			allSize = fileSize;
			// Если текст получен
			if(!text.empty()){
				// Формируем блок собранных данных
				currentSize += text.size();
				// Если собранных данных достаточно, добавляем данные в поток
				if(currentSize >= ceil(fileSize / float(this->threads))) setTextFn();
				// Добавляем в список текстов полученный текстs
				texts.push_back(text);
			}
		});
		// Выполняем добавление на обработку оставшуюся порцию данных
		setTextFn();
		// Выполняем обработки всех данных
		this->tpool.wait();
	}
}
/**
 * readDir Метод запуска чтение текстового корпуса из каталога
 * @param path   адрес каталога для чтения
 * @param ext    расширение файла для поиска в каталоге
 * @param status функция вывода статуса обучения
 */
void anyks::Collector::readDir(const string & path, const string & ext, function <void (const string &, const u_short)> status){
	// Если адрес каталога передан
	if((this->toolkit != nullptr) && !path.empty() && !ext.empty() && fsys_t::isdir(path)){
		// Статус и процентное соотношение
		u_short actual = 0, rate = 101;
		// Идентификатор документа
		size_t idd = 0, size = 0, maxSize = 0;
		/**
		 * tokensFn Функция разбивки текста на токены
		 * @param filename адрес файла для обработки
		 * @param text     текст для обработки
		 */
		auto tokensFn = [&](const string & filename, const string & text){
			// Выполняем разбивку текста на токены
			this->toolkit->addText(text, idd);
			// Если отладка включена
			if(status != nullptr){
				// Общий полученный размер данных
				size += text.size();
				// Подсчитываем статус выполнения
				actual = u_short(size / float(maxSize) * 100.0f);
				// Если процентное соотношение изменилось
				if(rate != actual){
					// Запоминаем текущее процентное соотношение
					rate = actual;
					// Отображаем ход процесса
					status(filename, actual);
				}
			}
		};
		// Выполняем инициализацию тредпула
		this->tpool.init(this->threads);
		// Переходим по всему списку файлов в каталоге
		fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize){
			// Выполняем считывание всех строк текста
			fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize){
				// Запоминаем размер файла
				maxSize = dirSize;
				// Добавляем в список новый поток
				if(!text.empty()) this->tpool.push(tokensFn, filename, text);
			});
			// Увеличиваем идентификатор документа
			idd++;
		});
		// Выполняем обработки всех данных
		this->tpool.wait();
	}
}
/**
 * Конструктор
 * @param toolkit объект тулкита
 */
anyks::Collector::Collector(toolkit_t * toolkit){
	// Устанавливаем объект тулкита
	this->setToolkit(toolkit);
}
/**
 * Конструктор
 * @param toolkit объект тулкита
 */
anyks::Collector::Collector(const size_t threads){
	// Устанавливаем количество ядер используемых в работе
	this->setThreads(threads);
}
/**
 * Конструктор
 * @param threads количество потоков для работы
 * @param toolkit объект тулкита
 */
anyks::Collector::Collector(const size_t threads, toolkit_t * toolkit){
	// Устанавливаем количество ядер используемых в работе
	this->setThreads(threads);
	// Устанавливаем объект тулкита
	this->setToolkit(toolkit);
}

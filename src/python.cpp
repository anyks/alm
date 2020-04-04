/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <python.hpp>

/**
 * clear Метод очистки списка скриптов
 */
void anyks::Python::clear() noexcept {
	// Переходим по всем собранным скриптам
	for(auto & script : this->scripts){
		// Выполняем очистику всех параметров
		script.second.count = 0;
		Py_DECREF(script.second.run);
		Py_DECREF(script.second.init);
		Py_DECREF(script.second.module);
	}
	// Выполняем очистику всего списка
	this->scripts.clear();
}
/**
 * setDebug Метод установки режима отладки
 */
void anyks::Python::setDebug() noexcept {
	// Устанавливаем режим отладки
	this->debug = true;
}
/**
 * unsetDebug Метод отключения режима отладки
 */
void anyks::Python::unsetDebug() noexcept {
	// Отключаем режим отладки
	this->debug = false;
}
/**
 * remove Метод удаления добавленного скрипта по его имени
 * @param sid идентификатор скрипта
 */
void anyks::Python::remove(const size_t sid) noexcept {
	// Если имя скрипта передано
	if(sid > 0){
		// Извлекаем скрипт
		auto it = this->scripts.find(sid);
		// Если скрипт найден
		if(it != this->scripts.end()){
			// Выполняем очистику всех параметров
			it->second.count = 0;
			Py_DECREF(it->second.run);
			Py_DECREF(it->second.init);
			Py_DECREF(it->second.module);
			// Удаляем найденный скрипт
			this->scripts.erase(it);
		}
	}
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer указатель на токенизатор
 */
void anyks::Python::setTokenizer(const tokenizer_t * tokenizer) noexcept {
	// Устанавливаем модуль генератора
	this->tokenizer = tokenizer;
}
/**
 * empty Проверка на пустое количество скриптов
 * @return результат проверки
 */
const bool anyks::Python::empty() const noexcept {
	// Выводим результат
	return this->scripts.empty();
}
/**
 * count Метод определения количества добавленных скриптов
 * @return количество добавленных скриптов
 */
const size_t anyks::Python::count() const noexcept {
	// Выводим результат
	return this->scripts.size();
}
/**
 * add Метод добавления скрипта
 * @param script адрес скрипта
 * @param args   количество аргументов передаваемых в функцию запуска
 * @return       идентификатор добавленного скрипта
 */
const size_t anyks::Python::add(const string & script, const u_short args) noexcept {
	// Результат работы функции
	size_t result = 0;
	// Если скрипт передан
	if(!script.empty()){
		// Получаем путь до скрипта
		const char * file = realpath(script.c_str(), nullptr);
		// Получаем путь до скрипта
		const string & filename = (file != nullptr ? file : "");
		// Если путь не пустой
		if(!filename.empty() && fsys_t::isfile(filename)){
			// Выполняем поиск названия файла
			const size_t pos = filename.rfind("/");
			// Если позиция найдена
			if(pos != string::npos){
				// Получаем путь до файла
				const string & path = filename.substr(0, pos);
				// Получаем имя файла
				const string & file = filename.substr(pos + 1, filename.length() - 1);
				// Если и путь и файл получены, извлекаем название файла
				if(!path.empty() && !file.empty()){
					// Название файла
					string name = "";
					// Ищем название файла
					const size_t pos = file.rfind(".");
					// Если точка найдена
					if(pos != string::npos) name = file.substr(0, pos);
					// Иначе запоминаем скрипт как он есть
					else name = move(file);
					// Получаем идентификатор скрипта
					result = this->tokenizer->idw(alphabet_t().convert(name));
					// Если название скрипта получено
					if(!name.empty() && (this->scripts.count(result) < 1)){
						// Создаем объект скрипта
						script_t script;
						// Получаем объект системного пути
						PyObject * sysPath = PySys_GetObject("path");
						// Устанавливаем адрес хранения скрипта
						PyObject * scriptPath = PyUnicode_FromString(path.c_str());
						// Добавляем путь скрипта в список путей
						PyList_Append(sysPath, scriptPath);
						// Очищаем адрес хранения скрипта
						Py_DECREF(scriptPath);
						// Получаем имя скрипта
						PyObject * scriptName = PyUnicode_DecodeFSDefault(name.c_str());
						// Выполняем импорт модуля
						script.module = PyImport_Import(scriptName);
						// Удаляем объект системного пути
						// Py_DECREF(sysPath);
						// Удаляем объект имени скрипта
						Py_DECREF(scriptName);
						// Если модуль скрипта получен
						if(script.module != nullptr){
							// Создаем метод запуска скрипта
							script.run = PyObject_GetAttrString(script.module, "run");
							// Создаем метод инициализации скрипта
							script.init = PyObject_GetAttrString(script.module, "init");
							// Если функция инициализации существует
							if(script.init != nullptr){
								// Если метод существует
								if(PyCallable_Check(script.init)){
									// Создаем объект аргументов
									PyObject * args = PyTuple_New(0);
									// Вызываем функцию инициализации
									PyObject * value = PyObject_CallObject(script.init, args);
									// Если объект аргументов существует, очищаем его
									if(args != nullptr) Py_DECREF(args);
									// Если объект исходящего значения существует, очищаем его
									if(value != nullptr) Py_DECREF(value);
								}
								// Если функция запуска скрипта, создана
								if(script.run != nullptr){
									// Если функция запуска, найдена
									if(PyCallable_Check(script.run)){
										// Запоминаем сколько всего было передано аргументов
										script.count = args;
										// Добавляем в список очередной скрипт
										this->scripts.emplace(result, move(script));
									// Если функция запуска не найдена
									} else {
										// Очищаем функцию запуска скрипта
										Py_DECREF(script.run);
										// Очищаем функцию инициализации
										Py_DECREF(script.init);
										// Очищаем модуль скрипта
										Py_DECREF(script.module);
										// Выводим сообщение в консоль, что функция не найдена
										if(this->debug) cerr << "python - cannot find function: run" << endl;
									}
								// Если функция запуска скрипта не создана
								} else {
									// Выводим сообщение об ошибке
									if(PyErr_Occurred()) PyErr_Print();
									// Выводим сообщение в консоль, что функция не найдена
									if(this->debug) cerr << "python - cannot check function: run" << endl;
								}
							// Если функция инициализации не существует
							} else {
								// Если произошла ошибка выводим её
								if(PyErr_Occurred()) PyErr_Print();
								// Выводим сообщение в консоль, что функция не найдена
								if(this->debug) cerr << "python - cannot check function: init" << endl;
							}
						// Если скрипт не загружен
						} else {
							// Выводим ошибку скрипта
							PyErr_Print();
							// Выводим сообщение о не существовании скрипта
							if(this->debug) cerr << "python - failed to load file: " << file << endl;
						}
					}
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * run Метод запуска скрипта
 * @param  sid  идентификатор скрипта
 * @param  args список аргументов передаваемых в скрипт
 * @param  arr  массив аргументов для передачи в скрипт
 * @return      результат выполнения скрипта
 */
const wstring anyks::Python::run(const size_t sid, const vector <string> & args, const vector <string> & arr) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если имя передано
	if(sid > 0){
		// Выполняем поиск скрипта
		auto it = this->scripts.find(sid);
		// Если скрипт найден
		if(it != this->scripts.end()){
			// Извлекаем скрипт
			auto script = it->second;
			// Аргументы скрипта
			PyObject * scriptArgs = nullptr;
			// Если ожидаются входные аргументы
			if(script.count > 0){
				// Если количество аргументов совпадает
				if(script.count == u_short(args.size() + (!arr.empty() ? 1 : 0))){
					// Устанавливаем количество аргументов
					scriptArgs = PyTuple_New(script.count);
					// Если аргументы переданы
					if(!args.empty()){
						// Переходим по всему списку аргументов
						for(size_t i = 0; i < args.size(); i++){
							// Получаем передаваемое значение
							PyObject * value = PyUnicode_FromString(args[i].c_str());
							// Если значение получено
							if(value != nullptr) PyTuple_SetItem(scriptArgs, i, value);
							// Если значение не получено
							else {
								// Сообщаем об ошибке
								if(this->debug) cerr << "python - cannot convert argument" << endl;
								// Выходим из цикла
								break;
							}
						}
					}
					// Если массив слов передан
					if(!arr.empty()){
						// Создаем объект списка
						PyObject * value = PyList_New(arr.size());
						// Переходим по всему массиву слов
						for(size_t i = 0; i < arr.size(); i++){
							// Добавляем в список все аргументы
							PyList_SetItem(value, i, PyUnicode_FromString(arr[i].c_str()));
						}
						// Если значение получено
						if(value != nullptr) PyTuple_SetItem(scriptArgs, args.size(), value);
						// Если значение не получено
						else {
							// Сообщаем об ошибке
							if(this->debug) cerr << "python - cannot convert argument" << endl;
							// Выходим из метода
							return result;
						}
					}
				// Если количество аргументов не совпадает
				} else {
					// Сообщаем об ошибке
					if(this->debug) cerr << "python - broken count arguments" << endl;
					// Выходим из метода
					return result;
				}
			// Если аргументы не ожидаются
			} else scriptArgs = PyTuple_New(0);
			// Выполняем запуск функции
			PyObject * value = PyObject_CallObject(script.run, scriptArgs);
			// Очищаем объект аргументов
			Py_DECREF(scriptArgs);
			// Если значение получено
			if(value != nullptr){
				// Размер считываемых данных
				Py_ssize_t size = 0;
				// Получаем результат
				const wchar_t * res = PyUnicode_AsWideCharString(value, &size);
				// Получаем результат
				if(res != nullptr) result = wstring(res, size);
				// Очищаем полученное значение
				Py_DECREF(value);
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * Python Конструктор
 */
anyks::Python::Python() noexcept {
	// Выполняем инициализацию питона
	if(!Py_IsInitialized()) Py_Initialize();
}
/**
 * Python Конструктор
 * @param tokenizer указатель на токенизатор
 */
anyks::Python::Python(const tokenizer_t * tokenizer) noexcept {
	// Выполняем инициализацию питона
	if(!Py_IsInitialized()) Py_Initialize();
	// Выполняем установку токенизатора
	this->setTokenizer(tokenizer);
}
/**
 * ~Python Деструктор
 */
anyks::Python::~Python() noexcept {
	// Очищаем список полученных скриптов
	this->clear();
	// Выполняем завершение работы питона
	if(Py_IsInitialized()) Py_Finalize();
}

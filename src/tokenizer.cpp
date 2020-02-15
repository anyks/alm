/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <tokenizer.hpp>

/**
 * setAlphabet Метод установки алфавита
 * @param alphabet объект алфавита
 */
void anyks::Tokenizer::setAlphabet(const alphabet_t * alphabet){
	// Устанавливаем алфавит
	this->alphabet = alphabet;
}
/**
 * restore Метод восстановления текста из контекста
 * @param context токенизированный контекст
 * @return        результирующий текст
 */
const string anyks::Tokenizer::restore(const vector <string> & context) const {
	// Результат работы функции
	string result = "";
	// Если контекст передан
	if(!context.empty() && (this->alphabet != nullptr)){
		// Флаг конца текста
		bool end = false;
		// Собранный текст
		wstring text = L"";
		// Типы флагов
		enum class type_t : u_short {
			end,   // Конец контекста
			num,   // Символ числа в тексте
			null,  // Не определено
			word,  // нормальное слово
			math,  // Математическая операция
			open,  // Открытие изоляционного символа
			close, // Закрытие изоляционного символа
			specl, // Спец-символ в тексте
			space, // Символ пробела в тексте
			allow, // Разрешённый символ
			punct, // Знак препинания
			isolat // Изоляционный символ в строке
		};
		// Стек типов собранного контекста
		stack <type_t> typeContext;
		// Тип текущего токена контекста
		type_t tokenType = type_t::null, nextToken = type_t::null;
		/**
		 * typeFn Функция определения типа токена
		 * @param index индекс токена в контексте
		 * @return      тип токена в контексте
		 */
		auto typeFn = [&context, this](const size_t index = 0){
			// Результат работы функции
			type_t result = type_t::null;
			// Если индекс слова не вышел за границы массива
			if(index < context.size()){
				// Получаем текущий токен
				const wstring & token = this->alphabet->convert(context.at(index));
				// Если длина слова больше 1-го символа, значит это слово
				if(token.length() > 1) result = type_t::word;
				// Если это всего один символ
				else {
					// Получаем символ токена в нижнем регистре
					wchar_t letter = this->alphabet->toLower(token.front());
					// Определяем тип символа
					if(this->alphabet->isNumber({letter})) result = type_t::num;
					// Если это математическая операция
					else if(this->alphabet->isMath(letter)) result = type_t::math;
					// Если это разрешённая буква алфавита
					else if(this->alphabet->check(letter)) result = type_t::allow;
					// Если это символ пробела
					else if(this->alphabet->isSpace(letter)) result = type_t::space;
					// Если это знак пунктуации
					else if(this->alphabet->isPunct(letter)) result = type_t::punct;
					// Если это спец-символ
					else if(this->alphabet->isSpecial(letter)) result = type_t::specl;
					// Если это изоляционный символ
					else if(this->alphabet->isIsolation(letter)) {
						// Определяем тип изоляционных знаков
						const u_short type = (
							(letter == L'(') || (letter == L'[') || (letter == L'{') ||
							(letter == L'«') || (letter == L'„') ? 1 :
							((letter == L')') || (letter == L']') || (letter == L'}') ||
							(letter == L'»') || (letter == L'“') ? 2 : 0)
						);
						// Определяем тип изоляционного символа
						switch(type){
							// Если это просто изоляционный символ
							case 0: result = type_t::isolat; break;
							// Если это изоляционный символ открытия
							case 1: result = type_t::open;   break;
							// Если это изоляционный символ закрытия
							case 2: result = type_t::close;  break;
						}
					}
				}
			// Запоминаем что это конец контекста
			} else result = type_t::end;
			// Выводим результат
			return result;
		};
		// Переходим по всем токенам
		for(size_t i = 0; i < context.size(); ++i){
			// Получаем тип токена
			tokenType = typeFn(i);
			// Определяем является ли это концом предложения
			end = (i == (context.size() - 1));
			// Конвертируем строку
			const wstring & token = this->alphabet->convert(context.at(i));
			// Если это начало предложения
			if(text.empty()) text.append(token);
			// Иначе выполняем основную обработку
			else {
				// Определяем тип токена
				switch((u_short) tokenType){
					// Если это неизвестный символ
					case (u_short) type_t::null:
					// Если это просто слово
					case (u_short) type_t::word:
					// Если это открытый изоляционный символ
					case (u_short) type_t::open:
					// Если это нормальная буква
					case (u_short) type_t::allow: {
						// Если предыдущий символ не является открытым изоляционнмы символом
						if((typeContext.empty() ||
						(typeContext.top() != type_t::open)) &&
						(text.back() != L' ') &&
						(!this->alphabet->isNumber(token.substr(0, 1)) ||
						!this->alphabet->isMath(text.back()) ||
						(text.back() == L'='))) text.append(1, L' ');
						// Добавляем слово
						text.append(token);
					} break;
					// Если это число
					case (u_short) type_t::num:
					// Если это математическая операция
					case (u_short) type_t::math: {
						// Если предыдущий символ не является числом, математической операцией и пробелом
						if((typeContext.empty() ||
						((token.front() == L'=') && (text.back() != L'=')) ||
						((token.front() != L'=') && (text.back() == L'=')) ||
						((typeContext.top() != type_t::num) &&
						(typeContext.top() != type_t::math) &&
						(typeContext.top() != type_t::open) &&
						(typeContext.top() != type_t::space))) &&
						(text.back() != L' ')) text.append(1, L' ');
						// Добавляем слово
						text.append(token);
					} break;
					// Если это спец-символ
					case (u_short) type_t::specl: {
						// Если предыдущий символ не является спец-символом и пробелом
						if((typeContext.empty() ||
						((typeContext.top() != type_t::specl) &&
						(typeContext.top() != type_t::open) &&
						(typeContext.top() != type_t::space))) &&
						(text.back() != L' ')) text.append(1, L' ');
						// Добавляем слово
						text.append(token);
					} break;
					// Если это закрытый изоляционный символ
					case (u_short) type_t::close:
					// Если это знак-пунктуации
					case (u_short) type_t::punct: {
						// Получаем тип токена
						nextToken = typeFn(i + 1);
						// Добавляем слово
						text.append(token);
						// Если следующий символ не является знаком пунктуации
						if((nextToken != type_t::punct) &&
						(nextToken != type_t::end) &&
						(nextToken != type_t::space) &&
						(nextToken != type_t::close)) text.append(1, L' ');
					} break;
					// Если это изоляционный символ
					case (u_short) type_t::isolat: {
						// Получаем тип токена
						nextToken = typeFn(i + 1);
						// Если предыдущий символ не является изоляционным символом
						if((typeContext.empty() ||
						(typeContext.top() != type_t::space)) &&
						(text.back() != L' ')) text.append(1, L' ');
						// Добавляем слово
						text.append(token);
						// Если следующий символ является любым из слов кроме изоляционного символа и конца текста
						if((nextToken != type_t::end) &&
						(nextToken != type_t::space) &&
						(nextToken != type_t::punct)) text.append(1, L' ');
					} break;
				}
			}
			// Добавляем тип токена в контекст токенов
			typeContext.push(tokenType);
		}
		// Формируем результат
		result = move(this->alphabet->convert(text));
	}
	// Выводим результат
	return result;
}
/**
 * restore Метод восстановления текста из контекста
 * @param context токенизированный контекст
 * @return        результирующий текст
 */
const wstring anyks::Tokenizer::restore(const vector <wstring> & context) const {
	// Результат работы функции
	wstring result = L"";
	// Если контекст передан
	if(!context.empty() && (this->alphabet != nullptr)){
		// Флаг конца текста
		bool end = false;
		// Типы флагов
		enum class type_t : u_short {
			end,   // Конец контекста
			num,   // Символ числа в тексте
			null,  // Не определено
			word,  // нормальное слово
			math,  // Математическая операция
			open,  // Открытие изоляционного символа
			close, // Закрытие изоляционного символа
			specl, // Спец-символ в тексте
			space, // Символ пробела в тексте
			allow, // Разрешённый символ
			punct, // Знак препинания
			isolat // Изоляционный символ в строке
		};
		// Стек типов собранного контекста
		stack <type_t> typeContext;
		// Тип текущего токена контекста
		type_t tokenType = type_t::null, nextToken = type_t::null;
		/**
		 * typeFn Функция определения типа токена
		 * @param index индекс токена в контексте
		 * @return      тип токена в контексте
		 */
		auto typeFn = [&context, this](const size_t index = 0){
			// Результат работы функции
			type_t result = type_t::null;
			// Если индекс слова не вышел за границы массива
			if(index < context.size()){
				// Получаем текущий токен
				const wstring & token = context.at(index);
				// Если длина слова больше 1-го символа, значит это слово
				if(token.length() > 1) result = type_t::word;
				// Если это всего один символ
				else {
					// Получаем символ токена в нижнем регистре
					wchar_t letter = this->alphabet->toLower(token.front());
					// Определяем тип символа
					if(this->alphabet->isNumber({letter})) result = type_t::num;
					// Если это математическая операция
					else if(this->alphabet->isMath(letter)) result = type_t::math;
					// Если это разрешённая буква алфавита
					else if(this->alphabet->check(letter)) result = type_t::allow;
					// Если это символ пробела
					else if(this->alphabet->isSpace(letter)) result = type_t::space;
					// Если это знак пунктуации
					else if(this->alphabet->isPunct(letter)) result = type_t::punct;
					// Если это спец-символ
					else if(this->alphabet->isSpecial(letter)) result = type_t::specl;
					// Если это изоляционный символ
					else if(this->alphabet->isIsolation(letter)) {
						// Определяем тип изоляционных знаков
						const u_short type = (
							(letter == L'(') || (letter == L'[') || (letter == L'{') ||
							(letter == L'«') || (letter == L'„') ? 1 :
							((letter == L')') || (letter == L']') || (letter == L'}') ||
							(letter == L'»') || (letter == L'“') ? 2 : 0)
						);
						// Определяем тип изоляционного символа
						switch(type){
							// Если это просто изоляционный символ
							case 0: result = type_t::isolat; break;
							// Если это изоляционный символ открытия
							case 1: result = type_t::open;   break;
							// Если это изоляционный символ закрытия
							case 2: result = type_t::close;  break;
						}
					}
				}
			// Запоминаем что это конец контекста
			} else result = type_t::end;
			// Выводим результат
			return result;
		};
		// Переходим по всем токенам
		for(size_t i = 0; i < context.size(); ++i){
			// Получаем тип токена
			tokenType = typeFn(i);
			// Определяем является ли это концом предложения
			end = (i == (context.size() - 1));
			// Конвертируем строку
			const wstring & token = context.at(i);
			// Если это начало предложения
			if(result.empty()) result.append(token);
			// Иначе выполняем основную обработку
			else {
				// Определяем тип токена
				switch((u_short) tokenType){
					// Если это неизвестный символ
					case (u_short) type_t::null:
					// Если это просто слово
					case (u_short) type_t::word:
					// Если это открытый изоляционный символ
					case (u_short) type_t::open:
					// Если это нормальная буква
					case (u_short) type_t::allow: {
						// Если предыдущий символ не является открытым изоляционнмы символом
						if((typeContext.empty() ||
						(typeContext.top() != type_t::open)) &&
						(result.back() != L' ') &&
						(!this->alphabet->isNumber(token.substr(0, 1)) ||
						!this->alphabet->isMath(result.back()) ||
						(result.back() == L'='))) result.append(1, L' ');
						// Добавляем слово
						result.append(token);
					} break;
					// Если это число
					case (u_short) type_t::num:
					// Если это математическая операция
					case (u_short) type_t::math: {
						// Если предыдущий символ не является числом, математической операцией и пробелом
						if((typeContext.empty() ||
						((token.front() == L'=') && (result.back() != L'=')) ||
						((token.front() != L'=') && (result.back() == L'=')) ||
						((typeContext.top() != type_t::num) &&
						(typeContext.top() != type_t::math) &&
						(typeContext.top() != type_t::open) &&
						(typeContext.top() != type_t::space))) &&
						(result.back() != L' ')) result.append(1, L' ');
						// Добавляем слово
						result.append(token);
					} break;
					// Если это спец-символ
					case (u_short) type_t::specl: {
						// Если предыдущий символ не является спец-символом и пробелом
						if((typeContext.empty() ||
						((typeContext.top() != type_t::specl) &&
						(typeContext.top() != type_t::open) &&
						(typeContext.top() != type_t::space))) &&
						(result.back() != L' ')) result.append(1, L' ');
						// Добавляем слово
						result.append(token);
					} break;
					// Если это закрытый изоляционный символ
					case (u_short) type_t::close:
					// Если это знак-пунктуации
					case (u_short) type_t::punct: {
						// Получаем тип токена
						nextToken = typeFn(i + 1);
						// Добавляем слово
						result.append(token);
						// Если следующий символ не является знаком пунктуации
						if((nextToken != type_t::punct) &&
						(nextToken != type_t::end) &&
						(nextToken != type_t::space) &&
						(nextToken != type_t::close)) result.append(1, L' ');
					} break;
					// Если это изоляционный символ
					case (u_short) type_t::isolat: {
						// Получаем тип токена
						nextToken = typeFn(i + 1);
						// Если предыдущий символ не является изоляционным символом
						if((typeContext.empty() ||
						(typeContext.top() != type_t::space)) &&
						(result.back() != L' ')) result.append(1, L' ');
						// Добавляем слово
						result.append(token);
						// Если следующий символ является любым из слов кроме изоляционного символа и конца текста
						if((nextToken != type_t::end) &&
						(nextToken != type_t::space) &&
						(nextToken != type_t::punct)) result.append(1, L' ');
					} break;
				}
			}
			// Добавляем тип токена в контекст токенов
			typeContext.push(tokenType);
		}
	}
	// Выводим результат
	return result;
}
/**
 * readline Метод извлечения строки из текста
 * @param  is  файловый поток для чтения данных
 * @param  str строка для извлечения текста
 * @return     файловый поток с текущий позиции
 */
istream & anyks::Tokenizer::readline(istream & is, string & str) const {
	// Очищаем строку
	str.clear();
	// Создаем сторожа файлового потока
	istream::sentry se(is, true);
	// Создаем буфер чтения данных
	streambuf * sb = is.rdbuf();
	// Выполняем перебор всех входящих данных
	while(true){
		// Считываем символ из строки
		char c = sb->sbumpc();
		// Определяем тип символа
		switch(c){
			// Если это символ переноса строки
 			case '\n': return is;
 			// Если это символ возврата каретки
			case '\r': {
				// Проверяем следующий символ, если это перенос каретки, выходим
				if(sb->sgetc() == '\n') sb->sbumpc();
				// Выводим позицию файлового потока
				return is;
			}
			// Если это символ завершения файла
			case streambuf::traits_type::eof(): {
				// Если это конец, файла устанавливаем символ конца строки
				if(str.empty()) is.setstate(ios::eofbit);
				// Выводим позицию файлового потока
				return is;
			}
			// Если это любой другой символ, собираем строку
			default: str += c;
		}
	}
}
/**
 * jsonToText Метод преобразования текста в формате json в текст
 * @param text     текст для преобразования в формате json
 * @param callback функция обратного вызова, на каждой итерации
 */
void anyks::Tokenizer::jsonToText(const string & text, function <void (const string &)> callback) const {
	// Если текст передан, и текст не больше 100Mb в одну строку
	if(!text.empty() && (this->alphabet != nullptr)){
		// Получаем данные в виде json
		json obj = json::parse(text);
		// Если это массив
		if(obj.is_array()){
			// Слово в контексте
			string word = "";
			// Список полученных слов
			vector <string> context;
			// Переходим по всем элементам объекта
			for(auto & item : obj.items()){
				// Если значение является массивом
				if(item.value().is_array()){
					// Переходим по всем словам контекста
					for(auto & elem : item.value().items()){
						// Если это строка
						if(elem.value().is_string()) elem.value().get_to(word);
						// Если слово получено, добавляем в контекст
						if(!word.empty()) context.push_back(word);
					}
					// Если список токенов получен
					if(!context.empty()) callback(this->restore(context));
				}
				// Очищаем полученный контекст
				context.clear();
			}
		}
	}
}
/**
 * textToJson Метод преобразования текста в json
 * @param text     текст для преобразования
 * @param callback функция обратного вызова, на каждой итерации
 */
void anyks::Tokenizer::textToJson(const string & text, function <void (const string &)> callback) const {
	// Если текст передан, и текст не больше 100Mb в одну строку
	if(!text.empty() && (text.size() <= MAX_STRING_BYTES) && (this->alphabet != nullptr)){
		// Формируем результат
		vector <string> tokens;
		// Результирующий объект
		vector <vector <string>> result;
		// Выполняем разбивку на токенизацию
		this->run(text, [&result, &tokens, &callback, this](const wstring & word, const vector <string> & context, const bool reset, const bool end){
			// Если это сброс контекста
			if(reset || end){
				// Формируем новый список токенов
				if((!reset && end) && !word.empty()){
					// Дополняем список токенов
					tokens.push_back(this->alphabet->convert(word));
				}
				// Если список токенов получен
				if(!tokens.empty()){
					// Формируем строку результата
					result.push_back(tokens);
					// Очищаем список токенов
					tokens.clear();
				}
				// Если слово существует
				if(!word.empty()){
					// Формируем новый список токенов
					if(!end) tokens.push_back(this->alphabet->convert(word));
					// Если это сброс контекста и завершение текста
					else if(reset) result.push_back({this->alphabet->convert(word)});
				}
			// Добавляем новое слово в список токенов
			} else if(!word.empty()) tokens.push_back(this->alphabet->convert(word));
			// Выводим результат
			return true;
		});
		// Если результат получен
		if(!result.empty()){
			// Получаем результат в виде json
			json answer(result);
			// Выводим результат
			callback(answer.dump());
		}
	}
}
/**
 * run Метод разбивки текста на токены
 * @param text     входной текст для обработки
 * @param callback функция обратного вызова, на каждой итерации
 */
void anyks::Tokenizer::run(const string & text, function <const bool (const wstring &, const vector <string> &, const bool, const bool)> callback) const {
	// Если текст передан, и текст не больше 100Mb в одну строку
	if(!text.empty() && (text.size() <= MAX_STRING_BYTES) && (this->alphabet != nullptr)){
		// Типы флагов
		enum class type_t : u_short {
			url,   // Интернет адрес
			num,   // Символ числа в тексте
			null,  // Не определено
			math,  // Математическая операция
			specl, // Спец-символ в тексте
			space, // Символ пробела в тексте
			allow, // Разрешённый символ
			punct, // Знак препинания
			isolat // Изоляционный символ в строке
		};
		// Полученное слово
		wstring word = L"";
		// Собранный контекст
		vector <string> context;
		// Стек открытых скобок
		stack <wchar_t> brackets;
		// Тип следующего символа
		type_t type = type_t::null;
		// Текущая буква и следующий символ
		wchar_t letter = 0, lletter = 0, backLetter = 0, next = 0;
		// Основные флаги токенизации
		bool open = false, begin = false, end = false, nend = false;
		bool backPunct = false, stopToken = false, notClear = false;
		// Получаем буквы алфавита
		const wstring & letters = this->alphabet->convert(this->alphabet->get());
		/**
		 * callbackFn Функция вывода результата
		 * @param word полученное слово
		 * @param end  конец обработки текста
		 * @return     нужно ли завершить работу
		 */
		auto callbackFn = [&begin, &context, &callback](const wstring & word, const bool end){
			// Отдаём результат
			const bool result = callback(word, context, begin && context.empty(), end);
			// Запоминаем что работа началась
			begin = (!begin ? !begin : begin);
			// Выводим результат
			return result;
		};
		/**
		 * coordinatesFn Функция поиска координат запрещенных к обработке
		 * @param text текст для парсинга
		 * @return     список координат
		 */
		auto coordinatesFn = [&letters, this](const wstring & text){
			// Результат работы функции
			map <size_t, size_t> result = {{wstring::npos, wstring::npos}};
			// Если текст передан
			if(!text.empty()){
				// Позиция найденного uri адреса
				size_t pos = 0;
				// Объект работы с uri адресами
				uri_t uri(letters);
				// Выполням поиск ссылок в тексте
				while(pos < text.length()){
					// Выполняем парсинг uri адреса
					uri.parse(text.substr(pos));
					// Извлекаем данные uri адреса
					auto resUri = uri.get();
					// Если ссылка найдена
					if(resUri.type != uri_t::types_t::null){
						// Получаем данные слова
						const wstring & word = resUri.uri;
						// Если это не предупреждение
						if(resUri.type != uri_t::types_t::wrong){
							// Если позиция найдена
							if((pos = text.find(word, pos)) != wstring::npos){
								// Если в списке результатов найдены пустные значения, очищаем список
								if(result.count(wstring::npos) > 0) result.clear();
								// Добавляем в список нашу ссылку
								result.insert({pos, pos + word.length()});
							// Если ссылка не найдена в тексте, выходим
							} else break;
						}
						// Сдвигаем значение позиции
						pos += word.length();
					// Если uri адрес больше не найден то выходим
					} else break;
				}
			}
			// Выводим результат
			return result;
		};
		/**
		 * erangeFn Функция проверяющая вхождения позиции в диапазон координат
		 * @param pos         значение позиции
		 * @param coordinates список координат в тексте
		 * @return            результат проверки
		 */
		auto erangeFn = [](const size_t pos, const map <size_t, size_t> & coordinates){
			// Результат проверки
			bool result = false;
			// Если в списке нет неустановленных координат
			if(!coordinates.empty() && (coordinates.count(wstring::npos) < 1)){
				// Выполняем обход всех координат
				for(auto & item : coordinates){
					// Если позиция попала в запрещенную зону то сообщаем это
					result = !((pos <= item.first) || (pos >= item.second));
					// Если позиция вошла в указанный диапазон то выходим
					if(result) break;
				}
			}
			// Выводим результат
			return result;
		};
		// Получаем сконвертированный текст
		const wstring & tmp = this->alphabet->convert(text);
		// Выполняем поиск координат в тексте
		const auto coordinates = coordinatesFn(tmp);
		// Переходим по всему тексту
		for(size_t i = 0; i < tmp.length(); i++){
			// Получаем значение текущей буквы
			letter = tmp[i];
			// Переводим букву в нижний регистр
			lletter = this->alphabet->toLower(letter);
			// Определяем является ли это концом предложения
			end = (i == (tmp.length() - 1));
			// Определяем является ли слово адресом интернета
			if(erangeFn(i, coordinates)){
				// Формируем слово в виде url адреса
				word.append(1, letter);
				// Если это конец текста
				if(end && !callbackFn(word, end)) return;
			// Выполняем обычную обработку
			} else {
				// Получаем значение следующего символа
				next = this->alphabet->toLower(tmp[i + 1]);
				// Если следующий символ является концом строки
				if((i + 1) == (tmp.length() - 1)) nend = true;
				// Если следующий символ является знаком препинания
				if(this->alphabet->isPunct(next)) type = type_t::punct;
				// Если следующий символ является математической операцией
				else if(this->alphabet->isMath(next)) type = type_t::math;
				// Если следующий символ является пробелом
				else if(this->alphabet->isSpace(next)) type = type_t::space;
				// Если следующий символ является спец-символом
				else if(this->alphabet->isSpecial(next)) type = type_t::specl;
				// Если следующий символ является числом
				else if(this->alphabet->isNumber({next})) type = type_t::num;
				// Если следующий символ является символом изоляции
				else if(this->alphabet->isIsolation(next)) type = type_t::isolat;
				// Если следующий символ является разрешённым
				else if(this->alphabet->check(next)) type = type_t::allow;
				// Иначе зануляем следующий тип
				else type = type_t::null;
				// Если это пробел и слово не пустое
				if(this->alphabet->isSpace(lletter) && !word.empty()){
					// Выводим полученное слово
					if(!callbackFn(word, end)) return;
					// Добавляем слово в контекст
					context.push_back(this->alphabet->convert(word));
					// Очищаем слово
					word.clear();
				// Если это знак пунктуации
				} else if(this->alphabet->isPunct(lletter) ||
				this->alphabet->isSpecial(lletter) ||
				this->alphabet->isMath(lletter)) {
					// Проверяем следующие символы на стоп-токены
					stopToken = (
						(type == type_t::math) ||
						(type == type_t::space) ||
						(type == type_t::specl) ||
						(type == type_t::punct) ||
						(type == type_t::isolat)
					);
					// Проверяем является ли предыдущий символ также знаком пунктуации
					backPunct = ((backLetter > 0) && this->alphabet->isPunct(backLetter));
					// Выводим результат как он есть
					if(end){
						// Если слово не пустое
						if(!word.empty()){
							// Если это спец-символ
							if(this->alphabet->isSpecial(lletter)){
								// Добавляем букву в слово
								word.append(1, letter);
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
							// Если это не спец-символ а математическая операция или знак препинания
							} else {
								// Выводим полученное слово
								if(!callbackFn(word, !end)) return;
								// Добавляем слово в контекст
								context.push_back(this->alphabet->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
							}
						// Выводим знак препинания
						} else if(!callbackFn({letter}, end)) return;
					// Если слово не существует
					} else if(word.empty() && !this->alphabet->isPunct(lletter) &&
					!(((lletter == L'-') || (lletter == L'+') ||
					(lletter == L'*')) && (type == type_t::num))) {
						// Выводим знак препинания
						if(!callbackFn({letter}, end)) return;
						// Добавляем знак препинания в контекст
						context.push_back(this->alphabet->convert(wstring(1, letter)));
					// Если слово не пустое и это точка, запятая или двоеточие
					} else if((lletter == L'.') || (lletter == L',') ||
					(lletter == L':') || (lletter == L';') || (lletter == L'/')) {
						// Получаем символ для проверки
						wchar_t sumbol = (
							(lletter == L'.') ? ((type == type_t::space) ? tmp[i + 2] :
							((type == type_t::allow) ? tmp[i + 1] : 0)) : 0
						);
						// Проверяем является ли слово аббревиатурой
						bool abbr = (
							(!word.empty() && (word.find(L'.') != wstring::npos)) ||
							((sumbol > 0) && (word.length() <= 4) &&
							(!this->alphabet->isUpper(sumbol) || (word.length() == 1) ||
							(letters.find(tolower(sumbol)) == wstring::npos)))
						);
						// Если следующий символ является пробелом или нормальным словом
						if((lletter != L'/') && (((type == type_t::allow) &&
						((lletter != L'.') || !abbr)) || ((stopToken && !abbr) || backPunct))){
							// Выводим полученное слово
							if(!callbackFn(word, end)) return;
							// Добавляем слово в контекст
							if(!word.empty()) context.push_back(this->alphabet->convert(word));
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Добавляем знак препинания в контекст
							context.push_back(this->alphabet->convert(wstring(1, letter)));
							// Иначе очищаем контекст
							if(!notClear && (!backPunct || !!this->alphabet->isUpper(sumbol)) &&
							(lletter == L'.') && (type != type_t::punct)) context.clear();
							// Очищаем слово
							word.clear();
						// Если это не пробел
						} else if(!this->alphabet->isSpace(lletter)) {
							// Если следующий символ является концом текста или стоп-токеном
							if(nend && stopToken){
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Добавляем слово в контекст
								if(!word.empty()) context.push_back(this->alphabet->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Добавляем знак препинания в контекст
								context.push_back(this->alphabet->convert(wstring(1, letter)));
								// Очищаем слово
								word.clear();
							// Иначе добавляем символ в словарь
							} else word.append(1, letter);
						}
					// Если это другие знаки препинания
					} else if((lletter == L'!') || (lletter == L'?')) {
						// Если следующий символ является пробелом или нормальным словом
						if((type == type_t::allow) || (type == type_t::space) ||
						(type == type_t::isolat) || stopToken || backPunct) {
							// Если это бинарное сравнение, символ в слово
							if((lletter == L'!') && (next == L'=')) word.append(1, letter);
							// Иначе обрываем контекст
							else {
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Добавляем слово в контекст
								if(!word.empty()) context.push_back(this->alphabet->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Добавляем знак препинания в контекст
								context.push_back(this->alphabet->convert(wstring(1, letter)));
								// Иначе очищаем контекст
								if(!notClear && !nend && (type != type_t::punct)
								&& (type != type_t::isolat)) context.clear();
								// Очищаем слово
								word.clear();
							}
						// Если это не пробел
						} else if(!this->alphabet->isSpace(lletter)) {
							// Если следующий символ является концом текста или стоп-токеном
							if(nend && stopToken){
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Добавляем слово в контекст
								if(!word.empty()) context.push_back(this->alphabet->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Добавляем знак препинания в контекст
								context.push_back(this->alphabet->convert(wstring(1, letter)));
								// Очищаем слово
								word.clear();
							// Иначе добавляем символ в словарь
							} else word.append(1, letter);
						}
					// Если это спец-символ
					} else if(this->alphabet->isSpecial(lletter) || this->alphabet->isMath(lletter)) {
						// Добавляем символ в слово
						if(!this->alphabet->isSpace(lletter)) word.append(1, letter);
						// Если следующий символ является пробелом
						if(type == type_t::space){
							// Выводим полученное слово
							if(!callbackFn(word, end)) return;
							// Добавляем слово в контекст
							if(!word.empty()) context.push_back(this->alphabet->convert(word));
							// Очищаем слово
							word.clear();
						}
					}
				// Если это изоляционный символ
				} else if(this->alphabet->isIsolation(lletter)) {
					// Выводим результат как он есть
					if(end){
						// Если слово не пустое
						if(!word.empty()){
							// Выводим полученное слово
							if(!callbackFn(word, !end)) return;
							// Добавляем слово в контекст
							if(open) context.push_back(this->alphabet->convert(word));
							// Очищаем контекст
							else if(!notClear) context.clear();
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
						// Выводим знак препинания
						} else if(!callbackFn({letter}, end)) return;
					// Если это не конец
					} else {
						// Определяем тип изоляционных знаков
						u_short typeBracket = (
							(lletter == L'(') || (lletter == L'[') || (lletter == L'{') ||
							(lletter == L'«') || (lletter == L'„') ? 1 :
							((lletter == L')') || (lletter == L']') || (lletter == L'}') ||
							(lletter == L'»') || (lletter == L'“') ? 2 : 0)
						);
						// Если это изоляционный знак открытия
						if(typeBracket == 1){
							// Запрещаем очистку словаря
							notClear = true;
							// Добавляем в стек наш знак
							brackets.push(letter);
							// Если слово не пустое
							if(!word.empty()){
								// Добавляем слово в контекст
								context.push_back(this->alphabet->convert(word));
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Очищаем слово
								word.clear();
							}
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Добавляем знак препинания в контекст
							context.push_back(this->alphabet->convert(wstring(1, letter)));
						// Если это изоляционный знак закрытия и он совпадает с открытым
						} else if(typeBracket == 2) {
							// Разрешено продолжить работу
							bool mode = false;
							// Если стек не пустой
							if(!brackets.empty()){
								// Определяем тип открытой скобки
								switch(brackets.top()){
									// Если это круглая скобка
									case L'(': mode = (letter == L')'); break;
									// Если это квадратная скобка
									case L'[': mode = (letter == L']'); break;
									// Если это фигурная скобка
									case L'{': mode = (letter == L'}'); break;
									// Если это угловые кавычки
									case L'«': mode = (letter == L'»'); break;
									// Если это литературные кавычки
									case L'„': mode = (letter == L'“'); break;
								}
							}
							// Если слово не пустое
							if(!word.empty() && !callbackFn(word, end)) return;
							// Добавляем слово в контекст
							if(!word.empty()) context.push_back(this->alphabet->convert(word));
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Очищаем слово
							word.clear();
							// Добавляем знак препинания в контекст
							context.push_back(this->alphabet->convert(wstring(1, letter)));
							// Если разрешено продолжить
							if(mode) brackets.pop();
							// Если больше символов изоляции не открыто, разрешаем очистку контекста
							notClear = !brackets.empty();
						// Если это открытие изоляционного знака
						} else if((open = !open)) {
							// Если слово не пустое
							if(!word.empty()){
								// Добавляем слово в контекст
								context.push_back(this->alphabet->convert(word));
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Очищаем слово
								word.clear();
							}
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Добавляем знак препинания в контекст
							if(!this->alphabet->isSpace(letter)){
								// Добавляем знак препинания в контекст
								context.push_back(this->alphabet->convert(wstring(1, letter)));
							}
						// Если это закрытие изоляционного знака
						} else {
							// Если слово не пустое
							if(!word.empty() && !callbackFn(word, end)) return;
							// Добавляем слово в контекст
							if(!word.empty()) context.push_back(this->alphabet->convert(word));
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Очищаем слово
							word.clear();
							// Добавляем знак препинания в контекст
							context.push_back(this->alphabet->convert(wstring(1, letter)));
						}
					}
				// Если это другие символы
				} else {
					// Если это первая буква в первом слове
					if(context.empty() && word.empty())
						// Приводим букву к верхнему регистру
						letter = this->alphabet->toUpper(letter);
					// Формируем слово
					if(!this->alphabet->isSpace(lletter)) word.append(1, letter);
					// Выводим полученное слово, если это конец текста
					if(end) if(!callbackFn(word, end)) return;
				}
			}
			// Запоминаем текущую букву
			backLetter = lletter;
		}
	}
}
/**
 * Tokenizer Конструктор
 * @param alphabet объект алфавита
 */
anyks::Tokenizer::Tokenizer(const alphabet_t * alphabet){
	// Устанавливаем лафавит
	if(alphabet != nullptr) this->setAlphabet(alphabet);
}

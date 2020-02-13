/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include "alphabet.hpp"

// Устанавливаем шаблон функции
template <typename C, typename T, typename A>
/**
 * strim Функция удаления начальных и конечных пробелов
 * @param str строка для обработки
 * @param loc локаль
 * @return    результат работы функции
 */
basic_string <C, T, A> strim(const basic_string <C, T, A> & str, const locale & loc = locale::classic()){
	// Запоминаем итератор на первый левый символ
	auto begin = str.begin();
	// Переходим по всем символам в слове и проверяем является ли символ - символом пробела, если нашли то смещаем итератор
	while((begin != str.end()) && isspace(* begin, loc)) ++begin;
	// Если прошли все слово целиком значит пробелов нет и мы выходим
	if(begin == str.end()) return {};
	// Запоминаем итератор на первый правый символ
	auto rbegin = str.rbegin();
	// Переходим по всем символам в слове и проверяем является ли символ - символом пробела, если нашли то смещаем итератор
	while(rbegin != str.rend() && isspace(* rbegin, loc)) ++rbegin;
	// Выводим результат
	return {begin, rbegin.base()};
}
// Устанавливаем шаблон функции
template <typename A, typename B>
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void split(const A & str, const A & delim, B & v){
	// Очищаем словарь
	v.clear();
	// Параметры словаря
	anyks::alphabet_t alphabet;
	// Получаем счётчики перебора
	size_t i = 0, j = str.find(delim);
	const size_t len = delim.length();
	// Выполняем разбиение строк
	while(j != A::npos){
		v.push_back(alphabet.trim(str.substr(i, j - i)));
		i = ++j + (len - 1);
		j = str.find(delim, j);
		if(j == A::npos) v.push_back(alphabet.trim(str.substr(i, str.length())));
	}
	// Если слово передано а вектор пустой, тогда создаем вектори из 1-го элемента
	if(!str.empty() && v.empty()) v.push_back(alphabet.trim(str));
}
/**
 * cbegin Метод итератор начала списка
 * @return итератор
 */
const std::set <wchar_t>::const_iterator anyks::Alphabet::cbegin() const {
	// Выводим итератор
	return this->letters.cbegin();
}
/**
 * cend Метод итератор конца списка
 * @return итератор
 */
const std::set <wchar_t>::const_iterator anyks::Alphabet::cend() const {
	// Выводим итератор
	return this->letters.cend();
}
/**
 * crbegin Метод обратный итератор начала списка
 * @return итератор
 */
const std::set <wchar_t>::const_reverse_iterator anyks::Alphabet::crbegin() const {
	// Выводим итератор
	return this->letters.crbegin();
}
/**
 * crend Метод обратный итератор конца списка
 * @return итератор
 */
const std::set <wchar_t>::const_reverse_iterator anyks::Alphabet::crend() const {
	// Выводим итератор
	return this->letters.crend();
}
/**
 * alts Метод получения списка альтернативных букв
 * return список альтернативных букв
 */
const std::map <wchar_t, wchar_t> & anyks::Alphabet::alts() const {
	// Выводим результат
	return this->alters;
}
/**
 * readline Метод извлечения строки из текста
 * @param  is  файловый поток для чтения данных
 * @param  str строка для извлечения текста
 * @return     файловый поток с текущий позиции
 */
istream & anyks::Alphabet::readline(istream & is, string & str) const {
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
 * get Метод получения алфавита языка
 * @return алфавит языка
 */
const string anyks::Alphabet::get() const {
	// Выводим результат
	return this->convert(this->alphabet);
}
/**
 * trim Метод удаления пробелов вначале и конце текста
 * @param  text текст для удаления пробелов
 * @return      текст без пробелов
 */
const string anyks::Alphabet::trim(const string & text) const {
	// Выводим результат
	return (!text.empty() ? strim(text) : "");
}
/**
 * toLower Метод перевода русских букв в нижний регистр
 * @param str строка для перевода
 * @return    строка в нижнем регистре
 */
const string anyks::Alphabet::toLower(const string & str) const {
	// Результат работы функции
	string result = str;
	// Если строка передана
	if(!str.empty()){
		// Получаем временную строку
		wstring tmp = this->convert(result);
		// Объявляем локаль
		const locale utf8(this->encoding);
		// Переходим по всем символам
		for(auto & c : tmp) c = std::tolower(c, utf8);
		// Конвертируем обратно
		result = move(this->convert(tmp));
	}
	// Выводим результат
	return result;
}
/**
 * toUpper Метод перевода русских букв в верхний регистр
 * @param str строка для перевода
 * @return    строка в верхнем регистре
 */
const string anyks::Alphabet::toUpper(const string & str) const {
	// Результат работы функции
	string result = str;
	// Если строка передана
	if(!str.empty()){
		// Получаем временную строку
		wstring tmp = this->convert(result);
		// Объявляем локаль
		const locale utf8(this->encoding);
		// Переходим по всем символам
		for(auto & c : tmp) c = std::toupper(c, utf8);
		// Конвертируем обратно
		result = move(this->convert(tmp));
	}
	// Выводим результат
	return result;
}
/**
 * convert Метод конвертирования строки utf-8 в строку
 * @param  str строка utf-8 для конвертирования
 * @return     обычная строка
 */
const string anyks::Alphabet::convert(const wstring & str) const {
	// Результат работы функции
	string result = "";
	// Если строка передана
	if(!str.empty()){
		// Объявляем конвертер
		wstring_convert <codecvt_utf8 <wchar_t>> conv;
		// Выполняем конвертирование в utf-8 строку
		result = conv.to_bytes(str);
	}
	// Выводим результат
	return result;
}
/**
 * format Метод реализации функции формирования форматированной строки
 * @param format формат строки вывода
 * @param args   передаваемые аргументы
 * @return       сформированная строка
 */
const string anyks::Alphabet::format(const char * format, ...) const {
	// Результат работы функции
	string result = "";
	// Если формат передан
	if(format != nullptr){
		// Создаем список аргументов
		va_list args;
		// Создаем буфер
		char buffer[BUFFER_CHUNK];
		// Заполняем буфер нулями
		memset(buffer, 0, sizeof(buffer));
		// Устанавливаем начальный список аргументов
		va_start(args, format);
		// Выполняем запись в буфер
		const size_t size = vsprintf(buffer, format, args);
		// Завершаем список аргументов
		va_end(args);
		// Если размер не нулевой
		if(size > 0) result.assign(buffer, size);
	}
	// Выводим результат
	return result;
}
/**
 * alt Метод получения альтернативной буквы
 * @param  lid буква для проверки
 * @return     альтернативная буква
 */
const wchar_t anyks::Alphabet::alt(const wchar_t lid) const {
	// Резузльтат работы функции
	wchar_t result = 0;
	// Если буква передана
	if(lid > 0){
		// Ищем букву в списке
		auto it = this->alters.find(lid);
		// Выводим результат
		result = (it != this->alters.end() ? it->second : result);
	}
	// Выводим в результат
	return result;
}
/**
 * rel Метод получения реальной буквы из альтернативной
 * @param  lid альтернативная буква
 * @return     реальная буква
 */
const wchar_t anyks::Alphabet::rel(const wchar_t lid) const {
	// Резузльтат работы функции
	wchar_t result = 0;
	// Если альтернативная буква передана
	if((lid > 0) && !this->alters.empty()){
		// Переходим по всему списку альтернативных букв
		for(auto & item : this->alters){
			// Если буква найдена
			if(lid == item.second){
				// Запоминаем результат
				result = item.first;
				// Выходим из цикла
				break;
			}
		}
	}
	// Выводим в результат
	return result;
}
/**
 * trim Метод удаления пробелов вначале и конце текста
 * @param  text текст для удаления пробелов
 * @return      текст без пробелов
 */
const wstring anyks::Alphabet::trim(const wstring & text) const {
	// Выводим результат
	return (!text.empty() ? strim(text) : L"");
}
/**
 * convert Метод конвертирования строки в строку utf-8
 * @param  str строка для конвертирования
 * @return     строка в utf-8
 */
const wstring anyks::Alphabet::convert(const string & str) const {
	// Результат работы функции
	wstring result = L"";
	// Если строка передана
	if(!str.empty()){
		// Объявляем конвертер
		wstring_convert <codecvt_utf8 <wchar_t>> conv;
		// Выполняем конвертирование в utf-8 строку
		result = conv.from_bytes(str);
	}
	// Выводим результат
	return result;
}
/**
 * toLower Метод перевода русских букв в нижний регистр
 * @param str строка для перевода
 * @return    строка в нижнем регистре
 */
const wstring anyks::Alphabet::toLower(const wstring & str) const {
	// Результат работы функции
	wstring result = str;
	// Если строка передана
	if(!str.empty()){
		// Объявляем локаль
		const locale utf8(this->encoding);
		// Переходим по всем символам
		for(auto & c : result) c = tolower(c, utf8);
	}
	// Выводим результат
	return result;
}
/**
 * toUpper Метод перевода русских букв в верхний регистр
 * @param str строка для перевода
 * @return    строка в верхнем регистре
 */
const wstring anyks::Alphabet::toUpper(const wstring & str) const {
	// Результат работы функции
	wstring result = str;
	// Если строка передана
	if(!str.empty()){
		// Объявляем локаль
		const locale utf8(this->encoding);
		// Переходим по всем символам
		for(auto & c : result) c = toupper(c, utf8);
	}
	// Выводим результат
	return result;
}
/**
 * arabic2Roman Метод перевода арабских чисел в римские
 * @param  number арабское число от 1 до 4999
 * @return        римское число
 */
const wstring anyks::Alphabet::arabic2Roman(const u_int number) const {
	// Результат работы функции
	wstring result = L"";
	// Если число передано верное
	if((number >= 1) && (number <= 4999)){
		// Копируем полученное число
		u_int n = number;
		// Формируем вектора с абривиатурами
		vector <wstring> i = {L"", L"I", L"II", L"III", L"IV", L"V", L"VI", L"VII", L"VIII", L"IX"};
		vector <wstring> x = {L"", L"X", L"XX", L"XXX", L"XL", L"L", L"LX", L"LXX", L"LXXX", L"XC"};
		vector <wstring> c = {L"", L"C", L"CC", L"CCC", L"CD", L"D", L"DC", L"DCC", L"DCCC", L"CM"};
		vector <wstring> m = {L"", L"M", L"MM", L"MMM", L"MMMM"};
		// Вычисляем до тысяч
		result.append(m[floor(n / 1000)]);
		// Уменьшаем диапазон
		n %= 1000;
		// Вычисляем до сотен
		result.append(c[floor(n / 100)]);
		// Вычисляем до сотен
		n %= 100;
		// Вычисляем до десятых
		result.append(x[floor(n / 10)]);
		// Вычисляем до сотен
		n %= 10;
		// Формируем окончательный результат
		result.append(i[n]);
	}
	// Выводим результат
	return result;
}
/**
 * arabic2Roman Метод перевода арабских чисел в римские
 * @param  word арабское число от 1 до 4999
 * @return      римское число
 */
const wstring anyks::Alphabet::arabic2Roman(const wstring & word) const {
	// Результат работы функции
	wstring result = L"";
	// Если слово передано
	if(!word.empty()){
		// Преобразуем слово в число
		const u_int number = stoi(word);
		// Выполняем расчет
		result = this->arabic2Roman(number);
	}
	// Выводим результат
	return result;
}
/**
 * delAltInWord Метод удаления альтернативных букв в слове
 * @param word слово в котором нужно удалить альтернативную букву
 * @return     слово без альтернативной буквы
 */
const wstring anyks::Alphabet::delAltInWord(const wstring & word) const {
	// Результат работы функции
	wstring result = L"";
	// Если слово передано
	if(!word.empty()){
		// Копируем слово
		result = word;
		// Переходим по каждой букве
		for(size_t i = 0; i < result.length(); i++){
			// Получаем реальную букву
			wchar_t rel = this->rel(result[i]);
			// Если реальная буква получена то заменяем её на альтернативную
			if(rel > 0) result.replace(i, 1, wstring({rel}));
		}
	}
	// Выводим результат
	return result;
}
/**
 * delPunctInWord Метод очистки текста от всех знаков препинаний
 * @param word слово для очистки
 * @return     текст без запрещенных символов
 */
const wstring anyks::Alphabet::delPunctInWord(const wstring & word) const {
	// Результат работы функции
	wstring result = L"";
	// Если строка передана
	if(!word.empty()){
		// Выполняем копирование строки
		result = word;
		/**
		 * isHyphen Функция проверки на разделитель
		 * @param c символ для проверки
		 */
		auto isHyphen = [](const wchar_t c){
			// Выводим результат
			return (
				(c == L'~') || (c == L'-') || (c == L'+') ||
				(c == L'=') || (c == L'*') || (c == L'/') ||
				(c == L':') || (c == L'%') || (c == L'|') ||
				(c == L'^') || (c == L'&') || (c == L'#') ||
				(c == L'\\')
			);
		};
		// Выполняем удаление всех знаков препинания
		result.erase(remove_if(result.begin(), result.end(), [&isHyphen](const wchar_t c){
			// Если это буква или цифра или дефис
			return (
				!isHyphen(c) &&
				(c != L'\r') &&
				(c != L'\n') &&
				!iswalnum(c) &&
				!iswspace(c)
			);
		}), result.end());
	}
	// Выводим результат
	return result;
}
/**
 * delBrokenInWord Метод очистки текста от всех символов кроме разрешенных
 * @param word слово для очистки
 * @return     текст без запрещенных символов
 */
const wstring anyks::Alphabet::delBrokenInWord(const wstring & word) const {
	// Результат работы функции
	wstring result = L"";
	// Если строка передана
	if(!word.empty()){
		// Выполняем копирование строки
		result = word;
		/**
		 * isHyphen Функция проверки на разделитель
		 * @param c символ для проверки
		 */
		auto isHyphen = [](const wchar_t c){
			// Выводим результат
			return (
				(c == L'~') || (c == L'-') || (c == L'+') ||
				(c == L'=') || (c == L'*') || (c == L'/') ||
				(c == L':') || (c == L'%') || (c == L'|') ||
				(c == L'^') || (c == L'&') || (c == L'#') ||
				(c == L'\\')
			);
		};
		// Выполняем удаление всех знаков препинания
		result.erase(remove_if(result.begin(), result.end(), [&isHyphen, this](const wchar_t c){
			// Если это буква или цифра или дефис
			return (
				!isHyphen(c) &&
				(c != L'\r') &&
				(c != L'\n') &&
				!iswspace(c) &&
				!this->check(c) &&
				!this->isNumber({c})
			);
		}), result.end());
	}
	// Выводим результат
	return result;
}
/**
 * delHyphenInWord Метод удаления дефиса из слова
 * @param word слово в котором нужно удалить дефис
 * @return     слово без дефиса
 */
const wstring anyks::Alphabet::delHyphenInWord(const wstring & word) const {
	// Результат работы функции
	wstring result = L"";
	// Если слово передано
	if(!word.empty()){
		// Запоминаем результат
		result = word;
		// Позиция пробела в слове
		size_t pos = 0;
		// Если дефис не найден тогда вывадим слово как оно есть
		while((pos = result.find(L"-", pos)) != wstring::npos){
			// Выполняем удалени дефиса
			result.replace(pos, 1, L"");
			// Увеличиваем позицию
			pos++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * restore Метод восстановления текста из контекста
 * @param context токенизированный контекст
 * @return        результирующий текст
 */
const string anyks::Alphabet::restore(const vector <wstring> & context) const {
	// Результат работы функции
	string result = "";
	// Если контекст передан
	if(!context.empty()){
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
				// Если длина слова больше 1-го символа, значит это слово
				if(context[index].length() > 1) result = type_t::word;
				// Если это всего один символ
				else {
					// Устанавливаем локаль приложения
					const locale utf8(this->encoding);
					// Получаем символ токена в нижнем регистре
					wchar_t letter = tolower(context[index].front(), utf8);
					// Определяем тип символа
					if(this->isNumber({letter})) result = type_t::num;
					// Если это математическая операция
					else if(this->isMath(letter)) result = type_t::math;
					// Если это разрешённая буква алфавита
					else if(this->check(letter)) result = type_t::allow;
					// Если это символ пробела
					else if(this->isSpace(letter)) result = type_t::space;
					// Если это знак пунктуации
					else if(this->isPunct(letter)) result = type_t::punct;
					// Если это спец-символ
					else if(this->isSpecial(letter)) result = type_t::specl;
					// Если это изоляционный символ
					else if(this->isIsolation(letter)) {
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
		// Флаг конца текста
		bool end = false;
		// Стек типов собранного контекста
		stack <type_t> typeContext;
		// Тип текущего токена контекста
		type_t tokenType = type_t::null, nextToken = type_t::null;
		// Переходим по всем токенам
		for(size_t i = 0; i < context.size(); ++i){
			// Получаем тип токена
			tokenType = typeFn(i);
			// Определяем является ли это концом предложения
			end = (i == (context.size() - 1));
			// Если это начало предложения
			if(result.empty()) result.append(this->convert(context[i]));
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
						(result.back() != ' ') &&
						(!this->isNumber({context[i].front()}) ||
						!this->isMath(result.back()) ||
						(result.back() == '='))) result.append(" ");
						// Добавляем слово
						result.append(this->convert(context[i]));
					} break;
					// Если это число
					case (u_short) type_t::num:
					// Если это математическая операция
					case (u_short) type_t::math: {
						// Если предыдущий символ не является числом, математической операцией и пробелом
						if((typeContext.empty() ||
						((context[i].front() == L'=') && (result.back() != '=')) ||
						((context[i].front() != L'=') && (result.back() == '=')) ||
						((typeContext.top() != type_t::num) &&
						(typeContext.top() != type_t::math) &&
						(typeContext.top() != type_t::open) &&
						(typeContext.top() != type_t::space))) &&
						(result.back() != ' ')) result.append(" ");
						// Добавляем слово
						result.append(this->convert(context[i]));
					} break;
					// Если это спец-символ
					case (u_short) type_t::specl: {
						// Если предыдущий символ не является спец-символом и пробелом
						if((typeContext.empty() ||
						((typeContext.top() != type_t::specl) &&
						(typeContext.top() != type_t::open) &&
						(typeContext.top() != type_t::space))) &&
						(result.back() != ' ')) result.append(" ");
						// Добавляем слово
						result.append(this->convert(context[i]));
					} break;
					// Если это закрытый изоляционный символ
					case (u_short) type_t::close:
					// Если это знак-пунктуации
					case (u_short) type_t::punct: {
						// Получаем тип токена
						nextToken = typeFn(i + 1);
						// Добавляем слово
						result.append(this->convert(context[i]));
						// Если следующий символ не является знаком пунктуации
						if((nextToken != type_t::punct) &&
						(nextToken != type_t::end) &&
						(nextToken != type_t::space) &&
						(nextToken != type_t::close)) result.append(" ");
					} break;
					// Если это изоляционный символ
					case (u_short) type_t::isolat: {
						// Получаем тип токена
						nextToken = typeFn(i + 1);
						// Если предыдущий символ не является изоляционным символом
						if((typeContext.empty() ||
						(typeContext.top() != type_t::space)) &&
						(result.back() != ' ')) result.append(" ");
						// Добавляем слово
						result.append(this->convert(context[i]));
						// Если следующий символ является любым из слов кроме изоляционного символа и конца текста
						if((nextToken != type_t::end) &&
						(nextToken != type_t::space) &&
						(nextToken != type_t::punct)) result.append(" ");
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
 * replace Метод замены в тексте слово на другое слово
 * @param  text текст в котором нужно произвести замену
 * @param  word слово для поиска
 * @param  alt  слово на которое нужно произвести замену
 * @return      результирующий текст
 */
const wstring anyks::Alphabet::replace(const wstring & text, const wstring & word, const wstring & alt) const {
	// Результат работы функции
	wstring result = text;
	// Если текст передан и искомое слово не равно слову для замены
	if(!result.empty() && !word.empty() && (word.compare(alt) != 0)){
		// Позиция искомого текста
		size_t pos = 0;
		// Определяем текст на который нужно произвести замену
		const wstring & alternative = (!alt.empty() ? alt : L"");
		// Выполняем поиск всех слов
		while((pos = result.find(word, pos)) != wstring::npos){
			// Выполняем замену текста
			result.replace(pos, word.length(), alternative);
			// Смещаем позицию на единицу
			pos++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * errors Метод подсчета максимально-возможного количества ошибок в слове
 * @param  word слово для расчёта
 * @return      результат расчёта
 */
const u_short anyks::Alphabet::errors(const wstring & word) const {
	// Результат работы функции
	u_short result = 0;
	// Если слово передано
	if(!word.empty()){
		// Получаем длину слова
		const size_t length = word.length();
		// Выполняем расчёт количества ошибок разрешённых для данного слова
		// result = (length >= 9 ? 4 : (length > 5 ? 3 : (length > 3 ? 2 : (length > 2 ? 1 : 0))));
		// result = (length >= 9 ? 4 : (length >= 5 ? 3 : (length > 2 ? 1 : 0)));
		// result = (length >= 9 ? 4 : (length > 6 ? 3 : (length > 4 ? 2 : (length > 2 ? 1 : 0))));
		result = (length >= 9 ? 4 : (length > 6 ? 3 : (length > 2 ? 2 : (length > 1 ? 1 : 0))));
		// result = (length >= 9 ? 3 : (length > 6 ? 3 : (length > 2 ? 1 : 0)));
	}
	// Выводим результат
	return result;
}
/**
 * numSign Метод извлечения числового признака слова
 * @param  word слово для проверки
 * @return      результат проверки
 */
const u_short anyks::Alphabet::numSign(const wstring & word) const {
	// Результат работы функции
	u_short result = 0;
	// Если слово передано
	if(!word.empty()){
		// Длина переданного слова
		const size_t size = word.size();
		// Если это не одна буква
		if(size > 1){
			// Получаем первый символ слова
			const wchar_t first = word.front();
			// Получаем последний символ слова
			const wchar_t second = word.back();
			// Проверяем является ли первый символ числом
			const bool frontNum = this->isNumber({first});
			// Определяем является ли последний символ числом
			const bool backNum = this->isNumber({second});
			// Если первый символ не является числом а второй является (-12, -15.64, -18,22, ~25, ~845.53, ~12,46)
			if(!frontNum && backNum){
				// Проверяем является ли первый символ (- или ~)
				if((first == L'-') || (first == L'~')){
					// Получаем оставшуюся часть слова
					const wstring & tmp = word.substr(1);
					// Проверяем оставшуюся часть слова является числом
					if(this->isNumber(tmp) || this->isDecimal(tmp)){
						// Определяем тип признака
						switch(first){
							// Это обычное число
							case L'-': result = (u_short) sign_t::num;   break;
							// Это приблизительное число
							case L'~': result = (u_short) sign_t::aprox; break;
						}
					// Сообщаем что это псевдо-число
					} else result = (u_short) sign_t::anum;
				// Если это не отрицательное и не приблизительное число (Дом-2)
				} else {
					// Ищем дефис в конце слова
					size_t pos = word.rfind(L'-');
					// Если дефис не найден и не найдено завершение слова в виде числа
					if((pos == wstring::npos) || !this->isNumber(word.substr(pos + 1))){
						// Сообщаем что это псевдо-число
						result = (u_short) sign_t::anum;
					}
				}
			// Если первый символ является числом а последний нет (2-й, 13-летний)
			} else if(frontNum && !backNum) {
				// Ищем дефис в конце слова
				size_t pos = word.rfind(L'-');
				// Проверяем является ли слово сокращением (не 25TM)
				if(pos != wstring::npos){
					// Получаем новое слово для проверки
					const wstring & tmp = word.substr(pos + 1);
					// Если это не псевдо-число (не 2-15tm)
					if(!this->isANumber(tmp)){
						// Слово запрещено для использования
						bool noallow = false;
						// Длина переданного слова
						const size_t size = tmp.size();
						// Переходим по всему списку
						for(size_t i = 0, j = size - 1; j > (size / 2); i++, j--){
							// Проверяем является ли слово арабским числом
							noallow = (i == j ? !this->check(tmp[i]) : !this->check(tmp[i]) || !this->check(tmp[j]));
							// Если хоть один символ является числом, выходим
							if(noallow) break;
						}
						// Если слово разрешено, значит это аббревиатура
						if(!noallow) result = (u_short) sign_t::abbr;
						// Иначе запоминаем что это неизвестный символ (2-@tm)
						else result = (u_short) sign_t::anum;
					// Сообщаем что это псевдо-число
					} else result = (u_short) sign_t::anum;
				// Сообщаем что это псевдо-число
				} else result = (u_short) sign_t::anum;
			// Если оба символа являются числом (5353, 5353.243, 3:4, 18:00, 18:00:01, 18.02.2012, 18/02/2012, 2/3, 3х10, 3~4)
			} else if(frontNum && backNum) {
				// Если это число
				if(this->isNumber(word)) result = (u_short) sign_t::num;
				// Если это псевдо-число
				else {
					// Разделитель слова найден
					bool delim = false;
					// Запоминаем что это псевдо-число
					result = (u_short) sign_t::anum;
					// Переходим по всем символам слова
					for(size_t i = 0; i < size; i++){
						// Если плавающая точка найдена
						if((word[i] == L'.') || (word[i] == L',') || (word[i] == L':') || (word[i] == L'/') || (word[i] == L'х') || (word[i] == L'~') || (word[i] == L'-')){
							// Проверяем правые и левую части
							delim = (this->isNumber(word.substr(0, i)) && this->isNumber(word.substr(i + 1)));
							// Если число собрано
							if(delim){
								// Определяем тип разделителя
								switch(word[i]){
									case L',':
									case L'.': result = (u_short) sign_t::num;   break;
									case L'~': result = (u_short) sign_t::aprox; break;
									case L'-': result = (u_short) sign_t::range; break;
									case L'/': result = (u_short) sign_t::fract; break;
									case L'х': result = (u_short) sign_t::dimen; break;
									case L':': result = (u_short) sign_t::score; break;
								}
							// Если число не собрано а являетс временем или датой
							} else if((word[i] == L':') || (word[i] == L'.') || (word[i] == L'/')) {
								// Список элементов слова
								vector <wstring> words;
								// Выполняем разбивку на составляющие
								this->split(word, {word[i]}, words);
								// Если список разбит правильно
								if(words.size() == 3){
									// Переходим по всему списку слова
									for(auto & word : words){
										// Если слово не является числом
										if(!this->isNumber(word)) return 0;
									}
									// Определяем тип разделителя
									switch(word[i]){
										case L'/':
										case L'.': result = (u_short) sign_t::date; break;
										case L':': result = (u_short) sign_t::time; break;
									}
								}
							}
							// Выходим из цикла
							break;
						}
					}
				}
			// Если это вообще не число, проверяем может это римское число
			} else if(!frontNum && !backNum && (this->roman2Arabic(this->toLower(word)) > 0)) result = (u_short) sign_t::num;
		// Если это число то выводим признак числа
		} else if(this->isNumber(word)) result = (u_short) sign_t::num;
	}
	// Выводим результат
	return result;
}
/**
 * roman2Arabic Метод перевода римских цифр в арабские
 * @param  word римское число
 * @return      арабское число
 */
const u_int anyks::Alphabet::roman2Arabic(const wstring & word) const {
	// Результат работы функции
	u_int result = 0;
	// Если слово передано
	if(!word.empty()){
		// Флаг проверки соответствия слова
		bool isRoman = false;
		// Список римских цифр
		std::set <wchar_t> numbers = {L'm', L'd', L'c', L'l', L'x', L'i', L'v'};
		// Переходим по всем буквам слова
		for(size_t i = 0; i < word.length(); i++){
			// Проверяем является ли слово римским числом
			isRoman = (numbers.count(word[i]) > 0);
			// Если слово не соответствует тогда выходим
			if(!isRoman) break;
		}
		// Если слово соответствует римским цифрам
		if(isRoman){
			// Символ поиска
			wchar_t c;
			// Вспомогательные переменные
			u_int i = 0, v = 0, n = 0;
			// Преобразовываем цифру M
			if(word[i] == L'm'){
				for(n = 0; word[i] == L'm'; n++) i++;
				if(n > 4) return 0;
				v += n * 1000;
			}
			// Преобразовываем букву D и C
			if(word[i] == L'd' || word[i] == L'c'){
				if((c = word[i]) == L'd'){
					i++;
					v += 500;
				}
				if(c == L'c' && word[i + 1] == L'm'){
					i += 2;
					v += 900;
				} else if(c == L'c' && word[i + 1] == L'd') {
					i += 2;
					v += 400;
				} else {
					for(n = 0; word[i] == L'c'; n++) i++;
					if(n > 4) return 0;
					v += n * 100;
				}
			}
			// Преобразовываем букву L и X
			if(word[i] == L'l' || word[i] == L'x'){
				if((c = word[i]) == L'l'){
					i++;
					v += 50;
				}
				if(c == L'x' && word[i + 1] == L'c'){
					i += 2;
					v += 90;
				} else if(c == L'x' && word[i + 1] == L'l') {
					i += 2;
					v += 40;
				} else {
					for(n = 0; word[i] == L'x'; n++) i++;
					if(n > 4) return 0;
					v += n * 10;
				}
			}
			// Преобразовываем букву V и I
			if(word[i] == L'v' || word[i] == L'i'){
				if((c = word[i]) == L'v'){
					i++;
					v += 5;
				}
				if(c == L'i' && word[i + 1] == L'x'){
					i += 2;
					v += 9;
				} else if(c == L'i' && word[i + 1] == L'v'){
					i += 2;
					v += 4;
				} else {
					for(n = 0; word[i] == L'i'; n++) i++;
					if(n > 4) return 0;
					v += n;
				}
			}
			// Формируем реузльтат
			result = (((word.length() == i) && (v >= 1) && (v <= 4999)) ? v : 0);
		}
	}
	// Выводим результат
	return result;
}
/**
 * count Метод получения количества букв в словаре
 * @return количество букв в словаре
 */
const size_t anyks::Alphabet::count() const {
	// Выводим результат
	return this->letters.size();
}
/**
 * setCase Метод запоминания регистра слова
 * @param pos позиция для установки регистра
 * @param cur текущее значение регистра в бинарном виде
 * @return    позиция верхнего регистра в бинарном виде
 */
const size_t anyks::Alphabet::setCase(const size_t pos, const size_t cur) const {
	// Результат работы функции
	size_t result = cur;
	// Если позиция передана и длина слова тоже
	result += (1 << pos);
	// Выводим результат
	return result;
}
/**
 * countLetter Метод подсчета количества указанной буквы в слове
 * @param word   слово в котором нужно подсчитать букву
 * @param letter букву которую нужно подсчитать
 * @return       результат подсчёта
 */
const size_t anyks::Alphabet::countLetter(const wstring & word, const wchar_t letter) const {
	// Результат работы функции
	size_t result = 0;
	// Если слово и буква переданы
	if(!word.empty() && (letter > 0)){
		// Ищем нашу букву
		size_t pos = 0;
		// Выполняем подсчет количества указанных букв в слове
		while((pos = word.find(letter, pos)) != wstring::npos){
			// Считаем количество букв
			result++;
			// Увеличиваем позицию
			pos++;
		}
	}
	// Выводим результат
	return result;
}
/**
 * altemp Метод проверки на сущестования альтернативных букв
 * @return результат проверки
 */
const bool anyks::Alphabet::altemp() const {
	// Выводим результат проверки
	return this->alters.empty();
}
/**
 * isAlt Метод проверки существования альтернативной буквы
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::Alphabet::isAlt(const wchar_t letter) const {
	// Результат работы функции
	bool result = false;
	// Если буква передана
	if((letter > 0) && !this->alters.empty()){
		// Выполняем проверку
		result = (this->alters.count(letter) > 0);
	}
	// Выводим результат
	return result;
}
/**
 * isMath Метод определения математических операий
 * @param letter буква для проверки
 * @return       результат проверки
 */
const bool anyks::Alphabet::isMath(const wchar_t letter) const {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(letter > 0){
		// Выполняем проверку
		result = (
			(letter == L'+') || (letter == L'-')
			|| (letter == L'=') || (letter == L'/')
			|| (letter == L'*') || (letter == L'^')
		);
	}
	// Выводим результат
	return result;
}
/**
 * isUpper Метод проверки символ на верхний регистр
 * @param letter буква для проверки
 * @return       результат проверки
 */
const bool anyks::Alphabet::isUpper(const wchar_t letter) const {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(letter > 0){
		// Объявляем локаль
		const locale utf8(this->encoding);
		// Если код символа не изменился, значит регистр верхний
		result = (letter == toupper(letter, utf8));
	}
	// Выводим результат
	return result;
}
/**
 * isLatian Метод проверки является ли строка латиницей
 * @param str строка для проверки
 * @return    результат проверки
 */
const bool anyks::Alphabet::isLatian(const wstring & str) const {
	// Результат работы функции
	bool result = false;
	// Если строка передана
	if(!str.empty()){
		// Длина слова
		const size_t length = str.length();
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Переходим по всем буквам слова
			for(size_t i = 0, j = length - 1; j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово латинским
				result = (
					i == j ? (this->latian.count(str[i]) > 0) :
					(this->latian.count(str[i]) > 0)
					&& (this->latian.count(str[j]) > 0)
				);
				// Если слово не соответствует тогда выходим
				if(!result) break;
			}
		// Если символ всего один, проверяем его так
		} else result = (this->latian.count(str[0]) > 0);
	}
	// Выводим результат
	return result;
}
/**
 * isPunct Метод проверки является ли буква, знаком препинания
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::Alphabet::isPunct(const wchar_t letter) const {
	// Проверяем на знак пунктуации
	return (
		(letter == L'.') ||
		(letter == L',') ||
		(letter == L'?') ||
		(letter == L'!') ||
		(letter == L':') ||
		(letter == L';')
	);
}
/**
 * isSpace Метод проверки является ли буква, пробелом
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::Alphabet::isSpace(const wchar_t letter) const {
	// Получаем код символа
	const u_short lid = letter;
	// Выводим результат
	return ((lid == 32) || (lid == 160) || (lid == 173) || (lid == 9));
}
/**
 * isNumber Метод проверки является ли слово числом
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::Alphabet::isNumber(const wstring & word) const {
	// Результат работы функции
	bool result = false;
	// Если слово передана
	if(!word.empty()){
		// Длина слова
		const size_t length = word.length();
		// Список арабских цифр
		std::set <wchar_t> numbers = {L'0', L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9'};
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Переходим по всем буквам слова
			for(size_t i = 0, j = length - 1; j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово арабским числом
				result = !(i == j ? (numbers.count(word[i]) < 1) : (numbers.count(word[i]) < 1) || (numbers.count(word[j]) < 1));
				// Если слово не соответствует тогда выходим
				if(!result) break;
			}
		// Если символ всего один, проверяем его так
		} else result = (numbers.count(word[0]) > 0);
	}
	// Выводим результат
	return result;
}
/**
 * isDecimal Метод проверки является ли слово дробным числом
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::Alphabet::isDecimal(const wstring & word) const {
	// Результат работы функции
	bool result = false;
	// Если слово передана
	if(!word.empty()){
		// Длина слова
		const size_t length = word.length();
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Переходим по всем символам слова
			for(size_t i = 0; i < length; i++){
				// Если плавающая точка найдена
				if((word[i] == L'.') || (word[i] == L',')){
					// Проверяем правые и левую части
					result = (this->isNumber(word.substr(0, i)) && this->isNumber(word.substr(i + 1)));
					// Выходим из цикла
					break;
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * isANumber Метод проверки является ли косвенно слово числом
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::Alphabet::isANumber(const wstring & word) const {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty()){
		// Проверяем является ли слово числом
		result = this->isNumber(word);
		// Если не является то проверяем дальше
		if(!result){
			// Длина слова
			const size_t length = word.length();
			// Получаем первый символ слова
			const wchar_t first = word.front();
			// Получаем последний символ слова
			const wchar_t second = word.back();
			// Проверяем являются ли первая и последняя буква слова, числом
			result = (this->isNumber({first}) || this->isNumber({second}));
			// Если оба варианта не сработали
			if(!result && (length > 2)){
				// Первое слово
				wstring first;
				// Переходим по всему списку
				for(size_t i = 1, j = length - 2; j > ((length / 2) - 1); i++, j--){
					// Получаем первое слово
					first.assign(1, word[i]);
					// Проверяем является ли слово арабским числом
					result = (i == j ? this->isNumber(first) : this->isNumber(first) || this->isNumber({word[j]}));
					// Если хоть один символ является числом, выходим
					if(result) break;
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * isAllowed Метод проверки соответствия слова словарю
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::Alphabet::isAllowed(const wstring & word) const {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty()){
		// Длина слова
		const size_t length = word.length();
		/**
		 * isHyphen Функция проверки на разделитель
		 * @param c символ для проверки
		 */
		auto isHyphen = [](const wchar_t c){
			// Выводим результат
			return (
				(c == L'~') || (c == L'-') || (c == L'+') ||
				(c == L'=') || (c == L'*') || (c == L'/') ||
				(c == L':') || (c == L'%') || (c == L'|') ||
				(c == L'^') || (c == L'&') || (c == L'#') ||
				(c == L'\\')
			);
		};
		// Если строка длиннее 1-го символа
		if(length > 1){
			// Выполняем переход по всем буквам слова
			for(size_t i = 0, j = length - 1; j > ((length / 2) - 1); i++, j--){
				// Выполняем проверку соответствия словарю каждой буквы
				result = (
					i == j ?
					(isHyphen(word[i]) || this->check(word[i])) :
					(isHyphen(word[i]) || this->check(word[i])) &&
					(isHyphen(word[j]) || this->check(word[j]))
				);
				// Если буква не соответствует, выходим
				if(!result) break;
			}
		// Если строка всего из одного символа
		} else result = (isHyphen(word[0]) || this->check(word[0]));
	}
	// Выводим результат
	return result;
}
/**
 * isSpecial Метод определения спец-символа
 * @param letter буква для проверки
 * @return       результат проверки
 */
const bool anyks::Alphabet::isSpecial(const wchar_t letter) const {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(letter > 0){
		// Выполняем проверку
		result = (
			(letter == L'~') || (letter == L'_')
			|| (letter == L'@') || (letter == L'#')
			|| (letter == L'№') || (letter == L'%')
			|| (letter == L'&') || (letter == L'$')
			|| (letter == L'|') || (letter == L'\\')
			|| (letter == L'<') || (letter == L'>')
			|| (letter == L'§') || (letter == L'©')
		);
	}
	// Выводим результат
	return result;
}
/**
 * isIsolation Метод определения знака изоляции (кавычки, скобки)
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::Alphabet::isIsolation(const wchar_t letter) const {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(letter > 0){
		// Выполняем проверку
		result = (
			(letter == L'(') || (letter == L')')
			|| (letter == L'[') || (letter == L']')
			|| (letter == L'{') || (letter == L'}')
			|| (letter == L'"') || (letter == L'\'')
			|| (letter == L'«') || (letter == L'»')
			|| (letter == L'„') || (letter == L'“')
			|| (letter == L'`')
		);
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки соответствии буквы
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::Alphabet::check(const wchar_t letter) const {
	// Результат работы функции
	bool result = false;
	// Результат проверки
	if(letter > 0){
		// Если это не число, тогда выполняем проверку
		if(!(result = this->isNumber({letter}))){
			// Переводим букву в нижний регистр
			const wstring & str = this->toLower(wstring(1, letter));
			// Выполняем проверку буквы
			result = (this->letters.count(str[0]) > 0);
		}
	}
	// Выводим результат
	return result;
}
/**
 * checkHome2 Метод проверки слова на Дом-2
 * @param  word слово для проверки
 * @return      результат работы метода
 */
const bool anyks::Alphabet::checkHome2(const wstring & word) const {
	// Результат работы функции
	bool result = false;
	// Если слово передано, первая буква не является числом а последняя это число
	if(!word.empty() && !this->isNumber({word.front()}) && this->isNumber({word.back()})){
		// Позиция дефиса в слове
		size_t pos = 0;
		// Ищим дефис в слове
		if((pos = word.rfind(L"-")) != wstring::npos){
			// Извлекаем суффикс слова
			const wstring & suffix = word.substr(pos + 1);
			// Если только суффикс является числом выводим результат
			result = (!this->isNumber(word.substr(0, pos)) && this->isNumber(suffix));
		}
	}
	// Выводим результат
	return result;
}
/**
 * checkLatian Метод проверки наличия латинских символов в строке
 * @param str строка для проверки
 * @return    результат проверки
 */
const bool anyks::Alphabet::checkLatian(const wstring & str) const {
	// Результат работы функции
	bool result = false;
	// Если строка передана
	if(!str.empty()){
		// Длина слова
		const size_t length = str.length();
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Переходим по всем буквам слова
			for(size_t i = 0, j = length - 1; j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово латинским
				result = (
					i == j ? (this->latian.count(str[i]) > 0) :
					(this->latian.count(str[i]) > 0)
					|| (this->latian.count(str[j]) > 0)
				);
				// Если найдена хотя бы одна латинская буква тогда выходим
				if(result) break;
			}
		// Если символ всего один, проверяем его так
		} else result = (this->latian.count(str[0]) > 0);
	}
	// Выводим результат
	return result;
}
/**
 * checkHyphen Метод проверки наличия дефиса в строке
 * @param str строка для проверки
 * @return    результат проверки
 */
const bool anyks::Alphabet::checkHyphen(const wstring & str) const {
	// Результат работы функции
	bool result = false;
	// Если строка передана
	if(!str.empty()){
		// Длина слова
		const size_t length = str.length();
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Переходим по всем буквам слова
			for(size_t i = 0, j = length - 1; j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово латинским
				result = (i == j ? (str[i] == L'-') : (str[i] == L'-') || (str[j] == L'-'));
				// Если найдена хотя бы одна латинская буква тогда выходим
				if(result) break;
			}
		// Если символ всего один, проверяем его так
		} else result = (str[0] == L'-');
	}
	// Выводим результат
	return result;
}
/**
 * checkSimilars Метод проверки на симиляции букв с другими языками
 * @param  str строка для проверки
 * @return     результат проверки
 */
const bool anyks::Alphabet::checkSimilars(const wstring & str) const {
	// Результат работы функции
	bool result = false;
	// Если строка передана
	if(!str.empty()){
		// Длина слова
		const size_t length = str.length();
		// Если длина строки больше 1
		if(length > 1){
			// Список букв симиляции
			std::set <wchar_t> letters = {L'a', L'b', L'c', L'e', L'h', L'k', L'm', L'o', L'p', L't', L'u', L'x'};
			/**
			 * isHyphen Функция проверки на разделитель
			 * @param c символ для проверки
			 */
			auto isHyphen = [](const wchar_t c){
				// Выводим результат
				return (
					(c == L'~') || (c == L'-') || (c == L'+') ||
					(c == L'=') || (c == L'*') || (c == L'/') ||
					(c == L':') || (c == L'%') || (c == L'|') ||
					(c == L'^') || (c == L'&') || (c == L'#') ||
					(c == L'\\')
				);
			};
			/**
			 * existFn Функция проверки на существование буквы
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			auto existFn = [&isHyphen, this](const wchar_t letter){
				// Результат работы функци
				bool result = false;
				// Если буква передана
				if(letter > 0){
					// Создаем слово для проверки
					wstring word(1, letter);
					// Выполняем проверку буквы
					result = (
						!isHyphen(letter) &&
						!this->isNumber(word) &&
						!this->isLatian(word) &&
						this->check(letter)
					);
				}
				// Выводим результат
				return result;
			};
			// Первое и второе слово
			bool first = false, second = false;
			// Переходим по всему слову
			for(size_t i = 0; i < length; i++){
				// Если буква совпала
				if(letters.count(str[i]) > 0){
					// Проверяем первую букву
					first = (i > 0 ? existFn(str[i - 1]) : false);
					// Проверяем следующую букву
					second = (i < (length - 1) ? existFn(str[i + 1]) : false);
					// Если первая или вторая буква сработали, выходим
					result = (((i == (length - 1)) && first) || ((i == 0) && second) || (first && second));
					// Выходим из цикла
					if(result) break;
				}
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * checkHypLat Метод поиска дефиса и латинского символа
 * @param str строка для проверки
 * @return    результат проверки
 */
const pair <bool, bool> anyks::Alphabet::checkHypLat(const wstring & str) const {
	// Результат работы функции
	pair <bool, bool> result = {false, false};
	// Если строка передана
	if(!str.empty()){
		// Длина слова
		const size_t length = str.length();
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Флаги поиска
			bool hyphen, latian;
			// Переходим по всем буквам слова
			for(size_t i = 0, j = length - 1; j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово латинским
				hyphen = (i == j ? (str[i] == L'-') : (str[i] == L'-') || (str[j] == L'-'));
				// Проверяем является ли слово латинским
				if(!hyphen || (i != j)) latian = (
					i == j ? (this->latian.count(str[i]) > 0) :
					(this->latian.count(str[i]) > 0)
					|| (this->latian.count(str[j]) > 0)
				);
				// Если найден дефис
				if(!result.first && hyphen) result.first = hyphen;
				// Если найден латинский символ
				if(!result.second && latian) result.second = latian;
				// Если найден и пробел и латинский символ
				if(result.first && result.second) break;
			}
		// Если символ всего один, проверяем его так
		} else {
			// Запоминаем найден ли дефис
			result.first = (str[0] == L'-');
			// Если дефис не найден, проверяем на латинский символ
			if(!result.first) result.second = (this->latian.count(str[0]) > 0);
		}
	}
	// Выводим результат
	return result;
}
/**
 * log Метод вывода текстовой информации в консоль или файл
 * @param format   формат строки вывода
 * @param flag     флаг типа логирования
 * @param filename адрес файла для вывода
 */
void anyks::Alphabet::log(const string & format, log_t flag, const char * filename, ...) const {
	// Если формат передан
	if(!format.empty()){
		// Создаем список аргументов
		va_list args;
		// Строка результата
		string str = "";
		// Создаем буфер
		char buffer[BUFFER_CHUNK];
		// Заполняем буфер нулями
		memset(buffer, 0, sizeof(buffer));
		// Устанавливаем начальный список аргументов
		va_start(args, filename);
		// Выполняем запись в буфер
		const size_t size = vsprintf(buffer, format.c_str(), args);
		// Завершаем список аргументов
		va_end(args);
		// Если размер не нулевой
		if(size > 0) str.assign(buffer, size);
		// Если строка получена
		if(!str.empty()){
			// Формируем файловый поток
			ostream * file = (filename == nullptr ? nullptr : new ofstream(filename, ios::app));
			// Формируем выходной поток
			ostream & out = (filename != nullptr ? * file : cout);
			// Определяем тип сообщения
			switch((u_short) flag){
				// Выводим информационное сообщение в консоль
				case (u_short) log_t::info: out << (filename != nullptr ? this->format("info: %s", str.c_str()) : this->format("\x1B[32m\x1B[1minfo:\x1B[0m %s", str.c_str())) << endl; break;
				// Выводим сообщение об ошибке в консоль
				case (u_short) log_t::error: out << (filename != nullptr ? this->format("error: %s", str.c_str()) : this->format("\x1B[31m\x1B[1merror:\x1B[0m %s", str.c_str())) << endl; break;
				// Выводим сообщение предупреждения в консоль
				case (u_short) log_t::warning: out << (filename != nullptr ? this->format("warning: %s", str.c_str()) : this->format("\x1B[35m\x1B[1mwarning:\x1B[0m %s", str.c_str())) << endl; break;
			}
			// Если был создан файловый поток, удаляем память
			if(file != nullptr) delete file;
		}
	}
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const word_t & str, const word_t & delim, list <word_t> & v) const {
	// Выполняем сплит строки
	::split(str, delim, v);
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const word_t & str, const word_t & delim, vector <word_t> & v) const {
	// Выполняем сплит строки
	::split(str, delim, v);
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const wstring & str, const wstring & delim, list <wstring> & v) const {
	// Выполняем сплит строки
	::split(str, delim, v);
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const wstring & str, const wstring & delim, vector <wstring> & v) const {
	// Выполняем сплит строки
	::split(str, delim, v);
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const string & str, const string & delim, list <wstring> & v) const {
	// Выполняем сплит строки
	::split(this->convert(str), this->convert(delim), v);
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const string & str, const string & delim, vector <wstring> & v) const {
	// Выполняем сплит строки
	::split(this->convert(str), this->convert(delim), v);
}
/**
 * add Метод добавления буквы в алфавит
 * @param lid идентификатор буквы для добавления
 */
void anyks::Alphabet::add(const wchar_t lid){
	// Если буква передана и такой буквы еще нет
	if((lid > 0) && (this->letters.count(lid) < 1)){
		/**
		 * isHyphen Функция проверки на разделитель
		 * @param c символ для проверки
		 */
		auto isHyphen = [](const wchar_t c){
			// Выводим результат
			return (
				(c == L'~') || (c == L'-') || (c == L'+') ||
				(c == L'=') || (c == L'*') || (c == L'/') ||
				(c == L':') || (c == L'%') || (c == L'|') ||
				(c == L'^') || (c == L'&') || (c == L'#') ||
				(c == L'\\')
			);
		};
		// Проверяем является ли буква числом
		const bool isNumber = this->isNumber({lid});
		// Добавляем букву в список
		if(!isNumber && !isHyphen(lid)) this->letters.emplace(lid);
	}
}
/**
 * set Метод добавления алфавита в словарь
 * @param alphabet алфавит символов для текущего языка
 */
void anyks::Alphabet::set(const string & alphabet){
	// Удаляем список букв
	this->letters.clear();
	// Устанавливаем знак равно
	this->add(L'=');
	// Устанавливаем знак плюс
	this->add(L'+');
	// Устанавливаем знак минус
	this->add(L'-');
	// Устанавливаем знак амперсанда
	this->add(L'&');
	// Устанавливаем знак шарпа
	this->add(L'#');
	// Устанавливаем знак умножить
	this->add(L'*');
	// Устанавливаем знак разделить
	this->add(L'/');
	// Устанавливаем знак разделить
	this->add(L':');
	// Устанавливаем знак тильда
	this->add(L'~');
	// Устанавливаем знак процентов
	this->add(L'%');
	// Устанавливаем знак вертикальной черты
	this->add(L'|');
	// Устанавливаем знак возведения в степень
	this->add(L'^');
	// Устанавливаем знак косой черты
	this->add(L'\\');
	// Запоминаем алфавит
	if(!alphabet.empty()) this->alphabet = move(this->convert(alphabet));
	// Переходим по всем буквам алфавита
	for(size_t i = 0; i < this->alphabet.length(); i++){
		// Добавляем буквы алфавита в список
		this->add(this->alphabet[i]);
	}
}
/**
 * rmalt Метод удаления альтернативной буквы
 * @param letter буква у которой есть альтернативная буква
 */
void anyks::Alphabet::rmalt(const wchar_t letter){
	// Если буква передана
	if((letter > 0) && !this->alters.empty()){
		// Удаляем выбранную букву из списка
		this->alters.erase(letter);
	// Если буква не передана то удаляем все альтернативные буквы
	} else this->alters.clear();
}
/**
 * setlocale Метод установки локали
 * @param locale локализация приложения
 */
void anyks::Alphabet::setlocale(const string & locale){
	// Устанавливаем локаль
	if(!locale.empty()){
		// Запоминаем локализацию приложения
		this->encoding = move(locale);
		// Устанавливапм локализацию приложения
		::setlocale(LC_CTYPE, this->encoding.c_str());
		::setlocale(LC_COLLATE, this->encoding.c_str());
	}
}
/**
 * setalt Метод добавления альтернативной буквы
 * @param lid буква у которой есть альтернатива
 * @param alt альтернативная буква
 */
void anyks::Alphabet::setalt(const wchar_t lid, const wchar_t alt){
	// Если буквы переданы
	if((lid > 0) && (alt > 0)){
		// Если альтернативная буква не найдена в списке
		this->alters.emplace(lid, alt);
	}
}
/**
 * tokens Метод разбивки текста на токены
 * @param text    входной текст для обработки
 * @param функция обратного вызова, на каждой итерации
 */
void anyks::Alphabet::tokens(const string & text, function <const bool (const wstring &, const vector <string> &, const bool, const bool)> callback) const {
	// Если текст передан, и текст не больше 100Mb в одну строку
	if(!text.empty() && (text.size() <= MAX_STRING_BYTES)){
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
		// Устанавливаем локаль приложения
		const locale utf8(this->encoding);
		// Получаем буквы алфавита
		const wstring & letters = this->convert(this->get());
		// Текущая буква и следующий символ
		wchar_t letter = 0, lletter = 0, backLetter = 0, next = 0;
		// Основные флаги токенизации
		bool open = false, begin = false, end = false, nend = false;
		bool backPunct = false, stopToken = false, notClear = false;
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
		const wstring & tmp = this->convert(text);
		// Выполняем поиск координат в тексте
		const auto coordinates = coordinatesFn(tmp);
		// Переходим по всему тексту
		for(size_t i = 0; i < tmp.length(); i++){
			// Получаем значение текущей буквы
			letter = tmp[i];
			// Переводим букву в нижний регистр
			lletter = tolower(letter, utf8);
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
				next = tolower(tmp[i + 1], utf8);
				// Если следующий символ является концом строки
				if((i + 1) == (tmp.length() - 1)) nend = true;
				// Если следующий символ является знаком препинания
				if(this->isPunct(next)) type = type_t::punct;
				// Если следующий символ является математической операцией
				else if(this->isMath(next)) type = type_t::math;
				// Если следующий символ является пробелом
				else if(this->isSpace(next)) type = type_t::space;
				// Если следующий символ является спец-символом
				else if(this->isSpecial(next)) type = type_t::specl;
				// Если следующий символ является числом
				else if(this->isNumber({next})) type = type_t::num;
				// Если следующий символ является символом изоляции
				else if(this->isIsolation(next)) type = type_t::isolat;
				// Если следующий символ является разрешённым
				else if(this->check(next)) type = type_t::allow;
				// Иначе зануляем следующий тип
				else type = type_t::null;
				// Если это пробел и слово не пустое
				if(this->isSpace(lletter) && !word.empty()){
					// Выводим полученное слово
					if(!callbackFn(word, end)) return;
					// Добавляем слово в контекст
					context.push_back(this->convert(word));
					// Очищаем слово
					word.clear();
				// Если это знак пунктуации
				} else if(this->isPunct(lletter) || this->isSpecial(lletter) || this->isMath(lletter)) {
					// Проверяем следующие символы на стоп-токены
					stopToken = (
						(type == type_t::math) ||
						(type == type_t::space) ||
						(type == type_t::specl) ||
						(type == type_t::punct) ||
						(type == type_t::isolat)
					);
					// Проверяем является ли предыдущий символ также знаком пунктуации
					backPunct = ((backLetter > 0) && this->isPunct(backLetter));
					// Выводим результат как он есть
					if(end){
						// Если слово не пустое
						if(!word.empty()){
							// Если это спец-символ
							if(this->isSpecial(lletter)){
								// Добавляем букву в слово
								word.append(1, letter);
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
							// Если это не спец-символ а математическая операция или знак препинания
							} else {
								// Выводим полученное слово
								if(!callbackFn(word, !end)) return;
								// Добавляем слово в контекст
								context.push_back(this->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
							}
						// Выводим знак препинания
						} else if(!callbackFn({letter}, end)) return;
					// Если слово не существует
					} else if(word.empty() && !this->isPunct(lletter) &&
					!(((lletter == L'-') || (lletter == L'+') ||
					(lletter == L'*')) && (type == type_t::num))) {
						// Выводим знак препинания
						if(!callbackFn({letter}, end)) return;
						// Добавляем знак препинания в контекст
						context.push_back(this->convert(wstring(1, letter)));
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
							(!this->isUpper(sumbol) || (word.length() == 1) ||
							(letters.find(tolower(sumbol)) == wstring::npos)))
						);
						// Если следующий символ является пробелом или нормальным словом
						if((lletter != L'/') && (((type == type_t::allow) &&
						((lletter != L'.') || !abbr)) || ((stopToken && !abbr) || backPunct))){
							// Выводим полученное слово
							if(!callbackFn(word, end)) return;
							// Добавляем слово в контекст
							if(!word.empty()) context.push_back(this->convert(word));
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Добавляем знак препинания в контекст
							context.push_back(this->convert(wstring(1, letter)));
							// Иначе очищаем контекст
							if(!notClear && (!backPunct || !!this->isUpper(sumbol)) &&
							(lletter == L'.') && (type != type_t::punct)) context.clear();
							// Очищаем слово
							word.clear();
						// Если это не пробел
						} else if(!this->isSpace(lletter)) {
							// Если следующий символ является концом текста или стоп-токеном
							if(nend && stopToken){
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Добавляем слово в контекст
								if(!word.empty()) context.push_back(this->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Добавляем знак препинания в контекст
								context.push_back(this->convert(wstring(1, letter)));
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
								if(!word.empty()) context.push_back(this->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Добавляем знак препинания в контекст
								context.push_back(this->convert(wstring(1, letter)));
								// Иначе очищаем контекст
								if(!notClear && !nend && (type != type_t::punct)
								&& (type != type_t::isolat)) context.clear();
								// Очищаем слово
								word.clear();
							}
						// Если это не пробел
						} else if(!this->isSpace(lletter)) {
							// Если следующий символ является концом текста или стоп-токеном
							if(nend && stopToken){
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Добавляем слово в контекст
								if(!word.empty()) context.push_back(this->convert(word));
								// Выводим знак препинания
								if(!callbackFn({letter}, end)) return;
								// Добавляем знак препинания в контекст
								context.push_back(this->convert(wstring(1, letter)));
								// Очищаем слово
								word.clear();
							// Иначе добавляем символ в словарь
							} else word.append(1, letter);
						}
					// Если это спец-символ
					} else if(this->isSpecial(lletter) || this->isMath(lletter)) {
						// Добавляем символ в слово
						if(!this->isSpace(lletter)) word.append(1, letter);
						// Если следующий символ является пробелом
						if(type == type_t::space){
							// Выводим полученное слово
							if(!callbackFn(word, end)) return;
							// Добавляем слово в контекст
							if(!word.empty()) context.push_back(this->convert(word));
							// Очищаем слово
							word.clear();
						}
					}
				// Если это изоляционный символ
				} else if(this->isIsolation(lletter)) {
					// Выводим результат как он есть
					if(end){
						// Если слово не пустое
						if(!word.empty()){
							// Выводим полученное слово
							if(!callbackFn(word, !end)) return;
							// Добавляем слово в контекст
							if(open) context.push_back(this->convert(word));
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
								context.push_back(this->convert(word));
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Очищаем слово
								word.clear();
							}
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Добавляем знак препинания в контекст
							context.push_back(this->convert(wstring(1, letter)));
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
							if(!word.empty()) context.push_back(this->convert(word));
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Очищаем слово
							word.clear();
							// Добавляем знак препинания в контекст
							context.push_back(this->convert(wstring(1, letter)));
							// Если разрешено продолжить
							if(mode) brackets.pop();
							// Если больше символов изоляции не открыто, разрешаем очистку контекста
							notClear = !brackets.empty();
						// Если это открытие изоляционного знака
						} else if((open = !open)) {
							// Если слово не пустое
							if(!word.empty()){
								// Добавляем слово в контекст
								context.push_back(this->convert(word));
								// Выводим полученное слово
								if(!callbackFn(word, end)) return;
								// Очищаем слово
								word.clear();
							}
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Добавляем знак препинания в контекст
							if(!this->isSpace(letter)) context.push_back(this->convert(wstring(1, letter)));
						// Если это закрытие изоляционного знака
						} else {
							// Если слово не пустое
							if(!word.empty() && !callbackFn(word, end)) return;
							// Добавляем слово в контекст
							if(!word.empty()) context.push_back(this->convert(word));
							// Выводим знак препинания
							if(!callbackFn({letter}, end)) return;
							// Очищаем слово
							word.clear();
							// Добавляем знак препинания в контекст
							context.push_back(this->convert(wstring(1, letter)));
						}
					}
				// Если это другие символы
				} else {
					// Если это первая буква в первом слове
					if(context.empty() && word.empty())
						// Приводим букву к верхнему регистру
						letter = toupper(letter, utf8);
					// Формируем слово
					if(!this->isSpace(lletter)) word.append(1, letter);
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
 * Alphabet Конструктор
 * @param alphabet алфавит символов для текущего языка
 * @param locale   локализация приложения
 */
anyks::Alphabet::Alphabet(const string & alphabet, const string & locale){
	// Устанавливаем локализацию системы
	this->setlocale(locale);
	// Переходим по всем буквам алфавита
	for(auto & letter : this->alphabet){
		// Добавляем буквы в латинский алфавит
		this->latian.emplace(letter);
	}
	// Если алфавит передан
	if(!alphabet.empty())
		// Устанавливаем переданный алфавит
		this->set(alphabet);
	// Устанавливаем алфавит по умолчанию
	else this->set();
}

/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <alphabet.hpp>


// Устанавливаем шаблон функции
template <typename T>
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void split(const wstring & str, const wstring & delim, T & v) noexcept {
	/**
	 * trimFn Метод удаления пробелов вначале и конце текста
	 * @param text текст для удаления пробелов
	 * @return     результат работы функции
	 */
	function <const wstring (const wstring &)> trimFn = [](const wstring & text) noexcept {
		// Получаем временный текст
		wstring tmp = text;
		// Выполняем удаление пробелов по краям
		tmp.erase(tmp.begin(), find_if_not(tmp.begin(), tmp.end(), [](wchar_t c){ return iswspace(c); }));
		tmp.erase(find_if_not(tmp.rbegin(), tmp.rend(), [](wchar_t c){ return iswspace(c); }).base(), tmp.end());
		// Выводим результат
		return tmp;
	};
	// Очищаем словарь
	v.clear();
	// Получаем счётчики перебора
	size_t i = 0, j = str.find(delim);
	const size_t len = delim.length();
	// Выполняем разбиение строк
	while(j != wstring::npos){
		v.push_back(trimFn(str.substr(i, j - i)));
		i = ++j + (len - 1);
		j = str.find(delim, j);
		if(j == wstring::npos) v.push_back(trimFn(str.substr(i, str.length())));
	}
	// Если слово передано а вектор пустой, тогда создаем вектори из 1-го элемента
	if(!str.empty() && v.empty()) v.push_back(trimFn(str));
}
/**
 * cbegin Метод итератор начала списка
 * @return итератор
 */
const std::set <wchar_t>::const_iterator anyks::Alphabet::cbegin() const noexcept {
	// Выводим итератор
	return this->letters.cbegin();
}
/**
 * cend Метод итератор конца списка
 * @return итератор
 */
const std::set <wchar_t>::const_iterator anyks::Alphabet::cend() const noexcept {
	// Выводим итератор
	return this->letters.cend();
}
/**
 * crbegin Метод обратный итератор начала списка
 * @return итератор
 */
const std::set <wchar_t>::const_reverse_iterator anyks::Alphabet::crbegin() const noexcept {
	// Выводим итератор
	return this->letters.crbegin();
}
/**
 * crend Метод обратный итератор конца списка
 * @return итератор
 */
const std::set <wchar_t>::const_reverse_iterator anyks::Alphabet::crend() const noexcept {
	// Выводим итератор
	return this->letters.crend();
}
/**
 * get Метод получения алфавита языка
 * @return алфавит языка
 */
const string anyks::Alphabet::get() const noexcept {
	// Выводим результат
	return this->convert(this->alphabet);
}
/**
 * get Метод получения алфавита языка
 * @return алфавит языка
 */
const wstring & anyks::Alphabet::wget() const noexcept {
	// Выводим результат
	return this->alphabet;
}
/**
 * trim Метод удаления пробелов вначале и конце текста
 * @param text текст для удаления пробелов
 * @return     результат работы функции
 */
const string anyks::Alphabet::trim(const string & text) const noexcept {
	// Получаем временный текст
	string tmp = text;
	// Выполняем удаление пробелов по краям
	tmp.erase(tmp.begin(), find_if_not(tmp.begin(), tmp.end(), [](char c){ return isspace(c); }));
	tmp.erase(find_if_not(tmp.rbegin(), tmp.rend(), [](char c){ return isspace(c); }).base(), tmp.end());
	// Выводим результат
	return tmp;
}
/**
 * toLower Метод перевода русских букв в нижний регистр
 * @param str строка для перевода
 * @return    строка в нижнем регистре
 */
const string anyks::Alphabet::toLower(const string & str) const noexcept {
	// Результат работы функции
	string result = str;
	// Если строка передана
	if(!str.empty()){
		// Получаем временную строку
		wstring tmp = this->convert(result);
		// Переходим по всем символам
		for(auto & c : tmp) c = towlower(c);
		// Конвертируем обратно
		result = this->convert(tmp);
	}
	// Выводим результат
	return result;
}
/**
 * toUpper Метод перевода русских букв в верхний регистр
 * @param str строка для перевода
 * @return    строка в верхнем регистре
 */
const string anyks::Alphabet::toUpper(const string & str) const noexcept {
	// Результат работы функции
	string result = str;
	// Если строка передана
	if(!str.empty()){
		// Получаем временную строку
		wstring tmp = this->convert(result);
		// Переходим по всем символам
		for(auto & c : tmp) c = towupper(c);
		// Конвертируем обратно
		result = this->convert(tmp);
	}
	// Выводим результат
	return result;
}
/**
 * convert Метод конвертирования строки utf-8 в строку
 * @param  str строка utf-8 для конвертирования
 * @return     обычная строка
 */
const string anyks::Alphabet::convert(const wstring & str) const noexcept {
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
const string anyks::Alphabet::format(const char * format, ...) const noexcept {
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
 * toLower Метод перевода русских букв в нижний регистр
 * @param str строка для перевода
 * @return    строка в нижнем регистре
 */
const char anyks::Alphabet::toLower(const char letter) const noexcept {
	// Результат работы функции
	char result = 0;
	// Если строка передана
	if(letter > 0){
		// Строка для конвертации
		wstring str = L"";
		// Объявляем конвертер
		wstring_convert <codecvt_utf8 <wchar_t>> conv;
		// Выполняем конвертирование в utf-8 строку
		const wchar_t c = conv.from_bytes({1, letter}).front();
		// Формируем новую строку
		str.assign(1, towlower(c));
		// Выполняем конвертирование в utf-8 строку
		result = conv.to_bytes(str).front();
	}
	// Выводим результат
	return result;
}
/**
 * toUpper Метод перевода русских букв в верхний регистр
 * @param str строка для перевода
 * @return    строка в верхнем регистре
 */
const char anyks::Alphabet::toUpper(const char letter) const noexcept {
	// Результат работы функции
	char result = 0;
	// Если строка передана
	if(letter > 0){
		// Строка для конвертации
		wstring str = L"";
		// Объявляем конвертер
		wstring_convert <codecvt_utf8 <wchar_t>> conv;
		// Выполняем конвертирование в utf-8 строку
		const wchar_t c = conv.from_bytes({1, letter}).front();
		// Формируем новую строку
		str.assign(1, towupper(c));
		// Выполняем конвертирование в utf-8 строку
		result = conv.to_bytes(str).front();
	}
	// Выводим результат
	return result;
}
/**
 * toLower Метод перевода русских букв в нижний регистр
 * @param str строка для перевода
 * @return    строка в нижнем регистре
 */
const wchar_t anyks::Alphabet::toLower(const wchar_t letter) const noexcept {
	// Результат работы функции
	wchar_t result = 0;
	// Если строка передана
	if(letter > 0) result = towlower(letter);
	// Выводим результат
	return result;
}
/**
 * toUpper Метод перевода русских букв в верхний регистр
 * @param str строка для перевода
 * @return    строка в верхнем регистре
 */
const wchar_t anyks::Alphabet::toUpper(const wchar_t letter) const noexcept {
	// Результат работы функции
	wchar_t result = 0;
	// Если строка передана
	if(letter > 0) result = towupper(letter);
	// Выводим результат
	return result;
}
/**
 * trim Метод удаления пробелов вначале и конце текста
 * @param text текст для удаления пробелов
 * @return     результат работы функции
 */
const wstring anyks::Alphabet::trim(const wstring & text) const noexcept {
	// Получаем временный текст
	wstring tmp = text;
	// Выполняем удаление пробелов по краям
	tmp.erase(tmp.begin(), find_if_not(tmp.begin(), tmp.end(), [this](wchar_t c){ return this->isSpace(c); }));
	tmp.erase(find_if_not(tmp.rbegin(), tmp.rend(), [this](wchar_t c){ return this->isSpace(c); }).base(), tmp.end());
	// Выводим результат
	return tmp;
}
/**
 * convert Метод конвертирования строки в строку utf-8
 * @param  str строка для конвертирования
 * @return     строка в utf-8
 */
const wstring anyks::Alphabet::convert(const string & str) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если строка передана
	if(!str.empty()){
		// Объявляем конвертер
		wstring_convert <codecvt_utf8 <wchar_t>> conv;
		// Выполняем конвертирование в utf-8 строку
		result.assign(conv.from_bytes(str));
	}
	// Выводим результат
	return result;
}
/**
 * toLower Метод перевода русских букв в нижний регистр
 * @param str строка для перевода
 * @return    строка в нижнем регистре
 */
const wstring anyks::Alphabet::toLower(const wstring & str) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если строка передана
	if(!str.empty()){
		// Переходим по всем символам
		for(auto & c : str) result.append(1, towlower(c));
	}
	// Выводим результат
	return result;
}
/**
 * toUpper Метод перевода русских букв в верхний регистр
 * @param str строка для перевода
 * @return    строка в верхнем регистре
 */
const wstring anyks::Alphabet::toUpper(const wstring & str) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если строка передана
	if(!str.empty()){
		// Переходим по всем символам
		for(auto & c : str) result.append(1, towupper(c));
	}
	// Выводим результат
	return result;
}
/**
 * arabic2Roman Метод перевода арабских чисел в римские
 * @param  number арабское число от 1 до 4999
 * @return        римское число
 */
const wstring anyks::Alphabet::arabic2Roman(const u_int number) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если число передано верное
	if((number >= 1) && (number <= 4999)){
		// Копируем полученное число
		u_int n = number;
		// Вычисляем до тысяч
		result.append(this->numsSymbols.m[floor(n / 1000)]);
		// Уменьшаем диапазон
		n %= 1000;
		// Вычисляем до сотен
		result.append(this->numsSymbols.c[floor(n / 100)]);
		// Вычисляем до сотен
		n %= 100;
		// Вычисляем до десятых
		result.append(this->numsSymbols.x[floor(n / 10)]);
		// Вычисляем до сотен
		n %= 10;
		// Формируем окончательный результат
		result.append(this->numsSymbols.i[n]);
	}
	// Выводим результат
	return result;
}
/**
 * arabic2Roman Метод перевода арабских чисел в римские
 * @param  word арабское число от 1 до 4999
 * @return      римское число
 */
const wstring anyks::Alphabet::arabic2Roman(const wstring & word) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если слово передано
	if(!word.empty()){
		// Преобразуем слово в число
		const u_int number = stoi(word);
		// Выполняем расчет
		result.assign(this->arabic2Roman(number));
	}
	// Выводим результат
	return result;
}
/**
 * delPunctInWord Метод очистки текста от всех знаков препинаний
 * @param word слово для очистки
 * @return     текст без запрещенных символов
 */
const wstring anyks::Alphabet::delPunctInWord(const wstring & word) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если строка передана
	if(!word.empty()){
		// Выполняем копирование строки
		result.assign(word);
		// Выполняем удаление всех знаков препинания
		result.erase(remove_if(result.begin(), result.end(), [this](const wchar_t c) noexcept {
			// Если это буква или цифра или дефис
			return ((this->allowedSymbols.count(c) < 1) && (c != L'\r') && (c != L'\n') && !iswalnum(c) && !iswspace(c));
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
const wstring anyks::Alphabet::delBrokenInWord(const wstring & word) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если строка передана
	if(!word.empty()){
		// Выполняем копирование строки
		result.assign(word);
		// Выполняем удаление всех знаков препинания
		result.erase(remove_if(result.begin(), result.end(), [this](const wchar_t c) noexcept {
			// Выполняем конвертацию символа
			const wchar_t letter = towlower(c);
			// Если это буква или цифра или дефис
			return (
				(this->allowedSymbols.count(letter) < 1) &&
				(letter != L'\r') && (letter != L'\n') &&
				!iswspace(letter) && !this->check(letter)
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
const wstring anyks::Alphabet::delHyphenInWord(const wstring & word) const noexcept {
	// Результат работы функции
	wstring result = L"";
	// Если слово передано
	if(!word.empty()){
		// Позиция пробела в слове
		size_t pos = 0;
		// Выполняем копирование строки
		result.assign(word);
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
 * replace Метод замены в тексте слово на другое слово
 * @param  text текст в котором нужно произвести замену
 * @param  word слово для поиска
 * @param  alt  слово на которое нужно произвести замену
 * @return      результирующий текст
 */
const wstring anyks::Alphabet::replace(const wstring & text, const wstring & word, const wstring & alt) const noexcept {
	// Результат работы функции
	wstring result = move(text);
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
const u_short anyks::Alphabet::errors(const wstring & word) const noexcept {
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
 * roman2Arabic Метод перевода римских цифр в арабские
 * @param  word римское число
 * @return      арабское число
 */
const u_short anyks::Alphabet::roman2Arabic(const wstring & word) const noexcept {
	// Результат работы функции
	u_short result = 0;
	// Если слово передано
	if(!word.empty()){
		// Символ поиска
		wchar_t c, o;
		// Вспомогательные переменные
		u_int i = 0, v = 0, n = 0;
		// Получаем длину слова
		const size_t length = word.length();
		// Если слово состоит всего из одной буквы
		if((length == 1) && (this->numsSymbols.roman.count(word.front()) < 1)) return result;
		// Если слово длиннее одной буквы
		else {
			// Переходим по всем буквам слова
			for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово римским числом
				if(!(i == j ? (this->numsSymbols.roman.count(word.at(i)) > 0) :
					(this->numsSymbols.roman.count(word.at(i)) > 0) &&
					(this->numsSymbols.roman.count(word.at(j)) > 0)
				)) return result;
			}
		}
		// Преобразовываем цифру M
		if(word.front() == L'm'){
			for(n = 0; word[i] == L'm'; n++) i++;
			if(n > 4) return 0;
			v += n * 1000;
		}
		o = word[i];
		// Преобразовываем букву D и C
		if((o == L'd') || (o == L'c')){
			if((c = o) == L'd'){
				i++;
				v += 500;
			}
			o = word[i + 1];
			if((c == L'c') && (o == L'm')){
				i += 2;
				v += 900;
			} else if((c == L'c') && (o == L'd')) {
				i += 2;
				v += 400;
			} else {
				for(n = 0; word[i] == L'c'; n++) i++;
				if(n > 4) return 0;
				v += n * 100;
			}
		}
		o = word[i];
		// Преобразовываем букву L и X
		if((o == L'l') || (o == L'x')){
			if((c = o) == L'l'){
				i++;
				v += 50;
			}
			o = word[i + 1];
			if((c == L'x') && (o == L'c')){
				i += 2;
				v += 90;
			} else if((c == L'x') && (o == L'l')) {
				i += 2;
				v += 40;
			} else {
				for(n = 0; word[i] == L'x'; n++) i++;
				if(n > 4) return 0;
				v += n * 10;
			}
		}
		o = word[i];
		// Преобразовываем букву V и I
		if((o == L'v') || (o == L'i')){
			if((c = o) == L'v'){
				i++;
				v += 5;
			}
			o = word[i + 1];
			if((c == L'i') && (o == L'x')){
				i += 2;
				v += 9;
			} else if((c == L'i') && (o == L'v')){
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
	// Выводим результат
	return result;
}
/**
 * count Метод получения количества букв в словаре
 * @return количество букв в словаре
 */
const size_t anyks::Alphabet::count() const noexcept {
	// Выводим результат
	return this->letters.size();
}
/**
 * setCase Метод запоминания регистра слова
 * @param pos позиция для установки регистра
 * @param cur текущее значение регистра в бинарном виде
 * @return    позиция верхнего регистра в бинарном виде
 */
const size_t anyks::Alphabet::setCase(const size_t pos, const size_t cur) const noexcept {
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
const size_t anyks::Alphabet::countLetter(const wstring & word, const wchar_t letter) const noexcept {
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
 * isAllowApostrophe Метод проверки разрешения апострофа
 * @return результат проверки
 */
const bool anyks::Alphabet::isAllowApostrophe() const noexcept {
	// Выводим результат проверки апострофа
	return this->apostrophe;
}
/**
 * isUrl Метод проверки соответствия слова url адресу
 * @param word слово для проверки
 * @return     результат проверки
 */
const bool anyks::Alphabet::isUrl(const wstring & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty()){
		// Выполняем парсинг uri адреса
		this->uri.parse(word);
		// Извлекаем данные uri адреса
		auto resUri = this->uri.get();
		// Если ссылка найдена
		result = ((resUri.type != uri_t::types_t::null) && (resUri.type != uri_t::types_t::wrong));
	}
	// Выводим результат
	return result;
}
/**
 * isMath Метод определения математических операий
 * @param letter буква для проверки
 * @return       результат проверки
 */
const bool anyks::Alphabet::isMath(const wchar_t letter) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(letter > 0) result = (this->mathSymbols.count(letter) > 0);
	// Выводим результат
	return result;
}
/**
 * isAbbr Метод проверки слова на соответствие аббревиатуры
 * @param word слово для проверки
 * @return     результат проверки
 */
const bool anyks::Alphabet::isAbbr(const wstring & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty() && (word.length() > 1) && (word.front() != L'-') && (word.back() != L'-')){
		// Позиция пробела
		size_t pos = wstring::npos;
		// Ищем дефис в слове
		if((pos = word.find(L'-')) != wstring::npos){
			// Если первая часть является числом
			if(this->isNumber(word.substr(0, pos))){
				/**
				 * isAllowedFn Метод проверки соответствия слова словарю
				 * @param  word слово для проверки
				 * @return      результат проверки
				 */
				auto isAllowedFn = [this](const wstring & word) noexcept {
					// Результат работы функции
					bool result = false;
					// Если слово передано
					if(!word.empty()){
						// Длина слова
						const size_t length = word.length();
						// Переводим слово в нижний регистр
						const wstring & tmp = this->toLower(word);
						// Если строка длиннее 1-го символа
						if(length > 1){
							// Текущая буква
							wchar_t letter = 0;
							// Выполняем переход по всем буквам слова
							for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
								// Получаем текущую букву
								letter = tmp.at(i);
								// Проверяем соответствует ли буква
								result = (this->letters.count(letter) > 0);
								// Проверяем вторую букву в слове
								if(result && (i != j)){
									// Получаем текущую букву
									letter = tmp.at(j);
									// Проверяем соответствует ли буква
									result = (this->letters.count(letter) > 0);
								}
								// Если буква не соответствует, выходим
								if(!result) break;
							}
						// Если строка всего из одного символа
						} else result = (this->letters.count(tmp.front()) > 0);
					}
					// Выводим результат
					return result;
				};
				// Получаем суффикс
				const wstring & suffix = this->toLower(word.substr(pos + 1));
				// Если суффикс не является числом
				result = (!this->isNumber(suffix) && isAllowedFn(suffix));
			}
		// Если это не дефис
		} else {
			// Переводим слово в нижний регистр
			const wstring & tmp = this->toLower(word);
			// Если первый символ является разрешённым
			if(this->letters.count(tmp.front()) > 0){
				// Флаг найденой точки
				bool point = false;
				// Длина слова до точки
				u_short length = 1;
				// Текущая буква в слове
				wchar_t letter = 0;
				// Выполняем переход по всему слову
				for(size_t i = 1; i < tmp.length(); i++){
					// Получаем текущую букву слова
					letter = tmp.at(i);
					// Если это не точка
					if((result = (letter == L'.'))){
						// Сбрасываем длину слова
						length = 0;
						// Запоминаем что точка найдена
						point = true;
					// Если слово не прошло проверку
					} else if((length > 3) || (this->letters.count(letter) < 1)) {
						// Запоминаем, что результат не получен
						result = false;
						// Выходим из цикла
						break;
					// Увеличиваем количество обработанных букв
					} else if((result = true)) length++;
				}
				// Если точка не найдена, сбрасываем результат
				if(!point) result = false;
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * isUpper Метод проверки символ на верхний регистр
 * @param letter буква для проверки
 * @return       результат проверки
 */
const bool anyks::Alphabet::isUpper(const wchar_t letter) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(letter > 0){
		// Если код символа не изменился, значит регистр верхний
		result = (wint_t(letter) == towupper(letter));
	}
	// Выводим результат
	return result;
}
/**
 * isLatian Метод проверки является ли строка латиницей
 * @param str строка для проверки
 * @return    результат проверки
 */
const bool anyks::Alphabet::isLatian(const wstring & str) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если строка передана
	if(!str.empty()){
		// Длина слова
		const size_t length = str.length();
		// Переводим слово в нижний регистр
		const wstring & tmp = this->toLower(str);
		// Если длина слова больше 1-го символа
		if(length > 1){
			/**
			 * checkFn Функция проверки на валидность символа
			 * @param text  текст для проверки
			 * @param index индекс буквы в слове
			 * @return      результат проверки
			 */
			auto checkFn = [this](const wstring & text, const size_t index) noexcept {
				// Результат работы функции
				bool result = false;
				// Получаем текущую букву
				const wchar_t letter = text.at(index);
				// Если буква не первая и не последняя
				if((index > 0) && (index < (text.length() - 1))){
					// Получаем предыдущую букву
					const wchar_t first = text.at(index - 1);
					// Получаем следующую букву
					const wchar_t second = text.at(index + 1);
					// Если это дефис
					result = ((letter == L'-') && (first != L'-') && (second != L'-'));
					// Если проверка не пройдена, проверяем на апостроф
					if(!result){
						// Выполняем проверку на апостроф
						result = (
							(letter == L'\'') && ((this->isAllowApostrophe() && ((first != L'\'') && (second != L'\''))) ||
							((this->latian.count(first) > 0) && (this->latian.count(second) > 0)))
						);
					}
					// Если результат не получен
					if(!result) result = (this->latian.count(letter) > 0);
				// Выводим проверку как она есть
				} else result = (this->latian.count(letter) > 0);
				// Выводим результат
				return result;
			};
			// Переходим по всем буквам слова
			for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово латинским
				result = (i == j ? checkFn(tmp, i) : checkFn(tmp, i) && checkFn(tmp, j));
				// Если слово не соответствует тогда выходим
				if(!result) break;
			}
		// Если символ всего один, проверяем его так
		} else result = (this->latian.count(tmp.front()) > 0);
	}
	// Выводим результат
	return result;
}
/**
 * isPunct Метод проверки является ли буква, знаком препинания
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::Alphabet::isPunct(const wchar_t letter) const noexcept {
	// Проверяем на знак пунктуации
	return (this->punctsSymbols.count(letter) > 0);
}
/**
 * isSpace Метод проверки является ли буква, пробелом
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::Alphabet::isSpace(const wchar_t letter) const noexcept {
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
const bool anyks::Alphabet::isNumber(const wstring & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передана
	if(!word.empty()){
		// Длина слова
		const size_t length = word.length();
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Переходим по всем буквам слова
			for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово арабским числом
				result = !(
					i == j ? (this->numsSymbols.arabs.count(word.at(i)) < 1) :
					(this->numsSymbols.arabs.count(word.at(i)) < 1) ||
					(this->numsSymbols.arabs.count(word.at(j)) < 1)
				);
				// Если слово не соответствует тогда выходим
				if(!result) break;
			}
		// Если символ всего один, проверяем его так
		} else result = (this->numsSymbols.arabs.count(word.front()) > 0);
	}
	// Выводим результат
	return result;
}
/**
 * isDecimal Метод проверки является ли слово дробным числом
 * @param  word слово для проверки
 * @return      результат проверки
 */
const bool anyks::Alphabet::isDecimal(const wstring & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передана
	if(!word.empty()){
		// Длина слова
		const size_t length = word.length();
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Текущая буква
			wchar_t letter = 0;
			// Начальная позиция поиска
			const u_short pos = ((word.front() == L'-') || (word.front() == L'+') ? 1 : 0);
			// Переходим по всем символам слова
			for(size_t i = pos; i < length; i++){
				// Получаем текущую букву
				letter = word.at(i);
				// Если плавающая точка найдена
				if((letter == L'.') || (letter == L',')){
					// Проверяем правые и левую части
					result = (this->isNumber(word.substr(pos, i - 1)) && this->isNumber(word.substr(i + 1)));
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
const bool anyks::Alphabet::isANumber(const wstring & word) const noexcept {
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
			// Проверяем являются ли первая и последняя буква слова, числом
			result = (this->isNumber({word.front()}) || this->isNumber({word.back()}));
			// Если оба варианта не сработали
			if(!result && (length > 2)){
				// Первое слово
				wstring first = L"";
				// Переходим по всему списку
				for(size_t i = 1, j = length - 2; j > ((length / 2) - 1); i++, j--){
					// Получаем первое слово
					first.assign(1, word.at(i));
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
const bool anyks::Alphabet::isAllowed(const wstring & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty()){
		// Длина слова
		const size_t length = word.length();
		// Переводим слово в нижний регистр
		const wstring & tmp = this->toLower(word);
		// Если строка длиннее 1-го символа
		if(length > 1){
			/**
			 * checkFn Функция проверки на валидность символа
			 * @param text  текст для проверки
			 * @param index индекс буквы в слове
			 * @return      результат проверки
			 */
			auto checkFn = [this](const wstring & text, const size_t index) noexcept {
				// Результат работы функции
				bool result = false;
				// Получаем текущую букву
				const wchar_t letter = text.at(index);
				// Если буква не первая и не последняя
				if((index > 0) && (index < (text.length() - 1))){
					// Получаем предыдущую букву
					const wchar_t first = text.at(index - 1);
					// Получаем следующую букву
					const wchar_t second = text.at(index + 1);
					// Если это дефис
					result = ((letter == L'-') && (first != L'-') && (second != L'-'));
					// Если проверка не пройдена, проверяем на апостроф
					if(!result){
						// Выполняем проверку на апостроф
						result = (
							(letter == L'\'') && ((this->isAllowApostrophe() && ((first != L'\'') && (second != L'\''))) ||
							((this->latian.count(first) > 0) && (this->latian.count(second) > 0)))
						);
					}
					// Если результат не получен
					if(!result) result = this->check(letter);
				// Выводим проверку как она есть
				} else result = this->check(letter);
				// Выводим результат
				return result;
			};
			// Выполняем переход по всем буквам слова
			for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово разрешённым
				result = (i == j ? checkFn(tmp, i) : checkFn(tmp, i) && checkFn(tmp, j));
				// Если слово не соответствует тогда выходим
				if(!result) break;
			}
		// Если строка всего из одного символа
		} else result = this->check(tmp.front());
	}
	// Выводим результат
	return result;
}
/**
 * isSpecial Метод определения спец-символа
 * @param letter буква для проверки
 * @return       результат проверки
 */
const bool anyks::Alphabet::isSpecial(const wchar_t letter) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(letter > 0) result = (this->specialSymbols.count(letter) > 0);
	// Выводим результат
	return result;
}
/**
 * isIsolation Метод определения знака изоляции (кавычки, скобки)
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::Alphabet::isIsolation(const wchar_t letter) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(letter > 0) result = (this->isolationSymbols.count(letter) > 0);
	// Выводим результат
	return result;
}
/**
 * rest Метод исправления и детектирования слов со смешенными алфавитами
 * @param  word слово для проверки и исправления
 * @return      результат проверки
 */
const bool anyks::Alphabet::rest(wstring & word) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty() && (word.length() > 1)){
		// Список остальных символов
		multimap <wchar_t, u_short> other;
		// Список латинских букв
		multimap <wchar_t, u_short> latian;
		// Список нормального алфавита
		multimap <wchar_t, u_short> normal;
		/**
		 * setFn Метод установки полученной буквы
		 * @param letter буква для установки
		 * @param pos    позиция буквы в слове
		 */
		auto setFn = [&other, &latian, &normal, this](const wchar_t letter, const u_short pos) noexcept {
			// Если это латинский алфавит
			if(this->checkLatian({letter})) latian.emplace(letter, pos);
			// Если это нормальная буква
			else if(!this->typeLatian && (this->letters.count(letter) > 0)) normal.emplace(letter, pos);
			// Иначе это другие символы
			else other.emplace(letter, pos);
		};
		// Получаем длину слова
		const size_t length = word.length();
		// Выполняем переход по всем буквам слова
		for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
			// Если это центр слова
			if(i == j) setFn(word.at(i), i);
			// Если это разные стороны слова
			else {
				// Добавляем первое слово
				setFn(word.at(i), i);
				// Добавляем второе слово
				setFn(word.at(j), j);
			}
		}
		// Проверяем соответствует ли слово одному алфавиту
		result = (((normal.size() + other.size()) != length) && ((latian.size() + other.size()) != length));
		// Если слово не соответствует алфавиту
		if(result && !this->substitutes.empty()){
			// Если это предположительно это сломанное слово нормального алфавита
			if(!latian.empty() && (normal.size() >= latian.size())){
				// Переходим по всему списку собранных латинских букв
				for(auto & item : latian){
					// Выполняем поиск буквы в списке репласеров
					auto it = this->substitutes.find(item.first);
					// Если буква найдена
					if(it != this->substitutes.end()){
						// Выполняем замену буквы в слове
						word.replace(item.second, 1, 1, it->second);
					}
				}
				// Если слово больше не латинское
				result = this->checkLatian(word);
			// Если это англоязычное слово
			} else if(!other.empty()) {
				// Переходим по всему списку собранных латинских букв
				for(auto & item : other){
					// Выполняем поиск буквы в списке репласеров
					auto it = this->substitutes.find(item.first);
					// Если буква найдена
					if(it != this->substitutes.end()){
						// Выполняем замену буквы в слове
						word.replace(item.second, 1, 1, it->second);
					}
				}
				// Если слово больше не латинское
				result = this->isLatian(word);
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * check Метод проверки соответствии буквы
 * @param  letter буква для проверки
 * @return        результат проверки
 */
const bool anyks::Alphabet::check(const wchar_t letter) const noexcept {
	// Результат работы функции
	bool result = false;
	// Результат проверки
	if(letter > 0){
		// Если это не число, тогда выполняем проверку
		if(!(result = this->isNumber({letter}))){
			// Выполняем проверку буквы
			result = (this->letters.count(letter) > 0);
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
const bool anyks::Alphabet::checkHome2(const wstring & word) const noexcept {
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
const bool anyks::Alphabet::checkLatian(const wstring & str) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если строка передана
	if(!str.empty()){
		// Длина слова
		const size_t length = str.length();
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Переходим по всем буквам слова
			for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово латинским
				result = (i == j ? (this->latian.count(str.at(i)) > 0) : (this->latian.count(str.at(i)) > 0) || (this->latian.count(str.at(j)) > 0));
				// Если найдена хотя бы одна латинская буква тогда выходим
				if(result) break;
			}
		// Если символ всего один, проверяем его так
		} else result = (this->latian.count(str.front()) > 0);
	}
	// Выводим результат
	return result;
}
/**
 * checkHyphen Метод проверки наличия дефиса в строке
 * @param str строка для проверки
 * @return    результат проверки
 */
const bool anyks::Alphabet::checkHyphen(const wstring & str) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если строка передана
	if(!str.empty()){
		// Длина слова
		const size_t length = str.length();
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Переходим по всем буквам слова
			for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
				// Проверяем является ли слово латинским
				result = (i == j ? (str.at(i) == L'-') : (str.at(i) == L'-') || (str.at(j) == L'-'));
				// Если найдена хотя бы одна латинская буква тогда выходим
				if(result) break;
			}
		// Если символ всего один, проверяем его так
		} else result = (str.front() == L'-');
	}
	// Выводим результат
	return result;
}
/**
 * checkSimilars Метод проверки на симиляции букв с другими языками
 * @param  str строка для проверки
 * @return     результат проверки
 */
const bool anyks::Alphabet::checkSimilars(const wstring & str) const noexcept {
	// Результат работы функции
	bool result = false;
	// Если строка передана
	if(!str.empty()){
		// Длина слова
		const size_t length = str.length();
		// Если длина строки больше 1
		if(length > 1){
			/**
			 * existFn Функция проверки на существование буквы
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			auto existFn = [this](const wchar_t letter) noexcept {
				// Результат работы функци
				bool result = false;
				// Если буква передана
				if(letter > 0){
					// Выполняем проверку буквы
					result = (
						(letter != L'-') &&
						!this->isNumber({letter}) &&
						!this->isLatian({letter}) &&
						(this->letters.count(letter) > 0)
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
				if(this->similarsSymbols.count(str.at(i)) > 0){
					// Проверяем первую букву
					first = (i > 0 ? existFn(str.at(i - 1)) : false);
					// Проверяем следующую букву
					second = (i < (length - 1) ? existFn(str.at(i + 1)) : false);
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
 * getzones Метод извлечения списка пользовательских зон интернета
 */
const std::set <wstring> & anyks::Alphabet::getzones() const noexcept {
	// Выводим список доменных зон интернета
	return this->uri.getZones();
}
/**
 * getSubstitutes Метод извлечения букв для исправления слов из смешанных алфавитов
 * @param return список букв разных алфавитов соответствующих друг-другу
 */
const std::map <string, string> & anyks::Alphabet::getSubstitutes() const noexcept {
	// Выводим результат
	const static std::map <string, string> result;
	// Если список букв передан
	if(!this->substitutes.empty()){
		// Переходим по всем буквам
		for(auto & item : this->substitutes){
			// Добавляем букву в список
			const_cast <std::map <string, string> *> (&result)->emplace(
				this->convert(wstring{item.first}),
				this->convert(wstring{item.second})
			);
		}
	}
	// Выводим результат
	return result;
}
/**
 * urls Метод извлечения координат url адресов в строке
 * @param text текст для извлечения url адресов
 * @return     список координат с url адресами
 */
const std::map <size_t, size_t> anyks::Alphabet::urls(const wstring & text) const noexcept {
	// Результат работы функции
	map <size_t, size_t> result;
	// Если текст передан
	if(!text.empty()){
		// Позиция найденного uri адреса
		size_t pos = 0;
		// Выполням поиск ссылок в тексте
		while(pos < text.length()){
			// Выполняем парсинг uri адреса
			this->uri.parse(text.substr(pos));
			// Извлекаем данные uri адреса
			auto resUri = this->uri.get();
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
}
/**
 * checkHypLat Метод поиска дефиса и латинского символа
 * @param str строка для проверки
 * @return    результат проверки
 */
const pair <bool, bool> anyks::Alphabet::checkHypLat(const wstring & str) const noexcept {
	// Результат работы функции
	pair <bool, bool> result = {false, false};
	// Если строка передана
	if(!str.empty()){
		// Длина слова
		const size_t length = str.length();
		// Если длина слова больше 1-го символа
		if(length > 1){
			// Значение текущей буквы
			wchar_t letter = 0;
			// Переходим по всем буквам слова
			for(size_t i = 0, j = (length - 1); j > ((length / 2) - 1); i++, j--){
				// Получаем значение текущей буквы
				letter = str.at(i);
				// Проверяем является ли слово латинским
				if(!result.first) result.first = (letter == L'-');
				// Проверяем на латинский символ
				if(!result.first && !result.second) result.second = (this->latian.count(letter) > 0);
				// Проверяем вторую букву
				if((!result.first || !result.second) && (i != j)){
					// Получаем значение текущей буквы
					letter = str.at(j);
					// Проверяем является ли слово латинским
					if(!result.first) result.first = (letter == L'-');
					// Проверяем на латинский символ
					if(!result.first && !result.second) result.second = (this->latian.count(letter) > 0);
				}
				// Если найден и пробел и латинский символ
				if(result.first && result.second) break;
			}
		// Если символ всего один, проверяем его так
		} else {
			// Запоминаем найден ли дефис
			result.first = (str.front() == L'-');
			// Если дефис не найден, проверяем на латинский символ
			if(!result.first) result.second = (this->latian.count(str.front()) > 0);
		}
	}
	// Выводим результат
	return result;
}
/**
 * clear Метод очистки собранных данных
 */
void anyks::Alphabet::clear() noexcept {
	// Алфавит латинских символов
	wstring alphabet = L"";
	// Переходим по всем буквам латинских символов
	for(auto & letter : this->latian){
		// Формируем алфавит латинских символов
		alphabet.append(1, letter);
	}
	// Очищаем список похожих букв в разных алфавитах
	this->substitutes.clear();
	// Формируем алфавит
	this->set(this->convert(alphabet));
}
/**
 * switchAllowApostrophe Метод разрешения или запрещения апострофа как части слова
 */
void anyks::Alphabet::switchAllowApostrophe() noexcept {
	// Выполняем переключение разрешения использования апострофа
	this->apostrophe = !this->apostrophe;
}
/**
 * log Метод вывода текстовой информации в консоль или файл
 * @param format   формат строки вывода
 * @param flag     флаг типа логирования
 * @param filename адрес файла для вывода
 */
void anyks::Alphabet::log(const string & format, log_t flag, const char * filename, ...) const noexcept {
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
			// Проверяем является ли это переводом строки
			const bool isEnd = ((str.compare("\r\n") == 0) || (str.compare("\n") == 0));
			// Формируем файловый поток
			ostream * file = (filename == nullptr ? nullptr : new ofstream(filename, ios::app));
			// Формируем выходной поток
			ostream & out = (filename != nullptr ? * file : cout);
			// Определяем тип сообщения
			switch((u_short) flag){
				// Выводим сообщение так-как оно есть
				case (u_short) log_t::null: out << this->format("%s", str.c_str()) << (!isEnd ? "\r\n" : ""); break;
				// Выводим информационное сообщение
				case (u_short) log_t::info: out << (filename != nullptr ? this->format("info: %s", str.c_str()) : this->format("\x1B[32m\x1B[1minfo:\x1B[0m %s", str.c_str())) << (!isEnd ? "\r\n" : ""); break;
				// Выводим сообщение об ошибке
				case (u_short) log_t::error: out << (filename != nullptr ? this->format("error: %s", str.c_str()) : this->format("\x1B[31m\x1B[1merror:\x1B[0m %s", str.c_str())) << (!isEnd ? "\r\n" : ""); break;
				// Выводим сообщение предупреждения
				case (u_short) log_t::warning: out << (filename != nullptr ? this->format("warning: %s", str.c_str()) : this->format("\x1B[35m\x1B[1mwarning:\x1B[0m %s", str.c_str())) << (!isEnd ? "\r\n" : ""); break;
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
void anyks::Alphabet::split(const word_t & str, const word_t & delim, list <word_t> & v) const noexcept {
	// Выполняем сплит строки
	::split(str, delim, v);
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const word_t & str, const word_t & delim, vector <word_t> & v) const noexcept {
	// Выполняем сплит строки
	::split(str, delim, v);
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const wstring & str, const wstring & delim, list <wstring> & v) const noexcept {
	// Выполняем сплит строки
	::split(str, delim, v);
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const wstring & str, const wstring & delim, vector <wstring> & v) const noexcept {
	// Выполняем сплит строки
	::split(str, delim, v);
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const string & str, const string & delim, list <wstring> & v) const noexcept {
	// Выполняем сплит строки
	::split(this->convert(str), this->convert(delim), v);
}
/**
 * split Метод разделения строк на составляющие
 * @param str   строка для поиска
 * @param delim разделитель
 * @param v     результирующий вектор
 */
void anyks::Alphabet::split(const string & str, const string & delim, vector <wstring> & v) const noexcept {
	// Выполняем сплит строки
	::split(this->convert(str), this->convert(delim), v);
}
/**
 * add Метод добавления буквы в алфавит
 * @param letter буква для добавления
 */
void anyks::Alphabet::add(const wchar_t letter) noexcept {
	// Если буква передана и такой буквы еще нет
	if((letter > 0) && (this->letters.count(letter) < 1)){
		// Добавляем букву в список
		if(!this->isNumber({letter}) && (this->allowedSymbols.count(letter) < 1)) this->letters.emplace(letter);
	}
}
/**
 * set Метод добавления алфавита в словарь
 * @param alphabet алфавит символов для текущего языка
 */
void anyks::Alphabet::set(const string & alphabet) noexcept {
	// Удаляем список букв
	this->letters.clear();
	// Запоминаем список букв
	if(!alphabet.empty()) this->alphabet = this->convert(alphabet);
	// Переходим по всем буквам алфавита
	for(auto & letter : this->alphabet){
		// Добавляем буквы алфавита в список
		this->add(letter);
	}
	// Переходим по всем буквам алфавита
	for(auto & letter : this->alphabet){
		// Если буква не является латинской - запоминаем, что это не латинский алфавит
		if(!(this->typeLatian = this->checkLatian({letter}))) break;
	}
	// Если список букв получен
	if(!this->alphabet.empty()) this->uri.setLetters(this->alphabet);
}
/**
 * setzone Метод установки пользовательской зоны
 * @param zone пользовательская зона
 */
void anyks::Alphabet::setzone(const string & zone) noexcept {
	// Если зона передана, устанавливаем её
	if(!zone.empty()) this->uri.setZone(this->convert(zone));
}
/**
 * setzone Метод установки пользовательской зоны
 * @param zone пользовательская зона
 */
void anyks::Alphabet::setzone(const wstring & zone) noexcept {
	// Если зона передана, устанавливаем её
	if(!zone.empty()) this->uri.setZone(zone);
}
/**
 * setzones Метод установки списка пользовательских зон
 * @param zones список доменных зон интернета
 */
void anyks::Alphabet::setzones(const std::set <string> & zones) noexcept {
	// Устанавливаем список доменных зон
	if(!zones.empty()){
		// Переходим по всему списку доменных зон
		for(auto & zone : zones) this->setzone(zone);
	}
}
/**
 * setzones Метод установки списка пользовательских зон
 * @param zones список доменных зон интернета
 */
void anyks::Alphabet::setzones(const std::set <wstring> & zones) noexcept {
	// Устанавливаем список доменных зон
	if(!zones.empty()) this->uri.setZones(zones);
}
/**
 * setlocale Метод установки локали
 * @param locale локализация приложения
 */
void anyks::Alphabet::setlocale(const string & locale) noexcept {
	// Устанавливаем локаль
	if(!locale.empty()){
		// Устанавливапм локализацию приложения
		::setlocale(LC_CTYPE, locale.c_str());
		::setlocale(LC_COLLATE, locale.c_str());
	}
}
/**
 * setSubstitutes Метод установки букв для исправления слов из смешанных алфавитов
 * @param letters список букв разных алфавитов соответствующих друг-другу
 */
void anyks::Alphabet::setSubstitutes(const std::map <string, string> & letters) noexcept {
	// Если список букв передан
	if(!letters.empty()){
		// Очищаем текущий список букв
		this->substitutes.clear();
		// Переходим по всем буквам
		for(auto & item : letters){
			// Добавляем букву в список
			this->substitutes.emplace(
				this->convert(item.first).front(),
				this->convert(item.second).front()
			);
		}
	}
}
/**
 * Alphabet Конструктор
 */
anyks::Alphabet::Alphabet() noexcept {
	// Устанавливаем локализацию системы
	this->setlocale();
	// Переходим по всем буквам алфавита
	for(auto & letter : this->alphabet){
		// Добавляем буквы в латинский алфавит
		this->latian.emplace(letter);
	}
	// Устанавливаем алфавит по умолчанию
	this->set();
}
/**
 * Alphabet Конструктор
 * @param locale локализация приложения
 */
anyks::Alphabet::Alphabet(const string & locale) noexcept {
	// Устанавливаем локализацию системы
	this->setlocale(locale);
	// Переходим по всем буквам алфавита
	for(auto & letter : this->alphabet){
		// Добавляем буквы в латинский алфавит
		this->latian.emplace(letter);
	}
	// Устанавливаем алфавит по умолчанию
	this->set();
}
/**
 * Alphabet Конструктор
 * @param alphabet алфавит символов для текущего языка
 * @param locale   локализация приложения
 */
anyks::Alphabet::Alphabet(const string & alphabet, const string & locale) noexcept {
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
/**
 * Оператор чтения из потока
 * @param is   поток для чтения
 * @param word слово куда нужно считать данные из потока
 */
istream & anyks::operator >> (istream & is, wstring & word) noexcept {
	// Получаем строку
	string str = "";
	// Считываем в строку значение
	is >> str;
	// Устанавливаем значение
	if(!str.empty()) word.assign(alphabet_t().convert(str));
	// Выводим результат
	return is;
}
/**
 * Оператор вывода в поток из слова
 * @param os   поток куда нужно вывести данные
 * @param word слово из которого нужно вывести данные
 */
ostream & anyks::operator << (ostream & os, const wstring & word) noexcept {
	// Записываем в поток
	os << alphabet_t().convert(word);
	// Выводим результат
	return os;
}

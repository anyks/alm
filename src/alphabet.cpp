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
		for(auto & c : result) c = std::tolower(c, utf8);
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
		for(auto & c : result) c = std::toupper(c, utf8);
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
			const wstring & suffix = word.substr(pos + 1, word.length() - (pos + 1));
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
		);
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
 * isAbbreviation Метод проверки аббривиатуры
 * @param word  слово для проверки
 * @param count количество дефисов в строке
 * @return      результат проверки
 */
const bool anyks::Alphabet::isAbbreviation(const wstring & word, const u_short count) const {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty() && this->isNumber({word.front()})){
		// Если дефис всего один
		if((count > 0 ? count == 1 : this->countLetter(word, L'-') == 1)){
			// Ищем дефис
			const size_t pos = word.rfind(L'-');
			// Получаем размер суффикса
			const u_short size = (word.length() - (pos + 1));
			// Если размер суффикса соответствует
			if((size > 0) && (size < 4)){
				// Получаем суффикс слова
				const wstring & suffix = word.substr(pos + 1, size);
				// Результат проверка суффикса на валидность
				bool valid = this->check(suffix.front());
				// Если размер слова больше 1
				if(size > 1){
					// Выполняем переход по всем буквам слова
					for(size_t i = 0, j = (size - 1); j > ((size / 2) - 1); i++, j--){
						// Выполняем проверку соответствия словарю каждой буквы
						if(!(valid = (i == j ? (this->check(suffix[i])) : (this->check(suffix[i])) && (this->check(suffix[j]))))) break;
					}
				}
				// Выводим результат
				result = (valid && !this->isNumber(suffix));
			}
		}
	}
	// Выводим результат
	return result;
}
/**
 * isRangeNumber Метод проверки является ли слово диапазоном чисел
 * @param  word  слово для проверки
 * @param  delim символ разделителя
 * @return       результат проверки
 */
const bool anyks::Alphabet::isRangeNumber(const wstring & word, const wchar_t delim) const {
	// Результат работы функции
	bool result = false;
	// Если слово передано
	if(!word.empty()){
		// Длина слова
		const size_t length = word.length();
		// Если первая и последняя буквы это числа
		if(this->isNumber({word.front()}) && this->isNumber({word.back()})){
			// Выполняем поиск дефиса
			const size_t pos = word.find(delim);
			// Если позиция найдена
			if(pos != wstring::npos){
				// Найдены первое и второе число
				bool first = false, second = false;
				// Переходим по всему списку
				for(int i = pos - 1, j = pos + 1; i > -1 || j < length; i--, j++){
					// Запоминаем результат поиска первого числа
					if(i > -1) first = this->isNumber({word[i]});
					// Запоминаем результат поиска второго числа
					if(j < length) second = this->isNumber({word[j]});
					// Запоминаем результат
					result = (first && second);
					// Если результат не удачный, выходим
					if(!result) break;
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
 * @param text     входной текст для обработки
 * @param callback функция обратного вызова, на каждой итерации
 */
void anyks::Alphabet::tokens(const string & text, function <const bool (const wstring &, const wstring &, const wchar_t, const size_t, const bool)> callback) const {
	// Если текст передан, и текст не длинее 1024 символов
	if(!text.empty()){
		// Типы флагов
		enum class type_t {
			end,    // Конец текста
			net,    // Интернет адрес
			wrap,   // Перенос строки
			comp,   // Совпадающие слова
			space,  // Пробел в тексте
			punct,  // Знак пунктуации
			isolat, // Символ изоляции
			reduct  // Сокращение слова
		};
		// Установленные флаги
		bitset <8> flags;
		// Полученные фразы
		wstring word = L"", first = L"", joined = L"";
		/**
		 * isFn Функция проверки на установку флага
		 * @param flag флаг для проверки
		 * @return     результат проверки
		 */
		auto isFn = [&flags](const type_t flag){
			// Проверяем установлен ли флаг
			return flags.test((u_short) flag);
		};
		/**
		 * setFn Функция установки или снятия флага
		 * @param flag флаг для установки
		 * @param set  флаг нужно установить
		 */
		auto setFn = [&flags](const type_t flag, const bool set = true){
			// Устанавливаем флаг
			if(set) flags.set((u_short) flag);
			// Снимаем установленный флаг
			else flags.reset((u_short) flag);
		};
		/**
		 * coordinatesFn Функция поиска координат запрещенных к обработке
		 * @param text текст для парсинга
		 * @return     список координат
		 */
		auto coordinatesFn = [this](const wstring & text){
			// Результат работы функции
			map <size_t, size_t> result = {{wstring::npos, wstring::npos}};
			// Если текст передан
			if(!text.empty()){
				// Позиция найденного uri адреса
				size_t pos = 0;
				// Копируем входной текст
				wstring tmp = text;
				// Объект работы с uri адресами
				uri_t uri(this->convert(this->get()));
				// Выполням поиск ссылок в тексте
				while(true){
					// Выполняем парсинг uri адреса
					uri.parse(tmp);
					// Извлекаем данные uri адреса
					auto data = uri.get();
					// Если ссылка найдена
					if(data.type != uri_t::types_t::null){
						// Получаем данные слова
						const wstring & word = data.uri;
						// Выполняем поиск ссылок в тексте
						pos = text.find(word, pos);
						// Если позиция найдена
						if(pos != wstring::npos){
							// Получаем длину uri адреса
							size_t length = word.length();
							// Если это не варнинг
							if(data.type != uri_t::types_t::wrong){
								// Если в списке результатов найдены пустные значения, очищаем список
								if(result.count(wstring::npos) > 0) result.clear();
								// Добавляем в список нашу ссылку
								result.insert({pos, pos + length});
							}
							// Ищем данный uri в временном тексте
							size_t location = tmp.find(word);
							// Удаляем из текста нашу ссылку
							if(location != wstring::npos) tmp.replace(0, location + length, L"");
							// Сдвигаем значение позиции
							pos += length;
						// Если ссылка не найдена в тексте, выходим
						} else break;
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
		auto erangeFn = [](const size_t pos, map <size_t, size_t> & coordinates){
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
		// Конвертируем строку текста
		wstring wtext = move(this->convert(text));
		// Удаляем символ возврата каретки
		wtext = move(this->replace(wtext, L"\r"));
		// Удаляем пробелы по краям текста
		wtext = move(this->trim(wtext));
		// Выполняем поиск координат в тексте
		auto coordinates = coordinatesFn(wtext);
		// Выполняем переход по всей длине предложения
		for(size_t i = 0; i < wtext.length(); i++){
			// Проверяем дошли ли мы до конца
			setFn(type_t::end, (i == (wtext.length() - 1)));
			// Определяем является ли слово адресом интернета
			if(erangeFn(i, coordinates)){
				// Получаем букву
				wchar_t letter = wtext[i];
				// Добавляем знак препинания в слово
				word.append(1, letter);
				// Устанавливаем флаг
				setFn(type_t::net);
			// Снимаем установленный флаг
			} else setFn(type_t::net, false);
			// Если позиция не входит в указанный диапазон
			if(isFn(type_t::end) || !isFn(type_t::net)){
				// Получаем букву
				wchar_t letter = wtext[i];
				// Проверяем является ли буква пробелом
				setFn(type_t::space, this->isSpace(letter));
				// Проверяем является ли буква знаком пунктуации
				setFn(type_t::punct, this->isPunct(letter));
				// Проверяем является ли буква переносом строки
				setFn(type_t::wrap, ((letter == L'\r') || (letter == L'\n')));
				// Если это пробел или перенос строки - отключаем сокращение
				if(isFn(type_t::space) || isFn(type_t::wrap) || isFn(type_t::end)) setFn(type_t::reduct, false);
				// Если это не специальные символы
				if((!isFn(type_t::space) && !isFn(type_t::wrap) && (!isFn(type_t::punct) ||
				(letter == L':'))) || ((letter == L'.') && this->isNumber(word))){
					// Проверяем букву на дефис
					bool isHyphen = (
						(letter == L'~') || (letter == L'-') || (letter == L'+') ||
						(letter == L'=') || (letter == L'*') || (letter == L'/') ||
						(letter == L':') || (letter == L'%') || (letter == L'|') ||
						(letter == L'^') || (letter == L'&') || (letter == L'#') ||
						(letter == L'\\')
					);
					// Проверяем является ли буква изоляционной
					setFn(type_t::isolat, this->isIsolation(letter));
					// Если символ является правильным
					if(!isFn(type_t::isolat)) word.append(1, letter);
					// Очищаем слово, если символ является не корректным
					else if(!isFn(type_t::isolat)) word.clear();
				// Если это знак пунктуации
				} else if(isFn(type_t::punct) && !isFn(type_t::end)) {
					// Отлавливаем дробные числа (324,24 или 324.24)
					// Если это точка или запятая и собранное слово является числом
					if(((letter == L'.') || (letter == L',')) && this->isNumber({word.back()})){
						// Проверяем является ли следующая буква тоже числом
						if(this->isNumber({wtext[i + 1]})){
							// Добавляем знак препинания в слово
							word.append(1, letter);
							// Снимаем установленный флаг
							setFn(type_t::punct, false);
						}
					// Если это точка и размер слова очень короткий
					} else if((letter == L'.') && (word.length() < 3)){
						// Получаем следующую букву
						wchar_t nextLetter = ((i + 1) >= wtext.length() ? 0 : wtext[i + 1]);
						// Если следующая буква не является нормальным словом
						if((nextLetter > 0) && !this->isSpace(nextLetter) && !isFn(type_t::wrap)){
							// Добавляем знак препинания в слово
							word.append(1, letter);
							// Запоминаем что это сокращение
							setFn(type_t::reduct);
							// Снимаем установленный флаг
							setFn(type_t::punct, false);
						}
					// Если это точка и это режим слова сокращения
					} else if((letter == L'.') && isFn(type_t::reduct)) {
						// Добавляем знак препинания в слово
						word.append(1, letter);
						// Снимаем установленный флаг
						setFn(type_t::punct, false);
					}
				}
				// Если это пробел или перенос слов
				if((!word.empty() && !isFn(type_t::reduct) &&
				((isFn(type_t::punct) && isFn(type_t::space)) || isFn(type_t::space) ||
				isFn(type_t::wrap) || isFn(type_t::isolat))) || isFn(type_t::end)){
					// Запоминаем первое слово
					first = joined;
					// Формируем второе слово
					joined = (!first.empty() ? move(first + word) : word);
					// Проверяем совпадают ли слова
					setFn(type_t::comp, !(!word.empty() && (word.compare(joined) != 0)));
					// Если и эталонное слово и вараинт слова получены
					if(!word.empty() || isFn(type_t::end)){
						// Если вконце слова стоит дефис, удаляем слово
						if((word[0] == L'-') && (word.back() == L'-')) word.clear();
						// Выводим результат итерации
						if(!callback(
							(!isFn(type_t::comp) && !isFn(type_t::net) ? first : L""),
							(!isFn(type_t::net) ? word : L""),
							(isFn(type_t::punct) || isFn(type_t::isolat) ? letter : 0),
							(i - word.length()), isFn(type_t::end)
						)) return;
					}
					// Если был встречен знак препинания
					if(isFn(type_t::punct)) joined.clear();
					// Запоминаем начало второго слова
					else joined = word;
					// Очищаем полученное слово
					word.clear();
				}
			}
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

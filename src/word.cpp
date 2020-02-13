/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include "word.hpp"

/**
 * strtrim Функция удаления начальных и конечных пробелов
 * @param str  строка для обработки
 * @param meta метаданные слова
 * @param loc  локаль
 * @return     результат работы функции
 */
anyks::word_t trim(const wstring & str, const anyks::ocdc_t & meta, const locale & loc = locale::classic()){
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
	// Создаем результирующую строку
	anyks::word_t result(wstring{begin, rbegin.base()}, meta);
	// Выводим результат
	return result;
}
/**
 * Оператор объединения
 * @param meta объект структуры для объединения
 */
anyks::Meta anyks::Meta::operator + (const Meta & meta){
	// Создаем новый объект
	meta_t result = {
		this->oc + meta.oc,
		this->dc + meta.dc,
		this->tf + meta.tf,
		this->idf + meta.idf,
		this->wltf + meta.wltf
	};
	// Выводим результат
	return result;
}
/**
 * Оператор присваивания
 * @param meta объект структуры для присваивания
 */
anyks::Meta & anyks::Meta::operator = (const Meta & meta){
	// Присваиваем новые значения
	this->oc = meta.oc;
	this->dc = meta.dc;
	this->tf = meta.tf;
	this->idf = meta.idf;
	this->wltf = meta.wltf;
	// Выводим результат
	return (* this);
}
/**
 * Оператор объединения
 * @param meta объект структуры для объединения
 */
anyks::Meta & anyks::Meta::operator += (const Meta & meta){
	// Увеличиваем значение текущего объекта
	this->oc += meta.oc;
	this->dc += meta.dc;
	this->tf += meta.tf;
	this->idf += meta.idf;
	this->wltf += meta.wltf;
	// Выводим результат
	return (* this);
}
/**
 * calc Метод расчета метрик для слова
 * @return результат расчета метрик
 */
const anyks::Meta anyks::Metrics::calc(){
	// Количество всех документов
	const size_t ad = (this->ad > 0 ? this->ad : 1);
	// Встречаемость в документах
	const size_t dc = (this->dc > 0 ? this->dc : 1);
	// Количество используемых слов
	const size_t cw = (this->cw > 0 ? this->cw : 1);
	// Встречаемость в текстах
	const size_t oc = (this->oc > 0 ? this->oc : 1);
	// Выполняем расчет tf
	const float tf = (float(oc) / float(cw));
	// Выполняем расчет idf
	const float idf = log(float(ad) / float(dc));
	// Считаем вес слова
	const float wltf = (float(1) + log(tf * float(dc)));
	// Формируем результат расчета
	return {oc, dc, tf, idf, wltf};
}
/**
 * wordCase Метод запоминания регистра слова
 */
void anyks::Word::wordCase(){
	// Если строка не пустая и пробел не найден
	if(!this->empty() && (this->find(L" ") == npos)){
		// Очищаем список позиций
		this->uppers = 0;
		// Текущая буква слова
		wchar_t letter = 0;
		// Получаем строку из текущего слова
		wstring word = this->wstr();
		// Объявляем локаль
		const locale utf8("en_US.UTF-8");
		// Переходим по всем символам
		for(size_t i = 0; i < word.length(); i++){
			// Переводим букву в нижний регистр
			letter = tolower(word[i], utf8);
			// Если буквы не равны значит это верхний регистр
			if(letter != word[i]){
				// Переводим букву в нижний регистр
				word[i] = letter;
				// Добавляем позицию буквы в список
				this->uppers += (1 << i);
			}
		}
		// Если набраны буквы в верхнем регистре
		if(this->uppers > 0){
			// Запоминаем метаданные
			const ocdc_t meta = this->getmeta();
			// Выполняем преобразование объекта
			wstring * tmp = reinterpret_cast <wstring *> (this);
			// Выполняем замену подстроки
			tmp->assign(word);
			// Добавляем метаданные
			this->setmeta(meta);
		}
	}
}
/**
 * isUpCase Метод проверки на верхний регистр
 * @param c буква для проверки
 * @return  результат проверки
 */
bool anyks::Word::isUpCase(wchar_t c){
	// Объявляем локаль
	const locale utf8("en_US.UTF-8");
	// Если буквы не равны значит это верхний регистр
	return (tolower(c, utf8) != c);
}
/**
 * str Метод конвертирования строки utf-8 в строку
 * @param  str строка utf-8 для конвертирования
 * @return     обычная строка
 */
const string anyks::Word::str(const wstring & str) const {
	// Результат работы функции
	string result;
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
 * wstr Метод конвертирования строки в строку utf-8
 * @param  str строка для конвертирования
 * @return     строка в utf-8
 */
const wstring anyks::Word::wstr(const string & str) const {
	// Результат работы функции
	wstring result;
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
 * clear Метод очистки строки
 */
void anyks::Word::clear(){
	// Обнуляем метаданные
	this->oc = 0;
	this->dc = 0;
	// Очищаем список позиций
	this->uppers = 0;
	// Очищаем текущее слово
	this->current.clear();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Очищаем строку
	tmp->clear();
}
/**
 * push_back Метод добавления символа в конец строки
 * @param c символ для добавления
 */
void anyks::Word::push_back(char c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Изменяем строку
	this->append(1, c);
	// Добавляем метаданные
	this->setmeta(meta);
}
/**
 * push_back Метод добавления символа в конец строки
 * @param c символ для добавления
 */
void anyks::Word::push_back(wchar_t c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Добавляем символ в конец строки
	this->append(1, c);
	// Добавляем метаданные
	this->setmeta(meta);
}
/**
 * setUppers Метод установки списка регистров в бинарном виде
 * @param uppers список регистров в бинарном виде
 */
void anyks::Word::setUppers(const size_t uppers){
	// Устанавливаем список регистров
	this->uppers = uppers;
}
/**
 * compare Метод сравнения строк
 * @param  str строка для сравнения
 * @return     результат сравнения
 */
int anyks::Word::compare(const string & str) const {
	// Выводим результат
	return this->str().compare(str);
}
/**
 * compare Метод сравнения строк
 * @param  str строка для сравнения
 * @return     результат сравнения
 */
int anyks::Word::compare(const wstring & str) const {
	// Выводим результат
	return this->wstr().compare(str);
}
/**
 * compare Метод сравнения строк
 * @param  pos позиция строки для сравнения
 * @param  len длина строки для сравнения
 * @param  str строка для сравнения
 * @return     результат сравнения
 */
int anyks::Word::compare(size_t pos, size_t len, const string & str) const {
	// Выводим результат
	return this->str().compare(pos, len, str);
}
/**
 * compare Метод сравнения строк
 * @param  pos позиция строки для сравнения
 * @param  len длина строки для сравнения
 * @param  str строка для сравнения
 * @return     результат сравнения
 */
int anyks::Word::compare(size_t pos, size_t len, const wstring & str) const {
	// Выводим результат
	return this->wstr().compare(pos, len, str);
}
/**
 * compare Метод сравнения строк
 * @param  pos    позиция строки для сравнения
 * @param  len    длина строки для сравнения
 * @param  str    строка для сравнения
 * @param  subpos позиция подстроки для сравнения
 * @param  sublen длина подстроки для сравнения
 * @return        результат сравнения
 */
int anyks::Word::compare(size_t pos, size_t len, const string & str, size_t subpos, size_t sublen) const {
	// Выводим результат
	return this->str().compare(pos, len, str, subpos, sublen);
}
/**
 * compare Метод сравнения строк
 * @param  pos    позиция строки для сравнения
 * @param  len    длина строки для сравнения
 * @param  str    строка для сравнения
 * @param  subpos позиция подстроки для сравнения
 * @param  sublen длина подстроки для сравнения
 * @return        результат сравнения
 */
int anyks::Word::compare(size_t pos, size_t len, const wstring & str, size_t subpos, size_t sublen) const {
	// Выводим результат
	return this->wstr().compare(pos, len, str, subpos, sublen);
}
/**
 * compare Метод сравнения строк
 * @param  s указатель на символы в памяти
 * @return   результат сравнения
 */
int anyks::Word::compare(const char * s) const {
	// Выводим результат
	return this->str().compare(s);
}
/**
 * compare Метод сравнения строк
 * @param  s указатель на символы в памяти
 * @return   результат сравнения
 */
int anyks::Word::compare(const wchar_t * s) const {
	// Выводим результат
	return this->wstr().compare(s);
}
/**
 * compare Метод сравнения строк
 * @param  pos позиция строки для сравнения
 * @param  len длина строки для сравнения
 * @param  s   указатель на символы в памяти
 * @return     результат сравнения
 */
int anyks::Word::compare(size_t pos, size_t len, const char * s) const {
	// Выводим результат
	return this->str().compare(pos, len, s);
}
/**
 * compare Метод сравнения строк
 * @param  pos позиция строки для сравнения
 * @param  len длина строки для сравнения
 * @param  s   указатель на символы в памяти
 * @return     результат сравнения
 */
int anyks::Word::compare(size_t pos, size_t len, const wchar_t * s) const {
	// Выводим результат
	return this->wstr().compare(pos, len, s);
}
/**
 * compare Метод сравнения строк
 * @param  pos позиция строки для сравнения
 * @param  len длина строки для сравнения
 * @param  s   указатель на символы в памяти
 * @param  n   количество символов
 * @return     результат сравнения
 */
int anyks::Word::compare(size_t pos, size_t len, const char * s, size_t n) const {
	// Выводим результат
	return this->str().compare(pos, len, s, n);
}
/**
 * compare Метод сравнения строк
 * @param  pos позиция строки для сравнения
 * @param  len длина строки для сравнения
 * @param  s   указатель на символы в памяти
 * @param  n   количество символов
 * @return     результат сравнения
 */
int anyks::Word::compare(size_t pos, size_t len, const wchar_t * s, size_t n) const {
	// Выводим результат
	return this->wstr().compare(pos, len, s, n);
}
/**
 * compare Метод сравнения двух слов
 * @param word слово для сравнения
 * @return     результат сравнения
 */
int anyks::Word::compare(const Word & word) const {
	// Выводим результат сравнения
	return this->compare(word.wstr());
}
/**
 * append Метод добавления строки
 * @param str строка для добавления
 */
anyks::Word & anyks::Word::append(const string & str){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Запоминаем текущий регистр букв
	size_t uppers = this->uppers;
	// Получаем временную строку
	wstring wstr = this->wstr(str);
	// Переходим по всей строке
	for(size_t i = 0; i < wstr.length(); i++){
		// Если регистр букв изменился, запоминаем это
		if(this->isUpCase(wstr[i])) uppers += (1 << (tmp->length() + i));
	}
	// Добавляем подстроку к строке
	tmp->append(this->wstr(str));
	// Добавляем метаданные
	this->setmeta(meta);
	// Формируем всю строку
	this->wordCase();
	// Восстанавливаем регистр букв
	this->uppers = uppers;
	// Выводим результат
	return (* this);
}
/**
 * append Метод добавления строки
 * @param str строка для добавления
 * @return    получившаяся строка
 */
anyks::Word & anyks::Word::append(const wstring & str){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Запоминаем текущий регистр букв
	size_t uppers = this->uppers;
	// Переходим по всей строке
	for(size_t i = 0; i < str.length(); i++){
		// Если регистр букв изменился, запоминаем это
		if(this->isUpCase(str[i])) uppers += (1 << (tmp->length() + i));
	}
	// Добавляем подстроку к строке
	tmp->append(str);
	// Добавляем метаданные
	this->setmeta(meta);
	// Формируем всю строку
	this->wordCase();
	// Восстанавливаем регистр букв
	this->uppers = uppers;
	// Выводим результат
	return (* this);
}
/**
 * append Метод добавления строки
 * @param str    строка для добавления
 * @param subpos начальная позиция
 * @param sublen длина строки
 */
anyks::Word & anyks::Word::append(const string & str, size_t subpos, size_t sublen){
	// Формируем временную строку
	string tstr = "";
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Запоминаем текущий регистр букв
	size_t uppers = this->uppers;
	// Добавляем наши данные
	tstr.append(str, subpos, sublen);
	// Получаем временную строку
	wstring wstr = this->wstr(tstr);
	// Переходим по всей строке
	for(size_t i = 0; i < wstr.length(); i++){
		// Если регистр букв изменился, запоминаем это
		if(this->isUpCase(wstr[i])) uppers += (1 << (tmp->length() + i));
	}
	// Добавляем подстроку к строке
	tmp->append(this->wstr(str), subpos, sublen);
	// Добавляем метаданные
	this->setmeta(meta);
	// Формируем всю строку
	this->wordCase();
	// Восстанавливаем регистр букв
	this->uppers = uppers;
	// Выводим результат
	return (* this);
}
/**
 * append Метод добавления строки
 * @param str    строка для добавления
 * @param subpos начальная позиция
 * @param sublen длина строки
 * @return       получившаяся строка
 */
anyks::Word & anyks::Word::append(const wstring & str, size_t subpos, size_t sublen){
	// Формируем временную строку
	wstring tstr = L"";
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Запоминаем текущий регистр букв
	size_t uppers = this->uppers;
	// Добавляем наши данные
	tstr.append(str, subpos, sublen);
	// Переходим по всей строке
	for(size_t i = 0; i < tstr.length(); i++){
		// Если регистр букв изменился, запоминаем это
		if(this->isUpCase(tstr[i])) uppers += (1 << (tmp->length() + i));
	}
	// Добавляем подстроку к строке
	tmp->append(str, subpos, sublen);
	// Добавляем метаданные
	this->setmeta(meta);
	// Формируем всю строку
	this->wordCase();
	// Восстанавливаем регистр букв
	this->uppers = uppers;
	// Выводим результат
	return (* this);
}
/**
 * append Метод добавления строки
 * @param  s символ для добавления
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::append(const char * s){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Запоминаем текущий регистр букв
	size_t uppers = this->uppers;
	// Получаем временную строку
	wstring wstr = this->wstr(s);
	// Переходим по всей строке
	for(size_t i = 0; i < wstr.length(); i++){
		// Если регистр букв изменился, запоминаем это
		if(this->isUpCase(wstr[i])) uppers += (1 << (tmp->length() + i));
	}
	// Получаем текущее слово в виде строки
	string word = this->str();
	// Добавляем к строке переданную строку
	word.append(s);
	// Заменяем строку на полученную
	this->assign(word, meta);
	// Формируем всю строку
	this->wordCase();
	// Восстанавливаем регистр букв
	this->uppers = uppers;
	// Выводим результат
	return (* this);
}
/**
 * append Метод добавления строки
 * @param  s символ для добавления
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::append(const wchar_t * s){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Запоминаем текущий регистр букв
	size_t uppers = this->uppers;
	// Получаем временную строку
	wstring wstr(s);
	// Переходим по всей строке
	for(size_t i = 0; i < wstr.length(); i++){
		// Если регистр букв изменился, запоминаем это
		if(this->isUpCase(wstr[i])) uppers += (1 << (tmp->length() + i));
	}
	// Добавляем подстроку к строке
	tmp->append(s);
	// Добавляем метаданные
	this->setmeta(meta);
	// Формируем всю строку
	this->wordCase();
	// Восстанавливаем регистр букв
	this->uppers = uppers;
	// Выводим результат
	return (* this);
}
/**
 * append Метод добавления строки
 * @param  s указатель на символы
 * @param  n количество символов
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::append(const char * s, size_t n){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Запоминаем текущий регистр букв
	size_t uppers = this->uppers;
	// Получаем временную строку
	wstring wstr = this->wstr(string(s, n));
	// Переходим по всей строке
	for(size_t i = 0; i < wstr.length(); i++){
		// Если регистр букв изменился, запоминаем это
		if(this->isUpCase(wstr[i])) uppers += (1 << (tmp->length() + i));
	}
	// Получаем текущее слово в виде строки
	string word = this->str();
	// Добавляем к строке переданную строку
	word.append(s, n);
	// Заменяем строку на полученную
	this->assign(word, meta);
	// Формируем всю строку
	this->wordCase();
	// Восстанавливаем регистр букв
	this->uppers = uppers;
	// Выводим результат
	return (* this);
}
/**
 * append Метод добавления строки
 * @param  s указатель на символы
 * @param  n количество символов
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::append(const wchar_t * s, size_t n){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Запоминаем текущий регистр букв
	size_t uppers = this->uppers;
	// Получаем временную строку
	wstring wstr(s, n);
	// Переходим по всей строке
	for(size_t i = 0; i < wstr.length(); i++){
		// Если регистр букв изменился, запоминаем это
		if(this->isUpCase(wstr[i])) uppers += (1 << (tmp->length() + i));
	}
	// Добавляем подстроку к строке
	tmp->append(s, n);
	// Добавляем метаданные
	this->setmeta(meta);
	// Формируем всю строку
	this->wordCase();
	// Восстанавливаем регистр букв
	this->uppers = uppers;
	// Выводим результат
	return (* this);
}
/**
 * append Метод добавления строки
 * @param  n количество символов
 * @param  c символ для добавления
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::append(size_t n, char c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем текущее слово в виде строки
	string word = this->str();
	// Запоминаем текущий регистр букв
	size_t uppers = this->uppers;
	// Добавляем к строке переданную строку
	word.append(n, c);
	// Если регистр букв изменился, запоминаем это
	if(this->isUpCase(word.back())) uppers += (1 << (word.length() - 1));
	// Заменяем строку на полученную
	this->assign(word, meta);
	// Формируем всю строку
	this->wordCase();
	// Восстанавливаем регистр букв
	this->uppers = uppers;
	// Выводим результат
	return (* this);
}
/**
 * append Метод добавления строки
 * @param  n количество символов
 * @param  c символ для добавления
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::append(size_t n, wchar_t c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Запоминаем текущий регистр букв
	size_t uppers = this->uppers;
	// Если регистр букв изменился, запоминаем это
	if(this->isUpCase(c)) uppers += (1 << tmp->length());
	// Добавляем подстроку к строке
	tmp->append(n, c);
	// Добавляем метаданные
	this->setmeta(meta);
	// Формируем всю строку
	this->wordCase();
	// Восстанавливаем регистр букв
	this->uppers = uppers;
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param word строка для замены
 * @param      получившаяся строка
 */
anyks::Word & anyks::Word::assign(const string & str){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Заменяем строку на полученную
	this->assign(str, meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param word строка для замены
 * @param      получившаяся строка
 */
anyks::Word & anyks::Word::assign(const wstring & str){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Заменяем строку
	tmp->assign(str);
	// Запоминаем регистр букв
	this->wordCase();
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param  str    строка для замены
 * @param  subpos позиция в строке
 * @param  sublen количество символов для замены
 * @return        получившаяся строка
 */
anyks::Word & anyks::Word::assign(const string & str, size_t subpos, size_t sublen){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем текущее слово в виде строки
	string word = this->str();
	// Добавляем к строке переданную строку
	word.assign(str, subpos, sublen);
	// Заменяем строку на полученную
	this->assign(word, meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param  str    строка для замены
 * @param  subpos позиция в строке
 * @param  sublen количество символов для замены
 * @return        получившаяся строка
 */
anyks::Word & anyks::Word::assign(const wstring & str, size_t subpos, size_t sublen){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Заменяем строку
	tmp->assign(str, subpos, sublen);
	// Запоминаем регистр букв
	this->wordCase();
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param  s символ для перезаписи
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::assign(const char * s){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем текущее слово в виде строки
	string word = this->str();
	// Добавляем к строке переданную строку
	word.assign(s);
	// Заменяем строку на полученную
	this->assign(word, meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param  s символ для перезаписи
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::assign(const wchar_t * s){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Заменяем строку
	tmp->assign(s);
	// Запоминаем регистр букв
	this->wordCase();
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param  s указатель на символы в памяти
 * @param  n количество символов
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::assign(const char * s, size_t n){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем текущее слово в виде строки
	string word = this->str();
	// Добавляем к строке переданную строку
	word.assign(s, n);
	// Заменяем строку на полученную
	this->assign(word, meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param  s указатель на символы в памяти
 * @param  n количество символов
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::assign(const wchar_t * s, size_t n){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Заменяем строку
	tmp->assign(s, n);
	// Запоминаем регистр букв
	this->wordCase();
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param  n количество символов для замены
 * @param  c символ для замены
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::assign(size_t n, char c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем текущее слово в виде строки
	string word = this->str();
	// Добавляем к строке переданную строку
	word.assign(n, c);
	// Заменяем строку на полученную
	this->assign(word, meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param  n количество символов для замены
 * @param  c символ для замены
 * @return   получившаяся строка
 */
anyks::Word & anyks::Word::assign(size_t n, wchar_t c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Заменяем строку
	tmp->assign(n, c);
	// Запоминаем регистр букв
	this->wordCase();
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param word строка для замены
 * @param meta метаданные слова
 */
anyks::Word & anyks::Word::assign(const string & word, const ocdc_t & meta){
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Выполняем замену подстроки
	tmp->assign(this->wstr(word));
	// Запоминаем регистр букв
	this->wordCase();
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param word строка для замены
 * @param meta метаданные слова
 */
anyks::Word & anyks::Word::assign(const wstring & word, const ocdc_t & meta){
	// Выполняем преобразование объекта
	wstring * tmp = reinterpret_cast <wstring *> (this);
	// Выполняем замену подстроки
	tmp->assign(word);
	// Запоминаем регистр букв
	this->wordCase();
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * assign Метод перезаполнения строки
 * @param word строка для замены
 */
anyks::Word & anyks::Word::assign(const Word & word){
	// Заменяем метаданные
	const ocdc_t meta = word.getmeta();
	// Выполняем замену подстроки
	this->assign(word.wreal(), meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param pos позиция с которой нужно произвести замену
 * @param len длина строки для замены
 * @param str строка для замены
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, const string & str){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, this->wstr(str));
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param pos  позиция с которой нужно произвести замену
 * @param len  длина строки для замены
 * @param str  строка для замены
 * @return     получившаяся строка
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, const wstring & str){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, str);
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param  pos    позиция в строке для замены
 * @param  len    длина строки для замены
 * @param  str    строка для замены
 * @param  subpos позиция для замены
 * @param  sublen количество заменяемых символов
 * @return        получившаяся строка
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, const string & str, size_t subpos, size_t sublen){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, this->wstr(str), subpos, sublen);
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param  pos    позиция в строке для замены
 * @param  len    длина строки для замены
 * @param  str    строка для замены
 * @param  subpos позиция для замены
 * @param  sublen количество заменяемых символов
 * @return        получившаяся строка
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, const wstring & str, size_t subpos, size_t sublen){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, str, subpos, sublen);
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param  pos позиция в строке для замены
 * @param  len длина строки для замены
 * @param  s   указатель на символы в памяти
 * @return     получившаяся строка
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, const char * s){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, this->wstr(s));
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param  pos позиция в строке для замены
 * @param  len длина строки для замены
 * @param  s   указатель на символы в памяти
 * @return     получившаяся строка
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, const wchar_t * s){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, s);
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param  pos позиция в строке для замены
 * @param  len длина строки для замены
 * @param  s   указатель на символы в памяти
 * @param  n   количество символов для замены
 * @return     получившаяся строка
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, const char * s, size_t n){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, this->wstr(string(s, n)));
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param  pos позиция в строке для замены
 * @param  len длина строки для замены
 * @param  s   указатель на символы в памяти
 * @param  n   количество символов для замены
 * @return     получившаяся строка
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, const wchar_t * s, size_t n){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, s, n);
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param  pos позиция в строке для замены
 * @param  len длина строки для замены
 * @param  n   количество символов для замены
 * @param  c   символ для замены
 * @return     получившаяся строка
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, size_t n, char c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, this->wstr(string(n, c)));
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param  pos позиция в строке для замены
 * @param  len длина строки для замены
 * @param  n   количество символов для замены
 * @param  c   символ для замены
 * @return     получившаяся строка
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, size_t n, wchar_t c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, n, c);
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * replace Метод замены строки в тексте
 * @param pos  позиция с которой нужно произвести замену
 * @param len  длина строки для замены
 * @param word слово для замены
 */
anyks::Word & anyks::Word::replace(size_t pos, size_t len, const Word & word){
	// Запоминаем метаданные
	const ocdc_t meta = word.getmeta();
	// Получаем строку
	wstring tmp = this->wreal();
	// Добавляем подстроку к строке
	tmp.replace(pos, len, word.wstr());
	// Устанавливаем новую строку
	this->assign(tmp);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * set Метод загрузки дампа
 * @param dump дамп для загрузки данных
 */
void anyks::Word::set(const vector <char> & dump){
	// Если данные переданы
	if(!dump.empty()){
		// Метаданные
		ocdc_t meta;
		// Смещеение в буфере
		size_t offset = 0;
		// Определяем размер метаданных
		const size_t size = sizeof(meta);
		// Получаем данные дампа
		const char * data = dump.data();
		// Запоминаем метаданные
		memcpy(&meta, data + offset, size);
		// Запоминаем смещение
		offset += size;
		// Если частота встречаемости не нулевая
		if((meta.oc > 0) && (meta.dc > 0)){
			// Список регистров букв в верхнем регистре
			size_t uppers = 0;
			// Получаем список регистров букв в верхнем регистре
			memcpy(&uppers, data + offset, sizeof(uppers));
			// Запоминаем смещение
			offset += sizeof(uppers);
			// Получаем данные слова
			string word(data + offset, dump.size() - offset);
			// Если слово не пустое
			if(!word.empty()){
				// Устанавливаем данные слова
				this->assign(word, meta);
				// Устанавливаем список букв в верхнем регистре
				this->uppers = uppers;
			}
		}
	}
}
/**
 * set Метод загрузки дампа
 * @param dump дамп для загрузки данных
 * @param n    количество байт
 */
void anyks::Word::set(const char * dump, size_t n){
	// Определяем размер метаданных
	const size_t size = sizeof(ocdc_t);
	// Если данные переданы
	if((dump != nullptr) && (n > size)){
		// Метаданные
		ocdc_t meta;
		// Смещеение в буфере
		size_t offset = 0;
		// Запоминаем метаданные
		memcpy(&meta, dump + offset, size);
		// Запоминаем смещение
		offset += size;
		// Если частота встречаемости не нулевая
		if((meta.oc > 0) && (meta.dc > 0)){
			// Список регистров букв в верхнем регистре
			size_t uppers = 0;
			// Получаем список регистров букв в верхнем регистре
			memcpy(&uppers, dump + offset, sizeof(uppers));
			// Запоминаем смещение
			offset += sizeof(uppers);
			// Получаем данные слова
			string word(dump + offset, n - offset);
			// Если слово не пустое
			if(!word.empty()){
				// Устанавливаем данные слова
				this->assign(word, meta);
				// Устанавливаем список букв в верхнем регистре
				this->uppers = uppers;
			}
		}
	}
}
/**
 * dump Метод сбора дампа данных в бинарном виде
 * @return дамп данных в байтах
 */
const vector <char> anyks::Word::dump(){
	// Результат работы функции
	vector <char> result;
	// Если строка не пустая
	if(!this->empty()){
		// Получаем данные строки
		const string & str = this->str();
		// Получаем метаданные слова
		const ocdc_t meta = this->getmeta();;
		// Получаем бинарные метаданные
		const char * mbin = reinterpret_cast <const char *> (&meta);
		// Получаем бинарные данные количества регистров
		const char * ubin = reinterpret_cast <const char *> (&this->uppers);
		// Добавляем в буфер метаданные
		result.insert(result.end(), mbin, mbin + sizeof(meta));
		// Добавляем в буфер количество регистров
		result.insert(result.end(), ubin, ubin + sizeof(this->uppers));
		// Добавляем в буфер данные строки
		result.insert(result.end(), str.begin(), str.end());
	}
	// Выводим результат
	return result;
}
/**
 * back Метод получения последней буквы слова
 * @return последняя бува слова
 */
wchar_t anyks::Word::back() const {
	// Результат работы функции
	wchar_t result = 0;
	// Если слово не пустое
	if(!this->empty()) result = (* this)[this->length() - 1];
	// Выводим результат
	return result;
}
/**
 * front Метод получения первой буквы слова
 * @return первая буква слова
 */
wchar_t anyks::Word::front() const {
	// Результат работы функции
	wchar_t result = 0;
	// Если слово не пустое
	if(!this->empty()) result = (* this)[0];
	// Выводим результат
	return result;
}
/**
 * getUppers Метод получения список регистров
 * @return список регистров в бинарном виде
 */
size_t anyks::Word::getUppers() const {
	// Выводим список регистров в бинарном виде
	return this->uppers;
}
/**
 * find Метод поиска подстроки в строке
 * @param str строка для поиска
 * @param pos позиция начиная с которой нужно производить поиск
 * @return    найденная позиция в строке
 */
size_t anyks::Word::find(const string & str, size_t pos) const {
	// Выполняем поиск в строке
	return this->str().find(str, pos);
}
/**
 * find Метод поиска подстроки в строке
 * @param str строка для поиска
 * @param pos позиция начиная с которой нужно производить поиск
 * @return    найденная позиция в строке
 */
size_t anyks::Word::find(const wstring & str, size_t pos) const {
	// Выполняем поиск в строке
	return this->wstr().find(str, pos);
}
/**
 * find Метод поиска подстроки в строке
 * @param  s   указатель на символы в памяти
 * @param  pos позиция в строке
 * @return     найденная позиция в строке
 */
size_t anyks::Word::find(const char * s, size_t pos) const {
	// Выполняем поиск в строке
	return this->str().find(s, pos);
}
/**
 * find Метод поиска подстроки в строке
 * @param  s   указатель на символы в памяти
 * @param  pos позиция в строке
 * @return     найденная позиция в строке
 */
size_t anyks::Word::find(const wchar_t * s, size_t pos) const {
	// Выполняем поиск в строке
	return this->wstr().find(s, pos);
}
/**
 * find Метод поиска подстроки в строке
 * @param  s   указатель на символы в памяти
 * @param  pos позиция в строке
 * @param  n   количество символов для поиска
 * @return     найденная позиция в строке
 */
size_t anyks::Word::find(const char * s, size_t pos, size_t n) const {
	// Выполняем поиск в строке
	return this->str().find(s, pos, n);
}
/**
 * find Метод поиска подстроки в строке
 * @param  s   указатель на символы в памяти
 * @param  pos позиция в строке
 * @param  n   количество символов для поиска
 * @return     найденная позиция в строке
 */
size_t anyks::Word::find(const wchar_t * s, size_t pos, size_t n) const {
	// Выполняем поиск в строке
	return this->wstr().find(s, pos, n);
}
/**
 * find Метод поиска подстроки в строке
 * @param  c   символ для поиска
 * @param  pos позиция в строке
 * @return     найденная позиция в строке
 */
size_t anyks::Word::find(char c, size_t pos) const {
	// Выполняем поиск в строке
	return this->str().find(c, pos);
}
/**
 * find Метод поиска подстроки в строке
 * @param  c   символ для поиска
 * @param  pos позиция в строке
 * @return     найденная позиция в строке
 */
size_t anyks::Word::find(wchar_t c, size_t pos) const {
	// Выполняем поиск в строке
	return this->wstr().find(c, pos);
}
/**
 * rfind Метод обратного поиска подстроки в строке
 * @param str строка для поиска
 * @param pos позиция начиная с которой нужно производить поиск
 * @return    найденная позиция в строке
 */
size_t anyks::Word::rfind(const string & str, size_t pos) const {
	// Выполняем поиск в строке
	return this->str().rfind(str, pos);
}
/**
 * rfind Метод обратного поиска подстроки в строке
 * @param str строка для поиска
 * @param pos позиция начиная с которой нужно производить поиск
 * @return    найденная позиция в строке
 */
size_t anyks::Word::rfind(const wstring & str, size_t pos) const {
	// Выполняем поиск в строке
	return this->wstr().rfind(str, pos);
}
/**
 * rfind Метод обратного поиска подстроки в строке
 * @param  s   указатель на символы в памяти
 * @param  pos позиция в строке
 * @return     найденная позиция в строке
 */
size_t anyks::Word::rfind(const char * s, size_t pos) const {
	// Выполняем поиск в строке
	return this->str().rfind(s, pos);
}
/**
 * rfind Метод обратного поиска подстроки в строке
 * @param  s   указатель на символы в памяти
 * @param  pos позиция в строке
 * @return     найденная позиция в строке
 */
size_t anyks::Word::rfind(const wchar_t * s, size_t pos) const {
	// Выполняем поиск в строке
	return this->wstr().rfind(s, pos);
}
/**
 * rfind Метод обратного поиска подстроки в строке
 * @param  s   указатель на символы в памяти
 * @param  pos позиция в строке
 * @param  n   количество символов для поиска
 * @return     найденная позиция в строке
 */
size_t anyks::Word::rfind(const char * s, size_t pos, size_t n) const {
	// Выполняем поиск в строке
	return this->str().rfind(s, pos, n);
}
/**
 * rfind Метод обратного поиска подстроки в строке
 * @param  s   указатель на символы в памяти
 * @param  pos позиция в строке
 * @param  n   количество символов для поиска
 * @return     найденная позиция в строке
 */
size_t anyks::Word::rfind(const wchar_t * s, size_t pos, size_t n) const {
	// Выполняем поиск в строке
	return this->wstr().rfind(s, pos, n);
}
/**
 * rfind Метод обратного поиска подстроки в строке
 * @param  c   символ для поиска
 * @param  pos позиция в строке
 * @return     найденная позиция в строке
 */
size_t anyks::Word::rfind(char c, size_t pos) const {
	// Выполняем поиск в строке
	return this->str().rfind(c, pos);
}
/**
 * rfind Метод обратного поиска подстроки в строке
 * @param  c   символ для поиска
 * @param  pos позиция в строке
 * @return     найденная позиция в строке
 */
size_t anyks::Word::rfind(wchar_t c, size_t pos) const {
	// Выполняем поиск в строке
	return this->wstr().rfind(c, pos);
}
/**
 * trim Метод удаления пробелов вначале и конце слова
 * @return получившаяся строка
 */
anyks::Word anyks::Word::trim() const {
	// Выводим результат
	return ::trim(this->wstr(), this->getmeta());
}
/**
 * lower Метод приведения в нижний регистр
 * @return получившаяся строка
 */
anyks::Word anyks::Word::lower() const {
	// Получаем строку из текущего слова
	wstring word = this->wstr();
	// Объявляем локаль
	const locale utf8("en_US.UTF-8");
	// Переходим по всем символам
	for(auto & c : word) c = tolower(c, utf8);
	// Выводим результат
	return ::trim(word, this->getmeta());
}
/**
 * upper Метод приведения в верхний регистр
 * @return получившаяся строка
 */
anyks::Word anyks::Word::upper() const {
	// Получаем строку из текущего слова
	wstring word = this->wstr();
	// Объявляем локаль
	const locale utf8("en_US.UTF-8");
	// Переходим по всем символам
	for(auto & c : word) c = toupper(c, utf8);
	// Выводим результат
	return ::trim(word, this->getmeta());
}
/**
 * substr Метод обрезки сроки
 * @param  pos позиция для начала обрезки
 * @param  len количество символов для обрезки
 * @return     получившаяся строка
 */
anyks::Word anyks::Word::substr(size_t pos, size_t len) const {
	// Получаем строку из текущего слова
	wstring word = this->wreal();
	// Обрезаем слово
	word = word.substr(pos, len);
	// Выводим результат
	return Word(word, this->getmeta());
}
/**
 * data Данные строки
 * @return указатель на бинарные данные
 */
const char * anyks::Word::data() const {
	// Получаем текущую строку
	this->current = this->str();
	// Выводим результат
	return this->current.data();
}
/**
 * c_str Метод получения указателя на строку
 * @return указатель на строку
 */
const char * anyks::Word::c_str() const {
	// Получаем текущую строку
	this->current = this->str();
	// Выводим результат
	return this->current.c_str();
}
/**
 * str Метод получения строки
 * @return строка в чистом виде
 */
const string anyks::Word::str() const {
	// Результат работы функции
	string result = "";
	// Если строка не пустая
	if(!this->empty()){
		// Выводим результат
		result = this->str(* reinterpret_cast <const wstring *> (this));
	}
	// Выводим результат
	return result;
}
/**
 * wstr Метод получения строки
 * @return строка в чистом виде
 */
const wstring anyks::Word::wstr() const {
	// Результат работы функции
	wstring result = L"";
	// Если строка не пустая
	if(!this->empty()){
		// Выводим результат
		result = (* reinterpret_cast <const wstring *> (this));
	}
	// Выводим результат
	return result;
}
/**
 * real Метод извлечения слова с учётом регистра
 * @return слово с учётом регистра
 */
const string anyks::Word::real() const {
	// Если список регистров пуст, выводим как есть
	if(this->uppers == 0) return this->str();
	// Если список не пустой
	else {
		// Получаем строку из текущего слова
		wstring word = this->wstr();
		// Объявляем локаль
		const locale utf8("en_US.UTF-8");
		// Переходим по всем символам
		for(size_t i = 0; i < word.length(); i++){
			// Проверяем существует ли позиция в списке регистров
			if((1 << i) & this->uppers) word[i] = toupper(word[i], utf8);
		}
		// Выводим результат
		return this->str(word);
	}
}
/**
 * wreal Метод извлечения слова с учётом регистра
 * @return слово с учётом регистра
 */
const wstring anyks::Word::wreal() const {
	// Если список регистров пуст, выводим как есть
	if(this->uppers == 0) return this->wstr();
	// Если список не пустой
	else {
		// Получаем строку из текущего слова
		wstring word = this->wstr();
		// Объявляем локаль
		const locale utf8("en_US.UTF-8");
		// Переходим по всем символам
		for(size_t i = 0; i < word.length(); i++){
			// Проверяем существует ли позиция в списке регистров
			if((1 << i) & this->uppers) word[i] = toupper(word[i], utf8);
		}
		// Выводим результат
		return word;
	}
}
/**
 * getmeta Метод получения метаданных
 * @return метаданные
 */
const anyks::ocdc_t anyks::Word::getmeta() const {
	// Выводим метаданные слова
	return ocdc_t(this->oc, this->dc);
}
/**
 * calc Метод подсчёта параметров слова
 * @param ad количество используемых документов
 * @param cw количество обработанных слов
 * @return   подсчитанные параметры слова
 */
const anyks::meta_t anyks::Word::calc(const size_t ad, const size_t cw) const {
	// Создаём объект метрики
	metric_t metric(ad, cw, this->dc, this->oc);
	// Выводим параметры слова
	return metric.calc();
}
/**
 * setmeta Метод установки метаданных
 * @param meta метаданные слова
 */
void anyks::Word::setmeta(const ocdc_t & meta){
	// Запоминаем метаданные
	this->oc = meta.oc;
	this->dc = meta.dc;
}
/**
 * setmeta Метод установки метаданных
 * @param dc количество документов где встретилось слово
 * @param oc количество встречь слова в текстах
 */
void anyks::Word::setmeta(const size_t dc, const size_t oc){
	// Запоминаем метаданные
	this->oc = oc;
	this->dc = dc;
}
/**
 * Оператор объединения
 * @param s указатель на символы в памяти
 * @return  получившееся слово
 */
anyks::Word anyks::Word::operator + (const char * s){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку слова
	string word = this->str();
	// Добавляем к слову символ
	word.append(s);
	// Выводим результат
	return Word(word, meta);
}
/**
 * Оператор объединения
 * @param s указатель на символы в памяти
 * @return  получившееся слово
 */
anyks::Word anyks::Word::operator + (const wchar_t * s){
	// Результат работы функции
	Word result = (* this);
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Добавляем строку
	result.append(s);
	// Добавляем метаданные
	result.setmeta(meta);
	// Выводим результат
	return result;
}
/**
 * Оператор объединения
 * @param c символ для присваивания
 * @return  получившееся слово
 */
anyks::Word anyks::Word::operator + (char c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Получаем строку слова
	string word = this->str();
	// Добавляем к слову символ
	word.append(1, c);
	// Выводим результат
	return Word(word, meta);
}
/**
 * Оператор объединения
 * @param c символ для присваивания
 * @return  получившееся слово
 */
anyks::Word anyks::Word::operator + (wchar_t c){
	// Результат работы функции
	Word result = (* this);
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Добавляем строку
	result.append(1, c);
	// Добавляем метаданные
	result.setmeta(meta);
	// Выводим результат
	return result;
}
/**
 * Оператор объединения
 * @param word объект слова
 */
anyks::Word anyks::Word::operator + (const string & word){
	// Результат работы функции
	Word result = (* this);
	// Если слово передано
	if(!word.empty()) result.append(word);
	// Выводим результат
	return result;
}
/**
 * Оператор объединения
 * @param word объект слова
 */
anyks::Word anyks::Word::operator + (const wstring & word){
	// Результат работы функции
	Word result = (* this);
	// Если слово передано
	if(!word.empty()) result.append(word);
	// Выводим результат
	return result;
}
/**
 * Оператор объединения
 * @param word объект слова
 */
anyks::Word anyks::Word::operator + (const Word & word){
	// Результат работы функции
	Word result = (* this);
	// Если слово передано
	if(!word.empty()){
		// Получаем метаданные слова
		const ocdc_t meta = word.getmeta();
		// Если метаданные не пустые
		if((meta.oc > 0) && (meta.dc > 0)){
			// Увеличиваем метаданные
			result.setmeta({meta.oc + this->getmeta().oc, meta.dc + this->getmeta().dc});
		}
		// Добавляем строку
		result.append(word);
	}
	// Выводим результат
	return result;
}
/**
 * Оператор присваивания
 * @param s указатель на символы в памяти
 * @return  получившееся слово
 */
anyks::Word & anyks::Word::operator = (const char * s){
	// Заменяем исходную строку
	this->assign(s);
	// Добавляем метаданные
	this->setmeta({1, 1});
	// Выводим результат
	return (* this);
}
/**
 * Оператор присваивания
 * @param s указатель на символы в памяти
 * @return  получившееся слово
 */
anyks::Word & anyks::Word::operator = (const wchar_t * s){
	// Заменяем исходную строку
	this->assign(s);
	// Добавляем метаданные
	this->setmeta({1, 1});
	// Выводим результат
	return (* this);
}
/**
 * Оператор присваивания
 * @param c символ для присваивания
 * @return  получившееся слово
 */
anyks::Word & anyks::Word::operator = (char c){
	// Заменяем исходную строку
	this->assign(1, c);
	// Добавляем метаданные
	this->setmeta({1, 1});
	// Выводим результат
	return (* this);
}
/**
 * Оператор присваивания
 * @param c символ для присваивания
 * @return  получившееся слово
 */
anyks::Word & anyks::Word::operator = (wchar_t c){
	// Заменяем исходную строку
	this->assign(1, c);
	// Добавляем метаданные
	this->setmeta({1, 1});
	// Выводим результат
	return (* this);
}
/**
 * Оператор присваивания
 * @param word объект слова
 */
anyks::Word & anyks::Word::operator = (const string & word){
	// Если слово передано
	if(!word.empty()) this->assign(word);
	// Добавляем метаданные
	this->setmeta({1, 1});
	// Выводим результат
	return (* this);
}
/**
 * Оператор присваивания
 * @param word объект слова
 */
anyks::Word & anyks::Word::operator = (const wstring & word){
	// Если слово передано
	if(!word.empty()) this->assign(word);
	// Добавляем метаданные
	this->setmeta({1, 1});
	// Выводим результат
	return (* this);
}
/**
 * Оператор присваивания
 * @param word объект слова
 */
anyks::Word & anyks::Word::operator = (const Word & word){
	// Если слово передано
	if(!word.empty()) this->assign(word);
	// Выводим результат
	return (* this);
}
/**
 * Оператор сложения и присваивания
 * @param s указатель на символы в памяти
 * @return  получившееся слово
 */
anyks::Word & anyks::Word::operator += (const char * s){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Добавляем строку
	this->append(s);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * Оператор сложения и присваивания
 * @param s указатель на символы в памяти
 * @return  получившееся слово
 */
anyks::Word & anyks::Word::operator += (const wchar_t * s){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Добавляем строку
	this->append(s);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * Оператор сложения и присваивания
 * @param c символ для присваивания
 * @return  получившееся слово
 */
anyks::Word & anyks::Word::operator += (char c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Добавляем строку
	this->append(1, c);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * Оператор сложения и присваивания
 * @param c символ для присваивания
 * @return  получившееся слово
 */
anyks::Word & anyks::Word::operator += (wchar_t c){
	// Запоминаем метаданные
	const ocdc_t meta = this->getmeta();
	// Добавляем строку
	this->append(1, c);
	// Добавляем метаданные
	this->setmeta(meta);
	// Выводим результат
	return (* this);
}
/**
 * Оператор объединения
 * @param word объект слова
 */
anyks::Word & anyks::Word::operator += (const Word & word){
	// Если слово передано
	if(!word.empty()){
		// Получаем метаданные слова
		const ocdc_t meta = word.getmeta();
		// Если метаданные не пустые
		if((meta.oc > 0) && (meta.dc > 0)){
			// Устанавливаем полученные метаданные
			this->setmeta({meta.oc + this->getmeta().oc, meta.dc + this->getmeta().dc});
		}
		// Добавляем строку
		this->append(word);
	}
	// Выводим результат
	return (* this);
}
/**
 * Оператор объединения
 * @param word объект слова
 */
anyks::Word & anyks::Word::operator += (const string & word){
	// Если слово передано
	if(!word.empty()) this->append(word);
	// Выводим результат
	return (* this);
}
/**
 * Оператор объединения
 * @param word объект слова
 */
anyks::Word & anyks::Word::operator += (const wstring & word){
	// Если слово передано
	if(!word.empty()) this->append(word);
	// Выводим результат
	return (* this);
}
/**
 * Оператор сравнения слов
 * @param word объект слова
 * @return     результат сравнения
 */
bool anyks::Word::operator == (const Word & word){
	// Выводим результат
	return (this->compare(word) == 0);
}
/**
 * Оператор сравнения слов
 * @param word объект слова
 * @return     результат сравнения
 */
bool anyks::Word::operator != (const Word & word){
	// Выводим результат
	return (this->compare(word) != 0);
}
/**
 * Оператор сравнения слов
 * @param word объект слова
 * @return     результат сравнения
 */
bool anyks::Word::operator < (const Word & word){
	// Выводим результат
	return (this->size() < word.size());
}
/**
 * Оператор сравнения слов
 * @param word объект слова
 * @return     результат сравнения
 */
bool anyks::Word::operator > (const Word & word){
	// Выводим результат
	return (this->size() > word.size());
}
/**
 * Оператор сравнения слов
 * @param word объект слова
 * @return     результат сравнения
 */
bool anyks::Word::operator <= (const Word & word){
	// Выводим результат
	return (this->size() <= word.size());
}
/**
 * Оператор сравнения слов
 * @param word объект слова
 * @return     результат сравнения
 */
bool anyks::Word::operator >= (const Word & word){
	// Выводим результат
	return (this->size() >= word.size());
}
/**
 * Оператор чтения из потока
 * @param is   поток для чтения
 * @param word слово куда нужно считать данные из потока
 */
istream & anyks::operator >> (istream & is, Word & word){
	// Получаем строку
	string str = "";
	// Считываем в строку значение
	is >> str;
	// Устанавливаем значение
	word.assign(str, word.getmeta());
	// Добавляем метаданные
	word.setmeta({1, 1});
	// Выводим результат
	return is;
}
/**
 * Оператор вывода в поток из слова
 * @param os   поток куда нужно вывести данные
 * @param word слово из которого нужно вывести данные
 */
ostream & anyks::operator << (ostream & os, const Word & word){
	// Записываем в поток
	os << word.str();
	// Выводим результат
	return os;
}
/**
 * Word Конструктор
 */
anyks::Word::Word(){
	// Изменяем строку
	this->assign(wstring());
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param word слово для инициализации
 * @param pos  позиция для вставки
 * @param len  длина слова для вставки
 */
anyks::Word::Word(const string & word, size_t pos, size_t len){
	// Создаем строку
	string str(word, pos, len);
	// Изменяем строку
	this->assign(str);
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param word слово для инициализации
 * @param pos  позиция для вставки
 * @param len  длина слова для вставки
 */
anyks::Word::Word(const wstring & word, size_t pos, size_t len){
	// Создаем строку
	wstring str(word, pos, len);
	// Изменяем строку
	this->assign(str);
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param s указатель на символы в памяти
 */
anyks::Word::Word(const char * s){
	// Создаем строку
	string str(s);
	// Изменяем строку
	this->assign(str);
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param s указатель на символы в памяти
 */
anyks::Word::Word(const wchar_t * s){
	// Создаем строку
	wstring str(s);
	// Изменяем строку
	this->assign(str);
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param s указатель на символы в памяти
 * @param n количество символов для вставки
 */
anyks::Word::Word(const char * s, size_t n){
	// Создаем строку
	string str(s, n);
	// Изменяем строку
	this->assign(str);
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param s указатель на символы в памяти
 * @param n количество символов для вставки
 */
anyks::Word::Word(const wchar_t * s, size_t n){
	// Создаем строку
	wstring str(s, n);
	// Изменяем строку
	this->assign(str);
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param n количество символов для вставки
 * @param c символ для вставки
 */
anyks::Word::Word(size_t n, char c){
	// Создаем строку
	string str(n, c);
	// Изменяем строку
	this->assign(str);
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param n количество символов для вставки
 * @param c символ для вставки
 */
anyks::Word::Word(size_t n, wchar_t c){
	// Создаем строку
	wstring str(n, c);
	// Изменяем строку
	this->assign(str);
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param str строка для установки
 */
anyks::Word::Word(const string & str){
	// Если строка передана
	if(!str.empty()) this->assign(this->wstr(str));
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param str строка для установки
 */
anyks::Word::Word(const wstring & str){
	// Если строка передана
	if(!str.empty()) this->assign(str);
	// Добавляем метаданные
	this->setmeta({1, 1});
}
/**
 * Word Конструктор
 * @param str строка для установки
 * @param meta метаданные слова
 */
anyks::Word::Word(const string & str, const ocdc_t & meta){
	// Если строка передана
	if(!str.empty()) this->assign(this->wstr(str), meta);
}
/**
 * Word Конструктор
 * @param str строка для установки
 * @param meta метаданные слова
 */
anyks::Word::Word(const wstring & str, const ocdc_t & meta){
	// Если строка передана
	if(!str.empty()) this->assign(str, meta);
}
/**
 * ~Word Деструктор
 */
anyks::Word::~Word(){
	// Очищаем все данные
	this->clear();
}

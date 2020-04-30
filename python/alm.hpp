/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_LM_
#define _ANYKS_LM_

/**
 * Наши модули
 */
#include "../include/alm.hpp"
#include "../include/ablm.hpp"

// Устанавливаем область видимости
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	// Основные опции
	enum class options_t : u_short {
		debug,     // Флаг режима отладки
		onlyGood,  // Флаг использования только слов из белого списка
		mixdicts,  // Флаг разрешающий детектировать слова из смешанных словарей
		confidence // Флаг разрешающий загружать n-граммы из arpa так-как они есть
	};
	/**
	 * Perplexity Структура результатов перплексии
	 */
	typedef struct Perplexity {
		size_t oovs;               // Количество неизвестных слов
		size_t words;              // Количество слов в расчёте
		size_t sentences;          // Количество предложений
		size_t zeroprobs;          // Количество нулевых проб
		double logprob, ppl, ppl1; // Значение перплексии
		/**
		 * Perplexity Конструктор
		 */
		Perplexity() : oovs(0), words(0), sentences(0), zeroprobs(0), logprob(0.0), ppl(idw_t::NIDW), ppl1(idw_t::NIDW) {}
	} ppl_t;
	/**
	 * Callback Класс коллбека
	 */
	typedef class Callback {
		public:
			/**
			 * status Метод вывода статуса
			 * @param status статус процесса
			 */
			virtual void status(const int status){};
			/**
			 * text Метод вывода собранного текста
			 * @param text полученный текст для вывода
			 */
			virtual void text(const string & text){};
			/**
			 * sentences Метод вывода собранных предложений
			 * @param sentences собранные предложения
			 */
			virtual bool sentences(const wstring & sentences){
				// Выполняем сбор всех предложений
				return true;
			};
			/**
			 * utoken Метод обработки пользовательского токена
			 * @param token токен соответствующий слову
			 * @param word  слово соответствующее токену
			 */
			virtual bool utoken(const string & token, const string & word){
				// Выполняем сбор всех предложений
				return false;
			};
			/**
			 * tokens Метод вывода результата токенизатора
			 * @param word    слово полученное токенизатором
			 * @param context контекст собранный токенизатором
			 * @param reset   сброс сборки предложения
			 * @param stop    завершение обработки текста
			 */
			virtual bool tokens(const wstring & word, const vector <string> & context, const bool reset, const bool stop){
				// Выполняем сбор всех предложений
				return true;
			};
			/**
			 * ~Callback Деструктор
			 */
			virtual ~Callback(){}; 
	} callback_t;
	/**
	 * LM Класс модуля языковой модели
	 */
	typedef class LM {
		private:
			// Адреса файлов логов
			string logfile = "";
		private:
			// Объект языковой модели
			alm_t * alm = nullptr;
			// Объект каллбека
			callback_t * callback = nullptr;
			// Объект алфавита
			alphabet_t * alphabet = nullptr;
			// Объект токенизатора
			tokenizer_t * tokenizer = nullptr;
		public:
			/**
			 * perplexity Метод расчёта перплексии текста
			 * @param  text текст для расчёта
			 * @return      результат расчёта
			 */
			const anyks::ppl_t perplexity(const wstring & text) const noexcept;
			/**
			 * perplexity Метод расчёта перплексии
			 * @param  seq список последовательностей
			 * @return     результат расчёта
			 */
			const anyks::ppl_t perplexity(const vector <size_t> & seq) const noexcept;
			/**
			 * pplByFiles Метод чтения расчёта перплексии по файлу или группе файлов
			 * @param path адрес каталога или файла для расчёта перплексии
			 * @param ext  расширение файлов в каталоге (если адрес передан каталога)
			 * @return     результат расчёта
			 */
			const anyks::ppl_t pplByFiles(const string & path, const string & ext = "txt") const noexcept;
			/**
			 * pplConcatenate Метод объединения перплексий
			 * @param ppl1 первая перплексия
			 * @param ppl2 вторая перплексия
			 * @return     объединённая перплексия
			 */
			const anyks::ppl_t pplConcatenate(const anyks::ppl_t & ppl1, const anyks::ppl_t & ppl2) const noexcept;
		public:
			/**
			 * check Метод проверки существования последовательности
			 * @param text     текст для проверки существования
			 * @param accurate режим точной проверки
			 * @return         результат проверки
			 */
			const pair <bool, size_t> check(const wstring & text, const bool accurate = false) const noexcept;
			/**
			 * check Метод проверки существования последовательности
			 * @param seq      список слов последовательности
			 * @param accurate режим точной проверки
			 * @return         результат проверки
			 */
			const pair <bool, size_t> check(const vector <size_t> & seq, const bool accurate = false) const noexcept;
		public:
			/**
			 * fixUppers Метод исправления регистров в тексте
			 * @param text текст для исправления регистров
			 * @return     текст с исправленными регистрами слов
			 */
			const wstring fixUppers(const wstring & text) const noexcept;
			/**
			 * context Метод сборки текстового контекста из последовательности
			 * @param seq  последовательность слов для сборки контекста
			 * @param nwrd флаг разрешающий вывод системных токенов
			 * @return     собранный текстовый контекст
			 */
			const wstring context(const vector <size_t> & seq, const bool nwrd = false) const noexcept;
		public:
			/**
			 * clear Метод очистки
			 */
			void clear();
			/**
			 * initPython Метод инициализации скриптов питона
			 */
			void initPython();
			/**
			 * clearBadwords Метод очистки списка плохих слов
			 */
			void clearBadwords() noexcept;
			/**
			 * clearGoodwords Метод очистки списка хороших слов
			 */
			void clearGoodwords() noexcept;
			/**
			 * clearUserTokens Метод очистки пользовательских токенов
			 */
			void clearUserTokens() noexcept;
		public:
			/**
			 * setAllTokenUnknown Метод установки всех токенов идентифицируемых как <unk>
			 */
			void setAllTokenUnknown() noexcept;
			/**
			 * setAllTokenDisable Метод установки всех токенов как не идентифицируемых
			 */
			void setAllTokenDisable() noexcept;
		public:
			/**
			 * addBadword Метод добавления идентификатора похого слова в список
			 * @param idw идентификатор слова
			 */
			void addBadword(const size_t idw) noexcept;
			/**
			 * addGoodword Метод добавления идентификатора хорошего слова в список
			 * @param idw идентификатор слова
			 */
			void addGoodword(const size_t idw) noexcept;
			/**
			 * addBadword Метод добавления похого слова в список
			 * @param word слово для добавления
			 */
			void addBadword(const string & word) noexcept;
			/**
			 * addGoodword Метод добавления хорошего слова в список
			 * @param word слово для добавления
			 */
			void addGoodword(const string & word) noexcept;
		public:
			/**
			 * sentences Метод генерации предложений
			 */
			void sentences() const noexcept;
			/**
			 * setUnknown Метод установки неизвестного слова
			 * @param word слово для добавления
			 */
			void setUnknown(const string & word) noexcept;
			/**
			 * find Метод поиска n-грамм в тексте
			 * @param text текст в котором необходимо найти n-граммы
			 */
			void find(const wstring & text) const noexcept;
			/**
			 * setOption Метод установки опций модуля
			 * @param option опция для установки
			 */
			void setOption(const options_t option) noexcept;
			/**
			 * unsetOption Метод отключения опции модуля
			 * @param option опция для отключения
			 */
			void unsetOption(const options_t option) noexcept;
			/**
			 * setLogfile Метод установки файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const string & logfile) noexcept;
			/**
			 * setOOvFile Метом установки файла для сохранения OOV слов
			 * @param oovfile адрес файла для сохранения oov слов
			 */
			void setOOvFile(const string & oovfile) noexcept;
			/**
			 * setUserToken Метод добавления токена пользователя
			 * @param name слово - обозначение токена
			 */
			void setUserToken(const string & name) noexcept;
			/**
			 * setThreads Метод установки количества потоков
			 * @param threads количество потоков для работы
			 */
			void setThreads(const size_t threads = 0) noexcept;
			/**
			 * setWordScript Метод установки скрипта обработки слов
			 * @param script скрипт python обработки слов
			 */
			void setWordScript(const string & script) noexcept;
			/**
			 * setTokenUnknown Метод установки списка токенов которых нужно идентифицировать как <unk>
			 * @param options список токенов которых нужно идентифицировать как <unk>
			 */
			void setTokenUnknown(const string & options) noexcept;
			/**
			 * setTokenDisable Метод установки списка не идентифицируемых токенов
			 * @param options список не идентифицируемых токенов
			 */
			void setTokenDisable(const string & options) noexcept;
			/**
			 * setUserTokenMethod Метод добавления функции обработки пользовательского токена
			 * @param name слово - обозначение токена
			 */
			void setUserTokenMethod(const string & name) noexcept;
			/**
			 * setUserTokenScript Метод установки скрипта обработки пользовательских токенов
			 * @param script скрипт python обработки пользовательских токенов
			 */
			void setUserTokenScript(const string & script) noexcept;
			/**
			 * setBadwords Метод установки списка идентификаторов плохих слов в список
			 * @param badwords список идентификаторов плохих слов
			 */
			void setBadwords(const set <size_t> & badwords) noexcept;
			/**
			 * setBadwords Метод установки списка плохих слов в список
			 * @param badwords список плохих слов
			 */
			void setBadwords(const vector <string> & badwords) noexcept;
			/**
			 * setTokensUnknown Метод установки списка токенов приводимых к <unk>
			 * @param tokens список токенов для установки
			 */
			void setTokensUnknown(const set <size_t> & tokens) noexcept;
			/**
			 * setTokensDisable Метод установки списка запрещённых токенов
			 * @param tokens список токенов для установки
			 */
			void setTokensDisable(const set <size_t> & tokens) noexcept;
			/**
			 * setGoodwords Метод установки списка идентификаторов хороших слов в список
			 * @param goodwords список идентификаторов хороших слов
			 */
			void setGoodwords(const set <size_t> & goodwords) noexcept;
			/**
			 * setGoodwords Метод установки списка хороших слов в список
			 * @param goodwords список хороших слов
			 */
			void setGoodwords(const vector <string> & goodwords) noexcept;
			/**
			 * read Метод чтения данных из файла arpa
			 * @param filename адрес файла для чтения
			 * @param meta метаданные бинарного контейнера в формате json
			 */
			void read(const string & filename, const string & meta = "") noexcept;
			/**
			 * getUppers Метод извлечения регистров для каждого слова
			 * @param seq  последовательность слов для сборки контекста
			 * @param upps список извлечённых последовательностей
			 */
			void getUppers(const vector <size_t> & seq, vector <size_t> & upps) const noexcept;
		public:
			/**
			 * sentencesToFile Метод сборки указанного количества предложений и записи в файл
			 * @param counts   количество предложений для сборки
			 * @param filename адрес файла для записи результата
			 */
			void sentencesToFile(const size_t counts, const string & filename) const noexcept;
			/**
			 * findByFiles Метод поиска n-грамм в текстовом файле
			 * @param path     адрес каталога или файла для обработки
			 * @param filename адрес файла для записи результата
			 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
			 */
			void findByFiles(const string & path, const string & filename, const string & ext = "txt") const noexcept;
			/**
			 * fixUppersByFiles Метод исправления регистров текста в текстовом файле
			 * @param path     адрес каталога или файла для обработки
			 * @param filename адрес файла для записи результата
			 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
			 */
			void fixUppersByFiles(const string & path, const string & filename, const string & ext = "txt") const noexcept;
			/**
			 * countsByFiles Метод подсчёта количества n-грамм в текстовом файле
			 * @param path     адрес каталога или файла для обработки
			 * @param filename адрес файла для записи результата
			 * @param ngrams   размер n-граммы для подсчёта
			 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
			 */
			void countsByFiles(const string & path, const string & filename, const size_t ngrams = 0, const string & ext = "txt") const noexcept;
			/**
			 * checkByFiles Метод проверки существования последовательности в текстовом файле
			 * @param path     адрес каталога или файла для обработки
			 * @param filename адрес файла для записи результата
			 * @param accurate режим точной проверки
			 * @param ext      расширение файлов в каталоге (если адрес передан каталога)
			 */
			void checkByFiles(const string & path, const string & filename, const bool accurate = false, const string & ext = "txt") const noexcept;
		public:
			/**
			 * getAlphabet Метод получения алфавита языка
			 * @return алфавит языка
			 */
			const string getAlphabet() const noexcept;
			/**
			 * rest Метод исправления и детектирования слов со смешенными алфавитами
			 * @param  word слово для проверки и исправления
			 * @return      результат исправления
			 */
			const wstring rest(const wstring & word) const noexcept;
			/**
			 * arabic2Roman Метод перевода арабских чисел в римские
			 * @param  number арабское число от 1 до 4999
			 * @return        римское число
			 */
			const wstring arabic2Roman(const size_t number) const noexcept;
			/**
			 * arabic2Roman Метод перевода арабских чисел в римские
			 * @param  word арабское число от 1 до 4999
			 * @return      римское число
			 */
			const wstring arabic2Roman(const wstring & word) const noexcept;
			/**
			 * delPunctInWord Метод очистки текста от всех знаков препинаний
			 * @param word слово для очистки
			 * @return     текст без запрещенных символов
			 */
			const wstring delPunctInWord(const wstring & word) const noexcept;
			/**
			 * delBrokenInWord Метод очистки текста от всех символов кроме разрешенных
			 * @param word слово для очистки
			 * @return     текст без запрещенных символов
			 */
			const wstring delBrokenInWord(const wstring & word) const noexcept;
			/**
			 * delHyphenInWord Метод удаления дефиса из слова
			 * @param word слово в котором нужно удалить дефис
			 * @return     слово без дефиса
			 */
			const wstring delHyphenInWord(const wstring & word) const noexcept;
		public:
			/**
			 * countAlphabet Метод получения количества букв в словаре
			 * @return количество букв в словаре
			 */
			const size_t countAlphabet() const noexcept;
			/**
			 * roman2Arabic Метод перевода римских цифр в арабские
			 * @param  word римское число
			 * @return      арабское число
			 */
			const size_t roman2Arabic(const wstring & word) const noexcept;
			/**
			 * countLetter Метод подсчета количества указанной буквы в слове
			 * @param word   слово в котором нужно подсчитать букву
			 * @param letter букву которую нужно подсчитать
			 * @return       результат подсчёта
			 */
			const size_t countLetter(const wstring & word, const wstring & letter) const noexcept;
		public:
			/**
			 * isUrl Метод проверки соответствия слова url адресу
			 * @param word слово для проверки
			 * @return     результат проверки
			 */
			const bool isUrl(const wstring & word) const noexcept;
			/**
			 * isLatian Метод проверки является ли строка латиницей
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const bool isLatian(const wstring & str) const noexcept;
			/**
			 * isMath Метод определения математических операий
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			const bool isMath(const wstring & letter) const noexcept;
			/**
			 * isNumber Метод проверки является ли слово числом
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isNumber(const wstring & word) const noexcept;
			/**
			 * isUpper Метод проверки символ на верхний регистр
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			const bool isUpper(const wstring & letter) const noexcept;
			/**
			 * isPunct Метод проверки является ли буква, знаком препинания
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool isPunct(const wstring & letter) const noexcept;
			/**
			 * isSpace Метод проверки является ли буква, пробелом
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool isSpace(const wstring & letter) const noexcept;
			/**
			 * isANumber Метод проверки является ли косвенно слово числом
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isANumber(const wstring & word) const noexcept;
			/**
			 * isAllowed Метод проверки соответствия слова словарю
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isAllowed(const wstring & word) const noexcept;
			/**
			 * isDecimal Метод проверки является ли слово дробным числом
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isDecimal(const wstring & word) const noexcept;
			/**
			 * isSpecial Метод определения спец-символа
			 * @param letter буква для проверки
			 * @return       результат проверки
			 */
			const bool isSpecial(const wstring & letter) const noexcept;
			/**
			 * isIsolation Метод определения знака изоляции (кавычки, скобки)
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool isIsolation(const wstring & letter) const noexcept;
		public:
			/**
			 * checkHome2 Метод проверки слова на Дом-2
			 * @param  word слово для проверки
			 * @return      результат работы метода
			 */
			const bool checkHome2(const wstring & word) const noexcept;
			/**
			 * checkLatian Метод проверки наличия латинских символов в строке
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const bool checkLatian(const wstring & str) const noexcept;
			/**
			 * checkHyphen Метод проверки наличия дефиса в строке
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const bool checkHyphen(const wstring & str) const noexcept;
			/**
			 * checkSimilars Метод проверки на симиляции букв с другими языками
			 * @param  str строка для проверки
			 * @return     результат проверки
			 */
			const bool checkSimilars(const wstring & str) const noexcept;
			/**
			 * checkLetter Метод проверки соответствии буквы
			 * @param  letter буква для проверки
			 * @return        результат проверки
			 */
			const bool checkLetter(const wstring & letter) const noexcept;
		public:
			/**
			 * getSubstitutes Метод извлечения букв для исправления слов из смешанных алфавитов
			 * @param return список букв разных алфавитов соответствующих друг-другу
			 */
			const map <string, string> & getSubstitutes() const noexcept;
			/**
			 * urls Метод извлечения координат url адресов в строке
			 * @param text текст для извлечения url адресов
			 * @return     список координат с url адресами
			 */
			const map <size_t, size_t> urls(const wstring & text) const noexcept;
			/**
			 * checkHypLat Метод поиска дефиса и латинского символа
			 * @param str строка для проверки
			 * @return    результат проверки
			 */
			const pair <bool, bool> checkHypLat(const wstring & str) const noexcept;
		public:
			/**
			 * setzone Метод установки пользовательской зоны
			 * @param zone пользовательская зона
			 */
			void setzone(const wstring & zone) noexcept;
			/**
			 * setCallback Метод установки каллбека
			 * @param callback каллбек для установки
			 */
			void setCallback(callback_t * callback) noexcept;
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet алфавит символов для текущего языка
			 */
			void setAlphabet(const string & alphabet) noexcept;
			/**
			 * setlocale Метод установки локали
			 * @param locale локализация приложения
			 */
			void setlocale(const string & locale = "en_US.UTF-8") noexcept;
			/**
			 * setSubstitutes Метод установки букв для исправления слов из смешанных алфавитов
			 * @param letters список букв разных алфавитов соответствующих друг-другу
			 */
			void setSubstitutes(const map <string, string> & letters) noexcept;
		public:
			/**
			 * idw Метод извлечения идентификатора слова
			 * @param  word слово для получения идентификатора
			 * @return      идентификатор слова
			 */
			const size_t idw(const wstring & word) const noexcept;
			/**
			 * idt Метод извлечения идентификатора токена
			 * @param  word слово для получения идентификатора
			 * @return      идентификатор токена
			 */
			const size_t idt(const wstring & word) const noexcept;
		public:
			/**
			 * isAllowApostrophe Метод проверки разрешения апострофа
			 * @return результат проверки
			 */
			const bool isAllowApostrophe() const noexcept;
			/**
			 * isAbbr Метод проверки слова на соответствие аббревиатуры
			 * @param  word слово для проверки
			 * @return      результат проверки
			 */
			const bool isAbbr(const wstring & word) const noexcept;
			/**
			 * restore Метод восстановления текста из контекста
			 * @param context токенизированный контекст
			 * @return        результирующий текст
			 */
			const wstring restore(const vector <wstring> & context) const noexcept;
			/**
			 * fti Метод удаления дробной части числа
			 * @param  num   число для обработки
			 * @param  count количество символов после запятой
			 * @return       число без дробной части
			 */
			const size_t fti(const double num, const size_t count = 0) const noexcept;
		public:
			/**
			 * switchAllowApostrophe Метод разрешения или запрещения апострофа как части слова
			 */
			void switchAllowApostrophe() noexcept;
			/**
			 * setAbbr Метод добавления аббревиатуры
			 * @param word слово для добавления
			 */
			void setAbbr(const wstring & word) noexcept;
			/**
			 * jsonToText Метод преобразования текста в формате json в текст
			 * @param text текст для преобразования в формате json
			 */
			void jsonToText(const string & text) const noexcept;
			/**
			 * textToJson Метод преобразования текста в json
			 * @param text текст для преобразования
			 */
			void textToJson(const string & text) const noexcept;
			/**
			 * tokenization Метод разбивки текста на токены
			 * @param text входной текст для обработки
			 */
			void tokenization(const wstring & text) const noexcept;
		public:
			/**
			 * getSize Метод получения размера n-грамы
			 * @return длина n-граммы в языковой моделе
			 */
			const size_t getSize() const;
			/**
			 * getVersion Метод получения версии языковой модели
			 * @return версия языковой модели
			 */
			const string getVersion() const noexcept;
			/**
			 * getUnknown Метод извлечения неизвестного слова
			 * @return установленное неизвестное слово
			 */
			const string & getUnknown() const noexcept;
			/**
			 * getWordScript Метод извлечения скрипта обработки слов
			 * @return адрес скрипта python обработки слов
			 */
			const string & getWordScript() const noexcept;
			/**
			 * getUserTokenScript Метод извлечения скрипта обработки пользовательских токенов
			 * @return адрес скрипта python обработки пользовательских токенов
			 */
			const string & getUserTokenScript() const noexcept;
			/**
			 * getBadwords Метод извлечения чёрного списка
			 * @return чёрный список слов
			 */
			const set <size_t> & getBadwords() const noexcept;
			/**
			 * getGoodwords Метод извлечения белого списка
			 * @return белый список слов
			 */
			const set <size_t> & getGoodwords() const noexcept;
			/**
			 * getTokensUnknown Метод извлечения списка токенов приводимых к <unk>
			 * @return список токенов
			 */
			const set <size_t> getTokensUnknown() const noexcept;
			/**
			 * getTokensDisable Метод извлечения списка запрещённых токенов
			 * @return список токенов
			 */
			const set <size_t> getTokensDisable() const noexcept;
		public:
			/**
			 * getUserTokens Метод извлечения списка пользовательских токенов
			 * @return список пользовательских токенов
			 */
			const vector <string> & getUserTokens() const noexcept;
			/**
			 * grams Метод проверки количества найденных в тексте n-грамм
			 * @param text текст для расчёта
			 * @return     количество найденных n-грамм
			 */
			const size_t grams(const string & text) const noexcept;
			/**
			 * grams Метод проверки количества найденных n-грамм
			 * @param seq список последовательностей
			 * @return    количество найденных n-грамм
			 */
			const size_t grams(const vector <size_t> & seq) const noexcept;
			/**
			 * bigrams Метод проверки количества найденных в тексте биграмм
			 * @param text текст для расчёта
			 * @return     количество найденных биграмм
			 */
			const size_t bigrams(const string & text) const noexcept;
			/**
			 * bigrams Метод проверки количества найденных биграмм
			 * @param seq список последовательностей
			 * @return    количество найденных биграмм
			 */
			const size_t bigrams(const vector <size_t> & seq) const noexcept;
			/**
			 * trigrams Метод проверки количества найденных в тексте триграмм
			 * @param text текст для расчёта
			 * @return     количество найденных триграмм
			 */
			const size_t trigrams(const string & text) const noexcept;
			/**
			 * trigrams Метод проверки количества найденных последовательных триграмм
			 * @param seq список последовательностей
			 * @return    количество найденных триграмм
			 */
			const size_t trigrams(const vector <size_t> & seq) const noexcept;
			/**
			 * getUserTokenWord Метод получения пользовательского токена по его идентификатору
			 * @param idw идентификатор пользовательского токена
			 * @return    пользовательский токен соответствующий идентификатору
			 */
			const string getUserTokenWord(const size_t idw) const noexcept;
			/**
			 * getUserTokenId Метод получения идентификатора пользовательского токена
			 * @param name слово для которого нужно получить идентификатор
			 * @return     идентификатор пользовательского токена соответствующий слову
			 */
			const size_t getUserTokenId(const string & name) const noexcept;
		public:
			/**
			 * LM Конструктор
			 */
			LM() noexcept;
			/**
			 * LM Конструктор
			 * @param alphabet алфавит символов для текущего языка
			 */
			LM(const string & alphabet) noexcept;
			/**
			 * LM Конструктор
			 * @param alphabet алфавит символов для текущего языка
			 * @param locale   локализация приложения
			 */
			LM(const string & alphabet, const string & locale) noexcept;
			/**
			 * ~LM Деструктор
			 */
			~LM() noexcept;
	} lm_t;
};

#endif // _ANYKS_LM_

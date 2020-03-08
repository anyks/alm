/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <env.hpp>
#include <ablm.hpp>
#include <toolkit.hpp>
#include <progress.hpp>
#include <collector.hpp>

// Устанавливаем область видимости
using namespace std;
using namespace anyks;
// Активируем пространство имён json
using json = nlohmann::json;

/**
 * version Функция вывода версии приложения
 * @param address адрес приложения
 */
void version(const char * address) noexcept {
	// Позиция в каталоге
	size_t pos = 0;
#ifdef __linux__
	// Название операционной системы
	const char * osname = "Linux";
#elif __FreeBSD__
	// Название операционной системы
	const char * osname = "FreeBSD";
#elif __APPLE__ || __MACH__
	// Название операционной системы
	const char * osname = "MacOS X";
#endif
	// Определяем адрес приложения
	string appname = realpath(address, nullptr);
	// Ищем каталог
	if((pos = appname.rfind("/")) != string::npos) appname = move(appname.substr(0, pos));
	// Выводим версию приложения
	printf(
		"\r\n%s %s (built: %s %s)\r\n"
		"target: %s\r\n"
		"installed dir: %s\r\n\r\n*\r\n"
		"* author:   %s\r\n"
		"* telegram: %s\r\n"
		"* email:    %s\r\n"
		"* site:     %s\r\n*\r\n\r\n",
		ANYKS_LM_NAME,
		ANYKS_VERSION,
		__DATE__,
		__TIME__,
		osname,
		appname.c_str(),
		ANYKS_AUTHOR,
		ANYKS_CONTACT,
		ANYKS_EMAIL,
		ANYKS_SITE
	);
}
/**
 * help Функция вывода справки
 */
void help() noexcept {
	// Формируем строку справки
	const string msg = "\r\n\x1B[32m\x1B[1musage:\x1B[0m alm [-V | --version] [-h | --help] "
	"[-alphabet <value> | --alphabet=<value>] [<args>]\r\n\r\n\r\n"
	"\x1B[34m\x1B[1mmethods:\x1B[0m\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m tokens: text tokenization method\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m vprune: vocabulary pruning method\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m aprune: language model pruning method\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m train:  language model training method\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m sweep:  high backoff n-gram removal method\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m info:   binary dictionary information method\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m repair: broken language model recovery method\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m modify: method for modifying a language model\r\n"
	"  \x1B[1m-\x1B[0m (emplace | remove | change | replace)\r\n\r\n\r\n"
	"\x1B[34m\x1B[1mflags:\x1B[0m\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m case-insensitive flag:                                                        [-lower-case | --lower-case]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m unknown word permission flag:                                                 [-allow-unk | --allow-unk]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag to disable all tokens:                                                   [-tokens-all-disable | --tokens-all-disable]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag identifying all tokens as <unk>:                                         [-tokens-all-unknown | --tokens-all-unknown]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag allowing to consider words from the white list only:                     [-only-good | --only-good]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag allowing to use interpolation in estimating:                             [-interpolate | --interpolate]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag allowing accounting of all collected n-grams:                            [-all-grams | --all-grams]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag allowing the use of words consisting of mixed dictionaries:              [-mixed-dicts | --mixed-dicts]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag to reset the frequency of an unknown word:                               [-reset-unk | --reset-unk]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag to clearing temporary data during training:                              [-train-autoclean | --train-autoclean]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag to performing data segmentation during training:                         [-train-segments | --train-segments]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag export in binary dictionary of all data:                                 [-w-bin-all | --w-bin-all]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag export in binary dictionary of users tokens:                             [-w-bin-utokens | --w-bin-utokens]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag export in binary dictionary of domain zones:                             [-w-bin-domzones | --w-bin-domzones]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag export in binary dictionary of toolkit options:                          [-w-bin-options | --w-bin-options]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag export in binary dictionary of blacklisted words:                        [-w-bin-badwords | --w-bin-badwords]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag export in binary dictionary of whitelisted words:                        [-w-bin-goodwords | --w-bin-goodwords]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag export in binary dictionary of abbreviations words:                      [-w-bin-abbreviations | --w-bin-abbreviations]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag export in binary dictionary of script python - word preprocessing:       [-w-bin-preword | --w-bin-preword]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m flag export in binary dictionary of only arpa data without n-gram statistics: [-w-bin-arpa | --w-bin-arpa]\r\n\r\n\r\n"
	"\x1B[34m\x1B[1margs:\x1B[0m\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m alphabet (file or value):                           [-alphabet <value> | --alphabet=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m size n-grams:                                       [-size <value> | --size=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m log file address:                                   [-log <value> | --log=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m blacklisted file address:                           [-badwords <value> | --badwords=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m whitelisted file address:                           [-goodwords <value> | --goodwords=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m unknown word name:                                  [-unknown-word <value> | --unknown-word=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m script python3 - word preprocessing:                [-word-script <value> | --word-script=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m script python3 - custom attribute detector:         [-utoken-script <value> | --utoken-script=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m text file training corpus:                          [-corpus <value> | --corpus=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m extension files corpus:                             [-ext <value> | --ext=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m directory path with text corpus:                    [-path <value> | --path=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m number of threads for data collection:              [-threads <value> | --threads=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.txt text for tokens export:          [-w-tokens-text <value> | --w-tokens-text=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.json text for tokens export:         [-w-tokens-json <value> | --w-tokens-json=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.txt text for tokens import:          [-r-tokens-text <value> | --r-tokens-text=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.json text for tokens import:         [-r-tokens-json <value> | --r-tokens-json=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m directory path with tokens files for read:          [-r-tokens-path <value> | --r-tokens-path=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m directory path with tokens files for write:         [-w-tokens-path <value> | --w-tokens-path=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m binary file address *.alm for export:               [-w-bin <value> | --w-bin=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.json meta for export:                [-w-bin-meta <value> | --w-bin-meta=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.map for export:                      [-w-map <value> | --w-map=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.arpa for export:                     [-w-arpa <value> | --w-arpa=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.vocab for export:                    [-w-vocab <value> | --w-vocab=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.ngrams for export:                   [-w-ngram <value> | --w-ngram=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m binary file address *.alm for import:               [-r-bin <value> | --r-bin=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.json meta for import:                [-r-bin-meta <value> | --r-bin-meta=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.map for import:                      [-r-map <value> | --r-map=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m directory path with *.map files:                    [-r-maps <value> | --r-maps=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.vocab for import:                    [-r-vocab <value> | --r-vocab=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m directory path with *.vocab files:                  [-r-vocabs <value> | --r-vocabs=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.ngrams for import:                   [-r-ngram <value> | --r-ngram=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m directory path with *.ngrams files:                 [-r-ngrams <value> | --r-ngrams=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address *.arpa for import:                     [-r-arpa <value> | --r-arpa=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m directory path with *.arpa files:                   [-r-arpas <value> | --r-arpas=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address abbreviations for import:              [-r-abbrs <value> | --r-abbrs=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address domain zones for import:               [-r-domain-zones <value> | --r-domain-zones=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m file address for restore mixed words for import:    [-r-mix-restwords <value> | --r-mix-restwords=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m pruning vocabulary wltf threshold:                  [-vprune-threshold <value> | --vprune-threshold=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m pruning language model frequency threshold:         [-aprune-threshold <value> | --aprune-threshold=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m the maximum size of n-grams of pruning:             [-aprune-max-gram <value> | --aprune-max-gram=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m list of custom attributes:                          [-utokens <value> | --utokens=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m modification flag for modify method:                [-modify <value> | --modify=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m delta size for smoothing by addsmooth algorithm:    [-delta <value> | --delta=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m discount size for smoothing by cdiscount algorithm: [-discount <value> | --discount=<value>]\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m list of tokens to disable:                          [-tokens-disable <value1|value2|...> | --tokens-disable=<value1|value2|...>]\r\n"
	"  \x1B[1m-\x1B[0m (num | url | abbr | date | time | anum | math | rnum | specl | ...)\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m list of tokens for identification as <unk>:         [-tokens-unknown <value1|value2|...> | --tokens-unknown=<value1|value2|...>]\r\n"
	"  \x1B[1m-\x1B[0m (num | url | abbr | date | time | anum | math | rnum | specl | ...)\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m segment size for training data segmentation:        [-train-segment-size <value> | --train-segment-size=<value>]\r\n"
	"  \x1B[1m-\x1B[0m (nb | nkb | nMb | nGb), example: --train-segment-size=4096Mb\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m debug mode:                                         [-debug <value> | --debug=<value>]\r\n"
	"  \x1B[1m-\x1B[0m (0 - off | 1 - progress | 2 - console)\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m method application:                                 [-method <value> | --method=<value>]\r\n"
	"  \x1B[1m-\x1B[0m (train | repair | modify | sweep | vprune | aprune | tokens | info)\r\n\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m smoothing algorithm:                                [-smoothing <value> | --smoothing=<value>]\r\n"
	"  \x1B[1m-\x1B[0m (goodturing | cdiscount | ndiscount | addsmooth | wittenbell | kneserney | mkneserney)\r\n\r\n";
	// Выводим сообщение справки
	printf("%s", msg.c_str());
}
/**
 * print Функция вывода сообщения в консоль
 * @param text     текст для вывода
 * @param filename адрес log файла
 * @param flag     флаг типа сообщения (info, error, warning)
 * @param end      флаг выхода из приложения
 */
void print(const string & text, const char * filename = nullptr, const alphabet_t::log_t flag = alphabet_t::log_t::error, const bool end = true) noexcept {
	// Результат работы функции
	if(!text.empty()){
		// Выполняем логирование
		alphabet_t().log("%s", flag, filename, text.c_str());
		// Выходим из приложения
		if(end) exit(EXIT_FAILURE);
	}
}
/**
 * main Главная функция приложения
 * @param  argc длина массива параметров
 * @param  argv массив параметров
 * @return      код выхода из приложения
 */
int main(int argc, char * argv[]) noexcept {
	// Создаём алфавит
	alphabet_t alphabet;
	// Бинарный файл словаря
	string binDictFile = "";
	// Значение параметра
	const char * value = nullptr;
	// Устанавливаем локаль
	alphabet.setlocale("en_US.UTF-8");
	// Объект отлова переменных
	env_t env(ANYKS_LM_NAME, "text", &alphabet);
	// Выполняем чтение переданных параметров
	env.read((const char **) argv, argc);
	// Если это вывод справки
	if(!env.count() || env.is("help") || env.is("h")) {
		// Выводим справочную информацию
		help();
		// Выходим из приложения
		exit(0);
	// Если версия получена
	} else if(env.is("version") || env.is("V")){
		// Выводим версию приложения
		version(argv[0]);
		// Выходим из приложения
		exit(0);
	}
	// Проверяем существует ли бинарный файл
	if(((value = env.get("r-bin")) != nullptr) && fsys_t::isfile(value)){
		// Запоминаем адрес бинарного файла для чтения
		binDictFile = value;
	}
	// Если алфавит снова не найден
	if(binDictFile.empty() && ((value = env.get("alphabet")) == nullptr)){
		// Выводим сообщение в консоль
		print("alphabet in not set", env.get("log"));
	// Продолжаем дальше
	} else {
		// Объявляем прогресс бра
		progress_t pss;
		// Расширение файла
		string ext = "txt";
		// Переводим указатель в строку
		string letters = (value != nullptr ? value : "");
		// Если расширение файлов передано
		if((value = env.get("ext")) != nullptr) ext = value;
		// Добавляем точку к расширению
		ext.insert(ext.begin(), 1, '.');
		// Проверяем является ли алфавит файлом
		if(letters.rfind(ext) != string::npos){
			// Получаем адрес файла
			const string filename = letters;
			// Очищаем список букв алфавита
			letters.clear();
			// Выполняем считывание всех строк текста
			fsys_t::rfile(filename, [&letters](const string & line, const uintmax_t size) noexcept {
				// Если текст получен
				if(!line.empty()) letters.append(line);
			});
		}
		// Если алфавит получен
		if(!binDictFile.empty() || !letters.empty()){
			// Дополнительный коэффициент алгоритма сглаживания
			float mod = 0.0f;
			// Режим отладки и общий размер n-граммы
			u_short debug = 0, order = 1;
			// Устанавливаем символы алфавита
			if(!letters.empty()) alphabet.set(alphabet.toLower(letters));
			// Если режим отладки получен
			if((value = env.get("debug")) != nullptr){
				// Устанавливаем тип режима
				debug = stoi(value);
				// Если режим отладки больше указанного, компенсируем
				if(debug > 2) debug = 2;
			}
			// Если общий размер n-граммы получен
			if((value = env.get("size")) != nullptr) order = stoi(value);
			// Если бинарный файл не указан
			if(binDictFile.empty() && !env.is("method", "tokens")){
				// Если алгоритм получен
				if(env.is("smoothing") && (string(env.get("smoothing")).compare("-yes-") != 0)){
					// Проверяем правильность введённого алгоритма сглаживания
					if(!env.is("smoothing", "goodturing") &&
					!env.is("smoothing", "cdiscount") &&
					!env.is("smoothing", "ndiscount") &&
					!env.is("smoothing", "addsmooth") &&
					!env.is("smoothing", "wittenbell") &&
					!env.is("smoothing", "kneserney") &&
					!env.is("smoothing", "mkneserney"))
						// Выводим сообщение в консоль
						print(alphabet.format("the algorithm name \"%s\" is bad", env.get("smoothing")), env.get("log"));
				// Сообщаем что алгоритм сглаживания не указан
				} else print("algorithm smoothing is not set", env.get("log"));
			}
			// Если основной метод работы получен
			if(env.is("method") && (string(env.get("method")).compare("-yes-") != 0)){
				// Проверяем правильность введённого основного метода работы
				if(!env.is("method", "info") &&
				!env.is("method", "sweep") &&
				!env.is("method", "train") &&
				!env.is("method", "tokens") &&
				!env.is("method", "repair") &&
				!env.is("method", "modify") &&
				!env.is("method", "aprune") &&
				!env.is("method", "vprune"))
					// Выводим сообщение в консоль
					print(alphabet.format("the method name \"%s\" is bad", env.get("method")), env.get("log"));
			// Сообщаем что метод не указан
			} else print("toolkit method is not set", env.get("log"));
			// Если ни один файл для сохранения не передан, выходим
			if(!env.is("w-map") && !env.is("w-arpa") &&
			!env.is("w-vocab") && !env.is("w-ngram") &&
			!env.is("w-bin") && !env.is("method", "info") &&
			!env.is("method", "tokens")){
				// Выводим сообщение и выходим из приложения
				print("file address to save is not specified", env.get("log"));
			}
			// Если алгоритм сглаживания ConstDiscount или AddSmooth, запрашиваем дополнительные параметры
			if(env.is("smoothing", "cdiscount") || env.is("smoothing", "addsmooth")){
				// Считываем флаг дополнительной модификации
				value = (env.is("smoothing", "cdiscount") ? env.get("discount") : env.get("delta"));
				// Если значение получено
				if(value != nullptr) mod = stof(value);
			}
			/** Начало работы основных методов **/
			// Создаём токенизатор
			tokenizer_t tokenizer(&alphabet);
			// Создаём объект тулкита языковой модели
			toolkit_t toolkit(&alphabet, &tokenizer, order);
			// Устанавливаем адрес файла для логирования
			toolkit.setLogfile(env.get("log"));
			// Устанавливаем режим отладки
			if(debug == 2) toolkit.setOption(toolkit_t::options_t::debug);
			// Разрешаем использовать токен неизвестного слова
			if(env.is("allow-unk")) toolkit.setOption(toolkit_t::options_t::allowUnk);
			// Разрешаем сбрасывать частоту токена неизвестного слова
			if(env.is("reset-unk")) toolkit.setOption(toolkit_t::options_t::resetUnk);
			// Разрешаем использовать абсолютно все полученные n-граммы
			if(env.is("all-grams")) toolkit.setOption(toolkit_t::options_t::allGrams);
			// Устанавливаем запрет на все слова кроме тех, что находятся в белом списке
			if(env.is("only-good")) toolkit.setOption(toolkit_t::options_t::onlyGood);
			// Разрешаем переводить слова в нижний регистр
			if(env.is("lower-case")) toolkit.setOption(toolkit_t::options_t::lowerCase);
			// Разрешаем детектировать слова состоящее из смешанных словарей
			if(env.is("mixed-dicts")) toolkit.setOption(toolkit_t::options_t::mixdicts);
			// Разрешаем выполнять интерполяцию при расчёте arpa
			if(env.is("interpolate")) toolkit.setOption(toolkit_t::options_t::interpolate);
			// Если нужно установить все токены для идентифицирования как <unk>
			if(env.is("tokens-all-unknown")) toolkit.setAllTokenUnknown();
			// Если нужно установить все токены как не идентифицируемые
			if(env.is("tokens-all-disable")) toolkit.setAllTokenDisable();
			// Если нужно установить список токенов которые нужно идентифицировать как <unk>
			if((value = env.get("tokens-unknown")) != nullptr) toolkit.setTokenUnknown(value);
			// Если нужно установить список не идентифицируемых токенов
			if((value = env.get("tokens-disable")) != nullptr) toolkit.setTokenDisable(value);
			// Если адрес файла чёрного списка получен
			if((value = env.get("badwords")) != nullptr){
				// Чёрный список слов
				vector <string> badwords;
				// Выполняем считывание всех слов для чёрного списка
				fsys_t::rfile(value, [&badwords, &toolkit](const string & line, const uintmax_t fileSize) noexcept {
					// Если текст получен
					if(!line.empty()) badwords.push_back(line);
				});
				// Если чёрный список получен, устанавливаем его
				if(!badwords.empty()) toolkit.setBadwords(badwords);
			}
			// Если адрес файла белого списка получен
			if((value = env.get("goodwords")) != nullptr){
				// Белый список слов
				vector <string> goodwords;
				// Выполняем считывание всех слов для белого списка
				fsys_t::rfile(value, [&goodwords, &toolkit](const string & line, const uintmax_t fileSize) noexcept {
					// Если текст получен
					if(!line.empty()) goodwords.push_back(line);
				});
				// Если белый список получен, устанавливаем его
				if(!goodwords.empty()) toolkit.setGoodwords(goodwords);
			}
			// Если неизвестное слово получено
			if((value = env.get("unknown-word")) != nullptr) toolkit.setUnknown(value);
			// Если пользовательские токены получены
			if(((value = env.get("utokens")) != nullptr) && (string(value).compare("-yes-") != 0)){
				// Список пользовательских токенов
				vector <wstring> tokens;
				// Выполняем извлечение пользовательских токенов
				alphabet.split(value, "|", tokens);
				// Если список токенов получен
				if(!tokens.empty()){
					// Если адрес скрипта получен
					if((value = env.get("utoken-script")) != nullptr){
						// Устанавливаем адрес скрипта
						toolkit.setUserTokenScript(value);
						// Переходим по всему списку токенов
						for(auto & item : tokens) toolkit.setUserToken(alphabet.convert(item));
					}
				}
			}
			// Если адрес скрипта получен
			if((value = env.get("word-script")) != nullptr) toolkit.setWordScript(value);
			// Если нужно использовать бинарный контейнер
			if(!binDictFile.empty()){
				// Создаём бинарный контейнер
				ablm_t ablm(binDictFile, &toolkit, &alphabet, &tokenizer, env.get("log"));
				// Если метаданные переданы
				if(((value = env.get("r-bin-meta")) != nullptr) && fsys_t::isfile(value)){
					// Данные в формате json
					string data = "";
					// Выполняем считывание всех строк текста
					fsys_t::rfile(realpath(value, nullptr), [&data](const string & line, const uintmax_t fileSize) noexcept {
						// Добавляем полученные строки
						data.append(line);
					});
					// Если скрипт получен
					if(!data.empty()) ablm.setMeta(json::parse(data));
				}
				// Устанавливаем флаг отладки
				if(debug == 1) ablm.setFlag(ablm_t::flag_t::debug);
				// Выполняем инициализацию словаря
				ablm.init();
				// Если отладка включена, выводим индикатор загрузки
				if(debug > 0){
					// Устанавливаем заголовки прогресс-бара
					pss.title("Read dictionary", "Read dictionary is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Выполняем чтение бинарных данных
				ablm.read([debug, &pss](const u_short status) noexcept {
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(status); break;
						case 2: pss.status(status); break;
					}
				}, env.is("method", "info"));
				// Если это запрос информации о словаре
				if(env.is("method", "info")){
					// Выводим информацию о словаре
					ablm.info();
					// Выходим из приложения∂
					exit(0);
				}
			// Если это метод токенизации
			} else if(env.is("method", "tokens")) {
				// Идентификатор документа
				size_t size = 0;
				// Результат в формате json
				json jsonData = {};
				// Результат в формате txt
				string textData = "";
				// Статус и процентное соотношение
				u_short status = 0, rate = 100;
				// Если отладка включена, выводим индикатор загрузки
				if((debug > 0) && !env.is("text")){
					// Очищаем предыдущий прогресс-бар
					pss.clear();
					// Устанавливаем заголовки прогресс-бара
					pss.title("Tokenization", "Tokenization is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Если файл текстового корпуса для токенизации получен
				if(((value = env.get("r-tokens-text")) != nullptr) && fsys_t::isfile(value)){
					// Получаем адрес текстового файла для чтения
					const string readfile = realpath(value, nullptr);
					// Если адрес текстового или json файла для записи, получен
					if((((value = env.get("w-tokens-text")) != nullptr) ||
					((value = env.get("w-tokens-json")) != nullptr)) &&
					(string(value).compare("-yes-") != 0)){
						// Получаем адрес файла для записи
						const string writefile = value;
						// Устанавливаем название файла
						if(debug > 0) pss.description(readfile + string(" -> ") + writefile);
						// Выполняем считывание всех строк текста
						fsys_t::rfile(readfile, [&](const string & text, const uintmax_t fileSize) noexcept {
							// Если текст получен
							if(!text.empty()){
								// Выполняем преобразование текста в json
								tokenizer.textToJson(text, [&](const string & text) noexcept {
									// Если текст получен
									if(!text.empty()){
										// Если нужно сохранить json
										if(env.is("w-tokens-json")) jsonData.push_back(json::parse(text));
										// Если нужно сохранить в виде текста
										else if(env.is("w-tokens-text")) {
											// Пытаемся восстановить текст из json объекта
											tokenizer.jsonToText(text, [&](const string & text) noexcept {
												// Если текст получен
												if(!text.empty()) textData.append(alphabet.format("%s\r\n", text.c_str()));
											});
										}
									}
								});
							}
							// Если отладка включена
							if(debug > 0){
								// Общий полученный размер данных
								size += text.size();
								// Подсчитываем статус выполнения
								status = u_short(size / float(fileSize) * 100.0f);
								// Если процентное соотношение изменилось
								if(rate != status){
									// Запоминаем текущее процентное соотношение
									rate = status;
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								}
							}
						});
						// Если результат получен
						if(!jsonData.empty() || !textData.empty()){
							// Открываем файл на запись
							ofstream file(writefile, ios::binary);
							// Если файл открыт
							if(file.is_open()){
								// Формируем текст результата
								const string & text = (env.is("w-tokens-json") ? jsonData.dump(4) : textData);
								// Выполняем запись данных в файл
								file.write(text.data(), text.size());
								// Закрываем файл
								file.close();
							}
							// Если отладка включена
							if(status < 100){
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							}
						// Сообщаем что контекст пустой
						} else print("context is empty", env.get("log"));
					// Выходим из приложения и выводим сообщение
					} else print("file to write result is not found", env.get("log"));
				// Если файл json с токенами получен
				} else if(((value = env.get("r-tokens-json")) != nullptr) && fsys_t::isfile(value)){
					// Получаем адрес json файла для чтения
					const string readfile = realpath(value, nullptr);
					// Если адрес текстового файла для записи, получен
					if(((value = env.get("w-tokens-text")) != nullptr) && (string(value).compare("-yes-") != 0)){
						// Получаем адрес файла для записи
						const string writefile = value;
						// Устанавливаем название файла
						if(debug > 0) pss.description(readfile + string(" -> ") + writefile);
						// Выполняем считывание всех строк текста
						fsys_t::rfile(readfile, [&](const string & text, const uintmax_t fileSize) noexcept {
							// Если текст получен
							if(!text.empty()) textData.append(text);
							// Если отладка включена
							if(debug > 0){
								// Общий полученный размер данных
								size += text.size();
								// Подсчитываем статус выполнения
								status = u_short(size / float(fileSize) * 100.0f);
								// Если процентное соотношение изменилось
								if(rate != status){
									// Запоминаем текущее процентное соотношение
									rate = status;
									// Отображаем ход процесса
									switch(debug){
										case 1: pss.update(status); break;
										case 2: pss.status(status); break;
									}
								}
							}
						});
						// Если результат получен
						if(!textData.empty()){
							// Выполняем парсинг json данных
							jsonData = json::parse(textData);
							// Если json данные получены
							if(!jsonData.empty() && jsonData.is_array()){
								// Очищаем блок текстовых данных
								textData.clear();
								// Переходим по всем json данным
								for(auto & item : jsonData.items()){
									// Если данные получены
									if(item.value().is_array()){
										// Пытаемся восстановить текст из json объекта
										tokenizer.jsonToText(item.value().dump(), [&](const string & text) noexcept {
											// Если текст получен
											if(!text.empty()) textData.append(alphabet.format("%s\r\n", text.c_str()));
										});
									}
								}
								// Если результат получен
								if(!textData.empty()){
									// Открываем файл на запись
									ofstream file(writefile, ios::binary);
									// Если файл открыт
									if(file.is_open()){
										// Выполняем запись данных в файл
										file.write(textData.data(), textData.size());
										// Закрываем файл
										file.close();
									}
								// Сообщаем что контекст в json файле пустой
								} else print("context in json file is empty", env.get("log"));
							// Сообщаем что json файл испорчен
							} else print("broken json file", env.get("log"));
						// Сообщаем что контекст пустой
						} else print("context is empty", env.get("log"));
						// Если отладка включена
						if(status < 100){
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						}
					// Выходим из приложения и выводим сообщение
					} else print("file to write result is not found", env.get("log"));
				// Если каталог с текстовымы файлами корпуса получен
				} else if(((value = env.get("r-tokens-path")) != nullptr) && fsys_t::isdir(value)) {
					// Получаем адрес каталога с текстовыми файлами для чтения
					const string readpath = realpath(value, nullptr);
					// Если адрес каталога для записи, получен
					if((value = env.get("w-tokens-path")) != nullptr){
						// Получаем адрес каталога для записи
						const string writepath = value;
						// Создаём каталог если не существует
						if(!fsys_t::isdir(writepath)) fsys_t::mkdir(writepath);
						// Расширение файлов текстового корпуса
						const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
						// Переходим по всему списку файлов в каталоге
						fsys_t::rdir(readpath, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
							// Очищаем объект данных json
							jsonData.clear();
							// Очищаем текстовый блок данных
							textData.clear();
							// Устанавливаем название файла
							if(debug > 0) pss.description(filename);
							// Выполняем считывание всех строк текста
							fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!text.empty()){
									// Если это json
									if(ext.compare("json") == 0) textData.append(text);
									// Если это текстовый формат
									else {
										// Выполняем преобразование текста в json
										tokenizer.textToJson(text, [&](const string & text) noexcept {
											// Если текст получен
											if(!text.empty()) jsonData.push_back(json::parse(text));
										});
									}
								}
								// Если отладка включена
								if(debug > 0){
									// Общий полученный размер данных
									size += text.size();
									// Подсчитываем статус выполнения
									status = u_short(size / float(dirSize) * 100.0f);
									// Если процентное соотношение изменилось
									if(rate != status){
										// Запоминаем текущее процентное соотношение
										rate = status;
										// Отображаем ход процесса
										switch(debug){
											case 1: pss.update(status); break;
											case 2: pss.status(status); break;
										}
									}
								}
							});
							// Если это json
							if(ext.compare("json") == 0){
								// Если результат получен
								if(!textData.empty()){
									// Выполняем парсинг json данных
									jsonData = json::parse(textData);
									// Если json данные получены
									if(!jsonData.empty() && jsonData.is_array()){
										// Очищаем блок текстовых данных
										textData.clear();
										// Переходим по всем json данным
										for(auto & item : jsonData.items()){
											// Если данные получены
											if(item.value().is_array()){
												// Пытаемся восстановить текст из json объекта
												tokenizer.jsonToText(item.value().dump(), [&](const string & text) noexcept {
													// Если текст получен
													if(!text.empty()) textData.append(alphabet.format("%s\r\n", text.c_str()));
												});
											}
										}
										// Если результат получен
										if(!textData.empty()){
											// Получаем параметры файла
											auto param = fsys_t::file(filename);
											// Если имя файла получено
											if(!param.first.empty()){
												// Формируем имя файла для записи
												const string & filename = alphabet.format("%s/%s.txt", writepath.c_str(), param.first.c_str());
												// Открываем файл на запись
												ofstream file(filename, ios::binary);
												// Если файл открыт
												if(file.is_open()){
													// Выполняем запись данных в файл
													file.write(textData.data(), textData.size());
													// Закрываем файл
													file.close();
												}
											}
										// Сообщаем что контекст в json файле пустой
										} else print("context in json file is empty", env.get("log"));
									// Сообщаем что json файл испорчен
									} else print("broken json file", env.get("log"));
								// Сообщаем что контекст пустой
								} else print("context is empty", env.get("log"));
							// Если это текстовые данные
							} else if(!jsonData.empty()) {
								// Получаем параметры файла
								auto param = fsys_t::file(filename);
								// Если имя файла получено
								if(!param.first.empty()){
									// Формируем имя файла для записи
									const string & filename = alphabet.format("%s/%s.json", writepath.c_str(), param.first.c_str());
									// Открываем файл на запись
									ofstream file(filename, ios::binary);
									// Если файл открыт
									if(file.is_open()){
										// Формируем текст результата
										const string & text = jsonData.dump(4);
										// Выполняем запись данных в файл
										file.write(text.data(), text.size());
										// Закрываем файл
										file.close();
									}
								}
							// Сообщаем что контекст пустой
							} else print("context is empty", env.get("log"));
							// Если отладка включена
							if(status < 100){
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(100); break;
									case 2: pss.status(100); break;
								}
							}
						});
					// Выходим из приложения и выводим сообщение
					} else print("path to write result is not found", env.get("log"));
				// Если текст передан
				} else if((value = env.get("text")) != nullptr) {
					// Флаг входящего типа данных
					bool isJSON = false;
					// Получаем значение текста
					const string text = value;
					// Проверяем является ли текст json-ом
					if((text.front() == '[') && (text.back() == ']')){
						// Запоминаем что это json
						isJSON = true;
						// Пытаемся восстановить текст из json объекта
						tokenizer.jsonToText(text, [&](const string & text) noexcept {
							// Если текст получен
							if(!text.empty()) textData.append(alphabet.format("%s\r\n", text.c_str()));
						});
					// Иначе обрабатываем как обычный текст
					} else {
						// Выполняем преобразование текста в json
						tokenizer.textToJson(text, [&](const string & text) noexcept {
							// Если текст получен
							if(!text.empty()) textData = move(alphabet.format("%s\r\n", text.c_str()));
						});
					}
					// Если результат получен
					if(!textData.empty()){
						// Получаем адрес файла для записи
						const string writefile = (
							(isJSON && env.is("w-tokens-json")) ? env.get("w-tokens-json") :
							((!isJSON && env.is("w-tokens-text")) ? env.get("w-tokens-text") : "")
						);
						// Если данные получены
						if(!writefile.empty() && (writefile.compare("-yes-") != 0)){
							// Открываем файл на запись
							ofstream file(writefile, ios::binary);
							// Если файл открыт
							if(file.is_open()){
								// Выполняем запись данных в файл
								file.write(textData.data(), textData.size());
								// Закрываем файл
								file.close();
							}
						// Выводим сообщение в консоль
						} else cout << textData;
					// Сообщаем что контекст пустой
					} else print("context is empty", env.get("log"));
				// Выходим из приложения и выводим сообщение
				} else print("text corpus for tokenization is not found", env.get("log"));
				// Выходим из приложения
				exit(0);
			// Иначе выполняем инициализацию алгоритма сглаживания
			} else if(env.is("smoothing")) {
				// Если это WittenBell
				if(env.is("smoothing", "wittenbell")) toolkit.init(toolkit_t::algorithm_t::wittenBell, false, false, 0.0);
				// Если это AddSmooth
				else if(env.is("smoothing", "addsmooth")) toolkit.init(toolkit_t::algorithm_t::addSmooth, false, false, mod);
				// Если это GoodTuring
				else if(env.is("smoothing", "goodturing")) toolkit.init(toolkit_t::algorithm_t::goodTuring, false, false, 0.0);
				// Если это ConstDiscount
				else if(env.is("smoothing", "cdiscount")) toolkit.init(toolkit_t::algorithm_t::constDiscount, false, false, mod);
				// Если это NaturalDiscount
				else if(env.is("smoothing", "ndiscount")) toolkit.init(toolkit_t::algorithm_t::naturalDiscount, false, false, 0.0);
				// Если это KneserNey
				else if(env.is("smoothing", "kneserney")) toolkit.init(toolkit_t::algorithm_t::kneserNey, env.is("kneserney-modified"), env.is("kneserney-prepares"), 0.0);
				// Если это ModKneserNey
				else if(env.is("smoothing", "mkneserney")) toolkit.init(toolkit_t::algorithm_t::modKneserNey, env.is("kneserney-modified"), env.is("kneserney-prepares"), 0.0);
				// Сообщаем что сглаживание выбрано не верно
				else print("smoothing is bad", env.get("log"));
			// Сообщаем что сглаживание выбрано не верно
			} else print("smoothing is bad", env.get("log"));
			// Замеряем время начала работы
			auto timeShifting = chrono::system_clock::now();
			// Если файл с буквами для восстановления слов, передан
			if(((value = env.get("r-mix-restwords")) != nullptr) && fsys_t::isfile(value)){
				// Идентификатор документа
				size_t size = 0;
				// Статус и процентное соотношение
				u_short status = 0, rate = 100;
				// Запоминаем адрес файла
				const string filename = realpath(value, nullptr);
				// Если отладка включена, выводим индикатор загрузки
				if(debug > 0){
					// Очищаем предыдущий прогресс-бар
					pss.clear();
					// Устанавливаем название файла
					pss.description(filename);
					// Устанавливаем заголовки прогресс-бара
					pss.title("Load mixed letters", "Load mixed letters is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Список полученных букв
				map <string, string> letters;
				// Выполняем считывание всех строк текста
				fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
					// Если текст получен
					if(!text.empty()){
						// Список пар букв
						vector <wstring> result;
						// Выполняем сплит текста
						alphabet.split(text, "\t", result);
						// Если результат получен
						if(!result.empty() && (result.size() == 2)){
							// Формируем список букв
							letters.emplace(alphabet.convert(result.at(0)), alphabet.convert(result.at(1)));
						}
					}
					// Если отладка включена
					if(debug > 0){
						// Общий полученный размер данных
						size += text.size();
						// Подсчитываем статус выполнения
						status = u_short(size / float(fileSize) * 100.0f);
						// Если процентное соотношение изменилось
						if(rate != status){
							// Запоминаем текущее процентное соотношение
							rate = status;
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						}
					}
				});
				// Устанавливаем собранные буквы
				if(!letters.empty()) alphabet.setSubstitutes(letters);
				// Если отладка включена
				if(status < 100){
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				}
			}
			// Если передан метод обучения, загрузка карт последовательностей или списка n-грамм
			if((env.is("r-abbrs") || env.is("r-domain-zones")) && (env.is("method", "train") ||
			env.is("r-map") || env.is("r-maps") || env.is("r-ngram") || env.is("r-ngrams"))){
				// Тип считываемого файла
				u_short type = 0;
				// Устанавливаем режим считывания файла аббревиатур
				if(((value = env.get("r-abbrs")) != nullptr) && fsys_t::isfile(value)) type = 2;
				// Устанавливаем режим считывания файла доменных зон
				else if(((value = env.get("r-domain-zones")) != nullptr) && fsys_t::isfile(value)) type = 1;
				// Выводим сообщение, что файл не найден
				else print(alphabet.format("filename %s is not found", value), env.get("log"), alphabet_t::log_t::warning, false);
				// Если тип файла определён
				if(type > 0){
					// Идентификатор документа
					size_t size = 0;
					// Статус и процентное соотношение
					u_short status = 0, rate = 100;
					// Запоминаем адрес файла
					const string filename = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Устанавливаем название файла
						pss.description(filename);
						// Устанавливаем заголовки прогресс-бара
						switch(type){
							// Если это загрузка доменных зон
							case 1: pss.title("Load domain zones", "Load domain zones is done"); break;
							// Если это загрузка аббревиатур
							case 2: pss.title("Load abbreviations", "Load abbreviations is done"); break;
						}
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем считывание всех строк текста
					fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
						// Если текст получен
						if(!text.empty()){
							// Устанавливаем собранные текстовые данные
							switch(type){
								// Добавляем доменную зону
								case 1: alphabet.setzone(text); break;
								// Добавляем абреввиатуру
								case 2: tokenizer.setAbbr(text); break;
							}
						}
						// Если отладка включена
						if(debug > 0){
							// Общий полученный размер данных
							size += text.size();
							// Подсчитываем статус выполнения
							status = u_short(size / float(fileSize) * 100.0f);
							// Если процентное соотношение изменилось
							if(rate != status){
								// Запоминаем текущее процентное соотношение
								rate = status;
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							}
						}
					});
					// Если отладка включена
					if(status < 100){
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
				}
			}
			// Если передан метод обучения
			if(env.is("method", "train")){
				// Если нужно использовать бинарный контейнер
				if(!binDictFile.empty() && env.is("w-arpa")){
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем заголовки прогресс-бара
						pss.title("Train arpa", "Train arpa is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем обучение
					toolkit.train([debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				// Иначе продолжаем стандартное обучение
				} else {
					// Если путь получен
					if(((value = env.get("path")) != nullptr) && fsys_t::isdir(value)){
						// Запоминаем путь к файлам
						const string path = realpath(value, nullptr);
						// Расширение файлов текстового корпуса
						const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
						// Если количество ядер передано
						if(((value = env.get("threads")) != nullptr) &&
						alphabet.isNumber(alphabet.convert(value))){
							// Объявляем объект коллектора
							collector_t collector(&toolkit, &alphabet, &tokenizer, env.get("log"));
							// Устанавливаем режим отладки
							collector.setDebug(debug);
							// Устанавливаем размер n-граммы
							collector.setOrder(order);
							// Устанавливаем количество потоков
							collector.setThreads(stoi(value));
							// Устанавливаем флаг автоочистки
							collector.setAutoClean(env.is("train-autoclean"));
							// Устанавливаем флаг сегментации
							collector.setSegment(
								env.is("train-segments"),
								((value = env.get("train-segment-size")) != nullptr ? value : "auto")
							);
							// Выполняем чтение данных файла
							collector.readDir(path, ext);
						// Иначе выполняем сборку обычным способом
						} else {
							// Идентификатор документа
							size_t idd = 0, size = 0;
							// Статус и процентное соотношение
							u_short status = 0, rate = 100;
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Устанавливаем заголовки прогресс-бара
								pss.title("Load text corpus", "Load text corpus is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Переходим по всему списку файлов в каталоге
							fsys_t::rdir(path, ext, [&idd, &rate, &status, &size, debug, &pss, &toolkit](const string & filename, const uintmax_t dirSize) noexcept {
								// Устанавливаем название файла
								if(debug > 0) pss.description(filename);
								// Выполняем считывание всех строк текста
								fsys_t::rfile(filename, [idd, &rate, &status, &size, dirSize, debug, &pss, &toolkit](const string & text, const uintmax_t fileSize) noexcept {
									// Если текст получен
									if(!text.empty()) toolkit.addText(text, idd);
									// Если отладка включена
									if(debug > 0){
										// Общий полученный размер данных
										size += text.size();
										// Подсчитываем статус выполнения
										status = u_short(size / float(dirSize) * 100.0f);
										// Если процентное соотношение изменилось
										if(rate != status){
											// Запоминаем текущее процентное соотношение
											rate = status;
											// Отображаем ход процесса
											switch(debug){
												case 1: pss.update(status); break;
												case 2: pss.status(status); break;
											}
										}
									}
								});
								// Увеличиваем идентификатор документа
								idd++;
							});
						}
						// Если файл arpa для записи указан
						if(env.is("w-arpa")){
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Train arpa", "Train arpa is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем обучение
							toolkit.train([debug, &pss](const u_short status) noexcept {
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						}
					// Если файл корпуса получен
					} else if(((value = env.get("corpus")) != nullptr) && fsys_t::isfile(value)){
						// Запоминаем адрес файла
						const string filename = realpath(value, nullptr);
						// Если количество ядер передано
						if(((value = env.get("threads")) != nullptr) &&
						alphabet.isNumber(alphabet.convert(value))){
							// Объявляем объект коллектора
							collector_t collector(&toolkit, &alphabet, &tokenizer, env.get("log"));
							// Устанавливаем режим отладки
							collector.setDebug(debug);
							// Устанавливаем размер n-граммы
							collector.setOrder(order);
							// Устанавливаем количество потоков
							collector.setThreads(stoi(value));
							// Устанавливаем флаг автоочистки
							collector.setAutoClean(env.is("train-autoclean"));
							// Устанавливаем флаг сегментации
							collector.setSegment(
								env.is("train-segments"),
								((value = env.get("train-segment-size")) != nullptr ? value : "auto")
							);
							// Выполняем чтение данных файла
							collector.readFile(filename);
						// Иначе выполняем сборку обычным способом
						} else {
							// Идентификатор документа
							size_t size = 0;
							// Статус и процентное соотношение
							u_short status = 0, rate = 100;
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Устанавливаем название файла
								pss.description(filename);
								// Устанавливаем заголовки прогресс-бара
								pss.title("Load text corpus", "Load text corpus is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем считывание всех строк текста
							fsys_t::rfile(filename, [&rate, &status, &size, debug, &pss, &toolkit](const string & text, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!text.empty()) toolkit.addText(text, 0);
								// Если отладка включена
								if(debug > 0){
									// Общий полученный размер данных
									size += text.size();
									// Подсчитываем статус выполнения
									status = u_short(size / float(fileSize) * 100.0f);
									// Если процентное соотношение изменилось
									if(rate != status){
										// Запоминаем текущее процентное соотношение
										rate = status;
										// Отображаем ход процесса
										switch(debug){
											case 1: pss.update(status); break;
											case 2: pss.status(status); break;
										}
									}
								}
							});
						}
						// Если файл arpa для записи указан
						if(env.is("w-arpa")){
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Очищаем предыдущий прогресс-бар
								pss.clear();
								// Устанавливаем заголовки прогресс-бара
								pss.title("Train arpa", "Train arpa is done");
								// Выводим индикатор прогресс-бара
								switch(debug){
									case 1: pss.update(); break;
									case 2: pss.status(); break;
								}
							}
							// Выполняем обучение
							toolkit.train([debug, &pss](const u_short status) noexcept {
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							});
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						}
					// Если путь не указан
					} else print("path or file with corpus texts is not specified", env.get("log"));
				}
			// Проверяем правильно ли указаны адреса файлов
			} else if(((env.is("r-map") || env.is("r-maps")) &&
			(env.is("r-vocab") || env.is("r-vocabs"))) || (env.is("r-ngram") ||
			env.is("r-ngrams")) || (env.is("r-arpa") || env.is("r-arpas")) || !binDictFile.empty()) {
				// Если требуется загрузить файл n-грамм
				if(((value = env.get("r-ngram")) != nullptr) && fsys_t::isfile(value)){
					// Запоминаем адрес файла
					const string filename = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(filename);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read n-gram file", "Read n-gram file is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку файла n-грамм
					toolkit.readNgram(filename, [debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				// Если требуется загрузить список файлов n-грамм
				} else if(((value = env.get("r-ngrams")) != nullptr) && fsys_t::isdir(value)) {
					// Запоминаем каталог для загрузки
					const string path = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(path);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read n-grams files", "Read n-grams files is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку файлов n-грамм
					toolkit.readNgrams(path, [debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				}
				// Если требуется загрузить arpa
				if(((value = env.get("r-arpa")) != nullptr) && fsys_t::isfile(value)){
					// Запоминаем адрес файла
					const string filename = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(filename);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read arpa file", "Read arpa file is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку файла arpa
					toolkit.readArpa(filename, [debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				// Если нужно загрузить список файлов arpa
				} else if(((value = env.get("r-arpas")) != nullptr) && fsys_t::isdir(value)) {
					// Запоминаем каталог для загрузки
					const string path = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(path);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read arpa files", "Read arpa files is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку файла arpa
					toolkit.readArpas(path, [debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				}
				// Если требуется загрузить файл словаря vocab
				if(((value = env.get("r-vocab")) != nullptr) && fsys_t::isfile(value)){
					// Запоминаем адрес файла
					const string filename = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(filename);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read vocab file", "Read vocab file is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку файла словаря vocab
					toolkit.readVocab(filename, [debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				// Если требуется загрузить список словарей
				} else if(((value = env.get("r-vocabs")) != nullptr) && fsys_t::isdir(value)) {
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read vocabs files", "Read vocabs files is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Переходим по всему списку словарей в каталоге
					fsys_t::rdir(realpath(value, nullptr), "vocab", [debug, &pss, &toolkit](const string & filename, const uintmax_t dirSize) noexcept {
						// Устанавливаем название файла
						if(debug > 0) pss.description(filename);
						// Выполняем загрузку файла словаря vocab
						toolkit.readVocab(filename, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
					});
				}
				// Если требуется загрузить карту последовательности или список карт последовательностей
				if((env.is("r-map") || env.is("r-maps")) && (env.is("r-vocab") || env.is("r-vocabs"))){
					// Если нужно загрузить карту последовательности
					if(((value = env.get("r-map")) != nullptr) && fsys_t::isfile(value)){
						// Запоминаем адрес файла
						const string filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(filename);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read map file", "Read map file is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Считываем карту последовательности
						toolkit.readMap(filename, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					// Если нужно загрузить список карт последовательностей
					} else if(((value = env.get("r-maps")) != nullptr) && fsys_t::isdir(value)){
						// Запоминаем каталог для загрузки
						const string path = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(path);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read maps files", "Read maps files is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Считываем список карт последовательностей
						toolkit.readMaps(path, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
				}
				// Если конфигурация файлов верная и требуется обучение
				if(env.is("w-arpa") && (env.is("r-map") || env.is("r-maps") || env.is("r-ngram") || env.is("r-ngrams"))){
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем заголовки прогресс-бара
						pss.title("Train arpa", "Train arpa is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем обучение
					toolkit.train([debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				}
				// Если передан метод исправления arpa
				if(env.is("method", "repair")){
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем заголовки прогресс-бара
						pss.title("Repair arpa", "Repair arpa is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем исправление arpa файла
					toolkit.repair([debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				// Если это метод удаление редких n-грамм
				} else if(env.is("method", "sweep")) {
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем заголовки прогресс-бара
						pss.title("Sweep arpa", "Sweep arpa is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем удаление редких n-грамм
					toolkit.sweep([debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				// Если нужно выполнить прунинг словаря
				} else if(env.is("method", "vprune") && ((value = env.get("vprune-threshold")) != nullptr)) {
					// Выполняем является ли переданная строка числом
					if(alphabet.isDecimal(alphabet.convert(value))){
						// Если отладка включена, выводим индикатор прунинга
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Prune vocab", "Prune vocab is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем прунинг словаря
						toolkit.pruneVocab(stof(value), [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					// Сообщаем что порог передан неверный
					} else print("vprune-threshold is broken", env.get("log"));
				// Если нужно выполнить прунинг arpa
				} else if(env.is("method", "aprune")) {
					// Максимальный размер n-граммы
					u_short size = 0;
					// Коэффициент прунинга
					float prune = 0.0f;
					// Если параметр прунинга получен
					if((value = env.get("aprune-threshold")) != nullptr) prune = stof(value);
					// Если параметр максимального размера n-граммы для прунинга получен
					if((value = env.get("aprune-max-gram")) != nullptr) size = stoi(value);
					// Если параметры получены
					if((size > 0) && (prune != 0.0f)){
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем заголовки прогресс-бара
							pss.title("Prune arpa", "Prune arpa is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем прунинг
						toolkit.prune(prune, size, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
				// Если нужно модифицировать arpa
				} else if(env.is("method", "modify")) {
					// Флаг модификации
					toolkit_t::modify_t modify;
					// Если флаг передан
					if(env.is("modify")){
						// Если это флаг добавления n-грамм
						if(env.is("modify", "emplace")) modify = toolkit_t::modify_t::emplace;
						// Если это флаг удаления n-грамм
						else if(env.is("modify", "remove")) modify = toolkit_t::modify_t::remove;
						// Если это флаг изменения n-грамм
						else if(env.is("modify", "change")) modify = toolkit_t::modify_t::change;
						// Если это флаг замены n-грамм
						else if(env.is("modify", "replace")) modify = toolkit_t::modify_t::replace;
						// Иначе выходим
						else print("modify flag is broken", env.get("log"));
					// Сообщаем что флаг не установлен
					} else print("modify flag is not set", env.get("log"));
					// Если файл передан
					if(((value = env.get("modify-file")) != nullptr) && fsys_t::isfile(value)){
						// Запоминаем адрес файла
						const string filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(filename);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Modify arpa", "Modify arpa is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем модификацию файла
						toolkit.modify(filename, modify, [debug, &pss](const u_short status) noexcept {
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					// Сообщаем что файл модификации не передан
					} else print("file modify not found", env.get("log"));
				}
			// Выводим сообщение что файлы не переданы
			} else print("arpa file is not loaded", env.get("log"));
			// Если файл для сохранения vocab передан
			if((value = env.get("w-vocab")) != nullptr){
				// Если отладка включена, выводим индикатор загрузки
				if(debug > 0){
					// Очищаем предыдущий прогресс-бар
					pss.clear();
					// Устанавливаем название файла
					pss.description(value);
					// Устанавливаем заголовки прогресс-бара
					pss.title("Write vocab", "Write vocab is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Выполняем извлечение словаря в файл
				toolkit.writeVocab(value, [debug, &pss](const u_short status) noexcept {
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(status); break;
						case 2: pss.status(status); break;
					}
				});
				// Отображаем ход процесса
				switch(debug){
					case 1: pss.update(100); break;
					case 2: pss.status(100); break;
				}
			}
			// Если файл для извлечения карты последовательности передан
			if((env.is("method", "train") || env.is("r-map") || env.is("r-maps") || env.is("r-ngram") ||
			env.is("r-ngrams") || !binDictFile.empty()) && ((value = env.get("w-map")) != nullptr)){
				// Если отладка включена, выводим индикатор загрузки
				if(debug > 0){
					// Очищаем предыдущий прогресс-бар
					pss.clear();
					// Устанавливаем название файла
					pss.description(value);
					// Устанавливаем заголовки прогресс-бара
					pss.title("Write map", "Write map is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Выполняем сохранение каты последовательности
				toolkit.writeMap(value, [debug, &pss](const u_short status) noexcept {
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(status); break;
						case 2: pss.status(status); break;
					}
				});
				// Отображаем ход процесса
				switch(debug){
					case 1: pss.update(100); break;
					case 2: pss.status(100); break;
				}
			}
			// Если файл для сохранения arpa передан
			if((value = env.get("w-arpa")) != nullptr){
				// Если отладка включена, выводим индикатор загрузки
				if(debug > 0){
					// Очищаем предыдущий прогресс-бар
					pss.clear();
					// Устанавливаем название файла
					pss.description(value);
					// Устанавливаем заголовки прогресс-бара
					pss.title("Write arpa", "Write arpa is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Выполняем сохранение arpa файла
				toolkit.writeArpa(value, [debug, &pss](const u_short status) noexcept {
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(status); break;
						case 2: pss.status(status); break;
					}
				});
				// Отображаем ход процесса
				switch(debug){
					case 1: pss.update(100); break;
					case 2: pss.status(100); break;
				}
			}
			// Если файл для сохранения n-грамм передан
			if((env.is("method", "train") || env.is("r-map") || env.is("r-maps") || env.is("r-ngram") ||
			env.is("r-ngrams") || !binDictFile.empty()) && ((value = env.get("w-ngram")) != nullptr)){
				// Если отладка включена, выводим индикатор загрузки
				if(debug > 0){
					// Очищаем предыдущий прогресс-бар
					pss.clear();
					// Устанавливаем название файла
					pss.description(value);
					// Устанавливаем заголовки прогресс-бара
					pss.title("Write ngrams", "Write ngrams is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Выполняем сохранение файлов n-грамм
				toolkit.writeNgrams(value, [debug, &pss](const u_short status) noexcept {
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(status); break;
						case 2: pss.status(status); break;
					}
				});
				// Отображаем ход процесса
				switch(debug){
					case 1: pss.update(100); break;
					case 2: pss.status(100); break;
				}
			}
			// Если нужно использовать бинарный контейнер
			if((value = env.get("w-bin")) != nullptr){
				// Создаём бинарный контейнер
				ablm_t ablm(value, &toolkit, &alphabet, &tokenizer, env.get("log"));
				// Если метаданные переданы
				if(((value = env.get("w-bin-meta")) != nullptr) && fsys_t::isfile(value)){
					// Данные в формате json
					string data = "";
					// Выполняем считывание всех строк текста
					fsys_t::rfile(realpath(value, nullptr), [&data](const string & line, const uintmax_t fileSize) noexcept {
						// Добавляем полученные строки
						data.append(line);
					});
					// Если скрипт получен
					if(!data.empty()) ablm.setMeta(json::parse(data));
				}
				// Устанавливаем флаг отладки
				if(debug == 1) ablm.setFlag(ablm_t::flag_t::debug);
				// Устанавливаем флаг сохранения только arpa данных
				if(env.is("w-bin-arpa")) ablm.setFlag(ablm_t::flag_t::onlyArpa);
				// Устанавливаем флаг сохранения всех данных
				if(env.is("w-bin-all")) ablm.setFlag(ablm_t::flag_t::expAllData);
				// Устанавливаем флаг экспорта опций тулкита
				if(env.is("w-bin-options")) ablm.setFlag(ablm_t::flag_t::expOptions);
				// Устанавливаем флаг экспорта скрипта предобработки текста
				if(env.is("w-bin-preword")) ablm.setFlag(ablm_t::flag_t::expPreword);
				// Устанавливаем флаг экспорта чёрного списка слов
				if(env.is("w-bin-badwords")) ablm.setFlag(ablm_t::flag_t::expBadwords);
				// Устанавливаем флаг экспорта доменных зон
				if(env.is("w-bin-domzones")) ablm.setFlag(ablm_t::flag_t::expDomZones);
				// Устанавливаем флаг экспорта пользовательских токенов
				if(env.is("w-bin-utokens")) ablm.setFlag(ablm_t::flag_t::expUserTokens);
				// Устанавливаем флаг экспорта белого списка слов
				if(env.is("w-bin-goodwords")) ablm.setFlag(ablm_t::flag_t::expGoodwords);
				// Устанавливаем флаг экспорта списка аббревиатур
				if(env.is("w-bin-abbreviations")) ablm.setFlag(ablm_t::flag_t::expAbbrevs);
				// Выполняем инициализацию словаря
				ablm.init();
				// Если отладка включена, выводим индикатор загрузки
				if(debug > 0){
					// Устанавливаем заголовки прогресс-бара
					pss.title("Write dictionary", "Write dictionary is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Выполняем запись бинарных данных
				ablm.write([debug, &pss](const u_short status) noexcept {
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(status); break;
						case 2: pss.status(status); break;
					}
				});
				// Отображаем ход процесса
				switch(debug){
					case 1: pss.update(100); break;
					case 2: pss.status(100); break;
				}
			}
			// Если режим отладки включён
			if(debug > 0){
				// Формируем строку результата времени работы
				const string & result = alphabet.format("work time shifting: %u seconds", chrono::duration_cast <chrono::seconds> (chrono::system_clock::now() - timeShifting).count());
				// Выводим результат
				print(result, env.get("log"), alphabet_t::log_t::info, false);
			}
		// Сообщаем что файл алфавита не найден
		} else print("file alphabet not found", env.get("log"));
	}
	// Успешно выходим
	return 0;
}

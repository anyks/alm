/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <env.hpp>
#include <alm.hpp>
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
	if((pos = appname.rfind("/")) != string::npos) appname = appname.substr(0, pos);
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
		ANYKS_LM_VERSION,
		__DATE__,
		__TIME__,
		osname,
		appname.c_str(),
		ANYKS_LM_AUTHOR,
		ANYKS_LM_CONTACT,
		ANYKS_LM_EMAIL,
		ANYKS_LM_SITE
	);
}
/**
 * help Функция вывода справки
 */
void help() noexcept {
	// Формируем строку справки
	const string msg = "\r\n\x1B[32m\x1B[1musage:\x1B[0m alm [-V | --version] [-h | --help] "
	"[-alphabet <value> | --alphabet=<value>] [<args>]\r\n\r\n\r\n"
	"\x1B[34m\x1B[1m[METHODS]\x1B[0m\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mppl\x1B[0m]       perplexity calculation method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mmix\x1B[0m]       mixing language models method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mfind\x1B[0m]      ngrams search method by text\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1minfo\x1B[0m]      binary dictionary information method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mtrain\x1B[0m]     language model training method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1msweep\x1B[0m]     high backoff n-gram removal method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mtokens\x1B[0m]    text tokenization method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mvprune\x1B[0m]    vocabulary pruning method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1maprune\x1B[0m]    language model pruning method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mcounts\x1B[0m]    method counts of ngrams in the text\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mrepair\x1B[0m]    broken language model recovery method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mfixcase\x1B[0m]   words case correction method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mchecktext\x1B[0m] text validation method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1msentences\x1B[0m] sentences generation method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mmerge\x1B[0m]     method merge of raw data from \x1B[1m*.map\x1B[0m files\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [\x1B[1mmodify\x1B[0m]    method for modifying a language model\r\n"
	"  \x1B[1m-\x1B[0m (emplace | remove | change | replace)\r\n\r\n\r\n"
	"\x1B[34m\x1B[1m[FLAGS]\x1B[0m\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-alm2 | --alm2]                               flag to set ALM type 2\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-accurate | --accurate]                       flag to accurate check ngram\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-allow-unk | --allow-unk]                     flag allowing to unknown word\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-only-good | --only-good]                     flag allowing to consider words from the white list only\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-all-grams | --all-grams]                     flag allowing accounting of all collected n-grams\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-reset-unk | --reset-unk]                     flag to reset the frequency of an unknown word\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin-all | --w-bin-all]                     flag export in binary dictionary of all data\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-lower-case | --lower-case]                   flag allowing to case-insensitive\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-confidence | --confidence]                   flag arpa file loading without pre-processing the words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin-arpa | --w-bin-arpa]                   flag export in binary dictionary of only arpa data without n-gram statistics\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-interpolate | --interpolate]                 flag allowing to use interpolation in estimating\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-mixed-dicts | --mixed-dicts]                 flag allowing the use of words consisting of mixed dictionaries\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-allow-stress | --allow-stress]               flag allowing the use of a stress symbol as part of the word\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-allow-uppers | --allow-uppers]               flag allowing to set upper case in words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-mix-backward | --mix-backward]               flag to mixing language models in the backward direction\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin-utokens | --w-bin-utokens]             flag export in binary dictionary of users tokens\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin-options | --w-bin-options]             flag export in binary dictionary of toolkit options\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin-preword | --w-bin-preword]             flag export in binary dictionary of script python - word preprocessing\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-train-intermed | --train-intermed]           flag to save intermediate data during training\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-train-segments | --train-segments]           flag to performing data segmentation during training\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin-domzones | --w-bin-domzones]           flag export in binary dictionary of domain zones\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin-badwords | --w-bin-badwords]           flag export in binary dictionary of blacklisted words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin-goodwords | --w-bin-goodwords]         flag export in binary dictionary of whitelisted words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-allow-apostrophe | --allow-apostrophe]       flag allowing the use of a hyphen as part of the word\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-only-token-words | --only-token-words]       flag that takes into account when assembling N-grams, only those tokens that match words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-tokens-all-disable | --tokens-all-disable]   flag to disable all tokens\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-tokens-all-unknown | --tokens-all-unknown]   flag identifying all tokens as <unk>\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-kneserney-prepares | --kneserney-prepares]   flag allowing need to change grams, after calculating\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-kneserney-modified | --kneserney-modified]   flag allowing modification of the number of already changed minor n-grams\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin-abbreviations | --w-bin-abbreviations] flag export in binary dictionary of abbreviations words\r\n\r\n"
	"\x1B[34m\x1B[1m[ARGS]\x1B[0m\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-log <value> | --log=<value>]                                               address of log file\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-size <value> | --size=<value>]                                             size n-grams of language model\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-step <value> | --step=<value>]                                             step size n-grams of language model\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-alphabet <value> | --alphabet=<value>]                                     list letters alphabet (file or value)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-badwords <value> | --badwords=<value>]                                     file address of blacklist words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-goodwords <value> | --goodwords=<value>]                                   file address of whitelist words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-unknown-word <value> | --unknown-word=<value>]                             name of token unknown word\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-word-script <value> | --word-script=<value>]                               address of script python3 - word preprocessing\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-utoken-script <value> | --utoken-script=<value>]                           address of script python3 - custom attribute detector\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-corpus <value> | --corpus=<value>]                                         address of text file training corpus\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-ext <value> | --ext=<value>]                                               extension of files in directory\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-gen <value> | --gen=<value>]                                               sentences count for generation text\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-text <value> | --text=<value>]                                             text to be processed as a string\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-threads <value> | --threads=<value>]                                       number of threads for data collection\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-bin <value> | --r-bin=<value>]                                           binary file address LM of \x1B[1m*.alm\x1B[0m for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-bin <value> | --w-bin=<value>]                                           binary file address LM of \x1B[1m*.alm\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-lictype <value> | --bin-lictype=<value>]                               license type for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-lictext <value> | --bin-lictext=<value>]                               license text for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-name <value> | --bin-name=<value>]                                     dictionary name for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-contacts <value> | --bin-contacts=<value>]                             author contact info for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-author <value> | --bin-author=<value>]                                 author of the dictionary for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-copyright <value> | --bin-copyright=<value>]                           copyright of the dictionary owner for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-aes <value> | --bin-aes=<value>]                                       aes encryption Size \x1B[1m(128, 192, 256) bits\x1B[0m for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-bin-password <value> | --bin-password=<value>]                             encryption password \x1B[1m(if required)\x1B[0m, encryption is performed only when setting a password for binary container\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-abbrs <value> | --abbrs=<value>]                                           file address abbreviations of \x1B[1m*.txt\x1B[0m for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-text <value> | --w-text=<value>]                                         file address text of \x1B[1m*.txt\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-map <value> | --w-map=<value>]                                           file address map of \x1B[1m*.map\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-arpa <value> | --w-arpa=<value>]                                         file address arpa of \x1B[1m*.arpa\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-abbr <value> | --w-abbr=<value>]                                         file address abbrs of \x1B[1m*.abbr\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-vocab <value> | --w-vocab=<value>]                                       file address vocab of \x1B[1m*.vocab\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-words <value> | --w-words=<value>]                                       file address words of \x1B[1m*.txt\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-ngram <value> | --w-ngram=<value>]                                       file address ngrams of \x1B[1m*.ngrams\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-oovfile <value> | --w-oovfile=<value>]                                   file address OOVs of \x1B[1m*.txt\x1B[0m for export oov words\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-json <value> | --w-json=<value>]                                         file address json data of \x1B[1m*.json\x1B[0m for export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-json <value> | --r-json=<value>]                                         file address json data of \x1B[1m*.json\x1B[0m for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-text <value> | --r-text=<value>]                                         file address text of \x1B[1m*.txt\x1B[0m or dir path for texts import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-words <value> | --r-words=<value>]                                       file address words of \x1B[1m*.txt\x1B[0m or dir path for words import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-map <value> | --r-map=<value>]                                           file address map of \x1B[1m*.map\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-arpa <value> | --r-arpa=<value>]                                         file address arpa of \x1B[1m*.arpa\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-abbr <value> | --r-abbr=<value>]                                         file address abbrs of \x1B[1m*.abbr\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-vocab <value> | --r-vocab=<value>]                                       file address vocab of \x1B[1m*.vocab\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-ngram <value> | --r-ngram=<value>]                                       file address ngrams of \x1B[1m*.ngrams\x1B[0m or dir path for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-tokens-text <value> | --w-tokens-text=<value>]                           file address tokens of \x1B[1m*.txt\x1B[0m text for tokens export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-tokens-json <value> | --w-tokens-json=<value>]                           file address tokens of \x1B[1m*.json\x1B[0m text for tokens export\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-tokens-text <value> | --r-tokens-text=<value>]                           file address tokens of \x1B[1m*.txt\x1B[0m text for tokens import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-tokens-json <value> | --r-tokens-json=<value>]                           file address tokens of \x1B[1m*.json\x1B[0m text for tokens import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-domain-zones <value> | --domain-zones=<value>]                             file address domain zones of \x1B[1m*.txt\x1B[0m for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-mix-restwords <value> | --mix-restwords=<value>]                           file address restore mixed words of \x1B[1m*.txt\x1B[0m for import\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-mix-arpa[1...N] <value> | --mix-arpa[1...N]=<value>]                       file address LM of \x1B[1m*.arpa\x1B[0m for mixing\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-r-tokens-path <value> | --r-tokens-path=<value>]                           directory path with tokens files for read\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-w-tokens-path <value> | --w-tokens-path=<value>]                           directory path with tokens files for write\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-train-intermed-dest <value> | --train-intermed-dest=<value>]               directory path with raw data files for write\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-mix-bayes-scale <value> | --mix-bayes-scale=<value>]                       log likelihood scale for bayes mixing\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-mix-bayes-length <value> | --mix-bayes-length=<value>]                     context length for Bayes mixture LM\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-mix-lambda[1...N] <value> | --mix-lambda[1...N]=<value>]                   weight lambda first model for mixing\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-vprune-oc <value> | --vprune-oc=<value>]                                   \x1B[1moc threshold\x1B[0m of pruning vocabulary\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-vprune-dc <value> | --vprune-dc=<value>]                                   \x1B[1mdc threshold\x1B[0m of pruning vocabulary\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-vprune-wltf <value> | --vprune-wltf=<value>]                               \x1B[1mwltf threshold\x1B[0m of pruning vocabulary\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-aprune-threshold <value> | --aprune-threshold=<value>]                     frequency threshold of pruning language model\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-aprune-max-gram <value> | --aprune-max-gram=<value>]                       maximum size of n-grams pruning\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-utokens <value> | --utokens=<value>]                                       list of custom attributes\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-modify <value> | --modify=<value>]                                         modification flag for modify method\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-delta <value> | --delta=<value>]                                           delta size for smoothing by addsmooth algorithm\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-discount <value> | --discount=<value>]                                     discount size for smoothing by cdiscount algorithm\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-ngrams <value> | --ngrams=<value>]                                         ngram size for the \x1B[1mcounts method\x1B[0m\r\n"
	"  \x1B[1m-\x1B[0m (bigram | trigram)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-mix <value> | --mix=<value>]                                               type of mixing language models\r\n"
	"  \x1B[1m-\x1B[0m (static | bayes | loglinear)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-debug <value> | --debug=<value>]                                           set debug mode\r\n"
	"  \x1B[1m-\x1B[0m (0 - off | 1 - progress | 2 - console)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-tokens-disable <value1|value2|...> | --tokens-disable=<value1|value2|...>] list of tokens to disable\r\n"
	"  \x1B[1m-\x1B[0m (num | url | abbr | date | time | anum | math | rnum | specl | ...)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-tokens-unknown <value1|value2|...> | --tokens-unknown=<value1|value2|...>] list of tokens for identification as <unk>\r\n"
	"  \x1B[1m-\x1B[0m (num | url | abbr | date | time | anum | math | rnum | specl | ...)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-train-segment-size <value> | --train-segment-size=<value>]                 segment size for training data segmentation\r\n"
	"  \x1B[1m-\x1B[0m (nb | nkb | nMb | nGb), example: --train-segment-size=4096Mb\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-smoothing <value> | --smoothing=<value>]                                   smoothing algorithm for \x1B[1mtrain method\x1B[0m]\r\n"
	"  \x1B[1m-\x1B[0m (goodturing | cdiscount | ndiscount | addsmooth | wittenbell | kneserney | mkneserney)\r\n"
	"\x1B[33m\x1B[1m×\x1B[0m [-method <value> | --method=<value>]                                         method application\r\n"
	"  \x1B[1m-\x1B[0m (ppl | find | train | mix | repair | modify | sweep | vprune | aprune | tokens | counts | fixcase | checktext | sentences | info)\r\n\r\n";
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
	// Если JSON файл указан
	if(((value = env.get("r-json")) != nullptr) && fsys_t::isfile(value)){
		// Данные в формате JSON
		string data = "";
		// Выполняем считывание всех строк текста
		fsys_t::rfile(value, [&data](const string & line, const uintmax_t size) noexcept {
			// Если текст получен
			if(!line.empty()) data.append(line);
		});
		// Если данные получены, устанавливаем их
		if(!data.empty()){
			try {
				// Выполняем парсинг JSON конфиг
				env.setJson(json::parse(data));
			// Если возникает ошибка
			} catch(const exception & e) {
				// Выводим сообщение об ошибке
				print(alphabet.format("the JSON config [%s] is broken\r\n", data.c_str()), env.get("log"));
			}
		}
	}
	// Проверяем существует ли бинарный файл
	if(((value = env.get("r-bin")) != nullptr) && fsys_t::isfile(value)){
		// Запоминаем адрес бинарного файла для чтения
		binDictFile = value;
	}
	// Если алфавит снова не найден
	if(binDictFile.empty() && ((value = env.get("alphabet")) == nullptr)){
		// Выводим сообщение в консоль
		print("alphabet in not set\r\n", env.get("log"));
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
			// Проверяем требуется ли указывать тип сглаживания
			const bool noSmoothing = (
				!binDictFile.empty() ||
				env.is("method", "ppl") ||
				env.is("method", "mix") ||
				env.is("method", "find") ||
				env.is("method", "info") ||
				env.is("method", "tokens") ||
				env.is("method", "counts") ||
				env.is("method", "fixcase") ||
				env.is("method", "checktext") ||
				env.is("method", "sentences")
			);
			// Если бинарный файл не указан
			if(!noSmoothing){
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
						print(alphabet.format("the algorithm name \"%s\" is bad\r\n", env.get("smoothing")), env.get("log"));
				// Сообщаем, что алгоритм сглаживания не указан
				} else print("algorithm smoothing is not set\r\n", env.get("log"));
			}
			// Если основной метод работы получен
			if(env.is("method") && (string(env.get("method")).compare("-yes-") != 0)){
				// Проверяем правильность введённого основного метода работы
				if(!env.is("method", "ppl") &&
				!env.is("method", "mix") &&
				!env.is("method", "find") &&
				!env.is("method", "info") &&
				!env.is("method", "sweep") &&
				!env.is("method", "train") &&
				!env.is("method", "merge") &&
				!env.is("method", "tokens") &&
				!env.is("method", "repair") &&
				!env.is("method", "modify") &&
				!env.is("method", "aprune") &&
				!env.is("method", "vprune") &&
				!env.is("method", "counts") &&
				!env.is("method", "fixcase") &&
				!env.is("method", "checktext") &&
				!env.is("method", "sentences"))
					// Выводим сообщение в консоль
					print(alphabet.format("the method name \"%s\" is bad\r\n", env.get("method")), env.get("log"));
			// Сообщаем, что метод не указан
			} else print("toolkit method is not set\r\n", env.get("log"));
			// Если ни один файл для сохранения не передан, выходим
			if(!env.is("w-map") && !env.is("w-arpa") && !env.is("w-vocab") && !env.is("w-words") && !env.is("w-ngram") &&
			!env.is("w-bin") && !env.is("method", "merge") && !env.is("method", "ppl") && !env.is("method", "find") &&
			!env.is("method", "info") && !env.is("method", "counts") && !env.is("method", "tokens") &&
			!env.is("method", "sentences") && !env.is("method", "fixcase") && !env.is("method", "checktext")){
				// Выводим сообщение и выходим из приложения
				print("file address to save is not specified\r\n", env.get("log"));
			}
			/** Начало работы основных методов **/
			// Создаём токенизатор
			tokenizer_t tokenizer(&alphabet);
			// Устанавливаем адрес файла для логирования
			tokenizer.setLogfile(env.get("log"));
			// Замеряем время начала работы
			auto timeShifting = chrono::system_clock::now();
			// Если апостроф разрешён в токенизаторе, активируем его
			if(env.is("allow-apostrophe")) alphabet.switchAllowApostrophe();
			// Устанавливаем режим отладки, если он передан
			if(debug == 2) tokenizer.setOption(tokenizer_t::options_t::debug);
			// Активируем разрешение проставлять регистры в словах
			if(env.is("allow-uppers")) tokenizer.setOption(tokenizer_t::options_t::uppers);
			// Если разрешено использовать символы ударения, активируем
			if(env.is("allow-stress")) tokenizer.setOption(tokenizer_t::options_t::stress);
			// Если файл с буквами для восстановления слов, передан
			if(((value = env.get("mix-restwords")) != nullptr) && fsys_t::isfile(value)){
				// Идентификатор документа
				size_t size = 0;
				// Статус и процентное соотношение
				u_short status = 0, rate = 100;
				// Запоминаем адрес файла
				const string & filename = realpath(value, nullptr);
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
						status = u_short(size / double(fileSize) * 100.0);
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
				// Отображаем ход процесса
				switch(debug){
					case 1: pss.update(100); break;
					case 2: pss.status(100); break;
				}
			}
			// Если загрузка списка аббревиатур или доменных зон
			if(env.is("abbrs") || env.is("domain-zones")){
				/**
				 * loadFn Функция загрузки списка доменных зон или аббревиатур
				 * @param filename адрес файла для загрузки
				 * @param type     тип файла для загрузки
				 */
				auto loadFn = [&](const string & filename, const u_short type){
					// Если файл передан
					if(!filename.empty()){
						// Идентификатор документа
						size_t size = 0;
						// Статус и процентное соотношение
						u_short status = 0, rate = 100;
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
									case 2: tokenizer.addAbbr(text); break;
								}
							}
							// Если отладка включена
							if(debug > 0){
								// Общий полученный размер данных
								size += text.size();
								// Подсчитываем статус выполнения
								status = u_short(size / double(fileSize) * 100.0);
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
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					}
				};
				// Устанавливаем режим считывания файла аббревиатур
				if(((value = env.get("abbrs")) != nullptr) && fsys_t::isfile(value)) loadFn(realpath(value, nullptr), 2);
				// Устанавливаем режим считывания файла доменных зон
				if(((value = env.get("domain-zones")) != nullptr) && fsys_t::isfile(value)) loadFn(realpath(value, nullptr), 1);
			}
			// Если это работа с уже собранной языковой моделью
			if(env.is("method", "sentences") || env.is("method", "ppl") || env.is("method", "find") ||
			env.is("method", "counts") || env.is("method", "fixcase") || env.is("method", "checktext")){
				// Создаём обхъект языковой модели
				unique_ptr <alm_t> alm(env.is("alm2") ? new alm2_t(&alphabet, &tokenizer) : new alm1_t(&alphabet, &tokenizer));
				// Устанавливаем адрес файла для логирования
				alm->setLogfile(env.get("log"));
				// Если количество ядер передано
				if(((value = env.get("threads")) != nullptr) && alphabet.isNumber(value)){
					// Устанавливаем количество потоков
					alm->setThreads(stoi(value));
				// Иначе устанавливаем 1 поток
				} else alm->setThreads(1);
				// Устанавливаем режим отладки
				if(debug == 2) alm->setOption(alm_t::options_t::debug);
				// Устанавливаем адрес файла oov слов
				if(env.is("method", "ppl")) alm->setOOvFile(env.get("w-oovfile"));
				// Разрешаем детектировать слова состоящее из смешанных словарей
				if(env.is("mixed-dicts")) alm->setOption(alm_t::options_t::mixdicts);
				// Разрешаем выполнять загрузку содержимого arpa, в том виде, в каком она есть. Без перетокенизации содержимого.
				if(env.is("confidence")) alm->setOption(alm_t::options_t::confidence);
				// Если нужно установить все токены для идентифицирования как <unk>
				if(env.is("tokens-all-unknown")) alm->setAllTokenUnknown();
				// Если нужно установить все токены как не идентифицируемые
				if(env.is("tokens-all-disable")) alm->setAllTokenDisable();
				// Если неизвестное слово получено
				if((value = env.get("unknown-word")) != nullptr) alm->setUnknown(value);
				// Если адрес скрипта получен
				if((value = env.get("word-script")) != nullptr) alm->setWordScript(value);
				// Если нужно установить список токенов которые нужно идентифицировать как <unk>
				if((value = env.get("tokens-unknown")) != nullptr) alm->setTokenUnknown(value);
				// Если нужно установить список не идентифицируемых токенов
				if((value = env.get("tokens-disable")) != nullptr) alm->setTokenDisable(value);
				// Если адрес файла чёрного списка получен
				if((value = env.get("badwords")) != nullptr){
					// Чёрный список слов
					vector <string> badwords;
					// Выполняем считывание всех слов для чёрного списка
					fsys_t::rfile(value, [&badwords](const string & line, const uintmax_t fileSize) noexcept {
						// Если текст получен
						if(!line.empty()) badwords.push_back(line);
					});
					// Если чёрный список получен, устанавливаем его
					if(!badwords.empty()) alm->setBadwords(badwords);
				}
				// Если адрес файла белого списка получен
				if((value = env.get("goodwords")) != nullptr){
					// Белый список слов
					vector <string> goodwords;
					// Выполняем считывание всех слов для белого списка
					fsys_t::rfile(value, [&goodwords](const string & line, const uintmax_t fileSize) noexcept {
						// Если текст получен
						if(!line.empty()) goodwords.push_back(line);
					});
					// Если белый список получен, устанавливаем его
					if(!goodwords.empty()) alm->setGoodwords(goodwords);
				}
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
							alm->setUserTokenScript(value);
							// Переходим по всему списку токенов
							for(auto & item : tokens) alm->setUserToken(alphabet.convert(item));
						}
					}
				}
				// Активируем питоновские скрипты
				alm->initPython();
				// Если нужно использовать бинарный контейнер
				if(!binDictFile.empty()){
					// Создаём бинарный контейнер
					ablm_t ablm(binDictFile, alm.get(), &alphabet, &tokenizer, env.get("log"));
					// Если размер шифрования передан
					if((value = env.get("bin-aes")) != nullptr){
						// Если размер шифрования получен
						switch(stoi(value)){
							// Если это 128-и битное шифрование
							case 128: ablm.setAES(aspl_t::types_t::aes128); break;
							// Если это 192-х битное шифрование
							case 192: ablm.setAES(aspl_t::types_t::aes192); break;
							// Если это 256-и битное шифрование
							case 256: ablm.setAES(aspl_t::types_t::aes256); break;
						}
					}
					// Если название словаря передано
					if((value = env.get("bin-name")) != nullptr) ablm.setName(value);
					// Если автор словаря передан
					if((value = env.get("bin-author")) != nullptr) ablm.setAuthor(value);
					// Если тип лицензии словаря передан
					if((value = env.get("bin-lictype")) != nullptr) ablm.setLictype(value);
					// Если текст лицензии словаря передан
					if((value = env.get("bin-lictext")) != nullptr) ablm.setLictext(value);
					// Если контакты автора словаря переданы
					if((value = env.get("bin-contacts")) != nullptr) ablm.setContacts(value);
					// Если пароль словаря передан
					if((value = env.get("bin-password")) != nullptr) ablm.setPassword(value);
					// Если копирайт словаря передан
					if((value = env.get("bin-copyright")) != nullptr) ablm.setCopyright(value);
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
					ablm.readAlm([debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
				// Если требуется загрузить arpa
				} else if(((value = env.get("r-arpa")) != nullptr) && fsys_t::isfile(value)){
					// Запоминаем адрес файла
					const string & filename = realpath(value, nullptr);
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
					// Выполняем чтение arpa
					alm->read(filename, [debug, &pss](const u_short status) noexcept {
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
				// Если arpa файл не указан
				} else print("language model file address is empty\r\n", env.get("log"));
				// Если требуется загрузить файл словаря abbr
				if((value = env.get("r-abbr")) != nullptr){
					// Запоминаем адрес файла
					const string & filename = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(filename);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read abbr", "Read abbr is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку файла суффиксов цифровых аббревиатур
					tokenizer.readSuffix(filename, [debug, &pss](const string & filename, const u_short status) noexcept {
						// Если отладка включена, выводим имя файла
						if(debug > 0) pss.description(filename);
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
				// Если это генерация предложений
				if(env.is("method", "sentences")){
					// Получаем количество предложений для генерации
					u_short counts = ((value = env.get("gen")) != nullptr ? stoi(value) : 65000);
					// Запоминаем файл для записи данных
					const string writefile = ((value = env.get("w-text")) != nullptr ? value : "");
					// Если количество предложений слишком большое, исправляем это
					if(counts > 65000) counts = 65000;
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем заголовки прогресс-бара
						pss.title("Sentences generation", "Sentences generation is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем генерацию предложений с записью их в файл
					alm->sentencesToFile(counts, writefile, [debug, &pss](const u_short status){
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
				// Если это расчёт перплексии
				} else if(env.is("method", "ppl")){
					// Если текст передан
					if((value = env.get("text")) != nullptr){
						// Выполняем расчёт перплексии
						auto ppl = alm->perplexity(value);
						// Если отладка отключена
						if(debug < 2){
							// Выводим текст перплексии
							// printf("Text: %s\r\n", value);
							// Выводим сообщение отладки - количество слов
							alphabet.log("%zu sentences, %zu words, %zu OOVs", alphabet_t::log_t::null, nullptr, ppl.sentences, ppl.words, ppl.oovs);
							// Выводим сообщение отладки - результатов расчёта
							alphabet.log("%zu zeroprobs, logprob= %4.8f ppl= %4.8f ppl1= %4.8f\r\n", alphabet_t::log_t::null, nullptr, ppl.zeroprobs, ppl.logprob, ppl.ppl, ppl.ppl1);
						}
					// Если адрес текстового файла или каталог передан
					} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value))){
						// Запоминаем адрес файла
						const string & path = realpath(value, nullptr);
						// Расширение файлов текстового корпуса
						const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(path);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Calculation perplexity", "Calculation perplexity is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем расчёт перплексии
						auto ppl = alm->pplByFiles(path, [debug, &pss](const u_short status){
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						}, ext);
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
						// Если отладка отключена
						if(debug < 2){
							// Выводим сообщение отладки - количество слов
							alphabet.log("%zu sentences, %zu words, %zu OOVs", alphabet_t::log_t::null, nullptr, ppl.sentences, ppl.words, ppl.oovs);
							// Выводим сообщение отладки - результатов расчёта
							alphabet.log("%zu zeroprobs, logprob= %4.8f ppl= %4.8f ppl1= %4.8f\r\n", alphabet_t::log_t::null, nullptr, ppl.zeroprobs, ppl.logprob, ppl.ppl, ppl.ppl1);
						}
					// Сообщаем, что текст не указан
					} else print("text is empty\r\n", env.get("log"));
				// Если это метод поиска n-граммы в тексте
				} else if(env.is("method", "find")) {
					// Если текст передан
					if((value = env.get("text")) != nullptr){
						// Выполняем поиск n-грамм
						alm->find(value, [&alphabet](const string & text){
							// Выводим список найденных n-рамм
							if(!text.empty()) alphabet.log("%s", alphabet_t::log_t::null, nullptr, text.c_str());
						});
					// Если адрес текстового файла передан
					} else if(((value = env.get("r-text")) != nullptr) && env.is("w-text") && fsys_t::isfile(value)){
						// Запоминаем файл для записи данных
						const string writefile = env.get("w-text");
						// Запоминаем каталога с файлами
						const string & path = realpath(value, nullptr);
						// Расширение файлов текстового корпуса
						const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(path);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Find ngrams", "Find ngrams is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем поиск n-грамм в текстовом файле
						alm->findByFiles(path, writefile, [debug, &pss, &writefile](const string & filename, const u_short status){
							// Устанавливаем название файла
							if(debug > 0) pss.description(filename + string(" -> ") + writefile);
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						}, ext);
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					// Сообщаем, что текст не указан
					} else print("text is empty\r\n", env.get("log"));
				// Если это метод проверки текста
				} else if(env.is("method", "checktext")){
					// Если текст передан
					if((value = env.get("text")) != nullptr){
						// Получаем данные текста
						const string text = value;
						// Если нужно выполнить проверку текста с точным соответствием
						if(env.is("accurate")){
							// Выполняем првоерку текста
							auto res = alm->check(text, true);
							// Выводим результат
							alphabet.log("%s | %s\r\n", alphabet_t::log_t::null, nullptr, (res.first ? "YES" : "NO"), text.c_str());
						// Если нужно выполнить проверку текста с указанным шагом N-граммы
						} else {
							// Получаем шаг N-граммы
							const u_short step = ((value = env.get("step")) != nullptr ? ((stoi(value) < 2) ? 2 : stoi(value)) : 2);
							// Выводим результат
							alphabet.log("%s | %s\r\n", alphabet_t::log_t::null, nullptr, (alm->exist(text, step).first ? "YES" : "NO"), text.c_str());
						}
					// Если адрес текстового файла или каталог передан
					} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value)) && env.is("w-text")){
						// Запоминаем файл для записи данных
						const string writefile = env.get("w-text");
						// Запоминаем каталога с файлами
						const string & path = realpath(value, nullptr);
						// Расширение файлов текстового корпуса
						const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(path);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read text file", "Read text file is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Если нужно выполнить проверку текста с точным соответствием
						if(env.is("accurate")){
							// Выполняем обработку текстовых данных
							alm->checkByFiles(path, writefile, true, [debug, &pss, &writefile](const string & filename, const u_short status){
								// Устанавливаем название файла
								if(debug > 0) pss.description(filename + string(" -> ") + writefile);
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							}, ext);
						// Если нужно выполнить проверку текста с указанным шагом N-граммы
						} else {
							// Получаем шаг N-граммы
							const u_short step = ((value = env.get("step")) != nullptr ? ((stoi(value) < 2) ? 2 : stoi(value)) : 2);
							// Выполняем обработку текстовых данных
							alm->checkByFiles(path, writefile, step, [debug, &pss, &writefile](const string & filename, const u_short status){
								// Устанавливаем название файла
								if(debug > 0) pss.description(filename + string(" -> ") + writefile);
								// Отображаем ход процесса
								switch(debug){
									case 1: pss.update(status); break;
									case 2: pss.status(status); break;
								}
							}, ext);
						}
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					// Сообщаем, что текст не указан
					} else print("text is empty\r\n", env.get("log"));
				// Если это метод исправления регистров слов
				} else if(env.is("method", "fixcase")){
					// Если текст передан
					if((value = env.get("text")) != nullptr){
						// Выводим результат
						alphabet.log("%s\r\n", alphabet_t::log_t::null, nullptr, alm->fixUppers(value).c_str());
					// Если адрес текстового файла или каталог передан
					} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value)) && env.is("w-text")){
						// Запоминаем файл для записи данных
						const string writefile = env.get("w-text");
						// Запоминаем каталога с файлами
						const string & path = realpath(value, nullptr);
						// Расширение файлов текстового корпуса
						const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(path);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read text file", "Read text file is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем исправление регистра в текстовом файле
						alm->fixUppersByFiles(path, writefile, [debug, &pss, &writefile](const string & filename, const u_short status){
							// Устанавливаем название файла
							if(debug > 0) pss.description(filename + string(" -> ") + writefile);
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						}, ext);
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					// Сообщаем, что текст не указан
					} else print("text is empty\r\n", env.get("log"));
				// Если это метод определения количества n-грамм в тексте
				} else if(env.is("method", "counts")){
					// Определяем какое количество n-грамм нужно проверить
					const u_short ngrams = (env.is("ngrams", "trigram") ? 3 : (env.is("ngrams", "bigram") ? 2 : 1));
					// Если текст передан
					if((value = env.get("text")) != nullptr){
						// Определяем тип размеров n-грамм
						switch(ngrams){
							// Если размер n-грамм не определен
							case 1: alphabet.log("%zu [%hugram] | %s\r\n", alphabet_t::log_t::null, nullptr, alm->grams(value), alm->getSize(), value); break;
							// Если размер n-грамм биграммы
							case 2: alphabet.log("%zu [%hugram] | %s\r\n", alphabet_t::log_t::null, nullptr, alm->bigrams(value), ngrams, value); break;
							// Если размер n-грамм триграммы
							case 3: alphabet.log("%zu [%hugram] | %s\r\n", alphabet_t::log_t::null, nullptr, alm->trigrams(value), ngrams, value); break;
						}
					// Если адрес текстового файла или каталог передан
					} else if(((value = env.get("r-text")) != nullptr) && (fsys_t::isfile(value) || fsys_t::isdir(value)) && env.is("w-text")){
						// Запоминаем файл для записи данных
						const string writefile = env.get("w-text");
						// Запоминаем каталога с файлами
						const string & path = realpath(value, nullptr);
						// Расширение файлов текстового корпуса
						const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(path);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read text file", "Read text file is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Выполняем подсчёт количества n-грамм
						alm->countsByFiles(path, writefile, ngrams, [debug, &pss, &writefile](const string & filename, const u_short status){
							// Устанавливаем название файла
							if(debug > 0) pss.description(filename + string(" -> ") + writefile);
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						}, ext);
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					// Сообщаем, что текст не указан
					} else print("text is empty\r\n", env.get("log"));
				}
				// Если режим отладки включён
				if(debug > 0){
					// Получаем диапазон времени
					auto dimension = pss.dimension(chrono::duration_cast <chrono::seconds> (chrono::system_clock::now() - timeShifting).count());
					// Выводим результат
					print(alphabet.format("work time shifting: %lld %s\r\n", dimension.first, dimension.second.c_str()), env.get("log"), alphabet_t::log_t::info);
				// Выходим из приложения
				} else exit(0);
			}
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
			// Разрешаем выполнять загрузку содержимого arpa, в том виде, в каком она есть. Без перетокенизации содержимого.
			if(env.is("confidence")) toolkit.setOption(toolkit_t::options_t::confidence);
			// Разрешаем выполнять интерполяцию при расчёте arpa
			if(env.is("interpolate")) toolkit.setOption(toolkit_t::options_t::interpolate);
			// Флаг учитывающий при сборке N-грамм, только те токены, которые соответствуют словам
			if(env.is("only-token-words")) toolkit.setOption(toolkit_t::options_t::tokenWords);
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
				fsys_t::rfile(value, [&badwords](const string & line, const uintmax_t fileSize) noexcept {
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
				fsys_t::rfile(value, [&goodwords](const string & line, const uintmax_t fileSize) noexcept {
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
				// Если размер шифрования передан
				if((value = env.get("bin-aes")) != nullptr){
					// Если размер шифрования получен
					switch(stoi(value)){
						// Если это 128-и битное шифрование
						case 128: ablm.setAES(aspl_t::types_t::aes128); break;
						// Если это 192-х битное шифрование
						case 192: ablm.setAES(aspl_t::types_t::aes192); break;
						// Если это 256-и битное шифрование
						case 256: ablm.setAES(aspl_t::types_t::aes256); break;
					}
				}
				// Если название словаря передано
				if((value = env.get("bin-name")) != nullptr) ablm.setName(value);
				// Если автор словаря передан
				if((value = env.get("bin-author")) != nullptr) ablm.setAuthor(value);
				// Если тип лицензии словаря передан
				if((value = env.get("bin-lictype")) != nullptr) ablm.setLictype(value);
				// Если текст лицензии словаря передан
				if((value = env.get("bin-lictext")) != nullptr) ablm.setLictext(value);
				// Если контакты автора словаря переданы
				if((value = env.get("bin-contacts")) != nullptr) ablm.setContacts(value);
				// Если пароль словаря передан
				if((value = env.get("bin-password")) != nullptr) ablm.setPassword(value);
				// Если копирайт словаря передан
				if((value = env.get("bin-copyright")) != nullptr) ablm.setCopyright(value);
				// Устанавливаем флаг отладки
				if(debug == 1) ablm.setFlag(ablm_t::flag_t::debug);
				// Выполняем инициализацию словаря
				ablm.init();
				// Если это запрос информации о словаре
				if(env.is("method", "info")){
					// Выводим информацию о словаре
					ablm.info();
					// Если режим отладки включён
					if(debug > 0){
						// Получаем диапазон времени
						auto dimension = pss.dimension(chrono::duration_cast <chrono::seconds> (chrono::system_clock::now() - timeShifting).count());
						// Выводим результат
						print(alphabet.format("work time shifting: %lld %s\r\n", dimension.first, dimension.second.c_str()), env.get("log"), alphabet_t::log_t::info);
					// Выходим из приложения
					} else exit(0);
				// Если - это обычная загрузка из бинарного контейнера
				} else {
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
					ablm.readToolkit([debug, &pss](const u_short status) noexcept {
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(status); break;
							case 2: pss.status(status); break;
						}
					});
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
					const string & readfile = realpath(value, nullptr);
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
								status = u_short(size / double(fileSize) * 100.0);
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
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
						// Сообщаем, что контекст пустой
						} else print("context is empty\r\n", env.get("log"));
					// Выходим из приложения и выводим сообщение
					} else print("file to write result is not found\r\n", env.get("log"));
				// Если файл json с токенами получен
				} else if(((value = env.get("r-tokens-json")) != nullptr) && fsys_t::isfile(value)){
					// Получаем адрес json файла для чтения
					const string & readfile = realpath(value, nullptr);
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
								status = u_short(size / double(fileSize) * 100.0);
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
							try {
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
									// Сообщаем, что контекст в json файле пустой
									} else print("context in json file is empty\r\n", env.get("log"));
								// Сообщаем, что json файл испорчен
								} else print("broken json file\r\n", env.get("log"));
							// Если возникает ошибка
							} catch(const exception & e) {
								// Выводим сообщение об ошибке
								print(alphabet.format("the JSON data [%s] is broken\r\n", textData.c_str()), env.get("log"));
							}
						// Сообщаем, что контекст пустой
						} else print("context is empty\r\n", env.get("log"));
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					// Выходим из приложения и выводим сообщение
					} else print("file to write result is not found\r\n", env.get("log"));
				// Если каталог с текстовымы файлами корпуса получен
				} else if(((value = env.get("r-tokens-path")) != nullptr) && fsys_t::isdir(value)) {
					// Получаем адрес каталога с текстовыми файлами для чтения
					const string & readpath = realpath(value, nullptr);
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
							fsys_t::rfile2(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
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
									status = u_short(size / double(dirSize) * 100.0);
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
									try {
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
											// Сообщаем, что контекст в json файле пустой
											} else print("context in json file is empty\r\n", env.get("log"));
										// Сообщаем, что json файл испорчен
										} else print("broken json file\r\n", env.get("log"));
									// Если возникает ошибка
									} catch(const exception & e) {
										// Выводим сообщение об ошибке
										print(alphabet.format("the JSON data [%s] is broken\r\n", textData.c_str()), env.get("log"));
									}
								// Сообщаем, что контекст пустой
								} else print("context is empty\r\n", env.get("log"));
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
							// Сообщаем, что контекст пустой
							} else print("context is empty\r\n", env.get("log"));
						});
						// Отображаем ход процесса
						switch(debug){
							case 1: pss.update(100); break;
							case 2: pss.status(100); break;
						}
					// Выходим из приложения и выводим сообщение
					} else print("path to write result is not found\r\n", env.get("log"));
				// Если текст передан
				} else if((value = env.get("text")) != nullptr) {
					// Флаг входящего типа данных
					bool isJSON = false;
					// Получаем значение текста
					const string text = value;
					// Проверяем является ли текст json-ом
					if((text.front() == '[') && (text.back() == ']')){
						// Запоминаем, что это json
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
							if(!text.empty()) textData = alphabet.format("%s\r\n", text.c_str());
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
					// Сообщаем, что контекст пустой
					} else print("context is empty\r\n", env.get("log"));
				// Выходим из приложения и выводим сообщение
				} else print("text corpus for tokenization is not found\r\n", env.get("log"));
				// Если режим отладки включён
				if(debug > 0){
					// Получаем диапазон времени
					auto dimension = pss.dimension(chrono::duration_cast <chrono::seconds> (chrono::system_clock::now() - timeShifting).count());
					// Выводим результат
					print(alphabet.format("work time shifting: %lld %s\r\n", dimension.first, dimension.second.c_str()), env.get("log"), alphabet_t::log_t::info);
				// Выходим из приложения
				} else exit(0);
			// Иначе выполняем инициализацию алгоритма сглаживания
			} else if(env.is("smoothing")) {
				// Дополнительный коэффициент алгоритма сглаживания
				double mod = 0.0;
				// Если алгоритм сглаживания ConstDiscount или AddSmooth, запрашиваем дополнительные параметры
				if(env.is("smoothing", "cdiscount") || env.is("smoothing", "addsmooth")){
					// Считываем флаг дополнительной модификации
					value = (env.is("smoothing", "cdiscount") ? env.get("discount") : env.get("delta"));
					// Если значение получено
					if(value != nullptr) mod = stod(value);
				}
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
				// Сообщаем, что сглаживание выбрано не верно
				else print("smoothing is bad\r\n", env.get("log"));
			// Сообщаем, что сглаживание выбрано не верно
			} else if(!env.is("method", "mix")) print("smoothing is bad\r\n", env.get("log"));
			// Если передан метод обучения
			if(env.is("method", "train") && !env.is("r-map") && !env.is("r-vocab")){
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
					if(((value = env.get("corpus")) != nullptr) && fsys_t::isdir(value)){
						// Запоминаем путь к файлам
						const string & path = realpath(value, nullptr);
						// Расширение файлов текстового корпуса
						const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
						// Если количество ядер передано
						if(((value = env.get("threads")) != nullptr) && alphabet.isNumber(value)){
							// Объявляем объект коллектора
							collector_t collector(&toolkit, &alphabet, &tokenizer, env.get("log"));
							// Устанавливаем режим отладки
							collector.setDebug(debug);
							// Устанавливаем размер n-граммы
							collector.setOrder(order);
							// Устанавливаем количество потоков
							collector.setThreads(stoi(value));
							// Устанавливаем флаг автоочистки
							collector.setIntermed(env.is("train-intermed"));
							// Устанавливаем флаг сегментации
							collector.setSegment(
								env.is("train-segments"),
								((value = env.get("train-segment-size")) != nullptr ? value : "auto")
							);
							// Устанавливаем путь назначения
							if((value = env.get("train-intermed-dest")) != nullptr) collector.setDest(value);
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
							fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
								// Устанавливаем название файла
								if(debug > 0) pss.description(filename);
								// Выполняем считывание всех строк текста
								fsys_t::rfile2(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
									// Если текст получен
									if(!text.empty()) toolkit.addText(text, idd);
									// Если отладка включена
									if(debug > 0){
										// Общий полученный размер данных
										size += text.size();
										// Подсчитываем статус выполнения
										status = u_short(size / double(dirSize) * 100.0);
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
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
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
						const string & filename = realpath(value, nullptr);
						// Если количество ядер передано
						if(((value = env.get("threads")) != nullptr) && alphabet.isNumber(value)){
							// Объявляем объект коллектора
							collector_t collector(&toolkit, &alphabet, &tokenizer, env.get("log"));
							// Устанавливаем режим отладки
							collector.setDebug(debug);
							// Устанавливаем размер n-граммы
							collector.setOrder(order);
							// Устанавливаем количество потоков
							collector.setThreads(stoi(value));
							// Устанавливаем флаг автоочистки
							collector.setIntermed(env.is("train-intermed"));
							// Устанавливаем флаг сегментации
							collector.setSegment(
								env.is("train-segments"),
								((value = env.get("train-segment-size")) != nullptr ? value : "auto")
							);
							// Устанавливаем путь назначения
							if((value = env.get("train-intermed-dest")) != nullptr) collector.setDest(value);
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
							fsys_t::rfile(filename, [&](const string & text, const uintmax_t fileSize) noexcept {
								// Если текст получен
								if(!text.empty()) toolkit.addText(text, 0);
								// Если отладка включена
								if(debug > 0){
									// Общий полученный размер данных
									size += text.size();
									// Подсчитываем статус выполнения
									status = u_short(size / double(fileSize) * 100.0);
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
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(100); break;
								case 2: pss.status(100); break;
							}
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
					} else print("path or file with corpus texts is not specified\r\n", env.get("log"));
				}
			// Если передан метод интерполяции
			} else if(env.is("method", "mix") && ((value = env.get("r-arpa")) != nullptr)) {
				// Веса моделей для интерполяции
				vector <double> lambdas;
				// Адреса файлов языковых моделей
				vector <string> filenames = {value};
				// Если отладка включена, выводим индикатор загрузки
				if(debug > 0){
					// Очищаем предыдущий прогресс-бар
					pss.clear();
					// Устанавливаем заголовки прогресс-бара
					pss.title("Mix arpa files", "Mix arpa files is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Если это методы интерполяции - Баес или Логарифмическо-линейный
				if(env.is("mix", "bayes") || env.is("mix", "loglinear")){
					// Загружаем 65000 языковых моделей
					for(u_short i = 1; i < 65000; i++){
						// Если языковая модель получена
						value = env.get(string("mix-arpa") + to_string(i));
						// Если адрес файла получен
						if(value != nullptr)
							// Получаем адрес файла arpa
							filenames.push_back(realpath(value, nullptr));
						// Иначе выходим из цикла
						else break;
					}
					// Выполняем сбор весов для интерполяции
					for(u_short i = 0; i < filenames.size(); i++){
						// Получаем данные лямбды для интерполяции
						value = env.get(string("mix-lambda") + to_string(i + 1));
						// Если лямбда передана
						if(value != nullptr)
							// Добавляем вес языковой модели для интерполяции
							lambdas.push_back(stod(value));
						// Если вес не передан, устанавливаем по умолчанию
						else lambdas.push_back(0.5);
					}
				}
				// Если это алгоритм интерполяции Баес
				if(env.is("mix", "bayes")){
					// Если адреса файлов языковых моделей получены
					if(filenames.size() > 1){
						// Логарифмическая шкала вероятности для алгоритма Баеса
						const double scale = ((value = env.get("mix-bayes-scale")) != nullptr ? stod(value) : 0.0);
						// Байесовская интерполяция с учетом контекста длины
						const size_t length = ((value = env.get("mix-bayes-length")) != nullptr ? stoi(value) : 0);
						// Выполняем интерполяцию
						toolkit.mix(filenames, lambdas, length, scale, [&pss, debug](const string & filename, const u_short status){
							// Устанавливаем название файла
							pss.description(filename);
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
					// Если файлы arpa указаны не верно
					} else print("the arpa files transferred incorrectly specified\r\n", env.get("log"));
				// Если это алгоритм интерполяции Логарифмическо-линейная
				} else if(env.is("mix", "loglinear")) {
					// Если адреса файлов языковых моделей получены
					if(filenames.size() > 1){
						// Выполняем интерполяцию
						toolkit.mix(filenames, lambdas, 0, 0.0, [&pss, debug](const string & filename, const u_short status){
							// Устанавливаем название файла
							pss.description(filename);
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
					// Если файлы arpa указаны не верно
					} else print("the arpa files transferred incorrectly specified\r\n", env.get("log"));
				// Если это интерполяция статическая
				} else if(env.is("mix", "static")) {
					// Загружаем 65000 языковых моделей
					for(u_short i = 1; i < 65000; i++){
						// Если языковая модель получена
						value = env.get(string("mix-arpa") + to_string(i));
						// Если адрес файла получен
						if(value != nullptr){
							// Получаем адрес файла arpa
							filenames.push_back(realpath(value, nullptr));
							// Получаем данные лямбды для интерполяции
							value = env.get(string("mix-lambda") + to_string(i));
							// Если лямбда передана
							if(value != nullptr)
								// Добавляем вес языковой модели для интерполяции
								lambdas.push_back(stod(value));
							// Если вес не передан, устанавливаем по умолчанию
							else lambdas.push_back(0.5);
						// Иначе выходим из цикла
						} else break;
					}
					// Если адреса файлов языковых моделей получены
					if(filenames.size() > 1){
						// Выполняем интерполяцию
						toolkit.mix(filenames, lambdas, env.is("mix-backward"), [&pss, debug](const string & filename, const u_short status){
							// Устанавливаем название файла
							pss.description(filename);
							// Отображаем ход процесса
							switch(debug){
								case 1: pss.update(status); break;
								case 2: pss.status(status); break;
							}
						});
					// Если файлы arpa указаны не верно
					} else print("the arpa files transferred incorrectly specified\r\n", env.get("log"));
				// Выводим сообщение об ошибке
				} else print("the mixing method is not defined\r\n", env.get("log"));
				// Отображаем ход процесса
				switch(debug){
					case 1: pss.update(100); break;
					case 2: pss.status(100); break;
				}
			// Проверяем правильно ли указаны адреса файлов
			} else if((env.is("r-map") && (env.is("r-vocab") || env.is("r-words"))) || env.is("r-ngram") || env.is("r-arpa") || !binDictFile.empty()) {
				// Если требуется загрузить файл n-грамм
				if((value = env.get("r-ngram")) != nullptr){
					// Запоминаем адрес файла
					const string & filename = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(filename);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read n-gram", "Read n-gram is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку файла n-грамм
					toolkit.readNgram(filename, [debug, &pss](const string & filename, const u_short status) noexcept {
						// Если отладка включена, устанавливаем название файла
						if(debug > 0) pss.description(filename);
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
				if(!env.is("method", "merge") && ((value = env.get("r-arpa")) != nullptr) && fsys_t::isfile(value)){
					// Запоминаем адрес файла
					const string & filename = realpath(value, nullptr);
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
				}
				// Если требуется загрузить файл словаря vocab
				if((value = env.get("r-vocab")) != nullptr){
					// Запоминаем адрес файла
					const string & filename = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(filename);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read vocab", "Read vocab is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку файла словаря vocab
					toolkit.readVocab(filename, [debug, &pss](const string & filename, const u_short status) noexcept {
						// Если отладка включена, выводим имя файла
						if(debug > 0) pss.description(filename);
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
				// Если требуется загрузить файл списка слов
				if(!env.is("method", "merge") && ((value = env.get("r-words")) != nullptr) && fsys_t::isfile(value)){
					// Параметры индикаторы процесса
					size_t size = 0, status = 0, rate = 0;
					// Запоминаем адрес файла
					const string & filename = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(filename);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read words file", "Read words file is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку файла словаря списка слов
					fsys_t::rfile(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
						// Если слово получено
						if(!word.empty()){
							// Добавляем слово в словарь
							toolkit.addWord(alphabet.convert(word));
							// Если отладка включена, выводим индикатор загрузки
							if(debug > 0){
								// Общий полученный размер данных
								size += word.size();
								// Подсчитываем статус выполнения
								status = u_short(size / double(fileSize) * 100.0);
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
						}
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				// Если требуется загрузить каталог списка слов
				} else if(((value = env.get("r-words")) != nullptr) && fsys_t::isdir(value)) {
					// Параметры индикаторы процесса
					size_t size = 0, status = 0, rate = 0, index = 0;
					// Запоминаем каталог для загрузки
					const string & path = realpath(value, nullptr);
					// Расширение файлов текстового корпуса
					const string ext = ((value = env.get("ext")) != nullptr ? value : "txt");
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read words file", "Read words file is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Переходим по всему списку словарей в каталоге
					fsys_t::rdir(path, ext, [&](const string & filename, const uintmax_t dirSize) noexcept {
						// Если отладка включена, выводим название файла
						if(debug > 0) pss.description(filename);
						// Выполняем загрузку файла словаря списка слов
						fsys_t::rfile2(filename, [&](const string & word, const uintmax_t fileSize) noexcept {
							// Если слово получено
							if(!word.empty()){
								// Добавляем слово в словарь
								toolkit.addWord(alphabet.convert(word), 0, index);
								// Если отладка включена, выводим индикатор загрузки
								if(debug > 0){
									// Общий полученный размер данных
									size += word.size();
									// Подсчитываем статус выполнения
									status = u_short(size / double(dirSize) * 100.0);
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
							}
						});
						// Увеличиваем количество обработанных документов
						index++;
					});
					// Отображаем ход процесса
					switch(debug){
						case 1: pss.update(100); break;
						case 2: pss.status(100); break;
					}
				}
				// Если требуется загрузить файл словаря abbr
				if((value = env.get("r-abbr")) != nullptr){
					// Запоминаем адрес файла
					const string & filename = realpath(value, nullptr);
					// Если отладка включена, выводим индикатор загрузки
					if(debug > 0){
						// Очищаем предыдущий прогресс-бар
						pss.clear();
						// Устанавливаем название файла
						pss.description(filename);
						// Устанавливаем заголовки прогресс-бара
						pss.title("Read abbr", "Read abbr is done");
						// Выводим индикатор прогресс-бара
						switch(debug){
							case 1: pss.update(); break;
							case 2: pss.status(); break;
						}
					}
					// Выполняем загрузку файла суффиксов цифровых аббревиатур
					tokenizer.readSuffix(filename, [debug, &pss](const string & filename, const u_short status) noexcept {
						// Если отладка включена, выводим имя файла
						if(debug > 0) pss.description(filename);
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
				// Если требуется загрузить карту последовательности или список карт последовательностей
				if(env.is("r-map") && env.is("r-vocab")){
					// Если нужно загрузить карту последовательности
					if((value = env.get("r-map")) != nullptr){
						// Запоминаем адрес файла
						const string & filename = realpath(value, nullptr);
						// Если отладка включена, выводим индикатор загрузки
						if(debug > 0){
							// Очищаем предыдущий прогресс-бар
							pss.clear();
							// Устанавливаем название файла
							pss.description(filename);
							// Устанавливаем заголовки прогресс-бара
							pss.title("Read map", "Read map is done");
							// Выводим индикатор прогресс-бара
							switch(debug){
								case 1: pss.update(); break;
								case 2: pss.status(); break;
							}
						}
						// Считываем карту последовательности
						toolkit.readMap(filename, [debug, &pss](const string & filename, const u_short status) noexcept {
							// Если отладка включена, устанавливаем название файла
							if(debug > 0) pss.description(filename);
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
				if(!env.is("method", "merge") && env.is("w-arpa") && (((env.is("r-vocab") || env.is("r-words")) && env.is("r-map")) || env.is("r-ngram"))){
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
				} else if(env.is("method", "vprune") && (env.is("vprune-wltf") || env.is("vprune-oc") || env.is("vprune-dc"))) {
					// Получаем порог встречаемости слова
					const size_t oc = (((value = env.get("vprune-oc")) != nullptr) && alphabet.isNumber(value) ? stoull(value) : 0);
					// Получаем порог количества документов, где встретилось слово
					const size_t dc = (((value = env.get("vprune-dc")) != nullptr) && alphabet.isNumber(value) ? stoull(value) : 0);
					// Получаем порог веса слова
					const double wltf = (((value = env.get("vprune-wltf")) != nullptr) && alphabet.isDecimal(value) ? stod(value) : 0.0);
					// Проверяем на правильность входных параметров
					if((wltf != 0.0) || (oc > 0) || (dc > 0)){
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
						toolkit.pruneVocab(wltf, oc, dc, [debug, &pss](const u_short status) noexcept {
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
					// Сообщаем, что порог передан неверный
					} else print("threshold is broken\r\n", env.get("log"));
				// Если нужно выполнить прунинг arpa
				} else if(env.is("method", "aprune")) {
					// Максимальный размер n-граммы
					u_short size = 0;
					// Коэффициент прунинга
					double prune = 0.0;
					// Если параметр максимального размера n-граммы для прунинга получен
					if((value = env.get("aprune-max-gram")) != nullptr) size = stoi(value);
					// Если параметр прунинга получен
					if((value = env.get("aprune-threshold")) != nullptr) prune = stod(value);
					// Если параметры получены
					if((size > 0) && (prune != 0.0)){
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
						else print("modify flag is broken\r\n", env.get("log"));
					// Сообщаем, что флаг не установлен
					} else print("modify flag is not set\r\n", env.get("log"));
					// Если файл передан
					if(((value = env.get("modify-file")) != nullptr) && fsys_t::isfile(value)){
						// Запоминаем адрес файла
						const string & filename = realpath(value, nullptr);
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
					// Сообщаем, что файл модификации не передан
					} else print("file modify not found\r\n", env.get("log"));
				}
			// Выводим сообщение, что файлы не переданы
			} else print("arpa file is not loaded\r\n", env.get("log"));
			// Если адрес файла списка суффиксов цифровых аббревиатур получен
			if((value = env.get("w-abbr")) != nullptr){
				// Если отладка включена, выводим индикатор загрузки
				if(debug > 0){
					// Очищаем предыдущий прогресс-бар
					pss.clear();
					// Устанавливаем название файла
					pss.description(value);
					// Устанавливаем заголовки прогресс-бара
					pss.title("Write abbrs", "Write abbrs is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Выполняем модификацию файла
				tokenizer.writeSuffix(value, [debug, &pss](const u_short status) noexcept {
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
			// Если файл для сохранения конфигурационных данных передан
			if((value = env.get("w-json")) != nullptr){
				// Получаем конфигурационные данные в формате JSON
				const json & config = env.getJson();
				// Если конфигурационные данные получены
				if(!config.empty()){
					// Открываем файл на запись
					ofstream file(value, ios::binary);
					// Если файл открыт
					if(file.is_open()){
						// Формируем текст результата
						const string & text = config.dump(4);
						// Если текст получен, записываем данные в файл
						if(!text.empty()) file.write(text.data(), text.size());
					}
					// Закрываем файл
					file.close();
				}
			}
			// Если файл для сохранения слов передан
			if((value = env.get("w-words")) != nullptr){
				// Если отладка включена, выводим индикатор загрузки
				if(debug > 0){
					// Очищаем предыдущий прогресс-бар
					pss.clear();
					// Устанавливаем название файла
					pss.description(value);
					// Устанавливаем заголовки прогресс-бара
					pss.title("Write words", "Write words is done");
					// Выводим индикатор прогресс-бара
					switch(debug){
						case 1: pss.update(); break;
						case 2: pss.status(); break;
					}
				}
				// Выполняем извлечение слов в файл
				toolkit.writeWords(value, [debug, &pss](const u_short status) noexcept {
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
			if((env.is("method", "train") || (env.is("r-map") && (env.is("r-vocab") || env.is("r-words"))) ||
			env.is("r-ngram") || !binDictFile.empty()) && ((value = env.get("w-map")) != nullptr)){
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
			if(!env.is("method", "merge") && ((value = env.get("w-arpa")) != nullptr)){
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
			if((env.is("method", "train") || (env.is("r-map") && (env.is("r-vocab") || env.is("r-words"))) ||
			env.is("r-ngram") || !binDictFile.empty()) && ((value = env.get("w-ngram")) != nullptr)){
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
			if(!env.is("method", "merge") && ((value = env.get("w-bin")) != nullptr)){
				// Создаём бинарный контейнер
				ablm_t ablm(value, &toolkit, &alphabet, &tokenizer, env.get("log"));
				// Если размер шифрования передан
				if((value = env.get("bin-aes")) != nullptr){
					// Если размер шифрования получен
					switch(stoi(value)){
						// Если это 128-и битное шифрование
						case 128: ablm.setAES(aspl_t::types_t::aes128); break;
						// Если это 192-х битное шифрование
						case 192: ablm.setAES(aspl_t::types_t::aes192); break;
						// Если это 256-и битное шифрование
						case 256: ablm.setAES(aspl_t::types_t::aes256); break;
					}
				}
				// Если название словаря передано
				if((value = env.get("bin-name")) != nullptr) ablm.setName(value);
				// Если автор словаря передан
				if((value = env.get("bin-author")) != nullptr) ablm.setAuthor(value);
				// Если тип лицензии словаря передан
				if((value = env.get("bin-lictype")) != nullptr) ablm.setLictype(value);
				// Если текст лицензии словаря передан
				if((value = env.get("bin-lictext")) != nullptr) ablm.setLictext(value);
				// Если контакты автора словаря переданы
				if((value = env.get("bin-contacts")) != nullptr) ablm.setContacts(value);
				// Если пароль словаря передан
				if((value = env.get("bin-password")) != nullptr) ablm.setPassword(value);
				// Если копирайт словаря передан
				if((value = env.get("bin-copyright")) != nullptr) ablm.setCopyright(value);
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
				// Получаем диапазон времени
				auto dimension = pss.dimension(chrono::duration_cast <chrono::seconds> (chrono::system_clock::now() - timeShifting).count());
				// Выводим результат
				print(alphabet.format("work time shifting: %lld %s\r\n", dimension.first, dimension.second.c_str()), env.get("log"), alphabet_t::log_t::info);
			}
		// Сообщаем, что файл алфавита не найден
		} else print("file alphabet not found\r\n", env.get("log"));
	}
	// Успешно выходим
	return 0;
}

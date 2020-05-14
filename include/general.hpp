/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

// Версия приложения
#define ANYKS_VERSION "2.1.6"
// Версия словаря
#define ANYKS_DICT_VERSION "1.0.0"
// Название языковой модели
#define ANYKS_LM_NAME "alm"
// Автор приложения
#define ANYKS_AUTHOR "Yuriy Lobarev"
// Телефон автора
#define ANYKS_PHONE "+7(910)983-95-90"
// Телеграм-контакт
#define ANYKS_CONTACT "@forman"
// Адрес электронной почты
#define ANYKS_EMAIL "forman@anyks.com"
// Адрес сайта автора
#define ANYKS_SITE "https://anyks.com"

/** ПАРАМЕТРЫ СЛОВ **/
// Максимальная длина последовательности в символах
#define MAX_SEQ_LENGTH 50
// Максимальная длина слова
#define MAX_WORD_LENGTH 35
// Максимальная длина строки 100mb
#define MAX_STRING_BYTES 0x19990
// Максимально-возможный вес слова
#define MAX_WORD_WEIGHT 1e+20f
// Минимально-возможный вес слова
#define MIN_WORD_WEIGHT -1e+20f

/** ПАРАМЕТРЫ NSWLIB **/
// Тип алгоритма nswlib
#define NSWLIB_TYPE "l2"
// Количество элементов к выдачи
#define NSWLIB_COUNT 10000
// Точность nswlib
#define NSWLIB_ACCURACY 10

/** ПАРАМЕТРЫ ПРОЧИЕ **/
// Длина n-грамм по умолчанию
#define DEFNGRAM 3
// Размер чанка буфера для чтения из файла
#define BUFFER_CHUNK 0x19990 // 0x3000

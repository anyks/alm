/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

// Название языковой модели
#define ANYKS_LM_NAME "alm"
// Версия приложения
#define ANYKS_LM_VERSION "3.3.5"
// Автор приложения
#define ANYKS_LM_AUTHOR "Yuriy Lobarev"
// Телефон автора
#define ANYKS_LM_PHONE "+7 (910) 983-95-90"
// Телеграм-контакт
#define ANYKS_LM_CONTACT "@forman"
// Адрес электронной почты
#define ANYKS_LM_EMAIL "forman@anyks.com"
// Адрес сайта автора
#define ANYKS_LM_SITE "https://anyks.com"

/** ПАРАМЕТРЫ СЛОВ **/
// Локаль словаря по умолчанию
#define ALPHABET_LOCALE "en_US.UTF-8"
// Максимальная длина последовательности в символах
#define MAX_SEQ_LENGTH 50
// Максимальная длина слова
#define MAX_WORD_LENGTH 35
// Максимальная длина строки 103kb
#define MAX_STRING_BYTES 0x19C00

/** ПАРАМЕТРЫ ПРОЧИЕ **/
// Длина n-грамм по умолчанию
#define DEFNGRAM 3
// Размер чанка буфера для чтения из файла
#define BUFFER_CHUNK 0x19C00
// Размер бинарного буфера языковой модели
#define BUFFER_SIZE 0x6400000

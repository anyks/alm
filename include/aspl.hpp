/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_ASPL_
#define _ANYKS_ASPL_

/**
 * Стандартная библиотека
 */
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <cstring>
#include <zlib.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <openssl/md5.h>
#include <openssl/aes.h>
#include <openssl/evp.h>
/**
 * Наши модули
 */
#include <bigint/BigIntegerLibrary.hh>

// Параметры Zlib
#define MOD_GZIP_ZLIB_CFACTOR 9
#define MOD_GZIP_ZLIB_BSIZE 8096
#define MOD_GZIP_ZLIB_WINDOWSIZE 15

// Объявляем пространство имен
using namespace std;

/*
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * ASpl Класс бинарного контейнера
	 */
	typedef class ASpl {
		private:
			/**
			 * Основные методы работы
			 */
			enum class method_t : bool {read, write};
		public:
			/**
			 * Набор размеров шифрования
			 */
			enum class types_t : u_short {aes128 = 128, aes192 = 192, aes256 = 256};
		private:
			// Максимальный размер слова
			static constexpr u_short MAXWORD = 35;
			// Размер чанка в байтах
			static constexpr u_int CHUNKSIZE = 1048576;
			// Заголовок бинарного файла
			static constexpr const char * HEADER = "ASPL";
			// Алфавит символов для генерации ключа
			static constexpr const char * ALPHABET = "¶abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+*-_./\\";
		private:
			/**
			 * State Стрейт шифрования
			 */
			mutable struct StateAES {
				// Количество обработанных байт
				int num;
				// Буфер данных для шифрования
				u_char ivec[AES_BLOCK_SIZE];
				/**
				 * StateAES Конструктор
				 */
				StateAES() : num(0) {}
			} stateAES;
		private:
			// Файловый поток для чтения
			ifstream ifs;
			// Файловый поток для записи
			ofstream ofs;
			// Список модулей для каждой буквы алфавита
			vector <size_t> xs;
			// Соль и пароль для шифрования
			string salt, password;
			// Путь, название и расширение файла
			string path, name, ext;
			// Список заголовков записей
			map <size_t, size_t> keys;
		private:
			// Ключ шифрования
			mutable AES_KEY aesKey;
			// Определяем длину шифрования по умолчанию
			types_t aeslen = types_t::aes128;
		private:
			// Позиция raw данных в файле
			long rawpos = -1;
			// Устанавливаем количество раундов
			int roundsAES = 5;
			// Модуль вектора
			size_t modulus = 0;
			// Адрес начала данных
			size_t address = 0;
		private:
			// Заголовок файла
			const char * header = HEADER;
			// Алфавит ключей
			const string alphabet = ALPHABET;
		private:
			/**
			 * setval Функция установки числовых и булевых значений
			 * @param idw    идентификатор записи
			 * @param rawpos бинарная позиция в файле
			 * @param keys   список ключей
			 * @param file   файловый поток для записи
			 * @param value  значение записи
			 * @return       результат операции (количество записанных байт)
			 */
			template <typename Value>
			static const int setval(const size_t idw, long & rawpos, map <size_t, size_t> & keys, ofstream & file, const Value value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if((idw > 0) && file.is_open()){
					// Размер записываемых данных
					const size_t size = sizeof(value);
					// Получаем бинарные данные
					const char * bin = reinterpret_cast <const char *> (&value);
					// Выполняем запись в файл
					file.write(bin, size);
					// Запоминаем текущую позицию файла
					rawpos = file.tellp();
					// Выполняем сброс данных в файл
					file.flush();
					// Запоминаем количество записанных байт
					result = size;
					// Запоминаем позицию записи
					keys.insert({idw, rawpos - result});
				}
				// Выводим результат
				return result;
			}
			/**
			 * setdat Функция установки бинарных данных
			 * @param idw    идентификатор записи
			 * @param rawpos бинарная позиция в файле
			 * @param keys   список ключей
			 * @param file   файловый поток для записи
			 * @param value  значение записи
			 * @param md5    хэш md5 если существует
			 * @return       результат операции (количество записанных байт)
			 */
			template <class Container>
			static const int setdat(const size_t idw, long & rawpos, map <size_t, size_t> & keys, ofstream & file, const Container & value, const string & md5 = "") noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if((idw > 0) && file.is_open()){
					// Размер записываемых данных
					const size_t size = value.size();
					// Размер md5 записи
					const size_t sizeMd5 = md5.size();
					// Получаем бинарные данные размера записываемых данных
					const char * bin = reinterpret_cast <const char *> (&size);
					// Получаем бинарные данные размера md5 записи
					const char * binMd5 = reinterpret_cast <const char *> (&sizeMd5);
					// Записываем размер md5 записи
					file.write(binMd5, sizeof(sizeMd5));
					// Выполняем запись в файл строку md5
					file.write(md5.data(), sizeMd5);
					// Выполняем запись в файл размер строки
					file.write(bin, sizeof(size));
					// Выполняем запись в файл саму строку
					file.write(value.data(), size);
					// Запоминаем текущую позицию файла
					rawpos = file.tellp();
					// Выполняем сброс данных в файл
					file.flush();
					// Запоминаем количество записанных байт
					result = (size + sizeMd5 + (sizeof(size) * 2));
					// Запоминаем позицию записи
					keys.insert({idw, rawpos - result});
				}
				// Выводим результат
				return result;
			}
			/**
			 * setvals Функция установки списка числовых значений
			 * @param idw    идентификатор записи
			 * @param rawpos бинарная позиция в файле
			 * @param keys   список ключей
			 * @param file   файловый поток для записи
			 * @param value  значение записи
			 * @return       результат операции (количество записанных байт)
			 */
			template <class Container>
			static const int setvals(const size_t idw, long & rawpos, map <size_t, size_t> & keys, ofstream & file, const Container & value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if((idw > 0) && file.is_open()){
					// Количество записей в списке
					const size_t count = value.size();
					// Общее количество записанных данных
					size_t size = sizeof(count);
					// Получаем бинарные данные размера записываемых данных
					const char * bin = reinterpret_cast <const char *> (&count);
					// Выполняем запись в файл количество записей
					file.write(bin, sizeof(count));
					// Переходим по всему списку записей
					for(auto it = value.cbegin(); it != value.cend(); ++it){
						// Получаем значение
						typename Container::value_type val = (* it);
						// Получаем бинарные данные размера записываемых данных
						const char * bin = reinterpret_cast <const char *> (&val);
						// Выполняем запись в файл каждое значение
						file.write(bin, sizeof(val));
						// Запоминаем текущую позицию файла
						rawpos = file.tellp();
						// Выполняем сброс данных в файл
						file.flush();
						// Увеличиваем общий размер данных
						size += sizeof(val);
					}
					// Запоминаем количество записанных байт
					result = size;
					// Запоминаем позицию записи
					keys.insert({idw, rawpos - result});
				}
				// Выводим результат
				return result;
			}
			/**
			 * setstrs Функция установки списка строковых значений
			 * @param idw    идентификатор записи
			 * @param rawpos бинарная позиция в файле
			 * @param keys   список ключей
			 * @param file   файловый поток для записи
			 * @param value  значение записи
			 * @return       результат операции (количество записанных байт)
			 */
			template <class Container>
			static const int setstrs(const size_t idw, long & rawpos, map <size_t, size_t> & keys, ofstream & file, const Container & value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if((idw > 0) && file.is_open()){
					// Количество записей в списке
					const size_t count = value.size();
					// Общее количество записанных данных
					size_t size = sizeof(count);
					// Получаем бинарные данные размера записываемых данных
					const char * bin = reinterpret_cast <const char *> (&count);
					// Выполняем запись в файл количество записей
					file.write(bin, sizeof(count));
					// Переходим по всему списку записей
					for(auto it = value.cbegin(); it != value.cend(); ++it){
						// Размер записываемых данных
						const size_t length = it->size();
						// Получаем бинарные данные размера записываемых данных
						const char * bin = reinterpret_cast <const char *> (&length);
						// Выполняем запись в файл размер строки
						file.write(bin, sizeof(length));
						// Выполняем запись в файл саму строку
						file.write(it->data(), length);
						// Запоминаем текущую позицию файла
						rawpos = file.tellp();
						// Выполняем сброс данных в файл
						file.flush();
						// Увеличиваем общий размер данных
						size += (sizeof(length) + length);
					}
					// Запоминаем количество записанных байт
					result = size;
					// Запоминаем позицию записи
					keys.insert({idw, rawpos - result});
				}
				// Выводим результат
				return result;
			}
			/**
			 * getval Функция считывания целочисленных значений
			 * @param pos   позиция записи в файле
			 * @param file  файловый поток для записи
			 * @param value значение записи
			 * @return      результат операции (количество записанных байт)
			 */
			template <typename Value>
			static const int getval(const size_t pos, ifstream & file, Value & value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если файл открыт удачно
				if(file.is_open()){
					// Перемещаемся в указанную позицию файла
					file.seekg(pos, file.beg);
					// Выполняем чтение из файла
					file.read((char *) &value, sizeof(value));
					// Запоминаем количество прочитанных байт
					result = sizeof(value);
				}
				// Выводим результат
				return result;
			}
			/**
			 * getdat Функция извлечения бинарных данных
			 * @param pos   позиция записи в файле
			 * @param file  файловый поток для записи
			 * @param value значение записи
			 * @param md5   хэш md5 если существует
			 * @return      результат операции (количество прочитанных байт)
			 */
			template <class Container>
			static const int getdat(const size_t pos, ifstream & file, Container & value, string & md5) noexcept {
				// Результат работы функции
				int result = -1;
				// Если файл открыт удачно
				if(file.is_open()){
					// Размер записываемых данных
					size_t size = 0, sizeMd5 = 0;
					// Перемещаемся в указанную позицию файла
					file.seekg(pos, file.beg);
					// Считываем размер записи md5
					file.read((char *) &sizeMd5, sizeof(sizeMd5));
					// Если md5 запись получена
					if(sizeMd5 > 0){
						// Буфер данных md5 хэша
						char buffer[64];
						// Зануляем буфер
						memset(buffer, 0 , sizeMd5);
						// Считываем в буфер md5 запись
						file.read(buffer, sizeMd5);
						// Запоминаем полученную строку md5
						md5.assign(buffer, sizeMd5);
					}
					// Выполняем чтение из файла
					file.read((char *) &size, sizeof(size));
					// Если размер строки получен
					if(size > 0){
						// Создаем буфер данных для чтения строки
						vector <char> buffer(size);
						// Выполняем чтение данных строки
						file.read(buffer.data(), size);
						// Запоминаем результат
						value.assign(buffer.begin(), buffer.end());
					}
					// Запоминаем количество прочитанных байт
					result = (size + sizeMd5 + (sizeof(size) * 2));
				}
				// Выводим результат
				return result;
			}
			/**
			 * getvals Функция чтения списка числовых значений
			 * @param pos   позиция записи в файле
			 * @param file  файловый поток для записи
			 * @param value значение записи
			 * @return      результат операции (количество записанных байт)
			 */
			template <class Container>
			static const int getvals(const size_t pos, ifstream & file, Container & value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если файл открыт удачно
				if(file.is_open()){
					// Количество записей в списке
					size_t count = 0;
					// Перемещаемся в указанную позицию файла
					file.seekg(pos, file.beg);
					// Выполняем чтение из файла количество записей
					file.read((char *) &count, sizeof(count));
					// Запоминаем количество прочитанных байт
					result = sizeof(count);
					// Если количество записей больше 0
					if(count > 0){
						// Очищаем контейнер
						value.clear();
						// Ожидаемое значение
						typename Container::value_type val;
						// Считываем нужное нам количество записей
						for(size_t i = 0; i < count; i++){
							// Считываем значение из списка
							file.read((char *) &val, sizeof(val));
							// Добавляем значение в список
							value.insert(value.end(), val);
							// Увеличиваем количество прочитанных байт
							result += sizeof(val);
						}
					}
				}
				// Выводим результат
				return result;
			}
			/**
			 * getstrs Функция чтения списка строковых значений
			 * @param pos   позиция записи в файле
			 * @param file  файловый поток для записи
			 * @param value значение записи
			 * @return       результат операции (количество записанных байт)
			 */
			template <class Container>
			static const int getstrs(const size_t pos, ifstream & file, Container & value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если файл открыт удачно
				if(file.is_open()){
					// Количество записей в списке
					size_t count = 0;
					// Перемещаемся в указанную позицию файла
					file.seekg(pos, file.beg);
					// Выполняем чтение из файла количество записей
					file.read((char *) &count, sizeof(count));
					// Запоминаем количество прочитанных байт
					result = sizeof(count);
					// Если количество записей больше 0
					if(count > 0){
						// Очищаем контейнер
						value.clear();
						// Длина строки
						size_t length = 0;
						// Считываем нужное нам количество записей
						for(size_t i = 0; i < count; i++){
							// Считываем длину строки из списка
							file.read((char *) &length, sizeof(length));
							// Если длина получена
							if(length > 0){
								try {
									// Создаем буфер данных
									char * buffer = new char [length + 1];
									// Зануляем буфер данных
									memset(buffer, 0, length);
									// Считываем в буфер данных значение строки
									file.read(buffer, length);
									// Добавляем конец строки
									buffer[length] = '\0';
									// Добавляем строку в список
									value.insert(value.end(), buffer);
									// Удаляем буфер данных
									delete [] buffer;
									// Увеличиваем количество прочитанных байт
									result += (length + sizeof(length));
								// Если происходит ошибка то игнорируем её
								} catch(const bad_alloc&) {
									// Выходим из приложения
									exit(EXIT_FAILURE);
								}
							}
						}
					}
				}
				// Выводим результат
				return result;
			}
		protected:
			/**
			 * init Метод инициализации класса
			 */
			void init() noexcept {
				// Смещаем указатель начала данных
				this->address = strlen(HEADER);
				// Выполняем расчёт модуля слова
				this->modulus = (pow(2, MAXWORD + 1) - 1);
				// Получаем длину алфавита
				const size_t length = this->alphabet.length();
				// Формируем диапазон значений
				for(size_t i = 0; i < MAXWORD; i++) this->xs.push_back(modexp(length, i, this->modulus).toUnsignedLong());
			}
			/**
			 * close Метод закрытия файла базы на чтение или запись
			 */
			void close() noexcept {
				// Обнуляем последнюю позицию в файле
				this->rawpos = -1;
				// Если файл открыт, закрываем его
				if(this->ifs.is_open()) this->ifs.close();
				// Если файл открыт, закрываем его
				if(this->ofs.is_open()) this->ofs.close();
			}
			/**
			 * open Метод открытия файла базы на чтение или запись
			 * @param status статус файлового потока
			 * @param raw    открыть сырой файл
			 */
			void open(const method_t status, const bool raw = false) noexcept {
				// Адрес файла для открытия
				const string & filename = (!raw ? this->filename() : this->filename() + ".raw");
				// Если адрес файла получен
				if(!filename.empty()){
					// Определяем тип статуса файлового потока
					switch((u_short) status){
						// Открыть на чтение
						case (u_short) method_t::read: {
							// Если файл открыт на чтение
							if(this->ifs.is_open()) this->close();
							// Открываем файл на чтение
							this->ifs.open(filename.c_str(), ios::binary);
						} break;
						// Открываем на запись
						case (u_short) method_t::write: {
							// Если файл открыт на запись
							if(this->ofs.is_open()) this->close();
							// Если файл существует
							if((this->rawpos == -1) && !this->isFile(filename)) this->rawpos = 0;
							// Открываем файл на запись
							this->ofs.open(filename.c_str(), ios::binary);
							// Если позиция не определена
							if(this->rawpos == -1){
								// Перемещаемся в конец файла
								this->ofs.seekp(0, this->ofs.end);
								// Определяем размер файла
								this->rawpos = this->ofs.tellp();
							}
						} break;
					}
				}
			}
		protected:
			/**
			 * filename Метод получения адреса файла базы
			 * @return адрес файла базы
			 */
			const string filename() const noexcept {
				// Результат работы функции
				string result = "";
				// Добавляем путь к базе
				result.append(this->path);
				// Добавляем разделитель каталога
				result.append("/");
				// Добавляем имя файла
				result.append(this->name);
				// Добавляем точку расширения
				result.append(".");
				// Добавляем расширение файла
				result.append(this->ext);
				// Выводим результат
				return result;
			}
			/**
			 * md5 Метод получения md5 хэша из строки
			 * @param text текст для перевода в строку
			 * @return     хэш md5
			 */
			const string md5(const vector <char> & dump) const noexcept {
				// Результат работы функции
				string result;
				// Если текст передан
				if(!dump.empty()){
					// Массив полученных значений
					u_char digest[16];
					// Создаем контекст
					MD5_CTX ctx;
					// Выполняем инициализацию контекста
					MD5_Init(&ctx);
					// Выполняем расчет суммы
					MD5_Update(&ctx, dump.data(), dump.size());
					// Копируем полученные данные
					MD5_Final(digest, &ctx);
					// Строка md5
					char mdString[33];
					// Заполняем массив нулями
					memset(mdString, 0, 33);
					// Заполняем строку данными md5
					for(u_short i = 0; i < 16; i++) sprintf(&mdString[i * 2], "%02x", (u_int) digest[i]);
					// Выводим результат
					result = move(mdString);
				}
				// Выводим результат
				return result;
			}
		protected:
			/**
			 * initAES Метод инициализации AES шифрования
			 * @return результат инициализации
			 */
			const bool initAES() const {
				// Экранируем возможность ошибки памяти
				try {
					// Создаем тип шифрования
					const EVP_CIPHER * cipher = EVP_enc_null();
					// Устанавливаем длину шифрования
					switch((u_short) this->aeslen){
						// Устанавливаем шифрование в 128
						case (u_short) types_t::aes128: cipher = EVP_aes_128_ecb(); break;
						// Устанавливаем шифрование в 192
						case (u_short) types_t::aes192: cipher = EVP_aes_192_ecb(); break;
						// Устанавливаем шифрование в 256
						case (u_short) types_t::aes256: cipher = EVP_aes_256_ecb(); break;
						// Если ничего не выбрано, сбрасываем
						default: return false;
					}
					// Создаем контекст
					EVP_CIPHER_CTX * ctx = EVP_CIPHER_CTX_new();
					// Привязываем контекст к типу шифрования
					EVP_EncryptInit_ex(ctx, cipher, nullptr, nullptr, nullptr);
					// Получаем размеры буферов
					const int ivlen = EVP_CIPHER_CTX_iv_length(ctx);
					const int keylen = EVP_CIPHER_CTX_key_length(ctx);
					// Выделяем нужное количество памяти
					u_char * iv = new u_char [ivlen];
					u_char * key = new u_char [keylen];
					// Выполняем инициализацию ключа
					const int ok = EVP_BytesToKey(cipher, EVP_sha256(), (this->salt.empty() ? nullptr : (u_char *) this->salt.data()), (u_char *) this->password.data(), this->password.length(), this->roundsAES, key, iv);
					// Очищаем контекст
					EVP_CIPHER_CTX_free(ctx);
					// Если инициализация не произошла
					if(ok == 0){
						// Очищаем выделенную память буферов
						delete [] iv;
						delete [] key;
						// Выходим
						return false;
					}
					// Устанавливаем ключ шифрования
					const int res = AES_set_encrypt_key(key, keylen * 8, &this->aesKey);
					// Удаляем выделенную память для ключа
					delete [] key;
					// Если установка ключа не произошло
					if(res != 0){
						// Очищаем выделенную память буферов
						delete [] iv;
						// Выходим
						return false;
					}
					// Обнуляем номер
					this->stateAES.num = 0;
					// Заполняем половину структуры нулями
					memset(this->stateAES.ivec, 0, sizeof(this->stateAES.ivec));
					// Копируем данные шифрования
					memcpy(this->stateAES.ivec, iv, ivlen);
					// Выполняем шифрование
					// AES_encrypt(this->stateAES.ivec, this->stateAES.count, &this->aesKey);
					// Очищаем выделенную память буферов
					delete [] iv;
					// Сообщаем что всё удачно
					return true;
				// Если происходит ошибка то игнорируем её
				} catch(const bad_alloc&) {
					// Выходим из приложения
					exit(EXIT_FAILURE);
				}
			}
			/**
			 * isFile Метод проверяющий существование файла
			 * @param  filename адрес файла
			 * @return          результат проверки
			 */
			const bool isFile(const string & filename) const noexcept {
				// Результат проверки
				bool result = false;
				// Если адрес файла передан
				if(!filename.empty()){
					// Структура проверка статистики
					struct stat info;
					// Проверяем переданный нам адрес
					result = ((stat(filename.c_str(), &info) == 0) && ((info.st_mode & S_IFMT) != 0));
				}
				// Выводим результат
				return result;
			}
			/**
			 * idw Метод генерирования идентификатора слова
			 * @param  word слово для генерации
			 * @return      идентификатор слова
			 */
			const size_t idw(const string & word) const noexcept {
				// Результат работы функции
				size_t result = 0;
				// Если слово передано
				if(!word.empty()){
					// Контрольная сумма
					size_t sum = 0;
					// Переходим по всему слову
					for(size_t i = 0; i < word.length(); i++){
						// Получаем букву
						const string & letter = word.substr(i, 1);
						// Получаем позицию буквы в алфавите
						const size_t pos = this->alphabet.find(letter);
						// Генерируем вектор
						if(pos != string::npos) sum += (this->xs[i] * pos);
					}
					// Убираем колизии
					result = (sum % this->modulus);
				}
				// Выводим результат
				return result;
			}
		protected:
			/**
			 * encrypt Метод шифрования текста
			 * @param  buffer буфер данных для шифрования
			 * @param  size   размер данных для шифрования
			 * @return        результат шифрования
			 */
			const vector <char> encrypt(const char * buffer = nullptr, const size_t size = 0) const {
				// Результат работы функции
				vector <char> result;
				// Если буфер данных передан
				if((buffer != nullptr) && (size > 0)){
					// Если пароль установлен
					if(!this->password.empty()){
						// Выполняем инициализацию
						this->initAES();
						// Экранируем возможность ошибки памяти
						try {
							// Размер буфера полученных данных
							size_t count = 0;
							// Определяем размер данных для считывания
							size_t len = size;
							// Входные данные
							const u_char * input = (u_char *) buffer;
							// Выделяем память для буфера данных
							u_char * output = new u_char [len];
							// Выполняем извлечение оставшихся данных
							do {
								// Максимальный размер считываемых данных
								int chunk = (len > CHUNKSIZE ? CHUNKSIZE : len);
								// Выполняем сжатие данных
								AES_cfb128_encrypt(input + count, output + count, chunk, &this->aesKey, this->stateAES.ivec, &this->stateAES.num, AES_ENCRYPT);
								// Увеличиваем смещение
								count += chunk;
								// Вычитаем считанные данные
								len -= chunk;
							} while(len > 0);
							// Запоминаем полученные данные
							result.insert(result.end(), output, output + count);
							// Очищаем буфер
							delete [] output;
						// Если происходит ошибка то игнорируем её
						} catch(const bad_alloc&) {
							// Выходим из приложения
							exit(EXIT_FAILURE);
						}
					// Выводим тот же самый буфер как он был передан
					} else result.insert(result.end(), buffer, buffer + size);
				}
				// Выводим результат
				return result;
			}
			/**
			 * decrypt Метод дешифрования текста
			 * @param  buffer буфер данных для дешифрования
			 * @param  size   размер данных для дешифрования
			 * @return        результат дешифрования
			 */
			const vector <char> decrypt(const char * buffer = nullptr, const size_t size = 0) const {
				// Результат работы функции
				vector <char> result;
				// Если буфер данных передан
				if((buffer != nullptr) && (size > 0)){
					// Если пароль установлен
					if(!this->password.empty()){
						// Выполняем инициализацию
						this->initAES();
						// Экранируем возможность ошибки памяти
						try {
							// Размер буфера полученных данных
							size_t count = 0;
							// Определяем размер данных для считывания
							size_t len = size;
							// Входные данные
							const u_char * input = (u_char *) buffer;
							// Выделяем память для буфера данных
							u_char * output = new u_char [len];
							// Выполняем извлечение оставшихся данных
							do {
								// Максимальный размер считываемых данных
								int chunk = (len > CHUNKSIZE ? CHUNKSIZE : len);
								// Выполняем сжатие данных
								AES_cfb128_encrypt(input + count, output + count, chunk, &this->aesKey, this->stateAES.ivec, &this->stateAES.num, AES_DECRYPT);
								// Увеличиваем смещение
								count += chunk;
								// Вычитаем считанные данные
								len -= chunk;
							} while(len > 0);
							// Запоминаем полученные данные
							result.insert(result.end(), output, output + count);
							// Очищаем буфер
							delete [] output;
						// Если происходит ошибка то игнорируем её
						} catch(const bad_alloc&) {
							// Выходим из приложения
							exit(EXIT_FAILURE);
						}
					// Выводим тот же самый буфер как он был передан
					} else result.insert(result.end(), buffer, buffer + size);
				}
				// Выводим результат
				return result;
			}
			/**
			 * compress Метод сжатия данных
			 * @param  buffer буфер данных для сжатия
			 * @param  size   размер данных для сжатия
			 * @return        результат сжатия
			 */
			const vector <char> compress(const char * buffer = nullptr, const size_t size = 0) const noexcept {
				// Результат работы функции
				vector <char> result;
				// Если буфер передан
				if((buffer != nullptr) && (size > 0)){
					// Создаем поток zip
					z_stream zs = {0};
					// Заполняем его нулями
					memset(&zs, 0, sizeof(zs));
					// Буфер входных данных
					uint8_t inbuff[CHUNKSIZE];
					// Буфер выходных данных
					uint8_t outbuff[CHUNKSIZE];
					// Если поток инициализировать не удалось, выходим
					if(deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED, MOD_GZIP_ZLIB_WINDOWSIZE + 16, MOD_GZIP_ZLIB_CFACTOR, Z_DEFAULT_STRATEGY) == Z_OK){
						// Смещение в буфере
						size_t offset = 0;
						// Переменная сброса данных
						int flush = Z_NO_FLUSH;
						do {
							// Заполняем нулями буфер входящих данных
							memset(inbuff, 0, sizeof(inbuff));
							// Заполняем нулями буфер исходящих данных
							memset(outbuff, 0, sizeof(outbuff));
							// Получаем размер доступных данных
							const size_t count = (size - offset);
							// Высчитываем количество доступных данных
							const size_t avail = (count > size_t(CHUNKSIZE) ? CHUNKSIZE : count);
							// Устанавливаем количество доступных данных
							zs.avail_in = avail;
							// Копируем в буфер данные для шифрования
							memcpy(inbuff, buffer + offset, zs.avail_in);
							// Определяем закончено ли шифрование
							flush = (count > 0 ? Z_NO_FLUSH : Z_FINISH);
							// Устанавливаем буфер с данными для шифрования
							zs.next_in = inbuff;
							do {
								// Устанавливаем буфер для записи шифрованных данных
								zs.next_out = outbuff;
								// Устанавливаем количество доступных данных для записи
								zs.avail_out = CHUNKSIZE;
								// Выполняем шифрование данных
								deflate(&zs, flush);
								// Получаем количество оставшихся байт
								const uint32_t nbytes = (CHUNKSIZE - zs.avail_out);
								// Добавляем оставшиеся данные в список
								result.insert(result.end(), outbuff, outbuff + nbytes);
							// Если все данные уже сжаты
							} while(zs.avail_out == 0);
							// Увеличиваем смещение в буфере
							offset += avail;
						// Если шифрование не зашифрованы
						} while(flush != Z_FINISH);
						// Закрываем поток
						deflateEnd(&zs);
					}
				}
				// Выводим результат
				return result;
			}
			/**
			 * decompress Метод рассжатия данных
			 * @param  buffer буфер данных для расжатия
			 * @param  size   размер данных для сжатия
			 * @return        результат расжатия
			 */
			const vector <char> decompress(const char * buffer = nullptr, const size_t size = 0) const noexcept {
				// Результат работы функции
				vector <char> result;
				// Если буфер передан
				if((buffer != nullptr) && (size > 0)){
					// Создаем поток zip
					z_stream zs = {0};
					// Заполняем его нулями
					memset(&zs, 0, sizeof(zs));
					// Буфер входных данных
					uint8_t inbuff[CHUNKSIZE];
					// Буфер выходных данных
					uint8_t outbuff[CHUNKSIZE];
					// Если поток инициализировать не удалось, выходим
					if(inflateInit2(&zs, MOD_GZIP_ZLIB_WINDOWSIZE + 16) == Z_OK){
						// Смещение в буфере
						size_t offset = 0;
						// Переменная сброса данных
						int flush = Z_NO_FLUSH;
						do {
							// Заполняем нулями буфер входящих данных
							memset(inbuff, 0, sizeof(inbuff));
							// Заполняем нулями буфер исходящих данных
							memset(outbuff, 0, sizeof(outbuff));
							// Получаем размер доступных данных
							const size_t count = (size - offset);
							// Высчитываем количество доступных данных
							const size_t avail = (count > size_t(CHUNKSIZE) ? CHUNKSIZE : count);
							// Устанавливаем количество доступных данных
							zs.avail_in = avail;
							// Если доступных данных нет
							if(zs.avail_in == 0) break;
							// Копируем в буфер данные для шифрования
							memcpy(inbuff, buffer + offset, zs.avail_in);
							// Копируем входящий буфер для дешифровки
							zs.next_in = inbuff;
							do {
								// Устанавливаем буфер для записи дешифрованных данных
								zs.next_out = outbuff;
								// Устанавливаем количество доступных данных для записи
								zs.avail_out = CHUNKSIZE;
								// Выполняем дешифровку данных
								flush = inflate(&zs, Z_NO_FLUSH);
								// Если произошла ошибка дешифровки
								if((flush == Z_NEED_DICT) || (flush == Z_DATA_ERROR) || (flush == Z_MEM_ERROR)){
									// Выполняем запись ошибки в файловый поток
									fprintf(stderr, "gzip decompress is failed: %d\n", flush);
									// Завершаем дешифровку данных
									inflateEnd(&zs);
									// Очищаем блок данных
									result.clear();
									// Выходим из функции дешифровки
									return result;
								}
								// Получаем количество оставшихся байт
								const uint32_t nbytes = (CHUNKSIZE - zs.avail_out);
								// Добавляем оставшиеся данные в список
								result.insert(result.end(), outbuff, outbuff + nbytes);
							// Если все данные уже дешифрованы
							} while(zs.avail_out == 0);
							// Увеличиваем смещение в буфере
							offset += avail;
						// Если дешифрование не закончено
						} while(flush != Z_STREAM_END);
						// Закрываем поток
						inflateEnd(&zs);
					}
				}
				// Выводим результат
				return result;
			}
		public:
			/**
			 * read Метод чтения данных из файлв в базу
			 * @return результат операции (количество прочитанных байт)
			 */
			const int read() noexcept {
				// Результат работы функции
				int result = -1;
				// Адрес файла для открытия
				const string & filename = this->filename();
				// Если адрес файла получен
				if(!filename.empty() && this->isFile(filename)){
					// Закрываем прежние открытые файлы
					this->close();
					// Открываем файл на чтение
					this->ifs.open(filename.c_str(), ios::binary);
					// Если файл открыт
					if(this->ifs.is_open()){
						// Количество записей в файле
						size_t count = 0;
						// Устанавливаем позицию чтения
						this->ifs.seekg(this->address, this->ifs.beg);
						// Считываем количество записей в файле
						this->ifs.read((char *) &count, sizeof(count));
						// Смещаем позицию в файле
						result = sizeof(count);
						// Если количество записей больше 0
						if(count > 0){
							// Загружаем полученное количество данных
							for(size_t i = 0; i < count; i++){
								// Ключ и значение
								size_t idw = 0, val = 0;
								// Считываем ключ записи
								this->ifs.read((char *) &idw, sizeof(idw));
								// Считываем позицию записи
								this->ifs.read((char *) &val, sizeof(val));
								// Смещаем позицию в файле
								result += (sizeof(size_t) * 2);
								// Если значение получено, заполняем заголовки ключей
								if(idw > 0) this->keys.insert({idw, val});
							}
						}
						// Закрываем файл
						this->close();
						// Запоминаем смещение в файле
						this->address += result;
					}
				}
				// Выводим результат
				return result;
			}
			/**
			 * write Метод записи данных базы в файл
			 * @return результат операции (количество записанных байт)
			 */
			const int write() noexcept {
				// Результат работы функции
				int result = -1;
				// Адрес файла для открытия
				const string & raw = (this->filename() + ".raw");
				// Если адрес файла получен
				if(!raw.empty() && this->isFile(raw)){
					// Закрываем прежние открытые файлы
					this->close();
					// Открываем файл на запись
					this->ofs.open(this->filename().c_str(), ios::binary);
					// Если файл открыт
					if(this->ofs.is_open()){
						// Получаем количество записей
						const size_t count = this->keys.size();
						// Получаем бинарные данные количества ключей
						const char * bin = reinterpret_cast <const char *> (&count);
						// Выполняем запись в файл заголовка
						this->ofs.write(header, strlen(HEADER));
						// Выполняем запись в файл количество ключей
						this->ofs.write(bin, sizeof(count));
						// Запоминаем количество записанных данных
						result = (strlen(HEADER) + sizeof(count));
						// Переходим по всему количеству ключей
						for(auto it = this->keys.cbegin(); it != this->keys.cend(); ++it){
							// Получаем ключ
							const size_t key = it->first;
							// Получаем значение ключа
							const size_t val = it->second;
							// Получаем бинарные данные ключа
							const char * binKey = reinterpret_cast <const char *> (&key);
							// Получаем бинарные данные значения
							const char * binVal = reinterpret_cast <const char *> (&val);
							// Выполняем запись в файл бинарные данные ключа
							this->ofs.write(binKey, sizeof(key));
							// Выполняем запись в файл бинарные данные значения
							this->ofs.write(binVal, sizeof(val));
							// Выполняем сброс данных в файл
							this->ofs.flush();
							// Запоминаем количество записанных данных
							result += (sizeof(key) + sizeof(val));
						}
						// Открываем файл на чтение
						ifstream file(raw.c_str(), ios::binary);
						// Если файл открыт
						if(file.is_open()){
							// Создаем буфер для чтения данных
							char bytes[CHUNKSIZE];
							// Считываем до тех пор пока все удачно
							while(file.good()){
								// Зануляем буфер данных
								memset(bytes, 0, CHUNKSIZE);
								// Выполняем чтение данных в буфер
								file.read(bytes, CHUNKSIZE);
								// Получаем количество прочитанных данных
								const size_t size = file.gcount();
								// Выполняем запись в файл
								this->ofs.write(bytes, size);
								// Выполняем сброс данных в файл
								this->ofs.flush();
								// Запоминаем количество записанных данных
								result += size;
							}
							// Закрываем файл
							file.close();
							// Удаляем временный файл с сырыми данными
							remove(raw.c_str());
						}
						// Закрываем файл
						this->close();
						// Очищаем объект
						this->clear();
					}
				}
				// Выводим результат
				return result;
			}
		public:
			/**
			 * set Метод установки числовых значений
			 * @param key ключ записи
			 * @param value значение записи
			 * @return      результат операции (количество записанных байт)
			 */
			template <typename Value>
			const int set(const string & key, const Value value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty()){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись не найдена
					if(this->keys.count(idw) < 1){
						// Открываем файл на запись
						if(!this->ofs.is_open()) this->open(method_t::write, true);
						// Выполняем установку данных в файл
						result = setval(idw, this->rawpos, this->keys, this->ofs, value);
					}
				}
				// Выводим результат
				return result;
			}
			/**
			 * set Метод установки строковых значений
			 * @param key     ключ записи
			 * @param value   значение записи
			 * @param encrypt зашифровать данные
			 * @return        результат операции (количество записанных байт)
			 */
			const int set(const string & key, const string & value, const bool encrypt = false) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty()){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись не найдена
					if(this->keys.count(idw) < 1){
						// Открываем файл на запись
						if(!this->ofs.is_open()) this->open(method_t::write, true);
						// Если нужно зашифровать данные
						if(encrypt){
							// Выполняем шифрование данных
							const vector <char> & data = this->encrypt(value.data(), value.size());
							// Если данные получены
							if(!data.empty()){
								// Получаем данные для записи
								const string & value = string(data.data(), data.size());
								// Выполняем шифрование данных
								result = setdat(idw, this->rawpos, this->keys, this->ofs, value);
							}
						// Выполняем установку данных в файл
						} else result = setdat(idw, this->rawpos, this->keys, this->ofs, value);
					}
				}
				// Выводим результат
				return result;
			}
			/**
			 * set Метод установки бинарных данных
			 * @param key     ключ записи
			 * @param value   значение записи
			 * @param encrypt зашифровать данные
			 * @return        результат операции (количество записанных байт)
			 */
			const int set(const string & key, const vector <char> & value, const bool encrypt = false) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty()){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись не найдена
					if(this->keys.count(idw) < 1){
						// Получаем md5 хэш
						const string & md5 = this->md5(value);
						// Открываем файл на запись
						if(!this->ofs.is_open()) this->open(method_t::write, true);
						// Если нужно зашифровать данные
						if(encrypt){
							// Выполняем шифрование данных
							auto res = this->encrypt(value.data(), value.size());
							// Если данные зашифрованы
							if(!res.empty()){
								// Сжимаем данные перед записью
								auto data = this->compress(res.data(), res.size());
								// Выполняем установку данных в файл
								result = setdat(idw, this->rawpos, this->keys, this->ofs, data, md5);
							}
						// Если шифровать данные не надо
						} else {
							// Сжимаем данные перед записью
							auto res = this->compress(value.data(), value.size());
							// Выполняем установку данных в файл
							result = setdat(idw, this->rawpos, this->keys, this->ofs, res, md5);
						}
					}
				}
				// Выводим результат
				return result;
			}
			/**
			 * set Метод установки бинарных данных
			 * @param key     ключ записи
			 * @param buffer  буфер для записи
			 * @param size    размер буфера для записи
			 * @param encrypt зашифровать данные
			 * @return        результат операции (количество записанных байт)
			 */
			const int set(const string & key, const char * buffer, const size_t size, const bool encrypt = false) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty() && (buffer != nullptr) && (size > 0)){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись не найдена
					if(this->keys.count(idw) < 1){
						// Получаем md5 хэш
						const string & md5 = this->md5(vector <char> (buffer, buffer + size));
						// Открываем файл на запись
						if(!this->ofs.is_open()) this->open(method_t::write, true);
						// Если нужно зашифровать данные
						if(encrypt){
							// Выполняем шифрование данных
							auto res = this->encrypt(buffer, size);
							// Если данные зашифрованны
							if(!res.empty()){
								// Сжимаем данные перед записью
								auto data = this->compress(res.data(), res.size());
								// Выполняем установку данных в файл
								result = setdat(idw, this->rawpos, this->keys, this->ofs, data, md5);
							}
						// Если шифровать данные не надо
						} else {
							// Сжимаем данные перед записью
							auto res = this->compress(buffer, size);
							// Выполняем установку данных в файл
							result = setdat(idw, this->rawpos, this->keys, this->ofs, res, md5);
						}
					}
				}
				// Выводим результат
				return result;
			}
		public:
			/**
			 * setValues Метод установки списка значений
			 * @param key   ключ записи
			 * @param value значение записи
			 * @return      результат операции (количество записанных байт)
			 */
			template <class Container>
			const int setValues(const string & key, const Container & value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty() && !value.empty()){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись не найдена
					if(this->keys.count(idw) < 1){
						// Открываем файл на запись
						if(!this->ofs.is_open()) this->open(method_t::write, true);
						// Выполняем установку данных в файл
						result = setvals(idw, this->rawpos, this->keys, this->ofs, value);
					}
				}
				// Выводим результат
				return result;
			}
			/**
			 * setStrings Метод установки списка строковых значений
			 * @param key   ключ записи
			 * @param value значение записи
			 * @return      результат операции (количество записанных байт)
			 */
			template <class Container>
			const int setStrings(const string & key, const Container & value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty() && !value.empty()){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись не найдена
					if(this->keys.count(idw) < 1){
						// Открываем файл на запись
						if(!this->ofs.is_open()) this->open(method_t::write, true);
						// Выполняем установку данных в файл
						result = setstrs(idw, this->rawpos, this->keys, this->ofs, value);
					}
				}
				// Выводим результат
				return result;
			}
		public:
			/**
			 * get Метод извлечения числовых значений
			 * @param key   ключ записи
			 * @param value полученное значение ключа
			 * @return      результат операции (количество прочитанных байт)
			 */
			template <typename Value>
			const int get(const string & key, Value & value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty()){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись найдена
					if(this->keys.count(idw) > 0){
						// Получаем позицию записи
						const size_t pos = this->keys.at(idw);
						// Открываем файл на чтение
						this->open(method_t::read);
						// Выполняем чтение данных из файла
						result = getval(this->address + pos, this->ifs, value);
						// Закрываем файл
						this->close();
					}
				}
				// Выводим результат
				return result;
			}
			/**
			 * get Метод извлечения строкового значения
			 * @param key     ключ записи
			 * @param value   полученное значение ключа
			 * @param decrypt расшифровать данные
			 * @return        результат операции (количество прочитанных байт)
			 */
			const int get(const string & key, string & value, const bool decrypt = false) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty()){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись найдена
					if(this->keys.count(idw) > 0){
						// Хэш md5 если существует
						string md5 = "";
						// Получаем позицию записи
						const size_t pos = this->keys.at(idw);
						// Открываем файл на чтение
						this->open(method_t::read);
						// Выполняем чтение данных из файла
						result = getdat(this->address + pos, this->ifs, value, md5);
						// Если нужно дешифровать данные
						if(decrypt){
							// Выполняем расшифровку данных
							auto res = this->decrypt(value.data(), value.size());
							// Если результат получен
							if(!res.empty()) value.assign(res.data(), res.size());
						}
						// Закрываем файл
						this->close();
					}
				}
				// Выводим результат
				return result;
			}
			/**
			 * get Метод извлечения бинарных данных
			 * @param key     ключ записи
			 * @param value   полученное значение ключа
			 * @param decrypt расшифровать данные
			 * @return        результат операции (количество прочитанных байт)
			 */
			const int get(const string & key, vector <char> & value, const bool decrypt = false) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty()){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись найдена
					if(this->keys.count(idw) > 0){
						// Хэш md5 если существует
						string md5 = "";
						// Данные для извлечения
						std::vector <char> buffer;
						// Получаем позицию записи
						const size_t pos = this->keys.at(idw);
						// Открываем файл на чтение
						this->open(method_t::read);
						// Выполняем чтение данных из файла
						result = getdat(this->address + pos, this->ifs, buffer, md5);
						// Если буфер не пустой
						if(!buffer.empty()){
							// Выполняем декомпрессию данных
							auto data = this->decompress(buffer.data(), buffer.size());
							// Если декомпрессия получилсь
							if(!data.empty()){
								// Если нужно дешифровать данные
								if(decrypt){
									// Выполняем расшифровку данных
									auto res = this->decrypt(data.data(), data.size());
									// Если данные расшифрованны
									if(!res.empty()){
										// Если хэш md5 совпадает
										if(md5.empty() || (this->md5(res).compare(md5) == 0)){
											// Выполняем обновление полученного буфера
											value.assign(res.begin(), res.end());
										// Если хэш не совпадает выводим сообщение об ошибке
										} else printf("Data is broken: %s\r\n", key.c_str());
									}
								// Если дешифровать данные не надо
								} else {
									// Если хэш md5 совпадает
									if(md5.empty() || (this->md5(data).compare(md5) == 0)){
										// Выполняем обновление полученного буфера
										value.assign(data.begin(), data.end());
									// Если хэш не совпадает выводим сообщение об ошибке
									} else printf("Data is broken: %s\r\n", key.c_str());
								}
							}
						}
						// Закрываем файл
						this->close();
					}
				}
				// Выводим результат
				return result;
			}
		public:
			/**
			 * getValues Метод извлечения списка значений
			 * @param key   ключ записи
			 * @param value полученное значение ключа
			 * @return      результат операции (количество прочитанных байт)
			 */
			template <class Container>
			const int getValues(const string & key, Container & value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty()){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись найдена
					if(this->keys.count(idw) > 0){
						// Получаем позицию записи
						const size_t pos = this->keys.at(idw);
						// Открываем файл на чтение
						this->open(method_t::read);
						// Выполняем чтение данных из файла
						result = getvals(this->address + pos, this->ifs, value);
						// Закрываем файл
						this->close();
					}
				}
				// Выводим результат
				return result;
			}
			/**
			 * getStrings Метод извлечения списка строковых значений
			 * @param key   ключ записи
			 * @param value полученное значение ключа
			 * @return      результат операции (количество прочитанных байт)
			 */
			template <class Container>
			const int getStrings(const string & key, Container & value) noexcept {
				// Результат работы функции
				int result = -1;
				// Если ключ передан
				if(!key.empty()){
					// Получаем идентификатор ключа
					const size_t idw = this->idw(key);
					// Если текущая запись найдена
					if(this->keys.count(idw) > 0){
						// Получаем позицию записи
						const size_t pos = this->keys.at(idw);
						// Открываем файл на чтение
						this->open(method_t::read);
						// Выполняем чтение данных из файла
						result = getstrs(this->address + pos, this->ifs, value);
						// Закрываем файл
						this->close();
					}
				}
				// Выводим результат
				return result;
			}
		public:
			/**
			 * clear Метод очистки всех данных
			 */
			void clear() noexcept {
				// Закрываем открытый файл
				this->close();
				// Очищаем список ключей
				this->keys.clear();
				// Очищаем позицию raw данных
				this->rawpos = -1;
				// Смещаем указатель начала данных
				this->address = strlen(HEADER);
			}
			/**
			 * setAES Метод установки размера шифрования
			 * @param aeslen размер шифрования (128, 192, 256)
			 */
			void setAES(types_t aeslen) noexcept {
				// Устанавливаем размер шифрования
				this->aeslen = aeslen;
			}
			/**
			 * setRoundAES Метод установки количества раундов шифрования
			 * @param round количество раундов шифрования
			 */
			void setRoundAES(const int round) noexcept {
				// Устанавливаем количество раундов шифрования
				this->roundsAES = round;
			}
			/**
			 * setSalt Метод установки соли шифрования
			 * @param salt соль для шифрования
			 */
			void setSalt(const string & salt) noexcept {
				// Если соль передана
				if(!salt.empty()) this->salt = move(salt);
			}
			/**
			 * setPassword Метод установки пароля шифрования
			 * @param password пароль шифрования
			 */
			void setPassword(const string & password) noexcept {
				// Если пароль передан
				if(!password.empty()) this->password = move(password);
			}
			/**
			 * setFilename Метод установки имени файла базы
			 * @param filename адрес файла базы
			 */
			void setFilename(const string & filename) noexcept {
				// Если файл получен
				if(!filename.empty()){
					// Ищем расширение файла
					size_t pos = filename.rfind(".");
					// Получаем длину всего адреса
					const size_t length = filename.length();
					// Если позиция найдена
					if(pos != string::npos){
						// Извлекаем позицию точки
						this->ext = filename.substr(pos + 1, length - (pos + 1));
						// Запоминаем название файла без расширения
						this->name = filename.substr(0, pos);
						// Ищем название файла
						pos = this->name.rfind("/");
						// Если позиция разделителя найдена
						if(pos != string::npos){
							// Запоминаем путь к файлу
							this->path = this->name.substr(0, pos);
							// Запоминаем название файла без расширения
							this->name = this->name.substr(pos + 1, this->name.length() - (pos + 1));
						}
					// Если расширение не найдено
					} else {
						// Запоминаем название файла без расширения
						this->name = filename;
						// Ищем название файла
						pos = this->name.rfind("/");
						// Если позиция разделителя найдена
						if(pos != string::npos){
							// Запоминаем путь к файлу
							this->path = this->name.substr(0, pos);
							// Запоминаем название файла без расширения
							this->name = this->name.substr(pos + 1, this->name.length() - (pos + 1));
						}
					}
				}
			}
		public:
			/**
			 * ASpl конструктор
			 */
			ASpl() noexcept {
				// Выполняем инициализацию
				this->init();
			}
			/**
			 * ASpl конструктор
			 * @param filename адрес файла базы
			 */
			ASpl(const string & filename) noexcept {
				// Выполняем инициализацию
				this->init();
				// Устанавливаем файл базы
				this->setFilename(filename);
			}
			/**
			 * ASpl конструктор
			 * @param filename адрес файла базы
			 * @param password пароль шифрования
			 */
			ASpl(const string & filename, const string & password) noexcept {
				// Выполняем инициализацию
				this->init();
				// Устанавливаем файл базы
				this->setFilename(filename);
				// Устанавливаем пароль шифрования
				this->setPassword(password);
			}
			/**
			 * ASpl конструктор
			 * @param filename адрес файла базы
			 * @param password пароль шифрования
			 * @param salt     соль для шифрования
			 */
			ASpl(const string & filename, const string & password, const string & salt) noexcept {
				// Выполняем инициализацию
				this->init();
				// Устанавливаем соль для шифрования
				this->setSalt(salt);
				// Устанавливаем файл базы
				this->setFilename(filename);
				// Устанавливаем пароль шифрования
				this->setPassword(password);
			}
			/**
			 * ~ASpl деструктор
			 */
			~ASpl() noexcept {
				// Очищаем все данные
				this->clear();
			}
	} aspl_t;
};

#endif // _ANYKS_ASPL_

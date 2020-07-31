/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef __ANYKS_COLLECTOR__
#define __ANYKS_COLLECTOR__

/**
 * Стандартная библиотека
 */
#include <cmath>
#include <mutex>
#include <ctime>
#include <regex>
#include <atomic>
#include <vector>
/**
 * Наши модули
 */
#include <toolkit.hpp>
#include <progress.hpp>
#include <threadpool.hpp>

using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Collector Класс для с общучением в многопоточном режиме
	 */
	typedef class Collector {
		private:
			// Прогресс бар
			progress_t pss;
			// Местоназначение
			string destination = "";
		private:
			// Общий размер считанных данных
			atomic <double> allSize{0};
			// Текущий статус прогресс-бара
			atomic <u_short> status{0};
			// Прошлый статус прогресс-бара
			atomic <u_short> rate{101};
		private:
			// Мютекс для блокировки потока
			mutex locker;
			// Тип режима отладки
			u_short debug = 0;
			// Общий размер n-граммы
			u_short order = 1;
			// Флаг проведения сегментации
			bool segments = false;
			// Флаг вывода промежуточных результатов
			bool intermed = false;
			// Общий размер данных
			uintmax_t dataSize = 0;
			// Размер сегмента в байтах
			uintmax_t segmentSize = 0;
			// Количество потоков для работы
			size_t threads = thread::hardware_concurrency();
		private:
			// Создаем тредпул
			tpool_t * tpool = nullptr;
			// Объект работы с python
			python_t * python = nullptr;
			// Объект тулкита
			toolkit_t * toolkit = nullptr;
			// Объект log файла
			const char * logfile = nullptr;
			// Объект алфавита
			const alphabet_t * alphabet = nullptr;
			// Объект токенизатора
			const tokenizer_t * tokenizer = nullptr;
		private:
			/**
			 * start Метод инициализации работы тредпула
			 */
			void start();
			/**
			 * finish Метод завершения работы тредпула
			 */
			void finish();
			/**
			 * dumpRaw Метод дампа промежуточных данных
			 */
			void dumpRaw();
			/**
			 * initPython Метод инициализации модуля питона
			 */
			void initPython();
			/**
			 * createDir Метод создания каталога для сохранения результата
			 * @return адрес созданного каталога
			 */
			const string & createDir() const noexcept;
			/**
			 * getSize Функция получения размера в байтах
			 * @param  str размер сегмента (b, kb, Mb, Gb)
			 * @return     размер сегмента в байтах
			 */
			const long getSize(const string & str) const noexcept;
			/**
			 * train Обучения полученного текста
			 * @param filename файл для чтения
			 * @param idd      идентификатор документа
			 */
			void train(const string & filename, const size_t idd) noexcept;
			/**
			 * train Обучения полученного текста
			 * @param texts список строк текста для обучения
			 * @param idd   идентификатор документа
			 */
			void train(const vector <string> & texts, const size_t idd) noexcept;
		public:
			/**
			 * setOrder Метод установки размер n-граммы
			 * @param order размер n-граммы
			 */
			void setOrder(const u_short order) noexcept;
			/**
			 * setDebug Метод установки флага отладки
			 * @param debug флаг отладки
			 */
			void setDebug(const u_short debug) noexcept;
			/**
			 * setIntermed Метод установки флага вывода промежуточных результатов
			 * @param flag значение флага для устаноки
			 */
			void setIntermed(const bool flag) noexcept;
			/**
			 * setToolkit Метод установки объекта тулкита
			 * @param toolkit объект тулкита
			 */
			void setToolkit(toolkit_t * toolkit) noexcept;
			/**
			 * setLogfile Метод установки файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile) noexcept;
			/**
			 * setDest Метод установки место назначения для сохранения сырых данных
			 * @param destination местоназначения для сырых данных
			 */
			void setDest(const string & destination) noexcept;
			/**
			 * setThreads Метод установки количества потоков
			 * @param threads количество потоков для работы
			 */
			void setThreads(const size_t threads = 0) noexcept;
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet) noexcept;
			/**
			 * setTokenizer Метод установки токенизатора
			 * @param tokenizer объект токенизатора
			 */
			void setTokenizer(const tokenizer_t * tokenizer) noexcept;
			/**
			 * setSegment Метод установки параметров сегментации
			 * @param segments разрешить выполнять сегментацию файлов
			 * @param size     размер каждого сегмента (auto - получить размер автоматически)
			 */
			void setSegment(const bool segments, const string & size = "auto") noexcept;
		public:
			/**
			 * readFile Метод запуска чтение текстового корпуса из одного файла
			 * @param filename адрес файла для чтения
			 */
			void readFile(const string & filename) noexcept;
			/**
			 * readDir Метод запуска чтение текстового корпуса из каталога
			 * @param path адрес каталога для чтения
			 * @param ext  расширение файла для поиска в каталоге
			 */
			void readDir(const string & path, const string & ext) noexcept;
		public:
			/**
			 * Конструктор
			 */
			Collector();
			/**
			 * Конструктор
			 * @param toolkit  объект тулкита
			 * @param alphabet объект алфавита
			 * @param alphabet объект токенизатора
			 * @param logifle  адрес файла для вывода отладочной информации
			 */
			Collector(toolkit_t * toolkit, const alphabet_t * alphabet, const tokenizer_t * tokenizer, const char * logfile);
			/**
			 * Деструктор
			 */
			~Collector() noexcept;
	} collector_t;
};

#endif // __ANYKS_COLLECTOR__

/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#ifndef _ANYKS_ARPA_
#define _ANYKS_ARPA_

/**
 * Стандартная библиотека
 */
#include <map>
#include <set>
#include <list>
#include <cmath>
#include <ctime>
#include <bitset>
#include <vector>
#include <limits>
#include <fstream>
#include <functional>
#include <unordered_map>
#include <sys/stat.h>
/**
 * Наши модули
 */
#include <word.hpp>
#include <general.hpp>
#include <alphabet.hpp>
#include <tokenizer.hpp>

// Устанавливаем область видимости
using namespace std;

/**
 * anyks пространство имён
 */
namespace anyks {
	/**
	 * Arpa Класс обучения языковой модели
	 */
	typedef class Arpa {
		public:
			// Основные опции
			enum class options_t : u_short {
				debug,      // Флаг режима отладки
				allGrams,   // Флаг обработки всех n-грамм
				lowerCase,  // Флаг работы только с нижним регистром
				interpolate // Флаг активации режима интерполирования
			};
		private:
			// Объявляем дружественные классы
			friend class KneserNey;
			friend class AddSmooth;
			friend class WittenBell;
			friend class GoodTuring;
			friend class ModKneserNey;
			friend class ConstDiscount;
			friend class NaturalDiscount;
		public:
			// Упрощаем тип функции для получения слова
			typedef function <const word_t * (const size_t)> words_t;
		private:
			// Максимальная длина n-граммы
			static constexpr u_short MAXSIZE = 9;
			// Значением постоянного эпсилон
			static constexpr double EPSILON = 3e-06;
		private:
			// Нулевое значение логорифма
			const float zero = log(0);
			// Псевдо ноль в системе
			const float pseudoZero = log10(1.0893313439140193e-99);
			// Список минимально-возможных встречаемостей для n-грамм
			const u_short minCount[MAXSIZE + 1] = {1, 1, 1, 2, 2, 2, 2, 2, 2, 2};
			// Список максимально-возможных встречаемостей для n-грамм
			const u_short maxCount[MAXSIZE + 1] = {5, 1, 7, 7, 7, 7, 7, 7, 7, 7};
		public:
			/**
			 * Структура слова для карты последовательности
			 */
			typedef struct Seq {
				/**
				 * Частота n-граммы и обратная частота
				 */
				float weight, backoff;
				/**
				 * Идентификатор слова, встречаемость n-граммы,
				 * количество документов где встретилась n-грамма,
				 * значение регистров слова
				 */
				size_t idw, oc, dc, ups;
				/**
				 * Seq Конструктор
				 */
				Seq() : idw(noID), oc(0), dc(0), ups(0), weight(0.0f), backoff(0.0f) {}
			} __attribute__((packed)) seq_t;
		private:
			/**
			 * Структура параметров расчёта
			 */
			typedef struct Param {
				// Общая вероятность n-граммы
				double prob;
				// Встречаемость всех n-грамм
				size_t total;
				// Количество предсказаний, предсказывающие псевдо-события
				size_t nonevent;
				// Количество реальных слов
				size_t observed;
				// Встречаемость больше 2-х
				size_t min2Vocab;
				// Встречаемость больше 3-х раз
				size_t min3Vocab;
				// Количество проб со скидкой до нуля
				size_t discounted;
				/**
				 * Конструктор
				 */
				Param() : prob(0.0), total(0), nonevent(0), observed(0), min2Vocab(0), min3Vocab(0), discounted(0) {}
			} __attribute__((packed)) param_t;
			/**
			 * Структура словаря языковой модели временного словаря
			 */
			typedef struct Vocab: std::map <size_t, Vocab> {
				/**
				 * Родительский объект
				 */
				const Vocab * father;
				/**
				 * Частота последовательности и
				 * обратная частота последовательности
				 */
				float weight, backoff;
				/**
				 * Встречаемость n-граммы,
				 * количество документов где встретилась n-грамма,
				 * идентификатор слова в n-грамме,
				 * идентификатор последнего документа
				 */
				size_t oc, dc, idw, idd;
				/**
				 * Список позиций букв в верхнем регистре
				 */
				std::map <size_t, size_t> uppers;
				/**
				 * Vocab Конструктор
				 */
				Vocab() : oc(0), dc(0), idw(noID), idd(noID), weight(0.0f), backoff(0.0f), father(nullptr) {}
			} vocab_t;
		private:
			// Максимальный размер n-граммы
			u_short size = DEFNGRAM;
			// Размер текущей n-граммы
			mutable u_short gram = 1;
			// Объект log файла
			const char * logfile = nullptr;
		private:
			// Флаги параметров
			bitset <4> options;
			// Параметры расчёта
			mutable param_t param;
			// Словарь языковой модели
			mutable vocab_t vocab;
			// Блок шагов (список дочерних грамм для текущей n-граммы)
			mutable std::map <u_short, list <vocab_t *>> ngrams;
		private:
			// Функция извлечения слова по его идентификатору
			words_t getWord = nullptr;
			// Объект основного алфавита
			const alphabet_t * alphabet = nullptr;
		protected:
			/**
			 * use Метод получения текущего размеры n-граммы
			 * @return текущий размер n-граммы
			 */
			const u_short use() const;
			/**
			 * unigrams Метод получения количества частотных юниграмм
			 * @return количество юниграмм
			 */
			const size_t unigrams() const;
			/**
			 * midw Метод извлечения идентификатора первого слова в n-грамме
			 * @param context контекст которому принадлежит слово
			 * @return        идентификатор первого слова в n-грамме
			 */
			const size_t midw(const vocab_t * context) const;
		protected:
			/**
			 * uppers Метод извлечения регистра слова
			 * @param context контекст n-граммы
			 * @return        регистр слова
			 */
			const pair_t uppers(const vocab_t * context) const;
			/**
			 * uppers Метод извлечения регистра слова
			 * @param  ups список регистров для выбора
			 * @param  oc  встречаемость граммы
			 * @return     регистр слова
			 */
			const pair_t uppers(const std::map <size_t, size_t> & ups, const size_t oc = 0) const;
		protected:
			/**
			 * weights Метод определения суммы весов всех дочерних грамм
			 * @param ngram контекст для расчёта
			 * @return      сумма всех весов дочерних грамм
			 */
			const float weights(const vocab_t * ngram) const;
			/**
			 * backoff Метод подсчёта обратной частоты n-граммы
			 * @param idw     идентификатор текущего слова
			 * @param context контекст которому принадлежит слово
			 * @param gram    размер n-граммы для отката
			 * @return        результат расчёта обратной частоты
			 */
			const float backoff(const size_t idw, const vocab_t * context, const u_short gram) const;
		protected:
			/**
			 * isOption Метод проверки наличия опции
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isOption(const options_t option) const;
			/**
			 * isContext Метод проверки сущестования контекста
			 * @param  seq последовательность для проверки
			 * @return     результат проверки
			 */
			const bool isContext(const vector <size_t> & seq) const;
			/**
			 * backoffs Метод для расчёта частот отката
			 * @param gram  размер n-граммы для работы
			 * @param ngram контекст для расчёта
			 * @return      результат расчёта
			 */
			const bool backoffs(const u_short gram, vocab_t * ngram) const;
			/**
			 * checkIdw Метод поиска наличия слова в корпусе
			 * @param idw  идентификатор слова для поиска (кроме юниграммы)
			 * @param gram размер n-граммы для работы
			 * @return     результат проверки
			 */
			const bool checkIdw(const size_t idw, const u_short gram) const;
			/**
			 * compute Метод расчёта коэффициентов для распределения весов в рамках 1.0
			 * @param ngram       контекст для расчёта
			 * @param gram        размер n-граммы для работы
			 * @param numerator   разность частот n-грамм
			 * @param denominator разность частот отката n-грамм
			 */
			const bool compute(vocab_t * ngram, const u_short gram, double & numerator, double & denominator) const;
		protected:
			/**
			 * contextFn Метод получения контекста
			 * @param context контекст n-граммы
			 * @return        текст контекста n-граммы
			 */
			const string context(const vocab_t * context) const;
			/**
			 * word Метод извлечения слова по его идентификатору
			 * @param idw идентификатор слова
			 * @param ups регистры слова
			 * @return    слово соответствующее идентификатору
			 */
			const string word(const size_t idw, const size_t ups = 0) const;
		protected:
			/**
			 * distribute Метод перерасчёта частоты
			 * @param mass  разность частот n-грамм
			 */
			void distribute(const double mass) const;
			/**
			 * fixupProbs Метод фиксации неверных частот
			 * @param gram размер n-граммы для работы
			 *
			 * Вставляем вероятности для всех контекстных грамм
			 * Формат ARPA заставляет нас иметь вероятность для каждой контекстной граммы.
			 * Таким образом, мы создаём его, если необходимо,
			 * и устанавливаем такую же вероятность,
			 * как и алгоритм отката (чтобы не изменять распределение).
			 */
			void fixupProbs(const u_short gram) const;
			/**
			 * Метод получения регистров слов для юниграммы
			 * @param idw    идентификатор слова для которого требуется получить регистры
			 * @param uppers список регистров слова
			 */
			void uniUppers(const size_t idw, std::set <size_t> & uppers) const;
			/**
			 * get Метод извлечения списка n-грамм указанного размера
			 * @param gram   размер n-граммы список грамм которой нужно извлечь
			 * @param ngrams указатель на список запрашиваемых n-грамм
			 */
			void get(const u_short gram, list <vocab_t *> * ngrams = nullptr) const;
		private:
			/**
			 * nodiscount Метод проверки на необходимость расчёта скидки
			 * @return результат проверки
			 */
			virtual const bool nodiscount() const;
			/**
			 * prepare Метод разбора встречаемостей
			 * @param gram максимальный размер n-граммы
			 */
			virtual void prepare(const u_short gram) const;
			/**
			 * estimate Метод оценки
			 * @param  gram максимальный размер n-граммы
			 * @return       результат оценки
			 */
			virtual const bool estimate(const u_short gram) const;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			virtual const double discount(const size_t count, const size_t total, const size_t observed) const;
			/**
			 * lowerWeight Метод расчёта интерполяции
			 * @param  total     общее количество встречаемости всех n-грамм
			 * @param  observed  количество существующих n-грамм
			 * @param  min2Vocab встречаемость больше 2-х раз
			 * @param  min3Vocab встречаемость больше 3-х раз
			 * @return           результат расчёта интерполяции
			 */
			virtual const double lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const;
		public:
			/**
			 * stamp Метод вывода штампа файла
			 * @return строка с данными штампа
			 */
			const string stamp() const;
			/**
			 * event Метод проверки на спец-слово
			 * @param idw идентификатор слова для проверки
			 * @return    результат проверки
			 */
			const bool event(const size_t idw) const;
			/**
			 * emplace Метод добавления последовательности в словарь
			 * @param seq    список идентификаторов слов которые нужно добавить
			 * @param weight вес n-граммы из файла arpa
			 */
			const bool emplace(const vector <pair_t> & seq, const float weight) const;
			/**
			 * rep Метод замны одной последовательности на другую
			 * @param seq1 последовательность которую нужно заменить
			 * @param seq2 последовательность на которую нужно зменить
			 */
			const bool replace(const vector <size_t> & seq1, const vector <pair_t> & seq2) const;
			/**
			 * count Количество грамм для указанного размера n-граммы
			 * @param  gram размер n-граммы для которой нужно получить количество грамм
			 * @param  real выводить реальное количество грамм
			 * @return      количество грамм для указанного размера n-граммы
			 */
			const size_t count(const u_short gram = 1, const bool real = false) const;
		public:
			/**
			 * clear Метод очистки словаря
			 */
			void clear();
			/**
			 * setWordMethod Метод установки функции получения слова
			 * @param word функция получения слова
			 */
			void setWordMethod(words_t word);
			/**
			 * setSize Метод установки максимального размера n-граммы
			 * @param size максимальный размер n-граммы
			 */
			void setSize(const u_short size);
			/**
			 * setLogfile Метод установка файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile);
			/**
			 * setOption Метод установки опций модуля
			 * @param option опция для установки
			 */
			void setOption(const options_t option);
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet);
			/**
			 * del Метод удаления последовательности
			 * @param seq последовательность слов для удаления
			 */
			void del(const vector <size_t> & seq) const;
			/**
			 * inc Метод увеличения последовательности в словаре
			 * @param seq   список идентификаторов слов которые нужно увеличить
			 * @param value значение на которое нужно увеличить вес n-граммы
			 */
			void inc(const vector <pair_t> & seq, const float value) const;
			/**
			 * load Метод загрузки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 * @param arpa   нужно добавить только данные arpa
			 */
			void load(const vector <char> & buffer, const bool arpa = false) const;
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq последовательность слов для установки
			 */
			void set(const vector <seq_t> & seq) const;
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq список идентификаторов слов которые нужно добавить
			 * @param oc  встречаемость последовательности
			 * @param dc  количество документов где встретилась последовательность
			 */
			void set(const vector <pair_t> & seq, const size_t oc, const size_t dc) const;
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq     список идентификаторов слов которые нужно добавить
			 * @param weight  вес n-граммы из файла arpa
			 * @param backoff обратная частота документа из файла arpa
			 */
			void set(const vector <pair_t> & seq, const float weight, const float backoff) const;
			/**
			 * add Метод добавления последовательности в словарь
			 * @param seq список идентификаторов слов которые нужно добавить
			 * @param idd идентификатор документа в котором получена n-грамма
			 */
			void add(const vector <pair_t> & seq, const size_t idd = 0) const;
			/**
			 * add Метод добавления последовательности в словарь
			 * @param seq     список идентификаторов слов которые нужно добавить
			 * @param weight  вес n-граммы из файла arpa
			 * @param backoff обратная частота документа из файла arpa
			 */
			void add(const vector <pair_t> & seq, const float weight, const float backoff) const;
			/**
			 * add Метод добавления последовательности в словарь
			 * @param seq  последовательность слов для установки
			 * @param idd  идентификатор документа в котором получена n-грамма
			 * @param rest необходимо сделать переоценку встречаемости (необходимо если объединяются две карты с разными размерами n-грамм)
			 */
			void add(const vector <seq_t> & seq, const size_t idd = 0, const bool rest = false) const;
			/**
			 * prune Метод прунинга языковой модели
			 * @param threshold порог частоты прунинга
			 * @param mingram   значение минимальной n-граммы за которую нельзя прунить
			 * @param status    функция вывода статуса обучения
			 */
			void prune(const double threshold = 0.003, const u_short mingram = 0, function <void (const u_short)> status = nullptr) const;
			/**
			 * sweep Метод удаления низкочастотных n-грамм arpa
			 * @param status статус расёта
			 */
			void sweep(function <void (const u_short)> status = nullptr) const;
			/**
			 * trian Метод расёта частот n-грамм
			 * @param status статус расёта
			 */
			void train(function <void (const u_short)> status = nullptr) const;
			/**
			 * repair Метод ремонта уже расчитанной ранее arpa
			 * @param status статус расёта
			 */
			void repair(function <void (const u_short)> status = nullptr) const;
			/**
			 * data Метод извлечения данных arpa
			 * @param gram     размер n-граммы для извлечения
			 * @param callback функция обратного вызова
			 */
			void data(const u_short gram, function <void (const string &)> callback) const;
			/**
			 * grams Метод извлечения данных n-грамм в текстовом виде
			 * @param gram     размер n-граммы для извлечения
			 * @param callback функция обратного вызова
			 */
			void grams(const u_short gram, function <void (const string &)> callback) const;
			/**
			 * save Метод извлечения данных arpa в бинарном виде
			 * @param arpa     флаг извлечения только arpa
			 * @param callback функция обратного вызова
			 */
			void save(const bool arpa, function <void (const vector <char> &, const u_short)> callback) const;
			/**
			 * map Метод извлечения карты последовательностей в виде CSV
			 * @param callback функция обратного вызова
			 * @param delim    разделитель последовательностей
			 */
			void map(function <void (const string &, const u_short)> callback, const string & delim = "|") const;
		public:
			/**
			 * Arpa Конструктор
			 */
			Arpa(){};
			/**
			 * Arpa Конструктор
			 * @param word функция получения слова
			 */
			Arpa(words_t word);
			/**
			 * Arpa Конструктор
			 * @param alphabet объект алфавита
			 */
			Arpa(const alphabet_t * alphabet);
			/**
			 * Arpa Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 */
			Arpa(const alphabet_t * alphabet, words_t word);
			/**
			 * ~Arpa Деструктор
			 */
			~Arpa();
	} arpa_t;
	/**
	 * Класс GoodTuring
	 */
	class GoodTuring: public Arpa {
		private:
			// Размер минимальной встречаемости
			mutable unordered_map <u_short, size_t> minCountTuring;
			// Размер максимальной встречаемости
			mutable unordered_map <u_short, size_t> maxCountTuring;
			// Кэш коэффициентов дисконтирования
			mutable unordered_map <u_short, vector <double>> discountCoeffs;
		private:
			/**
			 * init Метод инициализации первоначальных данных
			 */
			void init() const;
			/**
			 * nodiscount Метод проверки на необходимость расчёта скидки
			 * @return результат проверки
			 */
			const bool nodiscount() const;
			/**
			 * estimate Метод оценки
			 * @param  gram максимальный размер n-граммы
			 * @return      результат оценки
			 */
			const bool estimate(const u_short gram) const;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const;
		public:
			/**
			 * GoodTuring Конструктор
			 * @param word функция получения слова
			 */
			GoodTuring(words_t word);
			/**
			 * GoodTuring Конструктор
			 * @param alphabet объект алфавита
			 */
			GoodTuring(const alphabet_t * alphabet);
			/**
			 * GoodTuring Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 */
			GoodTuring(const alphabet_t * alphabet, words_t word);
	};
	/**
	 * Класс ConstDiscount
	 */
	class ConstDiscount: public Arpa {
		private:
			// Значение дисконтирования
			mutable unordered_map <u_short, double> cdiscount;
		private:
			/**
			 * nodiscount Метод проверки на необходимость расчёта скидки
			 * @return результат проверки
			 */
			const bool nodiscount() const;
			/**
			 * init Метод инициализации первоначальных данных
			 * @param discount значение дискаунта для расчёта
			 */
			void init(const double discount) const;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const;
			/**
			 * lowerWeight Метод расчёта интерполяции
			 * @param  total     общее количество встречаемости всех n-грамм
			 * @param  observed  количество существующих n-грамм
			 * @param  min2Vocab встречаемость больше 2-х раз
			 * @param  min3Vocab встречаемость больше 3-х раз
			 * @return           результат расчёта интерполяции
			 */
			const double lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const;
		public:
			/**
			 * ConstDiscount Конструктор
			 * @param discount значение дисконтирования
			 */
			ConstDiscount(const double discount = 0.3);
			/**
			 * ConstDiscount Конструктор
			 * @param word     функция получения слова
			 * @param discount значение дисконтирования
			 */
			ConstDiscount(words_t word, const double discount = 0.3);
			/**
			 * ConstDiscount Конструктор
			 * @param alphabet объект алфавита
			 * @param discount значение дисконтирования
			 */
			ConstDiscount(const alphabet_t * alphabet, const double discount = 0.3);
			/**
			 * ConstDiscount Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 * @param discount значение дисконтирования
			 */
			ConstDiscount(const alphabet_t * alphabet, words_t word, const double discount = 0.3);
	};
	/**
	 * Класс NaturalDiscount
	 */
	class NaturalDiscount: public Arpa {
		private:
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const;
		public:
			/**
			 * NaturalDiscount Конструктор
			 * @param word функция получения слова
			 */
			NaturalDiscount(words_t word) : Arpa(word) {};
			/**
			 * NaturalDiscount Конструктор
			 * @param alphabet объект алфавита
			 */
			NaturalDiscount(const alphabet_t * alphabet) : Arpa(alphabet) {};
			/**
			 * NaturalDiscount Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 */
			NaturalDiscount(const alphabet_t * alphabet, words_t word) : Arpa(alphabet, word) {};
	};
	/**
	 * Класс AddSmooth
	 */
	class AddSmooth: public Arpa {
		private:
			// Значение дельты
			mutable unordered_map <u_short, double> delta;
		private:
			/**
			 * nodiscount Метод проверки на необходимость расчёта скидки
			 * @return результат проверки
			 */
			const bool nodiscount() const;
			/**
			 * init Метод инициализации первоначальных данных
			 * @param delta значение дельты для расчёта
			 */
			void init(const double delta) const;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const;
		public:
			/**
			 * AddSmooth Конструктор
			 * @param delta значение дельты для расчёта
			 */
			AddSmooth(const double delta = 1.0);
			/**
			 * AddSmooth Конструктор
			 * @param word  функция получения слова
			 * @param delta значение дельты для расчёта
			 */
			AddSmooth(words_t word, const double delta = 1.0);
			/**
			 * AddSmooth Конструктор
			 * @param alphabet объект алфавита
			 * @param delta    значение дельты для расчёта
			 */
			AddSmooth(const alphabet_t * alphabet, const double delta = 1.0);
			/**
			 * AddSmooth Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 * @param delta    значение дельты для расчёта
			 */
			AddSmooth(const alphabet_t * alphabet, words_t word, const double delta = 1.0);
	};
	/**
	 * Класс AddSmooth
	 */
	class WittenBell: public Arpa {
		private:
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const;
			/**
			 * lowerWeight Метод расчёта интерполяции
			 * @param  total     общее количество встречаемости всех n-грамм
			 * @param  observed  количество существующих n-грамм
			 * @param  min2Vocab встречаемость больше 2-х раз
			 * @param  min3Vocab встречаемость больше 3-х раз
			 * @return           результат расчёта интерполяции
			 */
			const double lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const;
		public:
			/**
			 * WittenBell Конструктор
			 * @param word функция получения слова
			 */
			WittenBell(words_t word) : Arpa(word) {};
			/**
			 * WittenBell Конструктор
			 * @param alphabet объект алфавита
			 */
			WittenBell(const alphabet_t * alphabet) : Arpa(alphabet) {};
			/**
			 * WittenBell Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 */
			WittenBell(const alphabet_t * alphabet, words_t word) : Arpa(alphabet, word) {};
	};
	/**
	 * Класс KneserNey
	 */
	class KneserNey: public Arpa {
		private:
			// Объявляем дружественный класс
			friend class ModKneserNey;
		private:
			// Количество уже изменённых младших заказов
			mutable unordered_map <u_short, bool> modified;
			// Необходимость изменения счёта, после вычисления
			mutable unordered_map <u_short, bool> prepares;
			// Значение скидки
			mutable unordered_map <u_short, double> discounting;
		private:
			/**
			 * init Метод инициализации первоначальных данных
			 * @param modified количество уже изменённых младших заказов
			 * @param prepares необходимость изменения счёта, после вычисления
			 */
			void init(const bool modified, const bool prepares) const;
		private:
			/**
			 * prepare Метод разбора встречаемостей
			 * @param gram максимальный размер n-граммы
			 */
			virtual void prepare(const u_short gram) const;
			/**
			 * estimate Метод оценки
			 * @param  gram максимальный размер n-граммы
			 * @return      результат оценки
			 */
			virtual const bool estimate(const u_short gram) const;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			virtual const double discount(const size_t count, const size_t total, const size_t observed) const;
			/**
			 * lowerWeight Метод расчёта интерполяции
			 * @param  total     общее количество встречаемости всех n-грамм
			 * @param  observed  количество существующих n-грамм
			 * @param  min2Vocab встречаемость больше 2-х раз
			 * @param  min3Vocab встречаемость больше 3-х раз
			 * @return           результат расчёта интерполяции
			 */
			virtual const double lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const;
		public:
			/**
			 * KneserNey Конструктор
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			KneserNey(const bool modified = false, const bool prepares = false);
			/**
			 * KneserNey Конструктор
			 * @param word     функция получения слова
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			KneserNey(words_t word, const bool modified = false, const bool prepares = false);
			/**
			 * KneserNey Конструктор
			 * @param alphabet объект алфавита
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			KneserNey(const alphabet_t * alphabet, const bool modified = false, const bool prepares = false);
			/**
			 * KneserNey Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			KneserNey(const alphabet_t * alphabet, words_t word, const bool modified = false, const bool prepares = false);
	};
	/**
	 * Класс ModKneserNey
	 */
	class ModKneserNey: public KneserNey {
		private:
			// Прибавочное значение скидки
			mutable unordered_map <u_short, double> discountplus;
			// Дополнительное значение скидки
			mutable unordered_map <u_short, double> modDiscounting;
		private:
			/**
			 * init Метод инициализации первоначальных данных
			 */
			void init() const;
			/**
			 * estimate Метод оценки
			 * @param  gram максимальный размер n-граммы
			 * @return      результат оценки
			 */
			const bool estimate(const u_short gram) const;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const;
			/**
			 * lowerWeight Метод расчёта интерполяции
			 * @param  total     общее количество встречаемости всех n-грамм
			 * @param  observed  количество существующих n-грамм
			 * @param  min2Vocab встречаемость больше 2-х раз
			 * @param  min3Vocab встречаемость больше 3-х раз
			 * @return           результат расчёта интерполяции
			 */
			const double lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const;
		public:
			/**
			 * ModKneserNey Конструктор
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			ModKneserNey(const bool modified = false, const bool prepares = false);
			/**
			 * ModKneserNey Конструктор
			 * @param word     функция получения слова
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			ModKneserNey(words_t word, const bool modified = false, const bool prepares = false);
			/**
			 * ModKneserNey Конструктор
			 * @param alphabet объект алфавита
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			ModKneserNey(const alphabet_t * alphabet, const bool modified = false, const bool prepares = false);
			/**
			 * ModKneserNey Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			ModKneserNey(const alphabet_t * alphabet, words_t word, const bool modified = false, const bool prepares = false);
	};
};

#endif // _ANYKS_ARPA_

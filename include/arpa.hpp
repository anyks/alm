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
#include <alphabet.hpp>
#include <tokenizer.hpp>
#include <app/alm.hpp>

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
				notTrain,   // Флаг запрещающий выполнять обучние
				resetUnk,   // Флаг сброса частоты неизвестного слова
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
			const double zero = log(0);
			// Псевдо ноль в системе
			const double pseudoZero = log10(1.0893313439140193e-99);
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
				double weight, backoff;
				/**
				 * Идентификатор слова, встречаемость n-граммы,
				 * количество документов где встретилась n-грамма,
				 * значение регистров слова
				 */
				size_t idw, oc, dc, ups;
				/**
				 * Seq Конструктор
				 */
				Seq() : weight(log(0)), backoff(log(0)), idw(idw_t::NIDW), oc(0), dc(0), ups(0) {}
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
			typedef struct Data : std::map <size_t, Data> {
				/**
				 * Родительский объект
				 */
				const Data * father;
				/**
				 * Частота последовательности и
				 * обратная частота последовательности
				 */
				double weight, backoff;
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
				 * Data Конструктор
				 */
				Data() : father(nullptr), weight(log(0)), backoff(log(0)), oc(0), dc(0), idw(idw_t::NIDW), idd(idw_t::NIDW) {}
			} data_t;
		private:
			// Максимальный размер n-граммы
			u_short size = DEFNGRAM;
			// Размер текущей n-граммы
			mutable u_short gram = 1;
		private:
			// Флаги параметров
			bitset <6> options;
			// Словарь языковой модели
			mutable data_t data;
			// Параметры расчёта
			mutable param_t param;
			// Блок шагов (список дочерних грамм для текущей n-граммы)
			mutable std::map <u_short, list <data_t *>> ngrams;
		private:
			// Функция извлечения слова по его идентификатору
			words_t getWord = nullptr;
			// Объект основного алфавита
			const alphabet_t * alphabet = nullptr;
			// Объект log файла
			mutable const char * logfile = nullptr;
		protected:
			/**
			 * use Метод получения текущего размеры n-граммы
			 * @return текущий размер n-граммы
			 */
			const u_short use() const noexcept;
			/**
			 * unigrams Метод получения количества частотных юниграмм
			 * @return количество юниграмм
			 */
			const size_t unigrams() const noexcept;
			/**
			 * midw Метод извлечения идентификатора первого слова в n-грамме
			 * @param context контекст которому принадлежит слово
			 * @return        идентификатор первого слова в n-грамме
			 */
			const size_t midw(const data_t * context) const noexcept;
		protected:
			/**
			 * uppers Метод извлечения регистра слова
			 * @param context контекст n-граммы
			 * @return        регистр слова
			 */
			const pair_t uppers(const data_t * context) const noexcept;
			/**
			 * uppers Метод извлечения регистра слова
			 * @param  ups список регистров для выбора
			 * @param  oc  встречаемость граммы
			 * @return     регистр слова
			 */
			const pair_t uppers(const std::map <size_t, size_t> & ups, const size_t oc = 0) const noexcept;
		protected:
			/**
			 * isUnk Метод проверки неизвестного слова
			 * @param idw идентификатор слова для проверки
			 * @return    результат првоерки слова
			 */
			const bool isUnk(const size_t idw) const noexcept;
			/**
			 * isStart Метод проверки начального слова
			 * @param idw идентификатор слова для проверки
			 * @return    результат првоерки слова
			 */
			const bool isStart(const size_t idw) const noexcept;
			/**
			 * isWeight Метод проверки на валидность веса
			 * @param weight вес для проверки
			 * @return       результат проверки
			 */
			const bool isWeight(const double weight) const noexcept;
			/**
			 * isWeights Метод проверки существования дочерних n-грамм
			 * @param ngram контекст для проверки
			 * @return      результат проверки
			 */
			const bool isWeights(const data_t * ngram) const noexcept;
			/**
			 * isOption Метод проверки наличия опции
			 * @param option опция для проверки
			 * @return       результат проверки
			 */
			const bool isOption(const options_t option) const noexcept;
			/**
			 * isContext Метод проверки сущестования контекста
			 * @param  context контекст для проверки
			 * @return         результат проверки
			 */
			const bool isContext(const data_t * context) const noexcept;
			/**
			 * isContext Метод проверки сущестования контекста
			 * @param  seq последовательность для проверки
			 * @return     результат проверки
			 */
			const bool isContext(const vector <size_t> & seq) const noexcept;
			/**
			 * backoffs Метод для расчёта частот отката
			 * @param gram  размер n-граммы для работы
			 * @param ngram контекст для расчёта
			 * @return      результат расчёта
			 */
			const bool backoffs(const u_short gram, data_t * ngram) const noexcept;
			/**
			 * checkIdw Метод поиска наличия слова в корпусе
			 * @param idw  идентификатор слова для поиска (кроме юниграммы)
			 * @param gram размер n-граммы для работы
			 * @return     результат проверки
			 */
			const bool checkIdw(const size_t idw, const u_short gram) const noexcept;
			/**
			 * backoff Метод подсчёта обратной частоты n-граммы
			 * @param idw     идентификатор текущего слова
			 * @param context контекст которому принадлежит слово
			 * @param gram    размер n-граммы для отката
			 * @return        результат расчёта обратной частоты
			 */
			const double backoff(const size_t idw, const data_t * context, const u_short gram) const noexcept;
			/**
			 * compute Метод расчёта коэффициентов для распределения весов в рамках 1.0
			 * @param ngram       контекст для расчёта
			 * @param gram        размер n-граммы для работы
			 * @param numerator   разность частот n-грамм
			 * @param denominator разность частот отката n-грамм
			 */
			const bool compute(data_t * ngram, const u_short gram, double & numerator, double & denominator) const noexcept;
		protected:
			/**
			 * contextFn Метод получения контекста
			 * @param context контекст n-граммы
			 * @return        текст контекста n-граммы
			 */
			const string context(const data_t * context) const noexcept;
			/**
			 * word Метод извлечения слова по его идентификатору
			 * @param idw идентификатор слова
			 * @param ups регистры слова
			 * @return    слово соответствующее идентификатору
			 */
			const string word(const size_t idw, const size_t ups = 0) const noexcept;
		protected:
			/**
			 * distribute Метод перерасчёта частоты
			 * @param mass  разность частот n-грамм
			 */
			void distribute(const double mass) const noexcept;
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
			void fixupProbs(const u_short gram) const noexcept;
			/**
			 * Метод получения регистров слов
			 * @param uppers список регистров слова
			 */
			void uniUppers(multimap <size_t, size_t> & uppers) const noexcept;
			/**
			 * get Метод извлечения списка n-грамм указанного размера
			 * @param gram   размер n-граммы список грамм которой нужно извлечь
			 * @param ngrams указатель на список запрашиваемых n-грамм
			 */
			void get(const u_short gram, list <data_t *> * ngrams = nullptr) const noexcept;
			/**
			 * sequence Метод извлечения правильной последовательности, игнорирования <unk> токена
			 * @param seq      последовательность для обработки
			 * @param callback функция обратного вызова
			 */
			void sequence(const vector <seq_t> & seq, function <void (const vector <seq_t> &, const seq_t &)> callback) const noexcept;
			/**
			 * sequence Метод извлечения правильной последовательности, игнорирования <unk> токена
			 * @param seq      последовательность для обработки
			 * @param callback функция обратного вызова
			 */
			void sequence(const vector <pair_t> & seq, function <void (const vector <pair_t> &, const size_t)> callback) const noexcept;
		private:
			/**
			 * nodiscount Метод проверки на необходимость расчёта скидки
			 * @return результат проверки
			 */
			virtual const bool nodiscount() const noexcept;
			/**
			 * prepare Метод разбора встречаемостей
			 * @param gram максимальный размер n-граммы
			 */
			virtual void prepare(const u_short gram) const noexcept;
			/**
			 * estimate Метод оценки
			 * @param  gram максимальный размер n-граммы
			 * @return       результат оценки
			 */
			virtual const bool estimate(const u_short gram) const noexcept;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			virtual const double discount(const size_t count, const size_t total, const size_t observed) const noexcept;
			/**
			 * lowerWeight Метод расчёта интерполяции
			 * @param  total     общее количество встречаемости всех n-грамм
			 * @param  observed  количество существующих n-грамм
			 * @param  min2Vocab встречаемость больше 2-х раз
			 * @param  min3Vocab встречаемость больше 3-х раз
			 * @return           результат расчёта интерполяции
			 */
			virtual const double lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const noexcept;
		public:
			/**
			 * stamp Метод вывода штампа файла
			 * @return строка с данными штампа
			 */
			const string stamp() const noexcept;
		public:
			/**
			 * empty Метод проверки языковой модели на пустоту
			 * @return результат проверки
			 */
			const bool empty() const noexcept;
			/**
			 * event Метод проверки на спец-слово
			 * @param idw идентификатор слова для проверки
			 * @return    результат проверки
			 */
			const bool event(const size_t idw) const noexcept;
			/**
			 * emplace Метод добавления последовательности в словарь
			 * @param seq    список идентификаторов слов которые нужно добавить
			 * @param weight вес n-граммы из файла arpa
			 */
			const bool emplace(const vector <pair_t> & seq, const double weight) const noexcept;
			/**
			 * rep Метод замны одной последовательности на другую
			 * @param seq1 последовательность которую нужно заменить
			 * @param seq2 последовательность на которую нужно зменить
			 */
			const bool replace(const vector <size_t> & seq1, const vector <pair_t> & seq2) const noexcept;
		public:
			/**
			 * count Количество грамм для указанного размера n-граммы
			 * @param  gram размер n-граммы для которой нужно получить количество грамм
			 * @param  real выводить реальное количество грамм
			 * @return      количество грамм для указанного размера n-граммы
			 */
			const size_t count(const u_short gram = 1, const bool real = false) const noexcept;
		public:
			/**
			 * clear Метод очистки словаря
			 */
			void clear() noexcept;
			/**
			 * removeWord Метод удаления слова и всех дочерних n-грамм
			 * @param idw идентификатор слова
			 */
			void removeWord(const size_t idw) noexcept;
		public:
			/**
			 * setSize Метод установки максимального размера n-граммы
			 * @param size максимальный размер n-граммы
			 */
			void setSize(const u_short size) noexcept;
			/**
			 * setWordMethod Метод установки функции получения слова
			 * @param word функция получения слова
			 */
			void setWordMethod(words_t word) noexcept;
			/**
			 * setLogfile Метод установки файла для вывода логов
			 * @param logifle адрес файла для вывода отладочной информации
			 */
			void setLogfile(const char * logfile) noexcept;
			/**
			 * setOption Метод установки опций модуля
			 * @param option опция для установки
			 */
			void setOption(const options_t option) noexcept;
			/**
			 * setAlphabet Метод установки алфавита
			 * @param alphabet объект алфавита
			 */
			void setAlphabet(const alphabet_t * alphabet) noexcept;
		public:
			/**
			 * del Метод удаления последовательности
			 * @param seq последовательность слов для удаления
			 */
			void del(const vector <size_t> & seq) const noexcept;
			/**
			 * inc Метод увеличения последовательности в словаре
			 * @param seq   список идентификаторов слов которые нужно увеличить
			 * @param value значение на которое нужно увеличить вес n-граммы
			 */
			void inc(const vector <pair_t> & seq, const double value) const noexcept;
			/**
			 * addBin Метод добавления бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 * @param idd    идентификатор документа в котором получена n-грамма
			 */
			void addBin(const vector <char> & buffer, const size_t idd = 0) const noexcept;
			/**
			 * setBin Метод установки бинарных данных в словарь
			 * @param buffer буфер с бинарными данными
			 * @param arpa   нужно добавить только данные arpa
			 */
			void setBin(const vector <char> & buffer, const bool arpa = false) const noexcept;
			/**
			 * arpa Метод извлечения данных arpa
			 * @param gram     размер n-граммы для извлечения
			 * @param callback функция обратного вызова
			 */
			void arpa(const u_short gram, function <void (const string &)> callback) const noexcept;
			/**
			 * grams Метод извлечения данных n-грамм в текстовом виде
			 * @param gram     размер n-граммы для извлечения
			 * @param callback функция обратного вызова
			 */
			void grams(const u_short gram, function <void (const string &)> callback) const noexcept;
			/**
			 * getBin Метод извлечения данных arpa в бинарном виде
			 * @param arpa     флаг извлечения только arpa
			 * @param callback функция обратного вызова
			 */
			void getBin(const bool arpa, function <void (const vector <char> &, const u_short)> callback) const noexcept;
			/**
			 * map Метод извлечения карты последовательностей в виде CSV
			 * @param callback функция обратного вызова
			 * @param delim    разделитель последовательностей
			 */
			void map(function <void (const string &, const u_short)> callback, const string & delim = "|") const noexcept;
		public:
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq последовательность слов для установки
			 */
			void set(const vector <seq_t> & seq) const noexcept;
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq список идентификаторов слов которые нужно добавить
			 * @param oc  встречаемость последовательности
			 * @param dc  количество документов где встретилась последовательность
			 */
			void set(const vector <pair_t> & seq, const size_t oc, const size_t dc) const noexcept;
			/**
			 * set Метод установки последовательности в словарь
			 * @param seq     список идентификаторов слов которые нужно добавить
			 * @param weight  вес n-граммы из файла arpa
			 * @param backoff обратная частота документа из файла arpa
			 */
			void set(const vector <pair_t> & seq, const double weight, const double backoff) const noexcept;
		public:
			/**
			 * add Метод добавления последовательности в словарь
			 * @param seq список идентификаторов слов которые нужно добавить
			 * @param idd идентификатор документа в котором получена n-грамма
			 */
			void add(const vector <pair_t> & seq, const size_t idd = 0) const noexcept;
			/**
			 * add Метод добавления последовательности в словарь
			 * @param seq  последовательность слов для установки
			 * @param idd  идентификатор документа в котором получена n-грамма
			 * @param rest необходимо сделать переоценку встречаемости (необходимо если объединяются две карты с разными размерами n-грамм)
			 */
			void add(const vector <seq_t> & seq, const size_t idd = 0, const bool rest = false) const noexcept;
		public:
			/**
			 * sweep Метод удаления низкочастотных n-грамм arpa
			 * @param status статус расчёта
			 */
			void sweep(function <void (const u_short)> status = nullptr) const noexcept;
			/**
			 * trian Метод расчёта частот n-грамм
			 * @param status статус расчёта
			 */
			void train(function <void (const u_short)> status = nullptr) const noexcept;
			/**
			 * repair Метод ремонта уже расчитанной ранее языковой модели
			 * @param status статус расчёта
			 */
			void repair(function <void (const u_short)> status = nullptr) const noexcept;
			/**
			 * prune Метод прунинга языковой модели
			 * @param threshold порог частоты прунинга
			 * @param mingram   значение минимальной n-граммы за которую нельзя прунить
			 * @param status    функция вывода статуса обучения
			 */
			void prune(const double threshold = 0.003, const u_short mingram = 0, function <void (const u_short)> status = nullptr) const noexcept;
		public:
			/**
			 * mixForward Метод интерполяции нескольких моделей в прямом направлении
			 * @param lm     данные языковой модели для объединения
			 * @param lambda вес первой модели при интерполяции
			 * @param status статус расчёта
			 */
			void mixForward(const Arpa * lm, const double lambda = 0.5, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * mixBackward Метод интерполяции нескольких моделей в обратном направлении
			 * @param lm     данные языковой модели для объединения
			 * @param lambda вес первой модели при интерполяции
			 * @param status статус расчёта
			 */
			void mixBackward(const Arpa * lm, const double lambda = 0.5, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * mixLoglinear Метод интерполяции нескольких моделей алгоритмом Баеса
			 * @param lms     список данных языковых моделей для объединения
			 * @param lambdas список весов моделей при интерполяции
			 * @param status  статус расчёта
			 */
			void mixLoglinear(const vector <const Arpa *> & lms, const vector <double> & lambdas, function <void (const u_short)> status = nullptr) noexcept;
			/**
			 * mixBayes Метод интерполяции нескольких моделей алгоритмом Баеса
			 * @param lms     список данных языковых моделей для объединения
			 * @param lambdas список весов моделей при интерполяции
			 * @param length  длина контекста для расчёта
			 * @param scale   логарифмическая шкала вероятности
			 * @param status  статус расчёта
			 */
			void mixBayes(const vector <const Arpa *> & lms, const vector <double> & lambdas, const size_t length, const double scale = 0.0, function <void (const u_short)> status = nullptr) noexcept;
		public:
			/**
			 * Arpa Конструктор
			 */
			Arpa() noexcept {};
			/**
			 * Arpa Конструктор
			 * @param word функция получения слова
			 */
			Arpa(words_t word) noexcept;
			/**
			 * Arpa Конструктор
			 * @param alphabet объект алфавита
			 */
			Arpa(const alphabet_t * alphabet) noexcept;
			/**
			 * Arpa Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 */
			Arpa(const alphabet_t * alphabet, words_t word) noexcept;
			/**
			 * ~Arpa Деструктор
			 */
			virtual ~Arpa() noexcept;
	} arpa_t;
	/**
	 * Класс GoodTuring
	 */
	class GoodTuring : public Arpa {
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
			void init() const noexcept;
			/**
			 * nodiscount Метод проверки на необходимость расчёта скидки
			 * @return результат проверки
			 */
			const bool nodiscount() const noexcept;
			/**
			 * estimate Метод оценки
			 * @param  gram максимальный размер n-граммы
			 * @return      результат оценки
			 */
			const bool estimate(const u_short gram) const noexcept;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const noexcept;
		public:
			/**
			 * GoodTuring Конструктор
			 * @param word функция получения слова
			 */
			GoodTuring(words_t word) noexcept;
			/**
			 * GoodTuring Конструктор
			 * @param alphabet объект алфавита
			 */
			GoodTuring(const alphabet_t * alphabet) noexcept;
			/**
			 * GoodTuring Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 */
			GoodTuring(const alphabet_t * alphabet, words_t word) noexcept;
	};
	/**
	 * Класс ConstDiscount
	 */
	class ConstDiscount : public Arpa {
		private:
			// Значение дисконтирования
			mutable unordered_map <u_short, double> cdiscount;
		private:
			/**
			 * nodiscount Метод проверки на необходимость расчёта скидки
			 * @return результат проверки
			 */
			const bool nodiscount() const noexcept;
			/**
			 * init Метод инициализации первоначальных данных
			 * @param discount значение дискаунта для расчёта
			 */
			void init(const double discount) const noexcept;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const noexcept;
			/**
			 * lowerWeight Метод расчёта интерполяции
			 * @param  total     общее количество встречаемости всех n-грамм
			 * @param  observed  количество существующих n-грамм
			 * @param  min2Vocab встречаемость больше 2-х раз
			 * @param  min3Vocab встречаемость больше 3-х раз
			 * @return           результат расчёта интерполяции
			 */
			const double lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const noexcept;
		public:
			/**
			 * ConstDiscount Конструктор
			 * @param discount значение дисконтирования
			 */
			ConstDiscount(const double discount = 0.3) noexcept;
			/**
			 * ConstDiscount Конструктор
			 * @param word     функция получения слова
			 * @param discount значение дисконтирования
			 */
			ConstDiscount(words_t word, const double discount = 0.3) noexcept;
			/**
			 * ConstDiscount Конструктор
			 * @param alphabet объект алфавита
			 * @param discount значение дисконтирования
			 */
			ConstDiscount(const alphabet_t * alphabet, const double discount = 0.3) noexcept;
			/**
			 * ConstDiscount Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 * @param discount значение дисконтирования
			 */
			ConstDiscount(const alphabet_t * alphabet, words_t word, const double discount = 0.3) noexcept;
	};
	/**
	 * Класс NaturalDiscount
	 */
	class NaturalDiscount : public Arpa {
		private:
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const noexcept;
		public:
			/**
			 * NaturalDiscount Конструктор
			 * @param word функция получения слова
			 */
			NaturalDiscount(words_t word) noexcept : Arpa(word) {};
			/**
			 * NaturalDiscount Конструктор
			 * @param alphabet объект алфавита
			 */
			NaturalDiscount(const alphabet_t * alphabet) noexcept : Arpa(alphabet) {};
			/**
			 * NaturalDiscount Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 */
			NaturalDiscount(const alphabet_t * alphabet, words_t word) noexcept : Arpa(alphabet, word) {};
	};
	/**
	 * Класс AddSmooth
	 */
	class AddSmooth : public Arpa {
		private:
			// Значение дельты
			mutable unordered_map <u_short, double> delta;
		private:
			/**
			 * nodiscount Метод проверки на необходимость расчёта скидки
			 * @return результат проверки
			 */
			const bool nodiscount() const noexcept;
			/**
			 * init Метод инициализации первоначальных данных
			 * @param delta значение дельты для расчёта
			 */
			void init(const double delta) const noexcept;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const noexcept;
		public:
			/**
			 * AddSmooth Конструктор
			 * @param delta значение дельты для расчёта
			 */
			AddSmooth(const double delta = 1.0) noexcept;
			/**
			 * AddSmooth Конструктор
			 * @param word  функция получения слова
			 * @param delta значение дельты для расчёта
			 */
			AddSmooth(words_t word, const double delta = 1.0) noexcept;
			/**
			 * AddSmooth Конструктор
			 * @param alphabet объект алфавита
			 * @param delta    значение дельты для расчёта
			 */
			AddSmooth(const alphabet_t * alphabet, const double delta = 1.0) noexcept;
			/**
			 * AddSmooth Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 * @param delta    значение дельты для расчёта
			 */
			AddSmooth(const alphabet_t * alphabet, words_t word, const double delta = 1.0) noexcept;
	};
	/**
	 * Класс AddSmooth
	 */
	class WittenBell : public Arpa {
		private:
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const noexcept;
			/**
			 * lowerWeight Метод расчёта интерполяции
			 * @param  total     общее количество встречаемости всех n-грамм
			 * @param  observed  количество существующих n-грамм
			 * @param  min2Vocab встречаемость больше 2-х раз
			 * @param  min3Vocab встречаемость больше 3-х раз
			 * @return           результат расчёта интерполяции
			 */
			const double lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const noexcept;
		public:
			/**
			 * WittenBell Конструктор
			 * @param word функция получения слова
			 */
			WittenBell(words_t word) noexcept : Arpa(word) {};
			/**
			 * WittenBell Конструктор
			 * @param alphabet объект алфавита
			 */
			WittenBell(const alphabet_t * alphabet) noexcept : Arpa(alphabet) {};
			/**
			 * WittenBell Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 */
			WittenBell(const alphabet_t * alphabet, words_t word) noexcept : Arpa(alphabet, word) {};
	};
	/**
	 * Класс KneserNey
	 */
	class KneserNey : public Arpa {
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
			void init(const bool modified, const bool prepares) const noexcept;
		private:
			/**
			 * prepare Метод разбора встречаемостей
			 * @param gram максимальный размер n-граммы
			 */
			virtual void prepare(const u_short gram) const noexcept;
			/**
			 * estimate Метод оценки
			 * @param  gram максимальный размер n-граммы
			 * @return      результат оценки
			 */
			virtual const bool estimate(const u_short gram) const noexcept;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			virtual const double discount(const size_t count, const size_t total, const size_t observed) const noexcept;
			/**
			 * lowerWeight Метод расчёта интерполяции
			 * @param  total     общее количество встречаемости всех n-грамм
			 * @param  observed  количество существующих n-грамм
			 * @param  min2Vocab встречаемость больше 2-х раз
			 * @param  min3Vocab встречаемость больше 3-х раз
			 * @return           результат расчёта интерполяции
			 */
			virtual const double lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const noexcept;
		public:
			/**
			 * KneserNey Конструктор
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			KneserNey(const bool modified = false, const bool prepares = false) noexcept;
			/**
			 * KneserNey Конструктор
			 * @param word     функция получения слова
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			KneserNey(words_t word, const bool modified = false, const bool prepares = false) noexcept;
			/**
			 * KneserNey Конструктор
			 * @param alphabet объект алфавита
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			KneserNey(const alphabet_t * alphabet, const bool modified = false, const bool prepares = false) noexcept;
			/**
			 * KneserNey Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			KneserNey(const alphabet_t * alphabet, words_t word, const bool modified = false, const bool prepares = false) noexcept;
	};
	/**
	 * Класс ModKneserNey
	 */
	class ModKneserNey : public KneserNey {
		private:
			// Прибавочное значение скидки
			mutable unordered_map <u_short, double> discountplus;
			// Дополнительное значение скидки
			mutable unordered_map <u_short, double> modDiscounting;
		private:
			/**
			 * init Метод инициализации первоначальных данных
			 */
			void init() const noexcept;
			/**
			 * estimate Метод оценки
			 * @param  gram максимальный размер n-граммы
			 * @return      результат оценки
			 */
			const bool estimate(const u_short gram) const noexcept;
			/**
			 * discount Метод расчёта скидки
			 * @param  count    встречаемость n-граммы
			 * @param  total    общее количество встречаемости всех n-грамм
			 * @param  observed количество существующих n-грамм
			 * @return          результат расчёта
			 */
			const double discount(const size_t count, const size_t total, const size_t observed) const noexcept;
			/**
			 * lowerWeight Метод расчёта интерполяции
			 * @param  total     общее количество встречаемости всех n-грамм
			 * @param  observed  количество существующих n-грамм
			 * @param  min2Vocab встречаемость больше 2-х раз
			 * @param  min3Vocab встречаемость больше 3-х раз
			 * @return           результат расчёта интерполяции
			 */
			const double lowerWeight(const size_t total, const size_t observed, const size_t min2Vocab, const size_t min3Vocab) const noexcept;
		public:
			/**
			 * ModKneserNey Конструктор
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			ModKneserNey(const bool modified = false, const bool prepares = false) noexcept;
			/**
			 * ModKneserNey Конструктор
			 * @param word     функция получения слова
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			ModKneserNey(words_t word, const bool modified = false, const bool prepares = false) noexcept;
			/**
			 * ModKneserNey Конструктор
			 * @param alphabet объект алфавита
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			ModKneserNey(const alphabet_t * alphabet, const bool modified = false, const bool prepares = false) noexcept;
			/**
			 * ModKneserNey Конструктор
			 * @param alphabet объект алфавита
			 * @param word     функция получения слова
			 * @param modified модификация количества уже изменённых младших n-грамм
			 * @param prepares необходимость изменения граммности, после вычисления
			 */
			ModKneserNey(const alphabet_t * alphabet, words_t word, const bool modified = false, const bool prepares = false) noexcept;
	};
};

#endif // _ANYKS_ARPA_

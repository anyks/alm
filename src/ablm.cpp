/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <ablm.hpp>

/**
 * flag_t Метод проверки наличия флага
 * @param option опция для проверки
 * @return       результат проверки
 */
const bool anyks::AbLM::isFlag(const flag_t flag) const noexcept {
	// Выполняем проверку наличия флага
	return this->flags.test((u_short) flag);
}
/**
 * write Метод записи данных словаря в файл
 * @param status статус записи данных в бинарный контейнер
 */
const bool anyks::AbLM::write(function <void (const u_short)> status) noexcept {
	// Результат работы функции
	bool result = false;
	// Устанавливаем тип шифрования
	this->aspl->setAES(this->meta.aes);
	// Устанавливаем дату генерации словаря
	this->aspl->set("date", this->meta.date);
	// Устанавливаем название словаря
	this->aspl->set("name", this->meta.name);
	// Устанавливаем имя автора словаря
	this->aspl->set("author", this->meta.author);
	// Устанавливаем тип лицензии
	this->aspl->set("lictype", this->meta.lictype);
	// Устанавливаем текст лицензии
	this->aspl->set("lictext", this->meta.lictext);
	// Устанавливаем контактные данные пользователя
	this->aspl->set("contacts", this->meta.contacts);
	// Устанавливаем копирайт словаря
	this->aspl->set("copyright", this->meta.copyright);
	// Устанавливаем граммность словаря
	this->aspl->set("size", this->toolkit->getSize());
	// Устанавливаем буквы алфавита
	this->aspl->set("alphabet", this->alphabet->get());
	// Устанавливаем неизвестное слово если есть
	this->aspl->set("unknown", this->toolkit->getUnknown());
	// Устанавливаем список суффиксов цифровых аббревиатур
	this->aspl->setValues("abbrs-idw", this->tokenizer->getSuffixes());
	// Устанавливаем список токенов приводимых к <unk>
	this->aspl->setValues("tokensUnknown", this->toolkit->getTokensUnknown());
	// Устанавливаем список запрещённых токенов
	this->aspl->setValues("tokensDisable", this->toolkit->getTokensDisable());
	// Устанавливаем тип шифрования
	if(!this->meta.password.empty()){
		// Если размер шифрования получен
		switch((u_short) this->meta.aes){
			// Если это 128-и битное шифрование
			case (u_short) aspl_t::types_t::aes128: this->aspl->set("aes", (u_short) 128); break;
			// Если это 192-х битное шифрование
			case (u_short) aspl_t::types_t::aes192: this->aspl->set("aes", (u_short) 192); break;
			// Если это 256-и битное шифрование
			case (u_short) aspl_t::types_t::aes256: this->aspl->set("aes", (u_short) 256); break;
		}
	}
	// Устанавливаем значение апострофа
	this->aspl->set("allowApostrophe", this->alphabet->isAllowApostrophe());
	// Если нужно сохранить список аббревиатур
	if(this->isFlag(flag_t::expAbbrevs) || this->isFlag(flag_t::expAllData)){
		// Устанавливаем список аббревиатур
		this->aspl->setValues("abbrs", this->tokenizer->getAbbrs());
	}
	// Если нужно сохранить список доменных зон
	if(this->isFlag(flag_t::expDomZones) || this->isFlag(flag_t::expAllData)){
		// устанавливаем список доменных зон
		const auto & zones = this->alphabet->getzones();
		// Если список зон не пустой
		if(!zones.empty()){
			// Новый список доменных зон
			std::set <string> tmp;
			// Переходим по всем зонам и формирум новый список
			for(auto & zone : zones) tmp.emplace(this->alphabet->convert(zone));
			// Если список получен, сохраняем его
			if(!tmp.empty()) this->aspl->setStrings("dzones", tmp);
		}
	}
	// Если нужно сохранить установленные опции модуля
	if(this->isFlag(flag_t::expOptions) || this->isFlag(flag_t::expAllData)){
		// Устанавливаем опции модуля
		this->aspl->set("options", this->toolkit->getOptions());
	}
	// Если нужно сохранить пользовательские токены
	if(this->isFlag(flag_t::expUserTokens) || this->isFlag(flag_t::expAllData)){
		// Устанавливаем пользовательские токены
		this->aspl->setStrings("utokens", this->toolkit->getUserTokens());
	}
	/**
	 * Блок извлечения списка букв для восстановления слов состоящих из смешанных словарей
	 */
	{
		// Получаем список букв
		auto substitutes = this->alphabet->getSubstitutes();
		// Если список получен
		if(!substitutes.empty()){
			// Список букв для восстановления
			vector <string> letters;
			// Переходим по всему списку
			for(auto & item : substitutes){
				// Сохраняем первую букву
				letters.push_back(item.first);
				// Сохраняем вторую букву
				letters.push_back(item.second);
			}
			// Устанавливаем полученный список букв
			this->aspl->setStrings("substitutes", letters);
		}
	}
	/**
	 * Блок добавления скриптов python
	 */
	{
		/**
		 * Функция добавления скрипта
		 * @param key   ключ для добавления
		 * @param value значение для добавления
		 */
		auto addScript = [this](const string & key, const string & value){
			// Если ключ передан и файл существует
			if(!key.empty() && !value.empty() && fsys_t::isfile(value)){
				// Данные скрипта
				string script = "";
				// Выполняем считывание всех строк скрипта
				fsys_t::rfile(value, [&script](const string & text, const uintmax_t fileSize){
					// Добавляем данные скрипта
					script.append(text);
				});
				// Если скрипт получен, устанавливаем его
				if(!script.empty()) this->aspl->set(key, script);
			}
		};
		// Если нужно сохранить скрипт обработки слов
		if(this->isFlag(flag_t::expPreword) || this->isFlag(flag_t::expAllData)){
			// Добавляем скрипт обработки слов
			addScript("wordScript", this->toolkit->getWordScript());
		}
		// Если нужно сохранить пользовательские токены
		if(this->isFlag(flag_t::expUserTokens) || this->isFlag(flag_t::expAllData)){
			// Добавляем скрипт обработки пользовательских токенов
			addScript("utokenScript", this->toolkit->getUserTokenScript());
		}
	}
	// Если нужно сохранить черный список слов
	if(this->isFlag(flag_t::expBadwords) || this->isFlag(flag_t::expAllData)){
		// Устанавливаем данные чёрного списка
		this->aspl->setValues("badwords", this->toolkit->getBadwords());
	}
	// Если нужно сохранить белый список слов
	if(this->isFlag(flag_t::expGoodwords) || this->isFlag(flag_t::expAllData)){
		// Устанавливаем данные белого списка
		this->aspl->setValues("goodwords", this->toolkit->getGoodwords());
	}
	/**
	 * Блок сохранения параметров алгоритма сжатия
	 */
	{
		// Получаем параметры алгоритма сжатия
		const auto params = this->toolkit->getParams();
		// Выполняем сохранение дополнительного параметра дельты
		this->aspl->set("modAlgorithm", params.mod);
		// Выполняем сохранение количество уже изменённых младших заказов
		this->aspl->set("modified", params.modified);
		// Выполняем сохранение необходимости изменения счёта, после вычисления
		this->aspl->set("prepares", params.prepares);
		// Выполняем сохранение алгоритма сглаживания
		this->aspl->set("algorithm", params.algorithm);
	}
	/**
	 * Блок записи данных словаря и arpa
	 */
	{
		// Количество обработанных записей
		size_t count = 0;
		// Буфер данных
		vector <char> buffer;
		// Выполняем сохранение информационных данных словаря
		this->toolkit->saveInfoVocab(buffer);
		// Префиксы словаря и arpa
		const string prefixVocab = "vocab_", prefixArpa = "arpa_";
		// Если буфер данных получен
		if(!buffer.empty()) this->aspl->set("infoVocab", buffer, !this->meta.password.empty());
		// Выполняем сохранение словаря
		this->toolkit->saveVocab([&](const vector <char> & buffer, const u_short rate){
			// Если буфер не пустой
			if(!buffer.empty()){
				// Увеличиваем количество записей
				count++;
				// Если нужно вывести статистику загрузки
				if(status != nullptr) status(u_short(rate / double(200) * 100.0));
				// Выполняем запись буфера словаря
				this->aspl->set(prefixVocab + to_string(count), buffer, !this->meta.password.empty());
			}
		});
		// Если данные не получены, выходим
		if(!(result = (count > 0))) return result;
		// Сохраняем количество записей словаря
		this->aspl->set("vocabCount", count);
		// Обнуляем индекс записи
		count = 0;
		// Выполняем сохранение arpa
		this->toolkit->saveArpa([&](const vector <char> & buffer, const u_short rate){
			// Если буфер не пустой
			if(!buffer.empty()){
				// Увеличиваем количество записей
				count++;
				// Если нужно вывести статистику загрузки
				if(status != nullptr) status(u_short((rate + 100) / double(200) * 100.0));
				// Выполняем запись буфера словаря
				this->aspl->set(prefixArpa + to_string(count), buffer, !this->meta.password.empty());
			}
		}, this->isFlag(flag_t::onlyArpa));
		// Если данные не получены, выходим
		if(!(result = (count > 0))) return result;
		// Сохраняем количество записей arpa
		this->aspl->set("arpaCount", count);
		// Сохраняем флаг содержания в словаре только данных arpa
		this->aspl->set("onlyArpa", this->isFlag(flag_t::onlyArpa));
	}
	// Выполняем запись данных словаря
	if(this->aspl->write() < 1){
		// Выводим сообщение об ошибке
		if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - dictionary file is wrong");
		// Сбрасываем флаг результата
		result = false;
	}
	// Выводим результат
	return result;
}
/**
 * readAlm Метод чтения данных из файла в объект alm
 * @param status статус чтения данных из бинарного контейнера
 */
const bool anyks::AbLM::readAlm(function <void (const u_short)> status) noexcept {
	// Результат работы функции
	bool result = false;
	// Индекс выполнения загрузки и общее количество данных
	size_t index = 0, count = 7;
	// Количество основных блоков данных
	size_t vocabCount = 0, arpaCount = 0;
	// Выполняем чтение данных словаря
	if((this->aspl->read() > 0) && (this->alm != nullptr)){
		// Тип шифрования файла
		u_short aes = 0;
		// Извлекаем тип шифрования
		this->aspl->get("aes", aes);
		// Считываем дату генерации словаря
		this->aspl->get("date", this->meta.date);
		// Считываем название словаря
		this->aspl->get("name", this->meta.name);
		// Считываем имя автора словаря
		this->aspl->get("author", this->meta.author);
		// Считываем тип лицензии
		this->aspl->get("lictype", this->meta.lictype);
		// Считываем текст лицензии
		this->aspl->get("lictext", this->meta.lictext);
		// Считываем контактные данные пользователя
		this->aspl->get("contacts", this->meta.contacts);
		// Считываем копирайт словаря
		this->aspl->get("copyright", this->meta.copyright);
		// Если это метод вывода информации о словаре
		if(aes > 0){
			// Устанавливаем неверный пароль
			this->meta.password = "*";
			// Если размер шифрования получен
			switch(aes){
				// Если это 128-и битное шифрование
				case 128: this->meta.aes = aspl_t::types_t::aes128; break;
				// Если это 192-х битное шифрование
				case 192: this->meta.aes = aspl_t::types_t::aes192; break;
				// Если это 256-и битное шифрование
				case 256: this->meta.aes = aspl_t::types_t::aes256; break;
			}
		}
		/**
		 * Блок извлечения данных алфавита
		 */
		{
			// Данные алфавита
			string alphabet = "";
			// Извлекаем данные алфавита
			this->aspl->get("alphabet", alphabet);
			// Если алфавит получен, устанавливаем его
			if(!alphabet.empty()){
				// Устанавливаем алфавит
				this->alphabet->set(alphabet);
				// Выполняем обновление токенизатора
				this->tokenizer->update();
			// Иначе выходим с ошибкой
			} else {
				// Выполняем логирование
				if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - alphabet is broken");
				// Выходим из функции
				return result;
			}
		}
		// Если файл зашифрован и шифрование совпадает
		if((aes > 0) && !this->meta.password.empty()){
			// Если размер шифрования получен
			switch(aes){
				// Если это 128-и битное шифрование
				case 128: {
					// Если это 128 битный режим шифрования
					if(this->meta.aes == aspl_t::types_t::aes128) this->aspl->setAES(this->meta.aes);
				} break;
				// Если это 192-х битное шифрование
				case 192: {
					// Если это 192 битный режим шифрования
					if(this->meta.aes == aspl_t::types_t::aes192) this->aspl->setAES(this->meta.aes);
				} break;
				// Если это 256-и битное шифрование
				case 256: {
					// Если это 256 битный режим шифрования
					if(this->meta.aes == aspl_t::types_t::aes256) this->aspl->setAES(this->meta.aes);
				} break;
				// Если это что-то другое
				default: goto stop;
			}
		// Если файл зашифрован
		} else if(aes > 0) {
			// Метка останова
			stop:
			// Сообщаем что файл зашифрован и данные для расшифровки не предоставлены
			if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - the file dictionary is encrypted and data for decryption is not provided");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
		/**
		 * Блок получения количество основных данных
		 */
		{
			// Считываем количество записей arpa
			this->aspl->get("arpaCount", arpaCount);
			// Считываем количество записей словаря
			this->aspl->get("vocabCount", vocabCount);
			// Увеличиваем общее количество данных
			count += (arpaCount + vocabCount);
			// Иначе выходим с ошибкой
			if((arpaCount < 1) || (vocabCount < 1)){
				// Выполняем логирование
				if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - arpa or vocab is broken");
				// Выходим из функции
				return result;
			}
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения списков суффиксов цифровых аббревиатур
		 */
		{
			// Список суффиксов цифровых аббревиатур
			std::set <size_t> abbrs;
			// Устанавливаем список суффиксов цифровых аббревиатур
			this->aspl->getValues("abbrs-idw", abbrs);
			// Если список аббревиатур загружен
			if(!abbrs.empty()) this->tokenizer->setSuffixes(abbrs);
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения списков детектируемых токенов
		 */
		{
			// Список извлекаемых токенов
			set <token_t> tokens;
			// Извлекаем список токенов приводимых к <unk>
			this->aspl->getValues("tokensUnknown", tokens);
			// Если список токенов получен, устанавливаем его
			if(!tokens.empty()) this->alm->setTokensUnknown(tokens);
			// Извлекаем список запрещённых токенов
			this->aspl->setValues("tokensDisable", tokens);
			// Если список токенов получен, устанавливаем его
			if(!tokens.empty()) this->alm->setTokensDisable(tokens);
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения размера n-граммы
		 */
		{
			// Размер n-граммы
			u_short size = 0;
			// Выполняем чтение размера n-граммы
			this->aspl->get("size", size);
			// Устанавливаем размер n-граммы
			if(size > 0) this->alm->setSize(size);
			// Иначе выходим с ошибкой
			else {
				// Выполняем логирование
				if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - size n-gram is wrong");
				// Выходим из функции
				return result;
			}
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения списка аббревиатур
		 */
		{
			// Список аббревиатур
			std::set <size_t> abbrs;
			// Флаг значения апострофа
			bool allowApostrophe = false;
			// Извлекаем список аббревиатур
			this->aspl->getValues("abbrs", abbrs);
			// Считываем значение апострофа
			this->aspl->get("allowApostrophe", allowApostrophe);
			// Если список аббревиатур получен, устанавливаем его
			if(!abbrs.empty()) this->tokenizer->setAbbrs(abbrs);
			// Если апостроф разрешён, активируем его
			if(allowApostrophe) this->alphabet->switchAllowApostrophe();
		}
		/**
		 * Блок извлечения списка доменных зон
		 */
		{
			// Список доменных зон
			std::set <string> zones;
			// Извлекаем списко доменных зон
			this->aspl->getStrings("dzones", zones);
			// Если список доменных зон получен, устанавливаем его
			if(!zones.empty()) this->alphabet->setzones(zones);
		}
		/**
		 * Блок извлечения списка букв для восстановления слов состоящих из смешанных словарей
		 */
		{
			// Список букв для восстановления
			vector <string> letters;
			// Устанавливаем полученный список букв
			this->aspl->getStrings("substitutes", letters);
			// Если список букв получен
			if(!letters.empty()){
				// Первое и второе слово
				string first = "", second = "";
				// Список букв для восстановления слов
				map <string, string> substitutes;
				// Переходим по всему списку
				for(size_t i = 0; i < letters.size(); i++){
					// Получаем первое слово
					if((i % 2) == 0){
						// Очищаем второе слово
						second.clear();
						// Запоминаем первое слово
						first = move(letters.at(i));
					// Если это второе слово
					} else second = move(letters.at(i));
					// Если второе слово получено
					if(!first.empty() && !second.empty()) substitutes.emplace(first, second);
				}
				// Если список букв получен
				if(!substitutes.empty()) this->alphabet->setSubstitutes(substitutes);
			}
		}
		/**
		 * Блок извлечения неизвестного слова и пользовательских токенов
		 */
		{
			// Пользовательские токены
			vector <string> utokens;
			// Неизвестное слово и данные скрипта
			string unknownWord = "", script = "";
			// Считываем неизвестное слово если есть
			this->aspl->get("unknown", unknownWord);
			// Считываем пользовательские токены
			this->aspl->getStrings("utokens", utokens);
			// Если неизвестное слово получено
			if(!unknownWord.empty()) this->alm->setUnknown(unknownWord);
			// Если пользовательские токены получены
			if(!utokens.empty()){
				// Переходим по всему списку токенов
				for(auto & token : utokens) this->alm->setUserToken(token);
				// Извлекаем данные скрипта
				this->aspl->get("utokenScript", script);
				// Если скрипт получен
				if(!script.empty()){
					// Адрес файла скрипта
					const string filename = "/tmp/almUserTokenScript.py";
					// Открываем файл на запись
					ofstream file(filename, ios::binary);
					// Если файл открыт
					if(file.is_open()){
						// Выполняем запись скрипта в файл
						file.write(script.data(), script.size());
						// Закрываем файл
						file.close();
						// Устанавливаем адрес файла скрипта
						this->alm->setUserTokenScript(filename);
					}
				// Если скрипт не получен
				} else {
					// Очищаем пользовательские токены
					this->alm->clearUserTokens();
					// Выполняем логирование
					if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - script user token is broken");
				}
			}
			// Извлекаем данные скрипта предобработки
			this->aspl->get("wordScript", script);
			// Если скрипт получен
			if(!script.empty()){
				// Адрес файла скрипта
				const string filename = "/tmp/almWordScript.py";
				// Открываем файл на запись
				ofstream file(filename, ios::binary);
				// Если файл открыт
				if(file.is_open()){
					// Выполняем запись скрипта в файл
					file.write(script.data(), script.size());
					// Закрываем файл
					file.close();
					// Устанавливаем адрес файла скрипта
					this->alm->setWordScript(filename);
				}
			}
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
			// Выполняем инициализацию скриптов языковой модели
			this->alm->initPython();
		}
		/**
		 * Блок извлечения черного и белого списков
		 */
		{
			// Список идентификаторов слов
			set <size_t> words;
			// Выполняем извлечение чёрный список слов
			this->aspl->getValues("badwords", words);
			// Если список слов получен
			if(!words.empty()) this->alm->setBadwords(words);
			// Выполняем извлечение белого списка слов
			this->aspl->getValues("goodwords", words);
			// Если список слов получен
			if(!words.empty()) this->alm->setGoodwords(words);
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения списка флагов опций
		 */
		{
			// Список опций
			u_int options = 0;
			// Выполняем извлечение списка опций
			this->aspl->get("options", options);
			// Устанавливаем опции модуля
			if(options > 0){
				// Флаги параметров
				bitset <10> opt = options;
				// Устанавливаем все возможные опции языковой модели
				if(opt.test((u_short) toolkit_t::options_t::debug))      this->alm->setOption(alm_t::options_t::debug);
				if(opt.test((u_short) toolkit_t::options_t::onlyGood))   this->alm->setOption(alm_t::options_t::onlyGood);
				if(opt.test((u_short) toolkit_t::options_t::mixdicts))   this->alm->setOption(alm_t::options_t::mixdicts);
				if(opt.test((u_short) toolkit_t::options_t::confidence)) this->alm->setOption(alm_t::options_t::confidence);
			}
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения данных словаря и arpa
		 */
		{
			// Буфер бинарных данных словаря
			vector <char> buffer;
			// Флаг содержания в словаре только данных arpa
			bool onlyArpa = false;
			// Извлекаем флаг содержания в словаре только данных arpa
			this->aspl->get("onlyArpa", onlyArpa);
			// Если флаг установлен, устанавливаем его
			if(onlyArpa) this->setFlag(flag_t::onlyArpa);
			// Префиксы словаря и arpa
			const string prefixVocab = "vocab_", prefixArpa = "arpa_";
			// Переходим по всему списку слов словаря
			for(size_t i = 1; i <= vocabCount; i++){
				// Извлекаем бинарные данные словаря
				this->aspl->get(prefixVocab + to_string(i), buffer, !this->meta.password.empty());
				// Если бинарные данные словаря получены
				if(!buffer.empty()) this->alm->setVocab(buffer);
				// Если нужно вывести статистику загрузки
				if(status != nullptr){
					// Увеличиваем количество блоков
					index++;
					// Выводим результат если необходимо
					status(u_short(index / double(count) * 100.0));
				}
			}
			// Переходим по всему списку данных arpa
			for(size_t i = 1; i <= arpaCount; i++){
				// Извлекаем бинарные данные arpa
				this->aspl->get(prefixArpa + to_string(i), buffer, !this->meta.password.empty());
				// Если бинарные данные arpa получены
				if(!buffer.empty()) this->alm->setBin(buffer);
				// Если нужно вывести статистику загрузки
				if(status != nullptr){
					// Увеличиваем количество блоков
					index++;
					// Выводим результат если необходимо
					status(u_short(index / double(count) * 100.0));
				}
			}
		}
	// Выводим сообщение об ошибке
	} else if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - dictionary file is wrong");
	// Выводим результат
	return result;
}
/**
 * readToolkit Метод чтения данных из файла в объект toolkit
 * @param status статус чтения данных из бинарного контейнера
 */
const bool anyks::AbLM::readToolkit(function <void (const u_short)> status) noexcept {
	// Результат работы функции
	bool result = false;
	// Индекс выполнения загрузки и общее количество данных
	size_t index = 0, count = 8;
	// Количество основных блоков данных
	size_t vocabCount = 0, arpaCount = 0;
	// Выполняем чтение данных словаря
	if((this->aspl->read() > 0) && (this->toolkit != nullptr)){
		// Тип шифрования файла
		u_short aes = 0;
		// Извлекаем тип шифрования
		this->aspl->get("aes", aes);
		// Считываем дату генерации словаря
		this->aspl->get("date", this->meta.date);
		// Считываем название словаря
		this->aspl->get("name", this->meta.name);
		// Считываем имя автора словаря
		this->aspl->get("author", this->meta.author);
		// Считываем тип лицензии
		this->aspl->get("lictype", this->meta.lictype);
		// Считываем текст лицензии
		this->aspl->get("lictext", this->meta.lictext);
		// Считываем контактные данные пользователя
		this->aspl->get("contacts", this->meta.contacts);
		// Считываем копирайт словаря
		this->aspl->get("copyright", this->meta.copyright);
		// Если это метод вывода информации о словаре
		if(aes > 0){
			// Устанавливаем неверный пароль
			this->meta.password = "*";
			// Если размер шифрования получен
			switch(aes){
				// Если это 128-и битное шифрование
				case 128: this->meta.aes = aspl_t::types_t::aes128; break;
				// Если это 192-х битное шифрование
				case 192: this->meta.aes = aspl_t::types_t::aes192; break;
				// Если это 256-и битное шифрование
				case 256: this->meta.aes = aspl_t::types_t::aes256; break;
			}
		}
		/**
		 * Блок извлечения данных алфавита
		 */
		{
			// Данные алфавита
			string alphabet = "";
			// Извлекаем данные алфавита
			this->aspl->get("alphabet", alphabet);
			// Если алфавит получен, устанавливаем его
			if(!alphabet.empty()){
				// Устанавливаем алфавит
				this->alphabet->set(alphabet);
				// Выполняем обновление токенизатора
				this->tokenizer->update();
			// Иначе выходим с ошибкой
			} else {
				// Выполняем логирование
				if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - alphabet is broken");
				// Выходим из функции
				return result;
			}
		}
		// Если файл зашифрован и шифрование совпадает
		if((aes > 0) && !this->meta.password.empty()){
			// Если размер шифрования получен
			switch(aes){
				// Если это 128-и битное шифрование
				case 128: {
					// Если это 128 битный режим шифрования
					if(this->meta.aes == aspl_t::types_t::aes128) this->aspl->setAES(this->meta.aes);
				} break;
				// Если это 192-х битное шифрование
				case 192: {
					// Если это 192 битный режим шифрования
					if(this->meta.aes == aspl_t::types_t::aes192) this->aspl->setAES(this->meta.aes);
				} break;
				// Если это 256-и битное шифрование
				case 256: {
					// Если это 256 битный режим шифрования
					if(this->meta.aes == aspl_t::types_t::aes256) this->aspl->setAES(this->meta.aes);
				} break;
				// Если это что-то другое
				default: goto stop;
			}
		// Если файл зашифрован
		} else if(aes > 0) {
			// Метка останова
			stop:
			// Сообщаем что файл зашифрован и данные для расшифровки не предоставлены
			if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - the file dictionary is encrypted and data for decryption is not provided");
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
		/**
		 * Блок получения количество основных данных
		 */
		{
			// Считываем количество записей arpa
			this->aspl->get("arpaCount", arpaCount);
			// Считываем количество записей словаря
			this->aspl->get("vocabCount", vocabCount);
			// Увеличиваем общее количество данных
			count += (arpaCount + vocabCount);
			// Иначе выходим с ошибкой
			if((arpaCount < 1) || (vocabCount < 1)){
				// Выполняем логирование
				if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - arpa or vocab is broken");
				// Выходим из функции
				return result;
			}
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения списков суффиксов цифровых аббревиатур
		 */
		{
			// Список суффиксов цифровых аббревиатур
			std::set <size_t> abbrs;
			// Устанавливаем список суффиксов цифровых аббревиатур
			this->aspl->getValues("abbrs-idw", abbrs);
			// Если список аббревиатур загружен
			if(!abbrs.empty()) this->tokenizer->setSuffixes(abbrs);
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения списков детектируемых токенов
		 */
		{
			// Список извлекаемых токенов
			set <token_t> tokens;
			// Извлекаем список токенов приводимых к <unk>
			this->aspl->getValues("tokensUnknown", tokens);
			// Если список токенов получен, устанавливаем его
			if(!tokens.empty()) this->toolkit->setTokensUnknown(tokens);
			// Извлекаем список запрещённых токенов
			this->aspl->setValues("tokensDisable", tokens);
			// Если список токенов получен, устанавливаем его
			if(!tokens.empty()) this->toolkit->setTokensDisable(tokens);
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения размера n-граммы
		 */
		{
			// Размер n-граммы
			u_short size = 0;
			// Выполняем чтение размера n-граммы
			this->aspl->get("size", size);
			// Устанавливаем размер n-граммы
			if(size > 0) this->toolkit->setSize(size);
			// Иначе выходим с ошибкой
			else {
				// Выполняем логирование
				if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - size n-gram is wrong");
				// Выходим из функции
				return result;
			}
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения списка аббревиатур
		 */
		{
			// Список аббревиатур
			std::set <size_t> abbrs;
			// Флаг значения апострофа
			bool allowApostrophe = false;
			// Извлекаем список аббревиатур
			this->aspl->getValues("abbrs", abbrs);
			// Считываем значение апострофа
			this->aspl->get("allowApostrophe", allowApostrophe);
			// Если список аббревиатур получен, устанавливаем его
			if(!abbrs.empty()) this->tokenizer->setAbbrs(abbrs);
			// Если апостроф разрешён, активируем его
			if(allowApostrophe) this->alphabet->switchAllowApostrophe();
		}
		/**
		 * Блок извлечения списка доменных зон
		 */
		{
			// Список доменных зон
			std::set <string> zones;
			// Извлекаем списко доменных зон
			this->aspl->getStrings("dzones", zones);
			// Если список доменных зон получен, устанавливаем его
			if(!zones.empty()) this->alphabet->setzones(zones);
		}
		/**
		 * Блок извлечения списка букв для восстановления слов состоящих из смешанных словарей
		 */
		{
			// Список букв для восстановления
			vector <string> letters;
			// Устанавливаем полученный список букв
			this->aspl->getStrings("substitutes", letters);
			// Если список букв получен
			if(!letters.empty()){
				// Первое и второе слово
				string first = "", second = "";
				// Список букв для восстановления слов
				map <string, string> substitutes;
				// Переходим по всему списку
				for(size_t i = 0; i < letters.size(); i++){
					// Получаем первое слово
					if((i % 2) == 0){
						// Очищаем второе слово
						second.clear();
						// Запоминаем первое слово
						first = move(letters.at(i));
					// Если это второе слово
					} else second = move(letters.at(i));
					// Если второе слово получено
					if(!first.empty() && !second.empty()) substitutes.emplace(first, second);
				}
				// Если список букв получен
				if(!substitutes.empty()) this->alphabet->setSubstitutes(substitutes);
			}
		}
		/**
		 * Блок извлечения неизвестного слова и пользовательских токенов
		 */
		{
			// Пользовательские токены
			vector <string> utokens;
			// Неизвестное слово и данные скрипта
			string unknownWord = "", script = "";
			// Считываем неизвестное слово если есть
			this->aspl->get("unknown", unknownWord);
			// Считываем пользовательские токены
			this->aspl->getStrings("utokens", utokens);
			// Если неизвестное слово получено
			if(!unknownWord.empty()) this->toolkit->setUnknown(unknownWord);
			// Если пользовательские токены получены
			if(!utokens.empty()){
				// Переходим по всему списку токенов
				for(auto & token : utokens) this->toolkit->setUserToken(token);
				// Извлекаем данные скрипта
				this->aspl->get("utokenScript", script);
				// Если скрипт получен
				if(!script.empty()){
					// Адрес файла скрипта
					const string filename = "/tmp/almUserTokenScript.py";
					// Открываем файл на запись
					ofstream file(filename, ios::binary);
					// Если файл открыт
					if(file.is_open()){
						// Выполняем запись скрипта в файл
						file.write(script.data(), script.size());
						// Закрываем файл
						file.close();
						// Устанавливаем адрес файла скрипта
						this->toolkit->setUserTokenScript(filename);
					}
				// Если скрипт не получен
				} else {
					// Очищаем пользовательские токены
					this->toolkit->clearUserTokens();
					// Выполняем логирование
					if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - script user token is broken");
				}
			}
			// Извлекаем данные скрипта предобработки
			this->aspl->get("wordScript", script);
			// Если скрипт получен
			if(!script.empty()){
				// Адрес файла скрипта
				const string filename = "/tmp/almWordScript.py";
				// Открываем файл на запись
				ofstream file(filename, ios::binary);
				// Если файл открыт
				if(file.is_open()){
					// Выполняем запись скрипта в файл
					file.write(script.data(), script.size());
					// Закрываем файл
					file.close();
					// Устанавливаем адрес файла скрипта
					this->toolkit->setWordScript(filename);
				}
			}
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения черного и белого списков
		 */
		{
			// Список идентификаторов слов
			set <size_t> words;
			// Выполняем извлечение чёрный список слов
			this->aspl->getValues("badwords", words);
			// Если список слов получен
			if(!words.empty()) this->toolkit->setBadwords(words);
			// Выполняем извлечение белого списка слов
			this->aspl->getValues("goodwords", words);
			// Если список слов получен
			if(!words.empty()) this->toolkit->setGoodwords(words);
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения списка флагов опций
		 */
		{
			// Список опций
			u_int options = 0;
			// Выполняем извлечение списка опций
			this->aspl->get("options", options);
			// Устанавливаем опции модуля
			if(options > 0) this->toolkit->setOptions(options);
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок инициализации тулкита
		 */
		{
			// Алгоритм сглаживания
			u_short algorithm = 0;
			// Считываем алгоритм сглаживания
			this->aspl->get("algorithm", algorithm);
			// Если алгоритм получен
			if((result = (algorithm > 0))){
				// Дополнительный параметр дельты
				double mod = 0.0;
				// Количество уже изменённых младших заказов
				bool modified = false;
				// Необходимость изменения счёта, после вычисления
				bool prepares = false;
				// Считываем дополнительный параметр дельты
				this->aspl->get("modAlgorithm", mod);
				// Считываем количество уже изменённых младших заказов
				this->aspl->get("modified", modified);
				// Считываем необходимость изменения счёта, после вычисления
				this->aspl->get("prepares", prepares);
				// Выполняем инициализацию тулкита
				this->toolkit->init((toolkit_t::algorithm_t) algorithm, modified, prepares, mod);
			// Если алгоритм не получен
			} else {
				// Выполняем логирование
				if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - algorithm smoothing is wrong");
				// Выходим из функции
				return result;
			}
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
		}
		/**
		 * Блок извлечения данных словаря и arpa
		 */
		{
			// Буфер бинарных данных словаря
			vector <char> buffer;
			// Флаг содержания в словаре только данных arpa
			bool onlyArpa = false;
			// Извлекаем флаг содержания в словаре только данных arpa
			this->aspl->get("onlyArpa", onlyArpa);
			// Если флаг установлен, устанавливаем его
			if(onlyArpa) this->setFlag(flag_t::onlyArpa);
			// Префиксы словаря и arpa
			const string prefixVocab = "vocab_", prefixArpa = "arpa_";
			// Извлекаем информацию информацию словаря
			this->aspl->get("infoVocab", buffer, !this->meta.password.empty());
			// Если бинарная информация словаря получена
			if(!buffer.empty()) this->toolkit->loadInfoVocab(buffer);
			// Иначе выходим с ошибкой
			else {
				// Выполняем логирование
				if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - info vocab is broken");
				// Выходим из функции
				return result;
			}
			// Если нужно вывести статистику загрузки
			if(status != nullptr){
				// Увеличиваем количество блоков
				index++;
				// Выводим результат если необходимо
				status(u_short(index / double(count) * 100.0));
			}
			// Переходим по всему списку слов словаря
			for(size_t i = 1; i <= vocabCount; i++){
				// Извлекаем бинарные данные словаря
				this->aspl->get(prefixVocab + to_string(i), buffer, !this->meta.password.empty());
				// Если бинарные данные словаря получены
				if(!buffer.empty()) this->toolkit->loadVocab(buffer);
				// Если нужно вывести статистику загрузки
				if(status != nullptr){
					// Увеличиваем количество блоков
					index++;
					// Выводим результат если необходимо
					status(u_short(index / double(count) * 100.0));
				}
			}
			// Переходим по всему списку данных arpa
			for(size_t i = 1; i <= arpaCount; i++){
				// Извлекаем бинарные данные arpa
				this->aspl->get(prefixArpa + to_string(i), buffer, !this->meta.password.empty());
				// Если бинарные данные arpa получены
				if(!buffer.empty()) this->toolkit->loadArpa(buffer, onlyArpa);
				// Если нужно вывести статистику загрузки
				if(status != nullptr){
					// Увеличиваем количество блоков
					index++;
					// Выводим результат если необходимо
					status(u_short(index / double(count) * 100.0));
				}
			}
		}
	// Выводим сообщение об ошибке
	} else if(this->isFlag(flag_t::debug)) this->alphabet->log("%s", alphabet_t::log_t::error, this->logfile, "ablm - dictionary file is wrong");
	// Выводим результат
	return result;
}
/**
 * Метод инициализации
 */
void anyks::AbLM::init(){
	// Если объект бинарного контейнера еще не инициализирован
	if(this->aspl == nullptr){
		// Экранируем возможность ошибки памяти
		try {
			// Устанавливаем название словаря
			if(this->meta.name.empty()) this->meta.name = "alm";
			// Выполняем инициализацию объекта бинарного контейнера
			this->aspl = new aspl_t(this->filename, this->meta.password);
		// Если происходит ошибка то игнорируем её
		} catch(const bad_alloc &) {
			// Выходим из приложения
			exit(EXIT_FAILURE);
		}
	}
}
/**
 * clear Метод очистки данных словаря
 */
void anyks::AbLM::clear() noexcept {
	// Обнуляем метаданные
	this->meta = {};
	// Обнуляем установленные флаги
	this->flags = 0;
	// Очищаем имя файла бинарного контейнера
	this->filename = "";
	// Удаляем выделенную память для бинарного контейнера
	if(this->aspl != nullptr){
		// Очищаем выделенную память
		delete this->aspl;
		// Зануляем объект
		this->aspl = nullptr;
	}
}
/**
 * info Метод вывода инормационных данных словаря
 */
void anyks::AbLM::info() const noexcept {
	// Выполняем чтение данных словаря
	if(this->aspl->read() > 0){
		// Название словаря
		string name = "";
		// Считываем название словаря
		this->aspl->get("name", name);
		// Если название словаря указано
		if(!name.empty()){
			// Выполняем очистку консоли
			printf("\033c");
			// Отображаем разделители
			printf("\r\n\r\n* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\r\n\r\n");
			// Размер n-граммы и тип шифрования файла
			u_short size = 0, aes = 0;
			// Количество основных блоков данных
			size_t vocabCount = 0, arpaCount = 0;
			// Метаданные информации о словаре
			string alphabet = "", author = "", lictype = "", lictext = "", contacts = "", copyright = "";
			// Извлекаем тип шифрования
			this->aspl->get("aes", aes);
			// Выполняем чтение размера n-граммы
			this->aspl->get("size", size);
			// Считываем имя автора словаря
			this->aspl->get("author", author);
			// Считываем тип лицензии
			this->aspl->get("lictype", lictype);
			// Считываем текст лицензии
			this->aspl->get("lictext", lictext);
			// Считываем контактные данные пользователя
			this->aspl->get("contacts", contacts);
			// Извлекаем данные алфавита
			this->aspl->get("alphabet", alphabet);
			// Считываем копирайт словаря
			this->aspl->get("copyright", copyright);
			// Считываем количество записей arpa
			this->aspl->get("arpaCount", arpaCount);
			// Считываем количество записей словаря
			this->aspl->get("vocabCount", vocabCount);
			// Выводим в консоль название словаря
			printf("* \x1B[1mName:\x1B[0m %s\r\n\r\n", name.c_str());
			// Если размер шифрования получен
			switch(aes){
				// Если это 128-и битное шифрование
				case 128: printf("* \x1B[1mEncryption:\x1B[0m AES%u\r\n\r\n", 128); break;
				// Если это 192-х битное шифрование
				case 192: printf("* \x1B[1mEncryption:\x1B[0m AES%u\r\n\r\n", 192); break;
				// Если это 256-и битное шифрование
				case 256: printf("* \x1B[1mEncryption:\x1B[0m AES%u\r\n\r\n", 256); break;
			}
			// Выводим в консоль алфавит словаря
			if(!alphabet.empty()) printf("* \x1B[1mAlphabet:\x1B[0m %s\r\n\r\n", alphabet.c_str());
			/**
			 * Блок формирования даты генерации словаря
			 */
			{
				// Время генерации словаря
				time_t datetime = 0;
				// Считываем дату генерации словаря
				this->aspl->get("date", datetime);
				// Если дата генерации словаря получена
				if(datetime > 0){
					// Создаем буфер для хранения даты
					char date[80];
					// Заполняем его нулями
					memset(date, 0, sizeof(date));
					// Получаем структуру локального времени
					struct tm * timeinfo = localtime(&datetime);
					// Копируем в буфер полученную дату и время
					const int length = strftime(date, sizeof(date), "%m/%d/%Y %H:%M:%S", timeinfo);
					// Если дата создана
					if(length > 0){
						// Создаем строку с датой
						const string zdate(date, length);
						// Выводим в консоль дату генерации словаря
						printf("* \x1B[1mBuild date:\x1B[0m %s\r\n\r\n", zdate.c_str());
					}
				}
			}
			// Выводим размер языковой модели
			if(size > 0) printf("* \x1B[1mN-gram size:\x1B[0m %u\r\n\r\n", size);
			// Выводим количество слов в словаре
			if(vocabCount > 0) printf("* \x1B[1mWords:\x1B[0m %zu\r\n\r\n", vocabCount);
			// Выводим количество N-грамм в языковой моделе
			if(arpaCount > 0) printf("* \x1B[1mN-grams:\x1B[0m %zu\r\n\r\n", arpaCount);
			// Выводим в консоль данные автора
			if(!author.empty()) printf("* \x1B[1mAuthor:\x1B[0m %s\r\n\r\n", author.c_str());
			// Выводим в консоль данные автора
			if(!contacts.empty()) printf("* \x1B[1mContacts:\x1B[0m %s\r\n\r\n", contacts.c_str());
			// Выводим в консоль авторские права
			if(!copyright.empty()) printf("* \x1B[1mCopyright ©:\x1B[0m %s\r\n\r\n", copyright.c_str());
			// Выводим тип лицензии
			if(!lictype.empty()) printf("* \x1B[1mLicense type:\x1B[0m %s\r\n\r\n", lictype.c_str());
			// Выводим в консоль лицензионное сообщение
			if(!lictext.empty()) printf("* \x1B[1mLicense text:\x1B[0m\r\n%s\r\n\r\n", lictext.c_str());
			// Отображаем разделители
			printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\r\n\r\n");
		}
	}
}
/**
 * setAlm Метод установки объекта языковой модели
 * @param alm объект языковой модели для установки
 */
void anyks::AbLM::setAlm(alm_t * alm) noexcept {
	// Устанавливаем объект языковой модели
	this->alm = alm;
}
/**
 * setAES Метод установки размера шифрования
 * @param aes размер шифрования для установки
 */
void anyks::AbLM::setAES(aspl_t::types_t aes) noexcept {
	// Выполняем установку размера шифрования
	this->meta.aes = aes;
}
/**
 * setFlag Метод установки флага модуля
 * @param flag флаг для установки
 */
void anyks::AbLM::setFlag(const flag_t flag) noexcept {
	// Устанавливаем флаг модуля
	this->flags.set((u_short) flag);
}
/**
 * unsetFlag Метод отключения флага модуля
 * @param flag флаг для отключения
 */
void anyks::AbLM::unsetFlag(const flag_t flag) noexcept {
	// Удаляем флаг модуля
	this->flags.reset((u_short) flag);
}
/**
 * setName Метод установки названия словаря
 * @param name название словаря для установки
 */
void anyks::AbLM::setName(const string & name) noexcept {
	// Если название словаря передано
	if(!name.empty()) this->meta.name = name;
}
/**
 * setAuthor Метод установки автора словаря
 * @param author автор словаря для установки
 */
void anyks::AbLM::setAuthor(const string & author) noexcept {
	// Если автор словаря передан, устанавливаем его
	if(!author.empty()) this->meta.author = author;
}
/**
 * setLogfile Метод установки файла для вывода логов
 * @param logifle адрес файла для вывода отладочной информации
 */
void anyks::AbLM::setLogfile(const char * logfile) noexcept {
	// Устанавливаем адрес файла логирования
	this->logfile = logfile;
}
/**
 * setToolkit Метод установки объекта тулкита
 * @param toolkit объект тукита для установки
 */
void anyks::AbLM::setToolkit(toolkit_t * toolkit) noexcept {
	// Устанавливаем объект тулкита
	this->toolkit = toolkit;
}
/**
 * setLictype Метод установки типа лицензии
 * @param lictype тип лицензии для установки
 */
void anyks::AbLM::setLictype(const string & lictype) noexcept {
	// Если тип лицензии передан, устанавливаем его
	if(!lictype.empty()) this->meta.lictype = lictype;
}
/**
 * setLictext Метод установки текста лицензии
 * @param lictext текст лицензии для установки
 */
void anyks::AbLM::setLictext(const string & lictext) noexcept {
	// Если текст лицензии передан, устанавливаем его
	if(!lictext.empty()) this->meta.lictext = lictext;
}
/**
 * setAlphabet Метод установки объекта словаря
 * @param alphabet объект словаря для установки
 */
void anyks::AbLM::setAlphabet(alphabet_t * alphabet) noexcept {
	// Устанавливаем объект алфавита
	this->alphabet = alphabet;
}
/**
 * setContacts Метод установки контактов автора
 * @param contacts контакты автора для установки
 */
void anyks::AbLM::setContacts(const string & contacts) noexcept {
	// Устанавливаем контактную информацию автора
	if(!contacts.empty()) this->meta.contacts = contacts;
}
/**
 * setPassword Метод установки пароля словаря
 * @param password пароль словаря для установки
 */
void anyks::AbLM::setPassword(const string & password) noexcept {
	// Устанавливаем пароль словаря
	if(!password.empty()) this->meta.password = password;
}
/**
 * setTokenizer Метод установки токенизатора
 * @param tokenizer объект токенизатора
 */
void anyks::AbLM::setTokenizer(tokenizer_t * tokenizer) noexcept {
	// Устанавливаем объект токенизатора
	this->tokenizer = tokenizer;
}
/**
 * setFilename Метод установки адреса файла словаря
 * @param filename адрес файла словаря
 */
void anyks::AbLM::setFilename(const string & filename) noexcept {
	// Устанавливаем имя файла
	if(!filename.empty()) this->filename = filename;
}
/**
 * setCopyright Метод установки копирайта автора
 * @param copyright копирайт автора для установки
 */
void anyks::AbLM::setCopyright(const string & copyright) noexcept {
	// Устанавливаем копирайт пользователя
	if(!copyright.empty()) this->meta.copyright = copyright;
}
/**
 * AbLM Конструктор
 * @param filename  адрес файла словаря
 * @param alm       объект языковой модели для установки
 * @param alphabet  объект словаря для установки
 * @param tokenizer объект токенизатора
 * @param logifle   адрес файла для вывода отладочной информации
 */
anyks::AbLM::AbLM(const string & filename, alm_t * alm, alphabet_t * alphabet, tokenizer_t * tokenizer, const char * logfile) noexcept {
	// Устанавливаем объект языковой модели
	this->setAlm(alm);
	// Устанавливаем адрес файла логов
	this->setLogfile(logfile);
	// Устанавливаем имя файла
	this->setFilename(filename);
	// Устанавливаем объект алфавита
	this->setAlphabet(alphabet);
	// Устанавливаем объект токенизатора
	this->setTokenizer(tokenizer);
}
/**
 * AbLM Конструктор
 * @param filename  адрес файла словаря
 * @param toolkit   объект тукита для установки
 * @param alphabet  объект словаря для установки
 * @param tokenizer объект токенизатора
 * @param logifle   адрес файла для вывода отладочной информации
 */
anyks::AbLM::AbLM(const string & filename, toolkit_t * toolkit, alphabet_t * alphabet, tokenizer_t * tokenizer, const char * logfile) noexcept {
	// Устанавливаем адрес файла логов
	this->setLogfile(logfile);
	// Устанавливаем объект тулкита
	this->setToolkit(toolkit);
	// Устанавливаем имя файла
	this->setFilename(filename);
	// Устанавливаем объект алфавита
	this->setAlphabet(alphabet);
	// Устанавливаем объект токенизатора
	this->setTokenizer(tokenizer);
}
/**
 * ~AbLM Деструктор
 */
anyks::AbLM::~AbLM() noexcept {
	// Выполняем очистику выделенных решений
	this->clear();
}

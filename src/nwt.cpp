/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include <nwt.hpp>

/**
 * get Метод получения распарсенных данных
 * @return параметры полученные в результате парсинга
 */
const anyks::Uri::data_t & anyks::Uri::get() noexcept {
	// Выводим результат парсинга
	return this->data;
}
/**
 * getZones Метод извлечения списка пользовательских зон интернета
 */
const set <wstring> & anyks::Uri::getZones() const noexcept {
	// Выводим список пользовательских зон интернета
	return this->user;
}
/**
 * clear Метод очистки результатов парсинга
 */
void anyks::Uri::clear() noexcept {
	// Очищаем список пользовательских зон
	this->user.clear();
	// Очищаем блок полученных данных
	this->data.uri.clear();
	this->data.data.clear();
	this->data.path.clear();
	this->data.port.clear();
	this->data.user.clear();
	this->data.domain.clear();
	this->data.params.clear();
	this->data.protocol.clear();
	this->data.type = types_t::null;
}
/**
 * parse Метод парсинга URI строки
 * @param text текст для парсинга
 */
void anyks::Uri::parse(const wstring & text) noexcept {
	// Если текст передан
	if(!text.empty()){
		/**
		 * emailFn Функция извлечения данных электронного адреса
		 * @param text текст для парсинга
		 */
		auto emailFn = [this](const wstring & text) noexcept {
			// Результат работы функции
			data_t result;
			// Если текст передан
			if(!text.empty()){
				// Результат работы регулярного выражения
				wsmatch match;
				// Устанавливаем регулярное выражение для проверки электронной почты
				wregex e(
					wstring(L"((?:([\\w\\-")
					+ this->letters
					+ wstring(L"]+)\\@)(\\[(?:\\:\\:ffff\\:\\d{1,3}(?:\\.\\d{1,3}){3}|(?:[a-f\\d]{1,4}(?:(?:\\:[a-f\\d]{1,4})|\\:){1,6}\\:[a-f\\d]{1,4})|(?:[a-f\\d]{1,4}(?:(?:\\:[a-f\\d]{1,4}){7}|(?:\\:[a-f\\d]{1,4}){1,6}\\:\\:|\\:\\:)|\\:\\:))\\]|(?:\\d{1,3}(?:\\.\\d{1,3}){3})|(?:(?:xn\\-\\-[\\w\\d]+\\.){0,100}(?:xn\\-\\-[\\w\\d]+)|(?:[\\w\\-")
					+ this->letters
					+ wstring(L"]+\\.){0,100}[\\w\\-")
					+ this->letters
					+ wstring(L"]+)\\.(xn\\-\\-[\\w\\d]+|[a-z")
					+ this->letters
					+ wstring(L"]+)))"),
					wregex::ECMAScript | wregex::icase
				);
				// Выполняем проверку электронной почты
				regex_search(text, match, e);
				// Если результат найден
				if(!match.empty()){
					// Запоминаем тип параметра
					result.type = types_t::email;
					// Запоминаем uri адрес
					result.uri = match[1].str();
					// Запоминаем логин пользователя
					result.user = match[2].str();
					// Запоминаем название электронного ящика
					result.data = match[3].str();
					// Запоминаем домен верхнего уровня
					result.domain = match[4].str();
				}
			}
			// Выводим результат
			return result;
		};
		/**
		 * domainFn Функция извлечения данных доменного имени
		 * @param text текст для парсинга
		 */
		auto domainFn = [this](const wstring & text) noexcept {
			// Результат работы функции
			data_t result;
			// Если текст передан
			if(!text.empty()){
				// Результат работы регулярного выражения
				wsmatch match;
				// Устанавливаем правило регулярного выражения
				wregex e(
					wstring(L"(?:(http[s]?)\\:\\/\\/)?(\\[(?:\\:\\:ffff\\:\\d{1,3}(?:\\.\\d{1,3}){3}|(?:[a-f\\d]{1,4}(?:(?:\\:[a-f\\d]{1,4})|\\:){1,6}\\:[a-f\\d]{1,4})|(?:[a-f\\d]{1,4}(?:(?:\\:[a-f\\d]{1,4}){7}|(?:\\:[a-f\\d]{1,4}){1,6}\\:\\:|\\:\\:)|\\:\\:))\\]|(?:\\d{1,3}(?:\\.\\d{1,3}){3})|(?:(?:xn\\-\\-[\\w\\d]+\\.){0,100}(?:xn\\-\\-[\\w\\d]+)|(?:[\\w\\-")
					+ this->letters
					+ wstring(L"]+\\.){0,100}[\\w\\-")
					+ this->letters
					+ wstring(L"]+)\\.(xn\\-\\-[\\w\\d]+|[a-z")
					+ this->letters
					+ wstring(L"]+))(?:\\:(\\d+))?((?:\\/[\\w\\-]+){0,100}(?:$|\\/|\\.[\\w]+)|\\/)?(\\?(?:[\\w\\-\\.\\~\\:\\#\\[\\]\\@\\!\\$\\&\\'\\(\\)\\*\\+\\,\\;\\=]+)?)?"),
					wregex::ECMAScript | wregex::icase
				);
				// Выполняем проверку адреса сайта
				regex_search(text, match, e);
				// Если результат найден
				if(!match.empty()){
					// Запоминаем тип параметра
					result.type = types_t::domain;
					// Запоминаем uri адрес
					result.uri = match[0].str();
					// Запоминаем название домена
					result.data = match[2].str();
					// Запоминаем порт запроса
					result.port = match[4].str();
					// Запоминаем путь запроса
					result.path = match[5].str();
					// Запоминаем домен верхнего уровня
					result.domain = match[3].str();
					// Запоминаем параметры запроса
					result.params = match[6].str();
					// Запоминаем протокол
					result.protocol = match[1].str();
					// Если протокол не указан
					if(result.protocol.empty()){
						// Если домен верхнего уровня не является таковым, очищаем все
						if((this->national.count(result.domain) < 1)
						&& (this->general.count(result.domain) < 1)
						&& (this->user.count(result.domain) < 1)){
							// Очищаем блок полученных данных
							result.path.clear();
							result.port.clear();
							result.user.clear();
							result.domain.clear();
							result.params.clear();
							result.protocol.clear();
							result.type = types_t::wrong;
						}
					}
				}
			}
			// Выводим результат
			return result;
		};
		/**
		 * ipFn Функция извлечения данных ip адресов
		 * @param text текст для парсинга
		 */
		auto ipFn = [](const wstring & text) noexcept {
			// Результат работы функции
			data_t result;
			// Если текст передан
			if(!text.empty()){
				// Результат работы регулярного выражения
				wsmatch match;
				// Устанавливаем правило регулярного выражения
				wregex e(
					// Если это сеть
					L"(?:((?:\\d{1,3}(?:\\.\\d{1,3}){3}|(?:[a-f\\d]{1,4}(?:(?:\\:[a-f\\d]{1,4})|\\:){1,6}\\:[a-f\\d]{1,4})|(?:[a-f\\d]{1,4}(?:(?:\\:[a-f\\d]{1,4}){7}|(?:\\:[a-f\\d]{1,4}){1,6}[\\:]{2}|[\\:]{2})|[\\:]{2}))\\/(?:\\d{1,3}(?:\\.\\d{1,3}){3}|\\d+))|"
					// Определение мак адреса
					L"([a-f\\d]{2}(?:\\:[a-f\\d]{2}){5})|"
					// Определение ip6 адреса
					L"(?:(?:http[s]?\\:[\\/]{2})?(?:\\[?([\\:]{2}ffff\\:\\d{1,3}(?:\\.\\d{1,3}){3}|(?:[a-f\\d]{1,4}(?:(?:\\:[a-f\\d]{1,4})|\\:){1,6}\\:[a-f\\d]{1,4})|(?:[a-f\\d]{1,4}(?:(?:\\:[a-f\\d]{1,4}){7}|(?:\\:[a-f\\d]{1,4}){1,6}[\\:]{2}|[\\:]{2})|[\\:]{2}))\\]?)(?:\\:\\d+)?\\/?)|"
					// Определение ip4 адреса
					L"(?:(?:http[s]?\\:[\\/]{2})?(\\d{1,3}(?:\\.\\d{1,3}){3})(?:\\:\\d+)?\\/?))",
					wregex::ECMAScript | wregex::icase
				);
				// Выполняем проверку
				regex_search(text, match, e);
				// Если результат найден
				if(!match.empty()){
					// Запоминаем uri адрес
					result.uri = match[0].str();
					// Извлекаем полученные данные
					const wstring & mac = match[2].str();
					const wstring & ip4 = match[4].str();
					const wstring & ip6 = match[3].str();
					const wstring & network = match[1].str();
					// Если это MAC адрес
					if(!mac.empty()){
						// Запоминаем сам параметр
						result.data = mac;
						// Запоминаем тип параметра
						result.type = types_t::mac;
					// Если это IPv4 адрес
					} else if(!ip4.empty()){
						// Запоминаем сам параметр
						result.data = ip4;
						// Запоминаем тип параметра
						result.type = types_t::ipv4;
					// Если это IPv6 адрес
					} else if(!ip6.empty()) {
						// Запоминаем сам параметр
						result.data = ip6;
						// Запоминаем тип параметра
						result.type = types_t::ipv6;
					// Если это параметры сети
					} else if(!network.empty()) {
						// Запоминаем сам параметр
						result.data = network;
						// Запоминаем тип параметра
						result.type = types_t::network;
					}
				}
			}
			// Выводим результат
			return result;
		};
		// Очищаем результаты предыдущей работы
		this->clear();
		// Запрашиваем данные электронной почты
		data_t email = emailFn(text);
		// Если тип не получен
		if(email.type == types_t::null){
			// Запрашиваем данные доменного имени
			data_t domain = domainFn(text);
			// Если данные домена не получены или протокол не найден
			if((domain.type == types_t::null)
			|| (domain.type == types_t::wrong)){
				// Выполняем поиск ip адресов
				data_t ip = ipFn(text);
				// Если результат получен
				if(ip.type != types_t::null) this->data = move(ip);
				// Если же ip адре не получен то возвращаем данные домена
				else this->data = move(domain);
			// Иначе запоминаем результат
			} else this->data = move(domain);
		// Иначе запоминаем результат
		} else this->data = move(email);
	}
}
/**
 * setZone Метод установки пользовательской зоны
 * @param zone пользовательская зона
 */
void anyks::Uri::setZone(const wstring & zone) noexcept {
	// Если зона передана и она не существует
	if(!zone.empty() && (this->national.count(zone) < 1) && (this->general.count(zone) < 1)){
		// Добавляем зону в список
		this->user.emplace(zone);
	}
}
/**
 * setLetters Метод добавления букв алфавита
 * @param letters список букв алфавита
 */
void anyks::Uri::setLetters(const wstring & letters) noexcept {
	// Если буквы переданы запоминаем их
	if(!letters.empty()) this->letters = move(letters);
}
/**
 * setZones Метод установки списка пользовательских зон
 * @param zones список доменных зон интернета
 */
void anyks::Uri::setZones(const set <wstring> & zones) noexcept {
	// Если список зон не пустой
	if(!zones.empty()) this->user = move(zones);
}
/**
 * Uri Конструктор
 * @param letters список букв алфавита
 * @param text    текст для парсинга
 */
anyks::Uri::Uri(const wstring & letters, const wstring & text) noexcept {
	// Создаем список национальных доменов
	this->national.emplace(L"ac");
	this->national.emplace(L"ad");
	this->national.emplace(L"ae");
	this->national.emplace(L"af");
	this->national.emplace(L"ag");
	this->national.emplace(L"ai");
	this->national.emplace(L"al");
	this->national.emplace(L"am");
	this->national.emplace(L"an");
	this->national.emplace(L"ao");
	this->national.emplace(L"aq");
	this->national.emplace(L"ar");
	this->national.emplace(L"as");
	this->national.emplace(L"at");
	this->national.emplace(L"au");
	this->national.emplace(L"aw");
	this->national.emplace(L"ax");
	this->national.emplace(L"az");
	this->national.emplace(L"ba");
	this->national.emplace(L"bb");
	this->national.emplace(L"bd");
	this->national.emplace(L"be");
	this->national.emplace(L"bf");
	this->national.emplace(L"bg");
	this->national.emplace(L"bh");
	this->national.emplace(L"bi");
	this->national.emplace(L"bj");
	this->national.emplace(L"bm");
	this->national.emplace(L"bn");
	this->national.emplace(L"bo");
	this->national.emplace(L"br");
	this->national.emplace(L"bs");
	this->national.emplace(L"bt");
	this->national.emplace(L"bv");
	this->national.emplace(L"bw");
	this->national.emplace(L"by");
	this->national.emplace(L"bz");
	this->national.emplace(L"ca");
	this->national.emplace(L"cc");
	this->national.emplace(L"cd");
	this->national.emplace(L"cf");
	this->national.emplace(L"cg");
	this->national.emplace(L"ch");
	this->national.emplace(L"ci");
	this->national.emplace(L"ck");
	this->national.emplace(L"cl");
	this->national.emplace(L"cm");
	this->national.emplace(L"cn");
	this->national.emplace(L"co");
	this->national.emplace(L"cr");
	this->national.emplace(L"cu");
	this->national.emplace(L"cv");
	this->national.emplace(L"cx");
	this->national.emplace(L"cy");
	this->national.emplace(L"cz");
	this->national.emplace(L"dd");
	this->national.emplace(L"de");
	this->national.emplace(L"dj");
	this->national.emplace(L"dk");
	this->national.emplace(L"dm");
	this->national.emplace(L"do");
	this->national.emplace(L"dz");
	this->national.emplace(L"ec");
	this->national.emplace(L"ee");
	this->national.emplace(L"eg");
	this->national.emplace(L"er");
	this->national.emplace(L"es");
	this->national.emplace(L"et");
	this->national.emplace(L"eu");
	this->national.emplace(L"fi");
	this->national.emplace(L"fj");
	this->national.emplace(L"fk");
	this->national.emplace(L"fm");
	this->national.emplace(L"fo");
	this->national.emplace(L"fr");
	this->national.emplace(L"ga");
	this->national.emplace(L"gb");
	this->national.emplace(L"gd");
	this->national.emplace(L"ge");
	this->national.emplace(L"gf");
	this->national.emplace(L"gg");
	this->national.emplace(L"gh");
	this->national.emplace(L"gi");
	this->national.emplace(L"gl");
	this->national.emplace(L"gm");
	this->national.emplace(L"gn");
	this->national.emplace(L"gp");
	this->national.emplace(L"gq");
	this->national.emplace(L"gr");
	this->national.emplace(L"gs");
	this->national.emplace(L"gt");
	this->national.emplace(L"gu");
	this->national.emplace(L"gw");
	this->national.emplace(L"gy");
	this->national.emplace(L"hk");
	this->national.emplace(L"hm");
	this->national.emplace(L"hn");
	this->national.emplace(L"hr");
	this->national.emplace(L"ht");
	this->national.emplace(L"hu");
	this->national.emplace(L"id");
	this->national.emplace(L"ie");
	this->national.emplace(L"il");
	this->national.emplace(L"im");
	this->national.emplace(L"in");
	this->national.emplace(L"io");
	this->national.emplace(L"iq");
	this->national.emplace(L"ir");
	this->national.emplace(L"is");
	this->national.emplace(L"it");
	this->national.emplace(L"je");
	this->national.emplace(L"jm");
	this->national.emplace(L"jo");
	this->national.emplace(L"jp");
	this->national.emplace(L"ke");
	this->national.emplace(L"kg");
	this->national.emplace(L"kh");
	this->national.emplace(L"ki");
	this->national.emplace(L"km");
	this->national.emplace(L"kn");
	this->national.emplace(L"kp");
	this->national.emplace(L"kr");
	this->national.emplace(L"kw");
	this->national.emplace(L"ky");
	this->national.emplace(L"kz");
	this->national.emplace(L"la");
	this->national.emplace(L"lb");
	this->national.emplace(L"lc");
	this->national.emplace(L"li");
	this->national.emplace(L"lk");
	this->national.emplace(L"lr");
	this->national.emplace(L"ls");
	this->national.emplace(L"lt");
	this->national.emplace(L"lu");
	this->national.emplace(L"lv");
	this->national.emplace(L"ly");
	this->national.emplace(L"ma");
	this->national.emplace(L"mc");
	this->national.emplace(L"md");
	this->national.emplace(L"me");
	this->national.emplace(L"mg");
	this->national.emplace(L"mh");
	this->national.emplace(L"mk");
	this->national.emplace(L"ml");
	this->national.emplace(L"mm");
	this->national.emplace(L"mn");
	this->national.emplace(L"mo");
	this->national.emplace(L"mp");
	this->national.emplace(L"mq");
	this->national.emplace(L"mr");
	this->national.emplace(L"ms");
	this->national.emplace(L"mt");
	this->national.emplace(L"mu");
	this->national.emplace(L"mv");
	this->national.emplace(L"mw");
	this->national.emplace(L"mx");
	this->national.emplace(L"my");
	this->national.emplace(L"mz");
	this->national.emplace(L"na");
	this->national.emplace(L"nc");
	this->national.emplace(L"ne");
	this->national.emplace(L"nf");
	this->national.emplace(L"ng");
	this->national.emplace(L"ni");
	this->national.emplace(L"nl");
	this->national.emplace(L"no");
	this->national.emplace(L"np");
	this->national.emplace(L"nr");
	this->national.emplace(L"nu");
	this->national.emplace(L"nz");
	this->national.emplace(L"om");
	this->national.emplace(L"pa");
	this->national.emplace(L"pe");
	this->national.emplace(L"pf");
	this->national.emplace(L"pg");
	this->national.emplace(L"ph");
	this->national.emplace(L"pk");
	this->national.emplace(L"pl");
	this->national.emplace(L"pm");
	this->national.emplace(L"pn");
	this->national.emplace(L"pr");
	this->national.emplace(L"ps");
	this->national.emplace(L"pt");
	this->national.emplace(L"pw");
	this->national.emplace(L"py");
	this->national.emplace(L"qa");
	this->national.emplace(L"re");
	this->national.emplace(L"ro");
	this->national.emplace(L"rs");
	this->national.emplace(L"ru");
	this->national.emplace(L"рф");
	this->national.emplace(L"ру");
	this->national.emplace(L"су");
	this->national.emplace(L"rw");
	this->national.emplace(L"sa");
	this->national.emplace(L"sb");
	this->national.emplace(L"sc");
	this->national.emplace(L"sd");
	this->national.emplace(L"se");
	this->national.emplace(L"sg");
	this->national.emplace(L"sh");
	this->national.emplace(L"si");
	this->national.emplace(L"sj");
	this->national.emplace(L"sk");
	this->national.emplace(L"sl");
	this->national.emplace(L"sm");
	this->national.emplace(L"sn");
	this->national.emplace(L"so");
	this->national.emplace(L"sr");
	this->national.emplace(L"st");
	this->national.emplace(L"su");
	this->national.emplace(L"sv");
	this->national.emplace(L"sy");
	this->national.emplace(L"sz");
	this->national.emplace(L"tc");
	this->national.emplace(L"td");
	this->national.emplace(L"tf");
	this->national.emplace(L"tg");
	this->national.emplace(L"th");
	this->national.emplace(L"tj");
	this->national.emplace(L"tk");
	this->national.emplace(L"tl");
	this->national.emplace(L"tm");
	this->national.emplace(L"tn");
	this->national.emplace(L"to");
	this->national.emplace(L"tp");
	this->national.emplace(L"tr");
	this->national.emplace(L"tt");
	this->national.emplace(L"tv");
	this->national.emplace(L"tw");
	this->national.emplace(L"tz");
	this->national.emplace(L"ua");
	this->national.emplace(L"ug");
	this->national.emplace(L"uk");
	this->national.emplace(L"us");
	this->national.emplace(L"uy");
	this->national.emplace(L"uz");
	this->national.emplace(L"va");
	this->national.emplace(L"vc");
	this->national.emplace(L"ve");
	this->national.emplace(L"vg");
	this->national.emplace(L"vi");
	this->national.emplace(L"vn");
	this->national.emplace(L"vu");
	this->national.emplace(L"wf");
	this->national.emplace(L"ws");
	this->national.emplace(L"ye");
	this->national.emplace(L"yt");
	this->national.emplace(L"za");
	this->national.emplace(L"zm");
	this->national.emplace(L"zw");
	this->national.emplace(L"krd");
	this->national.emplace(L"укр");
	this->national.emplace(L"срб");
	this->national.emplace(L"мон");
	this->national.emplace(L"бел");
	this->national.emplace(L"ком");
	this->national.emplace(L"нет");
	this->national.emplace(L"биз");
	this->national.emplace(L"орг");
	this->national.emplace(L"инфо");
	// Создаем список общих доменов
	this->general.emplace(L"app");
	this->general.emplace(L"biz");
	this->general.emplace(L"cat");
	this->general.emplace(L"com");
	this->general.emplace(L"edu");
	this->general.emplace(L"eus");
	this->general.emplace(L"gov");
	this->general.emplace(L"int");
	this->general.emplace(L"mil");
	this->general.emplace(L"net");
	this->general.emplace(L"one");
	this->general.emplace(L"ong");
	this->general.emplace(L"onl");
	this->general.emplace(L"ooo");
	this->general.emplace(L"org");
	this->general.emplace(L"pro");
	this->general.emplace(L"red");
	this->general.emplace(L"ren");
	this->general.emplace(L"tel");
	this->general.emplace(L"xxx");
	this->general.emplace(L"xyz");
	this->general.emplace(L"rent");
	this->general.emplace(L"name");
	this->general.emplace(L"aero");
	this->general.emplace(L"mobi");
	this->general.emplace(L"jobs");
	this->general.emplace(L"info");
	this->general.emplace(L"coop");
	this->general.emplace(L"asia");
	this->general.emplace(L"army");
	this->general.emplace(L"pics");
	this->general.emplace(L"pink");
	this->general.emplace(L"plus");
	this->general.emplace(L"porn");
	this->general.emplace(L"post");
	this->general.emplace(L"prof");
	this->general.emplace(L"qpon");
	this->general.emplace(L"rest");
	this->general.emplace(L"rich");
	this->general.emplace(L"site");
	this->general.emplace(L"yoga");
	this->general.emplace(L"zone");
	this->general.emplace(L"rehab");
	this->general.emplace(L"press");
	this->general.emplace(L"poker");
	this->general.emplace(L"parts");
	this->general.emplace(L"party");
	this->general.emplace(L"audio");
	this->general.emplace(L"archi");
	this->general.emplace(L"dance");
	this->general.emplace(L"actor");
	this->general.emplace(L"adult");
	this->general.emplace(L"photo");
	this->general.emplace(L"pizza");
	this->general.emplace(L"place");
	this->general.emplace(L"travel");
	this->general.emplace(L"review");
	this->general.emplace(L"repair");
	this->general.emplace(L"report");
	this->general.emplace(L"racing");
	this->general.emplace(L"photos");
	this->general.emplace(L"physio");
	this->general.emplace(L"online");
	this->general.emplace(L"museum");
	this->general.emplace(L"agency");
	this->general.emplace(L"active");
	this->general.emplace(L"reviews");
	this->general.emplace(L"rentals");
	this->general.emplace(L"recipes");
	this->general.emplace(L"organic");
	this->general.emplace(L"academy");
	this->general.emplace(L"auction");
	this->general.emplace(L"plumbing");
	this->general.emplace(L"pharmacy");
	this->general.emplace(L"airforce");
	this->general.emplace(L"attorney");
	this->general.emplace(L"partners");
	this->general.emplace(L"pictures");
	this->general.emplace(L"feedback");
	this->general.emplace(L"property");
	this->general.emplace(L"republican");
	this->general.emplace(L"associates");
	this->general.emplace(L"apartments");
	this->general.emplace(L"accountant");
	this->general.emplace(L"properties");
	this->general.emplace(L"photography");
	this->general.emplace(L"accountants");
	this->general.emplace(L"productions");
	// Если буквы переданы запоминаем их
	this->setLetters(letters);
	// Если текст передан то выполняем парсинг
	if(!text.empty()) this->parse(text);
}

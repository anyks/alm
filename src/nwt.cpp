/**
 *  author:   Yuriy Lobarev
 *  telegram: @forman
 *  phone:    +7(910)983-95-90
 *  email:    forman@anyks.com
 *  site:     https://anyks.com
 */

#include "nwt.hpp"

/**
 * get Метод получения распарсенных данных
 * @return параметры полученные в результате парсинга
 */
const anyks::Uri::data_t & anyks::Uri::get(){
	// Выводим результат парсинга
	return this->data;
}
/**
 * clear Метод очистки результатов парсинга
 */
void anyks::Uri::clear(){
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
void anyks::Uri::parse(const wstring & text){
	// Если текст передан
	if(!text.empty()){
		/**
		 * emailFn Функция извлечения данных электронного адреса
		 * @param text текст для парсинга
		 */
		auto emailFn = [this](const wstring & text){
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
					result.uri = move(match[1].str());
					// Запоминаем логин пользователя
					result.user = move(match[2].str());
					// Запоминаем название электронного ящика
					result.data = move(match[3].str());
					// Запоминаем домен верхнего уровня
					result.domain = move(match[4].str());
				}
			}
			// Выводим результат
			return result;
		};
		/**
		 * domainFn Функция извлечения данных доменного имени
		 * @param text текст для парсинга
		 */
		auto domainFn = [this](const wstring & text){
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
					result.uri = move(match[0].str());
					// Запоминаем название домена
					result.data = move(match[2].str());
					// Запоминаем порт запроса
					result.port = move(match[4].str());
					// Запоминаем путь запроса
					result.path = move(match[5].str());
					// Запоминаем домен верхнего уровня
					result.domain = move(match[3].str());
					// Запоминаем параметры запроса
					result.params = move(match[6].str());
					// Запоминаем протокол
					result.protocol = move(match[1].str());
					// Если протокол не указан
					if(result.protocol.empty()){
						// Если домен верхнего уровня не является таковым, очищаем все
						if((this->national.count(result.domain) < 1)
						&& (this->general.count(result.domain) < 1)){
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
		auto ipFn = [](const wstring & text){
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
					result.uri = move(match[0].str());
					// Извлекаем полученные данные
					const wstring & mac = move(match[2].str());
					const wstring & ip4 = move(match[4].str());
					const wstring & ip6 = move(match[3].str());
					const wstring & network = move(match[1].str());
					// Если это MAC адрес
					if(!mac.empty()){
						// Запоминаем сам параметр
						result.data = move(mac);
						// Запоминаем тип параметра
						result.type = types_t::mac;
					// Если это IPv4 адрес
					} else if(!ip4.empty()){
						// Запоминаем сам параметр
						result.data = move(ip4);
						// Запоминаем тип параметра
						result.type = types_t::ipv4;
					// Если это IPv6 адрес
					} else if(!ip6.empty()) {
						// Запоминаем сам параметр
						result.data = move(ip6);
						// Запоминаем тип параметра
						result.type = types_t::ipv6;
					// Если это параметры сети
					} else if(!network.empty()) {
						// Запоминаем сам параметр
						result.data = move(network);
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
		data_t email = move(emailFn(text));
		// Если тип не получен
		if(email.type == types_t::null){
			// Запрашиваем данные доменного имени
			data_t domain = move(domainFn(text));
			// Если данные домена не получены или протокол не найден
			if((domain.type == types_t::null)
			|| (domain.type == types_t::wrong)
			|| domain.protocol.empty()){
				// Выполняем поиск ip адресов
				data_t ip = move(ipFn(text));
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
 * setLetters Метод добавления букв алфавита
 * @param letters список букв алфавита
 */
void anyks::Uri::setLetters(const wstring & letters){
	// Если буквы переданы запоминаем их
	if(!letters.empty()) this->letters = move(letters);
}
/**
 * Uri Конструктор
 * @param letters список букв алфавита
 * @param text    текст для парсинга
 */
anyks::Uri::Uri(const wstring & letters, const wstring & text){
	// Создаем список национальных доменов
	this->national.insert(L"ac");
	this->national.insert(L"ad");
	this->national.insert(L"ae");
	this->national.insert(L"af");
	this->national.insert(L"ag");
	this->national.insert(L"ai");
	this->national.insert(L"al");
	this->national.insert(L"am");
	this->national.insert(L"an");
	this->national.insert(L"ao");
	this->national.insert(L"aq");
	this->national.insert(L"ar");
	this->national.insert(L"as");
	this->national.insert(L"at");
	this->national.insert(L"au");
	this->national.insert(L"aw");
	this->national.insert(L"ax");
	this->national.insert(L"az");
	this->national.insert(L"ba");
	this->national.insert(L"bb");
	this->national.insert(L"bd");
	this->national.insert(L"be");
	this->national.insert(L"bf");
	this->national.insert(L"bg");
	this->national.insert(L"bh");
	this->national.insert(L"bi");
	this->national.insert(L"bj");
	this->national.insert(L"bm");
	this->national.insert(L"bn");
	this->national.insert(L"bo");
	this->national.insert(L"br");
	this->national.insert(L"bs");
	this->national.insert(L"bt");
	this->national.insert(L"bv");
	this->national.insert(L"bw");
	this->national.insert(L"by");
	this->national.insert(L"bz");
	this->national.insert(L"ca");
	this->national.insert(L"cc");
	this->national.insert(L"cd");
	this->national.insert(L"cf");
	this->national.insert(L"cg");
	this->national.insert(L"ch");
	this->national.insert(L"ci");
	this->national.insert(L"ck");
	this->national.insert(L"cl");
	this->national.insert(L"cm");
	this->national.insert(L"cn");
	this->national.insert(L"co");
	this->national.insert(L"cr");
	this->national.insert(L"cu");
	this->national.insert(L"cv");
	this->national.insert(L"cx");
	this->national.insert(L"cy");
	this->national.insert(L"cz");
	this->national.insert(L"dd");
	this->national.insert(L"de");
	this->national.insert(L"dj");
	this->national.insert(L"dk");
	this->national.insert(L"dm");
	this->national.insert(L"do");
	this->national.insert(L"dz");
	this->national.insert(L"ec");
	this->national.insert(L"ee");
	this->national.insert(L"eg");
	this->national.insert(L"er");
	this->national.insert(L"es");
	this->national.insert(L"et");
	this->national.insert(L"eu");
	this->national.insert(L"fi");
	this->national.insert(L"fj");
	this->national.insert(L"fk");
	this->national.insert(L"fm");
	this->national.insert(L"fo");
	this->national.insert(L"fr");
	this->national.insert(L"ga");
	this->national.insert(L"gb");
	this->national.insert(L"gd");
	this->national.insert(L"ge");
	this->national.insert(L"gf");
	this->national.insert(L"gg");
	this->national.insert(L"gh");
	this->national.insert(L"gi");
	this->national.insert(L"gl");
	this->national.insert(L"gm");
	this->national.insert(L"gn");
	this->national.insert(L"gp");
	this->national.insert(L"gq");
	this->national.insert(L"gr");
	this->national.insert(L"gs");
	this->national.insert(L"gt");
	this->national.insert(L"gu");
	this->national.insert(L"gw");
	this->national.insert(L"gy");
	this->national.insert(L"hk");
	this->national.insert(L"hm");
	this->national.insert(L"hn");
	this->national.insert(L"hr");
	this->national.insert(L"ht");
	this->national.insert(L"hu");
	this->national.insert(L"id");
	this->national.insert(L"ie");
	this->national.insert(L"il");
	this->national.insert(L"im");
	this->national.insert(L"in");
	this->national.insert(L"io");
	this->national.insert(L"iq");
	this->national.insert(L"ir");
	this->national.insert(L"is");
	this->national.insert(L"it");
	this->national.insert(L"je");
	this->national.insert(L"jm");
	this->national.insert(L"jo");
	this->national.insert(L"jp");
	this->national.insert(L"ke");
	this->national.insert(L"kg");
	this->national.insert(L"kh");
	this->national.insert(L"ki");
	this->national.insert(L"km");
	this->national.insert(L"kn");
	this->national.insert(L"kp");
	this->national.insert(L"kr");
	this->national.insert(L"kw");
	this->national.insert(L"ky");
	this->national.insert(L"kz");
	this->national.insert(L"la");
	this->national.insert(L"lb");
	this->national.insert(L"lc");
	this->national.insert(L"li");
	this->national.insert(L"lk");
	this->national.insert(L"lr");
	this->national.insert(L"ls");
	this->national.insert(L"lt");
	this->national.insert(L"lu");
	this->national.insert(L"lv");
	this->national.insert(L"ly");
	this->national.insert(L"ma");
	this->national.insert(L"mc");
	this->national.insert(L"md");
	this->national.insert(L"me");
	this->national.insert(L"mg");
	this->national.insert(L"mh");
	this->national.insert(L"mk");
	this->national.insert(L"ml");
	this->national.insert(L"mm");
	this->national.insert(L"mn");
	this->national.insert(L"mo");
	this->national.insert(L"mp");
	this->national.insert(L"mq");
	this->national.insert(L"mr");
	this->national.insert(L"ms");
	this->national.insert(L"mt");
	this->national.insert(L"mu");
	this->national.insert(L"mv");
	this->national.insert(L"mw");
	this->national.insert(L"mx");
	this->national.insert(L"my");
	this->national.insert(L"mz");
	this->national.insert(L"na");
	this->national.insert(L"nc");
	this->national.insert(L"ne");
	this->national.insert(L"nf");
	this->national.insert(L"ng");
	this->national.insert(L"ni");
	this->national.insert(L"nl");
	this->national.insert(L"no");
	this->national.insert(L"np");
	this->national.insert(L"nr");
	this->national.insert(L"nu");
	this->national.insert(L"nz");
	this->national.insert(L"om");
	this->national.insert(L"pa");
	this->national.insert(L"pe");
	this->national.insert(L"pf");
	this->national.insert(L"pg");
	this->national.insert(L"ph");
	this->national.insert(L"pk");
	this->national.insert(L"pl");
	this->national.insert(L"pm");
	this->national.insert(L"pn");
	this->national.insert(L"pr");
	this->national.insert(L"ps");
	this->national.insert(L"pt");
	this->national.insert(L"pw");
	this->national.insert(L"py");
	this->national.insert(L"qa");
	this->national.insert(L"re");
	this->national.insert(L"ro");
	this->national.insert(L"rs");
	this->national.insert(L"ru");
	this->national.insert(L"рф");
	this->national.insert(L"rw");
	this->national.insert(L"sa");
	this->national.insert(L"sb");
	this->national.insert(L"sc");
	this->national.insert(L"sd");
	this->national.insert(L"se");
	this->national.insert(L"sg");
	this->national.insert(L"sh");
	this->national.insert(L"si");
	this->national.insert(L"sj");
	this->national.insert(L"sk");
	this->national.insert(L"sl");
	this->national.insert(L"sm");
	this->national.insert(L"sn");
	this->national.insert(L"so");
	this->national.insert(L"sr");
	this->national.insert(L"st");
	this->national.insert(L"su");
	this->national.insert(L"sv");
	this->national.insert(L"sy");
	this->national.insert(L"sz");
	this->national.insert(L"tc");
	this->national.insert(L"td");
	this->national.insert(L"tf");
	this->national.insert(L"tg");
	this->national.insert(L"th");
	this->national.insert(L"tj");
	this->national.insert(L"tk");
	this->national.insert(L"tl");
	this->national.insert(L"tm");
	this->national.insert(L"tn");
	this->national.insert(L"to");
	this->national.insert(L"tp");
	this->national.insert(L"tr");
	this->national.insert(L"tt");
	this->national.insert(L"tv");
	this->national.insert(L"tw");
	this->national.insert(L"tz");
	this->national.insert(L"ua");
	this->national.insert(L"ug");
	this->national.insert(L"uk");
	this->national.insert(L"us");
	this->national.insert(L"uy");
	this->national.insert(L"uz");
	this->national.insert(L"va");
	this->national.insert(L"vc");
	this->national.insert(L"ve");
	this->national.insert(L"vg");
	this->national.insert(L"vi");
	this->national.insert(L"vn");
	this->national.insert(L"vu");
	this->national.insert(L"wf");
	this->national.insert(L"ws");
	this->national.insert(L"ye");
	this->national.insert(L"yt");
	this->national.insert(L"za");
	this->national.insert(L"zm");
	this->national.insert(L"zw");
	this->national.insert(L"krd");
	this->national.insert(L"укр");
	this->national.insert(L"срб");
	this->national.insert(L"мон");
	this->national.insert(L"бел");
	// Создаем список общих доменов
	this->general.insert(L"app");
	this->general.insert(L"biz");
	this->general.insert(L"cat");
	this->general.insert(L"com");
	this->general.insert(L"edu");
	this->general.insert(L"eus");
	this->general.insert(L"gov");
	this->general.insert(L"int");
	this->general.insert(L"mil");
	this->general.insert(L"net");
	this->general.insert(L"one");
	this->general.insert(L"ong");
	this->general.insert(L"onl");
	this->general.insert(L"ooo");
	this->general.insert(L"org");
	this->general.insert(L"pro");
	this->general.insert(L"red");
	this->general.insert(L"ren");
	this->general.insert(L"tel");
	this->general.insert(L"xxx");
	this->general.insert(L"xyz");
	this->general.insert(L"rent");
	this->general.insert(L"name");
	this->general.insert(L"aero");
	this->general.insert(L"mobi");
	this->general.insert(L"jobs");
	this->general.insert(L"info");
	this->general.insert(L"coop");
	this->general.insert(L"asia");
	this->general.insert(L"army");
	this->general.insert(L"pics");
	this->general.insert(L"pink");
	this->general.insert(L"plus");
	this->general.insert(L"porn");
	this->general.insert(L"post");
	this->general.insert(L"prof");
	this->general.insert(L"qpon");
	this->general.insert(L"rest");
	this->general.insert(L"rich");
	this->general.insert(L"site");
	this->general.insert(L"yoga");
	this->general.insert(L"zone");
	this->general.insert(L"rehab");
	this->general.insert(L"press");
	this->general.insert(L"poker");
	this->general.insert(L"parts");
	this->general.insert(L"party");
	this->general.insert(L"audio");
	this->general.insert(L"archi");
	this->general.insert(L"dance");
	this->general.insert(L"actor");
	this->general.insert(L"adult");
	this->general.insert(L"photo");
	this->general.insert(L"pizza");
	this->general.insert(L"place");
	this->general.insert(L"travel");
	this->general.insert(L"review");
	this->general.insert(L"repair");
	this->general.insert(L"report");
	this->general.insert(L"racing");
	this->general.insert(L"photos");
	this->general.insert(L"physio");
	this->general.insert(L"online");
	this->general.insert(L"museum");
	this->general.insert(L"agency");
	this->general.insert(L"active");
	this->general.insert(L"reviews");
	this->general.insert(L"rentals");
	this->general.insert(L"recipes");
	this->general.insert(L"organic");
	this->general.insert(L"academy");
	this->general.insert(L"auction");
	this->general.insert(L"plumbing");
	this->general.insert(L"pharmacy");
	this->general.insert(L"airforce");
	this->general.insert(L"attorney");
	this->general.insert(L"partners");
	this->general.insert(L"pictures");
	this->general.insert(L"feedback");
	this->general.insert(L"property");
	this->general.insert(L"republican");
	this->general.insert(L"associates");
	this->general.insert(L"apartments");
	this->general.insert(L"accountant");
	this->general.insert(L"properties");
	this->general.insert(L"photography");
	this->general.insert(L"accountants");
	this->general.insert(L"productions");
	// Если буквы переданы запоминаем их
	this->setLetters(letters);
	// Если текст передан то выполняем парсинг
	if(!text.empty()) this->parse(text);
}

/* Модель ALM */
%module(directors="1") alm

/* Подключаем стандартыне контейнеры */
%include <std_set.i>
%include <std_map.i>
%include <std_pair.i>
%include <std_vector.i>
%include <std_string.i>
%include <std_wstring.i>

/* Создаём шаблоны стандартных типов данных */
namespace std {
	%template(ALM_SetNumber) set <size_t>;
	%template(ALM_VectorNumber) vector <size_t>;
	%template(ALM_VectorString) vector <string>;
	%template(ALM_PairBoolean) pair <bool, bool>;
	%template(ALM_VectorWString) vector <wstring>;
	%template(ALM_MapNumber) map <size_t, size_t>;
	%template(ALM_MapString) map <string, string>;
	%template(ALM_PairBoolNumber) pair <bool, size_t>;
}
/* Подключаем языковую модель */
%{
#include "alm.hpp"
%}

/* Подключаем собственные структуры данных */
%feature("director") anyks::Callback;
%feature("director") anyks::Perplexity;

/* Создаём шаблон языковой модели */
%include "alm.hpp"

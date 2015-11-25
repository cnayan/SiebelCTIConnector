#include "Param.h"
#include "Utils.h"

Param::Param(JobType jt, const struct ISC_KVParamList* const list, const ISC_STRING& name)
	: jt(jt), commandName(TW(name))
{
	for (int i = 0; i < list->len; i++)
	{
		this->list[wstring(TW(list->dataItems[i].paramName))] = wstring(TW(list->dataItems[i].paramValue));
	}
}

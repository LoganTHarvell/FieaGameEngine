#include "pch.h"
#include "GameException.h"

namespace Library
{
	GameException::GameException(const char* const message, HRESULT hr) :
		exception(message), mHR(hr)
	{
	}

	HRESULT GameException::HR() const
	{
		return mHR;
	}

	std::wstring GameException::whatw() const
	{
		std::wstringstream whatw;
		whatw << what();
		return whatw.str();
	}
}
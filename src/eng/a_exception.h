#pragma once
#include <exception>
#include <iostream>
#include <string>
#include "global.h"

// CTauAudException
// Exception thrown by OpenAL subroutines.
class CTauAudException : public std::exception
{
public:
	CTauAudException(const char* message);
	inline const char* what() const throw()
	{
		return message;
	}
private:
	const char* message;
};


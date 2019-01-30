#pragma once
#include <exception>
#include <iostream>
#include <string>
#include "../global.h"

// CTauGraException
// Exception thrown by graphics subroutines.
class CTauGraException : public std::exception
{
public:
	CTauGraException(const char* message);
	inline const char* what() const throw()
	{
		return message;
	}
private:
	const char* message;
};


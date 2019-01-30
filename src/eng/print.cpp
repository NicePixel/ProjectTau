#include <iostream>
#include "print.h"

void tau_print(const std::string& message, const std::string& subname, int reason)
{
	const char* prefix = "[inf] ";
	if (reason == TED_PRINTREASON_ERROR)
		prefix = "[>:(] ";
	std::cout << prefix << '[' << subname << "] " << message << std::endl;
}

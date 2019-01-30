#include "io.h"
#include <fstream>
#include <sstream>
#include <exception>
#include "print.h"

#undef TED_CURSUB
#define TED_CURSUB "tau_io_file_*"
std::string tau_io_file_allcontent(const char* filepath)
{
	std::ifstream stream(filepath);
	if (!stream)
	{
		std::string err("Failed to load file ");
		err += filepath;
		TED_PRINT_ERROR(err);
		throw std::runtime_error(err);
	}
	std::stringstream buffer;
	buffer << stream.rdbuf();
	return buffer.str();
}

#include "io.h"
#include "configuration.h"

nlohmann::json tau_config_load(const char* filepath)
{
	std::string filecontent = tau_io_file_allcontent(filepath);
	nlohmann::json jobject  = nlohmann::json::parse(filecontent);
	return jobject;
}

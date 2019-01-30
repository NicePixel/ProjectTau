#pragma once
#include <string>
#include "global.h"

#define TED_PRINTREASON_INFO  0
#define TED_PRINTREASON_ERROR 1
#define TED_NOSUB  "NOSUB"
#define TED_CURSUB TED_NOSUB
#define TED_PRINT_INFO(str)  tau_print(str, TED_CURSUB, TED_PRINTREASON_INFO)
#define TED_PRINT_ERROR(str) tau_print(str, TED_CURSUB, TED_PRINTREASON_ERROR)

void tau_print(const std::string& message, const std::string& subname = TED_NOSUB, int reason = TED_PRINTREASON_INFO);


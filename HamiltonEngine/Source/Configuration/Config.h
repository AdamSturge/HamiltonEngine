#pragma once

#include <iostream> //TODO replace with final logging library. Used for json exceptions

//from nlohmann json
 #define JSON_TRY_USER if(true)
 #define JSON_CATCH_USER(exception) if(false)
 #define JSON_THROW_USER(exception)                           \
     {std::clog << "Error in " << __FILE__ << ":" << __LINE__ \
                << " (function " << __FUNCTION__ << ") - "    \
                << (exception).what() << std::endl;}
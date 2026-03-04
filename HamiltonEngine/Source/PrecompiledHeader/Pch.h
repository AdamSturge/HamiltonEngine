#pragma once

// stl and standard C++ libs
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#define _USE_MATH_DEFINES
#include <math.h>

//Config.h before libarries since it configures some via macros
#include "Configuration/Config.h"

//Libraries
#include <nlohmann/json.hpp>
#include <entt/entt.hpp>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <glad/glad.h>
#include <glfw/include/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define FMT_UNICODE 0
#include <spdlog/spdlog.h>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

//Our code. Add stuff that doesn't change often
//#include "Configuration/Globals.h"


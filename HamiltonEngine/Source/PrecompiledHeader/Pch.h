#pragma once

// stl and standard C++ libs
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <ostream>
#include <fstream>
#include <sstream>
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

//Our code. Add stuff that doesn't change often
//#include "Configuration/Globals.h"


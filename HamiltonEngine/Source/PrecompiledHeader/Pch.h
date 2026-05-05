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
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

//Config.h before libarries since it configures some via macros
#include "Configuration/Config.h"

//Libraries
#include <cereal/cereal.hpp>
#include <cereal/version.hpp>
#include <cereal/macros.hpp>
#include <cereal/access.hpp>
#include <cereal/specialize.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/details/helpers.hpp>
#include <cereal/archives/json.hpp>
#include <nlohmann/json.hpp>
#include <entt/entt.hpp>
#include <entt/meta/meta.hpp>
#include <entt/core/hashed_string.hpp>
#include <entt/core/type_traits.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/snapshot.hpp>
#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <glad/glad.h>
#include <glfw/include/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Our code. Add stuff that doesn't change often
#include "Logging/Logging.h"
#include "ECS/EntityToUnderlyingType.h"
#include "Serialization/EigenSerialization.h"


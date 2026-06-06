#pragma once
#include "PrecompiledHeader/Pch.h"
#include <RenderingSystem/RenderingSystemBase.h>
#define _USE_MATH_DEFINES
#include <math.h>

// This is purely a collection og random helper methods and utils.

float DegToRad(float deg);
unsigned RandomRange(unsigned range);
HamiltonEngine::RenderingSystem::TransformComponent RandomTransformComponent();
void PrintOpenGLBufferComponentData(HamiltonEngine::RenderingSystem::OpenGLBuffersComponent buff);
void PrintTransformComponent(HamiltonEngine::RenderingSystem::TransformComponent Trans);
void PopulatePrimativeMap();
void CreateTestObjects();
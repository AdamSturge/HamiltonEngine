#pragma once
#include "PrecompiledHeader/Pch.h"
#include <OpenGL/OpenGL.h>
#define _USE_MATH_DEFINES
#include <math.h>

// This is purely a collection og random helper methods and utils.

float DegToRad(float deg);
unsigned RandomRange(unsigned range);
HamiltonEngine::OpenGL::TransformComponent RandomTransformComponent();
void PrintOpenGLBufferComponentData(HamiltonEngine::OpenGL::OpenGLBuffersComponent buff);
void PrintTransformComponent(HamiltonEngine::OpenGL::TransformComponent Trans);
void createTestObjects();
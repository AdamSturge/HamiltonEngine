#pragma once

namespace HamiltonEngine::OpenGL
{
	OpenGLBuffersComponent CreateSphereBuffers(float Radius, int StackCount, int SectorCount);
	entt::entity CreateSphereEntity(TransformComponent trans, float Radius, int StackCount, int SectorCount);
	entt::entity CreateSphereEntity(TransformComponent Trans, OpenGLBuffersComponent Buffs);

}
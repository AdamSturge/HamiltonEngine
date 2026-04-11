#pragma once

#define ENTIY_TO_UNDERLYING_TYPE(Entity) entt::entt_traits<entt::entity>::to_entity(Entity)
#define ENTIY_TO_VERSION(Entity) entt::entt_traits<entt::entity>::to_version(Entity)
#define ENTIY_HADNLE_TO_UNDERLYING_TYPE(Handle) entt::entt_traits<entt::entity>::to_entity(Handle.entity())
#define ENTIY_HADNLE_TO_VERSION(Handle) entt::entt_traits<entt::entity>::to_version(Handle.entity())
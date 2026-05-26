#include "PrecompiledHeader/Pch.h"

#include "JsonSerialization.h"
#include "Physics/State/RigidBodyState.h"
#include "Physics/Potentials/ConstantGravityPotential.h"
#include "Physics/Potentials/SpringPotential.h"

namespace HamiltonEngine::Serialization
{
    using namespace entt;
	/**TODO:
	* 1.Switch to using arrays [count, elem1, elem2, ...]
	* 2 Update code to use our style for variables, brackets, etc
	* 3 remove using statement for entt
	**/
    
    template<typename Registry>
    class CerealJsonSnapshot 
    {
        static_assert(!std::is_const_v<Registry>, "Non-const registry type required");
        using traits_type = entt_traits<typename Registry::entity_type>;

    public:
        using registry_type = Registry;

        using entity_type = typename registry_type::entity_type;

        CerealJsonSnapshot(const registry_type& source) noexcept
            : reg{ &source } {
        }

        CerealJsonSnapshot(const CerealJsonSnapshot&) = delete;

        CerealJsonSnapshot(CerealJsonSnapshot&&) noexcept = default;

        ~CerealJsonSnapshot() = default;

        CerealJsonSnapshot& operator=(const CerealJsonSnapshot&) = delete;

        CerealJsonSnapshot& operator=(CerealJsonSnapshot&&) noexcept = default;

        template<typename Type>
        const CerealJsonSnapshot& get(cereal::JSONOutputArchive& archive, const char* key, const id_type id = type_hash<Type>::value()) const {
            if (const auto* storage = reg->template storage<Type>(id); storage) {
                const typename registry_type::common_type& base = *storage;

                std::string CountString = key;
				CountString.append("Count");

                archive(cereal::make_nvp(CountString.c_str(), static_cast<typename traits_type::entity_type>(storage->size())));

                if constexpr (std::is_same_v<Type, entity_type>) {
                    archive(cereal::make_nvp("FreeList", static_cast<typename traits_type::entity_type>(storage->free_list())));

                    for (auto first = base.rbegin(), last = base.rend(); first != last; ++first) {
                        archive(cereal::make_nvp("EntityId", *first));
                    }
                }
                else if constexpr (registry_type::template storage_for_type<Type>::storage_policy == deletion_policy::in_place) {
                    for (auto it = base.rbegin(), last = base.rend(); it != last; ++it) {
                        if (const auto entt = *it; entt == tombstone) {
                            archive(cereal::make_nvp("Tombstone",static_cast<entity_type>(null)));
                        }
                        else {
                            //This is likely wrong
							archive(cereal::make_nvp(key,entt));
                            std::apply([&archive, &key](auto &&...args) { (archive(cereal::make_nvp(key,std::forward<decltype(args)>(args))), ...); }, storage->get_as_tuple(entt));
                        }
                    }
                }
                else {
                    for (auto elem : storage->reach()) {
                        std::apply([&archive, &key](auto &&...args) { (archive(cereal::make_nvp(key,std::forward<decltype(args)>(args))), ...); }, elem);
                    }
                }
            }
            else {
                archive(typename traits_type::entity_type{});
            }

            return *this;
        }

    private:
        const registry_type* reg;
    };
	
	bool SerializeEnttRegistryAsJson(const entt::registry& Registry,
		const char* Filename,
		bool Overwrite)
	{
		std::ofstream Filestream;
		try
		{
			if (std::filesystem::exists(Filename))
			{
				if (!Overwrite) 
				{
					HAMILTON_LOG(Serialization,
						Warning,
						"Attempted to save %s but file already exists",
						Filename);
				
					return false;
				}
				
				//it exists so we erase the file contents
				Filestream.open(Filename, std::ofstream::out | std::ofstream::trunc);
			}
			else 
			{
				//Will create new file if it doesn't exist
				Filestream.open(Filename, std::ofstream::out);
			}

		}
		catch (std::filesystem::filesystem_error e)
		{
			HAMILTON_LOG(Serialization,
				Warning,
				"Attempting to open file %s but failed with exception: %s",
				Filename,
				e.what())

				return false;
		}
		
		{
			using namespace entt::literals;
			
			cereal::JSONOutputArchive output{ Filestream };

			//TODO maybe someday we automatically detect the things that go into this list
            CerealJsonSnapshot{ Registry }
				.get<entt::entity>(output, "EntityId")
			.get<Physics::RigidBodyStateComponent>(output, "RigidBodyStateComponent");
				//.get<Physics::RigidBodyGravityComponent>(output)
				//.get<Physics::SpringPotentialComponent>(output);
		}

		Filestream.close();
	}

	bool DeserializeEnttRegistryFromJson(entt::registry& Registry, const char* Filename)
	{
		std::ifstream Filestream;
		try
		{
			if (std::filesystem::exists(Filename))
			{
				Filestream.open(Filename, std::ifstream::in);
			}
			else
			{
				HAMILTON_LOG(Serialization,
					Warning,
					"Attempting to open file but it does not exist %s",
					Filename)

					return false;
			}

		}
		catch (std::filesystem::filesystem_error e)
		{
			HAMILTON_LOG(Serialization,
				Warning,
				"Attempting to open file %s but failed with exception: %s",
				Filename,
				e.what())
				return false;
		}
		
		{
			cereal::JSONInputArchive input{ Filestream };
		
			entt::snapshot_loader{ Registry }
				//.get<entt::entity>(input)
			.get<Physics::RigidBodyStateComponent>(input);
				//.get<Physics::RigidBodyGravityComponent>(input)
				//.get<Physics::SpringPotentialComponent>(input);
		}

		Filestream.close();
		
		return false;
	}
}

#include "PrecompiledHeader/Pch.h"

#include "EigenSerialization.h"

namespace Eigen
{
    void Save(cereal::JSONOutputArchive& Record, const Vector3f& V)
    {
        Record(cereal::make_nvp("X", V.x())
            , cereal::make_nvp("Y", V.y())
            , cereal::make_nvp("Z", V.z()));
    }

    void Load(cereal::JSONInputArchive& Record, Vector3f& V)
    {
        Record(cereal::make_nvp("X", V.x())
            , cereal::make_nvp("Y", V.y())
            , cereal::make_nvp("Z", V.z()));
    }
}
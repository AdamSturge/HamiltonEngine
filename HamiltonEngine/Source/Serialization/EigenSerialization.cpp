#include "PrecompiledHeader/Pch.h"

#include "EigenSerialization.h"
#include "Eigen/EulerAngles.h"

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

    void Save(cereal::JSONOutputArchive& Record, const Affine3f& Transform)
    {
        Record(cereal::make_nvp("Translation", Transform.translation()));
        HamiltonEngine::EulerAngles Angles;
        HamiltonEngine::RotationToEulerAngles(Transform.rotation(), Angles);
        Record(cereal::make_nvp("Rotation", Angles));
    }

    void Load(cereal::JSONInputArchive& Record, Affine3f& Transform)
    {

    }
}
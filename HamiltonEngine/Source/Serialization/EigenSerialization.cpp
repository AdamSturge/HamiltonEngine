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
        Eigen::Vector3f Translation(0.0f,0.0f,0.0f);
        Record(cereal::make_nvp("Translation", Translation));
        
        HamiltonEngine::EulerAngles Angles;
        Record(cereal::make_nvp("Rotation", Angles));
        Eigen::Matrix3f Rotation;
        Rotation.setIdentity();
        HamiltonEngine::EulerAnglesToRotation(Angles.Pitch, Angles.Yaw, Angles.Roll, Rotation);
        
        //The rotation part doesn't seem to be working
        Transform = Transform.prerotate(Rotation).pretranslate(Translation);

        std::cout << Transform.matrix() << std::endl << std::endl;
    }
}
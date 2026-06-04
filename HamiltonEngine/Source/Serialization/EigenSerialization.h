#pragma once
#include <Physics/Integrators/RigidBodySplitting.h>

namespace Eigen
{
    using Diagonal3f = Eigen::DiagonalMatrix<float, 3, 3>;
  
    void Save(cereal::JSONOutputArchive& Record, const Vector3f& V);
    void Load(cereal::JSONInputArchive& Record, Vector3f& V);

    void Save(cereal::JSONOutputArchive& Record, const Affine3f& Transform);
    void Load(cereal::JSONInputArchive& Record, Affine3f& Transform);

    void Save(cereal::JSONOutputArchive& Record, const Diagonal3f& Matrix);
    void Load(cereal::JSONInputArchive& Record, Diagonal3f& Matrix);
}
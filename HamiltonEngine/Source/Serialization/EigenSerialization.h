#pragma once

namespace Eigen
{
    void Save(cereal::JSONOutputArchive& Record, const Vector3f& V);
    void Load(cereal::JSONInputArchive& Record, Vector3f& V);
}
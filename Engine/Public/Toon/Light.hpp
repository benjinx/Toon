#ifndef TOON_LIGHT_H
#define TOON_LIGHT_H

#include <Toon/Config.hpp>
#include <Toon/Entity.hpp>
#include <Toon/Math.hpp>
#include <Toon/ShaderScene.hpp>

namespace Toon {

enum class LightType
{
    Point = 0,
    Spot = 1,
    Directional = 2,

}; // enum class LightType

class TOON_ENGINE_API Light : public Entity
{
public:

    DISALLOW_COPY_AND_ASSIGN(Light)

    Light() = default;

    virtual ~Light() = default;

    void SetType(LightType type);

    inline LightType GetType() const {
        return _type;
    }

    void FillLightData(LightData1 * data);

    void SetColor(glm::vec3 color);

    inline glm::vec3 GetColor() const {
        return _color;
    }

    // Spot
    void SetInnerCutOff(float cutOff);

    inline float GetInnerCutOff() const {
        return _innerCutOff;
    }
    void SetOuterCutOff(float cutOff);

    inline float GetOuterCutOff() const {
        return _outerCutOff;
    }

private:

    LightType _type;

    glm::vec3 _color;

    // Spot

    float _innerCutOff;

    float _outerCutOff;
};

} // namespace Toon

#endif // TOON_LIGHT_H
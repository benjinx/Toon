#include <Toon/Light.hpp>

namespace Toon {

void Light::SetType(LightType type)
{
    _type = type;
}

void Light::FillLightData(LightData1 * data)
{
    data->Type = static_cast<uint8_t>(_type);
    data->Position = GetPosition();
    data->Direction = GetOrientation() * GetWorldForward();
    data->Color = _color;
    data->InnerCutOff = _innerCutOff;
    data->OuterCutOff = _outerCutOff;
}

void Light::SetColor(glm::vec3 color)
{
    _color = color;
}

void Light::SetInnerCutOff(float cutOff)
{
    _innerCutOff = cutOff;
}

void Light::SetOuterCutOff(float cutOff)
{
    _outerCutOff = cutOff;
}

} // namespace Toon
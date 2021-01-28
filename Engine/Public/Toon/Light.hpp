#ifndef LIGHT_H
#define LIGHT_H

#include <Toon/Config.hpp>
#include <Toon/Entity.hpp>
#include <Toon/Math.hpp>

namespace Toon {

class Light : public Entity
{
public:
    Light() = default;
    virtual ~Light() = default;

    void SetColor(glm::vec3 color) { _mColor = color; }
    void SetColor(float r, float g, float b) {  _mColor.r = r;
                                                _mColor.g = g;
                                                _mColor.b = b; }
    glm::vec3 GetColor() { return _mColor; }

    void SetIntensity(int intensity) { _mIntensity = intensity; }
    int GetIntensity() { return _mIntensity; }
    
private:
    glm::vec3 _mColor;
    int _mIntensity;
};

class DirectionalLight : public Light
{
public:
    DirectionalLight() = default;
    /*DirectionalLight(glm::vec3 direction);

    void SetDirection(glm::vec3 direction);
    glm::vec3 GetDirection();*/

    // getQuat * worldforward = dir
};

class PointLight : public Light
{
public:
    PointLight() = default;
    PointLight(glm::vec3 position, float constant, float linear, float quadratic);

    void SetConstant(float constant);
    void SetLinear(float linear);
    void SetQuadratic(float quadratic);

    float GetConstant() { return _mConstant; }
    float GetLinear() { return _mLinear; }
    float GetQuadratic() { return _mQuadratic; }

private:
    float _mConstant,
          _mLinear,
          _mQuadratic;
};

class SpotLight : public Light
{
public:
    SpotLight() = default;
    SpotLight(glm::vec3 position, /*glm::vec3 direction,*/ float cutoff, float outerCutoff);

    //void SetDirection(glm::vec3 direction);
    void SetCutOff(float cutOff);
    void SetOuterCutOff(float outerCutOff);

    //glm::vec3 GetDirection();
    float GetCutOff() { return _mCutOff; }
    float GetOuterCutOff() { return _mOuterCutOff; }

private:
    float _mCutOff,
          _mOuterCutOff;
};

} // namespace Toon

#endif // LIGHT_H
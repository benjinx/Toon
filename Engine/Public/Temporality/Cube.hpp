#ifndef CUBE_H
#define CUBE_H

#include <Temporality/Config.hpp>
#include <Temporality/GameObject.hpp>
#include <Temporality/Rigidbody.hpp>

class Cube : public GameObject
{
public:
    Cube();
    virtual ~Cube() = default;

    void Update();

private:
    Rigidbody _mRigidbody;
};

#endif // !CUBE_H

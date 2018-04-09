#ifndef CUBE_H
#define CUBE_H
#include "GameObject.hpp"
#include "Rigidbody.hpp"

class Cube : public GameObject
{
public:
	Cube();
	~Cube() {};

	void Update();

private:
	Rigidbody _mRigidbody;
};

#endif // !CUBE_H

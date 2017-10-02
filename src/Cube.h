#ifndef CUBE_H
#define CUBE_H
#include "GameObject.h"
#include "Rigidbody.h"

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

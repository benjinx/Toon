#ifndef SCENE_H
#define SCENE_H

#include "GameObject.h"

class Scene : public GameObject
{
public:
	Scene();
	~Scene() {};

	void Pause();
	void Resume();
private:

};

#endif // SCENE_H
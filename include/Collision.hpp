#ifndef COLLISION_H
#define COLLISION_H

#include <Config.hpp>
#include <Math.hpp>

#include <vector>

// SHAPES
struct Circle
{
	double radius;
};

struct Sphere
{
	double radius;
};

struct Plane
{
	double width, height;
};

struct Box
{
	double width, height, depth;
};

struct Cylinder
{
	double radius, height;
};
// SHAPES

// COLLISION BOUNDS
struct BoundingVolumeSphere
{
	const Sphere *shape; // holds radius
};

struct BoundingVolumeBox
{
	
	const Box *shapeLimits; // half dimensions

	// axis-aligned flag
	int isAxisAligned;

	// all corners in local and world space
	// Point localCorners[8];
	// Point worldCorners[8];

	std::vector<glm::vec3> localCorners[8];
	std::vector<glm::vec3> worldCorners[8];
	unsigned int numCorners; // 4 if 2D, 8 if 3D
};
// COLLISION BOUNDS

class Collision
{
public:
	Collision();

private:

};

#endif // COLLISION_H

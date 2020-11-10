#ifndef MESH_H
#define MESH_H

#include <Animator.hpp>
#include <Config.hpp>
#include <Joint.hpp>
#include <Math.hpp>
#include <OpenGL.hpp>

#include <memory>
#include <vector>

class Material;
class Shader;

class Mesh
{
public:

    struct Primitive
    {
        GLuint      VAO;
        GLenum      Mode;
        GLsizei     Count;
        GLenum      Type;
        GLsizei     Offset;

        std::shared_ptr<Material> _Material;
    };

    
    enum AttributeID : GLint
    {
        POSITION = 0,
        NORMAL = 1,
        TEXCOORD = 2,
        TANGENT = 3,
        JOINTS_0 = 4,
        WEIGHTS_0 = 5,
        COLOR = 6,

    };

    Mesh() = default;

    /* Functions */
    Mesh(GLuint vao, GLenum mode, GLsizei count, GLenum type, GLsizei offset, std::shared_ptr<Material> material);
    Mesh(GLuint vao, GLenum mode, GLsizei count, GLenum type, GLsizei offset, std::shared_ptr<Material> material, Joint rootJoint, int jointCount);
    Mesh(std::vector<Primitive>&&);

    bool LoadFromData(std::vector<Primitive>&& primitives);

    void Update(const float dt);
    void Render(Shader * shader, glm::mat4 modelMat);

    void doAnimation(Animation animation) { _mAnimator.DoAnimation(animation); }

    Joint GetRootJoint() { return _mRootJoint; }

    std::vector<glm::mat4> GetJointTransforms();

    void AddJointsToArray(Joint headJoint, std::vector<glm::mat4> jointMatrices);

private:
    std::vector<Primitive> _mPrimitives;

    // Joints
    Joint _mRootJoint;
    int _mJointCount;

    // Animator
    Animator _mAnimator;

    // Not sure how to handle these yet, they need to be part of the rendering
    //glm::vec3 _mJointIDs; this will be an int
    //glm::vec3 _mVertexWeights; this will be a float

};

#endif // MESH_H

#ifndef TOON_COMMAND_LIST_HPP
#define TOON_COMMAND_LIST_HPP

#include <Toon/Config.hpp>
#include <Toon/Pipeline.hpp>
#include <Toon/Primitive.hpp>

#include <memory>

namespace Toon {

class Buffer;

enum class CommandType
{
    SetPipeline,
    SetIndexBuffer,
    SetVertexBuffers,
}; // enum class CommandType

struct Viewport
{
    union
    {
        glm::vec2 Position;

        struct
        {
            float X;
            float Y;
        };
    };

    union
    {
        glm::vec2 Size;

        struct
        {
            float Width;
            float Height;
        };
    };

    union
    {
        glm::vec2 Depth;

        struct
        {
            float Near;
            float Far;
        };
    };
}; // struct Viewport

struct Command
{
    CommandType Type;

    union
    {
        struct
        {
            std::shared_ptr<Pipeline> Pipeline;

        } SetPipeline;

        struct
        {
            std::shared_ptr<Buffer> IndexBuffer;

        } SetIndexBuffer;

        struct
        {
            std::vector<std::shared_ptr<Buffer>> VertexBuffers;

        } SetVertexBuffers;

        struct
        {
            glm::vec2 Position;
            glm::vec2 Size;

        } SetScissor;

        struct
        {
            Viewport viewport;

        } SetViewport;
        
        struct
        {
            uint32_t VertexCount;
            uint32_t FirstVertex;
            

        } Draw;

        struct
        {
            uint32_t IndexCount;
            uint32_t FirstIndex;
            
        } DrawIndexed;

        struct 
        {
            std::shared_ptr<Buffer> Source;
            std::shared_ptr<Buffer> Destination;
            // Regions?

        } CopyBuffer;

    };
}; // struct Command

class Command
{
public:

}; // class Command

class SetPipelineCommand : public Command
{
public:

    SetPipelineCommand(std::shared_ptr<Pipeline> pipeline)
        : _pipeline(pipeline)
    { }

protected:

    std::shared_ptr<Pipeline> _pipeline;
}; // class SetPipelineCommand

class VulkanSetPipelineCommand : SetPipelineCommand
{
public:

    void Execute() override {
        auto vulkanCommandBuffer = TOON_VULKAN_COMMAND_LIST(list.get()));
        auto vulkanPipeline = TOON_VULKAN_PIPELINE(_pipeline.get());
        vkCmdBindPipeline(vulkanCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline);
    }
}; // class VulkanSetPipelineCommand

class OpenGLSetPipelineCommand : SetPipelineCommand
{
public:
    
    void Execute() override {
        auto openglPipeline = DUSK_OPENGL_PIPELINE(_pipeline.get());
        openglPipeline->Bind();
    }
}; // class OpenGLSetPipelineCommand


// Command Bundles?




enum class CommandListUsage
{
    // TODO: Improve
    Default,
    OneTime,

}; // enum class CommandListUsage

class TOON_ENGINE_API CommandAllocator : public Object
{
public:

    // TODO: Revisit

}; // class CommandAllocator

class TOON_ENGINE_API RenderTarget : public Object
{
public:

private:

}; // class RenderTarget

class TOON_VULKAN_API VulkanRenderTarget : public RenderTarget
{
public:

    VulkanRenderTarget(VkFramebuffer * framebuffer);

};

class TOON_DIRECTX_API DirectXRenderTarget : public RenderTarget
{
public:

    DirectXRenderTarget(ComPtr<ID3D12Resource> renderTarget);
};

class TOON_ENGINE_API CommandList : public Object
{
public:

    DISALLOW_COPY_AND_ASSIGN(CommandList)

    CommandList() = default;

    virtual ~CommandList() = default;

    virtual bool Initialize(CommandAllocator * commandAllocator, CommandListUsage commandListUsage) = 0;

    virtual void Terminate() = 0;

    virtual void Begin(Pipeline * initialPipeline) = 0;

    virtual void End() = 0;

    virtual void BeginRenderPass(RenderTarget * framebuffer, vec4 clearColor, float depth, int stencil) = 0;

    virtual void EndRenderPass() = 0;

    virtual void SetPipeline(std::shared_ptr<Pipeline> pipeline) = 0;

    virtual void SetIndexBuffer(std::shared_ptr<Buffer> indexBuffer) = 0;

    virtual void SetVertexBuffers(std::vector<std::shared_ptr<Buffer>> vertexBuffers) = 0;

    // BindDescriptorSets?
    // PushConstants?
    // RootSignature?

    virtual void Draw() = 0;


    // SetRenderTarget (Framebuffer)
    // ClearColor
    // ClearDepth

    // void AddSetViewportCommand(Viewport viewport);

private:

    std::vector<Command> _commands;

}; // class CommandList

} // namespace Toon

#endif // TOON_COMMAND_LIST_HPP
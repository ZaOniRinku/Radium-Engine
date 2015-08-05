#ifndef RADIUMENGINE_RENDERQUEUE_HPP
#define RADIUMENGINE_RENDERQUEUE_HPP

#include <map>
#include <vector>

#include <Core/CoreMacros.hpp>

#include <Engine/Renderer/RenderQueue/ShaderKey.hpp>
#include <Engine/Renderer/Bindable/BindableMaterial.hpp>
#include <Engine/Renderer/Bindable/BindableTransform.hpp>
#include <Engine/Renderer/Bindable/BindableMesh.hpp>


namespace Ra { namespace Engine { class ShaderProgram; }}
namespace Ra { namespace Engine { class RenderParameters; }}

namespace Ra { namespace Engine {

template <typename Key, typename Child>
class RenderableMap : public std::map<Key, Child>
{
public:
    inline RA_API RenderableMap();
    inline virtual RA_API ~RenderableMap();

    inline void RA_API render(ShaderProgram* shader) const;
};

class BindableMeshVector : public std::vector<BindableMesh>
{
public:
    inline RA_API BindableMeshVector();
    inline virtual RA_API ~BindableMeshVector();

    inline void RA_API render(ShaderProgram* shader) const;
};

typedef RenderableMap<BindableTransform, BindableMeshVector> TransformRenderQueue;
typedef RenderableMap<BindableMaterial, TransformRenderQueue> MaterialRenderQueue;

class RenderQueue : public std::map<ShaderKey, MaterialRenderQueue>
{
public:
    inline RA_API RenderQueue();
    inline virtual RA_API ~RenderQueue();

    inline RA_API void render() const;
    inline RA_API void render(const RenderParameters& params) const;
    inline RA_API void render(ShaderProgram* shader) const;
    inline RA_API void render(ShaderProgram* shader, const RenderParameters& params) const;
};

} // namespace Engine
} // namespace Ra

#include <Engine/Renderer/RenderQueue/RenderQueue.inl>

#endif // RADIUMENGINE_RENDERQUEUE_HPP

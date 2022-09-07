#pragma once

#include <Engine/Scene/Component.hpp>
#include <Engine/Scene/Entity.hpp>

struct AllPrimitivesComponent : public Ra::Engine::Scene::Component {

    explicit AllPrimitivesComponent( Ra::Engine::Scene::Entity* entity );

    /// This function is called when the component is properly
    /// setup, i.e. it has an entity.
    void initialize() override;
};

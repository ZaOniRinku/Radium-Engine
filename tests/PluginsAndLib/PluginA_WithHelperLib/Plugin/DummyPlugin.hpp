#pragma once

#include <Core/CoreMacros.hpp>
/// Defines the correct macro to export dll symbols.
#if defined DummyPlugin_EXPORTS
#    define DummyPlugin_API DLL_EXPORT
#else
#    define DummyPlugin_API DLL_IMPORT
#endif

#include <PluginBase/RadiumPluginInterface.hpp>

namespace MyDummyPlugin {

/*!
 * \brief This plugin defines a file loader for glTF2.0 format.
 * Plugin that bring support for GLTF 2.0 in Radium.
 * See file Readme.md for informations about whet is supported from GLTF.
 * This plugin also allow to export Radium scenes in GLTF.
 *
 */
class DummyPlugin_API DummyPlugin : public QObject, Ra::Plugins::RadiumPluginInterface
{
    Q_OBJECT
    Q_RADIUM_PLUGIN_METADATA
    Q_INTERFACES( Ra::Plugins::RadiumPluginInterface )

  public:
    DummyPlugin() = default;

    ~DummyPlugin() override = default;

    void registerPlugin( const Ra::Plugins::Context& context ) override;

    bool doAddWidget( QString& name ) override { return false; }
    QWidget* getWidget() override { return nullptr; }
    bool doAddMenu() override { return false; }
    QMenu* getMenu() override { return nullptr; }
    bool doAddAction( int& nb ) override { return false; }
    QAction* getAction( int id ) override { return nullptr; }
};

} // namespace MyDummyPlugin

using ProjectTools.Platforms;
using ProjectTools.Projects;

namespace Networking.Modules;

public class HttpModule : AModuleDefinition
{
    public override EModuleBinaryType BinaryType => EModuleBinaryType.DynamicLibrary;

    public override string Name => "Http";

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddDependencyModuleNames(
            "Core"
        );

        AddDependencyModuleNames(
            ETargetPlatformGroup.Apple,
            "Foundation"
        );

        AddLinkWithLibrary(
            ETargetPlatform.Windows,
            "winhttp.lib"
        );
    }
}
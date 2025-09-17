using ProjectTools.Platforms;
using ProjectTools.Projects;

namespace Core.Modules;

public class CoreModule : AModuleDefinition
{
    public override string Name => "Core";
    public override EModuleBinaryType BinaryType => EModuleBinaryType.DynamicLibrary;

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddDependencyModuleNames(
            ETargetPlatformGroup.Apple,
            "Foundation"
        );

        AddLinkWithLibrary(
            ETargetPlatform.Windows,
            "shell32.lib",
            "ole32.lib"
        );
    }
}
using ProjectTools.Projects;
using ProjectTools.Platforms;

namespace Application.Modules;

public class UIModule : AModuleDefinition
{
    public override string Name => "UI";
    public override EModuleBinaryType BinaryType => EModuleBinaryType.DynamicLibrary;

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddDependencyModuleNames(
            "Core"
        );

        AddLinkWithLibrary(
            ETargetPlatform.Windows,
            "user32.lib",
            "gdi32.lib",
            "comctl32.lib"
        );

        PlatformSpecifics.GetIOS().AddFrameworkDependencies(
            "UIKit"
        );
    }
}
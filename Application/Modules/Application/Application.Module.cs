using ProjectTools.Projects;
using ProjectTools.Platforms;

namespace Application.Modules;

public class ApplicationModule : AModuleDefinition
{
    public override string Name => "Application";
    public override EModuleBinaryType BinaryType => EModuleBinaryType.DynamicLibrary;

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddDependencyModuleNames(
            "Core",
            "UI"
        );

        AddLinkWithLibrary(
            ETargetPlatform.Windows,
            "user32.lib"
        );

        PlatformSpecifics.GetIOS().AddFrameworkDependencies(
            "UIKit"
        );
    }
}
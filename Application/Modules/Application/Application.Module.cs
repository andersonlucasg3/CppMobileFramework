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
            "Core"
        );

        AddDependencyModuleNames(
            ETargetPlatform.iOS,
            "UIKit"
        );

        PlatformSpecifics.GetIOS().AddFrameworkDependencies(
            "UIKit"
        );
    }
}
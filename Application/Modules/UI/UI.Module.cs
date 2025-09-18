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

        PlatformSpecifics.GetIOS().AddFrameworkDependencies(
            "UIKit"
        );
    }
}
using ProjectTools.Platforms;
using ProjectTools.Projects;

namespace AppleFrameworks.UIKit;

public class UIKitModule : AModuleDefinition
{
    public override string Name => "UIKit";
    public override EModuleBinaryType BinaryType => EModuleBinaryType.DynamicLibrary;

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddDependencyModuleNames(
            ETargetPlatformGroup.Apple,
            "Foundation"
        );

        PlatformSpecifics.GetIOS().AddFrameworkDependencies(
            "UIKit"
        );
    }
}
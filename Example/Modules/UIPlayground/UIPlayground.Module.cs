using ProjectTools.Platforms;
using ProjectTools.Projects;

namespace Example.Modules;

public class UIPlaygroundModule : AModuleDefinition
{
    public override string Name => "UIPlayground";
    public override EModuleBinaryType BinaryType => EModuleBinaryType.Application;

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddDependencyModuleNames(
            "Core",
            "UI",
            "Application"
        );
    }
}

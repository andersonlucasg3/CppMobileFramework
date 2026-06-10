using ProjectTools.Platforms;
using ProjectTools.Projects;

namespace Example.Modules;

public class HelloWorldModule : AModuleDefinition
{
    public override string Name => "HelloWorld";
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
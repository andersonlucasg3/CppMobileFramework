using Shared.Platforms;
using Shared.Projects;

namespace Core.Modules;

public class CoreTestsModule : AModuleDefinition
{
    public override string Name => "CoreTests";
    public override EModuleBinaryType BinaryType => EModuleBinaryType.Application;
    
    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddDependencyModuleNames(
            "TestSuitLib",
            "Core"
        );
    }
}
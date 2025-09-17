using Shared.Platforms;
using Shared.Projects;

namespace AppleFrameworks.Foundation;

public class FoundationModule : AModuleDefinition
{
    public override EModuleBinaryType BinaryType => EModuleBinaryType.DynamicLibrary;

    public override string Name => "Foundation";

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        PlatformSpecifics.GetApple()
            .AddFrameworkDependencies("Foundation");
    }
}
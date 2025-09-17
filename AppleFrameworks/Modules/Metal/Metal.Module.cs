using Shared.Platforms;
using Shared.Projects;

namespace AppleFrameworks.Metal;

public class MetalModule : AModuleDefinition
{
    public override EModuleBinaryType BinaryType => EModuleBinaryType.DynamicLibrary;

    public override string Name => "Metal";

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddCompilerDefinition(ETargetPlatform.Any, "METAL_ENABLED");

        AddDependencyModuleNames(
            "Foundation"
        );

        PlatformSpecifics.GetMac()
            .AddFrameworkDependencies("Metal");
    }
}
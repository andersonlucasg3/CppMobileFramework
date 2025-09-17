using ProjectTools.Platforms;
using ProjectTools.Projects;

namespace Networking.Modules;

public class NetworkingTests : AModuleDefinition
{
    public override EModuleBinaryType BinaryType => EModuleBinaryType.Application;

    public override string Name => "NetworkingTests";

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddDependencyModuleNames(
            "Http",
            "TestSuitLib"
        );
    }
}
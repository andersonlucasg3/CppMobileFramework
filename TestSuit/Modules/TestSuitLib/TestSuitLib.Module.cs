using ProjectTools.Platforms;
using ProjectTools.Projects;

namespace TestSuit.Modules;

public class TestSuitLibModule : AModuleDefinition
{
    public override EModuleBinaryType BinaryType => EModuleBinaryType.DynamicLibrary;

    public override string Name => "TestSuitLib";

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        
    }
}

using ProjectTools.Projects;
using ProjectTools.Platforms;

namespace TestSuit.Projects;

using Modules;

public class TestSuitProject : AProjectDefinition
{
    public override string Name => "TestSuit";

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddModule<TestSuitLibModule>();
    }
}
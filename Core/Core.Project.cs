using ProjectTools.Projects;
using ProjectTools.Platforms;
using TestSuit.Projects;
using AppleFrameworks.Projects;

namespace Core.Projects;

using Modules;
using ProjectTools.Extensions;

public class CoreProject : AProjectDefinition
{
    public override string Name => "Core";

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddProjectDependencyToGroup<AppleFrameworksProject>(ETargetPlatformGroup.Apple);
        
        AddProjectDependency<TestSuitProject>();

        AddModule<CoreModule>();

        if (InTargetPlatform.Platform.GetPlatformType() == ETargetPlatformType.Desktop)
        {
            AddModule<CoreTestsModule>();
        }
    }
}
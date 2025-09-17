using AppleFrameworks.Foundation;
using AppleFrameworks.Metal;
using ProjectTools.Platforms;
using ProjectTools.Projects;

namespace AppleFrameworks.Projects;

public class AppleFrameworksProject : AProjectDefinition
{
    public override string Name => "AppleFrameworks";
    
    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddModuleToGroup<FoundationModule>(ETargetPlatformGroup.Apple);
        // AddModule<MetalModule>();
    }
}
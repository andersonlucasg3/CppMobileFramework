using ProjectTools.Projects;
using ProjectTools.Platforms;
using Core.Projects;
using Application.Modules;

namespace Application.Projects;

public class ApplicationProject : AProjectDefinition
{
    public override string Name => "Application";

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddProjectDependency<CoreProject>();

        AddModule<ApplicationModule>();
        AddModule<UIModule>();
    }
}
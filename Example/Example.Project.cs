using ProjectTools.Projects;
using ProjectTools.Platforms;
using Core.Projects;
using Application.Projects;

namespace Example.Projects;

using Modules;

public class ExampleProject : AProjectDefinition
{
    public override string Name => "Example";

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddProjectDependency<CoreProject>();
        AddProjectDependency<ApplicationProject>();

        AddModule<HelloWorldModule>();
    }
}
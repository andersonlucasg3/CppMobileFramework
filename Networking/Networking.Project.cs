using ProjectTools.Projects;
using ProjectTools.Platforms;
using Core.Projects;

namespace Networking.Projects;

using Modules;

public class NetworkingProject : AProjectDefinition
{
    public override string Name => "Networking";

    protected override void Configure(ATargetPlatform InTargetPlatform)
    {
        AddModule<HttpModule>();
        AddModule<NetworkingTests>();

        AddProjectDependency<CoreProject>();
    }
}
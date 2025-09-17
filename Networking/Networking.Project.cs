using Shared.Projects;
using Shared.Platforms;
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
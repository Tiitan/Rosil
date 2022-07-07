using System.Threading;
using System.Threading.Tasks;
using OmniSharp.Extensions.LanguageServer.Protocol.Client.Capabilities;
using OmniSharp.Extensions.LanguageServer.Protocol.Document;
using OmniSharp.Extensions.LanguageServer.Protocol.Models;

namespace RosilLanguageServer;

internal class CompletionHandler: ICompletionHandler
{
    public async Task<CompletionList> Handle(CompletionParams request, CancellationToken cancellationToken)
    {
        // TODO: auto-complete logic
        return new CompletionList();
    }

    public CompletionRegistrationOptions GetRegistrationOptions(CompletionCapability capability,
        ClientCapabilities clientCapabilities)
    {
        return new CompletionRegistrationOptions
        {
            DocumentSelector = DocumentSelector.ForPattern("**/*.ros"),
            ResolveProvider = false,
        };
    }
}

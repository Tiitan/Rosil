using System.Threading;
using System.Threading.Tasks;
using OmniSharp.Extensions.LanguageServer.Protocol.Client.Capabilities;
using OmniSharp.Extensions.LanguageServer.Protocol.Document;
using OmniSharp.Extensions.LanguageServer.Protocol.Models;

namespace RosilLanguageServer;

public class HighlightHandler: IDocumentHighlightHandler
{
    public Task<DocumentHighlightContainer> Handle(DocumentHighlightParams request, CancellationToken cancellationToken)
    {
        throw new System.NotImplementedException();
    }

    public DocumentHighlightRegistrationOptions GetRegistrationOptions(DocumentHighlightCapability capability,
        ClientCapabilities clientCapabilities)
    {
        return new DocumentHighlightRegistrationOptions()
        {
            DocumentSelector = DocumentSelector.ForPattern("**/*.ros"),
            WorkDoneProgress = false,
        };
    }
}
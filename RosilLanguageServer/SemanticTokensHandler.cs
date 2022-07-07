using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using OmniSharp.Extensions.LanguageServer.Protocol.Client.Capabilities;
using OmniSharp.Extensions.LanguageServer.Protocol.Document;
using OmniSharp.Extensions.LanguageServer.Protocol.Models;
using OmniSharp.Extensions.LanguageServer.Protocol.Server;
using OmniSharp.Extensions.LanguageServer.Protocol.Window;
using RosilClrWrapper;

namespace RosilLanguageServer;

internal class SemanticTokensHandler: ISemanticTokensFullHandler
{
    private readonly ILanguageServer _server;
    private readonly Tokenizer _tokenizer;
    private readonly DocumentCache _documentCache;

    
    public SemanticTokensHandler(ILanguageServer server, DocumentCache cache)
    {
        _server = server;
        _documentCache = cache;
        _tokenizer = new Tokenizer();
    }


    public Task<SemanticTokens> Handle(SemanticTokensParams request, CancellationToken cancellationToken)
    {
        _tokenizer.Run(_documentCache.Get(request.TextDocument.Uri.ToString()));
        foreach (Token token in _tokenizer.Tokens)
        {
            // TODO build semantic data array to fill SemanticTokens
        }
        
        var semanticTokens = new SemanticTokens();
        _server.Window.LogInfo($"SemanticTokens: {semanticTokens}");
        return Task.FromResult(semanticTokens);
    }

    public SemanticTokensRegistrationOptions GetRegistrationOptions(SemanticTokensCapability capability, ClientCapabilities clientCapabilities)
    {
        return new SemanticTokensRegistrationOptions()
        {
            Legend = new SemanticTokensLegend(),
            Full = true,
            Range = false, // TODO: support document subset
            DocumentSelector = DocumentSelector.ForPattern("**/*.ros")
        };
    }
}

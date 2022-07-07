using System.Collections.Concurrent;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using MediatR;
using OmniSharp.Extensions.LanguageServer.Protocol;
using OmniSharp.Extensions.LanguageServer.Protocol.Client.Capabilities;
using OmniSharp.Extensions.LanguageServer.Protocol.Document;
using OmniSharp.Extensions.LanguageServer.Protocol.Models;
using OmniSharp.Extensions.LanguageServer.Protocol.Server;
using OmniSharp.Extensions.LanguageServer.Protocol.Server.Capabilities;
using OmniSharp.Extensions.LanguageServer.Protocol.Window;

namespace RosilLanguageServer;

internal class DocumentSyncHandler: ITextDocumentSyncHandler
{
    private readonly ILanguageServer _server;
    private readonly DocumentCache _documentCache;
    private readonly ConcurrentDictionary<string, bool> _dirty = new();
    private readonly ConcurrentDictionary<string, string> _unchangedDocument = new();

    // TODO move tokenizer and lexer here to cache AST instead of script string 

    public DocumentSyncHandler(ILanguageServer server, DocumentCache cache)
    {
        _server = server;
        _documentCache = cache;
    }
    
    // On document opened
    public Task<Unit> Handle(DidOpenTextDocumentParams request, CancellationToken cancellationToken)
    {
        string path = request.TextDocument.Uri.ToString();
        string content = request.TextDocument.Text;
        
        _unchangedDocument.AddOrUpdate(path, content, (k, v) => v);
        _documentCache.Replace(path, content);
        _server.Window.LogInfo($"Open: {path}\n{content}");
        
        return Unit.Task;
    }
    
    // On document changed
    // currently same as opened, TODO: support incremental changes
    public Task<Unit> Handle(DidChangeTextDocumentParams request, CancellationToken cancellationToken)
    {
        string path = request.TextDocument.Uri.ToString();
        string content = request.ContentChanges.FirstOrDefault()?.Text;

        _dirty.AddOrUpdate(path, true, (s, b) => b);
        
        _documentCache.Replace(path, content);
        _server.Window.LogInfo($"Change: {path}\n{content}");
        
        return Unit.Task;
    }

    // On document closed
    public Task<Unit> Handle(DidCloseTextDocumentParams request, CancellationToken cancellationToken)
    {
        string path = request.TextDocument.Uri.ToString();
        
        // Discard pending changes on file close
        if (_dirty.TryGetValue(path, out bool value) && value)
            _documentCache.Replace(path, _unchangedDocument.TryGetValue(path, out string document) ? document : null);
        
        return Unit.Task;
    }

    // On document saved
    public Task<Unit> Handle(DidSaveTextDocumentParams request, CancellationToken cancellationToken)
    {
        string path = request.TextDocument.Uri.ToString();
        
        _dirty.AddOrUpdate(path, false, (s, b) => b);
        _unchangedDocument.AddOrUpdate(path, _documentCache.Get(path), (k, v) => v);
        
        _server.Window.LogInfo($"Save: {path}");
        return Unit.Task;
    }

    // Document change capabilities
    TextDocumentChangeRegistrationOptions IRegistration<TextDocumentChangeRegistrationOptions, SynchronizationCapability>.GetRegistrationOptions(SynchronizationCapability capability,
        ClientCapabilities clientCapabilities)
    {
        return new TextDocumentChangeRegistrationOptions()
        {
            DocumentSelector = DocumentSelector.ForPattern("**/*.ros"),
            SyncKind = TextDocumentSyncKind.Full // TODO: support incremental changes
        };
    }
    
    // Document open capabilities
    TextDocumentOpenRegistrationOptions IRegistration<TextDocumentOpenRegistrationOptions, SynchronizationCapability>.GetRegistrationOptions(SynchronizationCapability capability,
        ClientCapabilities clientCapabilities)
    {
        return new TextDocumentOpenRegistrationOptions() {DocumentSelector = DocumentSelector.ForPattern("**/*.ros")};
    }

    // Document close capabilities
    TextDocumentCloseRegistrationOptions IRegistration<TextDocumentCloseRegistrationOptions, SynchronizationCapability>.GetRegistrationOptions(SynchronizationCapability capability,
        ClientCapabilities clientCapabilities)
    {
        return new TextDocumentCloseRegistrationOptions() {DocumentSelector = DocumentSelector.ForPattern("**/*.ros")};
    }
    
    // Document save capabilities
    TextDocumentSaveRegistrationOptions IRegistration<TextDocumentSaveRegistrationOptions, SynchronizationCapability>.GetRegistrationOptions(SynchronizationCapability capability,
        ClientCapabilities clientCapabilities)
    {
        return new TextDocumentSaveRegistrationOptions() {DocumentSelector = DocumentSelector.ForPattern("**/*.ros")};
    }

    // I don't know what this is and where languageId should be declared
    public TextDocumentAttributes GetTextDocumentAttributes(DocumentUri uri)
    {
        return new TextDocumentAttributes(uri, "rosil");
    }
}

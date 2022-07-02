using System;
using System.Text;
using System.Collections.Concurrent;
using System.Collections.Generic;

namespace RosilLanguageServer;

/// <summary>
/// Cache the documents that are being edited.
/// the content does not match the file's content when they are modified.
/// The document are kept in sync by DocumentSyncHandler
/// TODO: support incremental changes
/// </summary>
internal class DocumentCache
{
    private readonly ConcurrentDictionary<string, string> _cache = new();

    public void Replace(string path, string content)
    {
        _cache.AddOrUpdate(path, content, (k, v) => v);
    }

    public string Get(string path)
    {
        return _cache.TryGetValue(path, out string content) ? content.ToString() : null;
    }
    
    public void Remove(string path)
    {
        _cache.TryRemove(path, out _);
    }
}

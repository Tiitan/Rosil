'use strict';

import { workspace, Disposable, ExtensionContext } from 'vscode';
import { LanguageClient, LanguageClientOptions, SettingMonitor, ServerOptions, TransportKind, InitializeParams } from 'vscode-languageclient';
import { Trace } from 'vscode-jsonrpc';

export function activate(context: ExtensionContext) {

    let serverOptions: ServerOptions = {
        run: { command: 'dotnet', args: ['..\RosilLanguageServer\bin\Release\net6.0\RosilLanguageServer.dll'] },
        debug: { command: 'dotnet', args: ['..\RosilLanguageServer\bin\Debug\net6.0\RosilLanguageServer.dll'] }
    }

    // Options to control the language client
    let clientOptions: LanguageClientOptions = {
        documentSelector: [{ pattern: '**/*.ros' }],
        synchronize: {
            // Synchronize the setting section 'RosilLanguageServer' to the server
            configurationSection: 'RosilLanguageServer',
            fileEvents: workspace.createFileSystemWatcher('**/*.ros')
        },
    }

    // Create the language client and start the client.
    const client = new LanguageClient('RosilLanguageServer', 'Rosil Language Server', serverOptions, clientOptions);
    client.trace = Trace.Verbose;
    let disposable = client.start();

    // Push the disposable to the context's subscriptions so that the client can be deactivated on extension deactivation
    context.subscriptions.push(disposable);
}

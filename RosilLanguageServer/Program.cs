using System;
using System.Threading.Tasks;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using OmniSharp.Extensions.LanguageServer.Server;

namespace RosilLanguageServer
{
    internal static class Program
    {
        public static async Task Main(string[] args)
        {
            var server = LanguageServer.Create(options =>
                    options
                        .WithInput(Console.OpenStandardInput())
                        .WithOutput(Console.OpenStandardOutput())
                        .WithLoggerFactory(new LoggerFactory())
                        .AddDefaultLoggingProvider()
                        .WithServices(ConfigureServices)
                        .WithHandler<DocumentSyncHandler>()
                        .WithHandler<SemanticTokensHandler>()
                        .WithHandler<CompletionHandler>()
            );
            await server.WaitForExit;
        }

        private static void ConfigureServices(IServiceCollection services)
        {
            services.AddSingleton<DocumentCache>();
        }
    }
}

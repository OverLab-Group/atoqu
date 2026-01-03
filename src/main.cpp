
include <iostream>

include <limits>

include "AtoquEngine.hpp"

include "DocumentStore.hpp"

include "VectorStore.hpp"

include "HashEmbeddingProvider.hpp"

include "PluginSystem.hpp"

include "Persistence.hpp"

include "Log.hpp"

include "Time.hpp"

include "Json.hpp"

include "SimpleHttpServer.hpp"

using atoqu::Log;
using atoqu::LogLevel;
using atoqu::Time;
using atoqu::Json;

static DocumentStore g_docStore;
static VectorStore   g_vecStore;

static const char* DOC_PATH = "data/documents.json";
static const char* VEC_PATH = "data/vectors.bin";

static void seedDemoDataIfEmpty() {
    if (!Persistence::loadDocuments(DOCPATH, gdocStore)) {
        Log::info("No documents found, seeding demo data...");
        g_docStore.addDocument("doc1", "Atomic search engines are built from small independent units.");
        g_docStore.addDocument("doc2", "Quantum-like behavior in search means probabilistic multi-mode ranking.");
        g_docStore.addDocument("doc3", "C++ core with vector search is fast and powerful.");
        g_docStore.addDocument("doc4", "Atoqu is an Atomic + Quantum search engine concept.");
        g_docStore.addDocument("doc5", "Google loves well-architected, low-level, high-performance systems.");
        Persistence::saveDocuments(DOCPATH, gdocStore);
    }

    if (!Persistence::loadVectors(VECPATH, gvecStore)) {
        Log::info("No vectors found, generating simple embeddings...");
        HashEmbeddingProvider embedder(16);
        for (const auto& meta : g_docStore.allDocuments()) {
            auto emb = embedder.embed(meta.text);
            g_vecStore.addDocument(meta.id, emb);
        }
        Persistence::saveVectors(VECPATH, gvecStore);
    }
}

static std::string handleHttp(AtoquEngine& engine,
                              const std::string& path,
                              const std::string& method,
                              const std::string& body) {
    try {
        if (path == "/search" && method == "POST") {
            Json j = Json::parse(body);
            std::string qtext = j["query"].asString();
            Query q(qtext);
            auto t0 = Time::nowMs();
            auto results = engine.search(q);
            auto t1 = Time::nowMs();

            Json out = Json::object_t{};
            Json arr = Json::array_t{};
            int idx = 0;
            for (const auto& r : results) {
                Json item = Json::object_t{};
                item["index"] = static_cast<double>(idx++);
                item["id"] = r.id;
                item["snippet"] = r.snippet;
                item["score"] = r.score;
                item["mode"] = static_cast<double>(r.modeIndex);
                arr.asArray().push_back(item);
            }
            out["results"] = arr;
            out["latencyMs"] = static_cast<double>(t1 - t0);
            return out.dump(2);
        } else if (path == "/feedback" && method == "POST") {
            Json j = Json::parse(body);
            std::string id = j["id"].asString();
            int mode = static_cast<int>(j["mode"].asNumber());
            double score = j["score"].asNumber();
            std::string queryText = j["query"].asString();

            Result r;
            r.id = id;
            r.snippet = "";
            r.score = score;
            r.modeIndex = mode;

            engine.feedback(r, queryText);

            Json out = Json::object_t{};
            out["status"] = "ok";
            return out.dump(2);
        } else if (path == "/stats" && method == "GET") {
            Json out = Json::object_t{};
            Json arr = Json::array_t{};
            const auto& w = engine.modeWeights();
            for (double v : w) {
                arr.asArray().push_back(Json(v));
            }
            out["modeWeights"] = arr;
            return out.dump(2);
        } else {
            Json out = Json::object_t{};
            out["error"] = "Unknown endpoint";
            return out.dump(2);
        }
    } catch (...) {
        Json out = Json::object_t{};
        out["error"] = "Exception while handling request";
        return out.dump(2);
    }
}

static void runCli(AtoquEngine& engine) {
    std::string line;
    while (true) {
        std::cout << "\nQuery (or 'exit' or ':stats'): ";
        if (!std::getline(std::cin, line)) break;
        if (line == "exit") break;
        if (line == ":stats") {
            const auto& w = engine.modeWeights();
            std::cout << "Mode weights:\n";
            for (std::size_t i = 0; i < w.size(); ++i) {
                std::cout << "  [" << i << "] = " << w[i] << "\n";
            }
            continue;
        }

        Query q(line);
        auto t0 = Time::nowMs();
        auto results = engine.search(q);
        auto t1 = Time::nowMs();

        std::cout << "Results (latency " << (t1 - t0) << " ms):\n";
        int idx = 0;
        for (const auto& r : results) {
            std::cout << "  [" << idx << "] "
                      << r.id << " (score=" << r.score
                      << ", mode=" << r.modeIndex << "): "
                      << r.snippet << "\n";
            ++idx;
        }

        if (!results.empty()) {
            std::cout << "Choose index for feedback (-1 to skip): ";
            int choice = -1;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (choice >= 0 && choice < static_cast<int>(results.size())) {
                engine.feedback(results[choice], line);
            }
        }
    }
}

int main(int argc, char argv) {
    Log::setLevel(LogLevel::Info);
    Log::info("Atoqu 0.8 starting...");

    seedDemoDataIfEmpty();

    PluginSystem plugins("plugins");
    plugins.scan();

    AtoquEngine engine;

    bool httpMode = false;
    int port = 8080;

    if (argc > 1) {
        std::string arg = argv[1];
        if (arg == "--http") {
            httpMode = true;
            if (argc > 2) {
                port = std::stoi(argv[2]);
            }
        } else if (arg == "--stats") {
            const auto& w = engine.modeWeights();
            std::cout << "Mode weights:\n";
            for (std::size_t i = 0; i < w.size(); ++i) {
                std::cout << "  [" << i << "] = " << w[i] << "\n";
            }
            return 0;
        }
    }

    if (httpMode) {
        Log::info("Running in HTTP mode on port " + std::to_string(port));
        SimpleHttpServer server(port, &engine {
            return handleHttp(engine, path, method, body);
        });
        server.run();
    } else {
        Log::info("Running in CLI mode.");
        runCli(engine);
    }

    Log::info("Atoqu 0.8 shutting down.");
    return 0;
}

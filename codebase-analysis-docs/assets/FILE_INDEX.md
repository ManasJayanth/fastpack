# Fastpack — File Index (Pass 0)

Priority: `A` = entry point / backbone, `B` = important feature module, `C` = support/util, `D` = infra/tests/vendor.
Line counts as of commit `173e0a4`.

| # | Pri | Path | Type | Lines | Notes |
|---|-----|------|------|-------|-------|
| 1 | A | `bin/fpack.re` | code | 3 | CLI executable entry — delegates to `Fastpack.Commands` |
| 2 | A | `Fastpack/Commands.re` | code | 529 | Cmdliner subcommands: build (default), watch, transpile, worker, explain-config, help |
| 3 | A | `Fastpack/Builder.re` | code | 392 | Build orchestration, rebuild logic, watch filters |
| 4 | A | `Fastpack/Config.re` | code | 1157 | CLI/JSON config parsing, precedence, pretty-print |
| 5 | A | `Fastpack/DependencyGraph.re` | code | 585 | Graph structure + async BFS module discovery |
| 6 | A | `Fastpack/Worker.re` | code | 827 | Worker subprocess protocol + the core ESM/CJS→bundle rewrite (`analyze`) |
| 7 | A | `Fastpack/Resolver.re` | code | 750 | Node-style module resolution, mocks, browser field, preprocessor requests |
| 8 | A | `Fastpack/Bundle.re` | code | 721 | Chunking, JS runtime template, emission |
| 9 | B | `Fastpack/Module.re` | code | 254 | Module record, `location` type, id encoding |
| 10 | B | `Fastpack/Preprocessor.re` | code | 304 | Preprocessor chains: builtin transpilers vs node-service |
| 11 | B | `Fastpack/Cache.re` | code | 228 | Persistent module cache (Marshal) |
| 12 | B | `Fastpack/FSCache.re` | code | 447 | In-memory FS cache w/ trust flags, symlinks, case-sensitivity |
| 13 | B | `Fastpack/ExportFinder.re` | code | 156 | Validates imported names exist in exporters |
| 14 | B | `Fastpack/Mode.re` | code | 215 | NODE_ENV constant folding / dead-code elimination |
| 15 | B | `Fastpack/Workspace.re` | code | 217 | Patch-list over source text, UTF-8 offset mapping |
| 16 | B | `Fastpack/Package.re` | code | 107 | package.json model: mainFields, browser shims |
| 17 | B | `Fastpack/Error.re` | code | 310 | Error taxonomy + human-friendly reports, node-lib hints |
| 18 | C | `Fastpack/Context.re` | code | 12 | Per-build context record |
| 19 | C | `Fastpack/Mode.re` | code | — | (see 14) |
| 20 | C | `Fastpack/Run.re` / `RunAsync.re` | code | 62/67 | result-with-context monads (sync/Lwt) |
| 21 | C | `Fastpack/Environment.re` | code | 35 | executable path, CPU count |
| 22 | C | `Fastpack/Version.re` | code | 2 | version + `%%COMMIT%%` placeholder |
| 23 | A | `node-service/index.js` | code | 227 | Node sidecar running webpack loaders (loader-runner) |
| 24 | B | `FastpackTranspiler/FastpackTranspiler.re` | code | 101 | transpiler driver + JS runtime helpers source |
| 25 | B | `FastpackTranspiler/ReactJSX.re` | code | 372 | JSX → React.createElement |
| 26 | B | `FastpackTranspiler/StripFlow.re` | code | 162 | Remove Flow type annotations |
| 27 | B | `FastpackTranspiler/Class.re` | code | 422 | Class properties/statics/decorators |
| 28 | B | `FastpackTranspiler/ObjectSpread.re` | code | 979 | Object spread/rest transform |
| 29 | C | `FastpackTranspiler/Context.re` | code | 32 | id generation, runtime-required flag |
| 30 | B | `FastpackUtil/Scope.re` | code | 688 | Lexical scope & exports model |
| 31 | B | `FastpackUtil/Visit.re` | code | 454 | Read-only AST visitor (used by analyze/Scope) |
| 32 | B | `FastpackUtil/AstMapper.re` | code | 666 | AST rewriting mapper (used by transpilers) |
| 33 | B | `FastpackUtil/Printer.re` | code | 1442 | AST → source printer (only when transpilers changed AST) |
| 34 | C | `FastpackUtil/Parser.re` | code | 18 | Flow parser wrapper w/ ES-proposal options |
| 35 | C | `FastpackUtil/Process.re` | code | 103 | subprocess mgmt, Marshal/line IO, 30s guard |
| 36 | C | `FastpackUtil/FS.re` | code | 135 | fs helpers, tmp dirs, rmdir, text-file regex |
| 37 | C | `FastpackUtil/AstHelper.re` | code | 152 | AST node constructors |
| 38 | C | `FastpackUtil/AstParentStack.re` | code | 42 | parent chain for visitors |
| 39 | C | `FastpackUtil/UTF8.re` | code | 33 | naive UTF-8 length/sub |
| 40 | C | `FastpackUtil/Terminal.re` | code | 8 | clear screen in watch mode |
| 41 | C | `FastpackUtil/sysutil.c` | code | — | `pid_of_handle` C stub |
| 42 | D | `FastpackTest/*.ml` | tests | — | ppx_expect unit tests (Resolver, Print, Transpile*, Watch) |
| 43 | D | `test/<case>/dev.test.js` | tests | — | ~50 snapshot integration fixtures |
| 44 | D | `scripts/test.js` | infra | — | integration snapshot runner (`--train` to re-record) |
| 45 | D | `scripts/setupTest.js` | infra | — | yarn-install each test fixture |
| 46 | D | `scripts/bump_version.js`, `replaceCommitVersion.js` | infra | — | release helpers |
| 47 | D | `dist/package.json`, `dist/postinstall.js` | infra | — | npm distribution of prebuilt binaries |
| 48 | D | `azure-pipelines.yml` | infra | — | CI: Linux (alpine docker), macOS, Windows |
| 49 | D | `linux-build/Dockerfile` | infra | — | glibc-on-alpine build image |
| 50 | D | `package.json`, `dune-project`, `*/dune`, `esy.lock/` | build | — | esy + dune build config, pinned OCaml deps |

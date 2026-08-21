# Fastpack — Integration Test Fixture Catalog (Pass 3)

The snapshot runner `scripts/test.js` discovers **any `test/<case>/*.test.js`** file
(`scripts/test.js:337`), not only `dev.test.js` — one fixture directory can hold several
tests (e.g. `error-cannot-find-exported-name/{dev,success,used}.test.js`). Each test module
exports a function receiving helpers:

- `({bundle}) => bundle("fpack --dev …")` — run fpack, snapshot stdout/stderr **and** the
  emitted bundle directory;
- `({error}) => error("fpack …")` — expect a non-zero exit, snapshot the error output;
- `({stdout}) => stdout("fpack …")` — snapshot stdout only (e.g. `explain-config`).

Committed `dev/` (and per-test-name) directories are the expected snapshots; re-record with
`make train-integration [pattern=…]`.

Platform skips (`scripts/test.js:1-8`): `pack-less` on win32; `error-resolve-case-sensitive`
on Linux (needs a case-insensitive FS).

**Legacy layer**: several fixtures contain `prod.sh`/`test.sh` scripts and `prod/` snapshots
from an older bash harness; `test/skip.sh` is that harness's skip list (nearly all `prod.sh`
entries skipped because production mode is disabled). Fixtures whose *only* test is a legacy
`.sh` script are **not exercised at all** by the current runner: `error-dependency-cycle`,
`error-scope-naming-collision`, `error-scope-previously-undefined-export`,
`pack-error-no-same-name-reexport`, `cache-reporting`, `transpile-builtin`. Treat `*.sh` +
`test/skip.sh` as historical.

## What each fixture pins down

### Error reporting (diagnostics are part of the product)

| Fixture | Asserts |
|---|---|
| `error-cannot-find-exported-name` | 3 tests: missing-name `PackError` (`dev`), clean pass under `--export-check-used-imports-only` (`success`), and that a *used* missing import still fails under that flag (`used`) |
| `error-cannot-leave-package-dir` (+ root helper `test/LeavePackageDir.js`) | `CannotLeavePackageDir` when resolution escapes `projectRootDir` |
| `error-cannot-resolve-modules` | resolver trace in `CannotResolveModule` output |
| `error-cannot-resolve-node-module` | per-builtin polyfill hints (`Error.re` `nodelibs`): separate `dns`/`path`/`url` tests |
| `error-parse`, `error-parse-tty` | `CannotParseFile` codeframe (tty variant: colored output) |
| `error-resolve-case-sensitive` | `caseSensitiveExactMatch` candidates listing (case-insensitive FS only) |
| `pack-eslint-error`, `pack-eslint-warnings` | loader-reported errors vs warnings surfacing (`PreprocessorError` / warning list) |
| *(legacy, not run)* `error-dependency-cycle`, `error-scope-naming-collision`, `error-scope-previously-undefined-export`, `pack-error-no-same-name-reexport` | prod-harness-era fixtures; the reexport one documents the known raw-`failwith` (gotcha #11) |

### Bundling & module-rewrite semantics

| Fixture | Asserts |
|---|---|
| `pack-simple`, `pack-flat` | baseline ESM→`__fastpack_require__` output shape |
| `pack-flat-collisions` | import-var naming collision avoidance against user bindings |
| `pack-flat-reexport`, `pack-export-all`, `pack-no-default-reexport` | `export {} from`, `export *`, `omitDefault` interop |
| `pack-export-class`, `pack-scoped-hoisted-function-export` | exported class/function declaration + live-binding `defineProperty` handling |
| `pack-hoisted-import` | use-before-import-statement identifier patching |
| `pack-scoped-require` | `require` shadowed by a local binding is NOT rewritten |
| `pack-all-static` | fully static graph (no chunks) |
| `pack-split-import` | dynamic `import()` → chunk files + `c` map + `imp()` runtime |
| `pack-mode` | `NODE_ENV` folding / dead-branch elimination (`Mode.re`) |
| `pack-env-var` | `--env-var` inlining into the runtime preamble |
| `pack-filename` | `-n/--output-filename` handling |
| `pack-esc-seq`, `pack-utf8`, `pack-jsx-escape` | eval-string escaping & unicode offset mapping (`Worker.to_eval`, `Workspace.write`) |
| `pack-json` | `.json` fast-path (`module.exports = <json>`) |
| `pack-transpiler-runtime` | `$fp$runtime` injection (class/decorator helpers) |
| `pack-browser` | package.json `browser` field shims/ignores |
| `pack-builtins` | despite the name: the **builtin transpiler** end-to-end — Flow types stripped inside JSX, object rest, and free identifiers (`path`, `module`) left untouched |

### Preprocessors / webpack-loader interop

| Fixture | Asserts |
|---|---|
| `pack-cra` | Create-React-App-style app via CLI `--preprocess` flags |
| `pack-cra-config` | same via `fastpack.json` (config-file path) |
| `pack-css`, `pack-less`, `pack-sass`, `pack-ts`, `pack-raw` | css/style/less/sass/ts/raw loader chains through node-service |
| `pack-custom-loader` | project-local (non-npm) loader resolution |
| `pack-preprocess-once` | first-matching-pattern-wins: `^log1\.js$:./logSource` before the catch-all `\.js$:builtin` |
| `pack-file-cached` | url/file-loader emitted assets (`Module.files`) survive cache round-trips |
| `pack-workspaces` | symlinked yarn-workspace packages + `--project-root` |

### Stdout / config

| Fixture | Asserts |
|---|---|
| `explain-config` (`no-config.test.js`) | `fpack explain-config --dev` output with no config file |

### Unit-test support (not run by scripts/test.js)

| Path | Used by |
|---|---|
| `test/resolve/` | `FastpackTest/Resolver.ml` expect tests (fixture node_modules tree; traces reference `test/resolve/...`) |
| `test/watch/` | `FastpackTest/Watch.ml` |
| `test/print.js`, `test/print-with-scope.js` | `FastpackTest/Print.ml`, `PrintWithScope.ml` |
| `test/transpile-class.js`, `transpile-object-spread.js`, `transpile-react-jsx.js`, `transpile-strip-flow.js` | `FastpackTest/Transpile*.ml` |
| `test/current.js`, `test/server.js` | scratch/legacy files |

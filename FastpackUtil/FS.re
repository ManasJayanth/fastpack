let stat_option = path =>
  try%lwt (
    {
      let%lwt stat = Lwt_unix.stat(path);
      Lwt.return_some(stat);
    }
  ) {
  | Unix.Unix_error(_) => Lwt.return_none
  };

let rec setInterval = (ms, f) => {
  let%lwt () = Lwt_unix.sleep(ms);
  let%lwt () = f();
  setInterval(ms, f);
};

let abs_path = (dir, filename) =>
  FilePath.reduce(~no_symlink=true) @@ FilePath.make_absolute(dir, filename);

let rec readlink = filename =>
  switch%lwt (Lwt_unix.readlink(filename)) {
  | value => readlink(abs_path(FilePath.dirname(filename), value))
  | exception (Unix.Unix_error(Unix.EINVAL, "readlink", _)) =>
    Lwt.return(filename)
  | exception (Unix.Unix_error(Unix.ENOENT, "readlink", _)) =>
    Lwt.return(filename)
  };

let relative_path = (dir, filename) => {
  let relative = FilePath.make_relative(dir, filename);
  if (relative == "") {
    filename;
  } else {
    relative;
  };
};

let rec makedirs = dir =>
  switch%lwt (stat_option(dir)) {
  | None =>
    let%lwt () = makedirs(FilePath.dirname(dir));
    Lwt_unix.mkdir(dir, 0o777);
  | Some(stat) =>
    switch (stat.st_kind) {
    | Lwt_unix.S_DIR => Lwt.return_unit
    | _ => Error.ie @@ Printf.sprintf("'%s' expected to be a directory", dir)
    }
  };

let try_dir = dir =>
  try%lwt (
    {
      let%lwt stat = Lwt_unix.stat(dir);
      switch (stat.st_kind) {
      | Lwt_unix.S_DIR => Lwt.return_some(dir)
      | _ => Lwt.return_none
      };
    }
  ) {
  | Unix.Unix_error(_) => Lwt.return_none
  };

let pat_text_ext =
  Re.Posix.compile_pat("\\.(js|jsx|mjs|ts|tsx|css|sass|scss|less)$");
let is_text_file = filename =>
  switch (Re.exec_opt(pat_text_ext, filename)) {
  | Some(_) => true
  | None => false
  };

let isatty = channel => {
  let forceTTY =
    switch (Sys.getenv_opt("FPACK_FORCE_TTY")) {
    | Some("true") =>
      Pastel.(setMode(Terminal));
      true;
    | _ => false
    };

  forceTTY || Unix.isatty(channel);
};

let copy_file = (~source, ~target, ()): Lwt.t(unit) => {
  let%lwt sourceFile = Lwt_io.open_file(~mode=Lwt_io.Input, source);
  let%lwt targetFile = Lwt_io.open_file(~mode=Lwt_io.Output, target);

  let%lwt file = Lwt_io.read(sourceFile);
  let%lwt () = Lwt_io.write(targetFile, file);

  let%lwt () = Lwt_io.close(sourceFile);
  let%lwt () = Lwt_io.close(targetFile);

  Lwt.return();
};

let rec rmdir = dir => {
  let%lwt files = Lwt_stream.to_list(Lwt_unix.files_of_directory(dir));
  let%lwt () =
    Lwt_list.iter_s(
      filename =>
        switch (filename) {
        | "."
        | ".." => Lwt.return_unit
        | _ =>
          let path = abs_path(dir, filename);
          switch%lwt (Lwt_unix.stat(path)) {
          | {st_kind: Lwt_unix.S_DIR, _} => rmdir(path)
          | _ => Lwt_unix.unlink(path)
          };
        },
      files,
    );
  Lwt_unix.rmdir(dir);
};

let makeTempDir = parent => {
  Random.self_init();
  let rec makeDir = () => {
    let path =
      abs_path(
        parent,
        ".fpack-" ++ Int64.(to_string(Random.int64(max_int))),
      );
    switch%lwt (stat_option(path)) {
    | None =>
      let%lwt () = makedirs(path);
      Lwt.return(path);
    | Some(_) => makeDir()
    };
  };
  makeDir();
};

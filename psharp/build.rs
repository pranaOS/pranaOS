fn main() {
    lalrpop::Configuration::new()
        .use_cargo_dir_conventions()
        .process_file("res/grammar.ps")
        .expect("failed to process LALRPOP grammar");

    if cfg!(test) {
        for path in ["res/examples/**/*.ps", "res/benchmarks/**/*.ps"] {
            build_deps::rerun_if_changed_paths(path).expect("could not read path");
        }
    }
}
/**
 * @file main.rs
 * @author Krisna Pranav
 * @brief utils
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use clap::Parser;
use commands::{init, add, commit, log, status, push, pull};

mod cli;
mod commands;
mod networking;
mod storage;
mod objects;
mod utils;

fn main() {
    let cli = cli::args::Cli::parse();

    match cli.command {
        cli::args::Command::Init { path } => init::execute(path),
        cli::args::Command::Add { file } => add::execute(file),
        cli::args::Command::Commit { message } => todo!(),
        cli::args::Command::Log => log::execute(),
        cli::args::Command::Status => status::execute(),
        cli::args::Command::Push { remote } => push::execute(remote),
        cli::args::Command::Pull { remote } => pull::execute(remote),
    }
}
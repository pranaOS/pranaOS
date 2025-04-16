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

mod commands;
mod object;
mod utils;
mod core;
mod remote;

use clap::{Parser, Subcommand};

#[derive(Parser)]
#[command(name = "dvcs")]
#[command(about = "A simple Git-inspired DVCS written in Rust")]
struct Cli {
    #[command(subcommand)]
    command: Commands,
}

#[derive(Subcommand)]
enum Commands {
    Init,
    Commit { message: String },
    Log,
    Status,
}

fn main() -> anyhow::Result<()> {
    let cli = Cli::parse();

    match cli.command {
        Commands::Init => commands::init::run()?,
        Commands::Commit { message } => commands::commit::run(&message)?,
        Commands::Log => commands::log::run()?,
        Commands::Status => commands::status::run()?,
    }

    Ok(())
}

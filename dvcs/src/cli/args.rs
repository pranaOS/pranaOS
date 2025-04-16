/**
 * @file args.rs
 * @author Krisna Pranav
 * @brief args
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use clap::{Parser, Subcommand};

#[derive(Parser)]
#[command(name = "dvcs")]
#[command(about = "Distributed Version Control System")]
pub struct Cli {
    #[command(subcommand)]
    pub command: Command,
}

#[derive(Subcommand)]
pub enum Command {
    Init { path: String },
    Add { file: String },
    Commit { message: String },
    Log,
    Status,
    Push { remote: String },
    Pull { remote: String },
}

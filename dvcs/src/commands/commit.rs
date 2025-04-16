/**
 * @file commit.rs
 * @author Krisna Pranav
 * @brief commit[similar to git commit]
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use serde::{Serialize, Deserialize};
use std::fs::{self, File};
use std::io::{self, Write};
use std::path::{Path, PathBuf};
use std::time::{SystemTime, UNIX_EPOCH};
use std::process::Command;
use sha2::{Sha256, Digest};  // Add sha2 crate for hashing
use std::env;
use std::collections::HashMap;

#[derive(Serialize, Deserialize)]
pub struct Commit {
    pub message: String,
    pub author: String,
    pub timestamp: u64,
    pub commit_hash: String,
    pub tree_hash: String, // This represents the tree object associated with this commit
}

impl Commit {
    /// Creates a new Commit instance with dynamic values.
    ///
    /// # Arguments
    ///
    /// * `message` - A commit message describing the change (can be passed as a CLI argument).
    /// * `repo_path` - The path to the repository (to calculate the tree hash).
    ///
    /// # Returns
    ///
    /// Returns a new Commit instance.
    pub fn new(message: &str, repo_path: &Path) -> Self {
        let author = Commit::get_author();
        let timestamp = Commit::get_timestamp();
        let tree_hash = Commit::calculate_tree_hash(repo_path);
        let commit_hash = Commit::generate_commit_hash(message, &author, timestamp, &tree_hash);

        Commit {
            message: message.to_string(),
            author,
            timestamp,
            commit_hash,
            tree_hash,
        }
    }

    /// Generates a commit hash based on the commit details.
    ///
    /// # Arguments
    ///
    /// * `message` - A commit message describing the change.
    /// * `author` - The name of the person who made the commit.
    /// * `timestamp` - The timestamp of the commit.
    /// * `tree_hash` - The hash representing the tree object this commit points to.
    ///
    /// # Returns
    ///
    /// Returns a String representing the commit hash.
    fn generate_commit_hash(message: &str, author: &str, timestamp: u64, tree_hash: &str) -> String {
        let data = format!("{}{}{}{}", message, author, timestamp, tree_hash);
        let mut hasher = Sha256::new();
        hasher.update(data);
        let result = hasher.finalize();
        format!("{:x}", result)  // Return the hex string of the hash
    }

    /// Gets the current timestamp in seconds since UNIX epoch.
    ///
    /// # Returns
    ///
    /// Returns the current timestamp as a u64.
    fn get_timestamp() -> u64 {
        let start = SystemTime::now();
        let duration = start.duration_since(UNIX_EPOCH)
            .expect("Time went backwards");
        duration.as_secs()
    }

    /// Gets the author (username or email) of the current commit.
    ///
    /// # Returns
    ///
    /// Returns the author as a String.
    fn get_author() -> String {
        // Get the username of the user executing the commit (this can be improved based on your setup)
        if let Ok(username) = env::var("USER") {
            username
        } else {
            "Unknown Author".to_string() // Fallback to a default value
        }
    }

    /// Calculates the tree hash by hashing the contents of the repository files.
    ///
    /// # Arguments
    ///
    /// * `repo_path` - The path to the repository.
    ///
    /// # Returns
    ///
    /// Returns the hash of the repository's files (as a String).
    fn calculate_tree_hash(repo_path: &Path) -> String {
        // In a real system, this should hash the staged files or repository tree
        let files = Commit::get_repo_files(repo_path);
        let mut hasher = Sha256::new();
        for file in files {
            hasher.update(file);
        }
        let result = hasher.finalize();
        format!("{:x}", result)
    }

    /// Gets all the files in the repository (staged files in this case).
    ///
    /// # Arguments
    ///
    /// * `repo_path` - The path to the repository.
    ///
    /// # Returns
    ///
    /// Returns a Vec of file contents as `Vec<u8>`.
    fn get_repo_files(repo_path: &Path) -> Vec<Vec<u8>> {
        let mut files_data = Vec::new();
        if let Ok(entries) = fs::read_dir(repo_path) {
            for entry in entries.filter_map(Result::ok) {
                let path = entry.path();
                if path.is_file() {
                    if let Ok(data) = fs::read(path) {
                        files_data.push(data);
                    }
                }
            }
        }
        files_data
    }

    /// Save the commit to the repository (or log).
    ///
    /// # Arguments
    ///
    /// * `repo_path` - The path where the commit will be saved (like a `.dvcs/commits/` directory).
    ///
    /// # Returns
    ///
    /// Returns a Result indicating whether the commit was successfully saved.
    pub fn save(&self, repo_path: &Path) -> io::Result<()> {
        let commit_dir = repo_path.join("commits");
        if !commit_dir.exists() {
            fs::create_dir_all(&commit_dir)?;
        }

        let commit_file = commit_dir.join(&self.commit_hash);
        let mut file = File::create(commit_file)?;

        let commit_data = format!(
            "commit {}\nauthor {}\ndate {}\ntree_hash {}\n\n{}",
            self.commit_hash,
            self.author,
            self.timestamp,
            self.tree_hash,
            self.message
        );

        file.write_all(commit_data.as_bytes())?;
        Ok(())
    }

    /// Serialize the commit to a JSON string.
    pub fn serialize(&self) -> Result<String, serde_json::Error> {
        serde_json::to_string(self)
    }

    /// Prints the commit details to the console.
    pub fn display(&self) {
        println!("Commit: {}", self.commit_hash);
        println!("Author: {}", self.author);
        println!("Date: {}", self.timestamp);
        println!("Tree Hash: {}", self.tree_hash);
        println!("Message: {}", self.message);
    }

    pub fn execute(message: String) -> io::Result<()> {
        let current_dir = std::env::current_dir()?;

        let author = Self::get_author();

        // let tree_hash = Self::get_current_commit_hash()?;

        // let commit = Commit::new(&message, &author, &tree_hash);
        let commit = Commit::new(&message, &current_dir);

        commit.save(&current_dir)?;

        println!("Commit successful with message: {}", message);
        Ok(())
    }

}

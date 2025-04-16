/**
 * @file commit.rs
 * @author Krisna Pranav
 * @brief blob
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use serde::{Serialize, Deserialize};
use std::fs::File;
use std::io::{self, Write};
use std::path::Path;
use std::time::{SystemTime, UNIX_EPOCH};
use sha2::{Sha256, Digest};
use serde_json;

/// A struct representing a commit.
#[derive(Serialize, Deserialize)]
pub struct Commit {
    pub message: String,
    pub author: String,
    pub timestamp: u64,
    pub commit_hash: String,
    pub tree_hash: String, // This would typically represent the tree (files/directories) associated with this commit
}

impl Commit {
    /// Creates a new Commit instance.
    ///
    /// # Arguments
    ///
    /// * `message` - A commit message describing the change.
    /// * `author` - The name of the person who made the commit.
    /// * `tree_hash` - The hash representing the tree object this commit points to (the files and directories).
    ///
    /// # Returns
    ///
    /// Returns a new Commit instance.
    pub fn new(message: &str, author: &str, tree_hash: &str) -> Self {
        let timestamp = Commit::get_timestamp();
        let commit_hash = Commit::generate_commit_hash(message, author, timestamp, tree_hash);

        Commit {
            message: message.to_string(),
            author: author.to_string(),
            timestamp,
            commit_hash,
            tree_hash: tree_hash.to_string(),
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
            std::fs::create_dir_all(&commit_dir)?;
        }

        let commit_file = commit_dir.join(&self.commit_hash);
        let mut file = File::create(commit_file)?;

        // Serialize the commit object into a JSON string
        match self.serialize() {
            Ok(serialized_commit) => {
                // Convert the serialized string to a byte slice and write to the file
                file.write_all(serialized_commit.as_bytes())?;
                Ok(())
            },
            Err(e) => {
                // Handle the error case where serialization fails
                eprintln!("Error serializing commit: {}", e);
                Err(io::Error::new(io::ErrorKind::Other, "Serialization failed"))
            }
        }
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
}

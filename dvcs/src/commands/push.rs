/**
 * @file push.rs
 * @author Krisna Pranav
 * @brief push [similar to git push]
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use anyhow::{Context, Result};
use reqwest::Client;
use std::env;
use std::sync::{Arc, Mutex};

async fn push_to_remote(remote_url: &str, payload: Vec<u8>) -> Result<()> {
    let client = Client::new();
    let response = client
        .post(remote_url)
        .body(payload)
        .send()
        .await
        .context("Failed to send the payload to the remote server")?;

    if !response.status().is_success() {
        return Err(anyhow::anyhow!("Failed to push data: {}", response.status()).into());
    }

    Ok(())
}

async fn pull_from_remote(remote_url: &str) -> Result<Vec<u8>> {
    let client = Client::new();
    let response = client
        .get(remote_url)
        .send()
        .await
        .context("Failed to pull data from the remote server")?;

    if !response.status().is_success() {
        return Err(anyhow::anyhow!("Failed to pull data: {}", response.status()).into());
    }

    let data = response
        .bytes()
        .await
        .context("Failed to read response body")?
        .to_vec();

    Ok(data)
}

#[tokio::main]
async fn main() -> Result<()> {
    let remote_url = env::var("REMOTE_URL")
        .context("Failed to read REMOTE_URL from environment variable")?;

    let payload = vec![1, 2, 3, 4, 5];

    let push_result = tokio::spawn(async move {
        push_to_remote(&remote_url, payload).await
    });

    let pull_result = tokio::spawn(async move {
        pull_from_remote(&remote_url).await
    });

    let push_response = push_result.await??;
    let pull_response = pull_result.await??;

    Ok(())
}

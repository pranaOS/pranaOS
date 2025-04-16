/**
 * @file pull.rs
 * @author Krisna Pranav
 * @brief pull [similar to git pull]
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use anyhow::{Context, Result};
use reqwest::Client;
use serde::Deserialize;
use std::fs;
use std::sync::{Arc, Mutex};

#[derive(Deserialize)]
struct Config {
    remote: RemoteConfig,
}

#[derive(Deserialize)]
struct RemoteConfig {
    url: String,
}

fn read_config(config_path: &str) -> Result<Config> {
    let config_data = fs::read_to_string(config_path)
        .context("Failed to read the config file")?;
    let config: Config = toml::de::from_str(&config_data)
        .context("Failed to parse the config file")?;
    Ok(config)
}

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
    let config = read_config("../config.toml")?;

    // Use the remote_url from the config file
    let remote_url = &config.remote.url;
    let payload = vec![1, 2, 3, 4, 5]; // Replace with actual payload data

    let push_result = tokio::spawn(async move {
        push_to_remote(remote_url, payload).await
    });

    let pull_result = tokio::spawn(async move {
        pull_from_remote(remote_url).await
    });

    let push_response = push_result.await??;
    let pull_response = pull_result.await??;

    Ok(())
}

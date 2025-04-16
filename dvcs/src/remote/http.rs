/**
 * @file http.rs
 * @author Krisna Pranav
 * @brief http
 * @version 6.0
 * @date 2023-06-28
 *
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 *
 */

use reqwest::blocking::Client;
use std::error::Error;

pub fn push_to_remote(remote_url: &str, payload: Vec<u8>) -> Result<(), Box<dyn Error>> {
    let client = Client::new();
    let res = client.post(format!("{}/push", remote_url))
        .body(payload)
        .send()?;

    if res.status().is_success() {
        println!("Push successful.");
    } else {
        println!("Push failed: {}", res.status());
    }

    Ok(())
}

pub fn pull_from_remote(remote_url: &str) -> Result<Vec<u8>, Box<dyn Error>> {
    let client = Client::new();
    let res = client.get(format!("{}/pull", remote_url)).send()?;

    if res.status().is_success() {
        let data = res.bytes()?.to_vec();
        Ok(data)
    } else {
        Err(format!("Pull failed: {}", res.status()).into())
    }
}

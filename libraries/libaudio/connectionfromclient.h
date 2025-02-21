/**
 * @file connectionfromclient.h
 * @author Krisna Pranav
 * @brief Connection From Client
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

#pragma once

namespace Audio
{

    class ConnectionFromClient final : public IPC::ConnectionToServer<AudioClientEndpoint, AudioServerEndpoint>, public AudioClientEndpoint 
    {
    private:
        OwnPtr<AudioQueue> m_buffer;
        NonnullOwnPtr<UserSampleQueue> m_user_queue;
        NonnullRefPtr<Threading::Thread> m_background_audio_enqueuer;
        Core::EventLoop* m_enqueuer_loop;
        Threading::Mutex m_enqueuer_loop_destruction;
        Atomic<bool> m_audio_enqueuer_active { false };
        timespec m_good_sleep_time {};
    }; // class ConnectionFromClient final : public IPC::ConnectionToServer<AudioClientEndpoint, AudioServerEndpoint>, public AudioClientEndpoint 

} // namespace Audio
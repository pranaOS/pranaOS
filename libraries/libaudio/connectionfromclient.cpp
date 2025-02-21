/**
 * @file connectionfromclient.cpp
 * @author Krisna Pranav
 * @brief Connection From Client
 * @version 6.0
 * @date 2025-02-21
 * 
 * @copyright Copyright (c) 2021-2025 pranaOS Developers, Krisna Pranav
 * 
 */

 #include <AK/Atomic.h>
#include <AK/Format.h>
#include <AK/OwnPtr.h>
#include <AK/Time.h>
#include <AK/Types.h>
#include <LibAudio/ConnectionFromClient.h>
#include <LibAudio/UserSampleQueue.h>
#include <LibCore/Event.h>
#include <LibThreading/Mutex.h>
#include <time.h>

namespace Audio 
{
    /**
     * @brief Construct a new ConnectionFromClient::ConnectionFromClient object
     * 
     * @param socket 
     */
    ConnectionFromClient::ConnectionFromClient(NonnullOwnPtr<Core::Stream::LocalSocket> socket)
        : IPC::ConnectionToServer<AudioClientEndpoint, AudioServerEndpoint>(*this, move(socket))
        , m_buffer(make<AudioQueue>(MUST(AudioQueue::try_create())))
        , m_user_queue(make<UserSampleQueue>())
        , m_background_audio_enqueuer(Threading::Thread::construct([this]() {
            Core::EventLoop enqueuer_loop;
            m_enqueuer_loop = &enqueuer_loop;
            enqueuer_loop.exec();
            m_enqueuer_loop_destruction.lock();
            m_enqueuer_loop = nullptr;
            m_enqueuer_loop_destruction.unlock();
            return (intptr_t) nullptr;
        }))
    {
        m_background_audio_enqueuer->start();
        set_buffer(*m_buffer);
    }

    /**
     * @brief Destroy the ConnectionFromClient::ConnectionFromClient object
     * 
     */
    ConnectionFromClient::~ConnectionFromClient()
    {
        die();
    }

    void ConnectionFromClient::die()
    {
        m_enqueuer_loop_destruction.lock();

        if (m_enqueuer_loop != nullptr) {
            m_enqueuer_loop->wake();
            m_enqueuer_loop->quit(0);
        }

        m_enqueuer_loop_destruction.unlock();
        (void)m_background_audio_enqueuer->join();
    }

    /**
     * @param samples 
     * @return ErrorOr<void> 
     */
    ErrorOr<void> ConnectionFromClient::async_enqueue(FixedArray<Sample>&& samples)
    {
        update_good_sleep_time();
        m_user_queue->append(move(samples));
        
        if (!m_audio_enqueuer_active.load())
            m_enqueuer_loop->post_event(*this, make<Core::CustomEvent>(0), Core::EventLoop::ShouldWake::Yes);

        async_start_playback();

        return {};
    }

    void ConnectionFromClient::clear_client_buffer()
    {
        m_user_queue->clear();
    }

    void ConnectionFromClient::update_good_sleep_time()
    {
        auto sample_rate = static_cast<double>(get_sample_rate());
        auto buffer_play_time_ns = 1'000'000'000.0 / (sample_rate / static_cast<double>(AUDIO_BUFFER_SIZE));
        
        m_good_sleep_time = Time::from_nanoseconds(static_cast<unsigned>(buffer_play_time_ns)).to_timespec();
    }

    void ConnectionFromClient::custom_event(Core::CustomEvent&)
    {
        Array<Sample, AUDIO_BUFFER_SIZE> next_chunk;
        while (true) {
            m_audio_enqueuer_active.store(true);

            if (m_user_queue->is_empty()) {
                dbgln("Reached end of provided audio data, going to sleep");
                break;
            }

            auto available_samples = min(AUDIO_BUFFER_SIZE, m_user_queue->size());
            for (size_t i = 0; i < available_samples; ++i)
                next_chunk[i] = (*m_user_queue)[i];

            m_user_queue->discard_samples(available_samples);

            auto result = m_buffer->try_blocking_enqueue(next_chunk, [this]() {
                nanosleep(&m_good_sleep_time, nullptr);
            });
            if (result.is_error())
                dbgln("Error while writing samples to shared buffer: {}", result.error());
        }
        m_audio_enqueuer_active.store(false);
    }

    /**
     * @param samples 
     * @return ErrorOr<void, AudioQueue::QueueStatus> 
     */
    ErrorOr<void, AudioQueue::QueueStatus> ConnectionFromClient::realtime_enqueue(Array<Sample, AUDIO_BUFFER_SIZE> samples)
    {
        return m_buffer->try_enqueue(samples);
    }

    /**
     * @return unsigned 
     */
    unsigned ConnectionFromClient::total_played_samples() const
    {
        return m_buffer->weak_tail() * AUDIO_BUFFER_SIZE;
    }

    /**
     * @return unsigned 
     */
    unsigned ConnectionFromClient::remaining_samples()
    {
        return static_cast<unsigned>(m_user_queue->remaining_samples());
    }

    /**
     * @return size_t 
     */
    size_t ConnectionFromClient::remaining_buffers() const
    {
        return m_buffer->size() - m_buffer->weak_remaining_capacity();
    }

    /**
     * @param muted 
     */
    void ConnectionFromClient::main_mix_muted_state_changed(bool muted)
    {
        if (on_main_mix_muted_state_change)
            on_main_mix_muted_state_change(muted);
    }

    /**
     * @param volume 
     */
    void ConnectionFromClient::main_mix_volume_changed(double volume)
    {
        if (on_main_mix_volume_change)
            on_main_mix_volume_change(volume);
    }

    /**
     * @param volume 
     */
    void ConnectionFromClient::client_volume_changed(double volume)
    {
        if (on_client_volume_change)
            on_client_volume_change(volume);
    }

} // namespace Audio

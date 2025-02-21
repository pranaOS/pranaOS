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

#include <mods/concept.h>
#include <mods/fixedarray.h>
#include <mods/nonnullownptr.h>
#include <mods/ownptr.h>
#include <libaudio/buffer.h>
#include <libaudio/usersamplequeue.h>
#include <libcore/eventloop.h>
#include <libcore/object.h>
#include <libipc/connectiontoserver.h>
#include <libthreading/mutex.h>
#include <libthreading/thread.h>
#include <userland/services/audioserver/audioclientendpoint.h>
#include <userland/services/audioserver/audioserverendpoint.h>

namespace Audio 
{

    class ConnectionFromClient final
        : public IPC::ConnectionToServer<AudioClientEndpoint, AudioServerEndpoint>
        , public AudioClientEndpoint 
    {
        IPC_CLIENT_CONNECTION(ConnectionFromClient, "/tmp/portal/audio")
    public:
        /**
         * @brief Destroy the Connection From Client object
         * 
         */
        virtual ~ConnectionFromClient() override;

        /**
         * @tparam Samples 
         * @param samples 
         * @return ErrorOr<void> 
         */
        template<ArrayLike<Sample> Samples>
        ErrorOr<void> async_enqueue(Samples&& samples)
        {
            return async_enqueue(TRY(FixedArray<Sample>::try_create(samples.span())));
        }

        /**
         * @param samples 
         * @return ErrorOr<void> 
         */
        ErrorOr<void> async_enqueue(FixedArray<Sample>&& samples);

        void clear_client_buffer();

        /**
         * @param samples 
         * @return ErrorOr<void, AudioQueue::QueueStatus> 
         */
        ErrorOr<void, AudioQueue::QueueStatus> realtime_enqueue(Array<Sample, AUDIO_BUFFER_SIZE> samples);

        
        unsigned total_played_samples() const;
        
        unsigned remaining_samples();

        size_t remaining_buffers() const;

        virtual void die() override;

        Function<void(bool muted)> on_main_mix_muted_state_change;
        Function<void(double volume)> on_main_mix_volume_change;
        Function<void(double volume)> on_client_volume_change;

    private:
        /**
         * @brief Construct a new Connection From Client object
         * 
         */
        ConnectionFromClient(NonnullOwnPtr<Core::Stream::LocalSocket>);

        virtual void main_mix_muted_state_changed(bool) override;
        virtual void main_mix_volume_changed(double) override;
        virtual void client_volume_changed(double) override;

        virtual void custom_event(Core::CustomEvent&) override;

        void update_good_sleep_time();

        OwnPtr<AudioQueue> m_buffer;

        NonnullOwnPtr<UserSampleQueue> m_user_queue;

        NonnullRefPtr<Threading::Thread> m_background_audio_enqueuer;

        Core::EventLoop* m_enqueuer_loop;
        Threading::Mutex m_enqueuer_loop_destruction;

        Atomic<bool> m_audio_enqueuer_active { false };

        timespec m_good_sleep_time {};
    }; // class ConnectionFromClient

} // namespace Audio

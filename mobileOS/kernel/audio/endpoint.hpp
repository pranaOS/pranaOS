/**
 * @file endpoint.hpp
 * @author Krisna Pranav
 * @brief Endpoint for audio format and abstract streamer
 * @version 0.1
 * @date 2024-07-30
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma once

#include "abstractstream.hpp"
#include "audioformat.hpp"
#include <chrono>
#include <cstdint>
#include <optional>
#include <vector>

namespace audio
{
  class Endpoint
  {
    public:
      struct Traits
      {
          bool usesDMA = false;
          std::optional<std::size_t> blockSizeConstraint = std::nullopt;
          std::optional<std::chrono::milliseconds> timeConstraint = std::nullopt;
      }; // struct Traits

      Endpoint() = default;

      /**
       * @param stream
       */
      void connectStream(AbstractStream& stream);

      void disconnectStream();
      bool isConnected() const noexcept;

      [[nodiscard]] virtual auto getTraits() const -> Traits = 0;

      auto isFormatSupported(const AudioFormat& format) -> bool;

      virtual auto getSupportedFormats() -> std::vector<AudioFormat> = 0;

    protected:
      AbstractStream* _stream = nullptr;
  }; // class Endpoint

  class Sink : public Endpoint
  {
    public:
      virtual auto getSinkFormat() -> AudioFormat = 0;
      virtual void onDataSend() = 0;
      virtual void enableOutput() = 0;
      virtual void disableOutput() = 0;
  }; // class Sink

  class Source : public Endpoint
  {
    public:
      virtual auto getourceFormat() -> AudioFormat = 0;
      virtual void onDataReceive() = 0;
      virtual void enableInput() = 0;
      virtual void disableInput() = 0;
  }; // class Source

  class IOProxy : public Source, public Sink
  {
    public:
      inline bool isSinkConnected() const noexcept
      {
          return Sink::isConnected();
      }

      inline bool isSourceConnected() const noexcept
      {
          return Source::isConnected();
      }

      inline void connectOutputStream(AbstractStream& stream)
      {
          Sink::connectStream(stream);
      }

      inline void connectInputStream(AbstractStream& stream)
      {
          Source::connectStream(stream);
      }

      inline auto isFormatSupportedBySink(const AudioFormat& format) -> bool
      {
          return Sink::isFormatSupported(format);
      }

      inline auto isFormatSupportedBySource(const AudioFormat& format) -> bool
      {
          return Source::isFormatSupported(format);
      }
  }; // class IOProxy

  class StreamConection
  {
    public:
      StreamConection() = default;
      StreamConection(Source* source, Sink* sink, AbstractStream* stream);
      ~StreamConection();

      void enable();
      void disbale();
      void destroy();

      [[nodiscard]] Source* getSource() const noexcept;
      [[nodiscard]] Sink* getSink() const noexcept;
      [[nodiscard]] AbstractStream* getStream() const noexcept;
      [[nodiscard]] bool isEnabled() const noexcept;

    private:
      bool enabled = false;
      Sink* _sink = nullptr;
      Source* _source = nullptr;
      AbstractStream* _stream = nullptr;
  };

} // namespace audio
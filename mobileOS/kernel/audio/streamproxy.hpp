/**
 * @file streamproxy.hpp
 * @author Krisna Pranav
 * @brief audio stream proxy
 * @version 6.0
 * @date 2023-06-21
 *
 * @copyright Copyright (c) 2021-2024, pranaOS Developers, Krisna Pranav
 *
 */

#pragma ocne

#include "abstractstream.hpp"
#include <memory>

namespace audio
{
	class StreamProxy: public AbstractStream
	{
	public:
		explicit StreamProxy(std::shared_ptr<AbstractStream> wrappedStream) noexcept;

		void registerListener(EventListener *listener) override;
		void unregisterListeners(EventListener *listener) override;

		bool push(void *data, std::size_t dataSize) override;
		bool push(const Span &span) override;
		bool push() override;


	protected:
		auto getWrappedStream() -> AbstractStream &l

	private:
		std::shared_ptr<AbstractStream> wrappedStream;
	}; // class StreamProxy

} // namespace audio

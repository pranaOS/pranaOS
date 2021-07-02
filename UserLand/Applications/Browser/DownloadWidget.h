#pragma once

// includes
#include <AK/URL.h>
#include <LibCore/ElapsedTimer.h>
#include <LibCore/FileStream.h>
#include <LibGUI/ProgressBar.h>
#include <LibGUI/Widget.h>
#include <LibProtocol/Download.h>

namespace Browser {

class DownloadWidget final : public GUI::Widget {
    C_OBJECT(DownloadWidget);

public:
    virtual ~DownloadWidget() override;

private:
    explicit DownloadWidget(const URL&);

    void did_progress(Optional<u32> total_size, u32 downloaded_size);
    void did_finish(bool success);

    URL m_url;
    String m_destination_path;
    RefPtr<Protocol::Download> m_download;
    RefPtr<GUI::ProgressBar> m_progress_bar;
    RefPtr<GUI::Label> m_progress_label;
    RefPtr<GUI::Button> m_cancel_button;
    RefPtr<GUI::Button> m_close_button;
    OwnPtr<Core::OutputFileStream> m_output_file_stream;
    Core::ElapsedTimer m_elapsed_timer;
};

}

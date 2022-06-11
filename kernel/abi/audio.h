#pragma once

enum soundEncode {
    PCMS16LE = 0, 
    PCMS20LE = 1,
};

enum audioMixerIoCtl {
    IoCtlMixerSetMasterVolume = 0x1000,
    IoCtlMixerGetMasterVolume = 0x1001,
};

enum audioOutputIoCtl {
    IoCtlOutputSetVolume = 0x1000,
    IoCtlOutputGetVolume = 0x1001,
    IoCtlOutputGetEncoding = 0x1002,
    IoCtlOutputGetSampleRate = 0x1003,
    IoCtlOutputSetNumberOfChannels = 0x1004,
    IoCtlOutputGetNumberOfChannels = 0x1005,
    IoCtlOutputSetAsync = 0x1006,
};

#define PRANAOS_ABI_AUDIO_ENCODING_COUNT 2

static const char* const pranaosABIAudioEncodingNames[] = {
    "PCM signed 16-bit little endian",
    "PCM signed 20-bit little endian",
};
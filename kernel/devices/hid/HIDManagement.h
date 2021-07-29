/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#pragma once

// includes
#include <base/Atomic.h>
#include <base/CircularQueue.h>
#include <base/NonnullRefPtrVector.h>
#include <base/RefPtr.h>
#include <base/Time.h>
#include <base/Types.h>
#include <kernel/API/KeyCode.h>
#include <kernel/API/MousePacket.h>
#include <kernel/KResult.h>
#include <kernel/SpinLock.h>
#include <kernel/UnixTypes.h>
#include <libkeyboard/CharacterMap.h>

namespace Kernel {

extern Atomic<bool> g_caps_lock_remapped_to_ctrl;

class HIDDevice;
class I8042Controller;
class MouseDevice;
class KeyboardDevice;
class KeyboardClient;
class HIDManagement {
    friend class KeyboardDevice;
    friend class MouseDevice;
    BASE_MAKE_ETERNAL;

public:
    HIDManagement();
    static void initialize();
    static HIDManagement& the();

    void enumerate();

    const String& keymap_name() const { return m_character_map.character_map_name(); }
    const Keyboard::CharacterMapData& character_maps() const { return m_character_map.character_map_data(); }
    const Keyboard::CharacterMap& character_map() const { return m_character_map; }
    void set_client(KeyboardClient* client) { m_client = client; }
    void set_maps(const Keyboard::CharacterMapData& character_map, const String& character_map_name);

private:
    size_t generate_minor_device_number_for_mouse();
    size_t generate_minor_device_number_for_keyboard();

    size_t m_mouse_minor_number { 0 };
    size_t m_keyboard_minor_number { 0 };
    Keyboard::CharacterMap m_character_map;
    KeyboardClient* m_client { nullptr };
    RefPtr<I8042Controller> m_i8042_controller;
    NonnullRefPtrVector<HIDDevice> m_hid_devices;
};

class KeyboardClient {
public:
    virtual ~KeyboardClient() = default;
    virtual void on_key_pressed(KeyEvent) = 0;
};

}
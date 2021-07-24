/*
 * Copyright (c) 2021, Krisna Pranav
 *
 * SPDX-License-Identifier: BSD-2-Clause
*/

#include <base/Format.h>
#include <base/LexicalPath.h>
#include <base/QuickSort.h>
#include <base/Result.h>
#include <base/String.h>
#include <base/StringView.h>
#include <base/Vector.h>
#include <libcore/ConfigFile.h>
#include <libcore/File.h>
#include <spawn.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

enum class ComponentCategory {
    Optional,
    Recommended,
    Required
};

struct ComponentData {
    String name;
    String description;
    ComponentCategory category { ComponentCategory::Optional };
    bool was_selected { false };
    Vector<String> dependencies;
    bool is_selected { false };
};

struct WhiptailOption {
    String tag;
    String name;
    String description;
    bool checked { false };
};

enum class WhiptailMode {
    Menu,
    Checklist
};

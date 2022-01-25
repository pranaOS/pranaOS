//
// Created by KrisnaPranav on 25/01/22.
//

#pragma once

typedef struct font {
    uint8_t magic[2];
    uint8_t mode;
    uint8_t height;
} font_t;

uint32_t font_len = 4100;

uint8_t font_psf[] = {
};
/**
 * ----------------------------------------------------------------------------
 * Handling images on the Gamebuino META
 * © 2021 Stéphane Calderoni
 * ----------------------------------------------------------------------------
 * Assets encoded for full 16-bits RGB565 display mode
 * ----------------------------------------------------------------------------
 */

#pragma once

const uint16_t SPRITE_DATA[] = {

    // metadata

    8,      // frame width
    8,      // frame height
    4,      // frames
    0,      // frame loop
    0xf81f, // transparent color
    0,      // 16-bits color mode

    // colormap

    // frame 1/4
    0xf81f, 0xf81f, 0x632c, 0xad55, 0xad55, 0xad55, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0x0000, 0xff36, 0x0000, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xff36, 0xb4df, 0xb4df, 0xb4df, 0xff36, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6217, 0xf81f, 0xf81f, 0x6217, 0xf81f, 0xf81f,

    // frame 2/4
    0xf81f, 0xf81f, 0x632c, 0xad55, 0xad55, 0xad55, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0x0000, 0xff36, 0x0000, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xff36, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0x7afa, 0xff36,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x7afa, 0x6217, 0xf81f, 0xf81f, 0xf81f,

    // frame 3/4
    0xf81f, 0xf81f, 0x632c, 0xad55, 0xad55, 0xad55, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0x0000, 0xff36, 0x0000, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xff36, 0xb4df, 0xb4df, 0xb4df, 0xff36, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6217, 0xf81f, 0xf81f, 0x6217, 0xf81f, 0xf81f,

    // frame 4/4
    0xf81f, 0xf81f, 0x632c, 0xad55, 0xad55, 0xad55, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0x0000, 0xff36, 0x0000, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xee2f, 0xff36, 0xff36, 0xff36, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0x7afa, 0xff36, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x7afa, 0xb4df, 0xb4df, 0xb4df, 0xf81f, 0xf81f,
    0xf81f, 0x6217, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0x7afa, 0xf81f

};

const uint16_t TILESET_DATA[] = {

    // metadata

    16,     // frame width
    8,      // frame height
    4,      // frames
    0,      // frame loop
    0xf81f, // transparent color
    0,      // 16-bits color mode

    // colormap

    // frame 1/4
    0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x1926, 0x0000,
    0x1926, 0x1926, 0x10e4, 0x1926, 0x10e4, 0x1926, 0x10e4, 0x1926, 0x10e4, 0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862,
    0x1926, 0x10e4, 0x1926, 0x10e4, 0x1926, 0x10e4, 0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862,
    0x1926, 0x1926, 0x10e4, 0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862,
    0x1926, 0x10e4, 0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862,
    0x1926, 0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862,
    0x1926, 0x10e4, 0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862, 0x0862,
    0x1926, 0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862,

    // frame 2/4
    0x4228, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c, 0x632c,
    0xad55, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228,
    0x632c, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228, 0x4228,
    0x0000, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186,
    0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000,
    0x0000, 0x18c3, 0x0000, 0x3186, 0x0000, 0x18c3, 0x0000, 0x3186, 0x0000, 0x18c3, 0x0000, 0x3186, 0x0000, 0x18c3, 0x0000, 0x3186,
    0x3186, 0x0000, 0x3186, 0x0000, 0x3186, 0x0000, 0x3186, 0x0000, 0x3186, 0x0000, 0x3186, 0x0000, 0x3186, 0x0000, 0x3186, 0x0000,
    0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3,

    // frame 3/4
    0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862, 0x0862,
    0x1926, 0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862, 0x10e4, 0x0862,
    0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862, 0x0862,
    0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862, 0x10e4, 0x0862,
    0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862, 0x10e4, 0x0862, 0x0862,
    0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862, 0x10e4, 0x0862, 0x10e4, 0x0862, 0x10e4, 0x0862,
    0x1926, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x10e4, 0x0862, 0x10e4, 0x0862, 0x10e4, 0x0862, 0x10e4, 0x0862, 0x10e4, 0x0862, 0x0862,
    0x0000, 0x0862, 0x0862, 0x0862, 0x0862, 0x0862, 0x0862, 0x0862, 0x0862, 0x0862, 0x0862, 0x0862, 0x0862, 0x0862, 0x0862, 0x0000,

    // frame 4/4
    0x0000, 0x632c, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x3186, 0x18c3,
    0x632c, 0x3186, 0x18c3, 0x3186, 0x18c3, 0x3186, 0x18c3, 0x3186, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x0000,
    0x3186, 0x18c3, 0x3186, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x0000,
    0x3186, 0x3186, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x0000,
    0x3186, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x0000, 0x0000,
    0x3186, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000,
    0x3186, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x18c3, 0x0000, 0x0000,
    0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x18c3

};

const uint16_t TORCH_DATA[] = {

    // metadata

    8,      // frame width
    16,     // frame height
    10,     // frames
    2,      // frame loop
    0xf81f, // transparent color
    0,      // 16-bits color mode

    // colormap

    // frame 1/10
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x8b27, 0x8b27, 0x6a86, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x6a86, 0x6a86, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x6a86, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x8b27, 0x8b27, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x49c4, 0x49c4, 0xf81f, 0xf81f, 0xf81f,

    // frame 2/10
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xfb20, 0xfd40, 0xfd40, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xfb20, 0xfd40, 0xfd40, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x8b27, 0x8b27, 0x6a86, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x6a86, 0x6a86, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x6a86, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x8b27, 0x8b27, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x49c4, 0x49c4, 0xf81f, 0xf81f, 0xf81f,

    // frame 3/10
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xfb20, 0xfb20, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xfb20, 0xfd40, 0xfd40, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xfb20, 0xfd40, 0xfee4, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x8b27, 0x8b27, 0x6a86, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x6a86, 0x6a86, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x6a86, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x8b27, 0x8b27, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x49c4, 0x49c4, 0xf81f, 0xf81f, 0xf81f,

    // frame 4/10
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xfb20, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x8b27, 0x8b27, 0x6a86, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x6a86, 0x6a86, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x6a86, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x8b27, 0x8b27, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x49c4, 0x49c4, 0xf81f, 0xf81f, 0xf81f,

    // frame 5/10
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x8b27, 0x8b27, 0x6a86, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x6a86, 0x6a86, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x6a86, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x8b27, 0x8b27, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x49c4, 0x49c4, 0xf81f, 0xf81f, 0xf81f,

    // frame 6/10
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfd40, 0xfb20, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfee4, 0xfb20, 0xfb20, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x8b27, 0x8b27, 0x6a86, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x6a86, 0x6a86, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x6a86, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x8b27, 0x8b27, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x49c4, 0x49c4, 0xf81f, 0xf81f, 0xf81f,

    // frame 7/10
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfb20, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x8b27, 0x8b27, 0x6a86, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x6a86, 0x6a86, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x6a86, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x8b27, 0x8b27, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x49c4, 0x49c4, 0xf81f, 0xf81f, 0xf81f,

    // frame 8/10
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x8b27, 0x8b27, 0x6a86, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x6a86, 0x6a86, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x6a86, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x8b27, 0x8b27, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x49c4, 0x49c4, 0xf81f, 0xf81f, 0xf81f,

    // frame 9/10
    0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfb20, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x8b27, 0x8b27, 0x6a86, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x6a86, 0x6a86, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x6a86, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x8b27, 0x8b27, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x49c4, 0x49c4, 0xf81f, 0xf81f, 0xf81f,

    // frame 10/10
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0xfb20, 0xf81f, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfd40, 0xfb20, 0xf81f, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfd40, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xfb20, 0xfee4, 0xfee4, 0xfb20, 0xf81f, 0xf81f,
    0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x8b27, 0x8b27, 0x6a86, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x6a86, 0x6a86, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x6a86, 0x8b27, 0x8b27, 0x6a86, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0x49c4, 0x8b27, 0x8b27, 0x49c4, 0xf81f, 0xf81f,
    0xf81f, 0xf81f, 0xf81f, 0x49c4, 0x49c4, 0xf81f, 0xf81f, 0xf81f

};
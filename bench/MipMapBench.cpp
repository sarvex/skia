/*
 * Copyright 2015 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "Benchmark.h"
#include "SkBitmap.h"
#include "SkMipMap.h"

class MipMapBench: public Benchmark {
    SkBitmap fBitmap;

public:
    MipMapBench() {}

protected:
    bool isSuitableFor(Backend backend) SK_OVERRIDE {
        return kNonRendering_Backend == backend;
    }

    const char* onGetName() SK_OVERRIDE { return "mipmap_build"; }

    void onPreDraw() SK_OVERRIDE {
        fBitmap.allocN32Pixels(1000, 1000, true);
        fBitmap.eraseColor(SK_ColorWHITE);  // so we don't read uninitialized memory
    }

    void onDraw(const int loops, SkCanvas*) SK_OVERRIDE {
        for (int i = 0; i < loops; i++) {
            SkMipMap::Build(fBitmap, NULL)->unref();
        }
    }

private:
    typedef Benchmark INHERITED;
};

DEF_BENCH( return new MipMapBench; )

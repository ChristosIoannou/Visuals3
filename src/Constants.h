#pragma once

#define SPECTRAL_BANDS 48   // 48 is good for Release, 32 for Debug (fftBands)
#define FREQ_MAX 5025.0     // 44100.0
#define BARK_MAX 19
// buffersize = SPECTRAL_BANDS * 4;
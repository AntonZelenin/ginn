#include <assert.h>
#include "args.h"
#include <memory>

#define N_CEP 26
#define N_CONTEXT 9
#define BEAM_WIDTH 500
#define LM_WEIGHT 1.50f
#define VALID_WORD_COUNT_WEIGHT 2.25f

typedef struct {
  const char* string;
  double cpu_time_overall;
} ds_result;

typedef struct {
  char*  buffer;
  size_t buffer_size;
  int    sample_rate;
} ds_audio_buffer;

ds_result LocalDsSTT(ModelState* aCtx, const short* aBuffer, size_t aBufferSize, int aSampleRate);
ds_audio_buffer GetAudioBuffer(const char* path);
void ProcessFile(ModelState* context, const char* path, bool show_times);

#include "client_main.h"

using namespace std;

int main(int argc, char **argv)
{
  if (!ProcessArgs(argc, argv)) {
    return 1;
  }

  ModelState* ctx;
  int status = DS_CreateModel(model, N_CEP, N_CONTEXT, alphabet, BEAM_WIDTH, &ctx);
  if (status != 0) {
    fprintf(stderr, "Could not`create model.\n");
    return 1;
  }

  if (lm && trie) {
    int status = DS_EnableDecoderWithLM(ctx,
                                        alphabet,
                                        lm,
                                        trie,
                                        LM_WEIGHT,
                                        VALID_WORD_COUNT_WEIGHT);
    if (status != 0) {
      fprintf(stderr, "Could not enable CTC decoder with LM.\n");
      return 1;
    }
  }

  StreamingState* strs;
  status = DS_SetupStream(ctx, 0, 16000, &strs);
  if (status != 0) {
    fprintf(stderr, "Could not create streaming state.\n");
    return 1;
  }

  // Initialise SOX
  // assert(sox_init() == SOX_SUCCESS);

  int buffer_size = 256;
  short *buffer = new short[buffer_size];
  shared_ptr<FILE> pipe(popen("rec -q -V0 -e signed -L -c 1 -b 16 -r 16k -t raw - gain -2", "r"), pclose);
  if (!pipe) throw std::runtime_error("popen() failed!");

  printf("start recording 1\n");
  for (int n = 0; n < 900; n++) {
    fread(buffer, sizeof(short), buffer_size / 2, pipe.get());
    DS_FeedAudioContent(strs, buffer, buffer_size / 2);
    if (n % 300 == 0) {
      std::cout << DS_IntermediateDecode(strs) << endl;
      DS_DiscardStream(strs);
      DS_SetupStream(ctx, 0, 16000, &strs);
    }
  }
  printf("finished recording 1\n");

  std::cout << DS_FinishStream(strs) << endl;

  // Deinitialise and quit
  // sox_quit();
  DS_DestroyModel(ctx);
  printf("Model destroyed\n");

  return 0;
}

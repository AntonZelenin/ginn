include definitions.mk

ginn: client_main.cc
	$(CXX) -std=c++11 -o build/ginn $(CFLAGS) $(SOX_CFLAGS) client_main.cc $(GINN_CFLAGS) $(LDFLAGS) $(SOX_LDFLAGS)
ifeq ($(OS),Darwin)
	install_name_tool -change $$TASKCLUSTER_TASK_DIR/homebrew/opt/sox/lib/libsox.3.dylib @rpath/libsox.3.dylib deepspeech
	install_name_tool -change bazel-out/local-opt/bin/native_client/libdeepspeech.so @rpath/libdeepspeech.so deepspeech
endif

ginnd: client_main.cc
	$(CXX) -g -std=c++11 -o build/ginnd $(CFLAGS) $(SOX_CFLAGS) client_main.cc $(GINN_CFLAGS) $(LDFLAGS) $(SOX_LDFLAGS)
ifeq ($(OS),Darwin)
	install_name_tool -change $$TASKCLUSTER_TASK_DIR/homebrew/opt/sox/lib/libsox.3.dylib @rpath/libsox.3.dylib deepspeech
	install_name_tool -change bazel-out/local-opt/bin/native_client/libdeepspeech.so @rpath/libdeepspeech.so deepspeech
endif

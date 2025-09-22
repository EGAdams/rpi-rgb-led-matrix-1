#include <napi.h>
#include "CppInterface.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return CppInterface::Init(env, exports);
}

// NODE_API_MODULE(addon, InitAll)
NODE_API_MODULE( pickle_cpp, InitAll )

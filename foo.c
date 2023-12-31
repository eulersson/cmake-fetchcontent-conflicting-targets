#include <zmq.h>
#include <cjson/cJSON.h>

int main(void) {
  void *context = zmq_ctx_new();
  cJSON *json = cJSON_Parse("{\"key1\":\"val1\"}");
}

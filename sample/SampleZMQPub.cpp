#include <zmq.h>
#include <unistd.h>
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
  if(argc < 2 ) {
    std::cout << "Usage: samplezmqpub port" << std::endl;
    return 0;
  }
  void *pContext = zmq_ctx_new();
  void *pSocket = zmq_socket(pContext, ZMQ_PUB);
  std::string c = "tcp://*:" + std::string(argv[1]);
  zmq_bind(pSocket,c.c_str());
  while(1) {
    zmq_send(pSocket,"pose::1408580917.370764,0.226601,0.376283,1.35372,0.00474462,-0.758908,-0.650894,-0.0193067",91,ZMQ_DONTWAIT);
    sleep(1);
  }
  return 0;
}

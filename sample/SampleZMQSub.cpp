#include <iostream>
#include <iomanip>
#include <zmq.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

void *pContext;
void *pSocket;
extern int errno;

int main(int argc, char *argv[])
{
  if(argc < 2 ) {
    std::cout << "Usage: samplezmqsub ip:port" << std::endl;
    return 0;
  } 

  pContext = zmq_ctx_new();
  pSocket = zmq_socket(pContext,ZMQ_SUB);
  std::string connection = "tcp://" + std::string(argv[1]);
  int ret = zmq_connect(pSocket,connection.c_str());
  assert(ret==0);
  ret = zmq_setsockopt (pSocket, ZMQ_SUBSCRIBE,
                        "pose::", 6);
  assert(ret==0);
  while(1) {
    char buff[1024];
    ret = zmq_recv(pSocket,buff,sizeof(char)*1024,0);
    if(ret > 0) {
      std::cout << buff << std::endl;
    } else if(ret < 0) {
      std::cout << "ERROR: " << strerror(errno) << std::endl << std::endl;
    }
  }
  return 0;
}

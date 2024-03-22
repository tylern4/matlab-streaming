#include <iostream>
#include <string>

#include <fmt/core.h>
#include <zmq.hpp>

int main() {
  // initialize the zmq context with a single IO thread
  zmq::context_t context{1};

  // construct a REQ (request) socket and connect to interface
  zmq::socket_t socket{context, zmq::socket_type::req};
  socket.connect("tcp://localhost:5555");

  for (auto request_num = 0; request_num < 1000000; ++request_num) {
    double *ptr = (double *)malloc(sizeof(double));
    *ptr = (double)request_num;
    zmq::mutable_buffer msg_data = zmq::buffer(ptr, sizeof(double));

    socket.send(msg_data, zmq::send_flags::none);
    zmq::message_t reply{};
    auto out = socket.recv(reply, zmq::recv_flags::none);
    auto data = *(double *)(reply.data());
    if (request_num % 10000 == 0) {
      fmt::print("data = {}\n", data);
    }
  }

  return 0;
}

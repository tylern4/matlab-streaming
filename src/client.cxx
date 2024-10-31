#include <iostream>
#include <string>
#include <chrono>
#include <fmt/core.h>
#include <zmq.hpp>
#include "clipp.h"

int main(int argc, char **argv) {
  std::string host = "localhost";
  int port = 5555;
  int num = 1000;
  int print_num = 10;
  int length = 1700;
  auto cli = (clipp::option("-p", "--port") & clipp::value("port", port),
              clipp::option("-h", "--host") & clipp::value("host", host),
              clipp::option("-n", "--num") & clipp::value("num", num),
              clipp::option("-d", "--debug") & clipp::value("debug", print_num));

  if (!clipp::parse(argc, argv, cli)) {
    std::cout << clipp::make_man_page(cli, argv[0]);
    exit(2);
  }
  fmt::print("tcp://{}:{}\n\n", host, port);
  // initialize the zmq context with a single IO thread
  zmq::context_t context{1};

  // construct a REQ (request) socket and connect to interface
  zmq::socket_t socket{context, zmq::socket_type::req};
  socket.connect(fmt::format("tcp://{}:{}", host, port));

  for (auto request_num = 0; request_num < num; ++request_num) {
    // send a message
    auto msg = std::make_unique<double[]>(length);
    for (int i = 0; i < length; i++) {
	msg[i] = 1.0;
    }

    const auto p1 = std::chrono::system_clock::now();
    auto start = std::chrono::duration_cast<std::chrono::nanoseconds>(p1.time_since_epoch()).count();

    zmq::mutable_buffer msg_data = zmq::buffer(msg.get(), sizeof(double)*length);
    socket.send(msg_data, zmq::send_flags::none);

    // Recive the message back
    zmq::message_t reply{};
    auto out = socket.recv(reply, zmq::recv_flags::none);
    auto data = *(double *)(reply.data());
    const auto p2 = std::chrono::system_clock::now();
    auto end = std::chrono::duration_cast<std::chrono::nanoseconds>(p2.time_since_epoch()).count();

    
    fmt::print("roundtrip = {}\n", end-start);
    
  }

  return 0;
}

#include <fmt/core.h>
#include <chrono>
#include <iostream>
#include <numeric>
#include <string>
#include <thread>
#include <zmq.hpp>
#include "clipp.h"
#include "constants.hpp"

int main(int argc, char** argv) {
  int port = 5555;
  auto cli = (clipp::option("-p", "--port") & clipp::value("port", port));

  if (!clipp::parse(argc, argv, cli)) {
    std::cout << clipp::make_man_page(cli, argv[0]);
    exit(2);
  }

  fmt::print("tcp://*:{}\n\n", port);
  // initialize the zmq context with a single IO thread
  zmq::context_t context{1};

  // construct a REP (reply) socket and bind to interface
  zmq::socket_t socket{context, zmq::socket_type::rep};

  socket.bind(fmt::format("tcp://*:{}", port));

  // prepare some static data for responses
  const std::string data{"0"};

  size_t i = 0;
  fmt::print("Starting server on {}\n", port);
  for (;;) {
    zmq::message_t request;
    auto ptr = std::make_unique<FLOAT_TYPE[]>(SIZE * SIZE);
    zmq::mutable_buffer msg_data = zmq::buffer(ptr.get(), sizeof(FLOAT_TYPE) * SIZE * SIZE);
    auto out = socket.recv(msg_data, zmq::recv_flags::none);

    std::vector<FLOAT_TYPE> data(msg_data.size() / sizeof(FLOAT_TYPE));
    memcpy(data.data(), msg_data.data(), msg_data.size());
    // std::cout << "Message size: " << msg_data.size() << " bytes" << std::endl;

    auto result = (FLOAT_TYPE)msg_data.size();

    zmq::mutable_buffer msg_ptr_out = zmq::buffer(&result, sizeof(FLOAT_TYPE));

    // send the reply to the client
    socket.send(msg_ptr_out, zmq::send_flags::none);
  }

  return 0;
}

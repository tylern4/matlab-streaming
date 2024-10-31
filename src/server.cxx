#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <numeric>
#include <fmt/core.h>
#include <zmq.hpp>
#include "clipp.h"

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

    // receive a request from client
    // socket.recv(request, zmq::recv_flags::none);
    // std::cout << "Received " << request.to_string() << std::endl;

    // double *ptr = (double *)malloc(sizeof(double));
    auto ptr = std::make_unique<double[]>(1700);
    zmq::mutable_buffer msg_data = zmq::buffer(ptr.get(), sizeof(double)*1700);
    auto out = socket.recv(msg_data, zmq::recv_flags::none);

    std::vector<double> data(msg_data.size() / sizeof(double));
    memcpy(data.data(), msg_data.data(), msg_data.size());
    // if (i++ % 1000 == 0) std::cout << i << " Received " << data[0] << std::endl;
    auto result = std::reduce(data.begin(), data.end());
    // std::cout << result << std::endl;

    zmq::mutable_buffer msg_ptr_out = zmq::buffer(&result, sizeof(double));

    // send the reply to the client
    socket.send(msg_ptr_out, zmq::send_flags::none);
  }

  return 0;
}

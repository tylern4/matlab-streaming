#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>

int main()
{
    using namespace std::chrono_literals;

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{context, zmq::socket_type::rep};
    socket.bind("tcp://*:5555");

    // prepare some static data for responses
    const std::string data{"0"};

    size_t i = 0;

    for (;;)
    {
        zmq::message_t request;

        // receive a request from client
        // socket.recv(request, zmq::recv_flags::none);
        // std::cout << "Received " << request.to_string() << std::endl;

        // double *ptr = (double *)malloc(sizeof(double));
        auto ptr = std::make_unique<double>();
        zmq::mutable_buffer msg_data = zmq::buffer(ptr.get(), sizeof(double));
        auto out = socket.recv(msg_data, zmq::recv_flags::none);

        double data = *(double *)msg_data.data();
        std::cout << i++ << " Received " << data << std::endl;
        data += 1;

        zmq::mutable_buffer msg_ptr_out = zmq::buffer(&data, sizeof(double));
        // // simulate work
        // std::this_thread::sleep_for(10s);

        // send the reply to the client
        socket.send(msg_ptr_out, zmq::send_flags::none);
    }

    return 0;
}

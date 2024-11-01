/* MyMEXFunction
 * c = MyMEXFunction(a,b);
 * Adds offset argument a to each element of FLOAT_TYPE array b and
 * returns the modified array c.
 */

#include "mex.hpp"
#include "mexAdapter.hpp"

#include <fmt/core.h>
#include <string>
#include <zmq.hpp>

using namespace matlab::data;
using matlab::mex::ArgumentList;

#define FLOAT_TYPE float

class MexFunction : public matlab::mex::Function {
 public:
  void operator()(ArgumentList outputs, ArgumentList inputs) {
    const auto p1 = std::chrono::system_clock::now();
    auto start = std::chrono::duration_cast<std::chrono::nanoseconds>(p1.time_since_epoch()).count();
    checkArguments(outputs, inputs);
    ArrayFactory factory;
    TypedArray<FLOAT_TYPE> matlab_array = std::move(inputs[1]);

    // initialize the zmq context with a single IO thread
    zmq::context_t context{1};

    // construct a REQ (request) socket and connect to interface
    zmq::socket_t socket{context, zmq::socket_type::req};

    std::string host = "localhost";
    int port = static_cast<int>(std::move(inputs[0])[0]);
    socket.connect(fmt::format("tcp://{}:{}", host, port));
    auto numelm = matlab_array.getNumberOfElements();

    auto msg = std::vector<FLOAT_TYPE>(matlab_array.begin(), matlab_array.end());
    zmq::mutable_buffer msg_data = zmq::buffer(msg, sizeof(FLOAT_TYPE) * numelm);
    socket.send(msg_data, zmq::send_flags::none);

    zmq::message_t reply{};
    auto out = socket.recv(reply, zmq::recv_flags::none);

    std::vector<FLOAT_TYPE> data(reply.size() / sizeof(FLOAT_TYPE));
    memcpy(data.data(), reply.data(), reply.size());

    const auto p2 = std::chrono::system_clock::now();
    auto end = std::chrono::duration_cast<std::chrono::nanoseconds>(p2.time_since_epoch()).count();
    auto result = (FLOAT_TYPE)(end - start);
    // std::cout << "roundtrip = " << end - start << std::endl;

    TypedArray<FLOAT_TYPE> matlab_result = factory.createArray<FLOAT_TYPE>({1, 2}, {(FLOAT_TYPE)numelm, result});

    outputs[0] = matlab_result;
  }

  void checkArguments(ArgumentList outputs, ArgumentList inputs) {
    // Get pointer to engine
    std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();
    // Get array factory
    ArrayFactory factory;
    // Check array argument: Second input must be double array
    if (inputs[1].getType() != ArrayType::DOUBLE || inputs[1].getType() == ArrayType::COMPLEX_DOUBLE) {
      matlabPtr->feval(u"error", 0, std::vector<Array>({factory.createScalar("Input must be Double array")}));
    }
    // Check number of outputs
    if (outputs.size() > 1) {
      matlabPtr->feval(u"error", 0, std::vector<Array>({factory.createScalar("Only one output is returned")}));
    }
  }
};

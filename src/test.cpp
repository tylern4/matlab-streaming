/* MyMEXFunction
 * c = MyMEXFunction(a,b);
 * Adds offset argument a to each element of double array b and
 * returns the modified array c.
 */

#include "mex.hpp"
#include "mexAdapter.hpp"

#include <string>
#include <zmq.hpp>
#include <fmt/core.h>

using namespace matlab::data;
using matlab::mex::ArgumentList;

class MexFunction : public matlab::mex::Function
{
public:
    void operator()(ArgumentList outputs, ArgumentList inputs)
    {
        checkArguments(outputs, inputs);
        TypedArray<double> doubleArray = std::move(inputs[0]);

        // initialize the zmq context with a single IO thread
        zmq::context_t context{1};

        // construct a REQ (request) socket and connect to interface
        zmq::socket_t socket{context, zmq::socket_type::req};
        socket.connect("tcp://localhost:5555");

        for (auto &elem : doubleArray)
        {
            // std::cout << "Loop " << elem << "\n";
            void *ptr = malloc(sizeof(double));
            ptr = &elem;
            zmq::mutable_buffer msg_data = zmq::buffer(ptr, sizeof(double));

            socket.send(msg_data, zmq::send_flags::none);
            zmq::message_t reply{};
            auto out = socket.recv(reply, zmq::recv_flags::none);
            elem = *(double *)(reply.data());
            // std::cout << "elem " << elem << "\n";
        }
        outputs[0] = doubleArray;
    }

    void checkArguments(ArgumentList outputs, ArgumentList inputs)
    {
        // Get pointer to engine
        std::shared_ptr<matlab::engine::MATLABEngine> matlabPtr = getEngine();

        // Get array factory
        ArrayFactory factory;

        // Check array argument: Second input must be double array
        if (inputs[0].getType() != ArrayType::DOUBLE ||
            inputs[0].getType() == ArrayType::COMPLEX_DOUBLE)
        {
            matlabPtr->feval(u"error",
                             0,
                             std::vector<Array>({factory.createScalar("Input must be double array")}));
        }
        // Check number of outputs
        if (outputs.size() > 1)
        {
            matlabPtr->feval(u"error",
                             0,
                             std::vector<Array>({factory.createScalar("Only one output is returned")}));
        }
    }
};

import argparse
import zmq
import numpy as np
import time

SIZE = 1701


def main():
    parser = argparse.ArgumentParser(description='ZeroMQ client')
    parser.add_argument('-p', '--port', type=int, default=5555, help='port number')
    parser.add_argument('-i', '--host', default='localhost', help='host name')
    parser.add_argument('-n', '--num', type=int, default=1000, help='number of requests')
    parser.add_argument('-d', '--debug', type=int, default=10, help='debug print number')
    args = parser.parse_args()

    print(f"tcp://{args.host}:{args.port}\n")

    context = zmq.Context()
    socket = context.socket(zmq.REQ)
    socket.connect(f"tcp://{args.host}:{args.port}")

    length = SIZE * SIZE
    for request_num in range(args.num):
        msg = np.ones(length, dtype=np.float32)

        start = time.time_ns()
        socket.send(msg, flags=0)

        reply = socket.recv(flags=0)
        data = np.frombuffer(reply, dtype=np.float32)[0]

        end = time.time_ns()
        print(f"{data},{end - start},{data / (end - start)}")


if __name__ == "__main__":
    main()

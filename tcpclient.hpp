#ifndef UD1_VIEWER_TCPCLIENT_HPP
#define UD1_VIEWER_TCPCLIENT_HPP

#include "format.hpp"

#include <thread>
#include <boost/asio.hpp>
#include "blockingqueue.hpp"

struct TcpClient {
    BlockingQueue<Obj> inMsgs;
    std::thread readTcpThread;
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;

    TcpClient() : socket(io_service) {}

    void sendCmd(std::__cxx11::string cmd);

    void run();

    void sendObj(const Obj &obj);

    ~TcpClient()
    {
        io_service.stop();
        readTcpThread.join();
    }
};

#endif //UD1_VIEWER_TCPCLIENT_HPP

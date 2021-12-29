#include "ZMQServer.h"
void ZMQServer::start()
{
    // initialize the zmq context with a single IO thread
    zmq::context_t context{ 1 };

    // construct a REP (reply) socket and bind to interface
    zmq::socket_t socket{ context, zmq::socket_type::rep };

    socket.bind( "tcp://*:5555" );

    zmq::message_t request;

    // receive a request from client
    socket.recv( request, zmq::recv_flags::none );
    std::cout << "Received " << request.to_string() << std::endl;

    // prepare some static data for responses
    const std::string data{ "World" };

    // simulate work
    std::this_thread::sleep_for( 1s );

    // send the reply to the client
    socket.send( zmq::buffer( data ), zmq::send_flags::none );

}

void ZMQServer::getData()
{

}

void ZMQServer::sendData()
{

}
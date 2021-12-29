#pragma once
#include <string>
#include <chrono>
#include <thread>
#include <iostream>

#include <zmq.hpp>

using namespace std::chrono_literals;

class ZMQServer
{



public:
	void start();
    void getData();
    void sendData();
};
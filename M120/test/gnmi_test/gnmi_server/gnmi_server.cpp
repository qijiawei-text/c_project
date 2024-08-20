/*************************************************************************************/
/* File: gnmi_client.cc
   /*
   /* Description: Implements client based on gNMI.proto
   /*               => Subscribe Request (to get streaming data) against user inputted prefix/path
   /*               => Async Get Request against user inputted prefix/path
   /*
   /* Copyright 2016 Ciena. All Rights Reserved.
   /*
   /* CONFIDENTIALITY AND LIMITED USE
   /*
   /* This software, including any software of third parties embodied herein,
   /* contains information and concepts which are confidential to Ciena
   /* and such third parties. This software is licensed for use
   /* solely in accordance with the terms and conditions of the applicable
   /* license agreement with Ciena or its authorized distributor.
   /*************************************************************************************/





#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <chrono>
#include <cmath>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <sys/types.h>

#include <google/protobuf/util/message_differencer.h>
#include <grpcpp/grpcpp.h>

#include <sysrepo-cpp/Sysrepo.hpp>
#include <sysrepo-cpp/Connection.hpp>
#include <sysrepo-cpp/Session.hpp>

#include "gnmi_server.h"



using google::protobuf::util::MessageDifferencer;
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerReaderWriter;
using grpc::StatusCode;



int
main(int argc, char **argv)
{

    ServerBuilder builder;

    gNMIDialOutSubscription gnmi("gnmi"); /* gNMI Service */

    builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());
    builder.RegisterService(&gnmi);
    std::unique_ptr<Server> server(builder.BuildAndStart());

    server->Wait();

    return 0;
}
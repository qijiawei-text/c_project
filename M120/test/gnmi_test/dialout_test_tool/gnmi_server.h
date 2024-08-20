/*
 * Copyright 2020 Yohan Pipereau
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _GNMI_CLIENT_H
#define _GNMI_CLIENT_H


#include <sysrepo-cpp/Struct.hpp>
#include <sysrepo-cpp/Sysrepo.hpp>
#include <sysrepo-cpp/Session.hpp>
#include "gnmi.pb.h"
#include "gnmi_dialout.grpc.pb.h"
#include "gnmi_dialout.pb.h"


using namespace grpc;
using namespace gnmi;
using sysrepo::Session;
using sysrepo::Connection;

using google::protobuf::RepeatedPtrField;
using grpc::ServerReaderWriter;
using std::make_shared;
using grpc::ServerContext;
using grpc::Status;
using grpc::StatusCode;

using gnmi::SubscribeResponse;
using gnmi_dialout::gNMIDialout;
using gnmi_dialout::PublishResponse;
using namespace std;





class gNMIDialOutSubscription final : public gnmi_dialout::gNMIDialout::Service
{
    Status
    Publish(ServerContext *context,
            ServerReaderWriter<PublishResponse, SubscribeResponse> *stream)
    {
        SubscribeResponse request;
        while (stream->Read(&request)) {
            PublishResponse response;

            cout << "get telemdata::" << endl;

            cout << request.DebugString() << endl;

            response.set_persistentsubscriptionname("molex-oplk");

            stream->Write(response);
        }
        return Status::OK;
    }
};


#endif /* _GNMI_CLIENT_H */

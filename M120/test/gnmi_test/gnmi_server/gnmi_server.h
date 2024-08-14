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
#include "gnmi.grpc.pb.h"
#include "gnmi.pb.h"
#include "subscribe.h"
#include "encode.h"

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





class gNMIDialOutSubscription : public gnmi::gNMI::Service
{
public:
    gNMIDialOutSubscription(std::string app)
    {
        try {
            std::cout << "------------";
            sr_con = make_shared<Connection>(app.c_str(), SR_CONN_DAEMON_REQUIRED);
            sr_sess = make_shared<Session>(sr_con, SR_DS_RUNNING, SR_SESS_DEFAULT, "gnmi");
            encodef = make_shared<Encode>(sr_sess);
        } catch (sysrepo::sysrepo_exception &exc) {
            std::cerr << "Connection to sysrepo failed " << exc.what() << std::endl;
            exit(1);
        }
    }

    ~gNMIDialOutSubscription()
    {
        std::cout << "Quitting GNMI DialOut Client" << std::endl;
    }

    Status
    Subscribe(ServerContext *context,
              ServerReaderWriter<SubscribeResponse, SubscribeRequest> *stream)
    {
        impl::Subscribe rpc(sr_sess, encodef);

        return rpc.run(context, stream);
    }


private:

    sysrepo::S_Connection sr_con; /* sysrepo connection */
    sysrepo::S_Session sr_sess; /* sysrepo session */
    shared_ptr<Encode> encodef; /* support for json ietf encoding */
};


#endif /* _GNMI_CLIENT_H */
//
// Created by root on 11/18/18.
//

#include "gen-cpp/Search.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/TToString.h>
#include <iostream>

using std::cout;
using std::endl;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::Thrift_Example;

class SearchHandler : virtual public SearchIf {
public:
    SearchHandler() {
        // Your initialization goes here
    }

    void searchName(Response& _return, const int32_t id) {
        // Your implementation goes here

        if(id < 1000){
            InvalidId invalidId;
            invalidId.id = id;
            invalidId.why = "is < 1000, invalid";

            cout<<"invalid id: "<<id<<endl;
            throw invalidId;
        }

        _return.name = "xiongyu: " + to_string(id);
//        _return.id = id;

        printf("search ret :");
        _return.printTo(std::cout);
        std::cout<<std::endl;
    }

};

int main(int argc, char **argv) {
    int port = 9090;
    stdcxx::shared_ptr<SearchHandler> handler(new SearchHandler());
    stdcxx::shared_ptr<TProcessor> processor(new SearchProcessor(handler));
    stdcxx::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
    stdcxx::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
    stdcxx::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

    TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
    server.serve();
    return 0;
}


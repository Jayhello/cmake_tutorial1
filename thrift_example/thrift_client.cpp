//
// Created by root on 11/18/18.
//

#include "gen-cpp/Search.h"

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/transport/TSocket.h>
#include <iostream>

using std::cout;
using std::endl;

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;

using namespace  ::Thrift_Example;

int main(){
    int port = 9090;
    stdcxx::shared_ptr<TTransport> socket(new TSocket("localhost", port));
    stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
    stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));

    SearchClient client(protocol);

    try{
        transport->open();

        int id = 1;
        Response response;
        try {
            client.searchName(response, 1);
        }catch (InvalidId& ii){
            cout<<"searchName InvalidId: "<<ii.what()<<endl;
//            transport->close();
//            return -1;
        }

        cout<<"search result: ";
        response.printTo(cout);

        transport->close();

    }catch (TException& tx) {
        cout << "ERROR: " << tx.what() << endl;
    }

}

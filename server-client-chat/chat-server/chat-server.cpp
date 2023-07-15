// chat-server.cpp : Defines the entry point for the application.
//

#include <thread>
#include "chat-server.h"
#include "rpc/server.h"
#include "rpc/this_session.h"
using namespace std;
using namespace std::chrono_literals;
using namespace std::this_thread;

//message data queue with a single queue per client
//the pair<string,bool>, consists of:
//string element for the message
//bool element for the message status
std::unordered_map<rpc::session_id_t, std::pair<string,bool>> client_data;

void exit_client() {
    // client requests to disconnect
    auto id = rpc::this_session().id();
    std::string exit_msg = "disconnected";
    // send exit message to close callback handler
    client_data[id].first = exit_msg;
    client_data[id].second = true;
    // wait a second
    sleep_for(1s);
    // delete the client from the list
    client_data.erase(id);
    std::cout << "client with id " << id << " disconnected." << std::endl;
    // post exit to the queue
    rpc::this_session().post_exit();
}

void announce(std::string const& value) {
    // a new client requested a connection
    auto id = rpc::this_session().id();
    // create welcom message
    std::string welcome_msg = value;
    // add client id to the welcome message
    welcome_msg = welcome_msg + " is:" + to_string(id);
    // queue in the welcome message
    client_data[id].first = welcome_msg;
    client_data[id].second = true;
    std::cout << "client with id " << id << " annouced." << std::endl;
}

int broadcast(std::string const& value) {
    auto id = rpc::this_session().id();
    // fill the clients message queue
    for (auto & cd : client_data)
    {
        if (cd.first != id) // to not send to the sender
        {
            cd.second.first = value;
            cd.second.second = true;
        }
    }
    return 0;
}

string msg_handler(void)
{
    std::string message;
    auto id = rpc::this_session().id();
    // wait until a message in the queue to send
    while (false == client_data[id].second)
    {

    }
    // send the message to the client
    message = client_data[id].first;
    client_data[id].second = false;
    return message;
}


int main() {
    // Create a server that listens TCP on default port 8080
    rpc::server srv("0.0.0.0", rpc::constants::DEFAULT_PORT);

    cout << "Server created" << endl;

    srv.bind("msg_callback", &msg_handler);

    srv.bind("exit", &exit_client);

    srv.bind("announce", &announce);

    srv.bind("broadcast", &broadcast);

    constexpr size_t thread_count = 12;

    // Run the server loop.
    srv.async_run(thread_count); // non-blocking call, handlers execute on one of the workers
    std::cin.ignore();

    return 0;
}
// chat-client.cpp : Defines the entry point for the application.
//

#include <thread>
#include <iostream>
#include "chat-client.h"
#include "rpc/client.h"

using namespace std;

class Service{
private:
    bool b_run = true;
    const std::string exit_cmd = "exit";
    bool new_outgoing_msg;
    
public:
    std::string message;
    Service() {
        new_outgoing_msg = false;
    }

    bool get_status() {
        return b_run;
    }

    void submit(std::string inputLine) {
        // check for exit_cmd
        int res = exit_cmd.compare(inputLine);
        if (0!=res)
        {
            message = inputLine;
            new_outgoing_msg = true;
        }
        else
        {
            // user request exit_cmd
            // service shut down
            b_run = false;
        }
    }

    bool new_message() {
        bool result = new_outgoing_msg;
        return result;
    }

    void message_sent() {
        new_outgoing_msg = false;
    }
};

static Service service;
static rpc::client client("127.0.0.1", rpc::constants::DEFAULT_PORT);

/*
* Task to receive the messages from the server.
*/
void task_receive()
{
    client.call("announce", "my_id");
    std::cout << "client connected" << std::endl;

    while (service.get_status())
    {
        auto a_future = client.async_call("msg_callback"); // non-blocking, returns std::future

        string result = a_future.get().as<string>(); // possibly blocks if the result is not yet available

        std::cout << result << std::endl;
    }
}

/*
* Task to send the messages to the server.
*/
void task_send()
{
    while (service.get_status())
    {
        if (true == service.new_message())
        {
            auto a_future = client.async_call("broadcast", service.message);

            int result = a_future.get().as<int>(); // possibly blocks if server busy
            // TBD: result variable could be used to resend message if server timed out
            // TBD: pass result to message_sent() to mark message to be resend.
            service.message_sent();
        }
    }
    client.call("exit");
}

int main() {
    // launch receive thread
    std::thread receive_thread(task_receive);
    // launch send thread
    std::thread send_thread(task_send);

    std::cout << "type 'exit' to terminate" << std::endl;

    std::string inputLine;

    //loop user input 
    while (service.get_status()) {
        // get keaboard line input
        std::getline(cin, inputLine);
        // submit the message
        service.submit(inputLine);
    }

    // Wait for the threads to finish
    receive_thread.join();
    send_thread.join();

    return 0;
}

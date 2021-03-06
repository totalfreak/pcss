//
// Created by rasmus on 10/27/17.
//

#include "Client.h"

Client::Client(GameManager * creator){
    //yea there really isent a lot happening here, creates the object
    game = creator;
}

void Client::talk() { // used to send information to the server
    informOfConnection();   // tell the server that you want to join it

    while (started) { // loop as long as the server s started
        if(hasMsgBeenSent == true){ //check if the mesage has been sent
            continue ;  // if at has start from the top of the loop
        }
        send(sock, msgToBeSent, 1024, 0)!= 0; // if the msg has not been sent, send it
        hasMsgBeenSent = true;  // then mark that it has been sent
        cout << "CLIENT: msg sent : " << msgToBeSent<< endl;
    }
}

void Client::listen() {     //look for returned information on your socket
    char res[1024];         //buffer that will hold the data
    int dataReceived;       //used to see how many bytes we receive
    while (started){
        dataReceived = recv(sock, res , 1024 , 0);//check if theres any new information on the socket

        if (dataReceived < 1){ continue;} // if there is none return to the top of loop

        // else we received new data
        cout << "CLIENT: i received data from server: " << res << endl;
        game->addToQueue(reinterpret_cast<char *>(&res), 1024);
        //listen logic goes here

    }

}

void Client::start() { // used to start the server, (kinda obivous,)


    //starting multithreading
    started = true; // server is now online, so allow threads to loop
    sendThread = thread([this]{ talk();});
    recvThread = thread([this]{ listen();});

    sendThread.join();
    recvThread.join();

}
void Client::sendMessage(char * msg, int size) {    // used to send a char array to the server
    if(size != 1024){   //make sure that you can only send the right size of buffer
        cout << "msg to large or to small use 1024" << endl;
        return;
    }
    for(int i = 0 ; i < 1024; i++) {    // copy the msg into the array that will se send
        *(msgToBeSent + i) = *(msg + i);
    }
    hasMsgBeenSent = false; // and tell the client that there is a new message that needs sending
}

void Client::roll(char* arr){
    sendMessage(arr, 1024);
}

void Client::informOfConnection() { // tells the server that you would like to join it
    char arr[1024];
    string myName = name;
    string info = "c:j:" + myName + ":";
    strcpy(arr, info.c_str());
    sendMessage(arr, 1024);
}

void Client::iWannaHost() {
    isHost = true;
}

bool Client::isStarted() const {
    return started;
}

void Client::changeName(string newName) {
    strcpy(name,newName.c_str());
}

void Client::tellThatIExist() {
    char arr[1024];
    string myName = name;
    string info = "c:u:" + myName + ":";
    strcpy(arr, info.c_str());
    sendMessage(arr, 1024);
}

void Client::configureClient() {


    string tempName = game->menu.getName().c_str();
    changeName(tempName);
    //Setting up socket and establishing connection
    const char *ip = "127.0.0.1"; // Server IP
    if(!isHost) {
        ip = getIpAddr();
    }else{
        cout <<"CLIENT: We are hosting now." << endl;
    }

    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        cout << "\n-Error establishing socket..." << endl;
        exit(-1);
    }


    cout << "\n- Socket client has been created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip, &server_addr.sin_addr);


    if (connect(sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
        cout << "- Connection to the server port number: " << port << endl;
    }

}

void Client::changePic(int selected) {
    if(selected == -1){
        selected = 0;
    }
    char arr[1024];
    string myPic = to_string(selected);
    string info = "c:p:" + myPic + ":";
    strcpy(arr, info.c_str());
    sendMessage(arr, 1024);

}

const char * Client::getIpAddr() {
    return ipAddr;
}

void Client::setIpAddr(const char *ip) {
    ipAddr = ip;
}

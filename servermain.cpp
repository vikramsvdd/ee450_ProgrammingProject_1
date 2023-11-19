#include <fstream>
#include <iostream>             // All the essential Libraries
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include<set>
#include<map>
#include<vector>
using namespace std;


// IMP NOTE: I have used Beej's Network Programming using C++, https://github.com/bozkurthan/Simple-TCP-Server-Client-CPP-Example as references

map<string,string> departmentToServerMap;   // a map to store a key value pair corresponding to a department and its associated backend server ID.

void loadlist(const std::string &filename){         // function to load the department data into the Main Server using ifstream
	ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(1);
    }
	
    string line;
    string serverId;
    vector<string> departments;

    while (getline(file, line)) {
        if (line.empty()) {
            continue; 
        }  
		
		
		// essentially storing the department and Server ID data as key value pairs in a C++ STL map

        if (line.find_first_not_of("0123456789") == string::npos) { 
            if (!serverId.empty()) {
                for (const string &department : departments) {
                    departmentToServerMap[department] = serverId;
                }
                serverId.clear();
                departments.clear();
            }
            serverId = line;
        } else {
            size_t pos = 0;
            while ((pos = line.find(';')) != string::npos) {
                departments.push_back(line.substr(0, pos));
                line.erase(0, pos + 1);
            }
            departments.push_back(line); 
    }

    if (!serverId.empty()) {
        for (const string &department : departments) {
            departmentToServerMap[department] = serverId;
        }
    }
	}

    file.close();  // closing the file once we are done with it
	
}

void Clientprocess(int clientSocket,int clientid) {                     // fReferences: Beej's network Programming, chatGPT
	int lclientid=clientid; // storing the client id  (1,2, 3 etc)
    char buffer[15000];  // maximum message size is 15,000 characters
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            close(clientSocket);
            cerr << "Client disconnected." << endl;   // if Client presses ctrl+c, a keyboard interrupt
            return;
        }
		
		//Now starts a sea of On-screen Messages 
		cout<<"Main Server has received the request on Department "<<buffer<<" from client "<<lclientid<<" using TCP over port 23074"<<endl;
  string departmentName(buffer);
       string duffer=buffer;
        string response;
		string response1;
      // If the Department name that client queried has a unique backend server id
        if (departmentToServerMap.find(departmentName) != departmentToServerMap.end()) {
			response="Client has received results from Main Server: "+duffer+" is associated with backend server "+departmentToServerMap[departmentName];
            response1 = "Department name shows up in backend server: " + departmentToServerMap[departmentName];
			cout<<response1<<endl;
			cout<<"Main Server has sent searching result to client "<<lclientid<<" using TCP over port 23074"<<endl;
        } 
		// if the dept name does not have a relevant backend server id associated with it
		else {
            response = buffer;
			response+=" not found";
			cout<<buffer<<" does not show up in backend server ";
			set<string> s;
			for(auto z=departmentToServerMap.begin();z!=departmentToServerMap.end();++z){
				s.insert(z->second);  // using set to print the unique elements or server id''s
			}
			for(auto z=s.begin();z!=s.end();++z){
				cout<< *z<<" ";
			}
			cout<<endl;
			cout<<"The Main Server has sent Department Name: Not Found  to client "<<lclientid<<" using TCP over port 23074"<<endl;

        }

        send(clientSocket, response.c_str(), response.length(), 0);    // Send the result to the appropriate Client
        
    }
}

int main() {            // References : Beej's Network Programming
	loadlist("list.txt");  // load the list onto the server
	int clientid=0; // client id, every incoming connection will have one
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);  // a TCP socket
    if (serverSocket < 0) {
        cerr << "Error establishing the server socket" << endl;  // if error happens in socket creation
        exit(1);
    }

    sockaddr_in servAddr;
    bzero((char*)&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
	//const char* hostname="localhost";
	//struct hostent* host = gethostbyname(hostname);   
    //servAddr.sin_addr.s_addr=gethostbyname(host);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // ip address of local host
    servAddr.sin_port = htons(23074); // port no


	 int bindStatus = bind(serverSocket, (struct sockaddr*) &servAddr,   //binding
        sizeof(servAddr));
    if(bindStatus < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }
	cout<<"Main Server is up and running"<<endl;  // happy news, main server has successfully started
	cout<<"Main Server has read the department list from list.txt"<<endl;
	 map<string, vector<string>> serverToDepartments;
	for (const auto &entry : departmentToServerMap) {
        const string &department = entry.first;
        const string &serverId = entry.second;
        serverToDepartments[serverId].push_back(department);
    }

    cout << "Total number of Backend Servers: " << serverToDepartments.size() << endl;

    for (const auto &entry : serverToDepartments) {
        const string &serverId = entry.first;
        const vector<string> &departments = entry.second;

        cout << "Backend Server " << serverId << " contains " << departments.size() << " distinct departments:" << std::endl;
        
    }

    while (true) {
		listen(serverSocket,3);           // Listen upto 3 connections simultaniously!
		    sockaddr_in newSockAddr;
    socklen_t newSockAddrSize = sizeof(newSockAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&newSockAddr, &newSockAddrSize);


        if (clientSocket < 0) {
            cerr << "Error accepting request from client!" << endl;  // if the connection is not successful
            exit(1);
        }
		clientid+=1;

        //cout << "Connected with a client! " << clientid<<" "<<endl;   , succesful connection
        int portno = newSockAddr.sin_port;
        int pid = fork();
		// start the process (multiprocessing stars from this snippet)

        if (pid == 0) {
            close(serverSocket);
            Clientprocess(clientSocket,clientid);
            exit(0);
        } else if (pid < 0) {
            cerr << "Error forking a child process." << endl; 
        }

        close(clientSocket);  // close the client socket once done
    }

   // server socket not closed as it should keep listening until you manualy kill the server program
    return 0;
}

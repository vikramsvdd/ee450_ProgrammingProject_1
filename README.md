1. Vikram Vasudevan

2. 6201-1130-74

3. Platform: Ubuntu 20.04.6

4. In this project, I have implemented a simple server client system using the concept of socket programming, where a client queries the backend server ID associated with a department to the server and the main server responds to that query if the department is included in the server's database, else notifies the client regarding the unavailability. Additionally, the server can handle atmost 3 connections (TCP). The Client/s can terminate the connection using keyboard interrupt.

5.servermain.cpp : It is the code that drives the server and implements the server functionality.

client.cpp: It is the code that drives the client and implements the client functionality, a client makes a query and the server responds to that query appropriately. 

Makefile: a file that compiles the  servermain.cpp and client.cpp files into their respective object files and converts them further into an executable file for each cpp file.

6. the message exchange format is as follows: (Here C stands for client, and S for server)

C: Enter Department Name
C: Client has sent <Department Name> to Main Server using TCP.
S: Main server has received the request on Department<Department Name> from client<client ID> using TCP over port 23074
(if dept not found) 
S:<Department Name> does not show up in backend server < Backend Server IDs > The Main Server has sent “Department Name: Not found” to client<client ID> using TCP over port 23074.
C: <Department Name> not found
C: (continues with new query)
(if dept found)
S:<Department Name> shows up in backend server <Backend Server ID>
Main Server has sent searching result to client <client ID> using TCP over port 23074
C: Client has received results from Main Server: <Department Name> is associated with backend server <Backend Server ID>.
C: (continues with a new query)

7. I have designed my system to handle atmost 15,000 characters (scalable). Also, I have designed the system to be case-sensitive wrt the handling of the department name (say if in the list.txt, there is a department ECE, but if the client inputs ece or eCE, it will fail to recognise that!). Also the server can accept atmost 3 connects, but we can easily scale up the system if we want

8. References:  https://github.com/bozkurthan/Simple-TCP-Server-Client-CPP-Example as primary references, and chatGPT to refine my code.

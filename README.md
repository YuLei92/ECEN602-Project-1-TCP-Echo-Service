# Network Programming Assignment 1

* This project is implemented by Yu Lei and Zeyu Wang.

* How to run:
    *  Enter "make -f Makefile" to compile the code.
    *  Use the command "./server <port.no>" to open the server.
    *  Open another Terminal and use the command "./client <IP.address> <port.no>" to create a client.
    *  (Optional) Using ./client1 and ./client2 to create another 2 clients. It's for the test of multiple connection.
    *  Input text in the server terminal, and will see changes both of them. 

* This program gives an example of basic socket communication. The server will start at local host with the specified port. Then the client will start with the server address and the port number. The client reads a line for its input and send to server. The server reads reads line from the network and echos back to the client. The client will reads the line from server and prints out. This application also supports multiple clients. When the client reads an EOF, it will close the socket and the server will exit afterwards.

* writen() – The function writen will write n bytes to a socket and return the number of bytes
* readline() – The readline function will read the characters from a socket and return the number of bytes of the characters.

* Group distribution: 
    * Zeyu Wang writes the server part. He also takes over the readline() and fork() function. 
    * Yulei writes the client part, writen() function, test and debug. Yulei also helped with the readsline() and fork() function to fulfill the Homework's requirement.

* Test Case:
    * Input line of text terminated by a newline. (Passed)
    * Line of text the maximum line length without a newline. In our works, the maximum line length is 10. (Passed)
    * Line with no characters and EOF. (Passed)
    * Client terminated after entering text. (Passed)
    * 3 Clients connected to the server. (Passed)
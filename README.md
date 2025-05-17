First, I establish a connection between the server and the client. Once connected, I provide authentication for user login to access the file server. The server recognizes the username and logs user actions. When clients disconnect, their IP shows as 0.0.0.0 instead of the real IP, resulting in a NULL return value if the operation fails.

Next, I implement the "list" command to display all files, including names and sizes. Users must log in to access this list. Entries are read from the directory, formatted, and sent to the client, with a full stop (.) indicating the end of the list.

The server handles the "GET" command by extracting the filename, attempting to open the file, and sending its contents line by line to the client, ending with "\r\n.\r\n". If the file isn't found, a "404 FILE can't be found" message is sent.

For the "PUT" command, the server attempts to open the file for writing. It receives data from the client and writes it until an end-of-transmission signal (".\n") is detected. Then, it closes the file and sends a confirmation message.

For the "DELETE" command, the server extracts the filename and attempts to delete the file. If successful, the file is removed; otherwise, an error message is displayed.

The "QUIT" command signals the server that the client has disconnected. When a client enters "QUIT," the server responds with "Goodbye" and logs the disconnection.

Thank you.

https://github.com/Shubebs/CMPE-431-pROJECT
This function checks if the username and password you typed match any line in the password.cfg file. If there's a match, it lets you in (returns 1), otherwise, access is denied (returns 0).
Second commit: I tried running the code but it was displaying an error, i will rectify the error and recommit.
Third commit: I have fixed the error i encountered and i have corrected it in this commit
Commit 4: The program is not behaving as intended, I will rectify the error and recommit.
Commit 5: Finally rectified the error, I used = 0 intead of == 0.
Commit 6: I Added basic string parsing for client commands
Commit 7: Implement USER command with authentication check and it ran well
Comit 8: I Add access control message for unauthenticated users and is working fine
Commit 9: Start LIST command handler with logging
Commit 10: Open current directory for file listing.#
Commit 11: Loop through directory and send file names with sizes
Commit 12: Finish directory listing with closing and end signal
Commit 13: Add logic to check and open requested file
Commit 14: Send file contents to client or return 404 if not found
Commit 15: Implement PUT command to receive and store file on server
Commit 16: I Added graceful disconnection using QUIT command
Commit 18: Handle unrecognized commands with error message
Commit 19: Add basic command-line argument parsing for directory and port
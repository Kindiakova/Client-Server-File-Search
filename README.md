## Client/Server application to search file on remote HOST.
- client has functionality to search file by server request.
- server will request search by "file name"
- client has to replay with "processing" each 500ms of searching
- client has to reply with success or failure.
- success replay has to include full path to the file.


all headers in the headers/*.h

main files: server.cpp client.cpp

сlass "Connection" for working with Windows and Linux sockets: WinConnect and LinuxConnect in accordance
 all direct socket interaction hiden in Connection class

code to search for files in the file system starting from the current folder
	: WinFileSearch and LinuxFileSearch (depending on os)
	
	
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <netdb.h>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include <iostream>
#include <sstream>
#include <list>
#include <ctime>
#include <fcntl.h>
#include "server.h"

using namespace std;

int main(int argc, char** argv){
  Server server;
  while(true){
    server.loop();
  }
  return 0;
}

void Server::setup(){
  sockFd = socket(AF_INET, SOCK_STREAM, 0);
  fcntl(sockFd, F_SETFL, O_NONBLOCK);
  bzero((char*)&serverAddress, SOCKADDR_LEN);
  serverAddress.sin_family      = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
  serverAddress.sin_port        = htons(PORT_NO);
  bind(sockFd, (struct sockaddr*)&serverAddress, SOCKADDR_LEN);
  listen(sockFd, 5);
}

void Server::loop(){
  unsigned int clientLen = SOCKADDR_LEN;
  int fd = accept(sockFd, (struct sockaddr*)&clientAddress, &clientLen);
  if(fd > -1){
    fcntl(fd, F_SETFL, O_NONBLOCK);
    string response = parseRequest(receiveRequest(fd));
    write(fd, response.c_str(), response.size());
    close(fd);
  }
  sleep(1);
}

string Server::receiveRequest(int fd){
  char* buf = (char*)calloc(BUF_LEN+1, sizeof(char));
  int pos   = 0;
  int n     = 0;
  while((n = read(fd, buf+pos, BUF_LEN)) > 0){
    pos += n;
    if(n == BUF_LEN){//there could be more...
      buf = (char*)realloc(buf, pos+BUF_LEN+1);
    }
  }
  string request = (const char*)buf;
  free(buf);
  return request;
}

string Server::parseRequest(string request){
  string requestPart = string();
  int n = request.find("\r\n");
  if(n > 0){
    requestPart = request.substr(0, n);
    request.erase(0, n+2);
    cout << "request: " << requestPart << endl;
  }
  n = request.find("\r\n\r\n");
  if(n <= 0){
    cout << "couldn't find end of headers" << endl;
  }
  string headerPart = request.substr(0, n);
  request.erase(0, n+4);
  istringstream stream (headerPart);
  list<string> headers;
  while(!stream.eof()){
    string t;
    getline(stream, t);
    headers.push_back(t);
  }
  string bodyPart = request;
  for(string header : headers){
    cout << "header: " << header << endl;
  }
  if(!bodyPart.empty()){
    cout << "body(" << bodyPart.size() << "): " << bodyPart << endl;
  }
  n = 0;
  return "Content-type: text/plain\r\n\r\n";
}

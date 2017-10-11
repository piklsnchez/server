#ifndef SERVER_H
#define SERVER_H
class Server{
  private:
  const int BUF_LEN = 1000;
  const int PORT_NO = 8888;
  const size_t SOCKADDR_LEN = sizeof(struct sockaddr_in);
  struct sockaddr_in serverAddress;
  struct sockaddr_in clientAddress;
  int sockFd;
  public:
  Server(){
    setup();
  }
  ~Server(){close(sockFd);}
  void setup();
  void loop();
  std::string receiveRequest(int);
  std::string parseRequest(std::string);
};
#endif
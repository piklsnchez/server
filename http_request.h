#ifndef HTTP_REQUEST_H
#define HTTP_REQUEST_H
class HttpRequest{
  private:
  string method;
  string resource;
  double version;
  std::list<string> headers;
  string body;
  public:
  HttpRequest(){}
  ~HttpRequest(){}
  string getMethod(){return method;}
  void setMethod(string m): method(m){}
  string getResource(){return resource;}
  void setResource(string r): resource(r){}
  double getVersion(){return version;}
  void setVersion(double v): version(v){}
  std::list<string> getHeaders(){return headers;}
  void setHeaders(std::list<string> h): headers(h){}
}
#endif
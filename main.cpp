#include <iostream>
#include <string>
#include "native/native.h"
#include "qrcode_generator.h"

using namespace native::http;

int main() {

  http server;

  #if 0
  // benchmark
  // 3 /ms on my machine
  {
    for (int i = 0; i < 1000000; ++i)
    {
      QRCodeGenerator qr(version, mode, eclevel, masktype);
      qr.addData("ud87v9w8vw9vw98ev9whvrh4jb76jhv8h7gfjh3gv5k43j65jh4f67u5gbiudfhd7yb6g74yg4f6ge8y56");
      qr.toPNG(sep, mag);
    }

    return 0;
  }
  #endif


  if(!server.listen("0.0.0.0", 8080, [=](request& req, response& res) {

    try
    {
      // Version 5 has 37x37 modules.
      int version = 5;
      // 15% destroyed data can be corrected.
      int eclevel = QR_ECL_M;

      // max 688 bits on version=5, ecl=M     

      int mode = QR_EM_8BIT; //QR_EM_AUTO;  

      int masktype = -1; //auto
      //int errcode = QR_ERR_NONE;

      //min sep
      int sep = 4;
      //max mag
      int mag = 16;

      std::string path(req.url().path());
      std::string content(req.url().query());

      if(content.empty() && path == "/favicon.ico")
      {
        content = "libqr favicon.ico";
      }

      std::cout << "GET " << path << std::endl;

      QRCodeGenerator qr(version, mode, eclevel, masktype);

      std::cout << "generate QRCode from: " << content << std::endl;
      
      qr.addData(content);

      res.set_status(200);
      res.set_header("Content-Type", "image/png");

      res.end(qr.toPNG(sep, mag));
    }
    catch(std::exception & ex)
    {
      res.set_status(200);
      res.set_header("Content-Type", "text/plain");
      res.end(ex.what());
    }

  })) return 1; // Failed to run server.

  std::cout << "Server running at http://0.0.0.0:8080/" << std::endl;
  return native::run();
}

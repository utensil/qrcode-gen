#include <exception>
#include <string>
#include "qr.h"
#include "qr_util.h"

/**
  QR Code Generator  

  A lightweight C++ wrapper for libqr.

  @author utensil

  @note It only encapsulate the functions used, so it's not complete.
*/
class QRCodeGenerator
{
  public:

    /**
      Constructor.

      @param version  Symbol version (1-40)                       
                      @see http://www.qrcode.com/en/about/version.html

      @param mode     Encoding mode (N,A,8,K,S)
                      N: numeric, A: uppercase alphabet and numeric
                      8: 8-bit byte, K: JIS X 0208 Kanji, S: auto
                      @see qr_em_t

      @param eclevel  Error correction level (L,M,Q,H)
                      @see qr_ecl_t
                      @see http://qrserver.com/api/documentation/create-qr-code/

      @param masktype Mask pattern (0-7, -1: auto)

    */
    QRCodeGenerator(int version, int mode, int eclevel, int masktype)
    {
      int errcode = QR_ERR_NONE;

      m_qr = qrInit(version, mode, eclevel, masktype, &errcode);

      if(NULL == m_qr)
      {
        throw Exception(qrStrError(errcode));
      }
    }

    /**
        Add data to QR Code.

        @param source A binary buffer

        It's OK to store binary data in std::string

        @note You may not call addData after ant to* calls.
    */
    void addData(const std::string & source)
    {
      if(!qrAddData(m_qr, (const qr_byte_t *)source.data(), source.size()))
      {
        throw Exception(qrGetErrorInfo(m_qr));
      }
    }

    /**
        Add data to QR Code, and set corresponding encoding mode.

        @param source   A binary buffer

        @param mode     Encoding mode (N,A,8,K,S)
                        N: numeric, A: uppercase alphabet and numeric
                        8: 8-bit byte, K: JIS X 0208 Kanji, S: auto
                        @see qr_em_t        

        It's OK to store binary data in std::string

        @note You may not call addData after ant to* calls.
    */
    void addData(const std::string & source, int mode)
    {
      if(!qrAddData2(m_qr, (const qr_byte_t *)source.data(), source.size(), mode))
      {
        throw Exception(qrGetErrorInfo(m_qr));
      }
    }

    /**
      Convert to PNG image

      @param sep        Separator pattan width (4-16)
      @param mag        Magnifying ratio (1-16)

      @return A binary buffer keeping the PNG image
    */
    std::string toPNG(int sep, int mag)
    {
      ensureFinalized();

      size_t size = 0;

      int int_size = 0;

      qr_byte_t * buffer = qrSymbolToPNG(m_qr, sep, mag, &int_size);

      size += int_size;

      if(NULL == buffer)
      {
        throw Exception(qrGetErrorInfo(m_qr));
      }

      std::string binary((const char*)buffer, size);

      qrFree(buffer);

      return binary;
    }

    /**
      Destructor.
    */
    virtual ~QRCodeGenerator()
    {
      qrDestroy(m_qr);
    }

    /**
        Exception of QRCodeGenerator
    */
    class Exception : public std::exception
    {
      public:
        Exception(const char * error_info) throw()
        : m_error_info(error_info)
        {}

        Exception(const std::string & error_info) throw()
        : m_error_info(error_info)
        {}

        virtual const char* what() const throw()
        {
          return m_error_info.c_str();
        }

        virtual ~Exception() throw()
        {}

      private:
        std::string m_error_info;
    };

  private:

    void ensureFinalized()
    {
      if(!qrHasData(m_qr))
      {
        throw Exception("No data added");
      }

      if(!qrIsFinalized(m_qr))
      {
        if(!qrFinalize(m_qr))
        {
          throw Exception(qrGetErrorInfo(m_qr));
        }
      }
    }

    QRCode * m_qr;
};
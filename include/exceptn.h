

#include <exception>
#include <string>

#ifndef __exceptn__H_
#define __exceptn__H_




class test_exceptn_t : public std::exception
{
public:

  /**
   * Get the message of the exception.
   */
  const char * what() const throw() {return msg.c_str();}

  test_exceptn_t(const std::string& m = "Unknown error"){set_msg(m);}

  virtual ~test_exceptn_t() throw() { }

protected:
  void set_msg(const std::string& m){msg = "test: " + m;}

private:
  std::string msg;
};

#endif /* h-guard */

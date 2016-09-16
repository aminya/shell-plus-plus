#ifndef SETTI_MSG_H
#define SETTI_MSG_H

#include <string>
#include <memory>
#include <vector>

namespace setti {
namespace internal {

class Message {
 public:
  enum class Severity {
    INFO,
    WARNING,
    ERR
  };

  Message(Severity severity, std::string msg, uint line, uint pos)
      : severity_(severity)
      , msg_(msg)
      , line_(line)
      , pos_(pos) {}

 private:
  uint line_;
  uint pos_;
  std::string msg_;
  Severity severity_;
};

class Messages {
 public:
  Messages() = default;

  void Push(Message&& msg) {
    msg_vec_.push_back(std::move(msg));
  }

 private:
  std::vector<Message> msg_vec_;
};

}
}

#endif  // SETTI_MSG_H


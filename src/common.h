#ifndef COMMON_H
#define COMMON_H

class MmError {
public:
MmError (std::string msg) {
    this->message = msg;
}

std::string message;
};

#endif /* COMMON_H */

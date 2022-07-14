#pragma once

#include <exception>

namespace clapp {
    class ParseException : public std::exception {
        public:
            ParseException(const std::string& message) : msg(message) {}
            const char* what() const throw() override {
                return msg.c_str();
            }
        private:
            std::string msg;
    };
};
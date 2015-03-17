#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "errors.hpp"
#include <string>

class config
{
    public:
        config();
        virtual ~config();
        std::string adapter = "";
        std::string ipaddress = "";
        std::string port = "";
        std::string ipaddressmask = "";
        int result = NO_ERROR;
    protected:
    private:


};

#endif // CONFIG_HPP

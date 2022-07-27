/*
 * HostPortUDP class to handle serial communication - source
 */

#include "HostPortUDP.h"

//Begins
bool HostPortUDP::begin(const std::string& ipaddr, unsigned short port) {
    _terminator = TERMINATOR;
    _header = HEADER;
    _timeout = TIMEOUT;
    _port = port;
    _ip = ipaddr;
    if (init(ipaddr, port, _timeout)) {
        _isFirstRead = true;
        return true;
    }
    return false;
}

bool HostPortUDP::begin(const std::string& ipaddr, unsigned short port, unsigned int header, unsigned int terminator) {
    _terminator = terminator;
    _header = header;
    _timeout = TIMEOUT;
    _port = port;
    _ip = ipaddr;
    if (init(ipaddr, port, _timeout)) {
        _isFirstRead = true;
        return true;
    }
    return false;
}

bool HostPortUDP::begin(const std::string& ipaddr, unsigned short port, unsigned int header, unsigned int terminator, unsigned int timeout) {
    _terminator = terminator;
    _header = header;
    _port = port;
    _ip = ipaddr;
    _timeout = timeout;
    if (init(ipaddr, port, _timeout)) {
        _isFirstRead = true;
        return true;
    }
    return false;
}

HostPortUDP::~HostPortUDP() {
    client.close();
}

HostPortUDP::HostPortUDP() {
    //empty
}

//set/get funs
bool HostPortUDP::isInit(void) {
    return (bool)client;
}
bool HostPortUDP::setPort(unsigned short port) {
    //if (!serial) {
        _port = port;
        return true;
    //}
    //return false;
}
bool HostPortUDP::setIP(const std::string& ipaddr) {
   // if (!serial) {
        _ip = ipaddr;
        return true;
    //}
    //return false;
}
bool HostPortUDP::setHeader(unsigned int header) {
    //if (!serial) {
        _header = header;
        return true;
    //}
    //return false;
}
bool HostPortUDP::setTerminator(unsigned int terminator) {
    //if (!serial) {
        _terminator = terminator;
        return true;
    //}
    //return false;
}
bool HostPortUDP::setTimeout(unsigned int timeout) {
    //if (!serial) {
        _timeout = timeout;
        return true;
    //}
    //return false;
}
unsigned short HostPortUDP::getPort(void) {
    return _port;
}
std::string HostPortUDP::getIP(void) {
    return _ip;
}
unsigned int HostPortUDP::getHeader(void) {
    return _header;
}
unsigned int HostPortUDP::getTerminator(void) {
    return _terminator;
}
unsigned int HostPortUDP::getTimeout(void) {
    return _timeout;
}

//reset & close fun
bool HostPortUDP::close(void) {
    return client.close();
}
bool HostPortUDP::restart(void) {
    client.close();
    return init(_ip, _port, _timeout);
}

bool HostPortUDP::flush(void) {
    if (isInit()) {
        return restart();
    }
    return false;
}

//write
bool HostPortUDP::write(unsigned char* packetPtr, unsigned int size) {

    if (!client) {
        return false;
    }
    
    /*if (size > TX_BUF_SIZE) {
        return false;
    }*/

    size_t c = 0; //counter for sent size

    unsigned char* _tx_buf = (unsigned char*) malloc(size+8); //transmit buffer

    //put header
    _tx_buf[c++] = _header & MASK;
    _tx_buf[c++] = (_header >> 8) & MASK;
    _tx_buf[c++] = (_header >> 16) & MASK;
    _tx_buf[c++] = (_header >> 24) & MASK;

    //add data
    for (int i = 0; i < size; ++i) _tx_buf[c++] = *(packetPtr + i);
    
    //put terminator
    _tx_buf[c++] = _terminator & MASK;
    _tx_buf[c++] = (_terminator >> 8) & MASK;
    _tx_buf[c++] = (_terminator >> 16) & MASK;
    _tx_buf[c++] = (_terminator >> 24) & MASK;

    //put start bytes in buf
    client.send(_tx_buf, c);

    //free
    free(_tx_buf);

    return true;

}

//read
bool HostPortUDP::read(unsigned char* packetPtr, unsigned int size) {

    if (!client) {
        return false;
    }

    if (_isFirstRead) {
        _isFirstRead = false;
    }

    unsigned int c = 0;
    unsigned char b[MAX_PACKET_SIZE];
    size_t len = client.recv(b, MAX_PACKET_SIZE);

    //parse the packet
    if (len < (size + 4 + 4)) {
        return false;
    }
    while (1) {
        if (c < 4) { //check header
            if (b[c] == ((_header >> (8 * c)) & MASK)) {
                c++;
                continue;
            } else { break; }
        } else if (c==4) { //read data
            memcpy(packetPtr,&b[c], size);
            c++;
            continue;
        } else {
            if (b[c-1+size] == ((_terminator >> (8 * (c - 5))) & MASK)) {
                c++;
                if (c == 9) { //all ok
                    return true;
                }
                continue;
            } else { 
                break;
            }
        }
        break;
    }
    return false; //is ok???
}

//init
bool HostPortUDP::init(const std::string& ipaddr, unsigned short port, unsigned int timeout) { 
    if (client.set_option(SOL_SOCKET,SO_BROADCAST,&_enable,sizeof(_enable)) == -1) {
        return false;
    }
    client.bind({ipaddr, port});
    return (bool) client;
}


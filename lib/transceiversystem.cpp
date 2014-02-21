#include <iostream>
#include <thread>
#include "../beaglebone/crc8.h"
#include "transceiversystem.h"
using namespace std;

static char getHexOfNibble(char c)
{
    c = c & 0x0f;
    if (c < 10) return '0' + c;
    return 'a' + c - 10;
}

static string getChecksum(string tag, string data)
{
    unsigned char checksum = crc8(tag.c_str(), 0);
    checksum = crc8(data.c_str(), checksum);

    char hex1 = getHexOfNibble(checksum >> 4);
    char hex2 = getHexOfNibble(checksum);

    string hex = "";
    hex += hex1;
    hex += hex2;

    return hex;
}

TransceiverSystem::TransceiverSystem(Actor *parent, string name, std::unique_ptr<TTYDevice> tty) :
    Subsystem(parent, name),
    tty(move(tty))
{
    on<TransceiverEvent>([&] (TransceiverEvent *message) {
        *this->tty << "Sending: "
                   << message->tag.c_str() << "^"
                   << message->data.c_str() << ":"
                   << getChecksum(message->tag, message->data) << "\n";
    });
}

enum ParseResponse {
    Failure,
    Continue,
    Success
};

static ParseResponse parseTag(string &input, string &tag, string &data) {
    string::size_type tagEnd = input.find_first_of("^");
    string::size_type dataEnd = input.find_first_of(":");
    if(tagEnd != input.find_last_of("^") || dataEnd != input.find_last_of(":") || tagEnd > dataEnd)
        return Failure;
    else if(tagEnd == string::npos || dataEnd != input.size()-3)
        return Continue;

    string checksum = input.substr(input.size()-2);
    tag = input.substr(0,tagEnd);
    data = input.substr(tagEnd+1,dataEnd-tagEnd-1);

    if(!tag.size() || !data.size() || getChecksum(tag, data) != checksum)
        return Failure;

    return Success;
}

void TransceiverSystem::loop()
{
    string tag, data;
    int32_t byte = this->tty->readByte();
    if(byte != -1) {
        buffer += (char) byte;
        ParseResponse status = parseTag(buffer, tag, data);
        if(status == Success) {
            cout << "Found tag? " << tag    << "\t" << data << endl;
            buffer = "";
        } else if(status == Failure) {
            buffer = "";
        }
    }
    this_thread::sleep_for(chrono::microseconds(10));
}

#include <iostream>
#include <fstream>
#include <string>

bool readPacket(const std::string&, uint64_t&, uint16_t&, uint16_t&, uint64_t&);

// A bit hacky to use global variable. But feel it's well suited in this case, since I don't wanna change
// the function signatures just to sum the version numbers.
uint64_t G_VERSION_SUM = 0;

std::string readInput(std::ifstream& is)
{
    std::string line;
    std::string bits;

    is >> line;
    for ( const char& v : line )
    {
        switch (v)
        {
            case '0':
                bits.append("0000");
                break;
            case '1':
                bits.append("0001");
                break;
            case '2':
                bits.append("0010");
                break;
            case '3':
                bits.append("0011");
                break;
            case '4':
                bits.append("0100");
                break;
            case '5':
                bits.append("0101");
                break;
            case '6':
                bits.append("0110");
                break;
            case '7':
                bits.append("0111");
                break;
            case '8':
                bits.append("1000");
                break;
            case '9':
                bits.append("1001");
                break;
            case 'A':
                bits.append("1010");
                break;
            case 'B':
                bits.append("1011");
                break;
            case 'C':
                bits.append("1100");
                break;
            case 'D':
                bits.append("1101");
                break;
            case 'E':
                bits.append("1110");
                break;
            case 'F':
                bits.append("1111");
                break;
            default:
                std::cerr << "Got an unexpected value when parsing input stream.";
        }
    }

    return bits;
}

void readLiteralPacket(const std::string& bitstream, uint64_t& msgPtr, uint64_t& packetValue)
{
    std::string readBuf;
    // Read/Compute packet value.
    bool lastGroup = false;
    std::string literal;
    uint32_t size = 0;
    while ( !lastGroup )
    {
        if ( bitstream.at(msgPtr) == '0' )
            lastGroup = true;
        msgPtr++;

        readBuf.append(bitstream.substr(msgPtr, 4));
        literal.append(readBuf);
        size += 4;
        msgPtr += 4;
    }
    packetValue = std::stol(readBuf, 0, 2);
}

void readOperatorPacket(const std::string& bitstream, uint64_t& msgPtr, uint64_t& packetValue)
{
    std::string readBuf;

    char lengthTypeId = bitstream.at(msgPtr);
    msgPtr++;

    if ( lengthTypeId == '0' )
    {
        int packetNBits = std::stol(bitstream.substr(msgPtr, 15), 0, 2);
        msgPtr += 15;

        uint64_t origMsgPtr = msgPtr;
        while ( msgPtr - origMsgPtr < packetNBits )
        {
            // Part 2 will probably do something more with these values.
            uint16_t _version, _typeId;
            uint64_t _value;
            readPacket(bitstream, msgPtr, _version, _typeId, _value);
        }
   } else  // lengthTypeId == '1'
   {
        int nOfPackets = std::stol(bitstream.substr(msgPtr, 11), 0, 2);
        msgPtr += 11;

        for ( int i = 0; i < nOfPackets; i++ )
        {
            // Part 2 will probably do something more with these values.
            uint16_t _version, _typeId;
            uint64_t _value;
            readPacket(bitstream, msgPtr, _version, _typeId, _value);
        }
   }
}

bool readPacket(const std::string& bitstream, uint64_t& msgPtr, uint16_t& outVersion, uint16_t& outTypeId, 
    uint64_t& outValue)
{
    bool endOfMessage = false;
    std::string readBuf;
    
    // Reading packet's version.
    readBuf = bitstream.substr(msgPtr, 3);
    outVersion = std::stol(readBuf, 0, 2);
    msgPtr += 3;

    G_VERSION_SUM += outVersion;

    // Read packet type.
    readBuf = bitstream.substr(msgPtr, 3);
    outTypeId = std::stol(readBuf, 0, 2);
    msgPtr += 3;

    // Read packets.
    if ( outTypeId == 4 )
        readLiteralPacket(bitstream, msgPtr, outValue);
    else
        readOperatorPacket(bitstream, msgPtr, outValue);

    return endOfMessage;
}

int main(int argc, char* argv[])
{
    std::ifstream is(argv[1]);
    if ( !is.is_open() )
    {
        std::cerr << "Failed to open file." << std::endl;
        return 1;
    }
    std::string bitstream = readInput(is);

    uint64_t versionSum = 0;
    uint64_t msgPtr = 0;

    uint16_t packetVersion, packetTypeId;
    uint64_t packetValue;
    readPacket(bitstream, msgPtr, packetVersion, packetTypeId, packetValue);

    std::cout << "Version: " << packetVersion << " | Type: " << packetTypeId << " | Value: " << packetValue << std::endl;

    std::cout << "Version sum: " << G_VERSION_SUM << std::endl;

    return 0;
}

#include "MifareClassic.h"
#ifdef NDEF_SUPPORT_MIFARE_CLASSIC

#define BLOCK_SIZE 16
#define LONG_TLV_SIZE 4
#define SHORT_TLV_SIZE 2

#define MIFARE_CLASSIC_DEBUG

#define MIFARE_CLASSIC ("Mifare Classic")

MifareClassic::MifareClassic()
{
}

MifareClassic::~MifareClassic()
{
}

int MifareClassic::getBufferSize(int messageLength)
{

    int bufferSize = messageLength;

    // TLV header is 2 or 4 bytes, TLV terminator is 1 byte.
    if (messageLength < 0xFF)
    {
        bufferSize += SHORT_TLV_SIZE + 1;
    }
    else
    {
        bufferSize += LONG_TLV_SIZE + 1;
    }

    // bufferSize needs to be a multiple of BLOCK_SIZE
    if (bufferSize % BLOCK_SIZE != 0)
    {
        bufferSize = ((bufferSize / BLOCK_SIZE) + 1) * BLOCK_SIZE;
    }

    return bufferSize;
}

// skip null tlvs (0x0) before the real message
// technically unlimited null tlvs, but we assume
// T & L of TLV in the first block we read
int MifareClassic::getNdefStartIndex(byte *data)
{

    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        if (data[i] == 0x0)
        {
            // do nothing, skip
        }
        else if (data[i] == 0x3)
        {
            return i;
        }
        else
        {
#ifdef NDEF_USE_SERIAL
            Serial.print("Unknown TLV ");Serial.println(data[i], HEX);
#endif
            return -2;
        }
    }

    return -1;
}

// Decode the NDEF data length from the Mifare TLV
// Leading null TLVs (0x0) are skipped
// Assuming T & L of TLV will be in the first block
// messageLength and messageStartIndex written to the parameters
// success or failure status is returned
//
// { 0x3, LENGTH }
// { 0x3, 0xFF, LENGTH, LENGTH }
bool MifareClassic::decodeTlv(byte *data, int &messageLength, int &messageStartIndex)
{
    int i = getNdefStartIndex(data);

    if (i < 0 || data[i] != 0x3)
    {
#ifdef NDEF_USE_SERIAL
        Serial.println(F("Error. Can't decode message length."));
#endif
        return false;
    }
    else
    {
        if (data[i+1] == 0xFF)
        {
            messageLength = ((0xFF & data[i+2]) << 8) | (0xFF & data[i+3]);
            messageStartIndex = i + LONG_TLV_SIZE;
        }
        else
        {
            messageLength = data[i+1];
            messageStartIndex = i + SHORT_TLV_SIZE;
        }
    }

    return true;
}

NfcTag MifareClassic::read(byte* data, byte *uid, unsigned int uidLength)
{
    int messageStartIndex = 0;
    int messageLength = 0;
   
    if (!decodeTlv(data, messageLength, messageStartIndex)) {
        return NfcTag(uid, uidLength, "ERROR"); // TODO should the error message go in NfcTag?
    }

    // this should be nested in the message length loop
    // int index = 0;

    #ifdef MIFARE_CLASSIC_DEBUG
    Serial.print(F("Message Length "));Serial.println(messageLength);
    #endif

    return NfcTag(uid, uidLength, MIFARE_CLASSIC, &data[messageStartIndex], messageLength);
}
#endif
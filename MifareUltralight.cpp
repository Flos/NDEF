#include <MifareUltralight.h>

#define NFC_FORUM_TAG_TYPE_2 ("NFC Forum Type 2")

MifareUltralight::MifareUltralight()
{
    ndefStartIndex = 0;
    messageLength = 0;
}

MifareUltralight::~MifareUltralight()
{
}

NfcTag MifareUltralight::read(byte *data, byte *uid, unsigned int uidLength)
{
    findNdefMessage(data);
    NdefMessage ndefMessage = NdefMessage(&data[ndefStartIndex], messageLength);
    return NfcTag(uid, uidLength, NFC_FORUM_TAG_TYPE_2, ndefMessage);
}

// read enough of the message to find the ndef message length
void MifareUltralight::findNdefMessage(byte *data)
{
    
    if (data[0] == 0x03)
    {
        messageLength = data[1];
        ndefStartIndex = 2;
    }
    else if (data[5] == 0x3) // page 5 byte 1
    {
        // TODO should really read the lock control TLV to ensure byte[5] is correct
        messageLength = data[6];
        ndefStartIndex = 7;
    }
    

    #ifdef MIFARE_ULTRALIGHT_DEBUG
    Serial.print(F("messageLength "));Serial.println(messageLength);
    Serial.print(F("ndefStartIndex "));Serial.println(ndefStartIndex);
    #endif
}
// buffer is larger than the message, need to handle some data before and after
// message and need to ensure we read full pages
void MifareUltralight::calculateBufferSize()
{
    // TLV terminator 0xFE is 1 byte
    bufferSize = messageLength + ndefStartIndex + 1;

    // if (bufferSize % ULTRALIGHT_READ_SIZE != 0)
    // {
    //     // buffer must be an increment of page size
    //     bufferSize = ((bufferSize / ULTRALIGHT_READ_SIZE) + 1) * ULTRALIGHT_READ_SIZE;
    // }
}

#ifndef MifareUltralight_h
#define MifareUltralight_h

#include <NfcTag.h>
#include <Ndef.h>
#include <NdefMessage.h>

class MifareUltralight
{
    public:
        MifareUltralight();
        ~MifareUltralight();
        NfcTag read(byte *data, byte *uid, unsigned int uidLength);
    private:
        unsigned int tagCapacity;
        unsigned int messageLength;
        unsigned int bufferSize;
        unsigned int ndefStartIndex;
        void findNdefMessage(byte *data);
        void calculateBufferSize();
};

#endif

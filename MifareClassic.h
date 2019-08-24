#ifndef MifareClassic_h
#define MifareClassic_h

// Comment out next line to remove Mifare Classic and save memory
#define NDEF_SUPPORT_MIFARE_CLASSIC

#ifdef NDEF_SUPPORT_MIFARE_CLASSIC

#include <NdefMessage.h>
#include <Ndef.h>
#include <NfcTag.h>

class MifareClassic
{
    public:
        MifareClassic();
        ~MifareClassic();
        NfcTag read(byte *data, byte *uid, unsigned int uidLength);
    private:
        int getBufferSize(int messageLength);
        int getNdefStartIndex(byte *data);
        bool decodeTlv(byte *data, int &messageLength, int &messageStartIndex);
};

#endif
#endif

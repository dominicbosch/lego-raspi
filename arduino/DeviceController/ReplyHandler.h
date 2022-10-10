/*
    ReplyHandler.h - Library to handle replies to the master
*/

#ifndef ReplyHandler_h
#define ReplyHandler_h

#include <Arduino.h>

class ReplyHandler {
    public:
        ReplyHandler(int replyLength);
        char* getReply();
        void setConfirmReply(int arrBytes[], int byteCount);
        void setErrorReply(int errCode);
        void setErrorReply(int errCode, int arrMoreInfo[], int byteCount);
        
    private:
        int _replyLength;
        char* _arrReplyToMaster;
};

#endif
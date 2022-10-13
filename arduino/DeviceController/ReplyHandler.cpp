/*
    ReplyHandler.cpp - Library to handle replies to the master
*/

#include <Arduino.h>
#include "ReplyHandler.h"

#define INIT_BYTE 255       // 255 as default prefill of the message

ReplyHandler::ReplyHandler(int replyLength) {
    _replyLength = replyLength;
    // initialize the reply array that needs to be filled before the master requests the answer
    _arrReplyToMaster = new char[_replyLength];
    memset(_arrReplyToMaster, INIT_BYTE, _replyLength);
}

char* ReplyHandler::getReply() {
    return _arrReplyToMaster;
}

void ReplyHandler::setConfirmReply(char arrBytes[], int byteCount) {
    memset(_arrReplyToMaster, INIT_BYTE, _replyLength);
    // Clear the reply array
    // memset(_arrReplyToMaster, INIT_BYTE, _replyLength);
    for(int i = 0; i < byteCount; i++) {
        _arrReplyToMaster[i] = arrBytes[i];
    }
}

void ReplyHandler::setErrorReply(int errCode) {
    memset(_arrReplyToMaster, INIT_BYTE, _replyLength);
    _arrReplyToMaster[0] = 255;
    _arrReplyToMaster[1] = errCode;
}

void ReplyHandler::setErrorReply(int errCode, int arrMoreInfo[], int byteCount) {
    memset(_arrReplyToMaster, INIT_BYTE, _replyLength);
    _arrReplyToMaster[0] = 255;
    _arrReplyToMaster[1] = errCode;
    for (int i = 0; i < byteCount; i++) {
        _arrReplyToMaster[2 + i] = arrMoreInfo[i];
    }
}
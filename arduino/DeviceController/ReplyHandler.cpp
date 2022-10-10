/*
    ReplyHandler.cpp - Library to handle replies to the master
*/

#include <Arduino.h>
#include "ReplyHandler.h"

ReplyHandler::ReplyHandler(int replyLength) {
    _replyLength = replyLength;
    // initialize the reply array that needs to be filled before the master requests the answer
    _arrReplyToMaster = new char[_replyLength];
    memset(_arrReplyToMaster, 0, _replyLength);
}

char* ReplyHandler::getReply() {
    return _arrReplyToMaster;
}

void ReplyHandler::setConfirmReply(char arrBytes[], int byteCount) {
    // Clear the reply array
    // memset(_arrReplyToMaster, 0, _replyLength);
    for(int i = 0; i < byteCount; i++) {
        _arrReplyToMaster[i] = arrBytes[i];
    }
}

void ReplyHandler::setErrorReply(int errCode) {
    _arrReplyToMaster[0] = 255;
    _arrReplyToMaster[1] = errCode;
}

void ReplyHandler::setErrorReply(int errCode, int arrMoreInfo[], int byteCount) {
    _arrReplyToMaster[0] = 255;
    _arrReplyToMaster[1] = errCode;
    for (int i = 0; i < byteCount; i++) {
        _arrReplyToMaster[2 + i] = arrMoreInfo[i];
    }
}
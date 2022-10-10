/*
    ReplyHandler.cpp - Library to handle replies to the master
*/

#include <Arduino.h>
#include "ReplyHandler.h"

ReplyHandler::ReplyHandler(int replyLength) {
    _replyLength = replyLength;
    // initialize the reply array that needs to be filled before the master requests the answer
    _arrReplyToMaster = new char[_replyLength];
}

char* ReplyHandler::getReply() {
    return _arrReplyToMaster;
}

void ReplyHandler::setConfirmReply(int arrBytes[], int byteCount) {
    // Clear the reply array
    memset(_arrReplyToMaster, 0, _replyLength);
    // arrReplyToMaster[0] = arrBytes[0];
    // arrReplyToMaster[1] = arrBytes[1];
    // arrReplyToMaster[2] = arrBytes[2];
    // arrReplyToMaster[3] = arrBytes[3];
    // arrReplyToMaster[4] = arrBytes[4];
    // arrReplyToMaster[5] = arrBytes[5];
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
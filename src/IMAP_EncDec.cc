/******************************************************************************
* Copyright (c) 2005, 2015  Ericsson AB
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
* Tibor Bende
* Gábor Ziegler
* Gabor Szalai
******************************************************************************/
//
//  File:     IMAP_EncDec.cc
//  Rev:      R3A
//  Prodnr:   CNL 113 660
///////////////////////////////////////////////////////////////////////////////

#include <string.h>
#include <ctype.h>
#include "IMAP_Types.hh"


namespace IMAP__Types {

OCTETSTRING octenc__IMAP__PDU(const IMAP__PDU& pdu)
{
if (TTCN_Logger::log_this_event(TTCN_Logger::DEBUG_ENCDEC)) {
TTCN_Logger::begin_event(TTCN_Logger::DEBUG_ENCDEC);
TTCN_Logger::log_event_str("textenc_IMAP_PDU(): Encoding @IMAP_Types.IMAP_PDU: ");
pdu.log();
TTCN_Logger::end_event();
}
TTCN_EncDec::set_error_behavior(TTCN_EncDec::ET_ALL, TTCN_EncDec::EB_DEFAULT);
TTCN_Buffer ttcn_buffer;
pdu.encode(IMAP__PDU_descr_, ttcn_buffer, TTCN_EncDec::CT_TEXT);
OCTETSTRING ret_val;
ttcn_buffer.get_string(ret_val);
if (TTCN_Logger::log_this_event(TTCN_Logger::DEBUG_ENCDEC)) {
TTCN_Logger::begin_event(TTCN_Logger::DEBUG_ENCDEC);
TTCN_Logger::log_event_str("textenc_IMAP_PDU(): Stream after encoding: ");
ret_val.log();
TTCN_Logger::end_event();
}
return ret_val;
}

IMAP__ClientCommand octdec__IMAP__Command(const OCTETSTRING& stream)
{
if (TTCN_Logger::log_this_event(TTCN_Logger::DEBUG_ENCDEC)) {
TTCN_Logger::begin_event(TTCN_Logger::DEBUG_ENCDEC);
TTCN_Logger::log_event_str("textdec_IMAP_Command(): Stream before decoding: ");
stream.log();
TTCN_Logger::end_event();
}
TTCN_EncDec::set_error_behavior(TTCN_EncDec::ET_ALL, TTCN_EncDec::EB_DEFAULT);
TTCN_EncDec::set_error_behavior(TTCN_EncDec::ET_LOG_MATCHING, TTCN_EncDec::EB_WARNING);
TTCN_EncDec::clear_error();
TTCN_Buffer ttcn_buffer(stream);
IMAP__ClientCommand ret_val;
ret_val.decode(IMAP__ClientCommand_descr_, ttcn_buffer, TTCN_EncDec::CT_TEXT);
if (TTCN_Logger::log_this_event(TTCN_Logger::DEBUG_ENCDEC)) {
TTCN_Logger::begin_event(TTCN_Logger::DEBUG_ENCDEC);
TTCN_Logger::log_event_str("textdec_IMAP_Command(): Decoded @IMAP_Types.IMAP_ClientCommand: ");
ret_val.log();
TTCN_Logger::end_event();
}
if (TTCN_EncDec::get_last_error_type() == TTCN_EncDec::ET_NONE) {
if (ttcn_buffer.get_pos() < ttcn_buffer.get_len()-1 && TTCN_Logger::log_this_event(TTCN_WARNING)) {
ttcn_buffer.cut();
OCTETSTRING remaining_stream;
ttcn_buffer.get_string(remaining_stream);
TTCN_Logger::begin_event(TTCN_WARNING);
TTCN_Logger::log_event_str("textdec_IMAP_Command(): Warning: Data remained at the end of the stream after successful decoding: ");
remaining_stream.log();
TTCN_Logger::end_event();
}
}
return ret_val;
}

IMAP__ServerResponse octdec__IMAP__Response(const OCTETSTRING& stream)
{
if (TTCN_Logger::log_this_event(TTCN_Logger::DEBUG_ENCDEC)) {
TTCN_Logger::begin_event(TTCN_Logger::DEBUG_ENCDEC);
TTCN_Logger::log_event_str("textdec_IMAP_Response(): Stream before decoding: ");
stream.log();
TTCN_Logger::end_event();
}
TTCN_EncDec::set_error_behavior(TTCN_EncDec::ET_ALL, TTCN_EncDec::EB_DEFAULT);
TTCN_EncDec::clear_error();
TTCN_Buffer ttcn_buffer(stream);
IMAP__ServerResponse ret_val;
ret_val.decode(IMAP__ServerResponse_descr_, ttcn_buffer, TTCN_EncDec::CT_TEXT);
if (TTCN_Logger::log_this_event(TTCN_Logger::DEBUG_ENCDEC)) {
TTCN_Logger::begin_event(TTCN_Logger::DEBUG_ENCDEC);
TTCN_Logger::log_event_str("textdec_IMAP_Response(): Decoded @IMAP_Types.IMAP_ServerResponse: ");
ret_val.log();
TTCN_Logger::end_event();
}
if (TTCN_EncDec::get_last_error_type() == TTCN_EncDec::ET_NONE) {
if (ttcn_buffer.get_pos() < ttcn_buffer.get_len()-1 && TTCN_Logger::log_this_event(TTCN_WARNING)) {
ttcn_buffer.cut();
OCTETSTRING remaining_stream;
ttcn_buffer.get_string(remaining_stream);
TTCN_Logger::begin_event(TTCN_WARNING);
TTCN_Logger::log_event_str("textdec_IMAP_Response(): Warning: Data remained at the end of the stream after successful decoding: ");
remaining_stream.log();
TTCN_Logger::end_event();
}
}
return ret_val;
}

IMAP__ServerResponse__separated octdec__IMAP__Response__separated(const OCTETSTRING& stream)
{
if (TTCN_Logger::log_this_event(TTCN_Logger::DEBUG_ENCDEC)) {
TTCN_Logger::begin_event(TTCN_Logger::DEBUG_ENCDEC);
TTCN_Logger::log_event_str("textdec_IMAP_Response_separated(): Stream before decoding: ");
stream.log();
TTCN_Logger::end_event();
}
TTCN_EncDec::set_error_behavior(TTCN_EncDec::ET_ALL, TTCN_EncDec::EB_DEFAULT);
TTCN_EncDec::clear_error();
TTCN_Buffer ttcn_buffer(stream);
IMAP__ServerResponse__separated ret_val;
ret_val.decode(IMAP__ServerResponse__separated_descr_, ttcn_buffer, TTCN_EncDec::CT_TEXT);
if (TTCN_Logger::log_this_event(TTCN_Logger::DEBUG_ENCDEC)) {
TTCN_Logger::begin_event(TTCN_Logger::DEBUG_ENCDEC);
TTCN_Logger::log_event_str("textdec_IMAP_Response(): Decoded @IMAP_Types.IMAP_ServerResponse_separated: ");
ret_val.log();
TTCN_Logger::end_event();
}
if (TTCN_EncDec::get_last_error_type() == TTCN_EncDec::ET_NONE) {
if (ttcn_buffer.get_pos() < ttcn_buffer.get_len()-1 && TTCN_Logger::log_this_event(TTCN_WARNING)) {
ttcn_buffer.cut();
OCTETSTRING remaining_stream;
ttcn_buffer.get_string(remaining_stream);
TTCN_Logger::begin_event(TTCN_WARNING);
TTCN_Logger::log_event_str("textdec_IMAP_Response_separated(): Warning: Data remained at the end of the stream after successful decoding: ");
remaining_stream.log();
TTCN_Logger::end_event();
}
}
return ret_val;
}

INTEGER IMAP__get__msg__length(const OCTETSTRING& stream, const BOOLEAN& client__side){
  int i=stream.lengthof();  // number of bytes to search
  const unsigned char* s1=(const unsigned char*)stream; // search from here
  const unsigned char *s2; // pointer to the found \r
  int pp=0;  // number of already searched bytes
  int ret_val=-1;

  while(i && (s2=(const unsigned char *)memchr(s1,'\n',i))){  // search for the next \r
    int p=s2-s1+1;  // found one, the p-th byte
    if(*(s2-1)=='\r') {  //Is there \r before the \n
      if(client__side && p>=5 && *(s2-2)=='}'){  // check for imap literal = "{" number "}" CRLF *CHAR8
                                 // at least 5 byte including CRLF
        int k=p-3;
        const unsigned char *s3=s2-3;
        int a=1;
        int lit_len=0;
        while(k && isdigit(*s3)){ // search backward for the "{", the first non numeric char
          lit_len+=a*(*s3-'0');   // store the found digit
          k--;
          s3--;
          a*=10;
        }
        if(*s3=='{'){  // found the "{"
          if((i-p)>lit_len){ // skip the literal
            p+=lit_len;
            s2+=lit_len;
          } else { //not enough byte in the buffer
            return -1;
          }
        }  else { // in "{" not found just return the message until the CRLF
          ret_val= pp+p;  // YESSSS!, pp (skipped bytes previously) + p 
          break;
        }
      }  else {  // not a literal  
        ret_val= pp+p;  // YESSSS!, pp (skipped bytes previously) + p
        break;
      }
    }
    // NO :(  
    s1=s2+1;  // search from the bytes after the \n 
    pp+=p;   // just keep track the nuber of already checked bytes
    i-=p;    // we have that many bytes to search left
  }
  return ret_val;   // Nothing found, take a break

}

}

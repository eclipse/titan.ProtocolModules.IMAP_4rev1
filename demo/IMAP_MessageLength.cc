/******************************************************************************
* Copyright (c) 2000-2018 Ericsson Telecom AB
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v2.0
* which accompanies this distribution, and is available at
* https://www.eclipse.org/org/documents/epl-2.0/EPL-2.0.html
*
* Contributors:
* Tibor Bende
* Gábor Ziegler
* Gabor Szalai
******************************************************************************/
//
//  File:     TCCMessageHandling.cc
//  Rev:      R3A
//  Prodnr:   CNL 113 472
///////////////////////////////////////////////////////////////////////////////

#include "IMAP_Demo.hh"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

namespace IMAP__Demo {

const char* seekAfterCrlf(const char *buff, int length) {
	int current_state = 0, a = 0;

	TTCN_Logger::log(TTCN_DEBUG, "Seeking for initial CR");
	for (a = 0; a < length; a++) {
		//TTCN_Logger::log(TTCN_DEBUG, "Processing: %c, which is 0x%x", buff[a],	buff[a]);
		switch (current_state) {
		case 0:
			if (buff[a] == '\r') {
				current_state = 1;
				TTCN_Logger::log(TTCN_DEBUG,
						"Found a CR, now seeking for immediate LF");
			}
			break;
		case 1:
			if (buff[a] == '\n') {
				TTCN_Logger::log(TTCN_DEBUG, "Found immediate LF");
				current_state = 2;
			} else {
				if (buff[a] == '\r') {
					TTCN_Logger::log(TTCN_DEBUG,
							"Found a subsequent CR, keep seeking for immediate LF");
				} else {
					current_state = 0;
					TTCN_Logger::log(TTCN_DEBUG,
							"Back to seeking for initial CR");
				}
			}
			break;
		case 2:
			TTCN_Logger::log(TTCN_DEBUG, "Has seeked " + int2str(a) + " chars.");
			return buff + a;
			break;
		}
	}
	TTCN_Logger::log(TTCN_DEBUG, "Has seeked " + int2str(a)
			+ " chars, not found CRLF.");
	return NULL;
}
//this function supposes, that buff contains exactly one line terminated with CRLF
int get_char8count(const char *buff, const int length) {
	int ret_val = -1;
	int pos;
	TTCN_Logger::log(TTCN_DEBUG,
			"Last 3 chars of buff: %c (0x%x) %c (0x%x)%c (0x%x)",
			buff[length-3],buff[length-3],
			buff[length-2],buff[length-2],
			buff[length-1],buff[length-1]);

	//if contains CHAR8, then format looks like as "....{1}\r\n"
	if (length < 4) {
		TTCN_Logger::log(TTCN_DEBUG,
				"Buff length is less than 4, response cannot contain a \"literal\"");
		return ret_val;
	}
	pos = length - 3;
	//TTCN_Logger::log(TTCN_DEBUG,
	//		"3 chars from pos: %c (0x%x) %c (0x%x)%c (0x%x)",
	//		buff[pos],buff[pos],
	//		buff[pos+1],buff[pos+1],
	//		buff[pos+2],buff[pos+2]);

	//if buff does not end in "}\r\n", then response does not contain a "literal"
	if (strncmp("}\r\n", buff+pos, 3) != 0) {
		TTCN_Logger::log(TTCN_DEBUG,
				"Buff does not end in \"}\\r\\n\" response cannot contain a \"literal\"");
		return ret_val;
	}
	TTCN_Logger::log(TTCN_DEBUG,
			"Seeking backward for opening \"{\" before \"}\\r\\n\"");
        pos--;
	for (/*empty stmt.*/; pos >= 0; pos--) {
		TTCN_Logger::log(TTCN_DEBUG, "Processing: %c, which is 0x%x", buff[pos],
				buff[pos]);

		if (buff[pos] >= '0' && buff[pos] <= '9') {
			TTCN_Logger::log(TTCN_DEBUG, "It is a digit");
			continue;
		}
		if (buff[pos] == '{') {
			//TTCN_Logger::log(TTCN_DEBUG,
			//		"Found encoded length of \"literal\":%s", substr(buff, pos,
			//				length - pos));
			sscanf(buff + pos, "{%d}", &ret_val);
			TTCN_Logger::log(TTCN_DEBUG, "Decoded length as :%d", ret_val);
			return ret_val;
		}
                else {
		  TTCN_Logger::log(TTCN_DEBUG,
				"Found char which is neither digit, nor \"}\": %c, which is 0x%x, so response cannot contain a \"literal\"", buff[pos],
				buff[pos]);
		  return ret_val;
                }
	}
	return ret_val;
}

INTEGER f__IMAP__getMsgLen(const OCTETSTRING& stream, ro__integer& args) {
	char *msg_in_buffer_loc = (char*) ((const unsigned char*) stream);
	const int buf_length = stream.lengthof();
	const char *resp_end = NULL;
	const char * endpos = NULL;
	char *atm = msg_in_buffer_loc;
	int num_char8;

	//header_end=strstr(atm,"\n\r\n");  // search for alone crlf
	resp_end = seekAfterCrlf(atm, buf_length);

	//if we found CRLF
	if (resp_end) {
		TTCN_Logger::log(TTCN_DEBUG, "Found CRLF");
		//num. of CHAR8 is encoded as "...{1234}CRLF"
		num_char8 = get_char8count(atm, resp_end - atm);
		//if a literal length count has been found
		if (num_char8 > 0) {
			TTCN_Logger::log(TTCN_DEBUG,
					"Found \"literal\" string, seeking to its end beyond first CRLF");
			resp_end += num_char8; //proceed to end of literal CHAR8
		} else {
			TTCN_Logger::log(TTCN_DEBUG,
					"Not contains \"literal\" string argument, complete response found");
			return INTEGER(resp_end - atm); //otherwise this a complete resp.
		}

		//if literal and literal ends within buffer, then seek further until CRLF
		if (resp_end - atm <= buf_length) {
			TTCN_Logger::log(TTCN_DEBUG,
					"Found end of \"literal\" string, seeking to final CRLF");
			endpos = resp_end;
			resp_end = seekAfterCrlf(resp_end, buf_length - (resp_end - atm));
			//if we found CRLF
			if (resp_end) {
				TTCN_Logger::log(TTCN_DEBUG,
						"Found the final CRLF after \"literal\": complete response found");
				return INTEGER(resp_end - atm);
			} else
				TTCN_Logger::log(TTCN_DEBUG,
						"The final CRLF after \"literal\" is not in buffer: incomplete response found");
			return INTEGER(-1);
		}
	} else
		TTCN_Logger::log(TTCN_DEBUG, "No CRLF is in buffer: incomplete line");
	return INTEGER(-1);
}
}
//namespace

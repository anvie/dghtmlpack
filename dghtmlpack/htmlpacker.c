/**
 * Copyright (C) 2011 Robin Syihab.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "htmlpacker.h"


int get_n(const char* b, const size_t buff_size){
	int i;
	int cnt;
	cnt = 0;
	for (i = 0; i < buff_size; i++) {
		if (b[i] != 10 && b[i] != 13) {
			cnt++;
		}else {
			break;
		}
	}
	return cnt;
}


int long_space(const char* b, const size_t buff_size){
	int i;
	int spc;
	spc = 0;
	for (i = 0; i < buff_size; i++) {
		if (b[i] == 32) {
			spc++;
		}else{ break; }
	}
	return spc - 1;
}

int do_html_pack(const char* in_buff, const size_t in_buff_size, char* out_buff, const size_t out_buff_size)
{
	const char* ps;
	char* pd;
	int i;
	int o;
	int skip;
	int in_js;
	int brw;
	int in_quote;
	int in_js_comment;
	int in_html_comment;
	
	o = 0;
	skip = 0;
	ps = in_buff;
	pd = out_buff;
	in_js = 0;
	brw = 0;
	in_quote = 0;
	in_js_comment = 0;
	in_html_comment = 0;
	
	
	if(in_buff_size > 40 && strncmp(in_buff, "<!DOCTYPE", 9) == 0){
		// search for enclosed >
		i = 0;
		while (ps[i] != '>' && i < in_buff_size) {
			if (i >= 1024) {
				break;
			}
			i++;
		}
		if (i >= 1024) {
			ps = in_buff;
		}else {
			
			strncpy(out_buff, in_buff, (size_t)i+1);
			o = i + 1;
			out_buff[o++] = 10;
			ps = &in_buff[i+1];
			
		}
	}
	
	for(i=0; i < in_buff_size; i++ )
	{
		if (i >= out_buff_size) {
			break;
		}
		if (ps[i] == 13 || ps[i] == 10 || ps[i] == 9){
			continue;
		}
		skip = long_space(&ps[i], (out_buff_size - (size_t)i));
		if (skip >= 3) {
			pd[o++] = 32;
			i += skip;
			continue;
		}
		
		
		// strip js comment
		if (in_js == 1 && in_quote == 0 && (out_buff_size > (i + 2))) {
			if (ps[i] == 47 && ps[i+1] == 47) {
				brw = get_n(&ps[i], (out_buff_size - (size_t)i));
				if (brw > 0) {
					i += brw;
					continue;
				}
			}
		}
		
		// remove /* XXX */
		if (in_js_comment == 0 && ps[i] == 0x2F && ps[i+1] == 0x2A) {
			in_js_comment = 1;
		}
		
		if (in_js_comment == 1 && ps[i] == 0x2A && ps[i+1] == 0x2F) {
			i += 2;
			if (out_buff_size < (i + 2)) {
				break;
			}
			in_js_comment = 0;
		}
		
		if (in_js_comment == 1) { continue; }
		
		if (ps[i] == 0x22 || ps[i] == 0x27) {
			in_quote = in_quote == 0 ? 1 : 0;
		}
		
		// remove <!-- XXX -->
		if(in_html_comment == 0 && ps[i] == '<' && ps[i+1] == '!' && ps[i+2] == '-' && ps[i+3] == '-'){
			in_html_comment = 1;
		}
		
		if(in_html_comment == 1 && ps[i] == '-' && ps[i+1] == '-' && ps[i+2] == '>'){
			in_html_comment = 0;
		}
		
		if (in_html_comment == 1) { continue; }
		
		
		// inside <script?
		if (((i + 7) < out_buff_size) && in_js == 0 && in_quote != 1 && ps[i] == 0x3C && ps[i+1] == 0x73 && ps[i+2] == 0x63 && ps[i+3] == 0x72 && ps[i+4] == 0x69 && ps[i+5] == 0x70 && ps[i+6] == 0x74) {
			in_js = 1;
		}
		
		if (((i + 7) < out_buff_size) && in_js == 1 && in_quote != 1 &&  ps[i] == 0x3C && ps[i+1] == 0x2F && ps[i+2] == 0x73 && ps[i+3] == 0x63 && ps[i+4] == 0x72 && ps[i+5] == 0x69 && ps[i+6] == 0x70) {
			in_js = 0;
		}
		
		pd[o++] = ps[i];
	}
	return 1;
}





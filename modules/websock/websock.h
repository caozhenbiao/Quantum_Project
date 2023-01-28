#ifndef _WEBSOCK_H_
#define _WEBSOCK_H_
//websocket
enum WS_FrameType{
    WS_EMPTY_FRAME = 0xF0,
    WS_ERROR_FRAME = 0xF1,
    WS_TEXT_FRAME = 0x01,
    WS_BINARY_FRAME = 0x02,
    WS_PING_FRAME = 0x09,
    WS_PONG_FRAME = 0x0A,
    WS_OPENING_FRAME = 0xF3,
    WS_CLOSING_FRAME = 0x08
};
int last_conn_sock;
void websock_start(const char* ip, unsigned short port);
void websock_stop();
int websock_send_text(int cs, const char* text);
int websock_send_bin(int cs, unsigned char* buf, unsigned int len);
int websock_handshake(const char* key, char* rsp );
int websock_getip(const char* nc, char* ip);
#endif

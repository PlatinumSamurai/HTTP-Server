#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "http.h"

void index_page(int conn, HttpRequest *req);
void about_page(int conn, HttpRequest *req);
/*
extern typedef struct Ht Ht;
extern typedef struct Tp Tp;
*/
extern Ht* HtInit();
extern Tp* TpInit();
extern void HttpHandle();
extern int8_t HttpListen();
extern void HtFinalize();
extern void TpFinalize();


//void index_page(int conn, HttpRequest *req);
//void about_page(int conn, HttpRequest *req);

int main(void) {
	Ht *ser = HtInit("127.0.0.1");
    Tp *ve = TpInit(ser);
	HttpHandle(ser, ve, "/", index_page);
	HttpHandle(ser, ve, "/about", about_page);

	HttpListen(ser, ve);
	HtFinalize(ser);
    TpFinalize(ve);
	return 0;
}
/*
int soer() {
    main();
    exit(0);
}
*/
void index_page(int conn, HttpRequest *req) {
	if(strcmp(req->path, "/") != 0) {
        HttpParse(conn, "page404.html");
		return;
	}
	HttpParse(conn, "index.html");
}

void about_page(int conn, HttpRequest *req) {
	if(strcmp(req->path, "/about") != 0) {
        HttpParse(conn, "page404.html");
		return;
	}
	HttpParse(conn, "about.html");
}

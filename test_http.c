#include <stdio.h>
#include <string.h>

#include "http.h"

void index_page(int conn, HttpRequest *req);
void about_page(int conn, HttpRequest *req);

int main(void) {
	Http *serve = HttpInit("127.0.0.1");

	HttpHandle(serve, "/", index_page);
	HttpHandle(serve, "/about/", about_page);

	HttpListen(serve);
	HttpFinalize(serve);
	return 0;
}

void index_page(int conn, HttpRequest *req) {
	if(strcmp(req->path, "/") != 0) {
		HttpParse(conn, "page404.html");
		return;
	}
	HttpParse(conn, "index.html");
}

void about_page(int conn, HttpRequest *req) {
	if(strcmp(req->path, "/about/") != 0) {
		HttpParse(conn, "page404.html");
		return;
	}
	HttpParse(conn, "about.html");
}

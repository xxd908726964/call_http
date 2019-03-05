#include"call_http.h"

int main(){

    ssl_connet_host("www.jb51.net",443);
	char HttpsPostHeaders[] = "GET / HTTP/1.1\r\n"
		"Host: www.jb51.net\r\n"
		;





	reqest(HttpsPostHeaders);
	printf("--------\n");
	printf("%s\n", http_res.http_hander);
	//printf("%s\n", http_res.http_data);
	getchar();
    
    ssl_disconnect();

    return 0;
}
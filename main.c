#include "call_http.h"

int main()
{

	ssl_connet_host("cpc.people.com.cn", 80);
	char HttpsPostHeaders[] = "GET http://cpc.people.com.cn/xuexi/GB/425746/index.html HTTP/1.1\r\n"
							  "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
							  "Accept-Encoding: gzip, deflate\r\n"
							  "Accept-Language: zh-CN,zh;q=0.9\r\n"
							  "Cache-Control: max-age=0\r\n"
							  "Connection: keep-alive\r\n"
							  "Cookie: wdcid=1c269fc9be8ef48f; wdlast=1551789930; wdses=1ca5408dbe2da56d; sso_c=0; sfr=1\r\n"
							  "Host: cpc.people.com.cn\r\n"
							  "If-Modified-Since: Sun, 03 Mar 2019 23:49:20 GMT\r\n"
							  "If-None-Match: W/ 5c7c6800 - 52fd \r\n"
							  "Upgrade-Insecure-Requests: 1\r\n"
							  "User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36\r\n\r\n";

	reqest(HttpsPostHeaders);
	printf("--------\n");
	printf("%s\n", http_res.http_hander);
	printf("http_data_len=%d\n",http_res.http_data_len);
	printf("%s\n", http_res.http_data);
	getchar();

	ssl_disconnect();

	return 0;
}
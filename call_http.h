#ifndef H_CALL_HTTP_H
#define H_CALL_HTTP_H

#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>
#include<netdb.h>
#include<openssl/ssl.h>
#include<openssl/err.h>
#include<unistd.h>
#include<stdlib.h>
#include<regex.h>



struct http_res_
{
	char* http_hander;//http头部分
	char* http_data;//http数据部分
}http_res;

typedef int (*__io)(char* buf, int num);

/*
 * @Name 			- 创建tcp连接到http
 * @Parame 	*host 	- 主机地址, 即域名
 * @Parame 	 port 	- 端口号, 一般为443
 * @return 			- 成功返回0, 如果读取失败, 则返回值-1，并设置errno
 */
int ssl_connet_host(const char* hsot,int prot);



/*
 * @Name 		- 	初始化SSL, 并且绑定sockfd_fd到SSL
 * 					此作用主要目的是通过SSL来操作sock					
 * @return 		- 	成功返回0, 如果读取失败, 则返回值-1
 */
 int ssl_init();

 int read__(char *buf,int num);
 int write__(char *buf,int num);
 int SSL_read__(char *buf,int num);
 int SSL_write__(char *buf,int num);


/*发送请求，返回结果保存在结构http_res中，http_res结构如下：

	struct http_res_
	{
	char *http_hander=NULL;//http头部分
	char *http_data=NULL;//http数据部分
	}

* @Parame 	*req 	- 请求内容

*/
int reqest(char*req);



// 关闭会话, 释放内存
int ssl_disconnect();












#endif
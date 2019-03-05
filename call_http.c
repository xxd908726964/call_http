#include "call_http.h"

__io myread = NULL;//接收http、https返回函数指针
__io mywrite = NULL;//发送http、https请求函数指针
/*普通连接套接字*/
 int socket_fd = -1;
/*已经完成初始化并绑定了对应sock的SSL指针*/
 SSL* ssl = NULL;

int ssl_connet_host(const char *host, int prot)
{

    if (!host)
        return -1;
    struct hostent *host_add;
    struct sockaddr_in addr;

    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_fd < 0)
    {
        perror("create socket error ");
        return -1;
    }
    host_add = gethostbyname(host);
    if (!host_add)
    {
        perror("gethostbyname error ");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(prot);
    addr.sin_addr = *((struct in_addr *)(*host_add->h_addr_list));
    if (connect(socket_fd, (struct sockaddr *)(&addr), sizeof(addr)) < 0)
    {
        perror("connect error ");
        return -1;
    }
	switch (prot)
	{
	case 80:
		myread = read__;
		mywrite = write__;
		break;
	case 443:
		myread = SSL_read__;
		mywrite = SSL_write__;
		ssl_init();
		SSL_connect(ssl);
		break;
	default:
		fprintf(stderr, "%s", "需要http端口（端口号80）或者https端口（端口号443）");
		return -1;
	}

	

    return 0;
}

 int ssl_init()
{
    if (socket_fd < 0)
    {
        return -1;
    }
    int re = 0;
    SSL_CTX *ctx;

    SSL_library_init();
    SSL_load_error_strings();
    ctx = SSL_CTX_new(SSLv23_client_method());
    if (ctx == NULL)
    {
        return -1;
    }

    ssl = SSL_new(ctx);
    if (ssl == NULL)
    {
        return -1;
    }

    /* 把socket和SSL关联 */
    re = SSL_set_fd(ssl, socket_fd);
    if (re == 0)
    {
        SSL_free(ssl);
        return -1;
    }

    /*
     * ctx使用完成, 进行释放
     */
    SSL_CTX_free(ctx);
	
    return 0;
}
 int read__(char *buf,int num){
    return read(socket_fd,buf,num);
}

 int write__(char *buf,int num){
    return write(socket_fd,buf,num);
}
 int SSL_read__(char *buf,int num){
    return SSL_read(ssl,buf,num);
}
 int SSL_write__(char *buf,int num){
    return SSL_write(ssl,buf,num);
}

 int ssl_disconnect() {
	 // 关闭会话, 释放内存
	 if (!http_res.http_hander)free(http_res.http_hander);
	 if (!http_res.http_data)free(http_res.http_data);
	 close(socket_fd);
	 if (myread == SSL_read__) //--------------------------------------------
	 {
	 SSL_shutdown(ssl);
	 ERR_free_strings();
 }
}


int reqest( char* req) {

	mywrite(req, strlen(req));
	int http_hander_size = 100;
	http_res.http_hander = malloc(sizeof(char)* http_hander_size);
	if (!http_res.http_hander)return -1;
	memset(http_res.http_hander, 0, http_hander_size);
	char* http_hander_temp = http_res.http_hander;
	int _r_n_count = 0;
	for (int i = 0; i < 100;i++) {
		myread(http_hander_temp, 1);
		if (*http_hander_temp == '\r' || *http_hander_temp == '\n')
			_r_n_count++;
		if (4 == _r_n_count) {

			_r_n_count = 0;
			break;
		}
		if (99 == i && 4 != _r_n_count) {
			http_hander_size += 100;
			if (!realloc(http_res.http_hander, sizeof(char) * http_hander_size))return -1;;
			http_hander_temp = http_res.http_hander + http_hander_size-100;
			i = 0;
		}
		http_hander_temp++;
		fprintf(stderr,"%c", *http_hander_temp);
	}
	printf("%s\n", http_res.http_hander);
	regex_t reg;
	regmatch_t reg_mat[2]; 
	int a;
	a=regcomp(&reg, "Content-Length:(.*)", REG_EXTENDED | REG_NEWLINE|REG_ICASE);
		char err[50] = { '\0' };
		if (a != 0) {
			regerror(a, &reg, err, 50);
			printf("regcomp-%s\n", err);
			return -1;
		}
	a = regexec(&reg, http_res.http_hander, 2, reg_mat, 0);
	if (a!=0){
		regerror(a, &reg, err, 50);
		printf("regexec-%s\n", err); 
		return -1;
	}
	char http_leng[20] = { '\0' };
	memcpy(http_leng, http_res.http_hander + reg_mat[1].rm_so, reg_mat[1].rm_eo - reg_mat[1].rm_so);
	int http_data_leng=atoi(http_leng);
	http_res.http_data = malloc(http_data_leng);
	char* http_data_temp = http_res.http_data;
	int re = 0;
	while (1) {
		re+=myread(http_res.http_data, 100);
		http_data_temp += re;
		if (http_data_leng == re)break;
	}
	regfree(&reg);
	return 0;
}
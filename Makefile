a.out:call_http.c call_http.h main.c
		gcc -g call_http.c call_http.h main.c -o a.out -lssl -lcrypto
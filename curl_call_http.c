#include <stdio.h>
#include<curl/curl.h>
#include<string.h>
#include<regex.h>
#include<stdlib.h>


typedef struct MemoryStruct {
	char* memory;
	size_t size;
}MemoryStruct;

static size_t
WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*)userp;
	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if (mem->memory == NULL) {
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}


int regx(MemoryStruct chunk) {
	while (1)
	{
		regex_t reg;
		regmatch_t rm[3];
		char reg_c[] = "<p>(<strong>)?(.+)$";
		int err;
		err = regcomp(&reg, reg_c, REG_EXTENDED | REG_NEWLINE);
		char erbuf[100];
		regerror(err, &reg, erbuf, 100); //printf("regcomp====%s\n", erbuf);
		err = regexec(&reg, chunk.memory, 3, rm, 0);
		regerror(err, &reg, erbuf, 100); //printf("regerror====%s====rm_eo=%d  rm_so=%d\n", erbuf, rm[1].rm_eo, rm[1].rm_so);
		char* re_ = malloc(rm[2].rm_eo - rm[2].rm_so + 1);
		memset(re_, '\0', rm[2].rm_eo - rm[2].rm_so + 1);
		memcpy(re_, chunk.memory + rm[2].rm_so, rm[2].rm_eo - rm[2].rm_so);
		//printf("%s\n", re_);
		regfree(&reg);
		chunk.memory += rm[2].rm_eo;

		char reg_c_[] = "(.+)(</strong>)?</p>";
		err = regcomp(&reg, reg_c_, REG_EXTENDED | REG_NEWLINE);
		regerror(err, &reg, erbuf, 100);//printf("regcomp====%s\n", erbuf);
		err = regexec(&reg, re_, 3, rm, 0);
		regerror(err, &reg, erbuf, 100); //printf("regerror====%s====rm_eo=%d  rm_so=%d\n", erbuf, rm[1].rm_eo, rm[1].rm_so);
		char* re__ = malloc(rm[1].rm_eo - rm[1].rm_so + 1);
		memset(re__, '\0', rm[1].rm_eo - rm[1].rm_so + 1);
		memcpy(re__, re_ + rm[1].rm_so, rm[1].rm_eo - rm[1].rm_so);
		if (!err) printf("%s\n", re__);
		regfree(&reg);
		free(re__);
		free(re_);
		if (err)return 0;
	}
}

int main(int agv,char*agc[])
{	
	if (agv != 2)
	{
		printf("参数个数不对！！\n");
		return -1;
	}
	MemoryStruct chunk;
	chunk.memory = malloc(1); 
	chunk.size = 0;
	CURL* easy_handle =	curl_easy_init();
	curl_easy_setopt(easy_handle, CURLOPT_URL, agc[1]);
	curl_easy_setopt(easy_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	curl_easy_setopt(easy_handle, CURLOPT_WRITEDATA, (void*)& chunk);
	curl_easy_perform(easy_handle);

	//printf("%s\n-----------------------------------------------------------\n\n", chunk.memory);
	regx(chunk);
	free(chunk.memory);
	curl_easy_cleanup(easy_handle);
    return 0;
}
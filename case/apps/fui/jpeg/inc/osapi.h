#ifndef __OSAPI_H__
#define __OSAPI_H__

#ifdef __cplusplus
extern "C" {
#endif

void *OSSemCreate(int cnt);
int OSSemPend(void *psem, int timeout);
int OSSemPost(void *psem);
int OSSemAccept(void *psem);
void OSSemClose(void *psem);

void *OSTaskCreate(void (*task)(void*), void *pdata, int stack_size, int priority);
int OSTaskDel(void *ptask);
int OSTaskSuspend(void *ptask);
int OSTaskResume(void *ptask);

void *OSTimerCreate(int delay, void (*proc)(void*), void *pdata);
int OSTimerDel(void *ptimer);

void OSSleep(unsigned long time_ms);

int OSGetTime(unsigned long *psec, unsigned long *pusec);

typedef struct _os_q_data_
{
	void *OSMsg;
	int OSNMsgs;
	int OSQSize;
}OS_Q_DATA;

void *OSQCreate(void **start, int size);
void OSQDel(void *pq);
int OSQPost(void *pq, void *msg);
int OSQPostFront(void *pq, void *msg);
void *OSQPend(void *pq, int timeout);
void *OSQAccept(void *pq);
int OSQFlush(void *pq);
int OSQQuery(void *pq, OS_Q_DATA *pdata);

int OSKBOpen();
int OSKBClose();
int OSKBHit();
int OSKBGetChar();

typedef struct _find_file_info_
{
	char *name;
	char *path;
	int is_dir;
}OS_FILE_FIND_DATA;

void *OSFindFirstFile(char *dir_name, OS_FILE_FIND_DATA *p_find_file_data);
int OSFindNextFile(void *handle, OS_FILE_FIND_DATA *p_find_file_data);
int OSFindClose(void *handle);

void *OSDLOpen(char *file_name);
int OSDLClose(void *handle);
void *OSDLGetProcAddr(void *handle, char *func_name);

typedef struct _os_heap_data_
{
	int max_blk_size;
	int remain_size;
	int blk_cnt;
}OS_HEAP_DATA;

void *OSHCreate(void **pmem, unsigned long *pbus, int *psize, int n, int alignment);
int OSHDel(void *handle);
void *OSHmalloc(void *handle, int size, unsigned long *pbus);
void *OSHmalloc_r(void *handle, int size, unsigned long *pbus);
void *OSHrealloc(void *handle, void *ptr, int size, unsigned long *pbus);
void *OSHrealloc_r(void *handle, void *ptr, int size, unsigned long *pbus);
int OSHfree(void *handle, void *ptr);
int OSHQuery(void *handle, OS_HEAP_DATA *pdata);

void *OSmalloc(int size);
void OSfree(void *ptr);

void *OSmemset(void *s, int c, int n);
void *OSmemcpy(void *dest, void *src, int n);
void *OSmemmove(void *dest, void *src, int n);
int OSmemcmp(void *s1, void *s2, int n);

int OSprintf(char *fmt,...);
int OSputs(char *s);
int OSsprintf(char *str, char *fmt, ...);

char *OSstrcpy(char *dest, char *src);
char *OSstrncpy(char *dest, char *src, int n);

int OSstrcmp(char *s1, char *s2);
int OSstrncmp(char *s1, char *s2, int n);

int OSstrcasecmp(char *s1, char *s2);
int OSstrncasecmp(char *s1, char *s2, int n);

int OSstrlen(char *s);

char *OSstrcat(char *dest, char *src);
char *OSstrncat(char *dest, char *src, int n);

char *OSstrstr(char *haystack, char *needle);
char *OSstrcasestr(char *haystack, char *needle);

int OSatoi(char *nptr);

int OSrand(unsigned long *seedp);

#ifdef WIN32
#define int64	__int64
//typedef __int64 int64;
//typedef unsigned __int64 uint64;
#else
#define int64 long long
//typedef long long int64;
//typedef unsigned long long uint64;
#endif

void *OSfopen(char *path, char *mode);
int OSfclose(void *fp);
int OSfeof(void *fp);
int OSfread(void *ptr, int size, int nmemb, void *fp);
int OSfwrite(void *ptr, int size, int nmemb, void *fp);
int OSfseek_set(void *fp, long offset);
int OSfseek_cur(void *fp, long offset);
int OSfseek_end(void *fp, long offset);
long OSftell(void *fp);
int OSfseek64_set(void *fp, int64 offset);
int OSfseek64_cur(void *fp, int64 offset);
int OSfseek64_end(void *fp, int64 offset);
int64 OSftell64(void *fp);
int OSfflush(void *fp);
int OSfprintf(void *fp, char *fmt, ...);
int OSfputs(char *s, void *fp);
int OSfputc(int c, void *fp);

void *OSmopen();
void OSmclose(void *handle);
void *OSmmap(void *handle, unsigned long bus, int size);
int OSmunmap(void *handle, void *ptr, int size);

#ifndef NULL
#define NULL	((void*)0)
#endif//NULL

#ifdef __cplusplus
}
#endif

#endif//__OSAPI_H__

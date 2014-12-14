/* SDSLib, A C dynamic strings library
 *
 * Copyright (c) 2006-2010, Salvatore Sanfilippo <antirez at gmail dot com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __SDS_H
#define __SDS_H

#define SDS_MAX_PREALLOC (1024*1024)

#include <sys/types.h>
#include <stdarg.h>

// sds 就是数据指针，但在紧挨着sdshdr数据结构的
typedef char *sds;

struct sdshdr {
    unsigned int len;
    unsigned int free;
    char buf[];
};

// return lenght of len
//返回 len 长度
static inline size_t sdslen(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->len;
}

// return lenght of len
//返回 free 长度
static inline size_t sdsavail(const sds s) {
    struct sdshdr *sh = (void*)(s-(sizeof(struct sdshdr)));
    return sh->free;
}
// 创建新sds
sds sdsnewlen(const void *init, size_t initlen);
sds sdsnew(const char *init);
sds sdsempty(void);

// 返回长度
size_t sdslen(const sds s);

// 复制了新的sds类型，新分配内存
sds sdsdup(const sds s);

// 删除sds空间 
void sdsfree(sds s);

size_t sdsavail(const sds s);

// 扩展sds 到某一个长度，扩展的空间都设置为0
sds sdsgrowzero(sds s, size_t len);

// 拼接长度为len的数据
sds sdscatlen(sds s, const void *t, size_t len);

// 拼接字符串数据到sds类型中
sds sdscat(sds s, const char *t);
sds sdscatsds(sds s, const sds t);

// 长度为len的字符串数据复制到sds中
sds sdscpylen(sds s, const char *t, size_t len);
sds sdscpy(sds s, const char *t);

sds sdscatvprintf(sds s, const char *fmt, va_list ap);
#ifdef __GNUC__
sds sdscatprintf(sds s, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
sds sdscatprintf(sds s, const char *fmt, ...);
#endif

sds sdscatfmt(sds s, char const *fmt, ...);

// 两端删除字符串列表，空间并没有释放，多出来的长度设置在free中
sds sdstrim(sds s, const char *cset);

// sds数据只保存start 和 end 之间的字符串， 并没有释放内存，多余长度存储在free中
void sdsrange(sds s, int start, int end);

// 更新sds实际长度
void sdsupdatelen(sds s);

// 清空sds数据，长度为0, 并没有实际释放内存
void sdsclear(sds s);

// 比较两个sds类型
int sdscmp(const sds s1, const sds s2);

// 分割字符，返回元素列表
sds *sdssplitlen(const char *s, int len, const char *sep, int seplen, int *count);
void sdsfreesplitres(sds *tokens, int count);

void sdstolower(sds s);
void sdstoupper(sds s);

// long long 类型转化为sds
sds sdsfromlonglong(long long value);
sds sdscatrepr(sds s, const char *p, size_t len);
sds *sdssplitargs(const char *line, int *argc);

// 用to字符串替换from
sds sdsmapchars(sds s, const char *from, const char *to, size_t setlen);
sds sdsjoin(char **argv, int argc, char *sep);

/* Low level functions exposed to the user API */
// sds追加addlen长度，分配空间，设置free
sds sdsMakeRoomFor(sds s, size_t addlen);
void sdsIncrLen(sds s, int incr);

// 清除free所占的空间
sds sdsRemoveFreeSpace(sds s);

// sds 所占的空间大小
size_t sdsAllocSize(sds s);

#endif

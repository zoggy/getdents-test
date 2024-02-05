#include <caml/version.h>

#include <caml/alloc.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/unixsupport.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/syscall.h>

#include <lwt_unix.h>

typedef struct list { char* head; struct list* tail ; } *list_t;

struct job_getdents {
    struct lwt_unix_job job;
    list_t result;
    int error_code;
    char* dir;
    char data[];
};
struct linux_dirent {
           unsigned long  d_ino;
           off_t          d_off;
           unsigned short d_reclen;
           char           d_name[];
       };

#define BUF_SIZE 1024 * 1024 * 5

static void worker_getdents(struct job_getdents *job)
{
    int fd;
    long nread = 0;
    list_t result = NULL ;
    char buf[BUF_SIZE];
    struct linux_dirent  *d;

    fd = open(job->dir, O_RDONLY | O_DIRECTORY);
    if (fd == -1) {
      job->error_code = errno;
      return;
   }

   for (;;) {
        errno = 0;
        nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);

        /* An error happened. */
        if (nread == -1 || errno != 0) {
            job->error_code = errno;
            close(fd);
            return;
        }

        /* End of directory reached */
        if (nread == 0 && errno == 0) break;

        /* getdents is good */
        for (size_t bpos = 0; bpos < nread;) {
          d = (struct linux_dirent *) (buf + bpos);
          if (d->d_ino != 0) {
            list_t r= lwt_unix_malloc(sizeof(struct list)) ;
            r->tail = result ;
            r->head = strdup(d->d_name) ;
            result = r ;
          }
          bpos += d->d_reclen;
        }
        /* All is good */
        job->result = result;
    }
    close(fd);
    job->error_code = 0;
}

static value result_getdents(struct job_getdents *job)
{
    CAMLparam0();
    CAMLlocal2(result, res);
    int error_code = job->error_code;
    lwt_unix_free_job(&job->job);
    result = Val_emptylist;
    if (error_code) {
        unix_error(error_code, "getdents", Nothing);
    } else {
        list_t r = job->result;
        while (r) {
          res = caml_alloc_2(Tag_cons, caml_copy_string(r->head), result);
          free(r->head);
          result = res;
          r = r->tail ;
        }
        CAMLreturn(result);
    }
}

CAMLprim value lwt_unix_getdents_job(value dir)
{
    LWT_UNIX_INIT_JOB_STRING(job, getdents, 0, dir);
    return lwt_unix_alloc_job(&job->job);
}

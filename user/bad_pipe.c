#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define PIPESIZE 32

struct bad_pipe {
  char data[PIPESIZE];
  uint nread;   // number of bytes read
  uint nwrite;  // number of bytes written
};

void
pipe_write(struct bad_pipe *pi, char ch)
{
  // same core pattern as kernel/pipe.c
  pi->data[pi->nwrite % PIPESIZE] = ch;
  pi->nwrite++;
}

int
pipe_read(struct bad_pipe *pi)
{
  // empty when nread == nwrite
  if(pi->nread == pi->nwrite)
    return -1;

  char ch = pi->data[pi->nread % PIPESIZE];
  pi->nread++;
  return (unsigned char)ch;
}

int
main(void)
{
  struct bad_pipe pipe;
  pipe.nread = 0;
  pipe.nwrite = 0;

  char ch;
  char last3[3] = {0, 0, 0}; // Track last 3 characters to detect "ok?"

  printf("Type text. Enter 'ok?' to stop and display buffer contents.\n\n");

  while(read(0, &ch, 1) == 1){

    last3[0] = last3[1];
    last3[1] = last3[2];
    last3[2] = ch;

    pipe_write(&pipe, ch);

    if(last3[0] == 'o' && last3[1] == 'k' && last3[2] == '?'){
      if(pipe.nwrite >= 3)
        pipe.nwrite -= 3;
      break;
    }
  }

  printf("\n--- Stored output (bad pipe) ---\n");

  // Print contents by reading stored data
  int out;
  while((out = pipe_read(&pipe)) != -1){
    printf("%c", out);
  }

  printf("\n");
  exit(0);
}
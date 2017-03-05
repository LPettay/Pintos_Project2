#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

/* Collin Vossman - Project 2 */

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  int args[3];

  // Gets the number for the system call.  Check lib/syscallnr.h for numbers.
  int syscall_no = *(int *)create_kernel_ptr(f->esp);
  
  switch (syscall_no)
  {
    case SYS_HALT:
      syscall_halt();
      break;
    case SYS_EXIT: // Check
      get_args(f, args, 1);
      sys_exit(args[0]);
      break;
    case SYS_EXEC:
      break;
    case SYS_WAIT:
      break;
    case SYS_CREATE:
      break;
    case SYS_REMOVE:
      break
    case SYS_OPEN:
      break;
    case SYS_FILESIZE:
      break;
    case SYS_READ:
      break;
    case SYS_WRITE:
      break;
    case SYS_SEEK:
      break;
    case SYS_TELL:
      break;
    case SYS_CLOSE:
      break;
    default:
      sys_exit(-1);
  }
}

/* Collin Vossman - Project 2 */


void
syscall_halt(void)
{

}


/* Exit current process */
void
sys_exit(int status)
{
  // Saves exit status to the current thread.
  struct thread * cur = thread_current();
  cur->exit_status = status;

  // Print exit status.
  printf("%s: exit(%d)\n", cur->name, cur->exit_status);
  
  // Exits current thread.
  thread_exit();
}

pid_t
sys_exec(const char *cmd_line)
{
  // Converts cmd_line to a kernel pointer.
  cmd_line = create_kernel_ptr(cmd_line);

  pid_t pid = process_execute(cmd_line); // Check process_execute in process.c

  // Wait for child and return pid when it loads or -1 if it fails to load.
  return thread_wait_for_load(pid) ? pid : -1; // Double check this statement.
}

int
sys_wait(pid_t pid)
{
  
}

bool
sys_create(const char *file, unsigned initial_size)
{

}

bool
sys_remove(const char *file)
{

}

int
sys_open(const char *file)
{

}

int
sys_filesize(int fd)
{

}

int
sys_read(int fd, void *buffer, unsigned size)
{

}

int
sys_write(int fd, const void *buffer, unsigned size)
{
  
}

void 
sys_seek(int fd, unsigned position)
{

}

unsigned
sys_tell(int fd)
{

}

void
sys_close(int fd)
{

}


/* Get arguments from the stack.
   f    - The structure to hold the stack pointer and registers.
   args - The array of arguments that we are filling with the
   	  arguments from the stack.
   n    - Number of arguments to read in.
 */

static void get_args(struct intr_frame *f, int* args, int n)
{
  int * stack_args = f->esp + sizeof(int);
  for (int i = 0; i < n; i++)
  {
    if (!is_user_vaddr((const void *)stack_args))
    {
      sys_exit(-1);
    }
    args[i] = stack_args[i];
  }
}

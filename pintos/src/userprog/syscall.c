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

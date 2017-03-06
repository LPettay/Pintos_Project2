#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"

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
  int args[3]; // Since max args passed into any call is 3.

  // Gets the number for the system call.  Check lib/syscallnr.h for numbers.
  int syscall_no = *(int *)create_kernel_ptr(f->esp);
  
  switch (syscall_no)
  {
    case SYS_HALT:
      syscall_halt();
      break;
    case SYS_EXIT:
      get_args(f, args, 1);
      sys_exit(args[0]);
      break;
    case SYS_EXEC:
      get_args(f, args, 1);
      f->eax = sys_exec((const char *)args[0]);
      break;
    case SYS_WAIT:
      get_args(f, args, 1);
      f->eax = sys_wait(args[0]);
      break;
    case SYS_CREATE:
      get_args(f, args, 2);
      f->eax = sys_create((const char *)args[0], (unsigned)args[1]);
      break;
    case SYS_REMOVE:
      get_args(f, args, 1);
      f->eax = sys_remove((const char *)args[0]);
      break
    case SYS_OPEN:
      get_args(f, &args[0], 1);
      f->eax = sys_open((const char *)args[0]);
      break;
    case SYS_FILESIZE:
      get_args(f, args, 3);
      f->eax = sys_filesize(args[0]);
      break;
    case SYS_READ:
      get_args(f, args, 3);
      f->eax = sys_read(args[0], (void *)args[1], (unsigned)args[2]);
      break;
    case SYS_WRITE:
      get_args(f, args, 3);
      f->eax = sys_write(args[0], (const void *)args[1], (unsigned)args[2]);
      break;
    case SYS_SEEK:
      get_args(f, args, 2);
      sys_seek(args[0], args[1]);
      break;
    case SYS_TELL:
      get_args(f, args, 1);
      f->eax = sys_tell(args[0]);
      break;
    case SYS_CLOSE:
      get_args(f, args, 1);
      sys_close(args[0]);
      break;
    default:
      sys_exit(-1);
  }
}

/* Collin Vossman - Project 2 */


void
syscall_halt(void)
{
  shutdown_power_off();
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
  process_wait(pid);
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
  int file_size;
  struct process_file* pfile;
  
  // Get process file.
  pfile = get_process_file(fd);

  // Check if file is NULL
  if (pf == NULL) return -1;
  
  // Check if file is NULL
  if (pf->file == NULL) return -1;
  
  return file_length(pfile->file);
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
  // Get the corresponding process file
  struct process_file* pf;
  if((pf = get_process_file(fd)) == NULL) return;

  // LOCK while we check if file is valid.
  lock_acquire(&(pf->file_lock));
  if(pf->file == NULL)
  {
    lock_release(&(pf->file_lock));
    return;
  }

  // SEEK
  file_seek(pf->file, position);
  lock_release(&(pf->file_lock));
}

unsigned
sys_tell(int fd)
{
  // Get the corresponding process file
  struct process_file* pf;
  if((pf = get_process_file(fd)) == NULL) return(-1); // Check get_process_file

  // LOCK while checking if valid file.
  lock_acquire(&pf->file_lock);
  if(pf->file == NULL)
  {
    lock_release(&(pf->file_lock));
    return -1;
  }
  
  // TELL
  off_t pos = file_tell(pf->file);
  lock_release(&pf->file_lock);
  return pos;
}

void
sys_close(int fd)
{

}

/* EXTRA FUNCTIONS */

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

static struct process_file* get_process_file(int file_descriptor) 
{

}


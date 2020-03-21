#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int
sys_set_edx(void)
{
  int value;
  if(argint(0, &value) < 0)
    return -1;
  myproc()->tf->edx = value;
  cprintf("edx value = %d\n", myproc()->tf->edx);
  return 0;
}

int
sys_read_registers(void)
{
  cprintf("Trap frame Registers Value :\n");
  cprintf("edi : %d \n",  myproc()->tf->edi);
  cprintf("esi : %d \n",  myproc()->tf->esi);
  cprintf("ebp : %d \n",  myproc()->tf->ebp);
  cprintf("oesp : %d \n",  myproc()->tf->oesp);
  cprintf("ebx : %d \n",  myproc()->tf->ebx);
  cprintf("edx : %d \n",  myproc()->tf->edx);
  cprintf("ecx : %d \n",  myproc()->tf->ecx);
  cprintf("eax : %d \n",  myproc()->tf->eax);
  return 0;
}

int
sys_print_syscalls(void)
{
  return print_syscalls();
}

int
wait_to_alarm(int ticks_duration)
{
  int start_tick = sys_uptime();

  int now;

  while (1)
  {
    now = sys_uptime();
    if (now - start_tick == ticks_duration)
      break;
  }

  cprintf("Timer Alarm !!!!\n%d seconds passed.\n", ticks_duration / 100);
  return SUCCESS_CODE;
}

int
sys_set_alarm(void)
{
  int seconds;

  if (argint(0, &seconds) < 0)
    return ERROR_CODE;

  int ticks_duration = seconds * 100;

  // int parent_pid = sys_getpid();
  int child_pid = sys_fork();

  if (child_pid < 0)
  {
    cprintf("sysproc.c::sys_set_alarm::fork new process failed!\n");
    return ERROR_CODE;
  }
  else if (child_pid == 0)
  {
    cprintf("Child process\n");
    // kill(parent_pid);
    wait_to_alarm(ticks_duration);
  }

  wait();
  return SUCCESS_CODE;
}

int
sys_count_num_of_digits(void)
{
  int number = myproc()->tf->edx;
  int count = 0;
  if (number == 0)
  {
    count++;
  }
  else
  {
    while(number != 0)
    {
      count++;
      number /= 10;
    }

  }
  cprintf("number digits : %d \n",  count);
  return 0;
}
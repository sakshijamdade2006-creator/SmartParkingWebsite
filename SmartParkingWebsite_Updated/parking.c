#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SLOTS 12
#define QMAX 100
#define PLATE 24
#define FILE_NAME "parking_data.txt"
typedef struct { char plate[PLATE]; int occupied; } Slot;
typedef struct { char plate[PLATE]; } Item;
static Slot slots[SLOTS]; static Item q[QMAX]; static int front=0, count=0;
static void norm(char *s){for(int i=0;s[i];i++)s[i]=(char)toupper((unsigned char)s[i]);}
static int free_slot(void){for(int i=0;i<SLOTS;i++)if(!slots[i].occupied)return i;return -1;}
static int find_plate(const char *p){for(int i=0;i<SLOTS;i++)if(slots[i].occupied&&!strcmp(slots[i].plate,p))return i;return -1;}
static int waiting(const char *p){for(int i=0;i<count;i++)if(!strcmp(q[(front+i)%QMAX].plate,p))return 1;return 0;}
static int enqueue(const char *p){if(count>=QMAX)return 0;snprintf(q[(front+count)%QMAX].plate,PLATE,"%s",p);count++;return 1;}
static int dequeue(char *p){if(!count)return 0;snprintf(p,PLATE,"%s",q[front].plate);front=(front+1)%QMAX;count--;return 1;}
static void load(void){FILE*f=fopen(FILE_NAME,"r");if(!f)return;char t,p[PLATE];int n;while(fscanf(f," %c",&t)==1){if(t=='P'){if(fscanf(f,"%d %23s",&n,p)!=2)break;if(n>=0&&n<SLOTS){slots[n].occupied=1;snprintf(slots[n].plate,PLATE,"%s",p);}}else if(t=='W'){if(fscanf(f,"%23s",p)!=1)break;enqueue(p);}}fclose(f);}
static void save(void){FILE*f=fopen(FILE_NAME,"w");if(!f)return;for(int i=0;i<SLOTS;i++)if(slots[i].occupied)fprintf(f,"P %d %s\n",i,slots[i].plate);for(int i=0;i<count;i++)fprintf(f,"W %s\n",q[(front+i)%QMAX].plate);fclose(f);}
static void print_state(void){int occ=0;for(int i=0;i<SLOTS;i++)occ+=slots[i].occupied;printf("{\"total\":%d,\"occupied\":%d,\"available\":%d,\"waitingCount\":%d,\"slots\":[",SLOTS,occ,SLOTS-occ,count);for(int i=0;i<SLOTS;i++){if(i)printf(",");printf("{\"id\":%d,\"occupied\":%s,\"plate\":\"%s\"}",i+1,slots[i].occupied?"true":"false",slots[i].occupied?slots[i].plate:"");}printf("],\"vehicles\":[");int first=1;for(int i=0;i<SLOTS;i++)if(slots[i].occupied){if(!first)printf(",");printf("{\"slot\":%d,\"plate\":\"%s\"}",i+1,slots[i].plate);first=0;}printf("],\"queue\":[");for(int i=0;i<count;i++){if(i)printf(",");printf("\"%s\"",q[(front+i)%QMAX].plate);}printf("]}\n");}
int main(int argc,char**argv){load();if(argc<2){fprintf(stderr,"command required");return 2;}char plate[PLATE];if(!strcmp(argv[1],"state")){print_state();return 0;}if(argc<3){printf("{\"ok\":false,\"message\":\"Vehicle number required\"}");return 0;}snprintf(plate,PLATE,"%s",argv[2]);norm(plate);
 if(!strcmp(argv[1],"add")){int requested=-1;if(argc>=4){char *end=NULL;long v=strtol(argv[3],&end,10);if(!end||*end||v<0||v>SLOTS){printf("{\"ok\":false,\"message\":\"Choose a valid parking slot\"}");return 0;}if(v>0)requested=(int)v-1;}if(!plate[0])printf("{\"ok\":false,\"message\":\"Enter a vehicle number\"}");else if(find_plate(plate)>=0||waiting(plate))printf("{\"ok\":false,\"message\":\"Vehicle already exists\"}");else if(requested>=0){if(slots[requested].occupied)printf("{\"ok\":false,\"message\":\"Slot %d is already occupied. Please choose another slot.\"}",requested+1);else{slots[requested].occupied=1;snprintf(slots[requested].plate,PLATE,"%s",plate);save();printf("{\"ok\":true,\"message\":\"Vehicle parked in selected Slot %d\"}",requested+1);}}else{int n=free_slot();if(n>=0){slots[n].occupied=1;snprintf(slots[n].plate,PLATE,"%s",plate);save();printf("{\"ok\":true,\"message\":\"Vehicle parked in slot %d\"}",n+1);}else if(enqueue(plate)){save();printf("{\"ok\":true,\"message\":\"Parking full; vehicle added to waiting queue\"}");}else printf("{\"ok\":false,\"message\":\"Waiting queue full\"}");}}
 else if(!strcmp(argv[1],"exit")){int n=find_plate(plate);if(n<0)printf("{\"ok\":false,\"message\":\"Vehicle not found in parking\"}");else{slots[n].occupied=0;slots[n].plate[0]=0;char next[PLATE];if(dequeue(next)){slots[n].occupied=1;snprintf(slots[n].plate,PLATE,"%s",next);printf("{\"ok\":true,\"message\":\"Vehicle exited; %s assigned to slot %d\"}",next,n+1);}else printf("{\"ok\":true,\"message\":\"Vehicle exited; slot %d is available\"}",n+1);save();}}
 else printf("{\"ok\":false,\"message\":\"Unknown command\"}");return 0;}

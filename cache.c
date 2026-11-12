#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;
typedef struct cache_buf{
	bool valid_bit;
  uintptr_t tag;
	
  unsigned char byte[64];
	
}cache_buf;
cache_buf *cache=0;
int size_set=0;
void cycle_increase(int n) { cycle_cnt += n; }
int associativity_wid=0;
// TODO: implement the following functions

void is_cache_miss(uintptr_t addr,uintptr_t* buf_start){
	  
    uintptr_t ways = (uintptr_t)1 << associativity_wid;
    uintptr_t buf_num = addr / 64;
    uintptr_t index = buf_num % size_set;
		uintptr_t tag = buf_num / size_set;
		uintptr_t set_start = index * ways;
		for(uintptr_t i=0;i < ways;i++){
		    if(tag==cache[set_start+i].tag){
	  			  *buf_start=set_start+i;
					 return ;
				}
		}
		uint32_t add_buf=set_start+rand() % ways;
    mem_read(buf_num,(uint8_t *)cache[add_buf].byte);
    cache[add_buf].valid_bit=true;		
		 cache[add_buf].tag=tag;
    *buf_start = add_buf; 
		return ;
}
uint32_t cache_read(uintptr_t addr) {
  uintptr_t buf_start=0;
  is_cache_miss(addr,&buf_start);
	uintptr_t offset=addr&0x3F;
	unsigned char c1=cache[buf_start].byte[(offset&0x3C)+0];
	
	unsigned char c2=cache[buf_start].byte[(offset&0x3C)+1];
	unsigned char c3=cache[buf_start].byte[(offset&0x3C)+2];
	unsigned char c4=cache[buf_start].byte[(offset&0x3C)+3];
	uint32_t result=((uint32_t)c4<<24)|((uint32_t)c3<<16)|((uint32_t)c2<<8)|((uint32_t)c1);
	return result;
  return 0;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask) {
	  uint8_t *p=(uint8_t*)&data;
		uintptr_t buf_start=0;
    is_cache_miss(addr,&buf_start);
    uintptr_t offset=addr&0x3F;
		cache[buf_start].byte[(offset&0x3C)+0]=(cache[buf_start].byte[(offset&0x3C)+0]&~(uint8_t)(wmask>>0))|(p[0]&(uint8_t)(wmask>>0));
    cache[buf_start].byte[(offset&0x3C)+1]=(cache[buf_start].byte[(offset&0x3C)+1]&~(uint8_t)(wmask>>8))|(p[1]&(uint8_t)(wmask>>8));
		cache[buf_start].byte[(offset&0x3C)+2]=(cache[buf_start].byte[(offset&0x3C)+2]&~(uint8_t)(wmask>>16))|(p[2]&(uint8_t)(wmask>>16));
		cache[buf_start].byte[(offset&0x3C)+3]=(cache[buf_start].byte[(offset&0x3C)+3]&~(uint8_t)(wmask>>24))|(p[3]&(uint8_t)(wmask>>24));
		mem_write(addr/64,(uint8_t *)cache[buf_start].byte);
}
uintptr_t size_buf(int power){
    uintptr_t result=1;
		for(int i=0;i<power;i++){
		   result=result*2;
		}
		
		return result;
}
void init_cache(int total_size_width, int associativity_width) {
    if(total_size_width<6){return;}
		cache=(cache_buf*)malloc(size_buf(total_size_width)*(sizeof(cache_buf)+sizeof(bool)*(32-6-associativity_width)));
    size_set = size_buf(total_size_width) / 64 / size_buf(associativity_width);
		for(uintptr_t i=0;i<size_buf(total_size_width);i++){
		   cache[i].valid_bit=false;
		}
		associativity_wid=associativity_width;
		return ;
}

void display_statistic(void) {
}

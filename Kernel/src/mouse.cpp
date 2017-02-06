#include <stdtype.h>
#include <system.h>

#include <mouse.h>

namespace mouse{
	
	uint8_t mouseCycle = 0;
	int8_t mouseByte[3];
	int8_t x;
	int8_t y;
	
	void Handler(struct regs32* regs){
		switch(mouseCycle){
			case 0:
				mouseByte[0]=inportb(0x60);
				mouseCycle++;
				break;
			case 1:
				mouseByte[1]=inportb(0x60);
				mouseCycle++;
				break;
			case 2:
				mouseByte[2] = inportb(0x60);
				x = mouseByte[1];
				y = mouseByte[2];
				break;
		}
	}
	
	inline void Wait(uint8_t type){
		uint32_t timeout=100000;
		
		if(type == 0){
			while(timeout--){
				if((inportb(0x64)&1)==1){
					return;
				}
			}
			return;
		} else {
			while(timeout--){
				if((inportb(0x64)&2)==0){
					return;
				} 
			}
			return;
		}
	}
	
	inline void Write(uint8_t data){
		Wait(1);
		outportb(0x64,0xD4);
		Wait(1);
		outportb(0x60,data);
	}
	
	uint8_t Read(){
		Wait(0);
		return inportb(0x60);
	}
	
	void Init(){
		uint8_t status;
		
		Wait(1);
		outportb(0x64,0xA8);
		
		Wait(1);
		outportb(0x64,0x20);
		Wait(0);
		status = (inportb(0x60) | 2);
		Wait(1);
		outportb(0x64,0x60);
		Wait(1);
		outportb(0x60,status);
		
		Write(0xF6);
		Read();
		
		InstallIRQHandler(12,Handler);
	}
	
	Vector2 GetPos(){
		Vector2 vec;
		vec.x = x;
		vec.y = y;
	}
}
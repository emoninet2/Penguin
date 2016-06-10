/*
 * nrf24l01p_mesh.h
 *
 * Created: 01-Jun-16 7:10:28 PM
 *  Author: emon1
 */ 


#ifndef NRF24L01P_MESH_H_
#define NRF24L01P_MESH_H_





#define nrf14l01p_node_address  0x4141414140
uint64_t nrf24l01p_reachable_addresses[6];


void nrf24l01p_mesh_init(){
	
	//only for for main host
	for(int i=0;i<6;i++){
		nrf24l01p_reachable_addresses[i] = nrf14l01p_node_address + i;
	}
}




static void nrf24l01p_mesh_send(uint64_t address, uint8_t * data){
	uint8_t pack[33];
	
	for(int i=0;i<5;i++){
		pack[i] = (uint8_t*)(address>>(8*i));
	}
	
	for(int i=5;i<32;i++){
		pack[i] = *(data +i-5);
	}


	//printf("%s\r\n", pack);
	int known_address_cnt = sizeof(nrf24l01p_reachable_addresses)/sizeof(nrf24l01p_reachable_addresses[0]);
	for(int i=0;i<known_address_cnt;i++){
		_nrf24l01p_send_to_address_ack(nrf24l01p_reachable_addresses[i],pack,32);
	}
}



void mesh_pack_forward_test(uint8_t *data){
	
	uint64_t address = 0;
	uint8_t *packdata = &data[5];
	
	memcpy(&address, data, 5); //or swap the parameters


	bool pack_address_match = 0;
	
	int known_address_cnt = sizeof(nrf24l01p_reachable_addresses)/sizeof(nrf24l01p_reachable_addresses[0]);
	
	
	if(address == nrf14l01p_node_address){
		//if address matches its own node address
		PORTR.OUTTGL = (1<<1);
	}
	else{
		//check if address matches reachable package
		bool hop_packet_out = 0;
		for(int i=1;i<6;i++){ 
			if (nrf24l01p_reachable_addresses[i] - address == 0){
				hop_packet_out = 1;
				asm("nop");
			}
		}
		
		if(!hop_packet_out){
			//forward packet to reachable addresses
			_nrf24l01p_send_to_address_ack(address, packdata,strlen(packdata));
			PORTR.OUTTGL = (1<<0);
		}else{
			//discard packet
		}
	}
	
	
	
	



	
	
}



#endif /* NRF24L01P_MESH_H_ */
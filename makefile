CC = gcc
OBJ= get_sn.o get_temperature.o socket.o main.o tlv_unpack.o log.o init_db.o tlv_pack.o sqlite3.o crc-itu-t.o tlv_dns.o
OUTPUT = sock_tlv_client
ALL: ${OBJ}
	    ${CC} ${CFLAGS}${OBJ}  -o ${OUTPUT} -lpthread -ldl ${LDFLAGS}
.PHONY: clean 

clean:
	    rm -rf *.o ${OUTPUT}
disclean: 
	rm -rf *.o ${OUTPUT} tags cscope* tlv_message.log*

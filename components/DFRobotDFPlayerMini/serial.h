#ifndef __UART_H__
#define __UART_H__

void serial_begin(int baudrate, int txd, int rxd);
void serial_end(void);
int serial_available(void);
uint8_t serial_read(void);
uint8_t serial_peek(void);
void serial_print(char *ch);
void serial_println(char *ch);
void serial_write(uint8_t ch);
void serial_write_buffer(uint8_t *ch, int len);
void serial_flash();

#endif

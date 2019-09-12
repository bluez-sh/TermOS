#ifndef DRIVERS_PORTS_H
#define DRIVERS_PORTS_H

unsigned char port_r8(unsigned short port);
void port_w8(unsigned short port, unsigned char data);
unsigned short port_r16(unsigned short port);
void port_w16(unsigned short port, unsigned short data);

#endif

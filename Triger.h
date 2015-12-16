#ifndef Triger_h
#define Triger_h

#define IOPORT_BASE 0x378
#define IOPORT_DATA (IOPORT_BASE + 0)
#define IOPORT_STATUS (IOPORT_BASE + 1)
#define IOPORT_CONTROL (IOPORT_BASE + 2)

class Triger{
public:
	void triger_on();
	void triger_off();
};

#endif
this is a generic interrupt driver(kernel module).
it allows userspace program to register for a signal via ioctl for a specific interrupt, when interrupt event happens, this module would send the registered signal to the userspace program's pid


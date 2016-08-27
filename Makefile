makeshell: comms.c parse.c test.c utils.c shell_pipe.c headers.h limits.h
	gcc shell_pipe.c -lcurl -lreadline
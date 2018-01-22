# TPE_SO_Sockets
# El TP de los pibes de zona sur
# Dependencies needed fedora yum install sqlite-devel or in ubuntu apt-get install libsqlite3-dev
# Compile Server gcc -I 'include/' semaphores.c serverCore.c serverMain.c commandParser.c databaseCore.c clientHandlerCore.c serializeManager.c deserializeManager.c -lsqlite3 -pthread --pedantic
# Compile Client gcc -I 'include/' clientCore.c clientMain.c serializeManager.c serverHandlerCore.c deserializeManager.c --pedantic -pthread

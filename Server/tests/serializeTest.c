void testSerialSimpleCMD();
void printSimpleCMD(simpleCommand * cmd);

void testSerialSimpleCMD(){
	char * bytes_aux;
	int cmd_size;
	simpleCommand n_simpleCmd = {.command = 7, .extra = 7};
	simpleCommand simpleCmd = {.command = 65, .extra = 777};

	bytes_aux = serializeSimpleCommand(&simpleCmd, &cmd_size);
	printf("SERIALIZE DONE TOT SIZE:  %d\n ORIGINAL: \n", cmd_size);
	printSimpleCMD(&simpleCmd);
	if(deserializeSimpleCommand(bytes_aux, cmd_size, &n_simpleCmd) != DESERIALIZE_OK){
		printf("ERROR DESERIALIZE");
		return;
	}
	printf("DESERIALIZE: \n", cmd_size);
	printSimpleCMD(&n_simpleCmd);
}

void printSimpleCMD(simpleCommand * cmd){
	fprintf(stdout,"struct in : %p\nCommand: %c (at %p)\n", cmd, cmd->command, &(cmd->command));
	fprintf(stdout,"Extra int: %d (at %p)\n", cmd->extra, &(cmd->extra));
} 

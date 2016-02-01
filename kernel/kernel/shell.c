#include <kernel/shell.h>
#include <string.h>
#include <stdio.h>

char *read_command();
void process_command(char *cmd);

#define INFO_CMD "info"
#define HELP_CMD "help"

void shell()
{
  while(1)
  {
    char *cmd[50];
    printf("sh> ");
    gets(&cmd);
    printf("\n");
    process_command(cmd);
  }
}

void process_command(char *cmd)
{
  // Spew out some nonsense till we are done interfacing with the BIOS
  if (memcmp(cmd, INFO_CMD, strlen(cmd)) == 0) {
    printf("Here's some info about your current system:\n");
    printf("Available memory: %d\n", 32768);
    printf("Processor speed: 3.2Ghz\n");
    return;
  }
  if (memcmp(cmd, HELP_CMD, strlen(cmd)) == 0) {
    printf("Available commands are: %s, %s\n", HELP_CMD, INFO_CMD);
    return;
  }

  printf("Unknown command %s\n", cmd);
}

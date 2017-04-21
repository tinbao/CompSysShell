/***************************************************************************//**
  A program simulating the use of a shell, in C.
  @author       Stephen Brennan (https://github.com/brenns10/lsh)
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void lsh_loop();
int lsh_launch(char **args);
int lsh_execute(char **args);
char *lsh_read_line();
char **lsh_split_line(char *line);

int main(int argc, char **argv) {

  // Will loop over and over for user input
  lsh_loop();

  // Cleanup and housekeeping
  return EXIT_SUCCESS;
}

/*
 * The main loop waiting for user input
 */
void lsh_loop() {
  char *line;
  char **args;
  int status;

  do {
    printf("> ");
    line = lsh_read_line();
    args = lsh_split_line(line);
    status = lsh_execute(args);

    free(line);
    free(args);
  } while (status);

}

/*
 * The launcher that intialises the forking of the first process
 */
int lsh_launch(char **args) {
  pid_t pid, wpid;
  int status;
  pid = fork();

  if(pid == 0) {

    //printf("Fork failed\n");
    return EXIT_FAILURE;

  } else if (pid < 0) {
    // Error forking
    //printf("Fork failed\n");
    perror("lsh");

  } else {
    //printf("Fork successful\n");
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

/*
 * The code that executes the system commands
 */
int lsh_execute(char **args) {
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }
 /*
  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }*/

  return lsh_launch(args);
}

/*
 * Reads the line input
 */
char *lsh_read_line() {
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_split_line(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

/*
 * The exit command for the shell
 */
int lsh_exit(char **args)
{
  return 0;
}

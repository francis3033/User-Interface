/*Francis Otoo, 117890242, fotoo*/
#include <stdio.h>
#include <string.h>
#include "document.h"
#include <ctype.h>
#include <stdlib.h>
#include <sysexits.h>

#define COMMAND_LINE 1024
#define MAX_FUNCTION_CALL 19

/* This function loads a file with the specified name into the doc */
int load_file(Document *doc, const char *filename) {  
  FILE *file;
  char file_line[MAX_STR_SIZE + 1] = "";
  int paragraph_counter = 0, flag = 1, i = 0;

  /*Check if doc or filename is NULL or not*/
 if (doc == NULL || filename == NULL) {
    return FAILURE;
  }
 
  /*Open file with given name*/
  file = fopen(filename, "r");

  /*Check if opening the file worked and if doc is not at max for paragraphs*/
  if (file == NULL || doc->number_of_paragraphs >= MAX_PARAGRAPHS) {
    return FAILURE;
  }
  /* First create a new paragraph */
  add_paragraph_after(doc, paragraph_counter);
  paragraph_counter++;

  /* Iterate through all the lines in the doc */
  while (fgets(file_line, MAX_STR_SIZE + 1, file)) {
    /* Make sure there's a nulbyte at the end to get rid of the '\n' */
    file_line[strlen(file_line) - 1] = '\0';

    /* Go through the whole line to find out if there are any characters in the line */
    for (i = 0; i < strlen(file_line); i++) {
      if (!isspace(file_line[i])) {
	flag = 0;
      }
    }
    /* Check if we need to add a new paragraph or not */
    if (flag) {
      if (add_paragraph_after(doc, paragraph_counter) == FAILURE) {
	return FAILURE;
      }
      paragraph_counter++;

      /* If we're not adding a new paragraph we have to add a line to the respective paragraph */
    } else {

      append_line(doc, paragraph_counter, file_line);
      flag = 1;
    }
  }
  return SUCCESS;
}


/* Same logic as print document without the header but instead of printing it 
write it to the file thats given. */
int save_document(Document *doc, const char *filename) {
  FILE *save_file;  
  int i, j;

  /* NULL check for parameters */
  if (doc == NULL || filename == NULL) {
    return FAILURE;
  }

 
  save_file = fopen(filename, "w");
  /* Make sure that the fopen didn't fail */
  if (save_file == NULL) {
    return FAILURE;
  }

  /* Go through each paragraph */
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    /* Go through each line for each paragraph */
    for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      /* Write each line into the file that was given */
      fputs(doc->paragraphs[i].lines[j], save_file);
      fputs("\n", save_file);
  }

    /* Only add an extra line at the end of a paragraph if not the last paragraph and at
       end of a line in a paragraph */
    if (j == doc->paragraphs[i].number_of_lines &&
	i < doc->number_of_paragraphs - 1 &&
	doc->paragraphs[i].number_of_lines != 0) {
      fputs("\n", save_file);
    }
  }

  return SUCCESS;
}


int main(int argc, char *argv[]) {
  Document doc;
  FILE *new_file;
  char user_commands[COMMAND_LINE + 1] = "";
  char function_call[MAX_FUNCTION_CALL + 1] = "";
  char temp_add_line[MAX_STR_SIZE + 1] = "", temp_append_line[MAX_STR_SIZE + 1] = "", filename[MAX_STR_SIZE] = ""; 
  int paragraph_num = -1, line_num = -1, temp_variables = -1, flag = 1;
  char garbage[MAX_STR_SIZE] = "";
  char testing_command[MAX_STR_SIZE] = "";
 
  init_document(&doc, "main_document");
  if (argc == 2) {   
    new_file = fopen(argv[1], "r");

    /* Check if openeing the file worked or not */
    if (new_file == NULL) {
      fprintf(stderr, "%s cannont be opened.\n", argv[1]);
      exit(EX_OSERR);
    }
  } else if (argc == 1){
    new_file = stdin;

    /* Check if there are extra arguments passed in */
  } else if (argc > 2) {
    fprintf(stderr, "Usage: user_interface\nUsage: user_interface <filename>\n");
    exit(EX_USAGE);
  }

  /*Get all the lines in the file I opened until I get to the end*/
  while(fgets(user_commands, 1024 + 1, new_file) && flag) {
    if (new_file == stdin) {
    printf("> ");
    }

    if(user_commands[0] != '\n') {

      /* If first character is a #, then ignore because it's a comment */
      sscanf(user_commands, "%s", function_call);
      if (function_call[0] != '#') {

	/* ADD_PARAGRAPH_AFTER */
	if (strcmp(function_call, "add_paragraph_after") == 0) {

	/*Get the paragraph number to call the function*/	
	  temp_variables = sscanf(user_commands, "%s %d %s", testing_command, &paragraph_num, garbage);
	  /*Call the function after getting the parameters*/
      
	  if (temp_variables == 2 && paragraph_num >= 0) {
	    if (add_paragraph_after(&doc, paragraph_num) == FAILURE) {
	      printf("add_paragraph_after failed\n");	     
	    }  
	  } else { 
	    printf("Invalid Command\n");
	  }


	  /* ADD_LINE_AFTER */
	  /* FIND '*' and make pointer to that to copy the line */
	} else if (strcmp(function_call, "add_line_after") == 0) {
	  /* Get a temp char pointer to find the line that has to be added. */
	  char *p;
	  temp_variables = sscanf(user_commands, "%s %d %d %s", testing_command, &paragraph_num, &line_num, temp_add_line);

	 /* Checking for '*' */
	 if (temp_add_line[0] == '*' && temp_variables == 4 && paragraph_num > 0 && line_num >= 0) {
	   p = strchr(user_commands, '*');

	   p++;
	   p[strlen(p) - 1] = '\0';
	   if (add_line_after(&doc, paragraph_num, line_num, p) == FAILURE) {
	     printf("add_line_after failed\n");
	   }
	 } else {
	   printf("Invalid Command\n");
	 }

    

	  /* PRINT_DOCUMENT */      
	} else if (strcmp(function_call, "print_document") == 0) {
	  /* Set a garbage string to pickup anything after the function call */
	  temp_variables = sscanf(user_commands, "%s %s", testing_command, garbage);
	 
	  /* Make sure there was nothing picked up from the garbage value */
	  if (temp_variables == 1) {
	  print_document(&doc);
	  } else {
	    printf("Invalid Command\n");
	  }


	  /* QUIT AND EXIT */
	} else if ((strcmp(function_call, "quit") == 0) || (strcmp(function_call, "exit") == 0)) {
	  temp_variables = sscanf(user_commands, "%s %s", testing_command, garbage);

          /* Make sure there was nothing picked up from the garbage value */
          if (temp_variables == 1) {
	    /* Change the flag variable to false to break out of while loop */
	    flag = 0;
          } else {
	    printf("Invalid Command\n");
	  } 


	  /* APPEND_LINE */
	} else if(strcmp(function_call, "append_line") == 0) {
	  char *q;
	  /* Same logic as the add_line_after method */
	  temp_variables = sscanf(user_commands, "%s %d %s", testing_command, &paragraph_num, temp_append_line);


	  /* Checking for '*' */
	  if (temp_append_line[0] == '*' && temp_variables == 3 && paragraph_num > 0) {
	    /* Get a temp char pointer to find the line that has to be added. */
	    q = strchr(user_commands, '*');


	    q++;
	    q[strlen(q) - 1] = '\0';
	    /* Add the line after the '*' is found and dont include the '\n' */
	    if (append_line(&doc, paragraph_num, q) == FAILURE) {
	      printf("append_line_ failed\n");
	    }
	  } else {
	    printf("Invalid Command\n");
	  }


	  /* REMOVE_LINE */
	} else if (strcmp(function_call, "remove_line") == 0) {
	  /* Get the paragraph number and line number to call the function */
          temp_variables = sscanf(user_commands, "%s %d %d %s", testing_command, &paragraph_num, &line_num, garbage);

          if (temp_variables == 3 && paragraph_num > 0 && line_num > 0) {
            if (remove_line(&doc, paragraph_num, line_num) == FAILURE) {
              printf("remove_line failed\n");
            }
          } else {
            printf("Invalid Command\n");
          }


	  /* LOAD_FILE */
	} else if (strcmp(function_call, "load_file") == 0) {
	  temp_variables = sscanf(user_commands, "%s %s %s", testing_command, filename, garbage);

	  if (temp_variables == 2) {
	    if (load_file(&doc, filename) == FAILURE) {
	      printf("load_file failed\n");
	    }
	  } else {
	    printf("Invalid Command\n");
	  }
	  /* Reset function call so it doesn't try to load another file */
	  strcpy(function_call, "");

	  /* REPLACE_TEXT */
	} else if (strcmp(function_call, "replace_text") == 0) {
	  int target_quote_count = 0, replace_quote_count = 0, target_len = 0, replace_len = 0;
	  char *t = strstr(user_commands, "\""), *r; /* Get to the first " for target */
	  char replacement[MAX_STR_SIZE + 1] = "", target[MAX_STR_SIZE + 1] = "";
	  char temp_replace[2] = "", temp_target[2] = ""; /* Short string to use it for strcat */
	  
	  /* Make sure to check if strstr worked or not */
	  if (t != NULL) {
	    /* Keep going down the string to find the 2 quotes for target or til the end */
	    while (*t != '\0' && target_quote_count != 2) {
	      if (*t == '\"') {
		target_quote_count++;
	      } else {
		/* While I'm going through the string I want to store the target variable in a string as well */
		temp_target[0] = t[0];
		strcat(target, temp_target);
		target_len++;
	      }
	      t++;
	    }
	  } 
	    /* We broke out this loop for two possible reasons */
	    /* 1: We actually found 2 quotes */
	    /* 2: We reached the end of the user_command line without seeing a quote */


	    /* We only want to be in this if statement if both quotes were found --> Now I have to look for 2 quotes for replacement */
	    if (target_quote_count == 2) {

	      /* I want to start q where the next quote starts */	     
	      r = strstr(t + 1, "\"");

	      /* Make sure if strstr actually worked or not */
	      if (r != NULL) {
	      /* Same process to find out if the replacement has 2 quotes */
		while (*r != '\0' && replace_quote_count != 2) {
		  if (*r == '\"') {
		    replace_quote_count++;
		  } else {
		    /* While I'm going through the string I want to store the replace variable in a string as well */
		    temp_replace[0] = r[0];
		    strcat(replacement, temp_replace);		   
		    replace_len++;
		  }
		  r++;
		}
	      }
	    }

	    /* This means if we have a target and a replacement*/
	    if (target_quote_count == 2 && replace_quote_count == 2) {
	      if (replace_text(&doc, target, replacement) == FAILURE) {
		printf("replace_text failed\n");
	      }
	    } else {
	      printf("Invalid Command\n");
	    }


	    /* HIGHLIGHT_TEXT */
	    /* Same logic as trying to find a target or replacement */
	} else if (strcmp(function_call, "highlight_text") == 0) {
	  int high_quote_count = 0, high_len = 0;
          char *ht = strstr(user_commands, "\""); /* Get to the first " for target */
          char highlight[MAX_STR_SIZE + 1] = "";
          char temp_high[2] = ""; /* Short string to use it for strcat */

	  /* Make sure to check if strstr worked or not */
          if (ht != NULL) {
            /* Keep going down the string to find the 2 quotes for target or til the end */
            while (*ht != '\0' && high_quote_count != 2) {
              if (*ht == '\"') {
                high_quote_count++;
              } else {
                /* While I'm going through the string I want to store the target variable in a string as well */
                temp_high[0] = ht[0];
                strcat(highlight, temp_high);
                high_len++;
              }
              ht++;
            }
          }

	  /* This means if we have a target */
	  if (high_quote_count == 2) {
	    highlight_text(&doc, highlight);
	  } else {
	    printf("Invalid Command\n");
	  }


	  /* REMOVE_TEXT */
	  /* Same logic as highlight */
	} else if (strcmp(function_call, "remove_text") == 0) {
	  int remove_quote_count = 0, remove_len = 0;
          char *rt = strstr(user_commands, "\""); /* Get to the first " for target */
          char remove[MAX_STR_SIZE + 1] = "";
          char temp_remove[2] = ""; /* Short string to use it for strcat */

          /* Make sure to check if strstr worked or not */
          if (rt != NULL) {
            /* Keep going down the string to find the 2 quotes for target or til the end */
            while (*rt != '\0' && remove_quote_count != 2) {
              if (*rt == '\"') {
                remove_quote_count++;
              } else {
                /* While I'm going through the string I want to store the target variable in a string as well */
                temp_remove[0] = rt[0];
                strcat(remove, temp_remove);
                remove_len++;
              }
              rt++;
            }
          }

          /* This means if we have a target */
          if (remove_quote_count == 2) {
            remove_text(&doc, remove);
          } else {
            printf("Invalid Command\n");
          }


	  /* SAVE_DOCUMENT */
	  /* Same logic as load_file */
	} else if (strcmp(function_call, "save_document") == 0) {
	  temp_variables = sscanf(user_commands, "%s %s %s", testing_command, filename, garbage);

          if (temp_variables == 2) {
            if (save_document(&doc, filename) == FAILURE) {
              printf("save_document failed\n");
            }
          } else {
            printf("Invalid Command\n");
          }

          /* Reset function call so it doesn't try to save another file */
          strcpy(function_call, "");


	  /* RESET_DOCUMENT */
	} else if (strcmp(function_call, "reset_document") == 0) {
	  temp_variables = sscanf(user_commands,"%s %s", testing_command, garbage);
	  if (temp_variables == 1) {
	    reset_document(&doc);
	  } else {
	    printf("Invalid Command\n");
	  }
	}
      }
    }
  }
  return 0;
}

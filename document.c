/*Francis Otoo, 117890242, fotoo*/
#include <stdio.h>
#include <string.h>
#include "document.h"

/*Initialize new doc*/
int init_document(Document *doc, const char *name) {
  if (doc == NULL || name == NULL) {
    return FAILURE;
  }
  /*Cant check this unless I know name isn't null, could possibly give errors*/
  if (strlen(name) > MAX_STR_SIZE) {
    return FAILURE;
  }
  /*Add name for doc and change paragraph count to 0.*/
  strcpy(doc->name, name);
  doc->number_of_paragraphs = 0;
  return SUCCESS;
}

/*Change number of paragraphs to 0 to reset doc*/
int reset_document(Document *doc) {
  if (doc == NULL) {
    return FAILURE;
  }
  doc->number_of_paragraphs = 0;
  return SUCCESS;
}

/*Print out all the details of the doc*/
int print_document(Document *doc) {
  int i, j;
  /* char temp[MAX_STR_SIZE + 1];*/
  if (doc == NULL) {
    return FAILURE;
  }
  /*Print doc name and number of paragraphs first*/
  printf("Document name: \"%s\"\n", doc->name);
  printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

  /*Go through each paragraph*/
  for (i = 0; i < doc->number_of_paragraphs; i++) { 
    /*Go through each line of each paragraph*/
    for(j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      /*Print out each line*/
	printf("%s\n", doc->paragraphs[i].lines[j]);
    }
    /*Only add an extra line(space) if not the last paragraph and at
      end of a line in a paragraph*/
    if (j == doc->paragraphs[i].number_of_lines &&
	i < doc->number_of_paragraphs - 1 &&
	doc->paragraphs[i].number_of_lines != 0) {
      printf("\n");
    }
  }
  return SUCCESS;
}

/*Add new paragraph after given paragraph number*/
int add_paragraph_after(Document *doc, int paragraph_number) {
  int i;
  if (doc == NULL) {
    return FAILURE;
  }
  /*Make sure the doc is not at limit, and
    also paragraph number is in available space*/
  if ((doc->number_of_paragraphs >= MAX_PARAGRAPHS) ||
      (paragraph_number > doc->number_of_paragraphs)) {
    return FAILURE;
  }
  /*Iterate through the paragraphs backwards and push everything
    down 1 until you get to the right spot*/
      for (i = doc->number_of_paragraphs - 1; i >= paragraph_number; i--) { 
	doc->paragraphs[i+1] = doc->paragraphs[i];
      } 
      /*set that paragraph num_of_lines to 0*/
      doc->paragraphs[paragraph_number].number_of_lines = 0;
      /*increase num_of_paragraphs*/
      doc->number_of_paragraphs++;
  return SUCCESS;
}

/*Method does the same thing that paragraph adder does, but for lines.*/
int add_line_after(Document *doc, int paragraph_number, int line_number,
		   const char *new_line) {
  int i, j = paragraph_number;
  if (doc == NULL || new_line == NULL) {
  return FAILURE;
  }
 
  if ((paragraph_number > doc->number_of_paragraphs) ||
      (doc->paragraphs[paragraph_number - 1].number_of_lines
      >= MAX_PARAGRAPH_LINES) ||
      (line_number > doc->paragraphs[paragraph_number - 1].number_of_lines)) {
    return FAILURE;
  }

 /*Find the paragraph that needs the line. Add line to correct line of
paragraph.*/
  for (i = doc->paragraphs[paragraph_number - 1].number_of_lines - 1;
       i >= line_number; i--) {
    strcpy(doc->paragraphs[j-1].lines[i + 1],doc->paragraphs[j-1].lines[i]);
  }
  strcpy(doc->paragraphs[j-1].lines[line_number], new_line);
  doc->paragraphs[j-1].number_of_lines++;
  return SUCCESS;
}

/*Get number of lines for a specific paragraph*/
int get_number_lines_paragraph(Document *doc, int paragraph_number,
			       int *number_of_lines) {
  if (doc == NULL || number_of_lines == NULL) {
    return FAILURE;
  }
  if (paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  /*de-reference the parameter with the num_of_lines for a paragraph*/
  *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
  return SUCCESS;
}




/*Returns the number of lines in a doc*/
int get_number_lines(Document *doc, int *number_of_lines) {
  int i;
  if (doc == NULL || number_of_lines == NULL) {
    return FAILURE;
  }
  *number_of_lines = 0;
 /*FIND NUMBER OF LINES IN EACH PARAGRAPH OF DOC THEN ADD THEM TOGETHER*/
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    *number_of_lines += doc->paragraphs[i].number_of_lines;
  }
  return SUCCESS;
}



/*Adds a new line of text to a specific paragraph in a doc*/
int append_line(Document *doc, int paragraph_number, const char *new_line) {
  int line_num;
  if (doc == NULL || new_line == NULL) {
    return FAILURE;
  }
  if (paragraph_number > MAX_PARAGRAPHS ||
      doc->number_of_paragraphs > MAX_PARAGRAPHS || 
     (doc->paragraphs[paragraph_number - 1].number_of_lines
      >= MAX_PARAGRAPH_LINES)) {
    return FAILURE;
      }
  /*Find specific line_num for specific paragraph_number*/
  line_num = doc->paragraphs[paragraph_number - 1].number_of_lines;
  /*Insert new line*/
  strcpy(doc->paragraphs[paragraph_number - 1].lines[line_num], new_line);
  /*Increase number of lines after adding one*/
  doc->paragraphs[paragraph_number - 1].number_of_lines++; 
  return SUCCESS;
}

/*Removes a line from a paragrpah in the doc*/
int remove_line(Document *doc, int paragraph_number, int line_number) {
  int i;
  if (doc == NULL) {
    return FAILURE;
  }
  if (paragraph_number > MAX_PARAGRAPHS ||
      (line_number > doc->paragraphs[paragraph_number - 1].number_of_lines)) {
    return FAILURE;
  }
  /*Find and remove the specified line in the right paragraph*/
  for (i = line_number; i < doc->paragraphs[paragraph_number-1].number_of_lines;
       i++) {
    strcpy(doc->paragraphs[paragraph_number - 1].lines[i - 1],
	   doc->paragraphs[paragraph_number - 1].lines[i]);
  }
  /*Decrease number of lines in that paragraph*/
  doc->paragraphs[paragraph_number - 1].number_of_lines--;
  return SUCCESS;
}

/*Load doc with all the data from the list of lines from data*/
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1], int data_lines){
  int i = 0, paragraph_count = 0, temp_line_count; 
 if (doc == NULL || data == NULL || data_lines == 0) { 
    return FAILURE;
  }
 /*Create a new paragraph first*/
 add_paragraph_after(doc, 0);
 paragraph_count++;

 /*Go through all the lines in the given data*/
  for (i = 0; i < data_lines; i++) {
    /*Check if we need to add a new paragraph*/
    if (data[i][0] == '\0') {
      temp_line_count = 0;
      add_paragraph_after(doc, paragraph_count);
      paragraph_count++;
    }
    /*If we're not adding a new paragraph, then add a new line to the
      right paragraph*/
    if (data[i][0] != '\0') {
      add_line_after(doc, paragraph_count, temp_line_count, data[i]);
      temp_line_count++;
    }
  }
  return SUCCESS;
}

/*Replace target text with a replacement in all lines in a doc*/
int replace_text(Document *doc, const char *target, const char *replacement) {
  char *occurence, *cpy_lines;
  int length, i, j;
  if (doc == NULL || target == NULL || replacement == NULL) { 
    return FAILURE;
  }
  /*Get length after checking if target is null*/
  length = strlen(target);
  /*Loop through all the paragphs*/
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    /*For each paragraph, loop through each line*/
    for (j = 0; j < doc->paragraphs[i].number_of_lines; j++) {
      /*Find where the first time we see the target*/
      occurence = strstr(doc->paragraphs[i].lines[j], target);
      /*Keep checking the same line until there are no more targets*/
      while (occurence != '\0') {
	  char temp[MAX_STR_SIZE + 1] = "";
	  cpy_lines = temp;
	  strcpy(cpy_lines, occurence);
	  strcpy(occurence,"");
	  strcat(occurence, replacement);
	  cpy_lines += length;
	  /*Attach replaced+rest_of_line*/
	    strcat(occurence, cpy_lines);
	  /*Move pointer past the first replaced value*/
	    occurence += strlen(replacement);
	  /*Start looking for another occurence*/
	    occurence = strstr(occurence, target);
      }
    }
  }
  return SUCCESS;
}

/*Highlights a specific target word or phrase in a doc for every line*/
int highlight_text(Document *doc, const char *target) {
  char temp[MAX_STR_SIZE];
  if (doc == NULL || target == NULL) {
  return FAILURE;
  }
  /*Make the temp = [target]*/
  strcpy(temp, HIGHLIGHT_START_STR);
  strcat(temp, target);
  strcat(temp, HIGHLIGHT_END_STR);
  /*Call replace to replace the targets with the temp*/
  replace_text(doc, target, temp);
  return SUCCESS;
} 

/*Removes all target words or phrases in a doc*/
int remove_text(Document *doc, const char *target) {
  if (doc == NULL || target == NULL) {
  return FAILURE;
  }
  /*Call replace to replace all the targets with and empty string,
    so it basically removes it.*/
  replace_text(doc, target, "");
  return SUCCESS;
}


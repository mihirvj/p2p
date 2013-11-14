#include "parse.h"

int parse_line(char out[SIZE], char *line, int line_no, int *method);
int parse_line_1(char out[SIZE], char *line, int *method);
int parse_line_2(char out[SIZE], char *line);
int parse_line_3(char out[SIZE], char *line);
int parse_line_4(char out[SIZE], char *line);

int parse_response(char buf[1000], int index, char rfc[50], char title[50], char host[50], char port[50])
{
  int i=0;
  char *line;
  
  line = strtok(strdup(buf), CRLF);

  for(i=0; i < 5; i++)
  {
    line = strtok(NULL, CRLF);
  }

  for(i=0; i < index && line != NULL; i++)
  {
	line = strtok(NULL, CRLF);
  }

  if(line != NULL)
  {
	line = strtok(NULL, SPACE);

	if(line == NULL)
		return -1;

	strcpy(rfc, line);

	line = strtok(NULL, SPACE);

	strcpy(title, line);

	line = strtok(NULL, SPACE);

	strcpy(host, line);

	line = strtok(NULL, SPACE);

	strcpy(port, line);
  }

  return 1;
}

void generate_request(char req[1000], int req_type, int rfc, char host[50], char port[50], char title[50])
{
// str_replace(title, ' ', '_');

 switch(req_type)
 {
   case GET:
	sprintf(req, GETRFC, rfc, VERSION, host, port, title);
   break;

   case ADD:
	sprintf(req, ADDRFC, rfc, VERSION, host, port, title);
   break;

   case LOOKUP:
	sprintf(req, LOOKUPRFC, rfc, VERSION, host, port, title);
   break;

   case LISTALL:
	sprintf(req, LISTALLRFC, VERSION, host, port);
   break;
 } 
}

int parse_request(char request[1000], char rfc[50], char host[50], char port[50], char title[100], int *method)
{
 char response[1000];

 int errcode = OK;
 char in[SIZE];
 *method = -1;

 errcode = parse_line(in, request, 1, method);

 if(errcode == OK)
	 sprintf(rfc, "%s", in);
 else
 {
	return errcode;
 }

 errcode = parse_line(in, request, 2, method);

 if(errcode == OK)
	sprintf(host, "%s", in);
 else
 {
	return errcode;
 }

 errcode =  parse_line(in, request, 3, method);

 if(errcode == OK)
	sprintf(port, "%s", in);
 else
 {
	return errcode;
 }
 
 if(*method != LISTALL)
 {
	errcode = parse_line(in, request, 4, method);

	if(errcode == OK)
 		sprintf(title, "%s", in);
	else
	{
		return errcode;
 	}
 }

 return OK;
}

void generate_response(char buf[1000], int code, int method)
{
  char codestr[20];
  char time_now[100];
  struct utsname os;
  
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  sprintf(time_now, "%d-%d-%d %d:%d:%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

  uname(&os);

  switch(code)
  { 
  case OK:
	strcpy(codestr,"OK");
	break;

  case BadRequest:
	strcpy(codestr, "Bad Request");
	break;

  case NotFound:
	strcpy(codestr, "Not Found");
	break;

  case VersionNotFound:
	strcpy(codestr, "Version Not Found");
	break;
  }
 
  if(method != ADD)
	sprintf(buf, "%s %d %s\r\nDate: %s\r\nOS: %s\r\nLast Modified: %s\r\nContent-Length: 1000\r\nContent-type: text/text", VERSION, code, codestr, time_now, os.sysname, time_now);
  else
  {
	sprintf(buf, "%s %d %s", VERSION, code, codestr);
  }
}

void append_response(char buf[1000], int rfc, char host[50], char port[50], char title[50])
{
 //str_replace(title, ' ', '_');
	sprintf(buf, "%s\r\n%d %s %s %s", buf, rfc, title, host, port);
}

int parse_line(char out[SIZE], char *buf, int line_no, int *method)
{
 int i=0;

 char *line = strdup(strtok(strdup(buf), CRLF));

 for(i=0; i < line_no - 1; i++)
 {
	line = strdup(strtok(NULL, CRLF));
 }

 switch(line_no)
 {
   case 1:
	return parse_line_1(out, line, method);
   break;

   case 2:
	return parse_line_2(out, line);

   case 3:
	return parse_line_3(out, line);

   case 4:
	return parse_line_4(out, line);

   default:
	return BadRequest;
 }
}

int parse_line_1(char out[SIZE], char *line, int *method)
{
 char *field;
 char rfcnum[10];
 
 // get method
 field = strtok(line, SPACE);

 if(!strcmp(field, "GET"))
	*method = GET;
 else if(!strcmp(field, "ADD"))
	*method = ADD;
 else if(!strcmp(field, "LOOKUP"))
	*method = LOOKUP;
 else if(!strcmp(field, "LIST"))
	*method = LISTALL;
 else
 {
	return BadRequest;
 }

 if(*method == LISTALL)
 {
	field = strtok(NULL, SPACE);

	if(strcmp(field, "ALL"))
	{
		return BadRequest;
	}

	field = strtok(NULL, SPACE);

	if(strcmp(field, VERSION))
	{
		return VersionNotFound;
	}

	strcpy(out, "*");
 }
 else
 {
	field = strtok(NULL, SPACE);

	if(strcmp(field, "RFC"))
	{
		return BadRequest;
	}

	field = strtok(NULL, SPACE);

	strcpy(out, field);

	field = strtok(NULL, SPACE);

	if(strcmp(field, VERSION))
	{
		return VersionNotFound;
	}

 }
 
 return OK;
}

int parse_line_2(char out[SIZE], char *line)
{
  char *field;
 
  field = strtok(line, SPACE);

  if(strcmp(field, "Host:"))
  {
	return BadRequest;
  }

  field = strtok(NULL, SPACE);

  strcpy(out, field);

  return OK;
}

int parse_line_3(char out[SIZE], char *line)
{
  char *field;

  field = strtok(line, SPACE);

  if(strcmp(field, "Port:"))
  {
	return BadRequest;
  }

  field = strtok(NULL, SPACE);

  strcpy(out, field);

  return OK;
}

int parse_line_4(char out[SIZE], char *line)
{
  char *field;

  field = strtok(line, SPACE);

  if(strcmp(field, "Title:"))
  {
	return BadRequest;
  }

  field = strtok(NULL, SPACE);

  // <sp> in title is replaced while sending

  //str_replace(out, '_', ' ');

  strcpy(out, field);

  return OK;
}


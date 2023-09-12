#include "../headers.h"

const char *url = "man.he.net";
const char *port = "80";

bool inside_tag = false;
void print_clean(char *s)
{
  for (int i = 0; s[i] != '\0'; ++i)
  {
    if (s[i] == '<')
    {
      inside_tag = true;
    }
    else if (s[i] == '>')
    {
      inside_tag = false;
      continue;
    }

    if (!inside_tag)
      printf("%c", s[i]);
  }
}

int iman(command c)
{
  if (c.argc != 2)
  {
    ERROR_PRINT("iman expected 1 argument, found %i\n", c.argc - 1);
    return FAILURE;
  }

  char path[256];
  strcpy(path, "/?topic=");
  strcat(path, c.argv[1]);
  strcat(path, "&section=all");
  DEBUG_PRINT("path is %s\n", path);

  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;     // Use IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM; // Use TCP

  struct addrinfo *result;
  int status = getaddrinfo(url, port, &hints, &result);
  if (status != 0)
  {
    DEBUG_PRINT("getaddrinfo: %s\n", gai_strerror(status));
    ERROR_PRINT("Failed to resolve host name!\n");
    return FAILURE;
  }

  int fd = -1;
  for (struct addrinfo *r = result; r != NULL; r = r->ai_next)
  {
    int cur = socket(r->ai_family, r->ai_socktype, r->ai_protocol);
    if (cur == -1)
      continue;

    if (connect(cur, r->ai_addr, r->ai_addrlen) != -1)
    {
      fd = cur;
      break;
    }
    close(fd);
  }
  freeaddrinfo(result);
  if (fd == -1)
  {
    ERROR_PRINT("Failed to connect to man.he.net\n");
    return FAILURE;
  }

  char request[MAX_STR_LEN];

  snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, url);
  if (send(fd, request, strlen(request), 0) == -1)
  {
    DEBUG_PRINT("send failed with errno %i (%s)\n", errno, strerror(errno));
    ERROR_PRINT("Failed to send request\n");
    close(fd);
    return FAILURE;
  }

  char response[MAX_STR_LEN];
  ssize_t bytes_received;
  bool first = true;
  inside_tag = false;
  while ((bytes_received = recv(fd, response, sizeof(response) - 1, 0)) > 0)
  {
    response[bytes_received] = '\0';
    if (first) // Skip to NAME where actual description begins (Ignore starter HTML)
    {
      first = false;
      char *cur = strstr(response, "NAME");
      cur = strstr(cur + 1, "NAME");
      if (cur == NULL)
      {
        ERROR_PRINT();
        printf("\n\tNo such command\n");
        return FAILURE;
      }
      print_clean(cur);
    }
    else
      print_clean(response);
  }

  close(fd);
  return SUCCESS;
}

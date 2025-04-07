#pragma once

/******************************************/
/* NOTE: Server/location/configbase inits */
/******************************************/

/* Max. number of connections to a server simultaneously */
# define SERVERS_BACKLOG	10

/* Default max size of a HTTP request -> 100 bytes/characters */
# define CLIENT_MAX_BODY_SIZE_DEFAULT	100

/* Default alias -> None */
# define ALIAS_DEFAULT					""

/* Default folder where the pages will be searched */
# define ROOT_DEFAULT					"."

/* Default autoindex (the folders won't saw their files) */
# define AUTOINDEX_DEFAULT				false

/* Default server IP */
# define IP_DEFAULT						"127.0.0.1"

/* Limit for client_max_body_size - 50mb */
# define MAX_BODY_SIZE_BYTES			52428800

/* Server protocol */
# define SERVER_PROTOCOL				"HTML/1.1"

/***************************/
/* NOTE: HTTPResponse data */
/***************************/

/* Error codes path */
# define ERRORS_PATH "src/HTTPRequestResponse/codes.csv"

/* Extensions path */
# define EXTENSIONS_PATH "src/HTTPRequestResponse/extensions.csv"

/******************/
/* NOTE: Timeouts */
/******************/

/* Poll timeout (miliseconds)*/
# define POLL_TIMEOUT 1000 // 1 second

/* CGI timeout (seconds) */
# define CGI_TIMEOUT 5

/***************/
/* NOTE: Debug */
/***************/

#ifndef DEBUG
# define DEBUG false
#endif

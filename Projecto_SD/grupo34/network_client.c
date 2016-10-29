/* Sistemas Distribuidos - 2016 - Grupo 34
Elias Miguel Barreira 40821, Pedro Pais 41375
Silvia Ferreira 45511 */

#include "network_client-private.h"
#include <stdlib.h>


//int write_all(int sock, char *buf, int len){
	//TODO nao compila!
	// int bufsize = len;
	// while (len > 0){
	// 	int res = write(sock, buf, len);
	// 	if (res < 0){
	// 		if(errno == EINTR) continue;
	// 		perror("Write failed!");
	// 		return res;
	// 	}
	// 	buf += res;
	// 	len -= res;
	// }
	// return bufsize;
//}

//A DUVIDA � SE NOS TEMOS QUE PREOCUPAR COM O '/0'. Isso acrescenta-se no buffer? ???
//Caso sim, acrescenta-se write(sock, '/0', len); e no buf += res + 1; len -= res + 1;
//int read_all(int sock, char *buf, int len){
	//TODO nao compila!
	// int bufsize = len;
	// while(len > 0){
	// 	int res = read(sock, buf, len);
	// 	if (res < 0) {
	// 		if errno == EINTR) continue;
	// 		perror("Read failed!");
	// 		return res;
	// 	}
	// 	buf += res;
	// 	len -= res;
	// }
	// return bufsize;
//}


struct server_t *network_connect(const char *address_port){
	struct server_t *server = malloc(sizeof(struct server_t));
  struct sockaddr_in server_in;
  struct hostent *hostinfo;
  char str_aux[1000];
  char *server_name;
  char *server_port;
  int server_port_num = -1;
  int server_socket_fd = -1;

	/* Verificar parâmetro da função e alocação de memória */
  if (server == NULL || address_port == NULL) {
    network_close(server);
    return NULL;
  }

  strcpy(str_aux, address_port);
  server_name = strtok(str_aux, ":");
  if (server_name == NULL) {
    network_close(server);
    return NULL;
  }

  printf("host: %s\n", server_name);
  server_port = strtok(NULL, ":");
  if (server_port == NULL) {
    network_close(server);
    return NULL;
  }
  server_port_num = atoi(server_port);
  printf("port: %d\n", server_port_num);


	/* Estabelecer ligação ao servidor:

		Preencher estrutura struct sockaddr_in com dados do
		endereço do servidor.

		Criar a socket.

		Estabelecer ligação.
	*/

  //socket
  server_socket_fd = socket(PF_INET, SOCK_STREAM,0);
  if(server_socket_fd < 0) {
    network_close(server);
    return NULL;
  }
  server->socket_fd = server_socket_fd;

  server_in.sin_family = AF_INET;
  server_in.sin_port = htons(server_port_num);
  hostinfo = gethostbyname (server_name);
  if (hostinfo == NULL) {
    network_close(server);
    return NULL;
  }
  server_in.sin_addr = *(struct in_addr *) hostinfo->h_addr;


	/* Se a ligação não foi estabelecida, retornar NULL */
  if(connect (server_socket_fd, (struct sockaddr *)&server_in, sizeof(server_in)) < 0) {
    network_close(server);
    return NULL;
  }

  //TODO guardar mais info na estrutura server?

	return server;
}

struct message_t *network_send_receive(struct server_t *server, struct message_t *msg){
	char *message_out;
	int message_size, msg_size, result;
	struct message_t *msg_resposta;

	/* Verificar parâmetros de entrada */
	if (server == NULL || msg == NULL) return NULL;

	/* Serializar a mensagem recebida */
	message_size = message_to_buffer(msg, &message_out);

	/* Verificar se a serialização teve sucesso */

	/* Enviar ao servidor o tamanho da mensagem que será enviada
	   logo de seguida
	*/
	msg_size = htonl(message_size);
 	result = write_all(server->socket_fd, (char *) &msg_size, _INT);

	/* Verificar se o envio teve sucesso */

	/* Enviar a mensagem que foi previamente serializada */

	result = write_all(server->socket_fd, message_out, message_size);

	/* Verificar se o envio teve sucesso */

	/* De seguida vamos receber a resposta do servidor:

		Com a função read_all, receber num inteiro o tamanho da
		mensagem de resposta.

		Alocar memória para receber o número de bytes da
		mensagem de resposta.

		Com a função read_all, receber a mensagem de resposta.

	*/

	/* Desserializar a mensagem de resposta */
	//TODO
	//msg_resposta = buffer_to_message( /* */, /* */ );

	/* Verificar se a desserialização teve sucesso */

	/* Libertar memória */

	return msg_resposta;
}

int network_close(struct server_t *server){
	/* Verificar parâmetros de entrada */
  if (server == NULL) return -1;

	/* Terminar ligação ao servidor */
  close(server->socket_fd);

	/* Libertar memória */
  free(server);
  //TODO ver se eh preciso libertar memória
  // dentro da struct 'server'

  return 0;

}

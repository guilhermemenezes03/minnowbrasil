/* 
  esse header define uma abstração de socket TCP em C++ moderno
  escondendo detalhes mais crus do sistema operacional e forçando
  um jeito mais seguro de criar conexões
*/

#pragma once // avisa que esse arquivo deve ser incluído só uma vez durante a compilação

#include "util/address.hh" // inclui adress.hh, que é necessário para a classe Socket.
#include "util/file_descriptor.hh" // inclui o file descriptor que é necessário para a classe socket

namespace minnow { // namespace de onde as classes socket e tcpsocket estao definidas

class Socket : public FileDescriptor { // socket é uma subclasse com herança public do filedescriptor
public:
  using FileDescriptor::FileDescriptor; // herda os construtores da classe base FileDescriptor para a classe Socket

  void connect( const Address& address ); // void nao retorna nada, connect é um metodo da classe socket que recebe um endereço e conecta a ele
};

class TCPSocket : public Socket { // tcpsocket é uma subclasse com herança public da classe socket
public:
  [[nodiscard]] static TCPSocket connect_to( const Address& address ); 
  /* 
  nodiscard indica que o valor retornado por connect_to que é um socket nao deve ser ignorado, se voce chamar connect e nao usar o valor retornado, o compilador pode emitir um aviso. Isso é útil para garantir que os programadores não esqueçam de lidar com o resultado da conexão.
  static indica que posso chamar direto pela classe sem precisar criar um objeto antes, ou seja, posso chamar minnow::TCPSocket::connect_to() sem precisar criar um objeto tcpsocket antes.
  tcpsocket tipo de retorno da função connect_to, quando chamo connect_to, ele retorna um objeto do tipo tcpsocket já conectado
  connect_to nome do método, funcao estatica que cria e conecta ao mesmo tempo
  const adress& address é uma constante, ou seja, é inalteravel & é uma referencia ( nao faz copia do objeto ), 
  */

private: 
  explicit TCPSocket( int fd ) : Socket( fd ) {}
};
/*
  private é usado para indicar que apenas o construtor TCPSocket pode ser chamado dentro na classe
  explicit é usado para evitar conversões automaticas, ou seja, o construtor só pode ser chamado explicitamente, não pode ser usado para converter um inteiro em um TCPSocket sem chamar o construtor diretamente. Isso é útil para evitar erros acidentais de conversão.
 tcpsocket é um construtor privado usado para criar um objeto tcpsocket a partir de um numero inteiro(int fd)
 socket(fd) é o número interno que o sistema operacional usa para identificar o socket aberto
*/

} // namespace minnow


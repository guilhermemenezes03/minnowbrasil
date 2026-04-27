#include "util/socket.hh"

#include <cerrno>
#include <system_error>

#include <sys/socket.h> 

using namespace std; // "atalho" para nao precisar ficar escrevendo std:: antes de cada coisa da biblioteca

namespace minnow { // espaço onde as classes socket e tcpsocket estão definidas

namespace {

system_error make_system_error( const char* what )
{
  return system_error( errno, generic_category(), what );
} // função para identificar o erro criado, errno vem da biblioteca cerrno, o linux registra o erro em errno e a funcao make system pega o erro, cria um system error e rotula como connect ou ssocket

}

void Socket::connect( const Address& address ) // void nao chama, informa o tipo de retorno. esse metodo da classe socket conecta o socket a constante address
{
  if ( ::connect( fd(), address.sockaddr_ptr(), address.size() ) < 0 ) { // fd fornece o inteiro, connect tenta conectar o socket usando o inteiro, se retornar menor que 0 vai retornar a mensagem de erro connect
    throw make_system_error( "connect" ); // mensagem de erro que retorna
  }
} 

TCPSocket TCPSocket::connect_to( const Address& address ) // classe tcpsocket usa o connect_to para criar e conectar na constante adress
{
  const auto socket_fd = ::socket( address.family(), address.socktype(), address.protocol() ); // const auto é usado para nao precisar declarar se é um int, string, cria um socket com propriedades de endereço family socktype e protocolo

  if ( socket_fd < 0 ) { // se o socket criado for menor que 0 o sytem retorna uma mensagem tratada falando que o erro é o socket
    throw make_system_error( "socket" ); // mensagem de erro
  }

  TCPSocket socket( socket_fd ); // cria um objeto TCPSocket a partir do socket_fd como argumento
  socket.connect( address ); // conecta o objeto socketpassando o adress como argumento para conectar
  return socket; // retorna o socket que foi criado usando o socket_fd como argumento e conectado usando o connect passando o adress como argumento 
}

} // namespace minnow


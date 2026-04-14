#include "util/address.hh"
#include "util/socket.hh"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

namespace {

string normalize_path( const string& path )
{
  if ( path.empty() ) {
    return "/";
  }

  if ( path.front() == '/' ) {
    return path;
  }

  return "/" + path;
}

} // namespace

string get_URL( const string& host, const string& path, const string& service = "80" )
{
  const string normalized_path = normalize_path( path );

  // TODO: implemente um cliente HTTP/1.1 minimo.
  //
  // O objetivo e:
  // 1. Resolver o endereco com Address(host, service).
  // 2. Abrir a conexao usando TCPSocket::connect_to(...).
  // 3. Enviar uma requisicao HTTP parecida com:
  //
  //    GET /algum/caminho HTTP/1.1\r\n
  //    Host: exemplo.com\r\n
  //    Connection: close\r\n
  //    \r\n
  //
  // 4. Ler do socket ate EOF.
  // 5. Retornar a resposta completa como string.
  //
  // Dicas:
  // - Use \r\n em todas as linhas do HTTP.
  // - Uma unica leitura nao basta.
  // - "Connection: close" e o jeito mais simples de saber quando o servidor terminou.
  (void)host;
  (void)normalized_path;
  (void)service;

  throw runtime_error( "TODO: implemente get_URL() em apps/webget.cc" );
}

int main( int argc, char* argv[] )
{
  if ( argc != 3 && argc != 4 ) {
    cerr << "Usage: " << argv[0] << " HOST PATH [PORT]\n";
    return 1;
  }

  const string host = argv[1];
  const string path = argv[2];
  const string service = argc == 4 ? argv[3] : "80";

  try {
    cout << get_URL( host, path, service );
  } catch ( const exception& e ) {
    cerr << "webget: " << e.what() << '\n';
    return 1;
  }

  return 0;
}


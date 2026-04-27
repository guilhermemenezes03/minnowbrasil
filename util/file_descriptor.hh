#pragma once // avisa que esse arquivo só é incluido uma vez 

#include <cstddef> // traz tipos como std::size_t
#include <string> // biblioteca para usar string
#include <string_view> //traz std::string_view, que é uma visão leve de texto sem copiar os dados 

namespace minnow { // espaço onde as classes socket e tcpsocket estão definidas

class FileDescriptor { // cria uma classe Filedescriptor publica
public:
  explicit FileDescriptor( int fd ); 
/*  explicit usado para evitar conversoes automaticas, ou seja, nao pode ser usado um inteiro para converter em um filedescriptor sem chamar o construtor diretamente, isso é util para evitar erros acidentais de conversao. FileDescriptor é um construtor que recebe um inteiro como argumento, esse inteiro é o file descriptor que o sistema operacional usa para identificar arquivos abertos ou sockets.
    construtor da classe que recebe um inteiro fd*/
  FileDescriptor( const FileDescriptor& ) = delete;  // apaga o construtor de cópia (evitar plagio)
  FileDescriptor& operator=( const FileDescriptor& ) = delete; // apaga a atribuição por cópia ( evitar plagio )

  FileDescriptor( FileDescriptor&& other ) noexcept; // voce nao pode copiar o dono do recurso
  FileDescriptor& operator=( FileDescriptor&& other ) noexcept; // mas pode transferir a posse para outro objeto
/* objetoA tem o fd, objetoA é movido para objetoB, objetoA nao possui mais o fd*/
  ~FileDescriptor(); // destrutor, quando o objeto for destruido, ele vai fechar o file descriptor se ele for valido

  [[nodiscard]] int fd() const; // nodiscard indica que o valor retornado por fd nao pode ser ignorado se for chamado
  [[nodiscard]] bool valid() const; // nodiscard indica que o valor retornado por valid nao pode ser ignorado se for chamado, valid é um metodo do tipo booleano, otimo para indicar se o file descriptor é válido ou inválido

  void close(); // metodo para fechar o file descriptor, se o file descriptor for valido
  void write_all( std::string_view data ) const; // metodo para escrever os dados de uma string_view 
  [[nodiscard]] std::string read_some( std::size_t max_bytes = 4096 ) const; // le os bytes com maximo de 4096, devolve os bytes como std::string read_some mostra que le apenas um pedaço

protected:
  FileDescriptor() = default; // existe um construtor vazio por padrao porem ele é protegido, ou seja, so pode ser chamado por subclasses ou para propria classe

private:
  int fd_ { -1 }; // começa com -1 que é tratado como inválido, ou seja, está fechado fd_ representa o recurso no sistema
};

} // namespace minnow


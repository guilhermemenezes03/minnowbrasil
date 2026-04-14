#include "src/byte_stream.hh"

#include <algorithm>
#include <stdexcept>

using namespace std;

namespace minnow {

ByteStream::ByteStream( const uint64_t capacity ) : capacity_( capacity ) {}

void ByteStream::compact()
{
  if ( head_ == 0 ) {
    return;
  }

  storage_.erase( 0, head_ );
  head_ = 0;
}

void ByteStream::push( string data )
{
  if ( closed_ && !data.empty() ) {
    throw logic_error( "cannot push into a closed ByteStream" );
  }

  // TODO: implemente o comportamento correto do fluxo.
  //
  // Dicas:
  // 1. So aceite ate available_capacity() bytes.
  // 2. Atualize bytes_pushed_ com o numero de bytes realmente aceitos.
  // 3. Se head_ > 0, compacte storage_ antes de anexar novos dados.
  // 4. A capacidade limita os bytes armazenados agora, nao o total historico.
  (void)data;
}

void ByteStream::close()
{
  closed_ = true;
}

bool ByteStream::is_closed() const
{
  return closed_;
}

uint64_t ByteStream::available_capacity() const
{
  return capacity_ - bytes_buffered();
}

uint64_t ByteStream::bytes_pushed() const
{
  return bytes_pushed_;
}

string_view ByteStream::peek() const
{
  // TODO: devolva uma view dos bytes atualmente disponiveis para leitura.
  return {};
}

void ByteStream::pop( const uint64_t len )
{
  // TODO: remova ate len bytes do inicio do fluxo.
  //
  // Dicas:
  // 1. Nunca avance mais do que bytes_buffered().
  // 2. Atualize bytes_popped_.
  // 3. Quando todo o buffer tiver sido consumido, limpe storage_ e zere head_.
  // 4. Se quiser, compacte quando head_ ficar grande em relacao ao tamanho total.
  (void)len;
}

bool ByteStream::is_finished() const
{
  return closed_ && bytes_buffered() == 0;
}

bool ByteStream::has_error() const
{
  return error_;
}

uint64_t ByteStream::bytes_buffered() const
{
  return static_cast<uint64_t>( storage_.size() - head_ );
}

uint64_t ByteStream::bytes_popped() const
{
  return bytes_popped_;
}

} // namespace minnow

